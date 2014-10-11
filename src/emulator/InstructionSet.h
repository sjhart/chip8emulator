/*
 * InstructionSet.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#ifndef INSTRUCTIONSET_H_
#define INSTRUCTIONSET_H_

#include "Chip8VM.h"

class InstructionSet
{
public:
    InstructionSet(Chip8VM& vm);
    virtual ~InstructionSet();

    // CHIP8 instructions
    void clr();
    void ret();
    void rcacall(uint16_t address);
    void jmp(uint16_t address);
    void call(uint16_t address);
    void skpeq(uint8_t val1, uint8_t val2);
    void skpne(uint8_t val1, uint8_t val2);
    void add(uint8_t val1, uint8_t val2, bool carryFlag = false);
    void mov(uint8_t home, uint8_t val);
    void _or(uint8_t val1, uint8_t val2);
    void _and(uint8_t val1, uint8_t val2);
    void _xor(uint8_t val1, uint8_t val2);
    void sub(uint8_t val1, uint8_t val2);
    void rsub(uint8_t val1, uint8_t val2);
    void shr(uint8_t val);
    void shl(uint8_t val);
    void movi(uint16_t address);
    void jmpv0(uint16_t address);
    void rnd(uint8_t reg, uint8_t address);
    void sprite(uint8_t regx, uint8_t regy, uint8_t height);
    void skpdn(uint8_t key);
    void skpup(uint8_t key);
    void getdtmr(uint8_t reg);
    void getkey(uint8_t reg);
    void setdtmr(uint8_t reg);
    void setstmr(uint8_t reg);
    void addi(uint8_t reg);
    void hxchr(uint8_t reg);
    void bcd(uint8_t reg);
    void str(uint8_t reg);
    void load(uint8_t reg);

private:
    Chip8VM* _vm;
};

#endif /* INSTRUCTIONSET_H_ */
