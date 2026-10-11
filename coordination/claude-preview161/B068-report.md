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
