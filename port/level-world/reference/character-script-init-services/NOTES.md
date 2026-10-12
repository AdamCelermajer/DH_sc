# InitScriptProcess vitals and required skill services

All captures bind original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
This directory contains complete `_InitHpMp`, `SetSkillsAndSpells` and
`UpdateAllSkills` captures, plus ten dependencies and the two list-ID producers.

## Genuine vitals body

Character `_InitHpMp` (`0x3b3a70`,32 bytes) captures the Character receiver,
calls `RegenHP(-1)` (`0x3bdca4`), then tail-calls `RegenMP(-1)` (`0x3bdbb8`).
It does not change CombatActorState/dead/lifecycle/low-health fields. It is one
HP→MP pass. The separately recovered fresh InitPost/Revive double pass is not
the InitScriptProcess provider.

RegenHP reads current36 and maximum38 from the existing resolved property sheet;
RegenMP reads current41 and maximum43 later. The negative request uses maximum.
The sum wraps as uint32 then compares signed against maximum; if greater the
delta becomes wrapped maximum-current. Nonpositive delta returns unchanged.
Positive delta executes shared Debug.load (`0x337888`), genuine GetSwitch
(`0x337a88`) with `isTracingChar_Stats`, discarded result/string cleanup, then
actual CharProperties.AddProperty (`0x3e0708`). Captured current/delta survive
Debug callbacks. MP reads see later live sheet changes. Arithmetic is raw fixed
point; there is no floating conversion, dt scaling or direct maximum assignment.

Existing `dh2_vitals_initialize` already proves the raw single-pass arithmetic
and PropertyAdd behavior. The new bounded helper retains the reached Debug
prefix and its mutation ordering, without changing that frozen module.

## New binding API

`character_script_init_vitals.hpp/.cpp` provides
`dh2_character_script_init_vitals(ScriptInitVitals24*, ScriptInitVitals32*)`.
The32-byte borrowed input contains owner identity, live PropertyView pointer
and fx::PreloadServices16. The latter can use genuine
`dh2_fx_debug_preload_service` with shared DebugModules backing. Debug load/query
are mandatory delivered services when reached. Missing/provider failures stop
the prefix explicitly. Result1 means completed, native-1 malformed before
effects, native-2 required failure with prefix retained. Sheet/view/backing stay
stable through callbacks; their contents may mutate. Output aliases of all six
property sheets, view, input binding, buff groups/pointer arrays/sheets reject.

`character_script_init_vitals_session.cpp` supplies the direct
CharacterInitServices16 callback `dh2_character_script_session_init_service`.
Its persistent `ScriptSessionInit32` context contains owner, debug pointer,
required skills pointer and reserved0. Refresh validates the entry request's
subject against the bound identity and session.timers().owner, then borrows the
session's real PropertyView. It writes the retained shared PropertyState and
never copies/resets the shared CombatActorState. Configure/update requests have
subject0 in the frozen lifecycle ABI and forward to the required skills provider.
Absent skills reject; no successful no-op implementation is installed here.

Use `CharacterInitServices16{&binding,dh2_character_script_session_init_service}`
with the frozen CharacterDeferredScript helper. Caller-owned debug/skills contexts
remain alive through the synchronous call. This does not create an actor, own a
VM, choose Update eligibility, execute initial Idle or allocate skills.

## Skill source inventory and explicit gaps

`SetSkillsAndSpells` (`0x3ce044`,1916 bytes) is nonempty even for empty arrays.
It obtains shared Debug state, captures/restores the active LuaScript path and
argument vector, temporarily selects `data/scripts/skills`, and finally calls
active AIS virtual+0xcc (InitVCB). It cannot be replaced by an accepted empty
callback merely because native slots have not yet been constructed.

Skills use AI begin/end/capacity+b4/b8/bc; spell/faery scripts use+c0/c4/c8.
An existing nonempty vector skips its construction path. Otherwise it asks the
Character's real authored list, reserves its count, and loops records. For each
skill it reads record+0x24; false inserts a null slot. A true record loads actual
common/record script bytes through the active private LuaScript, sets arguments
from record+0x28 and the skill index, constructs `SkillScript` (`0x3cde2c`, source
allocation0x1c) only if loading succeeds, inserts the owned pointer or null,
and calls the real Lua reset routine. The spell path uses record+0x14/+0x18 and
argument -1. Allocation/vector growth/string/ReturnValues/Lua/cache and the
SkillScript ownership/Update bodies remain explicit reconstruction dependencies.

`GetCharSkillListId` (`0x3bc5c0`) reads resolved property28 at Character+0x1068.
Valid nonnegative IDs below the genuine table count are retained; otherwise
source fallback3. `GetCharFaeryListId` (`0x3ae5a0`) reads property29 at+0x106c,
retains valid IDs and otherwise returns0. These are source producers, not a
license to synthesize empty lists or assume the actual Crypt records' counts.
The genuine owned SkillTables decoder exists separately; complete Faery table,
per-Character skill slot construction and live SkillScript ownership are not
provided by this new vitals adapter.

`UpdateAllSkills` (`0x3d8894`,180 bytes) first checks two original FSM predicates
(`0x3c02e8`, `0x3c0334`) through the current live owner. A true predicate returns.
Otherwise it snapshots skill count from initial begin/end, iterates null/non-null
slots and calls actual SkillScript.Update (`0x3dabd0`) for nonnull entries. It
reloads the array base on subsequent iterations. It then separately snapshots
spell count and repeats. Counts are not dynamically rederived after each callback.
This inventory is static captured-instruction evidence; these skill setup/update
bodies were not claimed executed against a native complete ownership backend.

## Proofs and scope

`character-script-init-vitals-arm64-differential.json`: original `_InitHpMp`,
both Regen bodies and complete original property reads/AddProperty execute
against optimized NDK29 ARM64.900 cases,3333 ordered Debug/property requests,
133 reached Debug callback sheet mutations, zero mismatches. Boundary inputs
include signed extremes, overflow, default sentinel and nonpositive deltas.
Debug/string service bodies are explicit fixture boundaries; actual property
instructions are not replaced with arithmetic stubs. Golden fixture SHA256 is
`06b9126ea548b7977d90d625e86b2a53a31e2fe0097f1485d2072a5e2951caed`.

`character-script-init-vitals-host-audit.json`: O2 ASan/UBSan replays the same
900gold/3333 requests,15 atomic aliases/guards and2 required Debug failures.
Separate11 actual Crypt private Session initializations execute exact
commons/monster bytes, then the new genuine live session vitals provider matches
the existing single-pass vitals kernel. Eleven wrong high-bit subject identities
reject atomically; eleven missing-skills configurations reject; CombatState and
shared backing identities are unchanged. Real shared Debug backend executes
with an explicit missing-file provider (one delivery), while borrowed Host rows/
selection/cache are named fixtures. Skills configure/update are explicitly
fixtures in this test, not claimed implemented. Sanitizer diagnostics are zero.
Private central64 dependency copies and exact commands/source/input/binary hashes
are bound in the host report. No shared CMake/renderer/runtime/frozen sources,
APK or emulator work was performed.

One separate deferred-call boundary remains: the legacy generic VM call returns
-2 for both authored Lua error() and protected errors raised by unsupported
native closures. Diagnostic text alone cannot safely identify provenance. The
frozen deferred adapter retains diagnostics and source continuation; its return1
does not prove every nested native callback was implemented. Live callers must
audit/reject unavailable dependencies separately until a protected-call callback
failure provenance channel is provided. This observed ambiguity is not evidence
that actual authored empty Post/Final failed.
