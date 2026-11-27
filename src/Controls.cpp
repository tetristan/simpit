#include "ControlState.h"
#include "InputState.h"
#include "GameState.h"
#include "Config.h"
#include "KerbalSimpitMessageTypes.h"   // STAGE_ACTION, SAS_ACTION, AP_... (only the enums, no serial)
#include <Arduino.h>

// =====================================================================
//  Controls: turns "what the hardware does" (InputState) into
//  "what the game should do" (ControlState).
//  This is the ONLY file that decides what a button means.
//  Rules:
//   - Buttons act on EDGES (`pressed`), never on "is held", so a button
//     can never fire twice and the panel never fights the keyboard.
//   - GameState is only READ here, to make smart decisions
//     (e.g. "SAS is off, so turn it on before choosing a SAS mode").
// =====================================================================

ControlState controls = {};

// ---------------------------------------------------------------------
//  Helpers
// ---------------------------------------------------------------------
static void queueCommand(CommandType type, uint8_t value) {
    if (controls.commandCount >= COMMAND_QUEUE_SIZE) return;    // full: drop it (shouldn't happen)
    controls.commands[controls.commandCount] = {type, value};
    controls.commandCount++;
}

static bool pressed(ButtonID id) { return input.button[id].pressed; }
static bool held(ButtonID id)    { return input.button[id].held; }

// Deadzone: everything within JOYSTICK_DEADZONE of the centre becomes 0.
// The rest is stretched so the output still starts at 0 at the edge of
// the deadzone (no jump) and still reaches 1.0 at full deflection.
static float applyDeadzone(float v) {
    float mag = fabs(v);
    if (mag <= JOYSTICK_DEADZONE) return 0.0f;
    float out = (mag - JOYSTICK_DEADZONE) / (1.0f - JOYSTICK_DEADZONE);
    return (v < 0) ? -out : out;
}

// Response curve. Linear for now.
// To get finer control near the centre, replace the return with e.g.
//   return v * fabs(v);                                   // square curve
//   return AXIS_EXPO * v*v*v + (1.0f - AXIS_EXPO) * v;    // adjustable expo
// See docs/InputModule.md, "Tuning the joysticks".
static float applyCurve(float v) {
    return v;
}

// full joystick processing for one axis: deadzone -> curve -> precision
static float shapeAxis(AxisID id, bool precision) {
    float v = applyCurve(applyDeadzone(input.axis[id]));
    if (precision) v *= PRECISION_FACTOR;
    return v;
}

// -1..+1 -> Simpit int16
static int16_t toSimpit(float v) {
    v = constrain(v, -1.0f, 1.0f);
    return (int16_t)lroundf(v * 32767.0f);   // round, so full deflection really gives 32767
}

// ---------------------------------------------------------------------
//  Arm / stage / abort
// ---------------------------------------------------------------------
static void handleArmStageAbort() {
    controls.armed = held(BTN_ARM);

    // The stage button latches down: `pressed` happens once when it goes
    // down, releasing it (turning the ring) does nothing.
    // Not armed at the moment of the press = ignored, also when you arm later.
    if (controls.armed && pressed(BTN_STAGE)) {
        queueCommand(CMD_ACTION_ACTIVATE, STAGE_ACTION);
    }
    if (controls.armed && pressed(BTN_ABORT)) {
        queueCommand(CMD_ACTION_ACTIVATE, ABORT_ACTION);
    }
}

// ---------------------------------------------------------------------
//  SAS / RCS
// ---------------------------------------------------------------------
static void handleSasRcs() {
    if (pressed(BTN_SAS_TOGGLE)) queueCommand(CMD_ACTION_TOGGLE, SAS_ACTION);
    if (pressed(BTN_RCS_TOGGLE)) queueCommand(CMD_ACTION_TOGGLE, RCS_ACTION);

    // SAS mode buttons
    for (uint8_t i = 0; i < 10; i++) {
        if (!pressed((ButtonID)(BTN_SAS_STAB + i))) continue;

        // choosing a mode while SAS is off: switch SAS on first,
        // otherwise KSP ignores the mode. Commands are sent in order.
        bool sasOn = (game.actionGroups & SAS_ACTION) != 0;
        if (!sasOn) queueCommand(CMD_ACTION_ACTIVATE, SAS_ACTION);

        queueCommand(CMD_SAS_MODE, AP_STABILITYASSIST + i);
    }
}
static_assert(BTN_SAS_MANEUVER - BTN_SAS_STAB == 9,          "SAS buttons must be 10 in a row");
static_assert(AP_MANEUVER - AP_STABILITYASSIST == 9,         "AutopilotMode numbering changed?");

