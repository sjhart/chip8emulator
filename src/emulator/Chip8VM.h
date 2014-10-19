/*
 * Chip8VM.h
 *
 *  Created on: Oct 6, 2014
 *      Author: Stan
 */

#ifndef CHIP8VM_H_
#define CHIP8VM_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

class Chip8VM
{
public:
    // Constants
    const static uint16_t PROGRAM_MEMORY_START = 0x200;
    const static uint16_t FONTSET_MEMORY_START = 0x0;
    const static uint8_t FONTSET_SIZE = 80;
    const static uint8_t FONTSET[];

    // Program Vars
    uint8_t Program_Memory_Size;

    Chip8VM();
    virtual ~Chip8VM();
    void processChip8Ops();

    // Utility Methods
    void loadMemory(char *file);
    void clearDisplay();
    void decDelayTimer();
    void decSoundTimer();
    uint8_t getSoundTimer();
    void setKey(uint8_t index);
    void clearKey(uint8_t index);
    bool redraw();
    void clearRedraw();
    uint8_t* getDisplay();

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
    // Member variables
    uint8_t _v_register[16];
    uint16_t _i_register;
    uint16_t _sp;
    uint16_t _pc;
    uint16_t _stack[16];
    uint8_t _delay_timer;
    uint8_t _sound_timer;
    bool _key[16];
    uint8_t _memory[4096];
    uint8_t _display[2048]; //64 by 32 grid;
    bool _redraw;
};

//INLINES FUNCTIONS
inline uint8_t Chip8VM::getSoundTimer()
{
    return _sound_timer;
}

inline void Chip8VM::clearDisplay()
{
    memset(_display, 0, sizeof(_display));
}

inline void Chip8VM::decDelayTimer()
{
    if (_delay_timer > 0)
        _delay_timer--;
}

inline void Chip8VM::decSoundTimer()
{
    if (_sound_timer > 0)
        _sound_timer--;
}

inline void Chip8VM::setKey(uint8_t index)
{
    _key[index] = true;
}

inline void Chip8VM::clearKey(uint8_t index)
{
    _key[index] = false;
}

inline void Chip8VM::clr()
{
    clearDisplay();
    _pc += 2; // increment program counter
}

inline void Chip8VM::ret()
{
    _pc = _stack[--_sp]; // stackPop
    _pc += 2; // increment program counter
}

inline void Chip8VM::rcacall(uint16_t address)
{
    //TODO: implement
    _pc += 2; // increment program counter
}

inline void Chip8VM::jmp(uint16_t address)
{
    _pc = address;
}

inline void Chip8VM::call(uint16_t address)
{
    _stack[_sp++] = _pc; // stackPush
    _pc = address;
}

inline void Chip8VM::skpeq(uint8_t val1, uint8_t val2)
{
    if (val1 == val2)
        _pc += 2; // increment program counter
    _pc += 2; // increment program counter
}

inline void Chip8VM::skpne(uint8_t val1, uint8_t val2)
{
    if (val1 != val2)
        _pc += 2; // increment program counter
    _pc += 2; // increment program counter
}

inline void Chip8VM::add(uint8_t reg, uint8_t val, bool carryFlag)
{
    if (carryFlag)
    {
        if ((_v_register[reg]) > (0xFF - val)) // carry
            _v_register[0xF] = 1;
        else
            _v_register[0xF] = 0;
    }
    _v_register[reg] = _v_register[reg] + val;
    _pc += 2; // increment program counter
}

inline void Chip8VM::mov(uint8_t reg, uint8_t val)
{
    _v_register[reg] = val;
    _pc += 2; // increment program counter
}

inline void Chip8VM::_or(uint8_t reg1, uint8_t reg2)
{
    _v_register[reg1] = (_v_register[reg1] | reg2);
    _pc += 2; // increment program counter
}

inline void Chip8VM::_and(uint8_t reg1, uint8_t reg2)
{
    _v_register[reg1] = (_v_register[reg1] & reg2);
    _pc += 2; // increment program counter
}

inline void Chip8VM::_xor(uint8_t reg1, uint8_t reg2)
{
    _v_register[reg1] = (_v_register[reg1] ^ reg2);
    _pc += 2; // increment program counter
}

inline void Chip8VM::sub(uint8_t reg, uint8_t val)
{
    if (_v_register[reg] < val) // borrow
        _v_register[0xF] = 1;
    else
        _v_register[0xF] = 0;
    _v_register[reg] = _v_register[reg] - val;
    _pc += 2; // increment program counter
}

