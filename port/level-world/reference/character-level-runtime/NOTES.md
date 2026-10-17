# Native Character level and regeneration adapter

## Source ownership

Runtime composes the frozen original SetLevel caller, HP/MP regeneration callers,
DebugSwitches Runtime and live-map save writer with existing verified class and
property kernels. It is a reusable native adapter, with zero additional original
caller/dependency body claims. It does not reconstruct original GetInt or allocator
bodies. The original function manifest is provenance for reused behavior.

Native model_renderer ObjectInstance and PlayerCombat already own PropertyState.
Their base/saved/gear/resolved arrays and actor_property_rules are compatible with
the borrowed PropertyView. The renderer's ClassTables are currently local to scene
loading, so actual table rows/formulas and native ClassRow views must be retained
by the owner before this Runtime can be bound there. Design bindings retain actual
pycst views and their immutable original bytes for the same lifetime.

## Live effects and helper domain

The full set_level entry supports two genuinely fresh argument reads through a
typed service; set_level_fixed retains the float already supplied by the VM.
These are raw source binary32 values, after the original script's ToFixed call.
Actual design queries use the native manager identity and actual pycst data for
CharacterDesign/MaxLevelDVeryHard. Missing/ambiguous data fails explicitly.

The numerical conversion accepts finite binary32 in [-2^31,2^31), with exact
truncation toward zero. Other numerical helper inputs fail before the source
Level store. This is an explicit finite-domain implementation, not a claim for
the original helper's NaN/infinity/out-of-range behavior.

The original source Level store is published into mutable base[19] before calling
dh2_class_recalc_base directly with the live PropertyView. This avoids cached
copy/commit wrappers. A failing class formula retains the Level and preceding
base/resolved mutations. HP/MP callers read actual cached resolved words, then
use the real debug Runtime and owned native string tokens before PropertyAdd.
No arbitrary derived-stat writes or host/player/current-level facts are supplied.

Runtime retains string storage left by failed source query until its own
destruction. Debug selection/services are captured for each regeneration caller;
loaded/query phases use the retained owner. Providers retain retired debug owners
and all control/backing addresses. A busy guard rejects same Runtime reentry;
independent owners may nest. Errors preserve prior effects without added cleanup,
regeneration or rollback. Filesystem/save services remain genuine owner adapters,
with no successful no-op writes.

## Verification

The dedicated host gate reuses the unchanged positive-debug fixture prefix through
a generated header, recording both the complete source and extracted-prefix
hashes. Old fixtures are not edited. Eighteen unchanged OnInit cases cover both
Ghost records and actual Crypt/Swamp/Infected Village difficulty ranges, with
positive HP and MP and actual file persistence. The complete gate has 42 host
cases. Additional cases cover direct
native calls, partial class failure, finite/unsupported numerical conversion,
fresh arguments, reentry, missing design/debug/input services and mutable-sheet
alias rejection. A real save provider mutates live HP during the debug phase;
the source captured amount is retained while the subsequent actual PropertyAdd
uses the mutated saved sheet. A second fresh NaN argument rejects without storing
the first value, and duplicate design ownership is rejected. The actual original
cache files remain unchanged.

PlayerInfo/current-Level producers are explicitly host fixtures. No Android
wiring, full native initialization lifecycle, new original body count or original
OS stream/allocator body claim is made by this adapter test.

```powershell
python port/level-world/tests/run_character_level_runtime_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files
```

## Original application design producer

The original Application constructors start `+0x2c` at null. `PostInit`
(`0x32f7e8`, 628 bytes) allocates 36 bytes, calls the 80-byte
`PyDataConstants` constructor (`0x4c36b8`) with the app's `+0x28`
`DataReloaderManager`, then installs the new manager at `+0x2c`
(`0x32f858`). The constants constructor retains that dependency at `+0x1c`
and starts its loading stage `+0x20` at zero. Its `Load` stage 4 resolves
`pydata/design_pycst.bin` and `design_pycst.bin` from the original
PC-relative literals at `0x4c3d28`/`0x4c3d4c`; final strings reside at
`0x8d8810`/`0x8d8828`. The manifest pins those ranges, hashes and literal
calculations. This is producer evidence, with no new complete loader or
Application body claim.

SetLevel retains the Application selected after the first number query, then
reads its design manager at `0x3b7424` and rereads it in the upper-clamp branch
at `0x3b7480`. The native adapter's DesignBinding must map that source owner
to retained actual `design_pycst.bin` bytes. The actual cache value is 100;
the backend obtains it through `dh2_pycst_get`, without a maximum-level default.

Host PlayerInfo and current-Level identity have separate producer routes in
`reference/lua-script-level-queries/NOTES.md`: PlayerManager character-level
synchronization and the retained current GSLevel/Level. This backend does not
substitute an authored Prince level or install an unproduced host/global owner.

## Source initial HP and MP adapter

`Runtime::initialize_hp_mp` composes the separately reconstructed
`Character::_InitHpMp` caller with these same real HP/MP, property, string,
Debug and filesystem providers. It restores HP then MP using raw -1, with no
Level store or class recalculation. Each regeneration phase captures its live
Debug singleton independently. Missing used services fail, preceding file and
property effects remain, and mutable sheet/control aliases reject.

The extended gate has 50 host cases, including eight initial-vitals cases on
both authored Ghost records. These cover actual HP/MP depletion and restoration,
full-health fast paths, MP-only restoration, real file failure, missing Debug,
file-time HP mutation and aliased sheets. The preceding 42-case report remains
a historical checkpoint. This adapter is a provider for native InitScriptProcess;
its presence alone does not prove production skill/post/final execution.
