# Active skill state V4 boundary

`CharacterSkillContextV4` borrows `CharacterPlayerSkillsV3`, its same private
`CharacterScriptSessionV3` VM/timer store, the caller's `NativeFsm24::state`
(State56), and the same `CharacterAnimationInstance`. It rejects a mismatched
state/character binding. It owns no second VM/FSM/timer/animation clock.

Integration points:

* Register state6 Focus/Blur/Event/Update with `execute`; update is the recovered
  empty CSSkill body, not the complete Character frame or FSM machine update.
* SetSkillState calls `execute(skill_state_select_v4,index,raw_moving,payload,force)`.
  Required transition/state-event providers own predicates, blur/focus ordering,
  current presence and machine notifications on the same State56.
* Compose `animation_event` into the existing animation observer. Authored event
  0x28 forwards the original payload pointer string. `do_skill` consults current
  state then calls the same retained player's original V3 skill use/callback path.
  The adapter does not replace the existing observer; event0x22 closure is ignored.
* Blur timer10/event0x30 uses `session().start_timer` on the same V3 timer store.
  Existing session expiry/event routing remains the frame owner's responsibility.

All other services are required synchronous providers. Missing debug ownership,
body/heading/sneaking, animation selection/speed, class checks, table/stance,
transition, step counters, named prefixes and other state animation behavior fail
at their reached prefix with -2. Failure preserves prior source effects.

## Verified kernels

Original ARM32 CSSkill Focus/Blur/Event/Update/SetSkillState vs O2 ARM64:
4096 cases, 18390 ordered services, zero mismatches. Real original ARM32
CharAI::_OnAnimEvent state6 `do_skill`/ignored payloads vs O2 ARM64:
512 cases, 1791 services, zero mismatches. Named prefix deeper bodies and other
states are provider boundaries, not part of the latter differential claim.

Host `character_skill_state_v4_host.cpp` verifies borrowed State56 mutation,
source last-target copy before failed blur, service ordering, required-provider
failure, malformed pre-entry and payload string handling under ASan/UBSan.
Context compilation checks type compatibility; it is not a full retained-player
or rendered campaign execution proof.

## Actual source skills still need gameplay providers

The exact cached scripts are plaintext source in
`.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/skills/`:
`prince_warrior_bashdown.luac`, `prince_mage_coldray.luac`, and
`prince_rogue_jump_kick.luac`. This is not an alias fixture or campaign proof.

All three use CalcManaCost/GetCurrentSkillInfo and **HasMana** in Check, then
**UseMana** in Pre. Source native registrations are HasMana0x3b78ec and
UseMana0x3b7864. V4 does not supply these callbacks or force Check success.

All three Pre callbacks set CharacterFilter Enemy, ObjectFilter AttackableOnly,
Sorting FrontalFirst, perform TargetListSearch and optionally GetTargetListTop
and LookAt. ColdRay and JumpKick additionally use TargetListBackup and
MaxAngleSearch before selecting a facing target. Correct live provider semantics
require the actual world candidates, hostility/attackability, position/angle,
ordering and retained targets; static target handles cannot prove this.

* BashDown: range160; Pre plays `skill_dh2_prince_warrior_bash_down`; Skill applies
  the source property class then SkillCombatRoll against its retained target.
* ColdRay: range650/angle90 search repeats in Skill; each target is popped and
  SkillCombatRoll runs, with character FX `elem_dmg_02_water` when successful.
* JumpKick: Pre initial range250, Skill range160/angle270 at the current landing
  position; each target is popped and combat runs, with `bloodsplat` character FX.

Post callbacks ClearTarget. Live combat results, property-class effects, FX
resources, mana authority and target list mutation remain required providers.
The source skill context adapter and kernel proofs do not claim complete active
gameplay, rendered skill effects or full campaign support.
