# Runtime source skill cast preparation

`runtime_skill_cast_prepare_v1` resolves an authored `NativeHUDSkill` slot
through the same `CharacterState.skill_slots` assignment, source SkillList
order, CharacterTable/SkillTables rows, and retained CombatSession actor. Its
mana preparation uses the original `CalcManaCost(CLASS_ID, rank)` class formula
and same actor `OriginalCombatProperties.sheets`; source HasMana/UseMana
bypass facts are explicit. A successful debit changes source property 41,
publishes it through the same `PlayableActorWorld`, and synchronizes the same
ActorState and CharacterState MP projection. Insufficient MP leaves all three
unchanged. `mana_committed` describes only this source prefix, not a successful
cast.

## BashDown source evidence

The supplied v1.0.3 gameplay reference is
<https://www.youtube.com/watch?v=z_Zky7qQdYs&t=282s>; the local source cache is
v1.0.2, so version differences remain possible. Direct frame observations in
`docs/TARGETING-SKILL-REFERENCE-2026-10-06.md` show Warrior windup/facing at
4:41.995, selected-enemy ring and sword orientation at 4:42.495, impact flash
at 4:43.278, target HP reduction by 4:43.995, then a streak at 4:46.328 and
damage text plus reduced mana at 4:46.995. The video does not identify
BashDown; connecting those frames to this specific skill is an inference.

The cached `prince_warrior_bashdown.luac` and
`port/level-world/reference/character-skill-state-v4/skill-lua-v46.json`
show the source sequence: calculate cost and test HasMana; search
Enemy/AttackableOnly with FrontalFirst and range 160; retain the selected
Lua-local target; LookAt it; spend mana; at `do_skill`, set `SnS_Level`, apply
`Skill_Warrior_BashDown`, then call `SkillCombatRoll(target)`; Post clears the
target. Dispatch is NativeHUDSkill → Character::_DoSkill (`0x3b8bd8`) →
CharAI::AI_UseSkill (`0x3d8868`); AI usability (`0x3d8358`) calls authored
OnSkillCheck_Usable (`0x3da9dc`). Those native FSM/script admission owners are
not connected to the modern Session.

## Same-session prefix and focused test

`runtime_skill_target_query_v1` implements the character-only source target
subset over the caller's active population order. It uses same-world actor
position, heading, cached target center, Enemy/alive/targetable predicates,
stealth properties 198/199, and BashDown's range and FrontalFirst ordering.
Character interaction radius is looked up from the same actor's resolved AI
row (`resolved[1]`) in `world.factions()`, through original `ai_props`
semantics including invalid-ID fallback row8. The selected Lua-local target is
not written to `ActorState::target_id`. The LookAt adapter runs the recovered
point-math leaf against that same Session actor.

The connected session test exercises the mana prefix, target query, source
AI-row radius and fallback, and the authored BashDown `do_skill` marker. At
the marker it carries actual SkillTable row7 (`Flags=0x00335535`,
`ElementalType=-1`), SkillAttack mask bit `0x08000000`, the fixture's source
weapon category (`-1` for its empty equipment set), and same-actor rank1
`SnS_Level` / `Skill_Warrior_BashDown` formula scratch into
`CombatSession::apply_source_result`. The test observes positive target HP
removal and verifies a duplicate occurrence does not consume more RNG or
change health. This uses same-world calculation and `ActorCombatRuntime` basic
health plus existing injury/death behavior. It is a connected one-hit prefix
test, not complete source cast acceptance.

The cast preparation result now exposes `source_has_mana_check`, the
source-equivalent HasMana bool from BashDown's authored `OnSkillCheck_Usable` callback. The
connected Warrior slot0 fixture verifies both a real insufficient-MP false
result with no mutation and an exact-cost true result before its later Pre
UseMana debit. This output covers that Lua callback only; it does not claim
CharAI's outer using/casting, populated-instance, or callback-availability gates.

## Remaining source owners

The current fixture supplies accepted admission and target visibility,
zone/interactivity facts explicitly. Production providers for original
CharAI/FSM/script admission, modern visibility/zone/interactivity, and
non-character ObjectManager target enumeration are not established. The
native LookAt controller admission wrapper is also absent. `apply_source_result`
does not reproduce the full `F_ApplyResult` tail (DOT/leech/knockback/stun/slow,
native player/enemy routing, FX/AI/audio/trophy/text). Therefore full casts
must remain unclaimed until those reached source owners are supplied and
executed; a same-world animation or one-hit result alone is not a cast receipt.

## 2026-10-10 evidence note

Before the application/radius adapter change, the cached source handoff
identified `Character::_SkillCombatRoll` at `0x3b9fbc`. The V6 wrapper resolves
the Lua target, distinguishes Character from source object, reads the Skill
row, passes its mask/element to calculation and application, and repeats for
main/off-hand only when row bit `0x00800000` is set. BashDown row7 has no such
paired-hand bit, so it produces one result request. The native target owner
`character_world_target_owner_v1.cpp::search_service` obtains Character radius
from `ai_props(aiTables, resolved[1])->interact_radius`; this feature now uses
the same source AI lookup. The observed video frames above are reused evidence,
not a claim that the footage depicts BashDown.
## 2026-10-10 Act1 lane 11 admission audit (before admission-result change)

