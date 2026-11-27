#pragma once

void initSimpit();
void SimpitGetInfo();
void SimpitSendControls();   // sends ControlState to KSP: queued commands + changed axes
void printDebugToKSP();
