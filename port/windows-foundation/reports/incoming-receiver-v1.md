# Generic incoming damage receiver

Investigation before implementation: the source bank report
`features/combat/runtime-player-incoming-animation-bank-v1-report.json` resolves
actual player profiles Knight263/AnimTable48, Rogue325/table50 and Mage290/table49.
Each supplies Injured267 Type2 (three caller-selected prince_hurt clips) and
ordinary Died259 Type0 (prince_dying_01). No Knight bank or stance fallback is
allowed. The ordinary Died setter has no0x8000 stance bit in source mask0xd2.
Source SM_SetInjureState0x3c5d84 / CSInjuredFocus0x3c33e8 and
SM_SetDeadState0x3c58c8 / CSDeadFocus0x3c4d50 use the same Character animation
override/owner. Outcome16 and3000ms injury gate remain the existing tested path.
Cast/Skill source state departure rules are covered separately by
`reports/source-sequence-departure-v1.md`.

Visual evidence reused: original hit-reaction sequence375.25–376.25 shows changing
Warrior attack poses across a small health decrement; it does not prove all hits
interrupt. No available original Rogue/Mage injury/death sequence proves parity.
Recovered logic/assets arev1.0.2; video isv1.0.3. The new prepared incoming bank
has original source-asset and marker tests, not live receiver evidence.

Current defect: animationOnly disables both outgoing attacks and incoming
targetability. Flipping it would accidentally require/admit unprepared player
attack records. Add an explicit incoming capability to that no-outgoing profile,
requiring its own configured source Injury and Died leaves. The same World
ActorId/properties/equipment, ActorCombatRuntime, retained visual, root motion,
source hit timing and GameSave own the result. No second actor, native object
graph or class-specific rule. The selected injury leaf is explicit caller input;
the original Type2 random chooser is under a separate source audit.

Focused verification plan: actual Rogue/Mage profiles and source starting gear,
one same-Session Lizard; prove enemy target eligibility, ordinary marker damage,
no player outgoing attack records/admission, full source outcome/conditional
injury/death, same retained owner/root motion, terminal corpse and no-replay save
restore, and reject incoming profile without actual reaction/death. Check pure
presentation actors remain untargetable and existing Knight paths unchanged.
Use controlled formula status ratings only for otherwise-unreachable outcome
branches; do not claim that alters or proves authored ordinary Lizard balance.
Production model/camera/HUD/hit/death verification remains the lead's next step.

Type2 selection audit is now complete: `CharAnimator::_SetAnim`0x3cab38 checks
Type2 and `MP_MinimalRandoms`; normal mode draws once from app channel0 then
`_SetAnimStep`0x3ca79c resolves its selected clip. Minimal mode selects0 without
RNG. Evidence: `port/game-data/reference/selection/reference/original-functions.asm`
at0x3cac18–0x3cac88; Random clone0x3ca708 uses app globals0x99f89c/0x99f8a4,
seed=(seed*59051+177149)%14348907. No actor-local seed/counter is introduced.
The host's existing persisted World stream supplies that normal channel;
the runtime test compares its exact outcome to `dh2_animation_random`.
Caller-supplied optional reactionMinimalRandoms enables this audited flat Type2
root. Unset preserves explicit diagnostic choice; unsupported nested/type/loop
policy rejects rather than guessing. The target-footage debug switch value and
original saved-Type2 injury replay remain unknown. Ordinary death is Type0.

Initial implementation also fixed a role-cache defect: baseTargetable defaulted
true without copying the prepared trait. Generic casting therefore changed a
pure presentation actor's World role to targetable and broke strict reinit/save
restore. It now copies the actual initial role. Existing restore compatibility
checks are unchanged; pure display cast→quiescent save→reinit→restore is tested.

The strict actual-Session receiver runner passes eight cases: Rogue/Mage, all
three random Injury leaves, and MinimalRandoms step0/no draw. Existing generic
enemy controller acquires and damages each class at actual retained attack
markers using its unmodified source properties; test LOS is an explicit clear
visibility fixture, not full level spawning/navigation. Controlled source formula
ratings separately exercise Injury branches, exact shared result+selection RNG,
positive3000ms gate/no second selection, source poses and ordinary death. The
same retained owner and role survive recovery and strict GameSave corpse rebind;
an active motion observer verifies no corpse displacement/RNG replay. No outgoing
player attack IDs are created, and invalid missing-death capability rejects.
Runner: `tools/run_combat_session_incoming_receiver_tests.ps1`.
Log: `.local-inputs/session-incoming-receiver-test/run.log`.
Current EXE SHA256:
`E735384F7A96344D63D07B7BD395854D55288B77496CABE89851F07D783B6CD3`.
The independent reviewer previously reran06DACE successfully; root reran all
eight cases on current code after the additive source attack-bank seam.
This does not claim full player HitFor/status parity, production GPU behavior,
source encounter activation or complete Rogue/Mage outgoing combat.
