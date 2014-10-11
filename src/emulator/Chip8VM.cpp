/*
 * Chip8VM.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: Stan
 */

#include "Chip8VM.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

Chip8VM::Chip8VM()
{
    memset(_v_register, 0, sizeof(_v_register));
    _i_register = 0;
    _sp = 0;
    _pc = PROGRAM_MEMORY_START;
    memset(_stack, 0, sizeof(_stack));
    _delay_timer = 0;
    _sound_timer = 0;
    memset(_key, 0, sizeof(_key));
    memset(_memory, 0, sizeof(_memory));
    memset(_display, 0, sizeof(_display));
    memset(_memory, 0, sizeof(_memory));

    const static uint8_t FONTSET[] =
    {
        // 0
        0b11110000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b11110000,
        // 1
        0b00100000,
        0b01100000,
        0b00100000,
        0b00100000,
        0b01110000,
        // 2
        0b11110000,
        0b00010000,
        0b11110000,
        0b10000000,
        0b11110000,
        // 3
        0b11110000,
        0b00010000,
        0b11110000,
        0b00010000,
        0b11110000,
        // 4
        0b10010000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b00010000,
        // 5
        0b11110000,
        0b10000000,
        0b11110000,
        0b00010000,
        0b11110000,
        // 6
        0b11110000,
        0b10000000,
        0b11110000,
        0b10010000,
        0b11110000,
        // 7
        0b11110000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b01000000,
        // 8
        0b11110000,
        0b10010000,
        0b11110000,
        0b10010000,
        0b11110000,
        // 9
        0b11110000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b11110000,
        // A
        0b11110000,
        0b10010000,
        0b11110000,
        0b10010000,
        0b10010000,
        // B
        0b11100000,
        0b10010000,
        0b11100000,
        0b10010000,
        0b11100000,
        // C
        0b11110000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b11110000,
        // D
        0b11100000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b11100000,
        // E
        0b11110000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b11110000,
        // F
        0b11110000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b10000000
    };

    // load font set
    for (uint8_t i = 0, j = FONTSET_MEMORY_START; i < FONTSET_SIZE; i++, j++)
    {
        _memory[j] = FONTSET[i];
    }
}

Chip8VM::~Chip8VM()
{

}

uint8_t Chip8VM::getVRegister(uint8_t v_num) const
{
    //TODO error check register index
    return _v_register[v_num];
}

void Chip8VM::setVRegister(uint8_t v_num, uint8_t v_val)
{
    //TODO error check register index
    _v_register[v_num] = v_val;
}

uint16_t Chip8VM::getIRegister() const
{
    return _i_register;
}

void Chip8VM::setIRegister(uint16_t i)
{
    _i_register = i;
}

uint16_t Chip8VM::getPC() const
{
    return _pc;
}

void Chip8VM::setPC(uint16_t pc)
{
    _pc = pc;
}

uint8_t Chip8VM::getDelayTimer() const
{
    return _delay_timer;
}

void Chip8VM::setDelayTimer(uint8_t delayTimer)
{
    _delay_timer = delayTimer;
}

uint8_t Chip8VM::getSoundTimer() const
{
    return _sound_timer;
}

void Chip8VM::setSoundTimer(uint8_t soundTimer)
{
    _sound_timer = soundTimer;
}

const bool Chip8VM::isKeyPressed(uint8_t key_num) const
{
    return _key[key_num];
}

uint8_t Chip8VM::getMemory(uint16_t index) const
{
    //TODO error check index bounds
    return _memory[index];
}

void Chip8VM::setMemory(uint16_t index, uint8_t val)
{
    //TODO error check index bounds
    _memory[index] = val;
}

uint8_t Chip8VM::getDisplay(uint16_t index) const
{
    //TODO error check index bounds
    return _display[index];
}

void Chip8VM::setDisplay(uint16_t index, uint8_t val)
{
    //TODO error check index bounds
    _display[index] = val;
}

void Chip8VM::loadMemory(char *file)
{
    FILE *f = fopen(file, "rb");
    if (f == NULL)
    {
        cout << "error: Couldn't open " << file << endl;
        exit(1);
    }

    // Get the file size
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    // CHIP-8 convention puts programs in memory at 0x200
    // They will all have hardcoded addresses expecting that
    //
    // Read the file into memory at PROGRAM_MEMORY_START and close it.
    fread(_memory + PROGRAM_MEMORY_START, fsize, 1, f);
    fclose(f);
}

uint8_t Chip8VM::getCurrentMemory() const
{
    return getMemory(_pc);
}

void Chip8VM::stackPush()
{
    //TODO:validate stack is not full
    _stack[_sp++] = _pc;
}

void Chip8VM::stackPop()
{
    //TODO: validate stack is not empty
    _pc = _stack[_sp--];
}

void Chip8VM::clearDisplay()
{
    memset(_display, 0, sizeof(_display));
}

void Chip8VM::incPC()
{
    _pc += 2;
}

void Chip8VM::decDelayTimer()
{
    if (_delay_timer > 0)
        _delay_timer--;
}

void Chip8VM::decSoundTimer()
{
    if (_sound_timer > 0)
        _sound_timer--;
}
