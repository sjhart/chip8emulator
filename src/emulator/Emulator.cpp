/*
 * Emulator.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Stan
 */
#include "Chip8VM.h"

#include <stdint.h>
#include <GL/glut.h>
#include <unistd.h>
#include <iostream>

using namespace std;


// Display size
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

Chip8VM ch8;
int modifier = 10;
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;
uint8_t* display = ch8.getDisplay();

void keyboardDown(unsigned char key, int x, int y)
{
    if (key == 27)    // esc
        exit(0);

    if (key == '1')
        ch8.setKey(0x1);
    else if (key == '2')
        ch8.setKey(0x2);
    else if (key == '3')
        ch8.setKey(0x3);
    else if (key == '4')
        ch8.setKey(0xC);

    else if (key == 'q')
        ch8.setKey(0x4);
    else if (key == 'w')
        ch8.setKey(0x5);
    else if (key == 'e')
        ch8.setKey(0x6);
    else if (key == 'r')
        ch8.setKey(0xD);

    else if (key == 'a')
        ch8.setKey(0x7);
    else if (key == 's')
        ch8.setKey(0x8);
    else if (key == 'd')
        ch8.setKey(0x9);
    else if (key == 'f')
        ch8.setKey(0xE);

    else if (key == 'z')
        ch8.setKey(0xA);
    else if (key == 'x')
        ch8.setKey(0x0);
    else if (key == 'c')
        ch8.setKey(0xB);
    else if (key == 'v')
        ch8.setKey(0xF);
}

void keyboardUp(unsigned char key, int x, int y)
{
    if (key == '1')
        ch8.clearKey(0x1);
    else if (key == '2')
        ch8.clearKey(0x2);
    else if (key == '3')
        ch8.clearKey(0x3);
    else if (key == '4')
        ch8.clearKey(0xC);

    else if (key == 'q')
        ch8.clearKey(0x4);
    else if (key == 'w')
        ch8.clearKey(0x5);
    else if (key == 'e')
        ch8.clearKey(0x6);
    else if (key == 'r')
        ch8.clearKey(0xD);

    else if (key == 'a')
        ch8.clearKey(0x7);
    else if (key == 's')
        ch8.clearKey(0x8);
    else if (key == 'd')
        ch8.clearKey(0x9);
    else if (key == 'f')
        ch8.clearKey(0xE);

    else if (key == 'z')
        ch8.clearKey(0xA);
    else if (key == 'x')
        ch8.clearKey(0x0);
    else if (key == 'c')
        ch8.clearKey(0xB);
    else if (key == 'v')
        ch8.clearKey(0xF);
}

void drawPixel(int x, int y)
{
    glBegin(GL_QUADS);
    glVertex3f((x * modifier) + 0.0f, (y * modifier) + 0.0f, 0.0f);
    glVertex3f((x * modifier) + 0.0f, (y * modifier) + modifier, 0.0f);
    glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
    glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f, 0.0f);
    glEnd();
}
void updateQuads()
{
    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            if (display[(y * 64) + x] == 0)
            {
                glColor3f(0.0f, 0.0f, 0.0f);
            }
            else
            {
                glColor3f(1.0f, 1.0f, 1.0f);
            }
            drawPixel(x, y);
        }
    }
}

void render(void)
{
    ch8.processChip8Ops();

    // Update timers
    ch8.decDelayTimer();

    if (ch8.getSoundTimer() == 1)
        cout << "Beep!" << endl;
    ch8.decSoundTimer();

    if (ch8.redraw())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        updateQuads();
        glutSwapBuffers();
        ch8.clearRedraw();
    }
// Throttle
    usleep(160); // 1000ms / 16 = 16.666667ms
}

void reshape_window(GLsizei w, GLsizei h)
{
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
// Resize quad
    display_width = w;
    display_height = h;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Usage: Chip8Emulator.exe <rom>" << endl;
        return 1;
    }
// Load game
    ch8.loadMemory(argv[1]);
// Setup OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
    glutCreateWindow("Chip8");
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape_window);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutMainLoop();
    return 0;
}

