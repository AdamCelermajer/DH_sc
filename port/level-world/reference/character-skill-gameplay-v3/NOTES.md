# Whole retained player skills V3

This is an additive versioned replacement for a live player's V2 authority,
not a second player VM/FSM/timer/property sidecar. Frozen V2, return-V1 and their
reports remain unchanged. The original ARM32 library is an offline oracle only.

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Canonical cache ZIP SHA256: `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

## Native ownership and initialization

`CharacterPlayerSkillsV3` owns one `CharacterScriptSessionV3`, one native skill
instance owner, one Buff owner and one constructor-backed SkillAIStateV3.
The Session owns the private VM, ScriptOwnerV2, actual property sheets, timers,
temporary properties and pinned GameDesign backing. Saved skills/Faeries and
Skill/Faery tables are retained. The real renderer NativeFsm24 and same CharAI
event/flags projections are borrowed; there is no new FSM.

Caller eligibility remains outside this owner. `initialize(final)` executes
the genuine LoadNInit active guard, source loading/OnInit/publication, then the
mandatory vitals provider, full skill/spell configuration and updates, selected
Post and optional Final. InitVCB publishes actual writable flags on this same VM.
Borrowed providers/NativeFSM/AI projections must remain stable through calls and
VM finalizers. Session.close runs before Buff destruction while properties,
timer/design/saved/table backings are still pinned.

The optional cached-file service delivers exact borrowed bytes/found status;
the genuine ZipAssetPackV1/CharacterScriptAssetsV1 owns ASCII-casefold archive
lookup. Session does not guess class aliases or filesystem paths. The audit
loads all 219 original scripts, rather than the old scoped snapshot that omitted
existing Faery scripts. Actual absent spell scripts stay missing. Common files
are not duplicated into include aliases.

The selected player row is required to have script `__player__`. Source player
classification uses the genuine AI row type: type1 true, type0 the source
15-byte `PlayerCharacter` prefix, others false. Skill/list selection reads the
raw resolved properties28/29 (fallback3/0); V3 does not copy the older V2
uncached selection policy. This correction leaves frozen V2 evidence historical.

## Complete source commands and callback choreography

The `lifecycle`, `helpers`, `focus`, `dispatch`, `connections` captures contain
complete routines and hashes. `dh2_character_skill_ai_v3` implements:

| Operation | Original entry | Native behavior |
|---|---|---|
| Usable | 3d8358 | UsingSkill/flag8000, CastingSpell, signed script step>6, slot guard, real CheckUsable |
| Active | 3d85d4 | Source slot access, UsingSkill/current match, fresh slot and CheckActive |
| Begin | 3d86bc | Captured skill row, type1 active toggle/Pre, else Usable then cc/d0/d1 writes, SetSkillState, player/trophy path, fresh UsingSkill |
| End | 3d8474 | UsingSkill, type2, continued0 sets last1; otherwise actual StopLoop(true) service |
| Use | 3d8868 | Begin followed by End when Begin succeeds; not a direct OnSkill call |
| Cancel | 3d84e0 | Fresh slot, type1, CheckActive then fresh Pre |
| Focus/Event/Blur | 3d808c / 3d8bf8 / 3d8b7c | Live current index/slot then Pre/Use/Post |

Original CharAI construction initializes cc=-1, d0=0, d1=0 (constructor capture
in `reference/prince-live-ai`). These retained fields are exported by
`CharacterPlayerSkillsV3::skill_ai()` for the same live state/animation handlers.
They are not reset on every command or callback.

Begin retains its original row pointer across Check callbacks, but reads that
row's action byte after them. It captures TrophyManager before uncached property
216, then walks the exact case-sensitive `epic_withskills` names before Unlock.
Synchronous mutations/reloads are tested. Deeper SetSkillState/Animator/trophy/
network effects are typed required services, not accepted defaults.

The five actual CharAISkillScript bodies (Pre3da8b8, Use3da794, Post3da6c0,
CheckUsable3da9dc, CheckActive3db16c) construct results, capture first active,
SetSkill, erase nonempty results, reload owner/active, call, convert the selected
return and release. CheckActive uses return1; CheckUsable uses return0. Pre/Use
empty results mean true; Post ignores source Call status. Provider failures
preserve the completed prefix and do not publish a false success.

`script_runtime_return_v3.c` includes frozen `script_runtime_return_v1.c` once
and adds indexed observation after one real protected Call/all return projections.
No repeated Call, raw lua_State exposure, offset trick, or generic reentry is
used. Return Values are fully projected/released by the owned runtime before the
observer; adapter tokens preserve caller erase/release choreography. Existing
busy/scope/status/stack semantics remain. Nested gameplay that requires a second
busy VM call still fails explicitly; full SkillFSM6 reentry is not established.

## Buff, class and timer authority

CreateBuff3b86a8/RemoveBuff3b842c guards/coercions are native; owned Buff storage,
class application, source replacement/removal and property recomputation reuse
the actual native Buff/property kernels. Uncached ApplyClass3e2e20 writes the
resolved sheet with real recursive class arithmetic and ordered Buffs, rather
than inventing a stat multiplier. Only the discarded regen debug query is
excluded from its original arithmetic oracle.

Null DropAnimatedFX494978 follows its recovered early return. Nonnull FX
creation/removal requires real caller delivery. An unavailable FX catalog is
represented by UINT_MAX and its reached nonnumber guard fails, rather than
pretending the catalog is empty. Numeric FX IDs bypass that original count guard
and still require actual creation effects. Full visual Buff effects and failure
cleanup for nonnull FX are not claimed by this zero-FX lifecycle proof.

SetSkillCooldownTimerId3b97e0 repeats Value.getNumber after the nonnumber range
guard; that guard uses the live selected list, not the constructed vector count.
Numeric indices bypass it; unsafe later indices fail instead of dereferencing
an assertion-unsafe original pointer. Nil timer writes -1; numbers use source
unsigned IEEE conversion then bit-preserving signed storage. Spell writer
3b90e4 captures the real Faery-list count once and writes each nonnull slot.

Native Session `start_timer/stop_timer` and Lua StartTimer share the same owned
TimerStore, allocator, lowest-free ID, elapsed, blocked and expiry machinery.
Timer35 goes through full CharAI::RaiseAIEvent -> actual OnScriptTimer3d0ca0 ->
selected IPhone AIS3dcc80 -> this same VM alias. Concrete _Timer vtable966940
selects GetID3db288, which reads original+4; native Timer32.id is its projection.
Abstract Timer vtable966950 has a null pure GetID slot. `timer-id/probe.json`
executes the actual GetID over256 unsigned IDs. Services accept only identities
owned by the current retained TimerStore and the genuine callable key.

Buff timer36 uses the actual owned BuffExpired path. Other AI/DoT timer effects
are required services. There is exactly one caller dt advance; full Character
frame order remains outside this module.

## Complete-cache Faery path

GetCurrentSpellInfo3b6e30 uses frozen CurrentSpellV1 and this same saved/Faery
owner: fresh selected difficulty, first-ID validation, selected difficulty
again, then second-ID saved level with another fresh difficulty read. Null SG
returns source0/-1 without a difficulty read. Actual constants `FaeryTypes/COUNT`
are read twice in order, raw property29 selects/falls back to list0, and the
actual row type word8 must match the ID. Equipped element3b6dc4 returns rowword2;
equipped level3b6df8 follows its own selected/saved-level path without inventing
the validation step. Unknown assertion domains fail delivery. No copied tier0,
fabricated spell Info, or class-suffix filename fallback is used.

## Evidence and scope

Optimized ARM64 comparisons on one final oracle cover callback1920/6212 ordered
services, AI2304/3629, Buff wrappers704/434, cooldown writers1024/334 live-list
requests, Faery queries768/1536 and class2080/1,863,680 owner words: **8800 cases**,
zero mismatches. Source GetID256 is separate original-only evidence.

Private sanitizer composition replays AI and callback gold, all source return
cardinalities, required-service prefixes and malformed guards. Actual VM tests
load all219 scripts, initialize three class sessions/19 source skill instances,
query9 same-owner Faery paths, compare42 Info results/9408 original sheet words/
84 formatted strings, run15 passive callbacks and18 AI callbacks, and preserve
saved/table/cache backings through a real finalizer. Nonzero Hardiness,
StaffMaster and Acrobat apply/replace/remove their genuine owned Buffs above
the real Faery-created baseline.

Actual shared SetSkillCooldown with explicit test duration25 creates Timer35,
writes the same instance and expires through source AI/AIS/private VM, including
strict elapsed and blocked-clock checks. The test explicitly stops separately
produced AI/DoT timers whose effects are not implemented. Saved levels, duration,
difficulty, source flags and Idle FSM are declared test inputs; Focus/Event/Blur
use a controlled current index. This is not full active-skill FSM/combat/gameplay,
fresh campaign, frame, Android or GPU proof. Charge Info correctly fails its
reached missing HasShield provider. Host ASan/UBSan/LSan findings are zero.

## Central integration

See `integration.json` for exact production/test selection and arguments.
World adds the12 V3 CPPs; existing ScriptOwnerV2, current-skillV2,
playerVCBV2, CurrentSpellV1 and ScriptAssetsV1 remain shared dependencies. Runtime
**replaces** returnV1's TU with returnV3; never compile returnV3 beside returnV1
or core script_runtime.c. No frozen source needs editing. Link the existing
game-data, runtime, UI formatting dependencies; actual cached ZIP owner uses zlib.
Do not activate both PlayerV2 and PlayerV3 for one live Character. Route renderer
timer start/stop/update into SessionV3 and borrow its existing actual NativeFSM,
CharAI owner/flags and live difficulty. Hook animation/FSM providers explicitly;
unresolved target/combat/skill-state/FX effects must keep required failure.

Reproduce private runtime first with
`python port/level-world/tests/character_skill_gameplay_v3_runtime_host.py`, then
player/AI/callbacks with
`python port/level-world/tests/character_skill_gameplay_v3_player_host.py`.
These compile private DSOs only. The host report binds compiler inputs and
pre/post hashes of coherent main120 transitive dependencies, private runtime,
formatting and original ZIP. Main120 provenance remains historical; the report
does not claim central/current APK bytes. Root owns actual central/NDK integration.
