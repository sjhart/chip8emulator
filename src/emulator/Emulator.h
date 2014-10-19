/*
 * Emulator.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "Chip8VM.h"

class Emulator
{
public:
    Emulator();
    virtual ~Emulator();

    Chip8VM ch8;

};

#endif /* EMULATOR_H_ */