// ---------------------------------------------------------------------
//  Function buttons and custom action groups
//  All toggles: KSP flips the current state, and the button LED shows
//  the result from GameState. The panel never has its "own" opinion.
// ---------------------------------------------------------------------
static void handleFunctions() {
    if (pressed(BTN_FN_LIGHTS)) queueCommand(CMD_ACTION_TOGGLE, LIGHT_ACTION);
    if (pressed(BTN_FN_GEAR))   queueCommand(CMD_ACTION_TOGGLE, GEAR_ACTION);
    if (pressed(BTN_FN_BRAKES)) queueCommand(CMD_ACTION_TOGGLE, BRAKES_ACTION);
    if (pressed(BTN_FN_SOLAR))  queueCommand(CMD_CAG_TOGGLE, CAG_SOLAR);
    if (pressed(BTN_FN_COMMS))  queueCommand(CMD_CAG_TOGGLE, CAG_COMMS);

    for (uint8_t i = 0; i < 5; i++) {
        if (pressed((ButtonID)(BTN_CAG_1 + i))) queueCommand(CMD_CAG_TOGGLE, i + 1);
    }
}
static_assert(BTN_CAG_5 - BTN_CAG_1 == 4, "CAG buttons must be 5 in a row");

// ---------------------------------------------------------------------
//  Not mapped yet. The inputs are read, debounced and available in
//  `input`; add the actions here later.
// ---------------------------------------------------------------------
static void handleUnmapped() {
    // NAV 1-5:          pressed(BTN_NAV_1) ... pressed(BTN_NAV_5)
    // NAV_SWITCH 1 / 2: held(BTN_NAV_SWITCH_1_UP), held(BTN_NAV_SWITCH_1_DOWN), neither = middle
    // joystick buttons: pressed(BTN_J1_BUTTON), pressed(BTN_J2_BUTTON)
}

// ---------------------------------------------------------------------
//  Joystick 1: translation / wheels / EVA
//  Priority: EVA switch > wheel mode (dial) > translation.
//  Everything that is NOT active is set to 0, so KSP gets a 0 when you
//  switch modes and nothing stays stuck at the last value.
// ---------------------------------------------------------------------
static void updateJoystick1() {
    AxisOutputs& o = controls.axes;
    controls.j1Precision = held(BTN_J1_PRECISION);
    controls.j1Eva       = held(BTN_J1_EVA);
    controls.wheelMode   = (input.mode == MODE_PLANE || input.mode == MODE_SURFACE);

    int16_t x = toSimpit(shapeAxis(AXIS_J1_X, controls.j1Precision));
    int16_t y = toSimpit(shapeAxis(AXIS_J1_Y, controls.j1Precision));
    int16_t z = toSimpit(shapeAxis(AXIS_J1_Z, controls.j1Precision));

    o.transX = o.transY = o.transZ = 0;
    o.wheelSteer = o.wheelThrottle = 0;
    o.evaX = o.evaY = o.evaZ = 0;

    if (controls.j1Eva) {
        o.evaX = x;                 // stored only, see docs/KSP2-notes.md
        o.evaY = y;
        o.evaZ = z;
    } else if (controls.wheelMode) {
        o.wheelSteer    = x;
        o.wheelThrottle = y;
    } else {
        o.transX = x;
        o.transY = y;
        o.transZ = z;
    }
}

// ---------------------------------------------------------------------
//  Joystick 2: rotation / camera
// ---------------------------------------------------------------------
static void updateJoystick2() {
    AxisOutputs& o = controls.axes;
    controls.j2Precision = held(BTN_J2_PRECISION);
    controls.j2Camera    = held(BTN_J2_CAMERA);

    int16_t x = toSimpit(shapeAxis(AXIS_J2_X, controls.j2Precision));
    int16_t y = toSimpit(shapeAxis(AXIS_J2_Y, controls.j2Precision));
    int16_t z = toSimpit(shapeAxis(AXIS_J2_Z, controls.j2Precision));

    o.pitch = o.roll = o.yaw = 0;
    o.camPitch = o.camYaw = o.camZoom = 0;

    if (controls.j2Camera) {
        o.camYaw   = x;
        o.camPitch = y;
        o.camZoom  = z;
    } else {
        o.yaw   = x;
        o.pitch = y;
        o.roll  = z;
    }
}

// ---------------------------------------------------------------------
//  Throttle: bottom THROTTLE_CUT of the travel = exactly 0,
//  the rest is stretched to 0..100%.
// ---------------------------------------------------------------------
static void updateThrottle() {
    float t = input.axis[AXIS_THROTTLE];
    if (t <= THROTTLE_CUT) t = 0.0f;
    else                   t = (t - THROTTLE_CUT) / (1.0f - THROTTLE_CUT);
    controls.axes.throttle = toSimpit(t);
}

// ---------------------------------------------------------------------
//  setControls: every loop(), after getInputs()
// ---------------------------------------------------------------------
void setControls() {
    // commands are only queued here; SimpitSendControls() empties the queue
    handleArmStageAbort();
    handleSasRcs();
    handleFunctions();
    handleUnmapped();

    updateJoystick1();
    updateJoystick2();
    updateThrottle();
}
