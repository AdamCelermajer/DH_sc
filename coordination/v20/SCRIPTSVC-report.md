# v20 SCRIPTSVC report

Worktree `DH_wt/v20scriptsvc`, branch `v20/scriptsvc` from `v20/integrate` (107f2a86). Final HEAD `4b689ece`.
Frozen executable: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/scriptsvc/final/dh-foundation.exe`
SHA256 `07B3854ED415C5AE9A0C80ED879CC5D2EE583CAE78D52735054812C0B8C16109`, built from HEAD `4b689ece` (`build-v20scriptsvc`, build exit 0).

Status in one line: PlaySound/StopSound and the regular SaveGame now execute in the real compiled-bank path and are verified in the Witch cave. The swamp SaveGame is blocked by a live-state mismatch on an NPC, so the swamp cutscenes abort at SaveGame. Part 2 lifecycle branches are verified by logs, and the visual check is inconclusive. Part 3's positive "Rene follows when fresh" is verified only on an earlier build (see Gaps).

## 1. Investigation

Sources: IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c` (APK 1.0.2). Visual consumer: the Witch cutscene and Serpent/Opening sequences already inspected in the B068 report (reused; not re-inspected here, so the claims below are logic evidence, not new video evidence).

- `Script_PlaySound::Execute` 0x45fefc (l.248329): music (field12 != 0) calls `VoxSoundManager::PlayMusic(ordinal, loop, field8!=0, 2000)`; plain calls `VoxSoundManager::Play(ordinal, loop, delay=field8, group=field12, a6=field12)`. Field12=0 for plain, so no `SetGroup` (group 1..29 only).
- `VoxSoundManager::Play` 0x36b80c (l.79636): playback `VoxEngine::Play(emitter, XML loop flag, a4/1000)`. The script loop flag does not decide playback. Gate: `a2>=0 && (a6 || !online || !IGM)`.
- `VoxSoundManager::PlayMusic` 0x36bd78 (l.79801): ordinal -1 + change -> `StopMusic(fade)`; same ordinal -> resume 0.05 s; different ordinal -> stop previous with fade, then `Play(ordinal, loop, 2000, group 2)`. Gate: music volume >= 0.5 (not modelled, see Gaps).
- `Script_StopSound::Execute` 0x45fe2c (l.248304): never skip-suppressed; music -> `StopMusic(field8)`, plain -> `Stop(ordinal, field8)`.
- `Level::Update` 0x3f82d8 (l.179953): level music is started once, guarded by latch `this+324`; `SetInSafeZoneMusic` (0x36c014, l.79880) and revive start it explicitly. Per-frame re-assertion is not in the source.
- `Script_SaveGame::Execute` 0x45c994 (l.246757): regular save, not checkpoint. Order: `GetCurrentLevel` -> `SG_BlockLevel(false)` -> `QuickSave(false)` 0x3f059c -> `GetLocalPlayer` Character -> `SG_Block(false)` -> `SG_Save`. Ran during skip (not suppressed).
- ObjectBase conditions: `TestEnableCondition` 0x33e6d4 (l.41162-41197) and `TestDisableCondition` 0x33e61c (l.41138-41152), ObjectManager loop l.52815-52836.

Behaviour decisions from that evidence:
- SaveGame kind 69 maps to the regular save order, never to the checkpoint cache.
- PlayMusic/StopMusic reuse the existing shared music voice (`play_level_music`/`stop_level_music`). Plain Play reuses the menu's verified fresh-emitter body (gain, pitch, group from the sound row); `delay` is the source's `a4` ms, converted to frames.
- Level music is an explicit-start event (pending on new level, safe-zone track change, revive). The source starts it once; the port previously re-asserted it every frame, which overrode scripted music on the next frame (observed: "Level music transition: kind=switch" right after scripted PlaySound in the first run; absent in the final run).

## 2. Implementation (commits on `v20/scriptsvc`)

