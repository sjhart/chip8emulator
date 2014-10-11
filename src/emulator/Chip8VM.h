/*
 * Chip8VM.h
 *
 *  Created on: Oct 6, 2014
 *      Author: Stan
 */

#ifndef CHIP8VM_H_
#define CHIP8VM_H_

#include <stdint.h>

class Chip8VM
{
public:
    // Constants
    const static uint16_t PROGRAM_MEMORY_START = 0x200;
    const static uint16_t FONTSET_MEMORY_START = 0x0;
    const static uint8_t FONTSET_SIZE = 80;
    const static uint8_t FONTSET[];

    Chip8VM();
    virtual ~Chip8VM();

    // Getters/Setters
    uint8_t getVRegister(uint8_t v_num) const;
    void setVRegister(uint8_t v_num, uint8_t v_val);

    uint16_t getIRegister() const;
    void setIRegister(uint16_t v_val);

    uint16_t getPC() const;
    void setPC(uint16_t pc);

    uint8_t getDelayTimer() const;
    void setDelayTimer(uint8_t delayTimer);

    uint8_t getSoundTimer() const;
    void setSoundTimer(uint8_t soundTimer);

    const bool isKeyPressed(uint8_t key_num) const;

    uint8_t getMemory(uint16_t index) const;
    void setMemory(uint16_t index, uint8_t val);

    uint8_t getDisplay(uint16_t index) const;
    void setDisplay(uint16_t index, uint8_t val);

    // Utility Methods
    void loadMemory(char *file);
    uint8_t getCurrentMemory() const;
    void stackPush();
    void stackPop();
    void clearDisplay();
    void incPC();
    void decDelayTimer();
    void decSoundTimer();

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
};

#endif /* CHIP8VM_H_ */
