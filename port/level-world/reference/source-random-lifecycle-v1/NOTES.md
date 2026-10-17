# Source RNG lifecycle owner

The original ARM32 library is pinned by SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` hashes the complete `Random.cpp` static initializer,
`GSInit::Update`, `Level::Unload`, and `Timer::getRealTime` bodies. In the
source callers, startup state 13 and level unload each read the raw 32-bit
millisecond clock, write it to the ordinary seed, and clear the synchronized
seed. Neither path resets the debug counters. The static initializer zeros
both counter words but does not set either seed.

`source_random_lifecycle_v1.cpp` is the selected owner in `dh2_level_world`.
`process_state()` returns the same singleton `dh2_random_state` for every
consumer. `seed_from_gsinit_update(time)` and `seed_from_level_unload(time)`
perform only the two original seed stores, leaving both counters untouched.
The caller injects `time`; this owner does not read a host or Android clock,
perform random draws, reset counters, or create another VM or combat RNG.

The selected-library host audit links against `dh2_level_world.dll` and checks
stable owner identity, both lifecycle writes, preserved counters, and the full
`uint32_t` timer boundaries. It also verifies every function pin against the
original ELF. Run it with:

```powershell
python port/level-world/tests/run_source_random_lifecycle_v1_host.py `
  --original-elf C:\tmp\dh2-original-libDungeonHunter2.so
```

Android model_renderer injects the source-equivalent gettimeofday millisecond
word into GSInit::Update only on a fresh gameplay load (restore == false).
The Back-to-main-menu route explicitly tears down the gameplay owner before
calling the Level::Unload seed. EGL restoration, pause/resume, debug reload,
preview replacement, and generic error cleanup do not seed the stream.

The API 37/16 KiB menu smoke validates fresh start, Back, occupied-slot restart,
and Home/resume event boundaries. The owner is now selected in the Android
library. The native saved-class resolver borrows this owner; its current player
path draws nothing. NPC template selection, inventory and loot consumers are
still disconnected. Combat and animation RNG projections remain separate until
their draw ordering is reconciled.
