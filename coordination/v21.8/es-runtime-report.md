# ES-RUNTIME report (v21.8, branch v21.8-es-runtime)

Scope: ES-RUNTIME of coordination/v21.8/ENGINE-SYSTEMS-PLAN.md, with the ENGINE NEEDS items N1, N2, N3, N11 (Archer),
E1, E2, E8, E12, E13 (Necromancer, Berserker), N7 (Illusionist). Base: windows-foundation @ bb604d97.

## Results

| Item | Status | Evidence |
|---|---|---|
| (1) External-owner registry (N1) | WORKED (unit + wiring) | `runtime_skill_external_owner_registry_v1_tests` passes: duplicate/null refused, first-handling-owner dispatch, explicit error without owner, advance walk stops at failure, cleanup/reset reach all owners, real Mage owner stays on Mage skills beside a non-Mage owner. main.cpp puts `skillOwnerRegistry` in `request.external_owner`; build OK. Not exercised by a real-run cast. |
| (2) General skill timers (N2, E1, E13, N7 expiry callback) | WORKED (unit + migration + integration) | `runtime_skill_timer_service_v1_tests` (one-shot expiry, repeating catch-up 500/1000 ms, self-stop, same-instant order, nested start, owner cancel, error propagation, rejected intervals). The Mage owner's ad hoc clock now runs on this service; `mage_skill_owner_v1` still passes (StoneEscape statue fires at 2200 ms, not before). |
| (3) Death watch / IsDead (E2) | PARTIAL (helper only) | `runtime_skill_death_watch_v1_tests` passes (tracking order, dedup, poll removes exactly the dead entries, absent actor = dead). No Pestilence or Rise owner exists yet (phase 2b), so nothing consumes it in a real cast. |
| (4) Skill cleanup hook (E12) | PARTIAL | Interface `cleanup(session, actor)` on the owner base, forwarded by the registry (unit-tested). main.cpp calls it once on the local player's death transition (`playerSkillCleanupRan`). Build OK; the death path has not been run. Monster deaths and script unload are not wired. Mage has no cleanup (Lua default is empty), so no behaviour change. |
| (5) Delayed landing callbacks via `launch_skill` (E8 part, N3) | WORKED (existing evidence, unchanged) | `runtime_session_projectile_v1` (test lines ~675-740): delayed_check accept/reject, delayed(target, projectile) once, expiry delayed(nil, projectile). Archer owner (N3 routing of Archer rows) not built, phase 2b. |
| (6) Per-marker re-entry of Use (N11, E2 volleys) | WORKED (existing evidence, unchanged) | `runtime_skill_activation_session_v1` (passes): each distinct do_skill marker of ViciousStrike runs OnSkill_ again (`marker_count == spec.markers`, "each distinct marker is one OnSkill_ volley"). |
| (6b) Per-volley class token and target policy (E2 Berserk) | PARTIAL | `RuntimeSkillVolleyV1` plan + `select_volley_v1` on the profile (cursor % count), unit-tested in `runtime_skill_use_profile_v1_tests`. Coordinator generic Use: cursor per (actor, SkillTable id) persists across casts, the chosen volley's class replaces the row class, a top-only volley keeps the first target, cursor advances after a successful volley. Rows without a plan are unchanged (activation-session test green). Not exercised by a real cast: no Berserk row is admitted yet (phase 2b). |
| (7) Integration test: timer-driven repeating skill through the registry with a non-Mage owner | WORKED (unit/real session) | `runtime_skill_timer_registry_integration_v1` (real CombatSession, two lizards): Use starts one loop, no roll before 1000 ms, rolls at 1000/2000/3000 ms (2 targets each), loop stops after the third tick, no tick at 4000-8000 ms; registry reset cancels a running loop; empty target list stops the loop at its first tick. Animation/FX/sound not verified (no real run). |
| E8 four-direction free-spot probe (`MoveTo` + `HasPath`) | DID NOT WORK (not built) | Not in the six ES-RUNTIME items; no code. Needed by Rise (phase 2b). |
| Runtime Summon (E7, Rise) | DEFERRED | Plan defers runtime Summon. |

