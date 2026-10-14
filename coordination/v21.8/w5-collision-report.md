# w5-collision: Swamp collision stall (owner report)

Branch `v21.8-w5-collision` (worktree `.claude/worktrees/w5-collision`, commit fb201147, not pushed, not merged). Build dir `C:/dh219/w5-build`, subst `W:`.

## Verdict

- **Owner symptom (shipped Swamp, `swamp.args`): DID NOT WORK as a fix.** The hypothesis that `ActorMovement::advance` causes it is wrong for the shipped build: the shipped Swamp uses `--original-floor-motion`, so player displacement goes through the native PF kernels, not `advance()`. The shipped run stalls exactly as before the change (identical trace, section 4). I did not find a shipped-path defect that matches the owner's wall/corner description.
- **`advance()` wall sliding: WORKED in isolation.** Synthetic 45 degree wall: 70 units covered before, 427 after (tangent slide expects 424). Full CTest 222/222 passed.
- The one stall I reproduced in the shipped game is at a floor edge with no floor under the target (water beyond the stone platform), which the original's floor rule also refuses per IDA. Whether the original also treats that water as non-walkable is NOT verified.

## 1. Original and port evidence

PROVEN (IDA export `pseudocode-all.c`, port code read):
- `GameObject::Update` (line 104523) calls `UpdatePath`, then `UpdateRotation`, then `UpdateSubObjects`, then `UpdateTargetPosition`.
- `GameObject::UpdatePath` (line 109761) sets the heading. On the path-following branch with flag 452 set and `DisableWallAvoidance` off, it calls `PFWorld::ValidateDirection` (line 109852).
- `PFWorld::ValidateDirection` (line 378744): probes 10 units ahead; if floor exists there it keeps the direction. Otherwise it intersects the ray with the collision triangle's edges and slides along the edge (flipped to within 90 degrees of the desired direction), accepting only if floor exists 10 units along the slide. Port: `navigation_motion.cpp` `dh2_nav_validate_direction`, same structure.
- `PFWorld::ValidatePosition` (line 378892): accepts the requested point only when a floor is under it within `maximum_height_delta` and `CanPathOn`; otherwise the object keeps its old position. No substeps and no axis slides. Port: `dh2_nav_validate_position` (kind 3 = rejected).
- `PFWorld` constructor (line 376530): `+144` = `0x42C80000` = 100.0f, `+148` = 0. Port `floors.hpp` `source_motion_policy_v95{100.f,0}` matches.

PROVEN (port code):
- `actor_movement.hpp` `advance()` slid only along world X then Y when a sub-step was blocked.
- Shipped `swamp.args`/`startup.args` pass `--original-floor-motion --original-native-bodies --original-heading-rotation`. In `main.cpp` `applyActorRootMotion` the `!options.sourceFloorMotion` branch is the only one that calls `advance()`, so the shipped player never reaches `advance()`.
- The development controller policy (`development-controller.xml`, 50/200/60) is labelled "original PF step limits not yet verified". It only affects the `advance()` path.

INFERENCE (not verified):
- The port performs root displacement before `UpdateRotation` (it uses the previous facing), while IDA shows rotation before `UpdateSubObjects`. The header comments describe this order as deliberate; I did not change it and did not verify which phase carries the root motion in the original.
- The owner's wall/corner stall is probably in the native module colliders (the 24,738-triangle source world), which I could not reproduce.

## 2. Engine change (`actor_movement.cpp`)

- `blocked()` can now collect the horizontal normals of all wall hits, with duplicates removed (up to 8).
- `advance()`: when a full sub-step is blocked, the step is projected onto each wall contact plane and onto pairs (corners). The admitted candidate with the most progress along the step is taken. Axis slides remain as the fallback for blocks without a wall contact.
- Floor admission, step limits, and the `supported()` checks are unchanged.

`main.cpp`: `DH_STATE_TRACE=1`-only line `Player admission trace` (frame, current, delta, facing, heading, kind). Diagnostic only.

## 3. Tests

