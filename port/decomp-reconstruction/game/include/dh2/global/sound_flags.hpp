// byte_99F7E0: .bss byte at 0x99f7e0 (no symbol in the binary). Level::_LoadProcess (0x3f6990) sets it while a level loads and
// VoxSoundManager::PlayBeat (0x36bc58) and PlayMusic (0x36bd78) test it before they play. Definition: vox-audio/src/soundmanager/sound_flags.cpp.
#pragma once

extern unsigned char byte_99F7E0;