- `a36939e2` plain submit accepts an optional delay (default 0; existing callers unchanged).
- `daf662b7` audio: `RuntimeAudioHostV1::submit_script_plain_sound`/`stop_script_plain_sound`; `RuntimeSessionAudioV1::script_play_music`/`script_stop_music`/`script_play_plain`/`script_stop_plain`; level-music pending latch.
- `2d61bfbc` campaign host: kinds 13/14 decode music/loop/fade/ordinal and call `execute_script_play_sound_v1` / `execute_script_stop_sound_v1` (skip rules in the services), with dispatch logs. `script_source_services_v1.cpp` moved into `foundation_data`; its test target registered.
- `678b3bd8` main.cpp (localized to script binding and the condition resolver): binds the sound owners (no-op when audio is off) and the SaveGame owners. `quick_save` gates on a live non-dead local player and writes the level world via the regular `saveLevelWorld`; `save_character` writes `character.save` from the captured live character. Also `--condition-active/--condition-inactive` now apply to the Character enable resolver (test aid, same as the door path).
- `bd8f16c0` + `4b689ece`: `game_save.cpp` sheet-mismatch error now names actor/field/raw/sheet. The rule relaxation tried in `bd8f16c0` was reverted in `4b689ece` because `tests/combat_session_tests.cpp` requires a targetable actor with an absent vital sheet to be rejected. Net rule change on the branch: none.
- `c9415e75`: animation-only actors are skipped by the AI command loop and by the enemy controller owner selection (they reject `command_request` by contract). Adds `CombatSession::is_animation_only`. Without this, the leave-safe-zone target assignment aborted the frame with "Animation-only actor rejects combat commands".

## 3. Isolated tests

- `script_source_services_v1_tests.exe`: exit 0 (skip/plain/music/stop/save ordering and failure-prefix cases).
- `campaign_host_tests.exe <assets>`: exit 0.
- `combat_session_tests.exe <windows-shared-assets>`: exit 0 on the final tree.
- Full ctest on the final tree: second run `99% tests passed, 1 failed out of 162`; the one failure is `session_skill_binding` (known worktree-only asset root). The first parallel run also showed `save` and `schema_v4` failing; both pass when run directly and in the second parallel run, so that was parallel-load flakiness.

## 4. Integrated runtime

Final exe, quiet hidden silent runs via `tools/quiet_run.ps1` (DH_AUDIO_SILENT=1), fresh directories, outputs under `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/scriptsvc/final/`. Each dir holds its `job.args` (one arg per line).

| run | args (key) | result |
|---|---|---|
| `witchcave` | ce8 Witch cave args (no source-root, no condition override, `--campaign-triggers`, audio on) | exit 0, 1200 frames. `PlaySound dispatch` music=1 ord 22 fade 2000 @3; plain ord 343 @3; plain ord 617 @543; music ord 213 @739, skip=0. `SaveGame` @740 -> `Level world saved reason=script-quicksave` -> `SaveGame wrote profile=character.save`. Audio voices started. |
| `opening` | ce8-opening args | exit 0, 2400 frames, clean shutdown. |
| `npc05-true` | fb1 before_kill args minus nothing (keeps `--source-root-scopes`) | exit 0, 2000 frames. `_prim_NPC_05 deactivate=true enabled=true`. SaveGame aborted (see Gaps). |
| `npc05-forced-false` | as above + `--condition-inactive IsBefore_Swamp_KillWitch` | exit 0, 2000 frames. `_prim_NPC_05 deactivate=false tested=11 enabled=false`. Forced override, not a natural quest close. |
| `q8-fresh` | npc05 args minus the `--source-root-scopes` pair; `--fresh-player`, no save | exit 0. `RENE_FOLLOW/PriestGood activate=true enabled=true`. Rene `waiting` in this final run (SaveGame abort precedes him). Earlier clean run on build 8 (same args) logged `rene running` at frame ~957. |
| `q8-resume` | as q8-fresh, `--fresh-player` kept, `character.save` copied from a previous run with CQPG quest row 8 patched 0 -> 13 | exit 0. `PriestGood activate=false enabled=false`. Rene `waiting: not enabled in the level population`; never runs. |

CQPG layout used for the patch: bucket 0 (origin 2), state index 8, offset 63 from the `CQPG` magic. Quest row 8 is the authored `RENE_FOLLOW` quest per the condition table (op 1, threshold 13: active while `13 > state`).

### Launch-arg answer for the packaged default (Part 3)