**Visual evidence reused.** `docs/TARGETING-SKILL-REFERENCE-2026-10-06.md` directly records 4:41.995 windup/facing, 4:42.495 selected-enemy ring/sword orientation, 4:43.278 impact flash, 4:43.995 target HP loss, and at 4:46.328/4:46.995 streak, damage text, and reduced mana. The footage is v1.0.3 while local scripts/tables are v1.0.2 and does not name BashDown; matching the filmed sequence to this authored skill is inference.

**Logic evidence.** The plaintext cached `prince_warrior_bashdown.luac` has `OnSkillUpdate_` set `mana_cost = CalcManaCost(CLASS_ID, slvl)` and `OnSkillCheck_` return `HasMana(mana_cost), false`; its Pre callback searches Enemy/AttackableOnly, FrontalFirst, range160, LookAt, and `UseMana`; the `do_skill` callback applies `Skill_Warrior_BashDown` then `SkillCombatRoll` once. Native dispatch routes Character::_DoSkill `0x3b8bd8` → CharAI::AI_UseSkill `0x3d8868`; AI usability `0x3d8358` checks source FSM using/casting and instance presence before calling callback `skill_check_usable_v3`; the callback then reaches this script. Generic same-Session code can calculate the source cost and HasMana result and perform the UseMana property41 debit. No production source Character/FSM/VM loan is part of this Session fixture, and the query receives visibility/zone/interactivity facts from explicit test inputs.

**Expected behavior/test defined before implementation.** One selected Warrior hotbar slot0 should preserve saved-row→class-list-position0→BashDown row7 identity. An insufficient source property41 must reject HasMana without mutation. A sufficient exact-cost state should yield the authored OnSkillCheck result, then the existing connected fixture should preserve FrontalFirst selected target order, apply row7's source result to same-world HP once at `do_skill`, and suppress duplicate occurrences without extra RNG/HP effects. This source callback result is not to be mislabeled as complete CharAI cast admission.

**Boundary.** Complete admitted casting is conditional on production sources for CharAI using/casting/FSM and live skill instance/callback, plus same-actor visibility, zone/interactivity, and source non-character target enumeration when the character subset is empty. Missing gates remain unknown; accepted/rejected fixture values alone do not prove runtime providers.

## 2026-10-10 authored multi-target and Hotty event evidence

**Logic evidence reused before the coordinator expansion.** The original cached
`prince_warrior_ground_slam.luac` sets `SnS_Level`, calculates `RANGE` from
`TempProp1`, calculates `SnS_Cooldown` from the authored `Skill_Warrior_GroundSlam`
class, and computes `F_RANGE` from `TempProp2` but never passes `F_RANGE` to the
search. Its Pre callback calls UseMana then SetSkillCooldown. Its `OnSkill_`
searches Enemy/AttackableOnly with ClosestFirst using `RANGE`, applies the class,
then LookAt's only the first target and calls SkillCombatRoll in retained order.
`prince_warrior_charge.luac` uses `Skill_Warrior_Charge` for cost/cooldown,
clamps negative cooldown to zero, performs a FrontalFirst range-300 Pre search
and LookAt when nonempty, then UseMana and SetSkillCooldown. Its OnSkill re-searches
NoSort with range 300 and angle 120, applies the shield-bonus class only when
HasShield is true, and rolls the resulting target list in source order. Neither
script gates Pre on an empty Character search; a verified empty Character-only
subset may still spend mana/cooldown and later complete with zero Character hits.
The original `SetSkillCooldown` helper starts a timer only for positive delays.

**Hotty source event correction.** `combat_anim_audit` traced NativeHUDSpell
admission function 50006 and the state7 Cast route. `50006` is admission only;
retained numeric event 40 (`do_spell`) in state7 dispatches the authored `OnSkill`
body, while focus 32 and blur 33 route OnPreSkill and OnPostSkill. This supersedes
the earlier uncertainty in the Hotty report. The coordinator's new typed event
handoff requires caller-provided state6/`do_skill` or state7/`do_spell`; it never
aliases Hotty to Warrior's marker. The current feature API requires the same
EffectsFactory/session, exact EffectsTables rows/IDs, source Hotty prep data and
same lease/generation. OnPre dispatches Player_Pre only after the mana/cooldown
prefix; Use applies each retained target and its positive-return target FX in
source order. Empty list skips Player_Pre and has no target result. FX failure
preserves the reached combat prefix. The BDAE assets are absent, so this proves
effect instance dispatch, not rendered/packet output.

**Expected behavior and verification.** The connected runner now passes a real
source-trained GroundSlam row in a clearly labeled advanced saved-character
fixture. It derives level/points from Knight CharacterTable/ClassTables,
trains the actual SkillList position through `train_skill_v1`, assigns saved
source slot1, roundtrips through SaveStore, then runs the exact GroundSlam root
and same-session `do_skill` Use/Post. The test observes its source ordered
target list and one result per target, MP debit, HP/RNG changes, unchanged saved
rank/points, actor completion and duplicate-marker suppression. It also keeps
fresh-player rank-zero GroundSlam/Charge rejection and BashDown slot0 coverage.
The slot-matrix follow-up now also covers actual Charge success; full
`F_ApplyResult` tails remain outside this subset.

