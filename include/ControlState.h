#pragma once
#include <stdint.h>

// =====================================================================
//  ControlState: what the panel WANTS the game to do.
//  Made by setControls() (Controls.cpp) from InputState + GameState,
//  sent to KSP by SimpitSendControls() (SimpitLink.cpp).
//
//  It holds two different kinds of output:
//   1. Axes: continuous values. They always hold the current wish;
//      SimpitLink only sends a group when it has changed.
//   2. Commands: one-shot events (stage, toggle SAS, ...). They wait in a
//      small queue, SimpitLink sends them and then EMPTIES the queue.
//      A command is never sent twice, and never sent "late" after a reconnect.
// =====================================================================

// ---- one-shot commands ----
enum CommandType : uint8_t {
    CMD_ACTION_ACTIVATE,    // value = action-group bitmask (STAGE_ACTION, ABORT_ACTION, SAS_ACTION ...)
    CMD_ACTION_TOGGLE,      // value = action-group bitmask
    CMD_CAG_TOGGLE,         // value = custom action group number (1-10)
    CMD_SAS_MODE,           // value = AutopilotMode (AP_PROGRADE ...)
};

struct Command {
    CommandType type;
    uint8_t     value;
};

constexpr uint8_t COMMAND_QUEUE_SIZE = 8;   // more than enough: you can't press 8 buttons in one loop

// ---- axes, in Simpit units ----
// joysticks: -32767 .. +32767 (0 = centre), throttle: 0 .. 32767
struct AxisOutputs {
    // joystick 2, normal: vessel rotation
    int16_t pitch, roll, yaw;
    // joystick 2, camera switch on: camera
    int16_t camPitch, camYaw, camZoom;
    // joystick 1, normal: translation (RCS)
    int16_t transX, transY, transZ;
    // joystick 1, dial on PLANE or SURFACE: wheels
    int16_t wheelSteer, wheelThrottle;
    // joystick 1, EVA switch on: Kerbal movement.
    // Filled in, but NOT sent in KSP1 (see docs/KSP2-notes.md).
    int16_t evaX, evaY, evaZ;
    // throttle lever
    int16_t throttle;
};

struct ControlState {
    AxisOutputs axes;

    Command commands[COMMAND_QUEUE_SIZE];
    uint8_t commandCount;           // number of waiting commands

    // panel status, for the LEDs (ARM1, J1-J4) and displays later
    bool armed;
    bool j1Precision;
    bool j1Eva;
    bool j2Precision;
    bool j2Camera;
    bool wheelMode;                 // joystick 1 is driving wheels (dial on PLANE/SURFACE)
};

extern ControlState controls;

void setControls();     // every loop(), after getInputs()
