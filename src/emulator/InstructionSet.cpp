/*
 * InstructionSet.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#include "InstructionSet.h"

InstructionSet::InstructionSet(const Chip8VM& vm)
{
    _vm = vm;
}

InstructionSet::~InstructionSet()
{

}

void InstructionSet::clr()
{
    _vm.clearDisplay();
}

void InstructionSet::ret()
{
    _vm.stackPop();
}

void InstructionSet::rcacall(uint16_t address)
{
}

void InstructionSet::jmp(uint16_t address)
{
    _vm.setPC(address);

}

void InstructionSet::call(uint16_t address)
{
    _vm.stackPush();
    _vm.setPC(address);
}

void InstructionSet::skpeq(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::skpne(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::add(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::mov(uint8_t home, uint8_t val)
{
}

void InstructionSet::_or(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::_and(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::_xor(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::sub(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::rsub(uint8_t val1, uint8_t val2)
{
}

void InstructionSet::shr(uint8_t val)
{
}

void InstructionSet::shl(uint8_t val)
{
}

void InstructionSet::movi(uint16_t address)
{
}

void InstructionSet::jmpv0(uint16_t address)
{
}

void InstructionSet::rnd(uint8_t reg, uint8_t address)
{
}

void InstructionSet::sprite(uint8_t regx, uint8_t regy, uint8_t height)
{
}

void InstructionSet::skpdn(uint8_t key)
{
}

void InstructionSet::skpup(uint8_t key)
{
}

void InstructionSet::getdtmr(uint8_t reg)
{
}

void InstructionSet::getkey(uint8_t reg)
{
}

void InstructionSet::setdtmr(uint8_t reg)
{
}

void InstructionSet::setstmr(uint8_t reg)
{
}

void InstructionSet::addi(uint8_t reg)
{
}

void InstructionSet::hxchr(uint8_t reg)
{
}

void InstructionSet::bcd(uint8_t reg)
{
}

void InstructionSet::str(uint8_t reg)
{
}

void InstructionSet::load(uint8_t reg)
{
}
