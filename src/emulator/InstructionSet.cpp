/*
 * InstructionSet.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#include "InstructionSet.h"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

InstructionSet::InstructionSet(Chip8VM& vm)
{
    vm_ptr = &vm;
}

InstructionSet::~InstructionSet()
{
    vm_ptr = 0;
}

void InstructionSet::clr()
{
    vm_ptr->clearDisplay();
    vm_ptr->incPC();
}

void InstructionSet::ret()
{
    vm_ptr->stackPop();
}

void InstructionSet::rcacall(uint16_t address)
{
    //TODO: implement
}

void InstructionSet::jmp(uint16_t address)
{
    vm_ptr->setPC(address);
}

void InstructionSet::call(uint16_t address)
{
    vm_ptr->stackPush();
    vm_ptr->setPC(address);
}

void InstructionSet::skpeq(uint8_t val1, uint8_t val2)
{
    if (val1 == val2)
        vm_ptr->incPC();
    vm_ptr->incPC();
}

void InstructionSet::skpne(uint8_t val1, uint8_t val2)
{
    if (val1 != val2)
        vm_ptr->incPC();
    vm_ptr->incPC();
}

void InstructionSet::add(uint8_t reg, uint8_t val, bool carryFlag)
{
    if (carryFlag)
    {
        if ((vm_ptr->getVRegister(reg)) > (0xFF - val)) // carry
            vm_ptr->setVRegister(0xF, 1);
        else
            vm_ptr->setVRegister(0xF, 0);
    }
    vm_ptr->setVRegister(reg, vm_ptr->getVRegister(reg) + val);
    vm_ptr->incPC();
}

void InstructionSet::mov(uint8_t reg, uint8_t val)
{
    vm_ptr->setVRegister(reg, val);
    vm_ptr->incPC();
}

void InstructionSet::_or(uint8_t reg1, uint8_t reg2)
{
    vm_ptr->setVRegister(reg1, vm_ptr->getVRegister(reg1) | reg2);
    vm_ptr->incPC();
}

void InstructionSet::_and(uint8_t reg1, uint8_t reg2)
{
    vm_ptr->setVRegister(reg1, vm_ptr->getVRegister(reg1) & reg2);
    vm_ptr->incPC();
}

void InstructionSet::_xor(uint8_t reg1, uint8_t reg2)
{
    vm_ptr->setVRegister(reg1, vm_ptr->getVRegister(reg1) ^ reg2);
    vm_ptr->incPC();
}

void InstructionSet::sub(uint8_t reg, uint8_t val)
{
    if (vm_ptr->getVRegister(reg) < val) // borrow
        vm_ptr->setVRegister(0xF, 1);
    else
        vm_ptr->setVRegister(0xF, 0);
    vm_ptr->setVRegister(reg, vm_ptr->getVRegister(reg) - val);
    vm_ptr->incPC();
}

void InstructionSet::rsub(uint8_t reg, uint8_t val)
{
    if (vm_ptr->getVRegister(reg) > val) // borrow
        vm_ptr->setVRegister(0xF, 1);
    else
        vm_ptr->setVRegister(0xF, 0);
    vm_ptr->setVRegister(reg, val - vm_ptr->getVRegister(reg));
    vm_ptr->incPC();
}

void InstructionSet::shr(uint8_t reg)
{
    vm_ptr->setVRegister(0xF, vm_ptr->getVRegister(reg) & 0x01);
    vm_ptr->setVRegister(reg, vm_ptr->getVRegister(reg) >> 1);
    vm_ptr->incPC();
}

void InstructionSet::shl(uint8_t reg)
{
    vm_ptr->setVRegister(0xF, vm_ptr->getVRegister(reg) & 0x80);
    vm_ptr->setVRegister(reg, vm_ptr->getVRegister(reg) << 1);
    vm_ptr->incPC();
}

void InstructionSet::movi(uint16_t address)
{
    vm_ptr->setIRegister(address);
    vm_ptr->incPC();
}

void InstructionSet::jmpv0(uint16_t address)
{
    jmp(address + vm_ptr->getVRegister(0));
}

void InstructionSet::rnd(uint8_t reg, uint8_t mask)
{
    srand(time(0));
    uint8_t rndNum = rand() % 255 & mask;
    mov(reg, rndNum);
    // increment of PC counter handled by mov
}

void InstructionSet::sprite(uint8_t regx, uint8_t regy, uint8_t height)
{
    uint8_t pxl;

    vm_ptr->setVRegister(0xF, 0);
    for (int row = 0; row < height; row++)
    {
        pxl = vm_ptr->getMemory(vm_ptr->getIRegister() + row);
        for (int column = 0; column < 8; column++)
        {
            if ((pxl & (0x80 >> column)) != 0) // iterate through each bit in pxl
            {
                uint8_t curPxl = vm_ptr->getDisplay(
                        regx + column + ((regy + row) * 64));
                if (curPxl == 1)
                    vm_ptr->setVRegister(0xF, 1); // mark collision
                vm_ptr->setDisplay(curPxl, curPxl ^ 1);
            }
        }
    }
    vm_ptr->incPC();
}

void InstructionSet::skpdn(uint8_t key)
{
    if (vm_ptr->isKeyPressed(key))
        vm_ptr->incPC();
    vm_ptr->incPC();
}

void InstructionSet::skpup(uint8_t key)
{
    if (!vm_ptr->isKeyPressed(key))
        vm_ptr->incPC();
    vm_ptr->incPC();
}

void InstructionSet::getdtmr(uint8_t reg)
{
    vm_ptr->setVRegister(reg, vm_ptr->getDelayTimer());
    vm_ptr->incPC();
}

void InstructionSet::getkey(uint8_t reg)
{
    uint8_t i = 0;
    bool pressed = false;
    while (!pressed)
    {
        if (i == 16)
            i = 0;
        pressed = vm_ptr->isKeyPressed(i);
        if (pressed)
            break;
        i++;
    }
    vm_ptr->setVRegister(reg, i);
    vm_ptr->incPC();
}

void InstructionSet::setdtmr(uint8_t reg)
{
    vm_ptr->setDelayTimer(vm_ptr->getVRegister(reg));
    vm_ptr->incPC();
}

void InstructionSet::setstmr(uint8_t reg)
{
    vm_ptr->setSoundTimer(vm_ptr->getVRegister(reg));
    vm_ptr->incPC();
}

void InstructionSet::addi(uint8_t reg)
{
    vm_ptr->setIRegister(vm_ptr->getIRegister() + vm_ptr->getVRegister(reg));
    vm_ptr->incPC();
}

void InstructionSet::hxchr(uint8_t reg)
{
    vm_ptr->setIRegister(vm_ptr->FONTSET_MEMORY_START + reg);
    vm_ptr->incPC();
}

void InstructionSet::bcd(uint8_t reg)
{
    uint16_t Iaddr = vm_ptr->getIRegister();
    vm_ptr->setMemory(Iaddr++, vm_ptr->getVRegister(reg) / 100); // hundreds
    vm_ptr->setMemory(Iaddr++, (vm_ptr->getVRegister(reg) / 10) % 10); // tens
    vm_ptr->setMemory(Iaddr, vm_ptr->getVRegister(reg) % 100); // ones
    vm_ptr->incPC();
}

void InstructionSet::str(uint8_t reg)
{
    uint16_t Iaddr = vm_ptr->getIRegister();
    for (uint8_t i = 0; i <= reg; i++)
    {
        vm_ptr->setMemory(Iaddr++, vm_ptr->getVRegister(i));
    }
    vm_ptr->incPC();
}

void InstructionSet::load(uint8_t reg)
{
    uint16_t Iaddr = vm_ptr->getIRegister();
    for (uint8_t i = 0; i <= reg; i++)
    {
        vm_ptr->setVRegister(i, vm_ptr->getMemory(Iaddr++));
    }
    vm_ptr->incPC();
}