## Original evidence (investigation)

- Lua (`.local-inputs/assets-extra/android/data/scripts/skills/`, plain text): `_commons.luac` (StartSkillTimerCB wraps the callback to restore SKILL_ID; StopTimerCB; RegisterSkillEx cleanup default empty; OnSkillCleanUp global); `ai/_commons.luac` (StartTimerCB stores {callback, loop, refData} keyed by timer id; OnTimer calls it and removes non-looping entries; StopTimerCB clears the entry and calls native StopTimer).
- IDA (`ida-apk-export-2026-10-07/.../pseudocode-all.c`): `Character::_StartTimer` 0x3b7590 (loop true = TMR_Start(ms, -1, type 53); false = TMR_Start(ms, 0, 53); returns id); `Character::_StopTimer` 0x3b7064 (TMR_Stop). Both are gated on a state-machine check (`*(v4+4) == 3`) whose meaning is not decoded (not modelled; OPEN).
- IDA OnSkillCleanUp callers: `CharAI::_SkillCleanUp` 0x3d8ae0 and `CharAI::_SpellCleanUp` 0x3d8a98, called from `CharAI::AI_SetDead` 0x3d6cdc (death), `CharAI::AI_ScriptCleanUp` 0x3cfd7c, and `CharAI::AIUnLoadScriptProcess` 0x3cc9dc. This closes Q17 of the Necromancer report. Port hook: the death transition only.
- IDA `GameObject::_IsDead` 0x38ea48: pushes a boolean from a virtual call (callee not decoded). INFERENCE used: dead = absent from the Session or not alive.
- Atlas (now at primary `coordination/atlas`, read-only): `atlas find StartTimer` / `StopTimer` return only Lua reference rows, no native row. The IDA addresses above are the evidence. Disagreement noted: atlas has no native row for these timer natives.
- Lua discrepancy with the plan: FireSkullRain Lua has `MAX_TICKS = 2` (3 rolling ticks: counts 0,1,2 then stop). The plan's "max 3" matches the number of rolls, not the constant. The integration test follows the Lua.
- Berserk: `skill_inc` is a file-level counter that is not reset per cast (only wrapped at 3). The volley plan therefore persists per (actor, skill) in the coordinator, as the report on Berserker says (LIKELY).

## Behaviour change (documented)

The Mage owner's advance() order changed: statue explosions and tornado ticks now fire from the shared timer service before the tornado cleanup and carry loop. Before, carry ran before that frame's tick, so a victim rolled on a tick was carried from the next frame. Now it is carried in the same frame. No Mage test covers tornado carry; the one-frame difference is the only change.

## Tests

- Baseline (windows-foundation, before changes): CTest 204/205. `frame_pacer` failed once on a wall-clock check (worst frame 31 ms against a 10 ms target), not related to this work; it passed on the final run.
- Final (branch v21.8-es-runtime): **CTest 209/209 passed** (205 baseline + 4 new: `runtime_skill_timer_service_v1`, `runtime_skill_external_owner_registry_v1`, `runtime_skill_death_watch_v1`, `runtime_skill_timer_registry_integration_v1`).
- Affected existing tests green: `mage_skill_owner_v1`, `runtime_skill_use_profile_v1` (extended with volley cases), `runtime_session_projectile_v1`, `runtime_skill_activation_session_v1`.

## Not done / gaps

- No real-run (hidden window) verification of any item; all verification is CTest.
- Per-volley coordinator path (6b) has no cast-level test; Berserk row is phase 2b.
- Death watch and cleanup are not consumed by Pestilence or Rise (phase 2b). Cleanup wiring covers only the player death transition.
- Chance units, stun/knockback states, and the state-3 gate on native timers are not in scope and not verified.
- Archer owner, Illusionist expiry callbacks and Necromancer owners are not implemented; the registry and timer service are the seams for them.

## Branch

Branch `v21.8-es-runtime`, worktree `.claude/worktrees/es-runtime`, commit: see the final git log entry (committed on this branch; not pushed, not merged).
