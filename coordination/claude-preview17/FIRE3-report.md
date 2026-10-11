# FIRE3 report (Preview 17): red door wall of fire (`_prim_Swamp2Door_FIRE`)

Branch `p17/fire3` (worktree `DH_wt/p17fire3`), started from `p17/fix` (`a78a882f`). Not pushed.

## Investigation
- (pending) BRES decode of `go_door_fire_red.bdae`: skeleton, skin controllers, flame mesh vertex data/UVs, texture, material colour/blend tracks.
- (pending) Animation clips (`idleclosed`, `open`, `close`, ...): bone scale/translation keys, which bones the flames follow.
- (pending) Emitter: birth-rate track, atlas, velocity/lifetime, gravity, colour over life.
- (pending) Fade-in ramp (about 10 s): clip, emitter track, or alpha/colour track keyed on door state.

## Implementation
- (pending)

## Isolated tests
- (pending)

## Integrated runtime verification (real EXE, quiet_run, rc3 package)
1. Red door same-frame vs reference (`ref-wall-of-fire-0759.png`, Part 1 about 610-624 s): (pending)
2. Ramp sequence (4 frames): (pending)
3. Contact sheet: (pending)
4. Frame cost `DH_PERF=1`: (pending)
5. ctest (`-j 6`, llvm-mingw on PATH; expect only `session_skill_binding` to fail): (pending)

## Gaps
- (pending)

## Placeholders
- (pending)

## Package files required
- rc3 package (`windows-source-clock-v19-preview-15-rc3`).

## Verifier script
- (pending)
