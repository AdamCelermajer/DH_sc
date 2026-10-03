# Character script selection source

`character_script_selection.hpp/.cpp` reconstructs SetScriptByName at
`0x3ceeb0` and StepCreateScript at `0x3cf04c`. Both original instruction bodies
execute in the differential; six backend constructor bodies are captured for
identification but construction, allocation and ownership remain synchronous
services. Original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Selection and stores

The double-underscore prefix selects builtin scripts. Exact suffixes
`monster__`, `player__` and `faery__` choose AISMonster, AISPlayerIPhone and
AISFaery. `npc__` chooses AISDefault and preserves the filename field.
Unknown builtin names choose AISDefault and clear that field AFTER construction.
Non-builtin names choose AISExternal and store the borrowed name pointer AFTER
construction. Recognized builtins preserve constructor mutations to the filename.
SetScriptByName does not independently change the scripted flag.

StepCreateScript reads resolved AI row script length at `+0x28` and name at
`+0x2c`; these are runtime fields, distinct from packed cache offsets. A nonzero
length executes SetScriptByName and stores scripted=1 after it returns. An empty
script chooses ordinary AISPlayer only for the exact GameObject name `Player`,
otherwise AISDefault; afterward it clears the filename and scripted flag.
The authored Player row44 contains `__player__`; the empty Basic row8 fallback
still depends on the actual owner-name producer. See `authored-inputs.json`.

The service constructs a pending object at CharAI `+0x20`; active script
`+0x1c` is a separate ownership/initialization stage. This module does not
instantiate a complete AIS, publish a pending object as active, or replace the
backend with an always-accept script. Those lifecycle producers remain separate
source work. Filename identity is borrowed from immutable data and must outlive
the script lifecycle.

## Evidence

`original-functions.json/.asm`, `selection-reference.bin` and `differential.json`
bind the original and optimized ARM64 comparisons. 1,080 cases cover exact/case/
prefix/embedded-null names, four owner names, prior filename/flag values and
synchronous constructor mutations. All 1,080 factory callbacks and final stores
match. Native ARM64 has four atomic malformed-input checks and pointer identities
above4GiB.

The direct-source CMake host target `character_script_selection_audit` replays the
same gold under ASan/UBSan, with ten guards including unterminated names and
unused-null-field handling. `tests/character_script_selection_host.py` rebuilds
between stable input snapshots and binds the binary and gold hashes in
`reports/character-script-selection-host-audit.json`. There are zero mismatches
or sanitizer findings. The 4096-byte input bound and validation are native caller
contracts, not claimed original bounds checks.

These are source-only component results outside the saved timing APK. Complete
script construction, initialization, updates, Lua, controller ownership and
gameplay are not established by this selector proof.
