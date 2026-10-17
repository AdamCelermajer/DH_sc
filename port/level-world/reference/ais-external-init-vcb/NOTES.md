# AIS callback flag initialization

`ais_external_init_vcb.{hpp,cpp}` maintains two complete original callers:
AISDefault::InitVCB92B0x3dc7d8 and AISExternal::InitVCB408B0x3dcec8. The original
ELF and exact translated PT_LOAD function/string hashes are pinned in the
adjacent manifest. Supporting ranges are evidence, not extra complete bodies.
This module does not allocate an AIS, initialize a Character, run OnInit, own
Lua state, create the source state table, or bind native Ghost AI.

## Original query and store order

Default first stores b8=0, queries actual VFTable membership for OnTargetHit,
stores800 or0, queries OnTargetMissed, and stores the cached accumulator OR1000
when present. External runs Default first, then reads the resulting b8 once
into its cached accumulator. Every following query stores that accumulator
after its result, preserving all13 source stores and12 membership calls.
No short circuit skips an absent alias or a later query.

| Query order | VFTable key | Source bit |
| --- | --- | --- |
| 1 | OnTargetHit | 0x800 |
| 2 | OnTargetMissed | 0x1000 |
| 3 | OnUpdate | 0x1 |
| 4 | OnFriendSpotted | 0x2 |
| 5 | OnTargetOutOfRange | 0x4 |
| 6 | OnTargetInRangedRange | 0x8 |
| 7 | OnTargetInCloseRange | 0x10 |
| 8 | OnTargetInMeleeRange | 0x20 |
| 9 | OnMasterOutOfRange | 0x40 |
| 10 | OnMasterInRangedRange | 0x80 |
| 11 | OnMasterInCloseRange | 0x100 |
| 12 | OnMasterInMeleeRange | 0x200 |

All keys are decoded from original PC-relative literals, rather than inferred
from the callback names. Bit2 is OnFriendSpotted, not OnTimer. Source116B
LuaScript::IsInVFTable hashes a name and searches the unsigned hash-key map at
LuaScript+34, returning canonical0/1. It does not inspect global Lua functions.
External's first query ORs that raw canonical result directly; subsequent
queries map truthiness to their assigned bit.

The typed borrowed contains service must query the actual live registration
map. Registration mutation can change subsequent query results. Provider writes
to b8 are overwritten by the cached accumulator after normal return; rereading
the mutated flag word after each callback would change the original caller.
One owning thread retains AIS/VM/map/context and retired backing through return.
Stable identity, controls and output cannot be overwritten; same State reentry
is forbidden, independent objects may nest. Unavailable/error/throwing services
stop preserving prior source stores and provider effects without rollback.
Unavailable first query still follows the source initial reset to0. These error
statuses and alignment/alias guards are explicit port boundaries.

## Actual plain monster registrations

The host fixture executes unchanged ai/_commons.luac (SHA20d34968...) then
ai/monster.luac (SHA84f07caa...) using the maintained real float32 Lua VM and
the existing source hash-key alias map. Only GetPyStruct/SkillTree28, a controlled
GetProp value and the maintained FromFixed numeric helper are load-time
providers. No OnInit/combat/timer callback body is invoked by this fixture.

The registered target-range and hit/miss aliases produce b8=0x183c. This number
is a verified output, never a production initialization constant. Monster's
OnTargetMissed aliases monster_OnTargetHit. Commons declares a global
OnUpdate(timestamp), but neither script registers OnUpdate in its VFTable;
bit1 stays clear. Original AISExternal::OnUpdate64B calls Default, tests bit1,
optionally calls LuaScript::Call(char const*)112B with ZERO explicit arguments,
then calls state update and conditions. There is no timestamp producer in this
caller. A global-function-exists test or unconditional commons OnUpdate dispatch
would invent behavior. Later legitimate VFTable changes require actual source
InitVCB invocation; this module does not automatically reinitialize flags.

## State-table and constructor boundary

CharAIScript C1/C2 initialize owner+98=0, an empty state registry+9c and current
state pointer+b4=0. AISExternal C1/C2 then initialize b8=0, bc=0 and c0=0.
The distinct CharAI constructor has fields at the same numerical offsets; those
are different object layouts and cannot substitute for the AIS fields.

CharAIScript::_ChangeState128B is the verified non-constructor b4 store: resolve
the registered state, call old StatePost, store the captured new state pointer,
then call StateInit. The unchanged commons/monster sources register no states
and call neither RegisterState nor ChangeState. Their initial empty registry
and null b4 therefore remain bounded source facts while no external state
producer runs. CallStateUpdate20B and CallStateConditions20B first read live b4;
null returns without a Lua call. Nonnull uses state's callback-name+14/+2c and
calls Lua with no explicit arguments. This audit does not claim that another
script, later native callback or state-registry producer cannot change b4.
The producer/ownership binding must retain old states through synchronous calls.

## Verification

Host tests cover all individual registrations/complements, both callers, fresh
registration changes, cached flag behavior, all callback failure positions,
exceptions retaining effects, missing services, control aliases/alignment,
independent nested State calls, and unchanged real-script registration execution.
The original ARM fixture executes the entire92/408B callers and the actual116B
VFTable tree search/boolean result over retained original-layout nodes. Source
flag stores are observed directly. hashString316B remains an explicitly named
unsigned hash fixture dependency; original LuaVM/tree construction is not
claimed by the caller proof. No membership result or numeric flag branch is
mocked. Native lifecycle integration and full original VM parity remain separate.