The same test now calls `build_runtime_skill_animation_bank_v1` before
`CharacterVisual` initialization. With `preload_current_class_roots`, it
resolves the same CharacterState's active source SkillList and compiles each
nonnegative `SkillTable.Anim` root independently of saved rank or assignment.
Assigned roots compile first and remain required; an unlearned optional root
with missing source clips is retained with a per-position diagnostic without
invalidating the assigned roots. The builder can also read the same actor's
property2-resolved CharAnimTable `Spells[4]` root for the Faery Cast bank.
`merge_runtime_skill_animation_bank_v1` adds the exact compiled clip config,
sequence states and source policies to the base actor plan; the request's
hotbar entries carry selection state per original saved slot. The connected
Warrior test verifies that rank-zero/unassigned GroundSlam and Charge roots are
preloaded before the Session's CharacterVisual initializes. It then trains
and assigns both on the same CharacterState and already initialized Session,
freshly resolves each saved slot against the preloaded root, and casts without
a visual reload. It removes slot1, roundtrips the resulting slot0/slot2 state,
rejects slot1 without mutation, and proves slot2 still selects and casts
Charge. A restricted-asset fixture omits optional clips and verifies explicit
per-entry diagnostics while the assigned BashDown root remains loaded. The
fixture does not claim full `F_ApplyResult` tails or production hotkey wiring.
Current-Faery
record/script selection remains the separate Faery source owner; this builder
does not pick a Faery row or claim `NativeHUDSpell` admission.

Focused C++17 strict syntax checks and the private linked session runner pass
against refreshed read-only archives.

**Uncertainties/boundaries.** `PlayableActorWorld` does not expose the original
non-Character object target list, and object-query result order remains outside
this Character-only helper. The effects table's referenced BDAE assets are absent
from the current asset set. Shared `apply_source_result` is a same-session source
result/application subset, not the entire original F_ApplyResult tail. The
production state7 dispatcher must invoke `apply_retained_use_event_v1` with the
real retained event and its state identity; a synthetic event is not acceptance.

## 2026-10-10 source animation bank and key-slot audit

**Pre-initialization bank.** `runtime_skill_animation_bank_v1.hpp/.cpp` exposes
`build_runtime_skill_animation_bank_v1` and
`merge_runtime_skill_animation_bank_v1`, plus
`resolve_runtime_skill_animation_slot_v1` for fresh dispatch-time selection.
The builder requires the exact
CharacterState, CharacterTable, pinned SkillTables, AssetCatalog,
AnimationTables, animation dictionary, actor visual/role and equipment set.
It does not infer a class ID or invent an animation root: each populated
NativeHUDSkill slot resolves through the source saved assignment and source
class-list row to `SkillTable.Anim`; the optional Faery Cast animation root is
the actual same-actor CharAnimTable field31 (`Spells`) entry at source bank
slot4. The caller supplies the same property2-resolved CharAnimTable index.
Each source root is compiled by the existing animation program converter,
which retains original sequence type/loop, redirects, step metadata and URI
resolution. A build/merge failure leaves output untouched.
The optional class-root list records active list/position/table identity,
loaded status, and any source asset failure. `resolve_runtime_skill_animation_slot_v1`
re-reads the current saved assignment/rank each time and maps it to the already
loaded class root; callers must not reuse the pre-init `hotbar` snapshot after
training or remapping.

**Key/slot and mapping-circle route.** The original authored gameplay HUD
contract records `NativeHUDSkill(slot)` slots0/1/2 in
`port/engine-ui/authored_gameplay_hud_v1_handoff.md`; the live native bridge
passes that numeric argument unchanged to `player_gameplay_action`, whose
skill branch reads `Save::skill_in_slot(index)`. Android/source slot IDs remain
logical `[0,1,2]`.

The PC adaptation follows the authored Skills-page mapping-circle geometry.
Source action `refreshUsedSkills` in
`port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`
(`sprite496`, action root `0x20385`) iterates logical `slotId=i` and places
`btn_activeskill03`, `btn_activeskill01`, `btn_activeskill02` left-to-right.
The matching `original_skill_art.cpp` positions are approximately x=283.2,
335.85, and 389.85: physical left/middle/right therefore show logical source
slots `[2,0,1]`. The local Skills-page capture
`.local-inputs/v19-frontend-hotfix/current-gui/rogue-skills.png` has the only
assigned Rogue icon in the middle mapping circle (logical slot0); the gameplay
HUD capture `.local-inputs/publication/checkpoint/port/android-native/reports/native-skill-world-final/gameplay.png`
shows a separate left-to-right source-slot layout and is not evidence for the
PC keyboard adaptation.

The explicit PC adaptation is key1/2/3 → visible left/middle/right → source
slot `[2,0,1]`. `pc_skill_number_to_source_slot_v1` is the single conversion
helper; callers must use it once before building a cast request and must not
also permute SemanticInput bindings. Thus PC key1 rejects the empty Rogue
source slot2, while key2 selects the middle-circle source slot0 JumpKick row0.
This is a user-requested PC adaptation, not an original native keyboard rule;
the Android/native slot argument and saved indices are never rewritten.

