# v21.8 ES-STATUS report (es-status, branch v21.8-es-status)

Base: windows-foundation (merged into the branch at the start of round 2; the lead had already merged round 1).
CTest: 212/212 passing on the final tree (`ctest` from `C:/dh218/es-status-build`).
Round 2 commit (wiring): `3a1952ff`. Round 1 engine commit: `8d4c5acd`. Round 1 report commit: `044fb430`.

## Summary

- The status-state module is now wired into the live combat path. Stun (outcome 0x40) and scare (0x20) from skill hits reach
  monsters. Knockback (0x80) stays with the Push consumer (`b035_session_push_effect_v1`), and its state feeds the same gate.
- Real run, ViciousStrike (case `es-vicious-1`): two live lizards were stunned for 2200 ms, their decisions were blocked, and
  they made no attack on the player for the window. No visible stun pose (see item 6).
- Real run, JumpKick (case `es-jumpkick-1`): the push (0x80) knocked a lizard back at frame 3730. That lizard made no further
  attack, while the other lizard kept attacking. Frames of the knockback were NOT captured. Two follow-up runs did not
  reproduce the push, so this is escalated (see item 2).
- Finding 1 (is the stun/scare/knockback dispatch gated on Player?) is answered: NO. Three independent readings agree (section 1).
- Intimidation (Warcry) scare: not admitted (no use-profile row); not run.

## 1. Finding 1: is the stun/scare/knockback tail gated on Player/Idle? (answer: no, only on the defender being alive)

Readings (none relies on the others):
1. IDA pseudocode (`pseudocode-all.c`, F_ApplyResult 0x3b10b4): the Player/Idle block sets only the dodge/injure bits. The
   dispatch after it is `if ((v11&2)) goto LABEL_92; goto LABEL_20;`, outside that block, and reaches the stun (0x40), scare
   (0x20), knockback (0x80) and slow (0x100) tests for any defender.
2. IDA assembly listing (`full-listing.asm`, F_ApplyResult at 0x3b10b4), read directly from the instructions. R5 is the
   defender. `LDR PC,[R3,#0x34]` (vtable +52) followed by `BEQ` is the dead check, and it is the only guard on the whole
   block. `LDR PC,[R3,#0x28]` (vtable +40) then `BNE loc_3B17EC` enters the Idle block for Players only. The fall-through
   is `loc_3B1258`, the common chain. The `TST #0x40` test then calls `SM_SetStunState` on `R5+0x4F0+0xC`, the defender's
   state machine, with no Player check.
3. Virtual slots, from `vtables-and-rtti.json` (`_ZTV9Character` raw bytes): the function table starts after two header
   words, so vptr+40 is word 12 = 0x003a49f0 (`Character::IsPlayer`), vptr+52 is word 15 = 0x003a2ed4 (`Character::IsDead`,
   reads byte +5193).
4. Port reconstruction (`game-data/combat_application.cpp`, `dh2_combat_apply_monster`): stun, fear, push and slow requests
   have no Player gate. Only `!ds.dead` gates the tail. The Player/Idle test only adds the unclassified-hit bit.