- `tests/actor_collision_stall_tests.cpp` (ctest `actor_collision_stall`, arg = staged3 assets):
  - Swamp census (approximate `CollisionScene`, real level, Knight body 65.44/344.56, policy 50/200/60): 16 headings, 40 frames at 14 units per frame; stall = covered < 30% while a free heading covers >= 90%; wall stall = floor still exists 30 units ahead.
  - Synthetic 45 degree wall (tangent slide, check >= 80% of expected).
- Census before and after, identical:

| Area | starts / grounded | stalls | wall stalls |
|---|---|---|---|
| Start area (1000..1180, -320..-100) | 56 / 35 | 35 | 0 before, 0 after |
| Platform edge (260..340, 40..120) | 81 / 20 | 100 | 0 before, 0 after |

  - Reason: the Swamp approximate collision has 621 triangles, 16 floors and **0 collider instances** (run log `Collision triangles=621 floors=16 colliders=0`). `advance()` has no walls to slide on in that data, so every census stall is a floor edge. The census therefore cannot show a wall benefit on the Swamp.
- Synthetic wall: before `moved (-70.0, 0.0)`, after `(-335.0, -265.0)`, length 427.1 (expects about 424.3).
- Full CTest: **222 of 222 passed** (`C:/dh219/ctest-all.log`).

## 4. Real runs (shipped `swamp.args` via `coordination/v21.8/w5-collision-run.ps1`, hidden desktop, real keys)

Keys: Enter x5 to skip the intro (cinematic ends), then W 2.5 s from 8.0 s, D, S, A (same schedule each run). Position trace `Timed buff probe` every 4 frames. Frames viewed: `C:\dh219\cases\base-c\f620.png` (Knight on the stone platform edge, water beyond, lizard next to him).

Stall sequence (base-c and fix-c, both runs):
- Frames 528 to 576: W held, Knight moves diagonally `(791.8,-416.2)` to `(318.9,41.8)`, `kind=2` (accepted), heading -135 and facing -135.
- Frame 588: `kind=3` at `(309.8,79.2)`; the target `(~300,89)` has no floor (`collision_probe` returns floor=0 there). Position frozen for the rest of the W hold (about 110 frames, 113 `kind=3` lines per run). Heading not slid: no wall contact at this spot.
- Later D phase moves him to x 555, so a free direction exists from the stall point.
- `fix-c` vs `base-c`: same positions to the frame within wall-clock jitter. Same outcome, as expected for the shipped path.
- Source PF counters in base-c run: `Source PF motion final actor=... solves=613 accepted=303 clamped=300`; `Source manual heading final checks=601 slides=265`.

Not run: a real run without `--original-floor-motion` (the `advance()` path). The Swamp approximate collision has no walls, so such a run could only show floor-edge stalls, which the change does not affect.

## 5. What I could not reproduce or decide

1. The owner's exact wall/corner position. The only reproduced stall is at a water edge with no floor under the target.
2. Whether the original game lets the Knight enter that water. The water is not floor in the approximate collision or in the native floor graph (`worldHit=0`). Needs IDA on the water floor type, or original footage.
3. Whether the root-displacement-before-rotation ordering differs from the original (INFERENCE above).
4. Native-path wall corners (module colliders): not exercised.

## 6. Packaging deviation

The brief asked for `.local-inputs/windows-source-clock-v21.8-staged3`. That folder already exists and contains another run's `dh-foundation.exe` and `package-receipt.json`, so I did not overwrite it. The w5 package is `.local-inputs/windows-source-clock-v21.8-w5-collision`: my build's `dh-foundation.exe`, the startup/swamp args, Play scripts, and junctions for `assets`, `audio-assets`, `ui-assets` into staged3 (no asset copy).

## Files
- `port/windows-foundation/actor_movement.cpp`
- `port/windows-foundation/main.cpp` (diagnostic trace only)
- `port/windows-foundation/CMakeLists.txt`
- `port/windows-foundation/tests/actor_collision_stall_tests.cpp`
- `coordination/v21.8/w5-collision-run.ps1`
- Run evidence: `C:\dh219\cases\base-c\` (pre-change ran with diagnostic), `C:\dh219\cases\fix-c\`