**Focused test.**
`run_runtime_skill_activation_session_v1_tests.ps1` links the coordinator and
animation-bank feature against the refreshed foundation archives in a private
output directory. The passing fixture checks unlearned current-list root
preloading, missing optional clip diagnostics, training/assignment after
visual initialization on the same CharacterState and Session, fresh slot-to-
root resolution, level-3/level-6 SaveStore roundtrips, empty-slot1 rejection
without shifting, the optional `Spells[4]` root, and same-session `do_skill`
Use/Post for the three assigned slots. Its Rogue regression now verifies the
PC visual mapping adapter against actual saved source rows: key1→empty slot2
rejects without mutation and key2→middle-circle slot0 reaches JumpKick
row0/root521. It does not exercise the root's
production caller or a live keyboard event.

**Current focused-run result (2026-10-10).** The private activation-session
runner prints `PASS PC visual mapping regression: key1 rejects empty source2;
key2 selects middle-circle source0 JumpKick row0/root521.` The full runner then
exits nonzero in its later Warrior block at `GroundSlam did not retain the
source result/outcome for each ordered target`; that failure is outside this
PC mapping assertion and remains open. The PC helper/connected Rogue branch
therefore has a focused observed pass, while the full activation-session suite
is not green in this run.

The isolated semantic-input contract runner is
`features/generic_skills/run_pc_skill_input_binding_v1_tests.ps1`. It presses
the actual default `SemanticInput` keys 1/2/3, checks each `Frame.skills[i]`
edge, applies `pc_skill_number_to_source_slot_v1(i+1, ...)` once, and confirms
the mapping `[2,0,1]` while saved `CharacterState.skill_slots` stay byte-for-
field identical and native logical slots remain `[0,1,2]`. Result:
`pc_skill_input_binding_v1_tests PASS: physical keys1/2/3 -> source slots2/0/1
once; saved rows and native slots unchanged`. This is a feature contract test;
the production caller continues to make its single conversion immediately
before building the cast request.

## PC gameplay HUD frame and hit projection

The source menu's authored mapping panel is not the PC gameplay HUD. In
`character-menu-flow-v1/authored-actions.txt`, `refreshUsedSkills` iterates
logical source slots while placing `btn_activeskill03`, `btn_activeskill01`,
and `btn_activeskill02` left-to-right, yielding physical source order
`[2,0,1]`. `original_skill_art.cpp` records the corresponding authored x
positions. The inspected Skills-page capture shows JumpKick in the middle
mapping circle, while the gameplay HUD capture shows a source-slot-0 icon in
the leftmost gameplay cell. The original HUD has no PC key labels; labels 1/2/3
are the explicit desktop adaptation and follow physical left/middle/right.

`pc_skill_hud_projection_v1.hpp` exposes
`project_pc_skill_hud_v1(...)` and `pc_skill_hud_key_for_hit_v1(...)`. The
frame builder consumes the same `CharacterState`, source `CharacterTable`,
`SkillTables`, and logical source-status values; it projects cells in physical
order `[2,0,1]`, reads the exact saved row/rank and source skill/icon identity,
and passes unknown cooldown/usability through as unknown. A current cast
receipt is shown only when actor, equipment set, source slot, and saved row
match. After the existing HUD shape traversal reports a physical cell hit, the
hit helper emits PC key number 1/2/3. Main's existing single key-to-source
translation remains the only mapping step; SaveStore rows, Android logical
slots, and the CharacterState are not reordered.

The focused source-backed runner is
`features/generic_skills/run_pc_skill_hud_projection_v1_tests.ps1`. It loads
the actual Rogue source tables and verifies left/middle/right key labels,
source identities `[2,0,1]`, the middle-cell rank-1 JumpKick row/icon and active
cast receipt, shape-hit key emissions, one existing mapper application, and
unchanged saved rows. Result:
`pc_skill_hud_projection_v1_tests PASS: actual Rogue SkillTables row/icon;
left/middle/right labels1/2/3 -> source slots2/0/1; middle key2 JumpKick;
shape hits emit keys once; saved rows unchanged`.

This feature supplies a callable projection and hit contract. The production
renderer still owns visible HUD layout and must feed its actual shape-hit
position into the helper; no production HUD labels or pointer-hit wiring is
claimed by this test.

## Rank-zero source animation-root preload

The animation bank is a pre-`CharacterVisual` consumer of the same source
`SkillList` and `SkillTable.Anim` rows; it prepares available authored clips,
not gameplay skill admission. A fresh direct profile can already carry a known
source slot→saved-row mapping while that row remains rank zero, so resolving
every hotbar slot as a learned skill before enumerating the class roots rejects
the preload too early. The active source list must be resolved first. When
preloading is requested, a rank-zero saved row is accepted only if its exact
saved row and skill token match that active list position; it is omitted from
the active `bank.hotbar` projection, while its original nonnegative Anim root
is compiled by the existing all-class-root pass. The ordinary learned-slot
resolver is unchanged: before a source rank grant it still rejects, and after
the same-state source progression update it rereads the row/rank and resolves
the already-preloaded root. Without current-class preloading, a rank-zero
binding still rejects. A mismatched source row remains a hard failure.

