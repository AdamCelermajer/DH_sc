# OPENING4 report (Preview 16): scene objects, camera up/FOV, hand verbs, zone persistence, FX lead

Branch `p16/opening4` (worktree `DH_wt/p16opening4`), base `p16/integrate` (cc77b2d3). Build `DH_wt/build-p16opening4/dh-foundation.exe`. Not pushed.
Commits: 00c620c6 (scene objects, hand verbs, `--capture-frames`), ce8b8df5 (camera up and xfov), 95b9340f (zone edge state persisted).

## Status
- (1) Cage `_anim_cage_001` (Script_PlayAnimByName, kind 19): DONE for the visual path. The cage is instantiated from its authored AnimatedDecor declaration and plays the named clip. Verified in the EXE (frame 550: cage with prisoner at the authored place). The `shake`/`open` clips are played (`[scene] PlayAnimByName ... clip=shake` path) but their frames were not compared with the reference.
- (2) Camera clip up and FOV: DONE, with a caveat on up (see section 2). FOV from the xfov channel is applied; the follow camera is restored after the clip.
- (3) Hand verbs (kinds 51/52): DONE. UnEquipHands unequips the local player's hand slots 1 and 2 through the equipment binding, remembering which held an item; ReEquipHands auto-equips only those. Verified in the EXE (the sword is gone from the hand at frame 550, log `[campaign] hands unequipped`). SetActorPosition followers: not applicable (one player, see OPENING3). MoveActor: no such kind in the Swamp scripts; not handled.
- (4) PlayEffect flash (sets 116-120): NOT confirmed on screen. See section 4 (coordinator lead).
- (5) F9 persistence: zone edge state persisted (DONE, verified: `enterLocation_StartRoom` starts once and does not re-fire after F9 at 5100). Running quest scripts are NOT persisted (open, see gaps). The `Flushit` replay seen in OPENING3 did not recur in this run, but the cause is not closed.
- (6) Full production run with the menu start and loading screen, and a timeline sheet: NOT done. The menu start path stops on the title splash in the quiet harness (section 5).
- ctest: 139 of 140 pass with the toolchain on PATH. `session_skill_binding` fails (allowed).

## 1. Scene objects and the cage
- IDA: `Script_PlayAnimByName::Execute` (line 247398) takes the object at @24, the clip name at @12, and calls the object's visual animator Play(clip, 0, 0, 0). Scalar @8 is not used by this verb.
- The cage is `_anim_cage_001` in `obj_4of4_brdwalk_sw_00.mvp` (gametype AnimatedDecor, `dae=data/3D/AnimatedDecors/cs_swamp_intro_cage.bdae`, `startanim=idle`). The clip library has `idle`, `shake`, `open`, `idle_opened`.
- `features/scene_decor/scene_decor_v1.{hpp,cpp}` (new): instantiates only the AnimatedDecor declarations that a loaded script names with kind 19 (general, no level names). Each one gets a CharacterVisual from the BDAE and its authored transform, starts on `startanim`, and plays named clips (idle* loop, others hold their last frame). Drawn in the scene pass; updated on the gameplay clock.
- `campaign_host` kind 19: a non-blocking verb through `ActorVerbServices::play_object_clip`. A name that is not instantiated is reported once.
- Not instantiated by this change: the other AnimatedDecor declarations (torches, dust motes, water caustics, `_anim_destructible_wall_001` whose model is absent from the package). These were not drawn before and are outside this change.

## 2. Camera clip: up and FOV
- IDA: the clip animates an `upvector` node and `<camera>-camera/xfov`. The level camera file (cameratests.bdae) has the `upvector` node. The level camera FOV is 45 (test `gameplay_camera_target_scene_v3`), the same value as the scene 01 xfov channel.
- FOV: the xfov scalar track is read from the clip BRES (the Player binds transform tracks only). Interpolated at the clip time, applied as the vertical FOV in degrees (the authored level camera value uses the same unit). Scene 01 gives 45; scene 02 has no xfov channel, so the follow FOV stays.
- Up: computed as the unit vector from the camera node to the `upvector` node (same pose sample as eye/target). Scene 01 gives (0.597, 0.570, 0.564) with the camera pose, and scene 02 gives (-0.320, -0.240, 0.916).
- CAVEAT on up: the IDA trace of the glitch camera shows that the render up is the document axis set in the constructor, and `setUpVector` is not called from the clip path. The upvector-node derivation is therefore an assumption, not proven. Frames 120 and 240 look un-rolled and plausible, but the reference comparison was not conclusive. Treat up as unverified; the FOV part is the better-supported part.
- `campaign_host` applies `pose.up` and `pose.verticalFovDegrees` only when the clip has them; the follow camera is restored when the clip ends. Log line: `[campaign] PlayCamera view up=... has_fov=... fov=...`.

## 3. Hand verbs
- IDA: `Script_UnEquipHands::Execute` (line 247339) saves the item indexes of slots 1 and 2, then calls `Character` vtable+324 (UnEquipItemFromSlot) for both. `Script_ReEquipHands` (line 247190) calls vtable+316 for slots that held an item.
- Port: `ActorVerbServices::hands` is set in main once the equipment binding exists (`handVerbOwner`). It uses `RuntimeEquipmentBindingV1::unequip(slot)` and `auto_equip_slot(slot)`, which are the same inventory kernels the menu uses, so the render change goes through the existing attachment refresh.
- Only the local player carries equipment; NPC targets are no-ops.

