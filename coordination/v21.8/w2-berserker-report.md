# v21.8 w2-berserker: Berserker subclass skills on the general engine (phase 2b)

Branch `v21.8-w2-berserker`, worktree `.claude/worktrees/w2-berserker` (subst B:), build `C:/dh218/w2-berserker-build`. Base `windows-foundation` @ 99d4d1c5.

## Summary

- Engine and data work is implemented for all eight Berserker skills in scope. Unit tests cover them, and the full CTest run passes 213/213 on the final tree.
- Real hidden-window runs were NOT achieved. Every Berserker-class run stopped at the frontend, before any cast: `Selected profile class has no authored source preview actor: KnightPlayerBase_Berserker`. The same error occurs for `KnightPlayerBase_Paladin` on both `staged3` and the older `staged` package, so it is a subclass-wide menu-preview gap, not a Berserker or staged3 problem. The plain Knight control passes the frontend. The combat-init path with the patched bindings XML was therefore not reached in-run.
- Verdicts below are engine/unit level only. No skill has an in-game verdict of WORKED.

## Per-skill status

Evidence key: UT = unit or integration test in CTest; RR = real hidden-window run.

| Skill | Engine piece | Cast | Anim | FX | Sound file | Effect / numbers | Verdict |
|---|---|---|---|---|---|---|---|
| Berserk | Knight row admitted: 3-volley plan (stun all in 200/360, damage top, knockback top), OnPre HP cost (`HP% > 10`, `HP -= HP/10`) via `set_current_health` | UT rows pinned; RR not reached | not run | not run | not run | HP cost and volley class tokens are data-pinned; damage/stun/knockback outcomes not measured | PARTIAL (engine admitted, RR blocked) |
| Intimidation | Row fixed to `pre_list_all` (was `pre_selected_single`); `status_arm` (GetRand 1..4, stun on 3, fear/stun cues after loop); `script_status` path to status owner; boss/miniboss refused | UT row pinned | not run | not run | cues `..._hit_fear` / `..._hit_stun` recorded on receipt (wav resolves in staged, not listened) | scare/stun states not observed | PARTIAL (wired, RR blocked) |
| WhirlingDrift | Row was already admitted on base; unchanged | UT row existing | not run | not run | not run | push via roll chance not measured | NOT VERIFIED in-game |
| KillingFrenzy | Event owner (pre-existing, UT) unchanged; CanAttackInMelee gate still missing | UT (counter 1..5, 1500 ms reset, 5000 ms max) | n/a | n/a | activate_4 not played (commented out in Lua) | speed buff values UT only | PARTIAL |
| KillingStreak | `streak_kill` on player kill: body (capacity 128, stacks) + dummy (capacity 1) for SnS_Cooldown ms (class projection, 5000 ms at L2) | UT: kill-only, stacking, expiry | n/a | FX/sound NOT dispatched | not dispatched (`sfx_passive_skill_activate_2` exists in staged) | buff instances UT | PARTIAL |
| Ravager | Existing stat row (class 159) + new threshold buff (`Skill_Berserk_Ravager_buff`, cell 203 = L) from rank 7 | UT: rank 6 adds 91, not 203; rank 7 sets 203; back to 6 clears | n/a | n/a | none in Lua | UT on cells 91 and 203 | PARTIAL (see deviation) |
| Survival | Event owner `survival_hit`: HP% < 35 arms 8 s buff + flag, >= 35 removes; flag cleared by advance | UT: full HP no-arm, low-HP arm, no duplicate, player's own attack no-arm, re-arm, removal | n/a | FX set recorded on buff spec, not dispatched | not dispatched (`sfx_passive_skill_activate_3`) | UT | PARTIAL, trigger side INFERENCE |
| Toughness | Existing stat passive (formula 6 fix merged on base) | UT (existing `passive_skill_owner_v1` / event test A: Max_HP accumulates the class contribution, rank 0 restores) | n/a | n/a | none | character-sheet Max_HP checked in UT only, not against an in-game sheet | PARTIAL (UT only) |

## Engine changes

