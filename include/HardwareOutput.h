#pragma once

#include <stdint.h>

enum LcdID : uint8_t {
    LCD_L12,
    LCD_L3,
    LCD_R12,
    LCD_R3,
    LCD_COUNT
};

struct LcdDef{
    uint8_t address;
    uint8_t cols;
    uint8_t rows;
    char* name;
};

enum SlotID : uint8_t {
    SLOT_L1,
    SLOT_L2,
    SLOT_L3,
    SLOT_R1,
    SLOT_R2,
    SLOT_R3,
    SLOT_COUNT
};

struct SlotDef{
    LcdID lcd;
    uint8_t firstRow;
};

void initOutput();
void DisplayOutput();

//test fucties
void testOutput();
void testSlots();
