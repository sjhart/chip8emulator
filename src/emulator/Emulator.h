/*
 * Emulator.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "Chip8VM.h"
#include "InstructionSet.h"

class Emulator
{
public:
    Emulator();
    virtual ~Emulator();
    void processChip8Ops();

    Chip8VM ch8;
    InstructionSet* is;
};

#endif /* EMULATOR_H_ */
