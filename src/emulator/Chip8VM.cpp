/*
 * Chip8VM.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: Stan
 */

#include "Chip8VM.h"

#include <stdio.h>
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
    _redraw = false;

    const static uint8_t FONTSET[] =
    {
            // 0
            0b11110000, 0b10010000, 0b10010000, 0b10010000, 0b11110000,
            // 1
            0b00100000, 0b01100000, 0b00100000, 0b00100000, 0b01110000,
            // 2
            0b11110000, 0b00010000, 0b11110000, 0b10000000, 0b11110000,
            // 3
            0b11110000, 0b00010000, 0b11110000, 0b00010000, 0b11110000,
            // 4
            0b10010000, 0b10010000, 0b11110000, 0b00010000, 0b00010000,
            // 5
            0b11110000, 0b10000000, 0b11110000, 0b00010000, 0b11110000,
            // 6
            0b11110000, 0b10000000, 0b11110000, 0b10010000, 0b11110000,
            // 7
            0b11110000, 0b00010000, 0b00100000, 0b01000000, 0b01000000,
            // 8
            0b11110000, 0b10010000, 0b11110000, 0b10010000, 0b11110000,
            // 9
            0b11110000, 0b10010000, 0b11110000, 0b00010000, 0b11110000,
            // A
            0b11110000, 0b10010000, 0b11110000, 0b10010000, 0b10010000,
            // B
            0b11100000, 0b10010000, 0b11100000, 0b10010000, 0b11100000,
            // C
            0b11110000, 0b10000000, 0b10000000, 0b10000000, 0b11110000,
            // D
            0b11100000, 0b10010000, 0b10010000, 0b10010000, 0b11100000,
            // E
            0b11110000, 0b10000000, 0b11110000, 0b10000000, 0b11110000,
            // F
            0b11110000, 0b10000000, 0b11110000, 0b10000000, 0b10000000 };

    // load font set
    for (uint8_t i = 0, j = FONTSET_MEMORY_START; i < FONTSET_SIZE; i++, j++)
    {
        _memory[j] = FONTSET[i];
    }
}

Chip8VM::~Chip8VM()
{

}