Required: the normal arguments plus `--campaign-triggers`.
Must not appear in the packaged default: `--condition-active RENE_FOLLOW` (present in `claude-preview17/procgen2/a-ov-rc3menu/startup.args` and the preview16 startup args), any `--source-root-scopes` (the source root derives from the active level; the fixed Swamp override is wrong after transitions), `--campaign-commands original-campaign.xml`.
Correction to the brief: resume in this port uses `--fresh-player` with an existing `--save` profile. Without `--fresh-player` the profile is not loaded into the session. So a resumed save launch keeps `--fresh-player`; a true fresh start is `--fresh-player` with no save file.

## 5. Visual verification

- Opening (`opening/f900` vs `ce8-opening/f900`): the caged figure, lock block, and moth at left are present with the same composition in both. PSNR against the reference: f300 51.4 dB, f900 35.7, f1200 39.9, f1500 34.1 (this comparison used the build-8 opening run). The later divergence is consistent with animation timing; I did not establish a cause.
- NPC05 (`npc05-true/f2000` vs `npc05-forced-false/f2000`): both show the same cage/dialogue scene. A possible difference at upper right is not conclusive and I did not establish that it is NPC05. Visual NPC05 visibility is NOT verified. The lifecycle branch (enabled true vs false) is verified from logs only.
- Witch cave: no frame inspection beyond the captures existing at the run's frames; the PlaySound/SaveGame claims are log/file evidence, not heard or seen audio (silent runs).

## 6. Part 2 world-gate audit: findings

Agreed with source: enable before disable; false activation -> SetEnable(0); false deactivation -> SetEnable(0) and latch; true deactivation does not re-enable; latches reset on bind; initial enabled=true.

Open (not implemented):
- PlayerSave guards: `TestEnableCondition` proceeds only when the local player's PlayerSave (`+1632`, then `+5352`/`+20`) is ready; `TestDisableCondition` has the same guard (l.41140-41145, 41171-41172). Not modelled. Argument for equivalence in this port (unverified, [inf]): the local player and its save owner exist before any bind or flip, so the guard is always satisfied at the evaluation points. Not proven.
- Current-level guard: `TestEnableCondition` sets SetEnable(0) when object `+236` > `CurrentLevel+280` (l.41177-41181). Not modelled; the authored level index for these objects is unknown.
- Per-update re-evaluation: source runs both tests on each ObjectManager pass for objects not yet latched. Adapter runs them at bind and on watched flips. Equivalence holds for pure quest-state inputs, because such inputs change only at flips. It does not hold for an externally changed enable state (e.g. a script hides a gated actor while its activate condition stays true). Not implemented; the case is not exercised in these runs.

## 7. Remaining gaps (stated, not hidden)

1. Swamp SaveGame is blocked. `validate_game_save` rejects the NPC `_prim_NPC` (actor 2500177786731750928, HP 600, targetable, attacks 1, resource sheet -1, live max_resource 0). The live actor state is inconsistent with its sheet, and the existing test forbids relaxing the rule. Fixing the live-state owner (resource initialization for resource-less attack actors) is the next step; it is outside this scope. Consequence: Witch activation and the swamp cutscenes abort at SaveGame in the final build. The Witch cave, where no such actor exists, saves correctly.
2. QuickSave's checkpoint-cache copy (source 1468) is not made; the level-world write is the regular LevelSavegame path. Flagged as partial.
3. Character save block (`SG_Block`) has no owner in this build; unblock is a no-op.
4. Music volume gate (`GetSoundVolume(2) >= 0.5`) and online/IGM gates are not modelled in the scripted path (offline single-player assumed).
5. Plain-Play volume group: source `SetGroup` is skipped for field12=0; the port uses the sound row's group (menu-verified approach). Inference.
6. VXN (format 2) plain rows fail closed (no source music-state provider for plain Play).
7. Skip behaviour: covered by the services unit test only. No runtime skip run was made.
8. NPC05 natural false branch: not reproduced. The earlier fb1 "false" result (`value=false`) does not reproduce: same args, fresh directory, the frozen fb1 exe now gives `value=true`. The cause of the earlier divergence is unknown. Natural false needs the Witch quest to close (kill route); not attempted.
9. Part 3 positive fresh follow: observed (`rene running`) only in build 8 (clean run, SaveGame succeeded via the since-reverted rule). The final build aborts before Rene runs, so the final-build fresh-follow observation is pending gap 1.
10. `session_skill_binding` fails in worktrees (known).
11. Opening PSNR comparison used build 8, not the final exe (the opening path reaches no SaveGame within 2400 frames; the final opening run is clean).
