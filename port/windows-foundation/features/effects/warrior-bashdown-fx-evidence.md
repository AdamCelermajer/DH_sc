# Warrior first-skill impact FX evidence

## Visual evidence

The retained original gameplay reference is Dungeon Hunter II v1.0.3, Part 1,
`docs/TARGETING-SKILL-REFERENCE-2026-10-06.md`. I inspected local source-video
frames at 4:41.995, 4:42.495, 4:43.278, and 4:43.995. The player winds up
near a Bogwomp, the target ring and facing point toward it, a short yellow/green
flash appears on the target (the sampled 4:43.278 frame displays `Miss`), and
the target HP later falls. There is no clear ground shockwave in these sampled
frames. The footage does not expose the pressed control or identify its skill,
and it is v1.0.3 while the local authored tables/scripts are v1.0.2; associating
that observed hit with BashDown remains an inference. The exact inspected stills
are under `.local-inputs/warrior-bashdown-fx-audit/`.

## Logic and exact source content

For the fresh base Knight, the first class SkillList position is BashDown: source
SkillTable row 7, name `BashDown`, script `prince_warrior_bashdown`, `Anim=347`
(`Knight_BashDown`). The actual row/asset mapping is retained in
`port/level-world/reference/shared-target-facing-v1/knight-authored-skill-cache-v2.json`
and `all-skill-authored-fx-inventory-v4.json`.

The exact AnimTable root 347 is `Type=0`, `Loop=0`, with one step:
`Anim=1234`, `FX=164`, `AnchorFX=1`, `MoveGO=1`, `Speed=1.3`, `Sound=190`.
Clip 1234 is the authored
`data/3D/characters/prince/animations/skill_dh2_prince_warrior_bash_down.bdae`.
The source skill script describes BashDown as a “melee forward and downward
attack”; its `OnPreSkill_` explicitly comments that “The effect animation is
in animation.pyarray” and leaves a direct `PlayFX(...)` call commented out.
`OnSkill_` applies `Skill_Warrior_BashDown` and calls `SkillCombatRoll(target)`
for the retained single target. Thus the first skill's effect is AnimTable FX
set 164, not a script-level PlayFX. `GroundSlam` is a different SkillTable
entry, with its own animation root and effect set 173; do not substitute it
for the base first skill.

Effect set 164 is `skill_dh2_prince_warrior_bash_down`, `Type=0`, one step,
non-looping, source URI
`data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae`, `OrientWithAnchor=1`,
`ScaleWithAnchor=1`, `SelfIllum=1`, `PlayTime=-1`, `Speed=1.3`. The exact
original BDAE exists in `port/level-world/reference/shared-target-facing-v1/cache/`
and its extracted SHA is `21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483`.
Its authored scene contains `_mesh_cracks_nobatch`, `_mesh_shockwave_nobatch`,
and `_mesh_swoosh_nobatch`, plus debris and dust particle emitters; these names
support the impact presentation, but do not establish their exact visible timing.

## Expected behavior and current seam

At the retained entry to BashDown's source AnimTable step 0, dispatch FX set
164 through the current same-session `RuntimeSwingFxObserverV1` step observer;
let `CharacterMeshFxOwnerV4` run the source effect table/resource/pool path,
advance it using the source FX clock, then submit its retained mesh/particle
packets through the existing render bridge. The effect origin is the current
same-session player anchor because `AnchorFX=1`. Do not emit a fabricated
shockwave or replace the source BDAE.

The feature observer already routes any real AnimTable step's `FX`/`AnchorFX`
fields through `character_animation_step_fx_v2` to `manager.play_set`, and the
generic factory test proves same-session step dispatch and packet retention for
a normal weapon Swoosh. A focused Knight BashDown case is now authored in
`runtime_effects_factory_native_tests.cpp`: it requests root 347 through the
same Session, observes its actual step-0 entry, dispatches set 164, checks the
exact BDAE-derived mesh/particle packets, suppresses duplicate occurrence
delivery, and submits the retained frame. The linked case passes: root 347,
step 0 emitted set 164 for the same Session actor; a fresh manager retained the
exact BDAE, produced three mesh packets and two particle packets, suppressed a
replayed occurrence, and submitted the retained frame. The particle packet
test uses an identity CPU camera fixture and the explicit source-white particle
color branch; it does not claim live camera or GL fidelity. The minimal
production integration seam is registration/composition of this observer with
the retained Session's step-entry presentation observers, followed by same-frame
FX manager update and renderer queue submission. The script's commented PlayFX
is not that seam.