inline void Chip8VM::rsub(uint8_t reg, uint8_t val)
{
    if (_v_register[reg] > val) // borrow
        _v_register[0xF] = 1;
    else
        _v_register[0xF] = 0;
    _v_register[reg] = val - _v_register[reg];
    _pc += 2; // increment program counter
}

inline void Chip8VM::shr(uint8_t reg)
{
    _v_register[0xF] = (_v_register[reg] & 0x01);
    _v_register[reg] = (_v_register[reg] >> 1);
    _pc += 2; // increment program counter
}

inline void Chip8VM::shl(uint8_t reg)
{
    _v_register[0xF] = (_v_register[reg] & 0x80);
    _v_register[reg] = (_v_register[reg] << 1);
    _pc += 2; // increment program counter
}

inline void Chip8VM::movi(uint16_t address)
{
    _i_register = address;
    _pc += 2; // increment program counter
}

inline void Chip8VM::jmpv0(uint16_t address)
{
    jmp(address + _v_register[0]);
}

inline void Chip8VM::rnd(uint8_t reg, uint8_t mask)
{
    srand(time(0));
    uint8_t rndNum = rand() % 255 & mask;
    mov(reg, rndNum);
    // increment of PC counter handled by mov
}

inline void Chip8VM::sprite(uint8_t regx, uint8_t regy, uint8_t height)
{
    uint8_t pxl;
    _v_register[0xF] = 0;
    for (int row = 0; row < height; row++)
    {
        pxl = _memory[_i_register + row];
        for (int column = 0; column < 8; column++)
        {
            if ((pxl & (0x80 >> column)) != 0) // iterate through each bit in pxl
            {
                uint8_t curPxl = _display[regx + column + ((regy + row) * 64)];
                if (curPxl == 1)
                    _v_register[0xF] = 1; // mark collision
                _display[curPxl] = curPxl ^ 1;
            }
        }
    }
    _pc += 2; // increment program counter
    _redraw = true;
}

inline void Chip8VM::skpdn(uint8_t key)
{
    if (_key[key])
        _pc += 2; // increment program counter
    _pc += 2; // increment program counter
}

inline void Chip8VM::skpup(uint8_t key)
{
    if (!_key[key])
        _pc += 2; // increment program counter
    _pc += 2; // increment program counter
}

inline void Chip8VM::getdtmr(uint8_t reg)
{
    _v_register[reg] = _delay_timer;
    _pc += 2; // increment program counter
}

inline void Chip8VM::getkey(uint8_t reg)
{
    uint8_t i = 0;
    bool pressed = false;
    while (!pressed)
    {
        if (i == 16)
            i = 0;
        pressed = _key[i];
        if (pressed)
            break;
        i++;
    }
    _v_register[reg] = i;
    _pc += 2; // increment program counter
}

inline void Chip8VM::setdtmr(uint8_t reg)
{
    _delay_timer = _v_register[reg];
    _pc += 2; // increment program counter
}

inline void Chip8VM::setstmr(uint8_t reg)
{
    _sound_timer = _v_register[reg];
    _pc += 2; // increment program counter
}

inline void Chip8VM::addi(uint8_t reg)
{
    _i_register = _i_register + _v_register[reg];
    _pc += 2; // increment program counter
}

inline void Chip8VM::hxchr(uint8_t reg)
{
    _i_register = FONTSET_MEMORY_START + (reg * 5);
    _pc += 2; // increment program counter
}

inline void Chip8VM::bcd(uint8_t reg)
{
    uint16_t Iaddr = _i_register;
    _memory[Iaddr++] = _v_register[reg] / 100; // hundreds
    _memory[Iaddr++] = (_v_register[reg] / 10) % 10; // tens
    _memory[Iaddr] = _v_register[reg] % 100; // ones

    _pc += 2; // increment program counter
}

inline void Chip8VM::str(uint8_t reg)
{
    uint16_t Iaddr = _i_register;
    for (uint8_t i = 0; i <= reg; i++)
    {
        _memory[Iaddr++] = _v_register[i];
    }
    _pc += 2; // increment program counter
}

inline void Chip8VM::load(uint8_t reg)
{
    uint16_t Iaddr = _i_register;
    for (uint8_t i = 0; i <= reg; i++)
    {
        _v_register[i] = _memory[Iaddr++];
    }
    _pc += 2; // increment program counter
}

inline bool Chip8VM::redraw()
{
    return _redraw;
}

inline void Chip8VM::clearRedraw()
{
    _redraw = false;
}

inline uint8_t* Chip8VM::getDisplay()
{
    return _display;
}

#endif /* CHIP8VM_H_ */
