# Bounded SWAMP player-movement slice

This host-side component makes a small position step over the source-derived
SWAMP floor geometry in [`../navigation`](../navigation/README.md). It takes a
source-coordinate point, heading, normalized two-axis stick and elapsed time,
then samples actor-compatible floors at the current and endpoint positions.
The current slice is restricted to SWAMP module zero. Rejected steps preserve
the complete input position and heading.

## Step contract

`dh2_swamp_movement_step` is declared in `movement.hpp`. The caller supplies a
walk speed in source-coordinate units per second. This is an explicit host-slice
input, not a recovered player stat. The bounded test uses at most 0.1 seconds
and 100 units/second. If both stick axes are in `[-1,1]` but their combined
length exceeds one, the vector is normalized. Heading is radians with +Y
forward; movement itself uses the source world's X/Y axes, and +X input sets
heading to `-pi/2`.

The floor query uses the constructor-derived baseline object path mask `2`:
zero-requirement floors and water floors pass; hole floors requiring bit `1`
fail. Native Void and Wall category floors are excluded. Both the current and
candidate points must be strictly less than 100 source units from an eligible
floor, matching `PFWorld::ValidatePosition`'s default vertical validation
threshold. On an accepted nonzero step, it writes the candidate's Z from the
sampled floor height, matching the native successful-position path. A zero
stick or zero elapsed time returns the source point unchanged. Invalid inputs
and unsupported current or endpoint samples preserve the original position and
heading.

## Scope and limits

This is deterministic, bounded endpoint stepping for SWAMP module zero, using
the decoded source floor tags and actor-floor query. It is integrated into the
Android preview as a developer movement test with a following isometric view,
two-axis touch input and source idle/walk animation. The Android adapter supplies
an explicit 30 source-unit/second test speed; this is not recovered player
data. The native baseline path mask comes from the verified playable-character
constructor path. This module remains separate from the original
Character/CSMove controller. It does not perform a swept path test, wall or
actor collision, acceleration, camera-relative input, native thumbstick
processing, AI/path-graph updates, or module transitions. Endpoint-only checks
can cross gaps at larger caller-selected speeds. The speed and time bounds are
host-slice limits, not original game constants.

The recovered `PFWorld::ValidateDirection` entry takes an object radius and
path mask, but its collision adjustment is not clear enough to reproduce a
radius sweep or response safely. The native entry points and the reason this
slice remains endpoint-only are documented in
[`NATIVE-DIRECTION-EVIDENCE.md`](NATIVE-DIRECTION-EVIDENCE.md). The host test
includes a deliberate gap-crossing fixture to make this limitation measurable;
it does not assert that the behavior matches the game.

## Build and test

Run from the repository root with the supplied original cache at
`../cache/files`:

```text
python port/swamp-movement/build.py --report port/swamp-movement/build/build-validation.json
python port/swamp-movement/tests/check_movement.py \
  --library port/swamp-movement/build/libdh2_swamp_movement_host.dll \
  --cache ../cache/files \
  --report port/swamp-movement/build/movement-validation.json
```

On non-Windows hosts, use `libdh2_swamp_movement_host.so`. Set `CXX` to select a
C++17 host compiler. The library includes the existing navigation and source
asset readers in this standalone build. Test inputs are read from the supplied
cache; no assets are copied into this component. The checks cover the original
module-zero entry `(1090.75,-212.202,258)`, both movement axes, diagonal
normalization, deterministic/clamped time, zero-input release, invalid input,
unsupported starting points, baseline mask `2` passing source module-zero water
and rejecting source boss-room holes, the strict 100-unit floor-height
threshold, and a real module-zero boundary where `y=-199` is supported but
`y=-198` has no accepted floor. A small adjacent-floor fixture confirms that a
hole-requiring endpoint returns a rejected step and preserves position and
heading; source holes themselves occur in module 7, outside this experiment's
module-zero movement scope. Generated binaries and reports stay under the
ignored local `build/` directory.
