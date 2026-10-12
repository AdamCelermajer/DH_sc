// NVThread glue (0x533bd8 and 0x533c60). The spawn and JNI functions (0x533cb8, 0x533d20, 0x533e4c) need pthread and
// the JNI environment, which this port does not have, so they are not here; see tinyxml-and-c-libs/NOTES.md.
#pragma once

// Stores the JavaVM pointer for the thread glue and returns it. The pointer is passed as an int, as the 32-bit
// original does.
int NVThreadInit(int vm);

// Sleeps for the given number of milliseconds and returns 0.
int NVThreadSleep(unsigned int milliseconds);
