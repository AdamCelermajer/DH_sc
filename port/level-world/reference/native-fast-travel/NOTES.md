# Native fast-travel source boundary

IDA batch analysis of `libDungeonHunter2.so` (SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`)
confirms this order:

- `NativeGoToZone` (`0x4422b8`) checks `Application::GetCurrentLevel`; the current Level must be absent or have byte `+0x144` set. It converts entry `-1` to `0`, calls `QuickSave(false)` only in state `38`, then `SG_SaveAllPlayer(false)`, reads local Character slot, resolves the exact LevelList name, reads Character difficulty, invokes HUD `DisplayFastTravel(false, name, name, entry)`, then calls `Application::LoadLevel(LevelFile, entry, slot, true, true, difficulty, false, 0, 0)`.
- `Application::LoadLevel` (`0x32bdc8`) on an existing state-38 Level sends the score, saves the local Player (`0x3f07b0`, force `true`), calls `QuickSave(true)`, then `ResetIsLoaded` (`0x3ef224`) before dispatching `GSLevel::LoadLevel` (`0x386818`). The current `engine-ui/level_transition_v1` kernel covers only the NativeGoToZone prefix; a production load provider must include this second source sequence.
- `Level::QuickSave` is `0x3f059c`. It uses the local Character, requires the source LevelSavegame at Level `+0xec`, state `+0x130 == 38`, Character vtable `+0x34 == 0`, and the offline/host gates. It copies Character words `+0x160/+0x164/+0x168` to `+0x1468/+0x146c/+0x1470`, sets LevelSavegame byte `+0x39` to `0` when forced or `0x6c` otherwise, calls Save, then restores the byte.
- The `Level` constructor (`0x3f3128`) creates `LevelSavegame` only after matching a LevelList row. It passes the current slot, hub, row ordinal, difficulty, and checkpoint=false; the filename format is `dh2_%03u_%01u_%03u_%03u_level.savegame`. `LevelSavegame` ctor (`0x462934`) allocates its separate `Savegame` at `+4` and registers `INFO` and `OBJS`. `Save` (`0x4615ec`) calls `Savegame::saveAll` (`0x315fb8`) on that `+4` object when its gate permits. `INFO` stores the LevelSavegame info word; `OBJS` traverses ObjectManager and calls each qualifying source object's virtual save writer (`+0x10`).

The selected app has no LevelSavegame/Savegame owner for that per-level file and no source-compatible OBJS writer for the current renderer actors. Its PlayerSavegame profile Transport is a separate file and cannot stand in for this object. `NativeGoToZone` therefore remains fail-closed. The former `save_active_level_for_transition` adapter was removed because it incorrectly routed LevelSavegame::Save through PlayerSavegame Transport.

The level-world kernels' function addresses were corrected from stale values: QuickSave `0x3f059c`, LevelSavegame ctor `0x462934`, and Save `0x4615ec`.
