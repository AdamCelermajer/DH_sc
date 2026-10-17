// byte_99F7E0, the sound-allowed byte of the binary's .bss (no symbol). Level::_LoadProcess sets it during the level load; PlayBeat and
// PlayMusic return without playing while it is clear. Declared in dh2/global/sound_flags.hpp.
#include "dh2/global/sound_flags.hpp"

unsigned char byte_99F7E0 = 0;
