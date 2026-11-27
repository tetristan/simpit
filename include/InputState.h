#pragma once
#include <stdint.h>
#include "GameState.h"   // for SimpitMode (the 5-way dial writes input.mode)

// =====================================================================
//  InputState: what the panel hardware is doing RIGHT NOW.
//  Filled by getInputs() (InputHandler.cpp), read by setControls()
//  (Controls.cpp) and later by the LED/LCD code.
//  This struct knows nothing about KSP: it only says "button X is held",
//  "joystick 2 is 40% to the left".
// =====================================================================

// ---------------------------------------------------------------------
//  Every digital input on the panel (45 in total).
//  Which PIN belongs to which ID is set in the wiring table at the top of
//  InputHandler.cpp, so you can reorder this list freely EXCEPT for the
//  three groups marked "ORDER MATTERS": Controls.cpp does arithmetic on them.
// ---------------------------------------------------------------------
enum ButtonID : uint8_t {
    // SAS modes. ORDER MATTERS: BTN_SAS_x - BTN_SAS_STAB = KSP AutopilotMode (0-9)
    BTN_SAS_STAB,           // 0  stability assist
    BTN_SAS_PROGRADE,       // 1
    BTN_SAS_RETROGRADE,     // 2
    BTN_SAS_NORMAL,         // 3
    BTN_SAS_ANTINORMAL,     // 4
    BTN_SAS_RADIAL_IN,      // 5
    BTN_SAS_RADIAL_OUT,     // 6
    BTN_SAS_TARGET,         // 7
    BTN_SAS_ANTITARGET,     // 8
    BTN_SAS_MANEUVER,       // 9

    BTN_SAS_TOGGLE,         // SAS on/off (momentary)
    BTN_RCS_TOGGLE,         // RCS on/off (momentary)

    // function buttons (F1-F5 on the panel)
    BTN_FN_LIGHTS,
    BTN_FN_GEAR,
    BTN_FN_BRAKES,
    BTN_FN_SOLAR,           // custom action group 6 (assign in the VAB)
    BTN_FN_COMMS,           // custom action group 7 (assign in the VAB)

    // custom action groups. ORDER MATTERS: CAG number = BTN_CAG_x - BTN_CAG_1 + 1
    BTN_CAG_1,
    BTN_CAG_2,
    BTN_CAG_3,
    BTN_CAG_4,
    BTN_CAG_5,

    // arm / stage / abort
    BTN_ARM,                // latching key switch: held = armed
    BTN_STAGE,              // latches down when pressed, released by turning the ring
    BTN_ABORT,              // momentary

    // 5-way mode dial. ORDER MATTERS: BTN_MODE_x - BTN_MODE_1 = SimpitMode
    BTN_MODE_1,             // MODE_ORBIT
    BTN_MODE_2,             // MODE_LANDING
    BTN_MODE_3,             // MODE_PLANE
    BTN_MODE_4,             // MODE_SURFACE (rover)
    BTN_MODE_5,             // MODE_RENDEZVOUS

    // navigation (no function yet)
    BTN_NAV_1,
    BTN_NAV_2,
    BTN_NAV_3,
    BTN_NAV_4,
    BTN_NAV_5,
    BTN_NAV_SWITCH_1_UP,    // NAV_SWITCH 1, ON-OFF-ON: up position
    BTN_NAV_SWITCH_1_DOWN,  //                          down position
    BTN_NAV_SWITCH_2_UP,
    BTN_NAV_SWITCH_2_DOWN,

    // joystick buttons (no function yet)
    BTN_J1_BUTTON,
    BTN_J2_BUTTON,

    // joystick toggle switches (latching)
    BTN_J1_PRECISION,       // joystick 1 (translation) precision mode
    BTN_J1_EVA,             // joystick 1 controls the Kerbal on EVA
    BTN_J2_PRECISION,       // joystick 2 (rotation) precision mode
    BTN_J2_CAMERA,          // joystick 2 controls the camera

    BTN_COUNT               // always last = number of buttons
};

// ---------------------------------------------------------------------
//  Analog inputs.
// ---------------------------------------------------------------------
enum AxisID : uint8_t {
    AXIS_J1_X,              // joystick 1 (translation / wheels / EVA)
    AXIS_J1_Y,
    AXIS_J1_Z,              // twist
    AXIS_J2_X,              // joystick 2 (rotation / camera)
    AXIS_J2_Y,
    AXIS_J2_Z,              // twist
    AXIS_THROTTLE,
    AXIS_COUNT
};

// One button, after debouncing.
struct ButtonState {
    bool held;              // true as long as the contact is closed (debounced)
    bool pressed;           // true for exactly ONE loop: the moment it closed
    bool released;          // true for exactly ONE loop: the moment it opened
};

struct InputState {
    ButtonState button[BTN_COUNT];

    // Joysticks: -1.0 .. +1.0, 0 = centre (calibrated at startup, NO deadzone yet)
    // Throttle:   0.0 .. 1.0
    float axis[AXIS_COUNT];

    SimpitMode mode;        // position of the 5-way dial (keeps the last position between clicks)
    bool modeChanged;       // true for one loop after the dial moved
};

extern InputState input;

void initInputs();          // call once in setup(): pin modes, joystick centre calibration
void getInputs();           // call every loop(): reads all hardware into `input`
