# B151 report: hero breaks a vase with his back turned

Status: **investigation only, no code fix committed.** The logic that turns the hero toward the vase was verified to run and to reach the renderer. The visible body orientation during the swing is still unexplained, so I did not commit a speculative change. Tracker row B151 is not edited (per the brief).

Branch `v21.6-vase`, base `488d9e82`. Build: llvm-mingw clang++ 20261006, CMake 3.22.1 + Ninja, Release, `subst Q:` alias of the worktree, build dir `Q:/.local-inputs/v21.6/build` (gitignored). Clean EXE from the base source: `.local-inputs/v21.6/exe/base-v216.exe`, SHA256 `5BAA61331B7D293FCDE362FEE3626AFBA5ED0DB6AF5FD5729F00887AF4CAC1EB`. Full build exit 0.

## Code path (investigated)

- `features/combat/context_button_v1.cpp` `decide_context_button_v1`: a destructible (non-actor type 8) press edge sets `attack_held`. This is the B085 PC adaptation: the press starts the weapon swing.
- `main.cpp` `ctxUseContainer` (about line 7405): for an idle destructible it calls `ctxFaceContainer(ci,false)`, then sets `gameplayInput.attack=true`. It is reached from the in-range press, from the approach arrival (B069), and from the press during the approach.
- `ctxFaceContainer` calls `interaction_facing_v1` (`features/interactions/interaction_facing_v1.hpp`), which sets `yaw=atan2(dy,dx)` toward the object. It writes `owner->transform.rotation[2]` and `motor->setFacingRadians(yaw)`. For destructibles `respectSitting=false`, so the coincident-XY heading policy is the B085 one.
- Hit: `destructibleSwingValid` (B085) is set from `ctxContainerRange`; the hit fires on the `attack_mainhand` marker (line ~8140) and calls `containerRuntime.interact`.
- Render: the player's `actorWorld` is `sourceActorWorld(..., rotation)` (line ~8456), so the mesh is drawn with the yaw from `sourceVisualYaw` or `motor->facingRadians`.

Prior evidence: B085 (`coordination/v20.1/B085-report.md`) and B079's facing repair (`coordination/v20.1/B079-report.md`, "Facing repair") reported the vase facing as repaired in v20.1 (`before=0 after=1.5708`, "matching sword direction"). The B079 repair was judged by sword direction in still frames, not by a body-orientation review.

## Runtime evidence (hidden window, real Space/Continue input, isolated presave copy)

Harness: `.local-inputs/v21.6/verify/tools/run-case.ps1` (copy of the v21.4 harness, v21.5 package, case dirs under the worktree). Menu route `3:1135:226`, `6:1130:259` (Start, Single Player), anchor `S1.sync` from `--capture-at 100`, then Space (`-32`) at +1 s. Test aids: `--player-to X,Y,Z@60` (placement only), `--capture-at`. Seed: `presave` (slot1 now resumes in the witch cave, see gaps).

Vase: `_prim_DestructibleContainer_05_321_364_22_41_21_187_21_47` at (-7142.83,-1597.75,250) is in the swamp; I used `_prim_DestructibleContainer` at (-1229.64,-1224.21,250) because the first attempt put the hero among enemies (an enemy became the OOI).

| Case | Hero placement | Facing evidence | Result | Frames |
|---|---|---|---|---|
| R4-dbg (in place) | 120 east of vase, vase at -X | `Context container facing before=0 after=3.14159 frame=169`; drawn yaw = 3.14159 from frame 169 on | swing hit frame 185 (`accepted`, sound 325 submitted), opened 201 | `cases/R4-dbg/f150.png`, `f170.png`, `f185.png`, `f200.png` (crops in `hero-crops.png`) |
| R6 (approach) | 400 east, walk west | walk yaw -0.21 -> -0.84 (frames 170-173); arrival at frame 190 with `before=-1.60421 after=3.10818` | hit 206, opened 222; hero stood about 350 units from the vase when it swung | `cases/R6-approach/f150.png`, `f200.png`, `f240.png`, `cmp.png` |
| R5 (control, no press) | vase at +X (hero at -X side), yaw 0 | none | yaw 0 idle body faces screen down-right, the same direction the vase lies relative to the hero | `cases/R5-ctrl-plusX/cmp-f150.png` |

