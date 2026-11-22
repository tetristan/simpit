#include "LcdOutput.h"
#include "LiquidCrystal_I2C.h"
#include "GameState.h"
#include "Values.h"
#include "Format.h"
#include "Config.h"

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>

// hardware variables
/// LCD's
LcdUnit lcdUnits[] = {
    LiquidCrystal_I2C(0x25, 20, 4), 0x25, 20, 4, "Left Big",
    LiquidCrystal_I2C(0x24, 16, 2), 0x24, 16, 2, "Left small",
    LiquidCrystal_I2C(0x27, 20, 4), 0x27, 20, 4, "Right big",
    LiquidCrystal_I2C(0x26, 16, 2), 0x26, 16, 2, "Right small",
};
/// telt de grootte van de array
const uint8_t NUM_DISPLAYS = sizeof(lcdUnits) / sizeof(lcdUnits[0]);

/// houdt bij of alle LCD's verbonden zijn
bool lcdPresent[LCD_COUNT];

///layout, per mode/lcd/regel
constexpr ValueID LcdModeLayout[MODE_COUNT][NUM_DISPLAYS][4] = { //4 is de max line-count
    //---MODE_ORBIT---
    {
    /*LCD_LBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
    /*LCD_LSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
    /*LCD_RBIG*/{VAL_AP,       VAL_PE,     VAL_T_AP,       VAL_V_HOR},
    /*LCD_RSMALL*/{VAL_NODE_START,    VAL_ALT_RADAR,   VAL_NONE,   VAL_NONE},
    },

    //---MODE_LANDING---
    {
        /*LCD_LBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_LSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
    },

    //---MODE_PLANE---
    {
        /*LCD_LBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_LSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
    },

    //---MODE_SURFACE---
    {
        /*LCD_LBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE  },
        /*LCD_LSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
    },

    //---MODE_RENDEZVOUS---
    {
        /*LCD_LBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE  },
        /*LCD_LSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RBIG*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
        /*LCD_RSMALL*/{VAL_NONE,    VAL_NONE,   VAL_NONE,   VAL_NONE},
    },
};

//LCD buffer
char lastDrawn[NUM_DISPLAYS][MAX_LINES][MAX_COLS + 1]; //deze array bewaart voor alle LCD's de huidige staat


// functions
void invalidateDisplays() {     // forceert volledige redraw (voor na mode-change of na init_output)
    memset(lastDrawn, 0, sizeof(lastDrawn));
}

void initOutput(){
    //empty display buffer
    invalidateDisplays();

    //initialize LCD's
    for (uint8_t i = 0; i<NUM_DISPLAYS; i++) {
        LcdUnit& u = lcdUnits[i];

        Wire.beginTransmission(u.address);
        uint8_t err = Wire.endTransmission();
        if (err==0) {
            lcdPresent[i] = true;
            u.lcd.init();
            u.lcd.backlight();
            u.lcd.clear();
            u.lcd.print(u.label);
            u.lcd.setCursor(12, 0);
            u.lcd.print("0x");
            u.lcd.setCursor(14, 0);
            u.lcd.print(u.address, HEX);
        }
        else {
            lcdPresent[i] = false;
        }
    }
    //setclock
    Wire.setClock(I2C_CLOCK); //dit is nodig omdat de lcd.init functie intern de clock weer terugzet
    //empty LCD's
    invalidateDisplays();
}

void buildLine(const char* label, const char* valueText, char* out, uint8_t lcdCols) {
    // Missing text counts as empty text, so strlen never sees a nullptr
    if (label == nullptr)     label = "";
    if (valueText == nullptr) valueText = "";

    // maakt alles leeg en zet de end-marker neer
    memset(out, ' ', lcdCols);
    out[lcdCols] = '\0';

    // meet de input en clampt ze op een max lengte
    size_t labelLen = strlen(label);
    size_t valueLen = strlen(valueText);
    if (labelLen > lcdCols) labelLen = lcdCols;
    if (valueLen > lcdCols) valueLen = lcdCols;

    // plaatst label links
    memcpy(out, label, labelLen);

    // plaatst value rechts, plus een space ertussen
    size_t valueStart = lcdCols - valueLen;   // safe: valueLen <= lcdCols
    memcpy(out + valueStart, valueText, valueLen);

    // Keep one space between label and value
    if (valueLen > 0 && valueStart > 0) {
        out[valueStart - 1] = ' ';
    }
}

void valueToLine(ValueID id, uint8_t lcdCols, char* out){
    if (id == VAL_NONE){        //value none = lege line
        buildLine("","",out, lcdCols);
        return;
    }
    const ValueDef& def = valueDefs[id];

    if (!isValid(id)){     //value invalid = ---
        buildLine(def.label,"---" ,out, lcdCols);
        return;
    }
    char valueText[VALUE_TEXT_SIZE] = "";
    formatValue(def.kind, getValue(id), valueText, sizeof(valueText));
    buildLine(def.label,valueText,out, lcdCols);
}

void renderLcd(uint8_t lcdId, uint8_t mode) {
    if (!lcdPresent[lcdId]) return;
    LcdUnit& u = lcdUnits[lcdId];

    for (uint8_t line = 0; line < u.rows; line++) {
        char buf[MAX_COLS + 1];
        valueToLine(LcdModeLayout[mode][lcdId][line], u.cols, buf); //schrijft de huidige staat naar de

        if (strcmp(buf, lastDrawn[lcdId][line]) == 0) continue;  // waarde is veranderd

        u.lcd.setCursor(0, line);
        u.lcd.print(buf);
        strcpy(lastDrawn[lcdId][line], buf);
    }
}

void displayUpdate(){
    static unsigned long lastUpdate = 0;
    static uint8_t nextLcd = 0;

    if (millis() - lastUpdate < DISPLAY_INTERVAL_MS){
        return;
    }

    lastUpdate = millis();

    renderLcd(nextLcd, game.mode);
    nextLcd = (nextLcd +1)% LCD_COUNT;
}