There is no new gameplay animation or timing claim: this is invisible bank
preparation for the original selected-skill playback path. Existing gameplay
HUD captures show that the selected authored skill is the one expected to play;
this focused check does not render a new frame. The feature test uses the
actual Knight `CharacterTable.SkillTree`, SkillList, SkillTable.Anim root, and
AnimationTables assets, then runs the same `train_skill_v1` source progression
gates and verifies the rank-zero reject→rank-one fresh resolution. The private
strict C++17 runner is
`features/generic_skills/run_runtime_skill_animation_bank_rank0_v1_tests.ps1`;
result: `runtime_skill_animation_bank_rank0_v1_tests PASS: source rank0 slot
omitted from active hotbar; exact SkillList root preloaded; source training
then resolves row0/rank1; no-preload and mismatched rows reject`. The production
direct-preload integration still requires lead's coherent build/caller check.

## 2026-10-10 Rogue JumpKick source cast audit

**Visual evidence.** The inspected local capture
`.local-inputs/v19-frontend-hotfix/target-details/rogue-skill-details-selected.png`
shows the authored Rogue Skills page selected on “Heel Strike,” with its
description, current/next level, and slot-mapping art. It is a page capture, not
a gameplay sequence; it does not establish impact timing or target behavior.

**Logic evidence.** The exact cached
`.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/skills/prince_rogue_jump_kick.luac`
is readable source. Its authored script declares `Skill_Rogue_JumpKick`,
`RANGE=160`, `INIT_RANGE=250`, `ANGLE=270`, rank-derived `CalcManaCost`,
`HasMana` Check, and Pre order Enemy/AttackableOnly + FrontalFirst search at
250, `TargetListBackup("MaxAngleSearch")`, optional top-target `LookAt`, then
`UseMana`. Its `OnSkill_` searches at range160/angle270 with MaxAngleSearch; if
the list is nonempty it sets `SnS_Level`, applies the class, and drains each
target through `SkillCombatRoll`. The authored `bloodsplat` FX call is commented
out. The shared native state6 route is `Character::_DoSkill` →
`CharAI::AI_UseSkill`; the retained `do_skill` event dispatches the same active
skill's Use callback. The source animation bank resolves the loaded Rogue base
SkillList position0 to JumpKick row56 and exact Anim root521.

**Expected behavior and verification defined before implementation.** On the
same modern CharacterState/CombatSession, a fresh resolver must map logical
source slot0 to saved row0/rank1 JumpKick and root521. The retained source
sequence must reach its real `do_skill` event, apply same-session result receipts
to the exact source-selected targets, and complete through Post. Logical slot1
has no binding and must reject before changing MP, HP, RNG, heading, action, or
retained pose. It must not read the visible middle cell as a slot index or fall
back to a Knight skill.

**Uncertainties.** The local script/tables are cache v1.0.2; the inspected
gameplay-oriented video notes elsewhere are from v1.0.3 and do not identify
JumpKick, so no visual gameplay parity claim is made. Generic result application
does not reproduce every native `F_ApplyResult` tail. Since `bloodsplat` is
commented out in this script, the feature does not invent a cast FX.

**Connected verification.** `runtime_skill_activation_session_v1_tests.cpp`
also builds a distinct Rogue profile from the actual CharacterTable and source
SkillList, round-trips the learned row through SaveStore, resolves slot0 afresh
to saved row0/rank1 and root521, then runs the retained `do_skill` event on the
same CombatSession. The fixture checks the exact retained target order against
the two applied result receipts, positive HP changes, source mana debit, Post
completion, and no-op duplicate suppression. Its logical slot1 empty rejection
is checked before mutation; the displayed middle cell is never interpreted as
slot index1. The strict C++17 private linked runner passed with the coherent
foundation archives. This establishes the implemented generic same-session
result path, not all native `F_ApplyResult` tails or a visual gameplay match.

## 2026-10-10 production population-query correction audit

**Observed production evidence.** The isolated production capture
`.local-inputs/v19-frontend-hotfix/profile-projection/rogue-key1.log` reports
33 enabled population visuals but only 25 actors in the shared Session world.
Its key1 request correctly resolves the source slot, but the target query fails
on the first render-only population identity (`Population actor is absent from
this Session world`) before spending mana. Key2 independently rejects the
empty source slot with MP unchanged. This confirms that render-population order
is broader than the registered combat-Character domain in this host.

**Source/implementation evidence.** The recovered Character target search is
the `CharacterList` path in `character_world_target_owner_v1.cpp::search_service`;
the generic target-query helper already restricts its candidates to live
`CombatSession`/`PlayableActorWorld` actors and uses that world's source
properties. The rendering population includes visuals that have no corresponding
combat actor. No original visual asset or scene animation behavior is changed;
this is a same-world query-domain correction. The modern Session actor registry
is the authority for which ordered population entries can participate in its
Character-only query.

**Expected behavior / focused check.** Preserve enabled population order while
omitting IDs that are not registered in the same Session world; still fail on
invalid or duplicate stable IDs, and retain strict target-query errors for IDs
actually submitted but absent from that world. A connected fixture adds a
render-only visual entry alongside valid Rogue targets and verifies that slot0
continues to select and hit the valid same-world targets in source order, while
empty slot1 remains a no-mutation rejection.

