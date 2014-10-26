/*
 * Emulator.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */

#include "Emulator.h"

#include <stdint.h>
#include <iostream>
#include <GL/glut.h>
#include <unistd.h>

using namespace std;

int mainWindow;
Emulator emu;
const double border = 5;
uint8_t* display = emu.ch8.getDisplay();

Emulator::Emulator()
{
}

Emulator::~Emulator()
{
}

void keyDown(unsigned char key, int xx, int yy)
{
    uint8_t keyIndex = 0;
    switch (key)
    {
    case '0':
        keyIndex = 0;
        break;
    case '1':
        keyIndex = 1;
        break;
    case '2':
        keyIndex = 2;
        break;
    case '3':
        keyIndex = 3;
        break;
    case '4':
        keyIndex = 4;
        break;
    case '5':
        keyIndex = 5;
        break;
    case '6':
        keyIndex = 6;
        break;
    case '7':
        keyIndex = 7;
        break;
    case '8':
        keyIndex = 8;
        break;
    case '9':
        keyIndex = 9;
        break;
    case 'a':
        keyIndex = 10;
        break;
    case 'b':
        keyIndex = 11;
        break;
    case 'c':
        keyIndex = 12;
        break;
    case 'd':
        keyIndex = 13;
        break;
    case 'e':
        keyIndex = 14;
        break;
    case 'f':
        keyIndex = 15;
        break;
    }
    emu.ch8.setKey(keyIndex);
}

void keyUp(unsigned char key, int xx, int yy)
{
    uint8_t keyIndex = 0;
    switch (key)
    {
    case '0':
        keyIndex = 0;
        break;
    case '1':
        keyIndex = 1;
        break;
    case '2':
        keyIndex = 2;
        break;
    case '3':
        keyIndex = 3;
        break;
    case '4':
        keyIndex = 4;
        break;
    case '5':
        keyIndex = 5;
        break;
    case '6':
        keyIndex = 6;
        break;
    case '7':
        keyIndex = 7;
        break;
    case '8':
        keyIndex = 8;
        break;
    case '9':
        keyIndex = 9;
        break;
    case 'a':
        keyIndex = 10;
        break;
    case 'b':
        keyIndex = 11;
        break;
    case 'c':
        keyIndex = 12;
        break;
    case 'd':
        keyIndex = 13;
        break;
    case 'e':
        keyIndex = 14;
        break;
    case 'f':
        keyIndex = 15;
        break;
    }
    emu.ch8.clearKey(keyIndex);
}

void render(void)
{
    emu.ch8.processChip8Ops();

        // Update timers
        emu.ch8.decDelayTimer();

        if (emu.ch8.getSoundTimer() == 1)
            ; //TODO: need to beep
        emu.ch8.decSoundTimer();

    if (emu.ch8.redraw())
    {
        //clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();        //load identity matrix
        glOrtho(0.0f, 640.0, 320.0, 0.0f, 0.0f, 1.0f);

        glPointSize(10.0f);        //set point size to 10 pixels

        glBegin(GL_POINTS); //starts drawing of points
        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 64; j++)
            {
                if (display[(i * 64) + j])
                {
                    glVertex3f((i * 10) + border, (j * 10) + border, 0.0f);
                }
            }
        }
        glEnd(); //end drawing of points
        glFlush();
        emu.ch8.clearRedraw();

        // Throttle
       usleep(160); // 1000ms / 16 = 16.666667ms
    }
}


// MAIN
int main(int argc, char**argv)
{
    emu.ch8.loadMemory(argv[1]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(640 + 10, 320 + 10);
    glutInitWindowPosition(100, 100);
    mainWindow = glutCreateWindow("Chip8");
    glutDisplayFunc(render);
    glutIgnoreKeyRepeat(true);
    glutKeyboardUpFunc(keyUp);
    glutKeyboardFunc(keyDown);
    glutIdleFunc(render);
    glutMainLoop();

    return 0;
}