## Verification plan and uncertainty

The strict C++17 actual-table/actual-asset same-CombatSession case runs through
the feature runner using private copies of the frozen native and Windows
Foundation archives, each hash-checked before linking. The test first corrects
two stale harness assumptions: identical authored daggers must remain two
distinct main/off-hand entries pointing to the same ItemTable row, and the
generic weapon-Swoosh packet probe must not require the BashDown URI. These are
test expectations only; no production combat code changed. The BashDown case
observes root 347 step 0, requires set 164 and the same actor/lease, checks
duplicate delivery does not create a second instance, and verifies original
source mesh/particle packet kinds with retained resource bytes. This is a
feature-level resource/packet test, not proof that the currently running
preview's key binding or displayed slot selected this skill. The test does not
substitute GroundSlam root/set 173 for the selected first skill.

The earlier feature handoff lacked a same-run link from a PC key to its saved
assignment, source SkillTable row and sequence entry. The fresh frozen build
reproduction below now supplies that link and confirms the live set-164 dispatch
failure. Footage visual evidence still cannot identify BashDown from the observed
flash alone.

### B005 production reproduction and focused callback verification (2026-10-10)

The normal-executable reproduction is the frozen
`.local-inputs/v19-frontend-hotfix/knight-step-fx/dh-foundation.exe`, SHA-256
`227F229B81592D550C9A5684C8A6042F6FCC2E396882EEBFE4E8C7DD83186D78`, with
`knight-active28.args` and `knight-active28.log`. At frame 20, the actual PC2
route resolves slot 0 to BashDown and publishes actor `18446744073709551615`,
sequence 347, step 0, occurrence 4. The log directly records
`dispatched=0`, generic `Required source animation step FX anchored PlayAnimFXSet`,
and final `packetFrames=0`; this proves the effect is missing on that route, but
does not reveal the lower manager/provider reason. The screenshot/sequence is
not used to infer FX visibility from damage or animation alone.

Recovered-source caller chain for this failure: the actual retained step entry
feeds `RuntimeSwingFxObserverV1::dispatch`; `character_animation_step_fx_v2`
checks the step's Swoosh gate and, for `AnchorFX=1`, calls its Play callback with
the canonical origin and same Character owner. `step_play` passes that owner and
set 164 to `CharacterMeshFxOwnerV4::play_set`. The original set row has
`OrientWithAnchor=1` and `ScaleWithAnchor=1`; `CharacterMeshFxOwnerV4::sync`
therefore requests anchor position, rotation and scale while publishing the
actual effect instance. The production factory resolves these through its
same-session actor provider. Before this change, the step kernel replaced the
manager's concrete failure with the generic operation label, preventing the
actual source boundary from being identified. The change retains that provider
reason in the presentation diagnostic; it does not bypass the anchored path.

Focused test defined before implementation: replay actual Knight root 347 step 0
through the same CombatSession with a non-identity actor position, Euler rotation
and scale; require exactly set 164, a retained exact-resource frame containing
both source mesh and particle packets, a mesh matrix reflecting the supplied
anchor transform, duplicate-occurrence suppression, and renderer submission of
the same retained frame. The existing source-white particle selector and CPU
camera remain explicit test fixtures. The failure diagnostic path must preserve
the manager callback detail when the wrapped step kernel rejects. This proves
source anchor-query behavior and packet retention, not live GPU or camera parity.

Focused result: `runtime_effects_factory_native_tests.py` passes after the change
against hash-checked private native/Foundation archives. Actual root347/step0
dispatch emits set164 and retains exactly the original BDAE with 3 authored mesh
and 2 particle packets. While those packets remain retained, same-session actor
position changes by `(17,-29,11)` move each mesh packet by the same delta;
subsequent rotation/scale changes alter its basis. The same final frame reaches
the queue callback. Exact duplicate occurrence remains suppressed. A deliberately
rejected set-row callback reports both the generic anchored PlayAnimFXSet context
and its specific failure reason. The earlier alias between the wrapped source
error and callback error string caused the generic label to erase the useful
reason; the separate callback error now preserves it.

Current remaining uncertainty: the frozen executable's generic diagnostic has
no provider text. Integration lead must rebuild the updated diagnostic, rerun
this same isolated Knight route and record the newly exposed callback reason
before a production provider correction or normal-FX success claim. No normal
post-fix visual capture or integrated runtime pass has been produced yet.
