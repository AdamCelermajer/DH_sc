// Application singleton members used by core-platform code. These belong to
// the game and engine categories; tests provide stubs.
#pragma once

// Application::GetDt(): frame delta in milliseconds.
int ApplicationGetDt();

// Device feature bit mask (the original reads it through a virtual slot at +0x5c
// of the device object owned by Application).
unsigned DeviceFeatureFlags();

// Device-class globals set by the platform layer.
extern bool SHARP_DEVICES;
extern bool HTC_DEVICES;
extern bool Is_In_Multiplayer_Mode;