**Uncertainty.** This modern port uses the registered Session actor set as its
combat Character domain. Non-character GameObject targets remain outside this
query and are not inferred from render-only visuals.

## 2026-10-10 current-Faery Celest coordinator audit

**Visual evidence.** The available `.local-inputs/v19-frontend-hotfix` image is
a final scene capture, not an original Celest cast sequence; the existing
Faery evidence note records that no original Celest gameplay footage or
screenshot is present. The source resource names establish authored identity
only, not live visual timing.

**Logic evidence.** Reuse
`features/faery_menu/celest_source_evidence_v1.md` and its exact
`.local-inputs/character-skill-session-v2/cache/data/scripts/skills/faerie_celest.luac`
audit. Fresh profile CharacterTable FaeryList rows (Knight 1, Mage 2, Rogue 3)
all select list slot0/current_faery 0 → actual row1 Celest / script
`faerie_celest`, even with saved unlock state zero. The same decoded table
shows each profile list's slot4 is row7 `Hotty` / `faerie_hotty`. Its separate
`DEFAULT` menu list slot4 is row3 `Fake_Hotty` with an empty SpellScript; that
menu-only row is not a gameplay alias or fallback. The original
`NativeHUDSpell`/`AI_BeginSpell` chain enters state7/Cast; retained `do_spell`
(event40) is the sole Use callback. This coordinator already enforces that
state7 marker for the Hotty typed branch; it must choose Celest by the exact
active `FaeryRecord.script`, not by a Hotty default or Save unlock flag.
Celest OnPre commits mana/cooldown then calls Player FX; the Use loop invokes
two SpellCombatRoll calls per Character target and then unconditional target
FX. The helper currently preserves a partial receipt if the first roll kills
the target because the shared Session has no formula-only second occurrence.

**Cast-root correction evidence.** The recovered
`CharStateMachine::SM_SetCastState` (`0x3c6394`) reads the active saved slot's
`CharAnimTable.Spells[save_slot]`; it does not force `Spells[4]`. The existing
bank fixture's `Spells[4]` is the Hotty-current-slot case only. Fresh profiles
have `current_faery=0`, so Celest must use the exact `Spells[0]` root. Expected
bank behavior is to preload the five authored CharAnimTable spell roots before
visual initialization without changing Faery ranks/unlocks, then resolve the
current difficulty's saved Faery slot on every cast. A missing slot root must
stay explicit and reject that cast, never reuse the Hotty slot4 clip.

**Implementation.** `RuntimeSkillAnimationBankV1` now compiles every exact
nonnegative `CharAnimTable.Spells[0..4]` root into the same pre-init visual
bank, retaining per-slot load failures without substituting another root. The
new `resolve_runtime_faery_animation_slot_v1` reads `current_faery` from the
same current-difficulty `CharacterState` on each request and returns only that
slot's loaded Cast state. It does not inspect saved unlock/rank fields or reuse
the legacy slot4 diagnostic. The feature fixture now checks that each stored
slot ID equals the original AnimationTables field and resolves slots0 and4 to
their distinct roots.

The coordinator resolves the active Faery script from the same source FaeryList
row and dispatches only exact `Hotty`/`faerie_hotty` or `Celest`/`faerie_celest`
rows. The menu-only DEFAULT row3 `Fake_Hotty` has an empty script and is
rejected. Celest uses its typed OnPre and state7 `do_spell` Use helpers; Hotty
and Celest cannot substitute for one another. Player_Pre and target FX remain
non-veto diagnostics. Empty target lists still run the authored OnPre and empty
Use loop. The coordinator receipt retains reached roll/effect prefixes and
does not claim complete Celest Use after the unresolved lethal-first-roll
second occurrence.

**Verification.** Strict C++17 syntax passes for the animation-bank,
coordinator, and connected session test translation units. The private linked
feature runner passes against the current Session/World closure, including the
all-slot source ID checks and distinct loaded current slots0 and4. Its link
script includes `playable_actor_world.cpp` for the new Session source-result
calculation APIs. This runner is Warrior/Rogue-only; the Faery owner is adding a
separate retained state7 Celest coordinator fixture. Root's independent production
key1/key2 capture is available at
`.local-inputs/v19-frontend-hotfix/profile-projection/rogue-key1.log` and
`rogue-key1.png` (executable SHA
`CD84E2800DE827AD39FF316296C8273B4D46752FA2131BAE36E7E384E6B749A7`): key1
casts saved slot0 JumpKick and reaches its retained `do_skill` hit; key2 rejects
empty source slot1 without another mana debit. The final PNG is a scene capture,
not evidence of the hit frame or Celest visual timing.

**Uncertainties.** Celest source GameObject targets are outside the modern
Character-only session query. There is no original cast capture. Root has now
added the same-session formula-only occurrence API for a lethal-first-roll
target. The Faery helper now retains that full calculation separately from
applied `DamageEvent` receipts, and the coordinator carries the typed
`CelestDeadTargetCalculationV1` vector into its receipt. The lethal branch still
needs the Faery fixture rerun before it is reported as linked verification.

