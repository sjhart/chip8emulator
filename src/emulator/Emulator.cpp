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
}

Emulator::~Emulator()
{
}

// MAIN
int main(int argc, char**argv)
{
    Emulator emu;
    emu.ch8.loadMemory(argv[1]);

    while (1) // program loop
    {
        emu.ch8.processChip8Ops();

        // Update timers //TODO: consider making this its own method
        emu.ch8.decDelayTimer();

        if (emu.ch8.getSoundTimer() == 1)
            ; //TODO: need to beep
        emu.ch8.decSoundTimer();
    }
    return 0;
}
