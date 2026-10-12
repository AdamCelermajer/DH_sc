# B151 report: hero breaks a vase with his back turned

Status: **root cause found and fixed** (one generic heading-convention bug in the shared interaction-facing helper). Runtime-verified before/after in hidden windows with real input and isolated save copies. Tracker row B151 is not edited (per the brief). This replaces the earlier report-only version (commit 663e9a8a), whose leads are resolved below.

Branch `v21.6-vase` (base `488d9e82`). Build: llvm-mingw clang++ 20261006, CMake 3.22.1 + Ninja, Release, `subst Q:` alias of the worktree, build dir `Q:/.local-inputs/v21.6/build` (gitignored).
- Before EXE: `.local-inputs/v21.6/exe/base-v216.exe`, SHA256 `5BAA61331B7D293FCDE362FEE3626AFBA5ED0DB6AF5FD5729F00887AF4CAC1EB` (source 488d9e82).
- After EXE: `.local-inputs/v21.6/exe/b151-fix.exe`, SHA256 `DA518C6C8C522298498541BCFF0C6174187D3759EA896B50B3F4D1B3EB971597` (this commit's source).

## 1. Original behaviour

### Visual (direct observation)
Reference `dh2_video_research/video/part1_z_Zky7qQdYs.mp4` (v1.0.3, 640x360, 30 fps, no audio). Two swamp green urns on a boardwalk are broken at **P1 11:35.1** and **11:38.3**. Dense 10 fps sequence 11:34.0-11:39.5 under `.local-inputs/v21.6/scratch/video/seq_p1_1134/`; committed tiles `B151-evidence/original-P1-1134.4-1135.2-urn1.jpg` and `original-P1-1137.7-1138.5-urn2.jpg`.
- 11:34.4-34.6: the hero stands next to the urn (red target ring under it), chest toward the camera, sword upright.
- 11:34.7: shoulder turned toward the urn (screen up-left), sword low on his right.
- 11:34.8-35.0: the normal overhead diagonal weapon slash (same blue trail as his attacks on the Muck Fly at 11:36.4). The trail sweeps across the top of the urn and ends pointing down-left past it; his body leans toward the urn.
- 11:35.1: the urn shatters. Wind-up to break takes about 0.4 s. Urn 2 shows the same turn and slash.
- Distance: his body almost touches the urn's ring, a gap of about 0-0.3 hero body widths.
- No dedicated break pose. The hero faces and strikes **toward** the object.

### Logic (IDA `pseudocode-all.c`, APK 1.0.2)
- `AISDefault::OnTargetInMeleeRange` 0x3dc284: for a non-enemy target in `AI_IsInInteractionRange`, it calls `AI_InteractWith`.
- `CharAI::AI_InteractWith` 0x3cff34: `SM_SetInteractState(type=GetInteractionType(), lookAt=1, target)`. The destructible type is 8.
- `CSInteract::OnFocus` 0x3c5548: if not `IsCharSitting` and lookAt is set, it calls `Cmd_LookAt(target)`.
- `Ctrl_LookAt` 0x3addbc -> `GameObject::LookAt` 0x393cec -> `GameObject::LookTowards` 0x393b1c.
- `AISDefault::OnAttack` 0x3dc12c: on the swing hit, a target that is not a Character gets `target->Interact`, i.e. the vase hit (B085).
- **`GameObject::LookTowards` 0x393b1c** writes yaw (GameObject+376) as follows:
  - if dy == 0: dx > 0 gives `0x3FC90FDB` (pi/2) and dx < 0 gives `0x4096CBE4` (3pi/2);
  - otherwise `atan(dx/-dy)`, plus or minus pi when dy > 0.
  - So **source yaw 0 faces -Y**, and yaw = atan2(dy,dx) + pi/2. The recovered port function `dh2_nav_look_towards` / `dh2_character_look_at_point` (`port/level-world/navigation_heading.cpp`, `character_path_commands.cpp`) matches this and is differential-tested.
- Every other writer and reader of the hero's `transform.rotation[2]` in the port uses this source convention:
  - the source movement/turn kernel;
  - `injured_blur_look_at` in main.cpp, which stores `dh2_character_look_at_point` output directly;
  - the renderer `sourceActorWorld`, whose matrix maps local X to (cos, sin) and local Y to (-sin, cos). The character mesh faces local -Y, i.e. world (sin yaw, -cos yaw).

## 2. Reproduction and root cause

### Root cause
`features/interactions/interaction_facing_v1.hpp` (shared by B085 destructibles and B079 openables through `ctxFaceContainer`) computed `yaw = atan2(dy,dx)`. Its comment claimed a "+X-forward" convention that the port does not use. The hero was therefore turned **a quarter turn clockwise away from the target** (facing direction = target direction - 90 deg). At the game's camera this reads as swinging with the back or side to the vase, and the slash goes away from it.

The earlier session's two unexplained observations follow from the same convention:
- **"The approach walk's yaw trends away from the walk direction"**: not a bug. The walk kernel settles at -1.604 while walking toward a vase at world direction 3.109, which is exactly 3.109 + pi/2. The walk is drawn correctly (the walking frame 184 shows his back toward the vase). The arrival turn then overwrote that correct heading with atan2 = 3.108, a quarter turn off.
- **"Facing changes but the body does not follow"**: the body does follow the yaw, but the yaw was wrong. No pose or root-motion override was involved; the drawn yaw equals `rotation[2]` throughout (verified by the earlier session's debug print and by the frames below).

### Runtime evidence (hidden window, real menu clicks and Space, isolated `presave` copies, `--player-to` placement test aid)
Harness: `.local-inputs/v21.6/verify/tools/b151.ps1`, which wraps the copied `run-case.ps1`. Case dirs are `.local-inputs/v21.6/verify/cases/{B,A}-*` and hold full-size `f*.png` plus 2x crops `f*z.png`. Committed sheets in `coordination/v21.6/B151-evidence/` show the before row above the after row.

| Case | Placement | Before (base EXE) | After (fix EXE) | Sheet |
|---|---|---|---|---|
| E120 in place | 120 east of `_prim_DestructibleContainer` (-1229.64,-1224.21) | turn 0 -> 3.14159 at 168. Frames 176-188: body faces screen up-right and the arc goes up-right, away from the vase, which is behind his left. Hit 184, open 200. | turn 0 -> 4.71239 at 169. Frames 173-188: back to the camera facing up-left at the vase; the arc goes overhead and sweeps over the vase down-left, as in the original. Hit 185, open 201. | `E120-in-place.jpg` |
| A400 approach | 400 east, approach walk | walk heading -1.604 (back toward the vase, correct). Arrival 189 turns to 3.108; frames 199-208 swing up-right, away. Hit 205, open 221. | arrival 189 keeps -1.6041 (no extra turn). Frames 199-208: overhead arc over the vase, ending down-left past it. Hit 205, open 221. | `A400-approach.jpg` |
| N120 north | 120 at +Y of the vase (vase is screen down-left) | turn 0 -> -1.5708: faces screen up-left with his back to the camera and swings away. | 0 -> 0: already facing the vase, so no turn is needed. He faces the camera and lunges down onto the vase. | `N120-north.jpg` |
| Chest (B079 path) | 100 at +Y of `_prim_OpenableContainer_2` | turn 0 -> -1.5708: the interaction crouch faces screen up-left, beside the chest. | 0 -> 0: the crouch and lunge face and reach over the chest. Interact marker accepted at 198, open 212, one loot drop. | `chest-side.jpg` |

In every case the hit, sound (ordinal 325, `sfx_break_vase.wav`) and open timing are unchanged.

## 3. Change (generic, no names)
- `features/interactions/interaction_facing_v1.hpp` now computes the yaw with the recovered source `dh2_nav_look_towards` (GameObject::LookTowards). Coincident XY now always leaves the yaw unchanged, as the source does. The B085-only `preserve_coincident_heading=false` option set yaw 0, which was arbitrary and not source behaviour, so it is removed. Non-finite input still leaves the yaw unchanged. This one function serves every destructible (vase, barrel, crate, any type 8) and every openable container.
- `main.cpp`, 2 lines in `ctxFaceContainer` / `ctxUseContainer`: the call drops the removed argument, and one comment is updated.
- Tests:
  - `interaction_approach_v1_tests.cpp` gains source LookTowards constants and quadrants, a rendered-forward-faces-target check (the `sourceActorWorld` matrix, local -Y), equality with `dh2_character_look_at_point`, and the R6 runtime fixture (the arrival keeps the walk kernel's -1.60421). It is now registered as CTest `interaction_approach_v1`; before, it was only built by `run_context_p16_tests.ps1`, which is updated to link the two level-world sources.
  - Against the old header, 15 of the new checks FAIL (compiled in scratch `.local-inputs/v21.6/scratch/oldfacing/`). Against the fix, all 34 PASS.

## 4. Tests
- Focused: `interaction_approach_v1_tests` 34/34 PASS.
- Full CTest (`ctest -j 4 --timeout 300 --output-on-failure`, build `Q:/.local-inputs/v21.6/build`, same source as the after EXE): **180/180 passed** (the 179 existing tests plus the new `interaction_approach_v1`), 307 s. Log: `.local-inputs/v21.6/ctest-b151.log`.
- The worktree's `.local-inputs` (gitignored) now holds directory junctions to the primary checkout's `.local-inputs` subdirectories (read-only test assets), the same as the sibling v21.6 worktrees. Without them, the asset-dependent tests fail with "Asset root is not an existing directory". An earlier attempt without the junctions was discarded.

## 5. Verified vs not
- Runtime-verified (real EXE, hidden window, real input, isolated saves): the correct turn and slash direction toward the vase from the east (in place and after approach) and from the north; the chest interaction now faces the chest; hit, sound and open are unchanged. These are frame sequences, not single stills.
- Unit-only: the other quadrants (west and south placements are inside exit zones in this map area, so they were not run); coincident-XY and non-finite branches.
- Compared with the original: the turn toward the target and the overhead slash crossing the object match P1 11:34.7-35.1.

## 6. Remaining gaps (not changed here)
1. **Strike distance.** After the approach, the hero swings from about 326 units from the vase centre, about one body length away (frames A400 205-208); the original stands almost touching the urn. The type-8 gate is the source `AI_IsInInteractionRange` 0x3d4f98 branch `v = d - meleeRadius - targetRadius <= 0`, so this is not the "openable interaction range" the earlier report suspected. The gap is in the radii data (player melee reach and/or the container interaction radius), which were not audited. This is a separate fidelity candidate.
2. **Same convention in campaign `LookActor`** (`hostServices.actor_verbs.face`, main.cpp about line 3087) and its population-actor branch: they also use `atan2(dy,dx)`. Scripted cutscene clips add +pi/2 when drawn, which may hide the error during scripted poses only. Not verified visually and not changed here, because cinematics and witch code are owned by other sessions. Recommend a separate check.
3. The B079 report's "+X-forward" claim and its chest facing review (yaw -1.5708 judged as facing the chest) were wrong. The chest is fixed by the same change and verified above. Exact original chest crouch framing beyond facing direction was not re-compared.
4. The swamp seed limitation from the earlier report remains: slot1 resumes in the witch cave, so tests use `presave`. Placements (-1349.64,-1224.21) and (-1229.64,-1344.21) are in exit zones.
5. Version difference: APK logic is 1.0.2 and the footage is 1.0.3.