1. `runtime_skill_use_profile_v1.hpp`: appended `RuntimeSkillSelfHpCostV1`, `RuntimeSkillStatusArmV1`, and two trailing profile fields (`self_hp_cost`, `status_arm`). Existing rows are unaffected.
2. `runtime_skill_use_profile_knight_v1.hpp`: `kBerserkVolleysV1`, `kBerserkSelfHpCostV1`, `kWarcryStatusArmV1`; Berserk row admitted; Intimidation row fixed to `pre_list_all`.
3. `runtime_skill_cast_coordinator_v1.cpp`: OnPre self-HP write (before UseMana, after target query and LookAt); Use-path status arm (GetRand, `script_status_actor` per Character target, fear/stun sounds after the loop).
4. `combat_session.{hpp,cpp}`: `ScriptStatusServiceV1`, `set_script_status_service`, `script_status_actor` (mirrors the existing Stop service).
5. `status_states_session_v1.{hpp,cpp}`: `script_status` (boss/miniboss refused; persist=true, event path).
6. `main.cpp`: installs the script status service next to the Stop service (4 lines).
7. `event_passive_owner_v1.{hpp,cpp}`: `killing_streak` and `survival` kinds; `streak_kill`, `survival_hit`; on_hit now routes attacker-side and target-side events separately; advance ages the Survival flag.
8. `passive_skill_owner_v1.cpp`: `kThresholdStatPassives` (Ravager rank >= 7 buff, separate rank key `<skill>#threshold`).

## Tests

- `runtime_skill_use_profile_knight_v1_tests`: seven Knight-tree rows; Berserk volley classes, top-only flags, HP-cost gate and divisor; Intimidation pre_list_all, closest_first, status arm faces/stun face/fallback/cues.
- `event_passive_owner_v1_tests`: section E (Killing Streak), section F (Ravager threshold), section G (Survival).
- Full CTest: 213/213 pass on the final tree (rebuilt after the last edits; run from bash with the toolchain bin on PATH). An earlier run showed 13 failures with 0xc0000135; those were the missing-DLL PATH problem, not code failures.

## Real-run attempts (not successful)

- Case `C:/dh218/cases/w2-bers-a`: Berserker save `C:/dh218/w2-berserker-saves/bers30.save` (writer: class KnightPlayerBase_Berserker, Berserk in slot 0, level 30, 16 skills), staged3, own exe. Log: frontend preview error (above).
- Case `C:/dh218/cases/w2-knight-ctl`: Knight control on staged3. Passes the frontend (`profile_actor PASS`).
- Case `C:/dh218/cases/w2-paladin-ctl`: Paladin save on staged3. Frontend preview error.
- Case `C:/dh218/cases/w2-paladin-oldstaged`: Paladin save on staged (older package). Same frontend error.
- Earlier sub-berserker cases (`C:/dh218/cases/sub-berserker-1..3`) failed at combat init with the old bindings (`Source actor property row unavailable`), which is the root cause the lead identified. Those bindings are now patched in staged2 and staged3. Case `sub-berserker-4` reached session start through the Continue path with a gameplay save, so a subclass hero can start a session without the menu preview; that route is the one to try next.

## Deviations and inferences (labelled in code)

- Survival trigger: the Lua has no attacker guard. The port delivers OnTargetHit to the TARGET (player receives) and treats that as the Lua call. This is INFERENCE (OPEN-4: IDA caller of `CharAI::OnCombatResults` not identified; `AISDefault::OnCombatResults` at IDA 159458 is the only confirmed receiver). The HP check reads actor HP at dispatch time.
- KillingFrenzy CanAttackInMelee gate not implemented (IDA `Character::_CanAttackInMelee` 0x3b6f64 returns `ItemInventory::CanMeleeAttack`, which is not decoded in the export). INFERENCE: passes for a melee weapon.
- Warcry draws GetRand for bosses too (the Lua draws only for non-bosses). The boss refusal is in the status owner. The RNG sequence differs; the gameplay outcome does not.
- Warcry's `StartSkillTimerCB(BuffExpired)` is cosmetic and not modelled.
- Ravager: the Lua `SetProp(Special_Equip_2H_In_One)` persists after a level drop (OPEN-11). The port uses a buff that is removed at rank 6 or below.
- KillingStreak and Survival: Lua PlayFX / PlaySound are not dispatched (presentation gap).
- Berserk: the HP write happens before UseMana, as in the Lua; a later UseMana failure would leave the HP write committed (reported as partial failure, like the other OnPre prefixes).
- Berserk volley cursor persists per (actor, skill), matching the file-level `skill_inc`.
- Stun, knockback and damage chances for Berserk rely on the generic roll path. OPEN-1 (chance units) is still unresolved, so no chance value is verified.

## Remaining gaps and next steps

1. Unblock the subclass menu preview (`savedProfileActors` lookup in `features/frontend/frontend_presentation_v1.cpp:242`) or use the gameplay-save Continue route from `sub-berserker-4`. Lead decision needed: this affects all subclass real runs.
2. Once a Berserker session starts: RR for Berserk (cast, three volleys, HP drop 10%, stun/damage/knockback), Intimidation (scare/stun on non-bosses, fear/stun cue), Toughness (Max_HP on the character sheet), and the event passives on a real hit.
3. CanAttackInMelee gate (KillingFrenzy) and presentation dispatch for the passive FX and sounds.
4. Survival trigger confirmation (OPEN-4).
5. No commit has been pushed; the lead merges.
