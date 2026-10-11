# B068 cinematics report (continuation session)

Evidence: reference video z_Zky7qQdYs (Part 1) opening cutscene t=75-205 s, compared as frame sequences (2 fps video vs our captures every 30 frames at 60 fps; local t0 = video 74.85 s). IDA: libDungeonHunter2.so CameraLevel::__Callback 0x?(line 195941: only clears the playing flag), CameraLevel::PlayAnim, CameraLevel::Update (camera rig follows target; the clip does not reset on end), MenuMessageManager<DialogMsg>::FlushEnqueuedMessages 0x383ea8 (pops the pending deque only).

Earlier commits of this branch: input lock, SKIP consume, enemy AI freeze + lizard heading, caption variants/name plate/tap, New Quest hold 2.0 s, cutscene hero/troll frame, SetActorPosition/SetCameraClip/DialogBox tables.

This session (all verified by before/after frames in the real EXE, quiet run):
- Camera clip xfov is horizontal: vertical = 2 atan(tan(xfov/2)/aspect). Before: view far too wide (opening scene 1 now matches video 77.0 s framing).
- Prisoner (population-only actor with scripted cs: clip) was drawn unrotated; now turned like session actors: dragged by the troll, thrown, lying as in video 77-80 s. Hero scripted yaw taken from actor heading.
- Intro cage (AnimatedDecor) bound no textures: bars/padlock were flat grey; now textured (video 90 s).
- A finished PlayCamera clip keeps its last pose during the cutscene (video 96-100 s Rene dialogue camera); before it fell back to the overhead follow camera.
- Caption text was ~8% left of its plate (global local_bounds); now centred (video captions centred).
- EnterLocationDialog title "The Boglands - Ancient Prison": text only (no box), drawn during the cutscene, auto-hides, survives Script_FlushMessages (video 76-78 s). The auto-hide is video evidence only (Flash isSkipable bit for style 3 not read).
Tests: ctest 139/140 (session_skill_binding fails in worktrees as expected); cinematic_runner test extended.

Remaining gaps / unverified:
- Faery drawn as flat white silhouette; video shows a golden additive glow (video 96-130 s). Not fixed.
- Moth/bat sprite beside the cage at video 90-91 and 133-136 s absent in ours.
- Skin tones look more saturated red than video (lighting, B076 area).
- Title re-fires at the end of the opening (zone re-entered after teleport from z=0); not in video. Cause unknown.
- Scene 4 hold in video ~4.5 s vs ours ~5.5 s; unresolved.
- NOT audited this session: MothIntro, LizardMan ambush (frames re-check), CombatTuto, Movement_Tuto2, Camp_Intro, LizCaptain_Intro, Swamp_Witch/Serpent, TrollReturn. Only the opening was compared frame by frame.
Package files required: none new.

## Fresh v20 continuation: investigation before changes
Opening video sequence `v20audit/opening.jpg`, t85..142 at 3 s spacing, inspected 2026-10-11: Celeste's body/wings are white and a large golden halo follows her across camera angles (94..130 s). Before runtime `audit-opening-before/f1500.png` reproduces white body/wings with faint separated light streaks, without the central gold halo. Source `VisualObject::VisualObject` 0x472a0c (pseudocode lines 262150..262247) loads/registers the entire model scene and disables the `_colbox_` mesh; it does not admit only controller meshes. Current `CharacterVisual::build_parts` only admits rigid meshes when explicitly configured, while population customization and rebuilt session configurations default that setting off. Expected reusable change: include authored rigid visual instances in source population actors and keep collider ancestry excluded. Verification defined before coding: actual Celeste model gains its rigid additive halo mesh, collider meshes remain excluded, opening before/after frames show attached gold glow; run material tests plus real production opening with isolated save. Material tint/halo intensity must come from original asset, never a faery-specific recolor.

Camp safe-zone command is already routed in this branch (`CampaignHost::bind`, safe_zone provider); audit will verify the production Camp script instead of treating older P17 unsupported notes as current evidence.

Title re-fire root cause evidence: original `TriggerZone::Update` 0x39b458 returns before contact updates when local player's Character+5248 is set. `Script_MarkCharacterAsScripted::Set` 0x45958c and Unset 0x459550 write exactly that byte; Execute 0x45d4c8 chooses Set/Unset. Our main qualified trigger updates solely on player alive, so cutscene root/teleport z=0 produces a false zone leave, then end teleport re-enters and repeats location title. Expected fix: qualify contact updates using the same existing host scripted-character set. Test: full played and skipped opening each emit location title once; free-play leave/re-enter still fires repeatable zones.
Rigid halo implementation verified in production: `audit-opening-rigid/f1500.png` gains gold halo from the original static additive mesh; before frame `audit-opening-before/f1500.png` lacks it. Quiet EXE run exit0/2100 frames. Matches BOTH video appearance (gold halo, t94..130) and IDA model-scene admission 0x472a0c. Brightness and exact camera-facing orientation remain under full sequence review. No new asset required.
