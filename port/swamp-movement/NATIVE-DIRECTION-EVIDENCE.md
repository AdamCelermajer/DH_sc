# Native movement and collision evidence

This audit compares the source-backed SWAMP floor checks with the original
ARM32 movement-validation entry points. It records what can be reproduced
without guessing a collision radius or response policy.

## Recovered behavior

- `PFObject::CanPathOn(PFFloor*)` at ELF `0x524230` reads the floor mask at
  `PFFloor+0x24` and object path mask at `PFObject+0x14`. A zero floor mask
  passes; otherwise every required floor bit must be present in the object
  mask.
- `PFWorld::ValidatePosition(Point3D&, PFObject*)` at `0x525d84` tries the
  object's current floor, then room/world floor-height queries. It calls
  `CanPathOn`; successful object validation writes the sampled floor height
  back into candidate Z and updates the object's tracked floor/room. With a
  null object it samples a floor and writes its height into candidate Z. This
  is a position/floor eligibility check, not a segment sweep. Its native
  height rollback threshold is a strict distance-under-100 comparison against
  `world+0x90` (initialized to 100); equality rejects, while a nonzero
  `world+0x94` bypasses the threshold. The host movement slice was corrected
  to match the successful-path Z write.
- `PFWorld::ValidateDirection(Point3D&, Point3D const&, float, uint)` at
  `0x525944` accepts a floating-point radius and path mask. The object overload
  at `0x525d60` passes `PFObject+0x08` as the radius, `PFObject+0x14` as the
  path mask, and uses the object's position at `+0x18`. The base `PFObject`
  constructor initializes `+0x08` to `1.0f` and `+0x14` to `2`; derived setup
  may still change them.
- The direction routine normalizes the requested horizontal direction and
  probes a point ten world units ahead through the world floor/path queries.
  It then performs 2D line-intersection tests and may modify the candidate
  direction. The supplied radius argument is not used by the recovered
  1052-byte routine, so this is not a radius sweep. The available decompilation
  is flagged incomplete; the exact line orientation/response, collision normal,
  slide rule and rollback policy remain unresolved.
- `Character::GetObstacleRadius()` at `0x3a2ef0` returns `50.0f`. It is a
  separate obstacle-avoidance interface and is not evidence that the
  `ValidateDirection` radius argument is 50 units.

Assembly evidence is in `recovered/native/assembly/libDungeonHunter2.so/`
(`PFObject-d3788b6ddde1-001.asm`, `PFWorld-c21c45a6e09d-001.asm`, and
`Character-1405a63e8a78-001.asm`); corresponding decompiler entries are in
`recovered/native/decompiled/libDungeonHunter2.so/function-index.jsonl`.

## Decision for this host slice

The current `dh2_nav_query_segment` is a bounded zero-radius point/triangle
query. It is not the native character controller, and a horizontal movement
segment can be parallel to a flat floor, so it cannot by itself detect a gap
between two walkable floor patches. The current SWAMP movement routine checks
the start and endpoint only. This audit does not add a radius or invent slide,
wall, or obstacle behavior without a differential oracle for
`ValidateDirection`.

`tests/check_movement.py` now includes a synthetic pair of separated floor
patches. It verifies that an eight-unit step from `(1,1,0)` to `(9,1,0)` is
currently accepted even though it crosses the gap from `x=2` to `x=8`. This is
a known-gap regression documenting the endpoint-only limitation, not a claim
of desired native behavior. A future sweep implementation needs either a
validated `ValidateDirection` differential or a clearly separate host policy
with its own requirements; it must not be described as original collision
parity based on this evidence alone.
