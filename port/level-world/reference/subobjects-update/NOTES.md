# GameObject subobjects coordinator

`dh2_subobjects_update` reconstructs `GameObject::UpdateSubObjects` at
`0x3943cc` from the hash-bound original ELF. The API uses explicit logical
64-bit state and service pointers. It calls the recovered native physical
query, wake, velocity and position-request code. Visual/root motion, transform
application, camera and auxiliary execution are synchronous service boundaries.

The captured routine first updates visual and physical subobjects and samples
the position policies and arrival condition. Arrival uses strict XY squared
distance `<6400`, using the PF path target when a path exists. Z is ignored in
that arrival test, but included in the physical velocity normalization. The
velocity gate is strict positive squared 3D distance; GetSpeed runs before the
`>400` test. Zero or unordered length leaves existing velocity alone. Positive
length at most 400 requests zero velocity and the current position. Speed is a
supplied virtual fact, with no invented constant or frame advancement.

An awake body replaces game XY only when either absolute delta is strictly
greater than 1. Exactly-one floor policy runs early validation; a rejected
result immediately requests the corrected body transform. Accepted physics
position suppresses visual ApplyPosition. Otherwise that visual policy applies
root position, wakes the body, and then still calls SyncPosition. A physical
object always receives the subsequent game-position transform request. Physics
rotation writes the original `GameObject+0x174` field; the distinct heading
angle at `+0x178` is not changed. The base visual ApplyRotation is empty.

A false floor policy runs trailing validation after rotation/scaling sync.
An active camera rejection restores cached `PFObject+24` XYZ and skips that
floor call. Rejected position then requests a body transform, and visual sync
occurs again. Arrival sampled before movement resets destination to final
position. Absolute AABB is the six local coordinates plus final game XYZ.
Auxiliary update follows AABB. Auxiliary type 2/mode 3 makes the camera free;
modes 1/4 release it only when enabled and squared 3D distance is `<=3`.

`State::previous_position` is the PF cached position, so a real floor service
must copy the updated `NavigationObject::motion.position` back into that field.
`path_count` represents path presence and the supplied `path_target` is the
original terminal target. Auxiliary mode/type and virtual policies are fixed
caller facts for an invocation. Callback payload contracts are in the header;
update/root/transform services can mutate game/body state synchronously. The
speed observer receives a separate copy and cannot mutate const policy storage.

The default differential corpus is stable: 1,479 original-versus-ARM64 cases,
9,261 exact ordered service calls and 32 output-state carry ticks. All 374
executable original coordinator instructions are observed. Eleven malformed
caller checks reject before mutation/callbacks without changing that corpus.
It covers
all position/rotation gates, absent subobjects, awake/sleeping bodies, strict
1/20/80 distance boundaries, root fixtures, both floor-validation phases,
camera rejection/presence/auxiliary modes, and IEEE NaN/infinity/overflow.
Finite arithmetic bits and payload ordering are exact. Arithmetic NaN sign
and payload are compared by NaN classification. Original soft-float/libm
imports are modeled by the established instruction CPU harness.

`service-fixtures.bin` contains input/output logical fields and ordered
payloads for standalone host replay. ASan/UBSan replay passes all 1,479 records.
The original coordinator, actual Character policy flag getters,
IsAtDestination, normalization, AABB and physical getter/setter instructions
execute. Visual wrapper entries are observed and replaced with fixture
services; their backend internals are not claimed as reconstructed here.

An explicit optional differential mode attaches actual original and compiled
native `dh2_nav_validate_object_position` and obstacle-parent registry chains
through the service callback. It uses independent instruction CPUs with the
asset-verified Crypt geometry. All 64 ordered floor pairs in early and trailing
mode pass: 128 coordinator cases, 128 validations, 240 floor queries and 112
registry relocations. The separate Crypt report records all hashes and the
output checksum; the stable 1,479-record host fixture corpus is unchanged.

This establishes the coordinator and its native floor-service bridge. The
visual engine/root-motion backend, actor integration, camera/auxiliary
backends and Box2D world integration remain separate work. Original ELF code
is a desktop verification oracle only; it is not needed by the native API.
