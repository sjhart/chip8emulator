/*
 * Emulator.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#include "Emulator.h"

#include <stdint.h>
#include <iostream>

using namespace std;

Emulator::Emulator()
{
    is = new InstructionSet(ch8);

}

Emulator::~Emulator()
{
    delete is;
}

void Emulator::processChip8Ops()
{
    uint8_t byte1 = ch8.getCurrentMemory() >> 8;
    uint8_t byte2 = ch8.getCurrentMemory() & 0x00FF;
    uint8_t firstnib = (byte1 >> 4);

    switch (firstnib)
    {
    case 0x00:
    {
        if (byte1 == 0 && byte2 == 0xE0)
        {
            // Clears the screen
            is->clr();
        }
        else if (byte1 == 0 && byte2 == 0xEE)
        {
            // Returns from a subroutine
            is->ret();
        }
        else
        {
            // Calls RCA 1802 program at address NNN
            uint16_t address = byte1 & 0x0f;
            address <<= 8;
            address &= byte2;
            is->rcacall(address);
        }
    }
        break;
    case 0x01:
    {
        // Jumps to address NNN
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        is->jmp(address);
    }
        break;
    case 0x02:
    {
        // Calls subroutine at NNN
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        is->call(address);
    }
        break;
    case 0x03:
    {
        // Skips the next instruction if VX equals NN
        uint8_t reg = byte1 & 0x0f;
        is->skpeq(reg,byte2);
    }
        break;
    case 0x04:
    {
        // Skips the next instruction if VX doesn't equal NN
        uint8_t reg = byte1 & 0x0f;
        is->skpne(reg,byte2);
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
            is->skpeq(reg_x,reg_y);
        }
    }
        break;
    case 0x06:
    {
        // Sets VX to NN
        uint8_t reg = byte1 & 0x0f;
        is->mov(reg,byte2);
    }
        break;
    case 0x07:
    {
        // Adds NN to VX
        uint8_t reg = byte1 & 0x0f;
        is->add(reg,byte2);
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
            is->mov(reg_x,reg_y);
        }
            break;
        case 0x01:
        {
            // Sets VX to VX or VY.
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            is->_or(reg_x,reg_y);
        }
            break;
        case 0x02:
        {
            // Sets VX to VX and VY.
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            is->_and(reg_x,reg_y);
        }
            break;
        case 0x03:
        {
            // Sets VX to VX xor VY
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            is->_xor(reg_x,reg_y);
        }
            break;
        case 0x04:
        {
            // Adds VY to VX.
            // VF is set to 1 when there's a carry,
            // and to 0 when there isn't
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            is->add(reg_x,reg_y,true);
        }
            break;
        case 0x05:
        {
            // VY is subtracted from VX.
            // VF is set to 0 when there's a borrow,
            // and 1 when there isn't
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            is->sub(reg_x,reg_y);
        }
            break;
        case 0x06:
        {
            // Shifts VX right by one.
            // VF is set to the value of the least
            // significant bit of VX before the shift
            uint8_t reg = byte1 & 0x0f;
            is->shr(reg);
        }
            break;
        case 0x07:
        {
            // Sets VX to VY minus VX.
            // VF is set to 0 when there's a borrow,
            // and 1 when there isn't
            uint8_t reg_x = byte1 & 0x0f;
            uint8_t reg_y = (byte2 >> 4);
            is->rsub(reg_x,reg_y);
        }
            break;
        case 0x0E:
        {
            // Shifts VX left by one.
            // VF is set to the value of the most
            // significant bit of VX before the shift
            uint8_t reg = byte1 & 0x0f;
            is->shl(reg);
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
            is->skpne(reg_x,reg_y);
        }
    }
        break;
    case 0x0a:
    {
        // Sets I to the address NNN
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        is->movi(address);
    }
        break;
    case 0x0b:
    {
        // Jumps to the address NNN plus V0
        uint16_t address = byte1 & 0x0f;
        address <<= 8;
        address &= byte2;
        is->jmpv0(address);
    }
        break;
    case 0x0c:
    {
        // Sets VX to a random number and NN
        uint8_t reg = byte1 & 0x0f;
        is->rnd(reg,byte2);
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
        is->sprite(reg_x, reg_y, height);
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
            is->skpdn(reg);
        }
        else if (byte2 == 0xA1)
        {
            // Skips the next instruction if
            // the key stored in VX isn't pressed
            uint8_t reg = byte1 & 0x0f;
            is->skpup(reg);
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
            is->getdtmr(reg);
        }
            break;
        case 0x0A:
        {
            // A key press is awaited, and then stored in VX
            uint8_t reg = byte1 & 0x0f;
            is->getkey(reg);
        }
            break;
        case 0x15:
        {
            // Sets the delay timer to VX
            uint8_t reg = byte1 & 0x0f;
            is->setdtmr(reg);
        }
            break;
        case 0x18:
        {
            // Sets the sound timer to VX
            uint8_t reg = byte1 & 0x0f;
            is->setdtmr(reg);
        }
            break;
        case 0x1E:
        {
            // Adds VX to I
            uint8_t reg = byte1 & 0x0f;
            is->addi(reg);
        }
            break;
        case 0x29:
        {
            // Sets I to the location of the
            // sprite for the character in VX.
            // Characters 0-F (in hexadecimal)
            // are represented by a 4x5 font
            uint8_t reg = byte1 & 0x0f;
            is->hxchr(reg);
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
            is->bcd(reg);
        }
            break;
        case 0x55:
        {
            // Stores V0 to VX in memory
            // starting at address I
            uint8_t reg = byte1 & 0x0f;
            is->str(reg);
        }
            break;
        case 0x65:
        {
            // Fills V0 to VX with values
            // from memory starting at address I
            uint8_t reg = byte1 & 0x0f;
            is->load(reg);
        }
            break;
        default:
            cerr << "Unrecognized OpCode" << endl;
        }
    }
        break;
    }

    // Update timers
    ch8.decDelayTimer();

    if (ch8.getSoundTimer() == 1)
        ;//TODO: need to beep
    ch8.decSoundTimer();
}



// MAIN
int main(int argc, char**argv)
{
    Emulator emu;
    emu.ch8.loadMemory(argv[1]);

    while(1) // program loop
    {
        emu.processChip8Ops();
    }
    return 0;
}
