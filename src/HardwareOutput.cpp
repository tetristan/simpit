#include "HardwareOutput.h"
#include "LiquidCrystal_I2C.h"
#include "GameState.h"
#include "Values.h"
#include "Format.h"

#include <Arduino.h>

// hardware variables
/// alle LCD-addresses
LiquidCrystal_I2C lcds[] = {
    LiquidCrystal_I2C(0x27, 16, 2)
};
/// telt de grootte van de array
const uint8_t NUM_DISPLAYS = sizeof(lcds) / sizeof(lcds[0]);

/// LCD columns
constexpr uint8_t LCD_COLS = 16; //LCD COLS is 16, constexpr maak thet op compile-time voor optimization


// functions

void initOutput(){
    //initialice LCD's
    for (uint8_t i =0; i< NUM_DISPLAYS; i++) {
        lcds[i].init();
        lcds[i].backlight();
        lcds[i].clear();
        lcds[i].setCursor(0,0);
        lcds[i].print("LCD: ");
        lcds[i].print(i+1);
    }
};

void buildLine(const char* label, const char* valueText, char* out) {
    // Missing text counts as empty text, so strlen never sees a nullptr
    if (label == nullptr)     label = "";
    if (valueText == nullptr) valueText = "";

    // maakt alles leeg en zet de end-marker neer
    memset(out, ' ', LCD_COLS);
    out[LCD_COLS] = '\0';

    // meet de input en clampt ze op een max lengte
    size_t labelLen = strlen(label);
    size_t valueLen = strlen(valueText);
    if (labelLen > LCD_COLS) labelLen = LCD_COLS;
    if (valueLen > LCD_COLS) valueLen = LCD_COLS;

    // plaatst label links
    memcpy(out, label, labelLen);

    // plaatst value rechts, plus een space ertussen
    size_t valueStart = LCD_COLS - valueLen;   // safe: valueLen <= LCD_COLS
    memcpy(out + valueStart, valueText, valueLen);

    // Keep one space between label and value
    if (valueLen > 0 && valueStart > 0) {
        out[valueStart - 1] = ' ';
    }
}

void DisplayOutput(){
    // if(game.connectedToGame ==false){
    //     lcds[0].setCursor(0,0);
    //     lcds[0].print("waiting for KSP");
    // }
    //
    // else if (game.connectedToGame ==true) {
    //     lcds[0].setCursor(0,0);
    //     lcds[0].print("Apoapsis: ");
    //     lcds[0].setCursor(0,1);
    //     lcds[0].print(game.apoapsis);
    // }
};

//Testfuncties

void testFormatDistance() {
    float tests[] = {0, 850, 9999, 85100, -412000, 12000000, 1.2e10, 1e30};
    char out[11];   // value budget 10 + '\0'

    for (uint8_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        formatDistance(tests[i], out, sizeof(out));
        Serial.print('|');
        Serial.print(out);
        Serial.print("|  len=");
        Serial.println(strlen(out));
    }
}

void testFormatTime() {
    float tests[] = {0, 45, 59.6, 60, 725, -720005, 3600, 21599, 21600, 100000, NAN, 1e12};
    char out[11] = "";

    for (uint8_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        formatTime(tests[i], out, sizeof(out));
        Serial.print('|');
        Serial.print(out);
        Serial.print("|  len=");
        Serial.println(strlen(out));
    }
}

void testFormatAll() {
    for (uint8_t i = 1; i < VAL_COUNT; i++) {
        ValueID id = (ValueID)i;                // int → enum: needs a cast
        const ValueDef& def = valueDefs[id];    // short name for this row

        char valueText[VALUE_TEXT_SIZE] = "";
        if (isValid(id)) {
            formatValue(def.kind, getValue(id), valueText, sizeof(valueText));
        } else {
            snprintf(valueText, sizeof(valueText), "---");
        }

        char line[LCD_COLS + 1];
        buildLine(def.label, valueText, line);

        Serial.print('|');
        Serial.print(line);
        Serial.println('|');
    }
}

void testOutput(){
    for (int i=1; i< VAL_COUNT; i++) {
        // float v = getValue((ValueID)i);
        // Serial.println("TEST");
        // Serial.println(v);
        // Serial.println(valueDefs[i].label);

        char buf[LCD_COLS + 1];            // room for 16 characters + '\0'
        buildLine("Ap", "85.1km", buf);    // buildLine fills buf
        lcds[0].setCursor(0, 1);
        lcds[0].print(buf);
        Serial.print('|');                 // pipes make the spaces visible
        Serial.print(buf);
        Serial.println('|');
        Serial.println('TESTING DISTANCE');
        testFormatDistance();
        Serial.println('TESTING TIME');
        testFormatTime();
        Serial.println('TESTING ALL');
        testFormatAll();
    }
}


