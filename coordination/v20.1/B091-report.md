# B091 + B094 + remaining B080: scripted boss targeting and combat

Branch `fix/b091`, worktree `DH_wt/fix091`; base `dfc67d8d`. Implementation owner: `/root/b094_b091_boss_cinematics`; root supplies the fresh read-only reviewer and owns integration. This report supersedes the prior B091 synthetic-command-only verdict. The investigation note was committed before additional behavior changes at `ded1a260`; subsequent discoveries were recorded before the release predicate changed.

## Identity and requested result

User correction: B094 is the **brown boss lizard**, not Celeste/Rene. The old `fix094/B094-report.md` INTENDED conclusion relied on the wrong identity and does not resolve B094. Viewed shot-10 and shot-13: large brown mace-carrying creature beside the hero, no boss ring or plate. A still alone cannot establish continuous passivity. The actor/model/room match the captain `_prim_Monster_05_325`, `Swamp_LizadMan_Type3`, CharacterTable row363, `lizardman_big.bdae`. Other Type2/3 instances cannot be excluded solely by the still.

Expected: after playing or skipping the authored scene, a live gameplay monster can be selected, shows its ring/nameplate in range, attacks and receives normal hits. It remains at its source position and does not revive if killed/retired. Preserve presentation-only actors and existing plot kills.

## Visual investigation first

Reference P1 v1.0.3 `Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`. All extracted sequences are under `.local-inputs/claude-v20.1/fix091/audit/`.

- `captain462-478.png`, 2 fps, 462-477.5 s: captain approaches captive, strikes/fells it during SKIP/camera presentation at about466-470.5; HUD returns; at474-477.5 the brown actor has **Bogwomp Chief** plate/red ring and exchanges blows with the hero.
- `captain478-494.png`, 2 fps: combat continues; at483.5 the plate disappears/XP is shown, boss falls; corpse is visible in following frames and absent by486.5. Approximate duration only: 0.5 s sampling and another green actor partially obscure the body.
- `troll349-363.png`, 2 fps: TRUNGUS enters/roars, HUD returns, red ring/plate and actual hit reactions/damage appear.
- No SKIP button press occurs in these original excerpts. Skip outcome is supported by source logic and reconstruction tests, not original video observation.

## IDA/callers/gates

### Reviewer correction investigated before repair

Independent review found the prior byte+5248 interpretation wrong. Re-read `Script_MarkCharacterAsScripted::Set` **0x45958c** and `Unset` **0x459550**: they write Character+5248=1/0. It is the scripted gate, not an animation-playing flag. `CSAnim::OnUpdate` **0x3c1a3c** requires that byte to be0 and +1345 to be nonzero. The original P1 captain/troll sequences above already establish that scripted poses remain during the scene and combat returns after it; no unchanged footage replay is required. Previous reconstructed release logs at captain201/252 preceded End/Unset301 and therefore failed this source gate.

Expected repair: keep a completed clip pending while the host's existing `character_scripted(id)` flag is true; a replacement/chained/cancel command replaces or removes that pending completion; after Unset, the next host update may release exactly once through the existing live/dead/Limbus/PreSpawn owner gate. Explicit scripted Idle remains an authored command and can cancel pending animation completion, including during skip. Keep future skipped PlayActorAnim as no-op. Test before coding: true flag + completed clip produces no callback/release over multiple frames, Unset gives one release, duplicate updates/Unset do not repeat, chains/replacement/cancel remove obsolete completions, and skip preserves order until actual Unset. Runtime: short captain played/early/late skip and troll played/skip, assert no automatic release while scripted and retain plate/ring/hits/death behavior. Old frozen runtime is historical evidence; reviewer approval awaits a new frozen hash.

Interim gated runtime additionally showed captain clip32 has duration6075ms and is still active at Unset301: waiting for that duration postpones its callback to612, after AI has already replaced the pose. Re-read OnUpdate: it has **no animation-duration/ended test**, only scripted0 and +1345. Before this additional correction, expected behavior/test is therefore strengthened: a real Set->Unset edge admits the current scripted clip to Idle on the next host update even if its final nonblocking animation has not ended; do not start a follow clip after that Idle admission. Keep earlier completed candidates while gated, and cancel old candidates on replacement. Focused test adds Unset before clip-end and before a chained follow, one release and no follow/duplicate. The interim235730 runtime is not final source parity.

Further raw-source precision before final coding: the predicate is the **current** scripted0/returnIdle1 state, not an edge requirement. The host's ActorClipState entries come only from Script_PlayActorAnim(...,0,1), including their follow clips. Therefore already-un-scripted entries also admit Idle on their next host update regardless of duration, while true entries remain/chain as authored. Lua callers at pseudocode135811/135815 pass fourth argument0 and are outside this host; this rule must not affect those animations. Focused test adds already-clear source gate before duration, and duplicate Unset does not itself drain other actors or reissue a completed admission. Mark Execute0x45d4c8 remains active under skip; End order is PutIdle0, Unset4, Unlock5, Exit6.

