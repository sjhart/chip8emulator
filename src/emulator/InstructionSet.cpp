/*
 * InstructionSet.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#include "InstructionSet.h"

InstructionSet::InstructionSet(Chip8VM& vm)
{
    _vm = &vm;
}

InstructionSet::~InstructionSet()
{
    _vm = 0;

}

void InstructionSet::clr()
{
    _vm->clearDisplay();
    _vm->incPC();
}

void InstructionSet::ret()
{
    _vm->stackPop();
}

void InstructionSet::rcacall(uint16_t address)
{
}

void InstructionSet::jmp(uint16_t address)
{
    _vm->setPC(address);
}

void InstructionSet::call(uint16_t address)
{
    _vm->stackPush();
    _vm->setPC(address);
}

void InstructionSet::skpeq(uint8_t val1, uint8_t val2)
{
    if (val1 == val2)
        _vm->incPC();
    _vm->incPC();

}

void InstructionSet::skpne(uint8_t val1, uint8_t val2)
{
    if (val1 != val2)
        _vm->incPC();
    _vm->incPC();
}

void InstructionSet::add(uint8_t reg, uint8_t val, bool carryFlag)
{
    if (carryFlag)
    {
        if ((_vm->getVRegister(reg)) > (0xFF - val)) // carry
            _vm->setVRegister(0xF, 1);
        else
            _vm->setVRegister(0xF, 0);
    }
    _vm->setVRegister(reg, _vm->getVRegister(reg) + val);
    _vm->incPC();
}

void InstructionSet::mov(uint8_t reg, uint8_t val)
{
    _vm->setVRegister(reg, val);
    _vm->incPC();
}

void InstructionSet::_or(uint8_t reg1, uint8_t reg2)
{
    _vm->setVRegister(reg1, _vm->getVRegister(reg1) | reg2);
    _vm->incPC();
}

void InstructionSet::_and(uint8_t reg1, uint8_t reg2)
{
    _vm->setVRegister(reg1, _vm->getVRegister(reg1) & reg2);
    _vm->incPC();
}

void InstructionSet::_xor(uint8_t reg1, uint8_t reg2)
{
    _vm->setVRegister(reg1, _vm->getVRegister(reg1) ^ reg2);
    _vm->incPC();
}

void InstructionSet::sub(uint8_t reg, uint8_t val)
{
    if (_vm->getVRegister(reg) < val) // borrow
        _vm->setVRegister(0xF, 1);
    else
        _vm->setVRegister(0xF, 0);
    _vm->setVRegister(reg, _vm->getVRegister(reg) - val);
    _vm->incPC();
}

void InstructionSet::rsub(uint8_t reg, uint8_t val)
{
    if (_vm->getVRegister(reg) > val) // borrow
        _vm->setVRegister(0xF, 1);
    else
        _vm->setVRegister(0xF, 0);
    _vm->setVRegister(reg, val - _vm->getVRegister(reg));
    _vm->incPC();
}

void InstructionSet::shr(uint8_t reg)
{
    _vm->setVRegister(0xF, _vm->getVRegister(reg) & 0x01);
    _vm->setVRegister(reg, _vm->getVRegister(reg) >> 1);
    _vm->incPC();
}

void InstructionSet::shl(uint8_t reg)
{
    _vm->setVRegister(0xF, _vm->getVRegister(reg) & 0xFE);
    _vm->setVRegister(reg, _vm->getVRegister(reg) << 1);
    _vm->incPC();
}

void InstructionSet::movi(uint16_t address)
{
    _vm->setIRegister(address);
    _vm->incPC();
}

void InstructionSet::jmpv0(uint16_t address)
{
    jmp(address + _vm->getVRegister(0));
}

void InstructionSet::rnd(uint8_t reg, uint8_t address)
{

}

void InstructionSet::sprite(uint8_t regx, uint8_t regy, uint8_t height)
{
}

void InstructionSet::skpdn(uint8_t key)
{
    if (_vm->isKeyPressed(key))
        _vm->incPC();
    _vm->incPC();
}

void InstructionSet::skpup(uint8_t key)
{
    if (!_vm->isKeyPressed(key))
        _vm->incPC();
    _vm->incPC();
}

void InstructionSet::getdtmr(uint8_t reg)
{
    _vm->setVRegister(reg, _vm->getDelayTimer());
    _vm->incPC();
}

void InstructionSet::getkey(uint8_t reg)
{
    uint8_t i = 0;
    bool pressed = false;
    while (!pressed)
    {
        if (i == 16)
            i = 0;
        pressed = _vm->isKeyPressed(i);
        if (pressed)
            break;
        i++;
    }
    _vm->setVRegister(reg, i);
    _vm->incPC();
}

void InstructionSet::setdtmr(uint8_t reg)
{
    _vm->setDelayTimer(_vm->getVRegister(reg));
    _vm->incPC();
}

void InstructionSet::setstmr(uint8_t reg)
{
    _vm->setSoundTimer(_vm->getVRegister(reg));
    _vm->incPC();
}

void InstructionSet::addi(uint8_t reg)
{
    _vm->setIRegister(_vm->getIRegister() + _vm->getVRegister(reg));
    _vm->incPC();
}

void InstructionSet::hxchr(uint8_t reg)
{
}

void InstructionSet::bcd(uint8_t reg)
{
    uint16_t Iaddr = _vm->getIRegister();
    _vm->setMemory(Iaddr++, _vm->getVRegister(reg) / 100); // hundreds
    _vm->setMemory(Iaddr++, (_vm->getVRegister(reg) / 10) % 10); // tens
    _vm->setMemory(Iaddr, _vm->getVRegister(reg) % 100); // ones
    _vm->incPC();
}

void InstructionSet::str(uint8_t reg)
{
    uint16_t Iaddr = _vm->getIRegister();
    for (uint8_t i = 0; i <= reg; i++)
    {
        _vm->setMemory(Iaddr++, _vm->getVRegister(i));
    }
    _vm->incPC();
}

void InstructionSet::load(uint8_t reg)
{
    uint16_t Iaddr = _vm->getIRegister();
    for (uint8_t i = 0; i <= reg; i++)
    {
        _vm->setVRegister(i, _vm->getMemory(Iaddr++));
    }
    _vm->incPC();
}