**Earlier connected population test.** The feature fixture appends an enabled
render-only stable ID (`99`) to the live population after Session
initialization, without adding it to the Session actor registry. Rogue slot0
still reaches same-world target order and applies its source hits; direct target
query supplied ID99 still fails with the stale-actor diagnostic. The empty
slot1 rejection remains unchanged. A successful production key1/key2 capture is
now reported above; it is separate from the feature-only subset fixture.

## 2026-10-10 JumpKick Post target-clear audit

**Source evidence before implementation.** The actual
`prince_rogue_jump_kick.luac` calls `ClearTarget()` unconditionally in
`OnPostSkill_`; source rows identify this script as Rogue JumpKick. The other
currently supported authored scripts BashDown and GroundSlam also call
`ClearTarget()` unconditionally. Charge is different: it calls
`ClearTarget()` only when `not TargetInMeleeRange()`, so it needs that exact
Post-time predicate before it can share the unconditional rule. Hotty and
Celest `OnPostSkill_` are empty.

The original binding is `Character::_ClearTarget` at `0x3b5690`; the recovered
portable source binding in `port/level-world/character_target_bindings.cpp`
routes it through `dh2_character_clear_target`, which invokes
`dh2_character_ai_set_target(state, 0, 0, services)` and then
`dh2_character_ai_sync_last_target`. In the current generic Session, the
corresponding player target is `ActorState::target_id`; the frame's sticky
target observer explicitly drops its cached target when that field is cleared
(`combat_session.cpp` around the clear-between-frames comment). This operation
is distinct from attack completion, whose target restoration remains in the
existing Session path.

The original Post binding fixture
`port/level-world/tests/skill_post_target_v46.cpp` executes authored BashDown,
Charge, and GroundSlam Post callbacks and already verifies unconditional
versus `TargetInMeleeRange`-conditioned target retention. The connected
JumpKick test below sets an existing selected same-world target, reaches its
real retained `do_skill` and Post completion, and asserts the generic target
is cleared; no blanket clear applies to Faery or other skills. The linked
feature fixture passes with this assertion. The existing
`combat_session_target_retention_tests` continues to exercise ordinary attack
completion retaining a valid selected target; its target sticky/restore code is
unchanged by the skill-specific Post projection.

## 2026-10-10 cast interruption and cooldown persistence audit