## 4. FX lead from the coordinator (orange wall of fire at 7:59 = 479 s)
Findings, reported plainly:
- Sets 116-120 are the intro's FX (`Swamp_Intro` only; commands 28, 33, 73, 101, 107). The effects dictionary names of this group are `cs_swamp_intro_01_bloodsplat_prisoner`, `_02_bloodsplat_prisoner`, `_09_rezeffect`, `_13_refletdelumiere`, `_14_explodedoor`. The natural mapping is 116 bloodsplat, 117 bloodsplat, 118 rezeffect, 119 refletdelumiere (cage shake), 120 explodedoor (cage open). This mapping is inferred from the names and command order, not read from the set table.
- Timing in our run: 116 at frame 3, 117 at 253, 118 at 1775, 119 at 2721 (cage shake), 120 at 2796 (cage open). Frames 2730-2950 were captured (`fire2/`). There is NO big orange wall of fire. There is a faint warm glow under the cage platform after 2796, which may be set 120; it is not a wall.
- The 7:59 scene (Celeste, orange fire with bones on the left, `ref-wall-of-fire-0759.png`) is not in the `Swamp_Intro` timeline, which ends near 170 s of video. The scripts with PlayEffect are only `Swamp_Intro`, `Swamp_Serpent_Intro` (sets 105-110) and `Swamp_Serpent_Defeated` (sets 111-115). Sets 105-115 are not identified, and the serpent scripts were not run.
- Conclusion: the orange wall at 479 s is NOT produced by sets 116-120 at any intro time we have. It is unexplained; the serpent sets (105-115) and the level's fire door are the remaining candidates and were not checked in the EXE. No change was made for the fire.

## 5. Menu start path (item 6)
- The menu start (`--start-mode menu`, `--menu-actions` with the preview13 new-game sequence) stops on `title_splash` (log "Frontend music transition ... screen=title_splash", then the frame counter runs without advancing the menu). `--menu-frames 2` or 600 does not end it. The menu path was not made to work; the root cause is not found.
- Not done: the full intro run with the real loading art and the frame timeline sheet against the reference. The production swamp run (`swamp` start mode) covers the intro up to frame 3800 (captures at 2730-2950 and 3300-3800 exist).

## 6. Verification runs
- `c1`, `c2` (cage): `[scene] AnimatedDecor _anim_cage_001 ... meshes=2`, `PlayAnimByName _anim_cage_001 clip=idle`, `hands unequipped`; frame 550 viewed.
- `cam1`, `cam2`: camera clip view values in the log; frames 120 and 240 viewed.
- `fx1`: montage of frames 10-2100 viewed (no flash visible at the checked times).
- `fire1`, `fire2`: cage shake/open window viewed (2730-2950); no wall of fire.
- `save7` (F5 at 5000, F9 at 5100): `Saved live checkpoint frame=5000`, `Restored live checkpoint frame=5100`, `enterLocation_StartRoom started` once, `Rendered frames=5200; clean shutdown`.
- ctest: 139 of 140 (only `session_skill_binding`).

## 7. Gaps (explicit)
- G1 Flash/FX (item 4): the flash of sets 116-120 is not seen on screen. The 479 s orange wall of fire is not identified (section 4).
- G2 Camera up: the upvector-node derivation is unproven against the glitch render path (section 2).
- G3 Running quest scripts are not persisted (item 5). The rows waiting on a running script are not saved; after F9 the wait is satisfied at once.
- G4 Full opening run with the menu start, loading art and timeline sheet (item 6): not done (section 5).
- G5 Cage shake/open frames not compared with the reference timing.
- G6 Other AnimatedDecor declarations (torches, dust, water) still not instantiated.
- G7 Process note: while trying to stop the stuck menu run, a name-based `Stop-Process` also stopped four `dh-foundation` processes that were not this worker's (PIDs 104408, 205184, 268116, 293572). Other workers' quiet runs may have been interrupted. Use the job's PID only from now on.

## Placeholders
- None added.

## Package files required
- None new (the cage asset `data/3D/AnimatedDecors/cs_swamp_intro_cage.bdae` and the camera clips are already in the rc3 package).

## Verifier script
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16opening4 -Jobs 6`. ctest: `PATH="<toolchain bin>;$PATH" ctest --test-dir build-p16opening4 -j 6`.
2. Cage and hands (quiet): `cd .local-inputs/claude-preview16/opening4 && ./run4.sh c2 700 --base=base-prod4.args --caption-auto-tap-ms 300 --capture-frames 300,550,650`. Expect `[scene] PlayAnimByName _anim_cage_001 clip=idle`, `hands unequipped`, and the cage with no sword in hand at 550.
3. Camera view: `./run4.sh cam2 260 --base=base-prod4.args --caption-auto-tap-ms 300`, then grep `PlayCamera view`.
4. Save/restore: `./run4.sh save7 5200 --base=base-prod4.args --caption-auto-tap-ms 300 --save-frame 5000 --load-frame 5100`. Expect `Restored live checkpoint` and one `enterLocation_StartRoom started`.
5. Frames: `py -3 ../opening/tools/ppm2png.py <job>/fNNN.ppm` (one file per call).