5. `main.cpp` push sink already has a monster knockback branch (`faery2` comment: "F_ApplyResult guards the status block with
   IsDead, not IsPlayer"), which agrees.

Stale comment to correct (not changed here, hot file): `features/combat/b035_session_push_effect_v1.hpp` says the status block
is gated on "the defender's pre-effect Player/Idle state". The correct gate is alive-only for the tail.

## 2. Wiring (round 2), what changed

- `combat_session.hpp/.cpp`: `set_status_outcome_observer` and `StatusOutcomeEventV1` (attacker, target, outcomes, mask, the
  roll's formula sheet). Called in `apply_source_result` next to the es-passive combat-result observer, only for applied hits.
- `features/status_states/status_states_session_v1.hpp/.cpp` (new library `foundation_status_states_session`):
  - consumer `on_outcome`: skips player targets (not wired, see gaps), skips dead targets, reads the boss flag from the target's
    AI row (`AiProps.flags`, bit 2), applies stun and scare through the owner. Knockback is removed from the plan.
  - `bind(session)`: installs the observer, rebinds when the actor lease changes (a restore clears the handler).
  - `advance(dt_ms)`: the Stunned/Scared clock.
  - `can_act(actor)`: false while stunned, scared, or `action == knocked_back`.
- `features/status_states/status_states_v1.*`: `apply_status_outcome_without_knockback_v1` (new), used by the consumer.
- `features/enemy_ai/runtime_enemy_controller_v1.*`: `RuntimeEnemyControllerServicesV1::can_act`. A blocked monster takes no
  decision (no approach, no attack request). The first blocked frame calls the existing `stop` service once.
  `report().status_blocked_ticks` counts blocked monster-frames.
- `main.cpp`: include, `statusStates` (shared, same scope as `enemyController`), controller gate service, and in the gameplay
  block: bind, then `advance(gameplayDt*1000)`, then drain the owner log as `[status] ...` lines, and log blocked ticks every 60.
- Tests:
  - `status_states_v1_tests`: knockback excluded, boss immune inside the helper, plus the round-1 cases.
  - `runtime_skill_activation_session_v1` (real Rogue session, CTest): for every applied hit with the stun bit on a live lizard,
    assert `stunned && !can_act`. Assert the consumer saw every applied hit. Run `EnemyController::update` and assert
    `status_blocked_ticks >= 1`, no attack request, no pose. Advance past the stun and assert `can_act` again. Plus a knocked-back
    actor is blocked and an idle actor is not. The fixture stunned lizards in the run (`status ViciousStrike applied=12 stunHits=5
    stunnedNow=1`, `Roundhouse applied=4 stunHits=3`), so the assertions ran for real.

## 3. Real runs (hidden window, `act1rogue-run-rogue2.ps1`, seed `rogue-vicious` / `rogue-jumpkick`, package
`C:\dh218\act1rogue\pkg`, exe `C:\dh218\es-status-build\dh-foundation.exe`, Rogue hotbar key 2, lizard zone `-2900,-100,255@700`)

**Run es-vicious-1 (ViciousStrike).** Log `C:\dh218\act1rogue\cases\es-vicious-1\run.log`, frames `...\f3345.png ... f3420.png`.
- Cast: `Source skill key=2 slot=0 frame=3337 ... skill=ViciousStrike`.
- Stun on frame-3370 volley (the status lines print inside `apply_source_result`, before the per-volley hit lines):
  `[status] outcome attacker=18446744073709551615 target=4660011647479645029 outcomes=0x40 mask=0x8b35535 stunMs=2200 scareMs=0 boss=0 stunnedAfter=1`
  and the same for target `11259224883678223927`.
- `[status] actor 4660011647479645029: Idle -> Stunned (SetStunState event 50012)` (both lizards).
- Decisions blocked: `[status] enemy decisions blocked ticks=60, 120, 180, 240`.
- No attack on the player for the window. Lizard-to-player damage lines (`^Damage frame=`): 3362 (before), then none until
  3521 (after the stun ended about frame 3508). Player hits on the lizards continued (3440, 3445).
- Expiry: `[status] actor ...: Stunned -> Idle (stun timer end (INFERENCE))` (both).
- Frames viewed: `f3380.png` (strike FX on the lizard, its health bar; no stun pose), `f3420.png` (two lizards in normal pose,
  "Miss" text). No visible stun animation: not implemented (item 6).
- The run stopped at about frame 3700 with `Foundation error: Live combat: Source result identity/target eligibility rejected`.
  The pre-wiring baseline `C:\dh218\act1rogue\cases\t-vicious` stops with the same error at the same place (frame 3692 cast on a
  dead lizard). Pre-existing bug, not caused by this change (item 7).

**Run es-jumpkick-1 (JumpKick), exit 0 at 3900 frames.** Log `C:\dh218\act1rogue\cases\es-jumpkick-1\run.log`.
- Cast at frame 3336 (volley 1 all misses, `outcomes=0x1`). Volley 2 at frame 3729: `index=1 target=4660011647479645029 outcomes=0x80`.
- `[push] KnockBack monster target=4660011647479645029 profile=Swamp_LizadMan_Type1 attacker=18446744073709551615 source=JumpKick
  great=1 direct=1 outcomes=0x80 mask=0x8335535 frame=3730`.
- Gate: the knocked lizard made no attack after 3730 (its last attack was at 3639). The other lizard (`11259224883678223927`)
  kept attacking (Damage lines at 3763 and 3793). `[status] enemy decisions blocked ticks=60, 120` reached.
- No frames of the knockback (captures were set at 3345 to 3420 for this run).

**Frame-capture attempts, JumpKick knockback (`es-jumpkick-kb`, `es-jumpkick-kb3`): DID NOT WORK (escalate).** Two runs with
captures at 3728 to 3850 did not reproduce the push. Volley 2 rolled `outcomes=0x0` in both (`index=0`). The first run's push
was therefore not repeatable. Cause not established. Likely the injected-input timing feeds the shared roll RNG, but this was
not verified. Per the brief's two-attempt rule, this is escalated to Sonnet: capture the knockback frames at 3730 to 3760
once a push roll is reproduced.

**Warcry / Intimidation scare: NOT RUN.** `prince_berseeker_warcry` only appears in the text map (`runtime_skills_text_v1.cpp`).
It has no `SkillUseProfile` row, and its scare comes from a Lua `SetScareState` call, not the F_ApplyResult tail, so the
F_ApplyResult scare path is not what it uses. Not admitted this round.

## 4. Items

| # | Item | Status | Evidence |
|---|---|---|---|
| 1 | Stun 0x40 and scare 0x20 from live skill hits reach monsters | WORKED for stun; scare unit-tested only | es-vicious-1 `[status] outcome ... outcomes=0x40 ... stunnedAfter=1`; scare: unit `test_scare_and_knockback`, `apply_status_outcome_without_knockback_v1` |
| 2 | Knockback 0x80 (JumpKick) through the Push consumer into the state gate | PARTIAL: push and gate worked in run 1; frames DID NOT WORK | es-jumpkick-1 `[push] KnockBack monster ... outcomes=0x80 ... frame=3730`, no later attacks by that lizard; kb and kb3 runs did not reproduce the push |
| 3 | Per-frame clock; Stunned/Scared to Idle at expiry | WORKED for stun expiry in the live run; scare expiry (event 44) unit-tested | es-vicious-1 `Stunned -> Idle (stun timer end (INFERENCE))`; unit `test_stun_timer_and_gate`, `test_scare_and_knockback` |
| 4 | AI gate: a stunned, scared or knocked monster does not act or attack | WORKED live for stun and knockback | es-vicious-1 blocked ticks 60 to 240; no lizard attack 3370 to 3520; es-jumpkick-1 knocked lizard silent after 3730 while the other attacked. CTest session test asserts the gate |
| 5 | Bosses stun-immune | WORKED in unit tests; boss-AI module not wired (not needed) | `test_stun_no_restart_and_boss_immunity`, `boss_refusals` counter; the Swamp King AI is a separate module that is never stunned |
| 6 | Stun/knockback clip lookup from CharAnimTable | PARTIAL (documented) | Clip not played. Mapping in section 5 |
| 7 | Knight Intimidation scare | DID NOT RUN (not admitted) | no use-profile row; Lua-only SetScareState path |
| 8 | Finding 1 (Player gate) | ANSWERED: not gated | section 1 |

## 5. Animation clip (item 6): what exists and what is missing

- Original: `SM_SetStunState` (0x3c5ffc) reads `CharAnimTable` member +140 (`v11`). It selects the clip with `AnimStancedAnim`
  bit 0x200 (+ stance), then `_SetState(9, 50012)`. Scare: +124, bit 0x100, state 8, event 50013. Knockback: +36 (great, bit
  0x800) and +72 (normal, bit 0x400), already used by the Push consumer.
- Port schema (`game-data/animation_tables.cpp` `state_fields`): `Stunned` is field index 32, `Scared` is 29, `KnockedBack` 16,
  `GreatKnockedBack` 8. The rule `offset = 4 + 4*index + 4*(vector fields before index)`, with Interact (15) and Spells (31) as
  the vector fields, reproduces all four IDA offsets (36, 72, 124, 140). INFERENCE from the four data points, not from a struct
  definition.
- Missing: the clip is not played. The Push bank is built per profile before session initialisation and its clips are
  appended to the actor visual (`b035` header comment). A stun bank needs the same per-profile plumbing for `Stunned` (field 32).
  Not done this round. Visible stun pose therefore shows nothing (frames f3380, f3420).

## 6. Gaps and risks

- Stunned -> Idle at timer end is an INFERENCE (no event-43 registration found; the live log shows it happening).
- Zero-duration stun never expires (`CharTimers` interval 0 never fires; modelled, not observed live).
- Only non-player targets take status states. The player's own stun or scare (which would block input) is not wired.
- Knockback clip end: `finish_knockback` exists but the gate lifts only when `action` leaves `knocked_back`; the b035 path clears it.
- Owner state is keyed by ActorId and is not pruned on level change. ActorIds are large unique values in the live run.
- Non-determinism: the same JumpKick case rolled different outcomes across runs.
- Atlas: no "r2" notes exist in `coordination/atlas` (searched by name and text). `general/native-map.md` lists SetStunState as
  MISSING with no caller analysis. Nothing contradicted here.

## 7. Pre-existing bug found (not fixed, out of scope)

`Foundation error: Live combat: Source result identity/target eligibility rejected` aborts the run when a second ViciousStrike
volley targets a dead lizard (`resolve_source_result` requires `damage_eligible`). Reproduces in the pre-wiring baseline
(`C:\dh218\act1rogue\cases\t-vicious`, same frame region). Needs a ticket for the owner of the live target path. The JumpKick
run did not reach it.

## 8. Where the evidence is

- Logs: `C:\dh218\act1rogue\cases\es-vicious-1\run.log`, `...\es-jumpkick-1\run.log`, `...\es-jumpkick-kb\run.log`,
  `...\es-jumpkick-kb3\run.log`.
- Frames: `C:\dh218\act1rogue\cases\es-vicious-1\f3380.png`, `f3420.png`; `es-jumpkick-1\f3345.png ... f3420.png`;
  `es-jumpkick-kb3\f3728.png ... f3790.png` (no push in those frames).
- Driver: `C:\dh218\run-es-vicious.sh` and `C:\dh218\run-es-jumpkick*.sh`. Runner: `coordination/v21.8/act1rogue-run-rogue2.ps1`.