void Chip8VM::processChip8Ops()
{
    uint8_t byte1 = _memory[_pc] >> 8;
    uint8_t byte2 = _memory[_pc] & 0x00FF;
    uint8_t firstnib = (byte1 >> 4);

    switch (firstnib)
    {
    case 0x00:
    {
        if (byte1 == 0 && byte2 == 0xE0)
        {
            // Clears the screen
            clr();
        }
        else if (byte1 == 0 && byte2 == 0xEE)
        {
            // Returns from a subroutine
            ret();
        }
        else
        {
            // Calls RCA 1802 program at address NNN
            uint16_t address = byte1 & 0x0f;
            address <<= 8;
            address &= byte2;
            rcacall(address);
        }
    }
        break;
    case 0x01:
    {
        // Jumps to address NNN
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        jmp(address);
    }
        break;
    case 0x02:
    {
        // Calls subroutine at NNN
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        call(address);
    }
        break;
    case 0x03:
    {
        // Skips the next instruction if VX equals NN
        uint8_t reg = byte1 & 0x0f;
        skpeq(reg, byte2);
    }
        break;
    case 0x04:
    {
        // Skips the next instruction if VX doesn't equal NN
        uint8_t reg = byte1 & 0x0f;
        skpne(reg, byte2);
    }
        break;
    case 0x05:
    {
        if ((byte2 & 0x0f) != 0x0)
        {
            cerr << "Unrecognized OpCode" << endl;
        }
        else
        {
            // Skips the next instruction if VX equals VY
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            skpeq(reg_x, reg_y);
        }
    }
        break;
    case 0x06:
    {
        // Sets VX to NN
        uint8_t reg = byte1 & 0x0f;
        mov(reg, byte2);
    }
        break;
    case 0x07:
    {
        // Adds NN to VX
        uint8_t reg = byte1 & 0x0f;
        add(reg, byte2);
    }
        break;
    case 0x08:
    {
        uint8_t lastnib = byte2 & 0x0f;
        switch (lastnib)
        {
        case 0x00:
        {
            // Sets VX to the value of VY
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            mov(reg_x, reg_y);
        }
            break;
        case 0x01:
        {
            // Sets VX to VX or VY.
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            _or(reg_x, reg_y);
        }
            break;
        case 0x02:
        {
            // Sets VX to VX and VY.
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            _and(reg_x, reg_y);
        }
            break;
        case 0x03:
        {
            // Sets VX to VX xor VY
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            _xor(reg_x, reg_y);
        }
            break;
        case 0x04:
        {
            // Adds VY to VX.
            // VF is set to 1 when there's a carry,
            // and to 0 when there isn't
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            add(reg_x, reg_y, true);
        }
            break;
        case 0x05:
        {
            // VY is subtracted from VX.
            // VF is set to 0 when there's a borrow,
            // and 1 when there isn't
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            sub(reg_x, reg_y);
        }
            break;
        case 0x06:
        {
            // Shifts VX right by one.
            // VF is set to the value of the least
            // significant bit of VX before the shift
            uint8_t reg = byte1 & 0x0f;
            shr(reg);
        }
            break;
        case 0x07:
        {
            // Sets VX to VY minus VX.
            // VF is set to 0 when there's a borrow,
            // and 1 when there isn't
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            rsub(reg_x, reg_y);
        }
            break;
        case 0x0E:
        {
            // Shifts VX left by one.
            // VF is set to the value of the most
            // significant bit of VX before the shift
            uint8_t reg = byte1 & 0x0f;
            shl(reg);
        }
            break;
        default:
            cerr << "Unrecognized OpCode" << endl;
        }
    }
        break;
    case 0x09:
    {
        if ((byte2 & 0x0f) != 0x0)
        {
            cerr << "Unrecognized OpCode" << endl;
        }
        else
        {
            // Skips the next instruction if VX doesn't equal VY
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            skpne(reg_x, reg_y);
        }
    }
        break;
    case 0x0a:
    {
        // Sets I to the address NNN
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        movi(address);
    }
        break;
    case 0x0b:
    {
        // Jumps to the address NNN plus V0
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        jmpv0(address);
    }
        break;
    case 0x0c:
    {
        // Sets VX to a random number and NN
        uint8_t reg = byte1 & 0x0f;
        rnd(reg, byte2);
    }
        break;
    case 0x0d:
    {
        // Sprites stored in memory at location
        // in index register (I), maximum 8bits
        // wide. Wraps around the screen. If when
        // drawn, clears a pixel, register VF is
        // set to 1 otherwise it is zero. All drawing
        // is XOR drawing (e.g. it toggles the screen pixels)
        uint8_t reg_x = byte1 & 0x0f;
        uint8_t reg_y = (byte2 >> 4);
        uint8_t height = byte2 & 0x0f;
        sprite(reg_x, reg_y, height);
        //TODO: flag screen for redraw
    }
        break;
    case 0x0e:
    {
        if (byte2 == 0x9E)
        {
            // Skips the next instruction if
            // the key stored in VX is pressed
            uint8_t reg = byte1 & 0x0f;
            skpdn(reg);
        }
        else if (byte2 == 0xA1)
        {
            // Skips the next instruction if
            // the key stored in VX isn't pressed
            uint8_t reg = byte1 & 0x0f;
            skpup(reg);
        }
        else
        {
            cerr << "Unrecognized OpCode" << endl;
        }
    }
        break;
    case 0x0f:
    {
        switch (byte2)
        {
        case 0x07:
        {
            // Sets VX to the value of the delay timer
            uint8_t reg = byte1 & 0x0f;
            getdtmr(reg);
        }
            break;
        case 0x0A:
        {
            // A key press is awaited, and then stored in VX
            uint8_t reg = byte1 & 0x0f;
            getkey(reg);
        }
            break;
        case 0x15:
        {
            // Sets the delay timer to VX
            uint8_t reg = byte1 & 0x0f;
            setdtmr(reg);
        }
            break;
        case 0x18:
        {
            // Sets the sound timer to VX
            uint8_t reg = byte1 & 0x0f;
            setdtmr(reg);
        }
            break;
        case 0x1E:
        {
            // Adds VX to I
            uint8_t reg = byte1 & 0x0f;
            addi(reg);
        }
            break;
        case 0x29:
        {
            // Sets I to the location of the
            // sprite for the character in VX.
            // Characters 0-F (in hexadecimal)
            // are represented by a 4x5 font
            uint8_t reg = byte1 & 0x0f;
            hxchr(reg);
        }
            break;
        case 0x33:
        {
            // Stores the Binary-coded decimal
            // representation of VX, with the most
            // significant of three digits at the
            // address in I, the middle digit at I
            // plus 1, and the least significant digit
            // at I plus 2. (In other words, take the
            // decimal representation of VX, place the
            // hundreds digit in memory at location in I,
            // the tens digit at location I+1, and the
            // ones digit at location I+2.)
            uint8_t reg = byte1 & 0x0f;
            bcd(reg);
        }
            break;
        case 0x55:
        {
            // Stores V0 to VX in memory
            // starting at address I
            uint8_t reg = byte1 & 0x0f;
            str(reg);
        }
            break;
        case 0x65:
        {
            // Fills V0 to VX with values
            // from memory starting at address I
            uint8_t reg = byte1 & 0x0f;
            load(reg);
        }
            break;
        default:
            cerr << "Unrecognized OpCode" << endl;
        }
    }
        break;
    }
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
