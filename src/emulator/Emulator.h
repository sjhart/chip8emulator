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

    void keyDown(unsigned char key, int xx, int yy);
    void keyUp(unsigned char key, int xx, int yy);
    void idle();
    void render(void);
};

#endif /* EMULATOR_H_ */