Pseudocode is `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`.

- `Script_PlayActorAnim::Execute` **0x45e890**: skip=1 is a no-op; otherwise resolves the named Character, programs animation dictionary fields, calls `SM_SetAnimState(...,0,1)`.
- `SM_SetAnimState` **0x3c1aa8**: enters14 and sets the allow-Idle-on-update byte+69 (+1345 on Character). `CSAnim::OnFocus` **0x3c324c** writes flags8768 for that mode and selects the animation. `CSAnim::OnUpdate` **0x3c1a3c** enters Idle only when Character scripted byte+5248 is false and byte+1345 is true; it does not test an animation-playing byte here. `CSAnim::OnEvent` **0x3c1a14** has a separate event34 path controlled by byte+1344.
- `Script_SpawnCharacter::Execute` **0x45f400** enters Spawn even when skipping. `CSSpawn::OnEvent` **0x3c0b04** admits physics on marker40 `is_interactive`; **`CSSpawn::OnBlur` 0x3c2f7c** initializes physics when leaving Spawn if marker admission did not already happen. This is the missing early-skip body handoff.
- `Script_PutCharacterInIdle::Execute` **0x45d590** passes waitForAnim=!skip through `Script_CharacterState_Callback`; **SetState 0x45971c** rejects dead (virtual+52), Limbus and awaiting-to-spawn characters before calling Idle. Adapter kind31 has the same live/state gate. Common `EndScriptedCutScene` uses selector All. `CSIdle::OnBlur` **0x3c2d3c** clears +1336; `CSIdle::OnFocus` **0x3c3020** suppresses selection/flags while that byte is set. The port can hold a scripted pose even while canonical state is3; its completion must release that presentation too.
- `ScriptManager::SkipScript` **0x4604c0** flushes messages/sets skip; it does not stop Character animators. Therefore already-playing actor completions survive skip; future skipped clips remain no-ops.
- `TargetList::_IsCharacterValid` **0x4a1ab8** admits living enemies through mask1/AI_IsEnemy (virtual+52 is IsDead, not summoned; mask8's dead-friendly path is unrelated to boss targeting). Reconstruction also gates combat through `OriginalActorLifecycle::combat_enabled` (state3, enabled) and physical/collision presence in main.cpp. HP loss/AI target alone is insufficient proof.
- `CSDead::OnEvent` **0x3c4c3c** on End34 removes physical body and starts `CharacterDesign/Despawn_Delay` for non-player actors. Existing source delay is2000ms; death behavior was not modified.

## Reproduction and root cause

Normal production compiled script bank, real authored TriggerZone contacts, no campaign-command/start/condition injection. Fixtures copy the earlier privately generated post-opening `fix080/m7-skip/character.save` into fresh test directories and seed the starting world position at the relevant trigger. `--fresh-player` remains enabled: main.cpp:1284 loads that existing direct profile before bootstrap only under that option. Earlier missing-save/direct-bootstrap attempts overlapped Swamp_Intro and were discarded as combat proof. No accepted package or user save was written.

- Accepted v20 EXE **8BA1E537D33D4D4FD5B7DD9822C4CAB5080917FE2427A3903AB7C247827E8CCE**, own assets, run `accepted-v20-captain-skip`: skip80, then walk into boss range. Brown boss remains without plate/ring through1800, HP260, lifecycle1, physical0. No Damage line for captain. Exit0. Viewed the frame sequence.
- Private predecessor EXE `exe-91.exe`, **A95174FAE4BE07D552BAA263510635A8D32FC1105E3050E9FCC17274EF0C948F**, `compare-before-captain-skip`: same movement/input reproduces brown boss beside hero, no boss plate/ring, HP260, lifecycle1/physical0. It can follow the hero despite that gate. Its precise old build source receipt was not preserved; accepted v20 is the authoritative baseline.
- **Honesty correction:** `compare-before-captain-play` on that predecessor does fight and die when the hero approaches. Continuous played passivity was not reproduced. Initial stationary fixtures were out of range. The original B091 synthetic-command reproduction is not normal-route proof. Source clip completion is a fidelity/lifecycle correction; early skipped Spawn is the conclusively reproduced untargetability bug.

Root cause: End's non-waiting Idle changed CombatSession only. The lifecycle remained Spawn and retained no physical body, so targeting/combat permission stayed false. Separately, skip cleared scripted completion timers; the inherited B091 completion service excluded authored actors not spawned by a script (TRUNGUS) and state3 with a held scripted pose.

## Minimal portable changes

- main.cpp `hostServices.set_idle`: a non-waiting lifecycle actor Idle goes through `actorLifecycle.put_idle` before the ordinary session selection, preserving Spawn.Blur/body admission.
- Existing B091 `scripted_clip_finished`: apply to every registered gameplay actor with a still-held scripted pose, including held Idle; exclude animation-only props/companions, dead/Limbus/PreSpawn. Use the existing non-waiting Idle owner to clear all presentation/body suppression.
- CampaignHost: keep already-running Character clip completions across SKIP/presentation restoration; explicit non-waiting Idle cancels that actor's obsolete completion. Chained clips finish before releasing gameplay.
- No actor name/map/profile list, engine-platform dependency, or gameplay HP rule added. B080 gameplay actor classification remains the existing data-driven admission dependency.

## Historical pre-review tests and frozen runtime verification (superseded for source gate)

Defined before coding: no early release, chains/replacement, one completion, skip survival, failure/duplicate behavior; early Spawn body admission; real ring/plate plus hits both ways; legitimate death/body removal. Implemented tests in `campaign_host_tests.cpp` and `original_actor_lifecycle_tests.cpp`. Host tests cover retained prop refusal, explicit Idle cancellation, and completion failure/abort. Lifecycle tests assert one body admission before Idle.Focus, stable anchor, combat enable, and no second body on duplicate Idle.

Build `p14_build.ps1 -Name fix091 -Jobs 2`: exit0. PowerShell ctest with llvm-mingw DLL PATH: **167/168 pass**, only known `session_skill_binding` fails. Full output `audit/ctest-final.txt`; no other failure. Gameplay binary built from `e0ef9cf4`; added lifecycle tests at `c29918ab` do not change it. Later main changes are comments only.

Frozen tested EXE `audit/final.exe`: **1AF174C560396AEED6B67ACA784F6BB9BE069DD28A3B8808D348AC8795261F45**. Hidden `quiet_run.ps1`, `DH_AUDIO_SILENT=1`; every retained final run exits0/no timeout/no abort. The extra player HP1000@700 is explicitly a private combat-duration fixture; enemy HP/attack/damage/death are normal. No `--kill-test` or synthetic enemy HP0. That fixture causes reward XP maximum-consistency diagnostics, so XP correctness is not claimed here.

Receipt recorded **2026-10-11 18:57:06 UTC** in `audit/receipt.json`, including hashes of the EXE, ctest output and each retained run log/args. Boss Damage lines: played23 (5 enemy), early skip26 (6 enemy), late skip25 (6 enemy). Troll approach played/skipped each10 (3 enemy). Accepted v20 early-skip has0 captain Damage lines.

| Run | Observed final result | Legitimate kill / body release / hidden frames |
|---|---|---|
| `final-captain-play` | Bogwomp Chief ring/plate, mace/sword motions, Damage both ways |1327 /1388 End34 /1507|
| `final-captain-skip` (80) | Same ring/plate, both-way combat after early skip |1447 /1508 /1627|
| `final-captain-late-skip` (230) | Same after a scripted captain clip had started |1381 /1442 /1561|
| `approach-troll-play` | TRUNGUS ring/plate, hits both ways (enemy hit857), death pose and disappearance |897 /1037 /1156|
| `approach-troll-skip` (80) | Same, including enemy hit795 |897 /1037 /1156|

Captain capture frames180,300,600,700,800,900,1000,1100,1200,1300,1400,1500,1800 were inspected. Troll frames300,450,550,600,700,800,900,1000,1100,1200,1300,1400,1500 were inspected. Each run retains `job.args`, `run.log`, frame PPMs, contact `sheet.jpg`; summaries in `summary-final.json`/`summary-compare.json`. Initial troll stationary runs proved only lifecycle release or hits received; they are superseded by approach fixtures for combat claims. No unchanged passing long opening/moth scene was replayed.

## Explicit match statements / limits

**Matches video:** bounded post-captain and post-troll ring/plate, combat, legitimate death and disappearance. Scene action ordering matches the inspected captain captive-strike and troll walk/roar sequences. Exact visual lighting/FX/framing parity is outside this patch. Approximate captain corpse duration is consistent with483.5-486.5 footage.

**Matches IDA:** non-skipped script animation eventually returns to gameplay Idle; skip keeps new animation commands as no-ops; explicit skipped Idle leaves Spawn through physical admission; dead/retired actors are not revived. The port's timer callback for held Idle is a minimal implementation of gameplay release, not a claim of identical original callback structure.

**Not verified:** complete route from New Game through every scene; other user save/skip phases; continuous non-skipped B091 passivity; exact identity of B098 shot17. Witch/Serpent/Camp other skip paths were not replayed. Existing B080 moth/lizard evidence is reused; this establishes captain and TRUNGUS branches only. Original video v1.0.3 versus exported APK version differences remain possible. Audio unheard because runs are silent. B092/B098 user exact-case reproduction remains a separate caveat, although this actor's legitimate death and removal now work.

Package files required: none. Root must integrate/review and run the relevant frozen production checks before closing stable IDs or packaging.
