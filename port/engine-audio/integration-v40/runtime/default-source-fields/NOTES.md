# Fresh source emitter and general defaults V40

`proof.json` records bounded execution from the exact original ELF, with its SHA256. Twenty-four randomized receiver seeds prove the reached `EmitterObj::C1`866380 prefix before `Reset3D`8652b8; the fixture redirects to the real constructor epilogue, then separately executes the whole original Reset3D and GetGain/GetPitch methods. Mutex calls are explicit single-thread services. The supplied data/driver receiver cells are borrowed fixtures, not a running native audio device.

| Source field | Emitter offset | Fresh value |
|---|---|---|
|position|9c,a0,a4|0,0,0|
|velocity|a8,ac,b0|0,0,0|
|direction|b4,b8,bc|0,0,0|
|relative flag / integer parameter0|c0|false|
|maximum distance|c4|FLT_MAX, bits7f7fffff|
|reference distance|c8|100, bits42c80000|
|rolloff|cc|1|
|GetGain current value|3c|1|
|GetPitch current value|74|1|

This corrects the earlier diagnostic offset labels:9c is position, not relative; a8 begins velocity. `Reset3D` sets these same values through the original emitter parameter setters. `Update3D`863318 maps those fields to driver's Set3DParameter8909e8: position→6c/70/74, velocity→78/7c/80, direction→84/88/8c, relative→90, max→94, reference→98, rolloff→9c.

The bounded callback-driver Init893108 prefix independently writes gain/pitch Q14 units16384, and whole driver GetGain/GetPitch returns1. Its own pre-emitter-update reference distance is1 rather than the emitter's100. That low-level pre-update value must not replace the actual emitter value. Format and driver sample-rate are explicit fixtures; driver buffer provisioning and playback were not executed.

General spatial defaults are actual ELF `.data` initializers:

| Global | Address | Serialized value |
|---|---|---|
|s_distanceModel|99e260|integer2|
|s_dopplerFactor|99e264|float1, bits3f800000|
|s_alteredSpeedOfSound|99e268|float343.29998779296875, bits43aba666|

The reached manager Initialize36c2e4 prefix calls `VoxEngine::Set3DGeneralParameteri`861b38 at36c3d8 with parameter2,value4. The real wrapper and engine setter865ee0 execute against a borrowed engine singleton fixture, producing engine field430=4 and dirty flag436=1. Thus2 is the driver's boot initializer;4 is the actual source manager initialization request. This proof does not assert that unknown later setters leave those values unchanged, or that an engine's ordinary general-storage fields were initialized merely by its constructor.

`default_source_fields_v40.hpp` supplies the proved fresh-emitter fields and separate original general boot/manager-init operations. Feed the actual retained general authority; after real manager initialization, apply its documented model4 change. Then apply the parent `vox_source_emitter_fields_v38` using the genuine selected listener row and actual vectors. Fresh source emission has no actor-velocity argument; zero fresh velocity is supported by original construction. Reused/retained emitter state or later setters must be retained by their actual owner. Actual settings gain/group modifiers, event RNG, source lifetime and source readiness remain separate requirements.

The helper replays24 original captured records in221 O2 ASan+UBSan checks, and compiles strictly for both Android ABIs. `compile-validation.json` records source/gold hashes. No focus-packet, V38, shared application, World phase, device/runtime or root build files were changed.
