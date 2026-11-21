#pragma once

#include <stdint.h>
#include "LiquidCrystal_I2C.h"
#include "Values.h"

//LCD's
enum LcdID : uint8_t {
    LCD_LBIG,
    LCD_LSMALL,
    LCD_RBIG,
    LCD_RSMALL,
    LCD_COUNT
};

struct LcdDef{
    uint8_t address;
    uint8_t cols;
    uint8_t rows;
    char* name;
};

struct LcdUnit {
    LiquidCrystal_I2C lcd;
    uint8_t address;
    uint8_t cols;
    uint8_t rows;
    char* label;
};

//Slots (een slot is een slot voor informatie, dit bestaat uit 2 rijen (omdat values per 2 verpakt zijn). Deze zijn gemaakt omdat ik 6 'slots' heb en sommige displays kunnen 2 waarden laten zien en sommigen 4 (want 2 displays zijn 4 regels ipv 2).)

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

//Fields (een field is een verzameling aan 2 stukken informatie)

struct Field{
    ValueID line1;
    ValueID line2;
};

void initOutput();
void displayUpdate();

//test fucties
void testOutput();
void testSlots();
