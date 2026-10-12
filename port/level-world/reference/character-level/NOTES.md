# Character SetLevel and GetLevel

This additive module reconstructs the original Level wrapper and its complete
class/property/vital mutation chain. It does not initialize a Character, own a
network session, or produce design constants. The immutable original ELF is
`.local-inputs/libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Original instructions and state

The main capture records Character::_SetLevel `0x3b73a4` (260 bytes),
Character::GetLevel `0x3bd120` (16 bytes), CharProperties::RecalcProperties(bool)
`0x3e0810` (68 bytes), and UpdateBaseProperties `0x3e087c` (44 bytes).
The helper capture records PROPS_GetInt `0x3df6e0`, PyDataConstants::getConstant
`0x4c4bdc`, RegenHP `0x3bdca4`, and RegenMP `0x3bdbb8`. Captures contain exact
function byte hashes. UpdateBaseProperties is captured context, not an invented
SetLevel call: SetLevel directly stores base property 19 then calls
RecalcProperties(true).

SetLevel accepts only source NUMBER type 3 in the first argument, ignores extra
arguments, and returns no Lua values. The number remains raw fixed-point f32.
The wrapper loads Application's constants object at `+0x2c` and queries literal
`CharacterDesign` (`0x8c1758`) / `MaxLevelDVeryHard` (`0x8c4530`). It shifts the
signed constant word left eight bits with 32-bit wrap, converts the number with
signed `__aeabi_f2iz`, and performs a signed upper-bound comparison. It applies
no lower bound. If the number is greater, it queries the same constant a second
time and stores that second result shifted by eight. NaN converts to zero;
positive and negative overflow saturate to INT_MAX/INT_MIN.

At `0x3b7454` the store is Character `+0x5b8`: CharProperties `+0x560`, base
sheet `+8`, four-byte header, property 19. It writes the base sheet, not saved
properties. RecalcProperties(true) loads class ID from base property 26,
executes `_LoadClass(owner,base,class,false)` and then RecalcProperty for all
224 properties in ascending order. Native implementation reuses the existing
`dh2_class_recalc_base` and property resolver with actual decoded class records,
default/type sheets, base/saved/gear sheets and borrowed buff groups.

The wrapper then calls RegenHP(-1), followed by RegenMP(-1). Each reads current
and maximum cache values (HP 36/38, MP 41/43), substitutes maximum for the
negative request, compares wrapped current+delta against maximum, and retains
the resulting signed delta. A nonpositive delta causes no Add. A positive
delta executes DebugSwitches::load `0x337888`, constructs and queries
`isTracingChar_Stats` (`0x8c4898`) through GetSwitch `0x337a88`, destroys the
temporary string, and calls PROPS_Add with the retained delta. The switch return
is ignored. Callbacks can mutate sheets; Add rereads live property rules but
must not recompute the previously retained delta.

GetLevel calls PROPS_GetInt(19,false). It reads the resolved cache and performs
arithmetic right shift by eight. It does not recalculate, query a host player,
or divide a Lua float. No additional Lua global GetLevel is invented: existing
monster source obtains a level through GetProp and FromFixed where applicable.

## Native contracts and remaining producers

`character_level.hpp/.cpp` exports `dh2_character_set_level`,
`dh2_character_get_level`, `dh2_character_set_level_lua`, and
`dh2_character_level_bind`. `LevelModel32` borrows a mutable PropertyView,
decoded ClassRow records, and persistent script_design_bindings lookup services.
The exact design callback is compatible with the parent-owned native constants
backend. Cache design data contains MaxLevelDVeryHard=100; the module supplies
no fallback cap and distinguishes delivery failure from a genuine zero value.

`LevelServices16` projects genuine DebugSwitch load/query effects. It is required
when a retained refill delta is positive; absent or failed delivery returns 2
after the completed source prefix. The query result is not an acceptance gate.
This module does not claim to reconstruct DebugSwitch loading or string
allocation. Errors before mutation return 1; class/provider failures after a
prefix return 2. Mutable sheets and the receiver survive synchronous callbacks;
backing is aligned and disjoint and callbacks do not retarget the retained
PropertyView. Argument values/design receiver delivery remain stable during a
Lua call; same-VM reentry is outside the runtime callback contract.

Actual class/default/saved/gear/buff ownership and spawning remain caller
producers. The tests use genuine decoded rows plus explicitly named fixture
saved/gear/cache states. They do not present a supplied level integer as a host
level producer. In the VM proof GetHostPlayerLevel queries a separate borrowed
Knight property receiver through the native GetLevel kernel; identification of
that receiver as the session host remains an explicit test ownership boundary.
Position, difficulty and current-level range are explicit world-input fixtures.
GetPyStruct/GetPyOID test adapters read actual decoded metadata but are not a
proof of original global registration. The parent owns their genuine backend.

## Verification and reproduction

`tests/character_level_differential.py` executes actual original SetLevel,
GetLevel, class loading/recalculation, property resolution and PROPS_Add against
the O2 ARM64 implementation. Constants delivery is intercepted at the original
getConstant entry and compared with exact name/ordering. The positive DebugSwitch
block is skipped as an explicit backend boundary, while matching pre-Add effects,
retained delta and transitional base/cache/vital states. No property/class/Add
results are mocked. All four 224-word sheets are compared after every call.

The CHL1 `level-fixtures.bin` corpus contains 1,136 cases: ten genuine Crypt rows,
KnightPlayerBase, three synthetic vital extremes, all argument kinds, no/extra
arguments, IEEE nonfinite/signed-zero/extreme values, signed/wrapping caps,
different results from the two cap lookups, seeded bit patterns, and 84 carried
state updates. The comparison covers 1,017,856 sheet words and 6,110 ordered
original/native callbacks with zero mismatches. Build/source hashes are checked
before and after execution and the report binds the exact compiler inputs.

`tests/character_level_host.py` builds only the new Level source/test against the
actual main-world GetProp, game-data and runtime DSOs, with ASan/UBSan enabled.
It replays the full CHL1 corpus and observes 3,921 design/debug callbacks; class
and Add instruction-entry hooks are observed in the ARM64 differential audit,
not manufactured in the host test. It loads exact cache `_commons.luac` and
`monster.luac`, runs ten real monster initialization sessions, then verifies
SetLevel/GetProp/FromFixed state and return behavior in 60 VM checks. Eight
malformed/provider guards and 2,138,844 checks pass with zero sanitizer findings.
The report binds exact scripts/cache/source, executable and dladdr-resolved DSOs.
Dependency compiler provenance is explicitly not claimed by the isolated runner.

From repo root in PowerShell:

```powershell
& port/level-world/tools/build_character_level_oracle.ps1
$env:PYTHONDONTWRITEBYTECODE='1'
$env:PYTHONPATH='C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages'
& 'C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe' port/level-world/tests/character_level_differential.py
& 'C:\Users\adamc\AppData\Roaming\uv\python\cpython-3.12.13-windows-x86_64-none\python.exe' port/level-world/tests/character_level_host.py
```

Parent central integration can add the production CPP and test target named
`character_level_audit` (test CPP links world, game-data, script-runtime and dl).
The test accepts CHL1 gold, commons file, monster file and data asset directory.
Once built, the same host runner accepts
`--main-linked /home/adampalace/dh2-world-build/character_level_audit` and writes
the separate `reports/character-level-main-linked-host-audit.json`; it leaves the
isolated report intact and does not rebuild the central tree. Central compiler
provenance should be bound by the parent to actual central commands/inputs.

This is standalone source/ARM64 and genuine VM evidence. It does not claim
packaged APK execution, live monster initialization, original full-frame parity,
or full-game completion.