**Evidence before coordinator changes.** This is lifecycle infrastructure with
no supplied original gameplay capture of an interrupted skill. The recovered
source call chain is used as the behavioral evidence; no visual timing claim is
made here. In the pinned original ELF
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`,
`CharAI::AI_CancelSkill` (`0x3d84e0`) rechecks the current script/skill row and
`OnSkillCheck_Active` (`0x3db16c`) before tail-calling `OnPreSkill`
(`0x3da8b8`). `CharAI::AI_EndSkill` (`0x3d8474`) is a separate transition
through the source state machine (`0x3c948c`); cancellation and ordinary
damage therefore cannot be inferred from an HP change alone. The new modern
Session interruption callback is the accepted-exit authority: only its
explicit accepted interruption may retire an in-flight cast before Use. A
nonlethal ordinary damage result with no source departure must leave the cast
pending. On accepted pre-Use exit, the coordinator is to retire the generation
first, run the source Post at most once, and make any later retained marker
stale before incoming focus proceeds.

**Cooldown state is transient.** The source cooldown writers are
`Character::_SetSkillCooldownTimerId` (`0x3b97e0`) and
`Character::_SetSpellCooldownTimerId` (`0x3b90e4`). They write timers to the
same owned skill/spell instances; the source cooldown fixture verifies a
duration-25 skill writer creates and expires Timer35 through the original AI
timer path (`character-skill-gameplay-v3/NOTES.md`, “Buff, class and timer
authority”). The current `CharacterState`/SaveStore format persists skill
ranks and hotbar row bindings plus Faery current-slot/level state, but it has
no cooldown or timer ID/deadline field. This is consistent with source-owned
timers being runtime state rather than character-save data. The coordinator's
skill timer clock is likewise Session-bound transient state; a replaced
Session lease clears its skill-ready map only when the first update is observed.
A post-load cast must therefore bind to a fresh retained Session timer owner
before admission; it must not import an invented remaining duration from the
save. A still-live same-Session coordinator retains its deadlines. Faery's
cooldown clock must likewise be advanced on each Session update and reject a
foreign/replaced Session clock rather than silently reinitializing it.

**Source focus flags.** The pinned `CSSkill::OnFocus` (`0x3c4480`) writes
`Character+0x520 = 0x6341` at `0x3c44e4..0x3c44e8`, then raises SkillFocus
event `0x1e`, resets animation/speed and cancels sneaking. The exact `CSCast::OnFocus`
(`0x3c39d0`) decompilation in
`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`
writes `Character+0x520 = 25345` (`0x6301`), raises SpellFocus event `0x20`,
then resets animation/speed and cancels sneaking. Neither player policy includes
the monster-only `0x10000` augmentation. The source-session playback policy will
carry state6/`0x6341` for NativeHUDSkill and state7/`0x6301` for NativeHUDSpell;
the root's accepted-departure policy, rather than an HP/damage heuristic, decides
whether those retained states actually exit.

**Focused verification.** The strict connected-session runner
`features/generic_skills/run_runtime_skill_activation_session_v1_tests.ps1`
now executes the additive Session policy. After an actual learned Charge
source cast completes, it waits until the transient timer expires, begins a
second Charge from saved slot2, and cancels its retained state6 sequence to the
same state (a source-approved restart still runs OnBlur/Post). The receipt is
`interrupted`, the real OnPre mana debit remains, and the use/result list stays
empty. A later `do_skill` event is rejected as belonging to a retired
generation; duplicate cancellation is a no-op; HP and combat RNG are
unchanged. Checkpoint is rejected while the sequence or transient cooldown is
active and admitted after timer expiry. The fixture avoids the legacy
`select_actor_state_leaf` helper because that separately registers an
unpersisted campaign lifecycle service; generic completion resumes the
retained Idle owner on the next Session update. It then captures/restores a
GameSave after quiescence, verifies a fresh coordinator accepts the new lease,
and verifies the old transient clock rejects that replaced Session. The
source policy remains explicit: ordinary damage alone does not invoke the
departure callback, and actual accepted departure is the only interruption
authority. No mana refund, forced Use, or post-load timer reset is claimed.

## 2026-10-10 target-position pointer selection correction

This is an internal source-query parity correction; there is no separate visual
event to inspect. It preserves the visible/gameplay invariant that a far
character cannot enter a skill's retained target list because its constructor
cache happened to contain the zero vector, while an authored non-null visual
node can provide its actual cached target point. No new targeting radius or
nearest-target rule is introduced.

The original `GameObject::GetTargetPosition` body at `0x3935dc` is captured in
`port/level-world/reference/character-target-search/reference/original-functions.asm`.
It loads and tests `+0x180` first; null returns `this+0x160` immediately. Only a
non-null node proceeds to the `+0x80` byte test and can return cache `+0x184`.
The target-search notes document this source pointer choice for
`TargetList::SearchEff` at `0x4a3428`. The prior optional cache-first shortcut
inverted that order when a known-null node coexisted with a constructor-zeroed
cache.

The focused fixture uses the same CombatSession actors and target query. Actor2
at transform `{900,0,0}` with a known-null node and constructor-zeroed cache
`{0,0,0}` must be excluded from the 160-unit query. With a non-null node and a
near cached point, it must be included while its transform remains far away.
The LookAt leaf uses that same resolved target pointer and is checked against
the cached point. The strict C++17 connected runner verifies both the BashDown and general
Character query paths; it does not claim a separate original visual capture or
controller LookAt admission.

## 2026-10-10 PC3 trained Charge Use-time cone audit

The frozen production artifact is
`.local-inputs/v19-frontend-hotfix/profile-projection/knight-trained-key3.log`
with arguments in the adjacent `knight-trained-key3.args`. Its source-created
level-6 Knight has Charge row 2 trained from rank 0 to 1; key 3 selects logical
slot 2, spends the source cost (MP 33.5 to 29.5), reaches the authored
`do_skill` event, and records an empty Character result loop. The nearby Lizard
is at Y=1186.97; the player's root-motion clamp at the marker reports current
Y=1235.548, placing that Lizard about 49 units behind at impact. This run also
records 25 actors without a source target node. That is the expected native
null-node state; coherent build F242 (build 12210) compiled the corrected
node-first target resolver, so this artifact uses the corrected getter. The
null-node count is not evidence of the earlier optional-cache behavior.

The bundled plaintext
`.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/skills/prince_warrior_charge.luac`
shows two separate searches: `OnPreSkill_` sets Enemy/AttackableOnly and
FrontalFirst, searches RANGE=300, calls `LookAt(GetTargetListTop())` when
nonempty, then spends mana and sets cooldown. `OnSkill_` changes sorting to
NoSort and re-runs `TargetListSearch(300, 120)` from the caster's current
heading before rolling each result; the Pre target list is not reused. The
source LookAt kernel maps a target along +Y to heading -π, whose target-search
look vector points +Y. After the marker root motion passes the target, it lies
behind that heading and is excluded by the 120-degree Use cone. Thus the frozen
empty loop is source-plausible, not evidence that Use should reuse Pre targets
or widen the cone. The frozen log does not record pre-selection, exact marker
heading, per-candidate angle, or rejection reason, so the production case is
not yet a complete proof.

The focused same-Session regression exercises those exact source stages using
actual eligible actors and source properties: capture Pre's FrontalFirst target
and LookAt heading; move the caster past it; show that a full-cone control still
has the target within adjusted range; then show Charge's NoSort/120-degree Use
query excludes the behind-only case and retains a second in-front actor in
authored population order. No production query behavior is changed. A future
production trace should record the Pre target, Use heading, adjusted distance,
angle, and exact gate for each candidate before any policy change is proposed.
The strict C++17 `run_runtime_skill_activation_session_v1_tests.ps1` runner
passes this same-Session regression: Pre selects actor 2, the captured LookAt
heading is +Y, after the modeled pass a full-cone control still includes actor 2,
the behind-only source Use query is empty, and the exact 120-degree Use query
retains only in-front actor 3 in source order. This proves the query branch's
source math, not the missing production per-candidate heading trace.