What is directly observed:
- The logic turns the hero to face the vase in two entry paths: an in-place press at 120 units (R4) and approach arrival after a walk (R6). The R8 control (no turn needed) was not usable because the hero entered an exit zone.
- The turn reaches the renderer: the drawn yaw (debug print in `sourceActorWorld`, removed before commit) changes from 0 to pi on the frame of the press and stays there through the hit and the open.
- The swing hit and the open happen on the swing marker (frames 185/206) with the expected sound submission.

What is inference only (not established):
- Screen direction: from R5, yaw 0 (+X) draws toward screen down-right, so yaw pi (-X) should draw toward screen up-left, where the vase is in R4/R6. In R4 frames 150 and 170, and R6 frame 240, the body looks the same orientation as before the turn (facing screen down-right) with the vase behind-left. So the visible body may not follow the yaw during the attack/idle pose. Crops are about 300 px from a 1201x720 frame; I could not confirm this from the image alone.
- Which transform the attack pose uses (root yaw in the retained pose/clip versus the world matrix) was not established. I did not find where a root yaw would override the world matrix.

## Unexplained observations

1. Approach walk: the hero walks west (-X) toward the vase but his yaw during the walk trends from -0.21 to -0.84 (not toward pi). Not investigated; it may be the source turn kernel toward the camera-relative input.
2. Range: the approach arrival and the swing both pass at about 350 units from the vase, since the gate uses the B069 interaction range (about 572 per B069) and not the melee reach. The hit lands on the swing marker regardless of how far the sword is from the vase. Already noted in B085 ("swing-start range... not decoded for the melee swing"). This may make the swing look like it is aimed at the air. Not fixed here.
3. Exit zones: two test placements, (-1349.64,-1224.21) and (-1229.64,-1344.21), are inside an exit zone and trigger a level transition. Do not reuse them.

## Changes

None to source. The debug print used for the investigation was removed (`git checkout` of `port/windows-foundation/main.cpp`; the tree matches `488d9e82`). Only this report is added under `coordination/v21.6/`.

## Tests

- Full CTest of the clean base build: not run in this session (no source change; the base build itself compiled with exit 0). Expect 179/179 as in v21.5; not verified here.
- No unit test added, because no fix was made.

## Verified vs not

- Runtime-verified (hidden window, real keys, isolated save): the facing change at the press and approach arrival, the yaw reaching the render matrix, the swing hit and the vase open. Frame sequences listed above.
- Not verified: that the body visibly turns to the vase during the swing; the body orientation in the rendered frames is inference from low-resolution crops.
- Unit-only: none.

## Remaining gaps and next steps

1. Find out whether the attack/idle pose takes its root yaw from something other than the actor world matrix (retained pose, clip root, or the `visual` publish path). A higher-resolution capture of the hero alone (or a debug overlay of the facing vector) would make the body direction unambiguous.
2. Compare with the B079 chest turn, which was reported as visible. If the chest turn is visible while the vase turn is not, the difference is in the attack pose, not in the facing logic.
3. IDA check of `AISDefault::OnAttack` (0x3dc12c) for the non-Character target: does the original turn the hero toward a destructible before the swing, or does it swing in the current facing?
4. Swing gate (item 2 above): the destructible swing and hit use the openable interaction range; a melee-reach check would be the source-faithful gate. Separate bug candidate.
5. Slot1 seed now resumes in the witch cave on this build (B143), so swamp tests must use the presave seed or the direct swamp start.
6. Original reference video of a vase break (B085 found none). Not checked here.
