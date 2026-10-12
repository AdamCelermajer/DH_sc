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

// File-open path used by the gameswf callbacks (owned by the application and
// stream categories; declared here, defined there).
struct FsFileHandle;
struct IStreamBase;
IStreamBase* ApplicationOpenStream(const char* name);  // Application file system slot +0x88
void BufferedStreamFillBuffer(FsFileHandle* handle, IStreamBase* stream);  // BufferedStream::FillBuffer
void ApplicationCloseStream(IStreamBase* stream);       // file system close slot +0x78
