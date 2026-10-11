# B092 / B098 report (corpses): no code change, not reproduced on v20.1/integrate

## Evidence
- Video (P1, 422-430 s, 3 fps sheet): Bog Moth killed ~423.5 s, "+4 EXP", body lies, "8 GOLD" ~426.5 s, gone by ~426.5 s. Kill-to-vanish about 2.2-3.0 s, no visible fade/sink. (Prior DESPAWN2 report 1c, same footage, agrees.) Observed.
- IDA (reused from DESPAWN/DESPAWN2 reports, not re-decoded): CSDead event 34 -> body released, timer CharacterDesign.Despawn_Delay 2000 ms -> event 46 -> CSDespawn (SM_SetAnim(-1), no clip) -> Limbus hides; StartFadeOut/UpdateFadeOut empty. Death clip is the AnimTable "Died" row per profile.
- Ours (build-fix092 = v20.1/integrate, quiet EXE, fixed step 1/60): authored moth killed by sword: death pose frames 227-288 (1.0 s), hidden at 407 (total 3.0 s). Killed by skill (BashDown): death 283-344, hidden 463 (3.0 s). Authored lizards (13, 14): death pose played, hidden 2 s later, lizard 14 schedules respawn 20 s (property 11). Intro lizard (lifecycle) and script moth FakeMoth_01 (after B080 fix): killed, death end, hidden. Matches video (3.0 s vs 2.2-3.0 s) and IDA.
- Baseline v20 EXE: intro lizard also dies and is hidden (mo0). Script moths FakeMoth_01/02 were animation-only there (untargetable, never die; B080), so a lingering/unkillable moth in shot-11 is explained by that, already fixed by B080/B084 on this base.

## Verdict
- B092 (corpse takes long to vanish): on this base corpse lifetime = 1.0 s death clip + 2.0 s delay = 3.0 s, equal to the video. The user's long-lived moth is most likely the v20 animation-only movie moth (B080). Not reproduced otherwise.
- B098 (no death animation, stays): not reproduced for any killable monster kind reached (moth, lizard, intro lizard, script moth). Shot-17 shows a LIVE lizard next to the hero (no corpse), which is the B091 passive-monster symptom, not a corpse bug.
- Test artifact noted: --kill-test (HP=0 without a combat hit) leaves the death pose frozen (pose ms constant), so no despawn; real kills are fine. Debug-only path, not changed.

## Not verified
Boss (SwampKing), Type2/Type3 lizards, MisterSnack are not reachable by condition in the Swamp harness; their death/despawn uses the same owner but was not run. Respawn of authored monsters and the user's exact scenario (unknown kill method/route) not reproduced. Fade-out: none in original, none implemented.
