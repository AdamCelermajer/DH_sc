# Concrete obstacle producers and GameObject::UpdatePFObject

Recovered `navigation_producers.hpp/.cpp` implements the five captured concrete
IsObstacle/GetObstacleRadius/GetObstacleStrength groups, PhysicalObject::getRadius
conversion and GameObject::UpdatePFObject (0x393ea0, 240 bytes). Actual vtable
slots +0xb4/+0xb8/+0xbc in the original GameObject, Character, Container,
LiftableObject and TriggerTrap vtables execute in the instruction audit.

GameObject reports non-obstacle, radius/strength zero. Character reports obstacle,
radius 50, strength 20. Container, LiftableObject and TriggerTrap report obstacle,
radius 150, strength 10. These are specific concrete implementations, not generic
values for every descendant or unexamined custom override.

UpdatePFObject gates on the embedded PFObject's user pointer. Null user leaves
radius, flags and existing registry membership untouched. For a concrete obstacle
it calls InitObstacle with enable equal to physical-body presence and the concrete
radius/strength. It then writes physical-body radius times 100, or, without a body,
half the larger XY bound extent. Both extent differences and multiplication are
individually rounded. Bounds are existing GameObject fields, supplied by the caller
here; there is no new model-bounds calculation or minimum-one clamp in this update.

InitObstacle runs before radius refresh, and this order is compared at the original
call and native API entry. The force module's older field labels `obstacle_weight`
and `obstacle_extent` now have traced producer meanings: obstacle radius and
obstacle strength respectively. Shape body radius is separate and uses physical
units before conversion. A base GameObject skips InitObstacle entirely even with
preexisting registered caller state. Removing a body from a Character disables its
obstacle and then computes its radius from existing bounds.

Native bounded storage preserves the original observable ordered registry and
object state. Malformed/capacity failures preserve state, a native caller contract.
Inputs require finite, nonnegative resulting radii. Null user gates even unavailable
field/registry/world pointers. Caller storage must be distinct. The implementation
does not construct physical bodies, set physical filters, generate bounds or
initialize capability flags and position, nor does it move a character.

Each of the standalone oracle, packaged ARM64 and Studio ARM64 libraries matches
1,238 original comparisons: five concrete trait groups and 1,233 complete updates.
The corpus compares all eight object views and complete registry keys/membership,
including 4,314 member records, 1,893 map keys, 24 floor queries and 381 obstacle
calls at their old radius. It covers all concrete classes, body presence/removal,
null-user preservation, zero/signed-zero radii, bound-axis selection and ties,
uncached hit/miss, original deque growth/duplicate removal and 1,024 seeded updates.
Full concrete methods, UpdatePFObject, getRadius, InitObstacle and map/deque
instructions execute with caller-owned actor/body/bounds/storage fixtures;
independent original selector oracles execute floor queries and IEEE imports are
modeled. This is not a complete recovered GameObject/physics controller.

The actual-asset ASan/UBSan replay matches the complete corpus, ten atomic rejection
checks and a null-user early gate. Full previous avoidance, object, floor motion,
FindPath, waypoint, world-route and graph-search corpora also pass against the
current host library. Prior full ARM64 comparisons remain inherited evidence for
their unchanged modules; they are not relabeled as current Android full-corpus runs.

The Android startup probe registers a Character on each of the eight Crypt floors,
refreshes all eight physical radii and matches state FNV `30c1f3ac1bf81145`. Both
APKs build and verify 16 KiB load/ZIP alignment; ten emulator rendering/movement/
lifecycle cases and previous startup probes pass. The original engine is absent
and the existing 126 assets are bundled. These producer services currently run
only in the startup probe, while actor movement still uses the supported-floor
adapter. Physical-body/filter/bounds and capability/initialization producers,
cache lifecycle, speed/root motion/controller/pursuit, full game flow, rendering
fidelity, UI/audio/saves, full asset bundle and physical ARM64 tests remain open.
