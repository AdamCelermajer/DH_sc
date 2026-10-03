# Original obstacle force, avoidance and concrete physical filtering

`navigation_avoidance.hpp/.cpp` reconstructs PFWorld::_CalcObstaclesForce
(0x527660, 1,636 bytes), PFWorld::AvoidObstacles (0x527cc4, 1,004 bytes),
PhysicalObject::canCollide (0x46e768, 176 bytes) and its concrete
onCollisionTest implementation (0x46e6bc, 148 bytes). The captured reference
also includes Point3D normalization and angle entry points.

Force looks up the object's cached floor using map::operator[], retaining an
empty key even for the null floor. It traverses only that floor's ordered
members, skips self, unregistered or disabled neighbors and applies the concrete
physical filter only when both user links and physical bodies exist. Missing
either body bypasses that filter. Distance checks use XY only: a neighbor must
be strictly closer than both the desired target and, when present, the first
path segment's destination. Influence is strict squared distance below
`((self.radius + neighbor.radius) + neighbor.obstacle_weight)^2`.

The coefficient is `1 - distanceSquared / influenceSquared`. The XY separation
with Z set to positive zero is normalized, scaled by coefficient times the
neighbor's obstacle extent, optionally appended to the caller's existing force
records and accumulated in list order. Duplicate registrations contribute again;
the result count is new contributions, not total diagnostic buffer length.
Coincident XY positions preserve the original unguarded normalization and NaNs.
Individually rounded operations and their operand order are retained; NaN class
is compared without claiming portable NaN payload/sign bits.

Avoidance returns before map lookup with null floor, object flag 1 or missing
object flag 2. Otherwise it accumulates force. No contributors or a nonnegative
(or unordered) dot product leaves the requested direction unchanged. A backward
force turns along normalized cross(direction, Vec3f_K), chooses its side from
the signed tangent/force dot product and adds force magnitude times that tangent.
The original strict epsilon uses float word 0x38d1b717. With multiple contributors
and angle at least float word 0x3db2b8c2 (approximately five degrees), the change
is limited using original direction magnitude times float word 0x3db32d41.
This service neither advances an actor nor validates its resulting floor position.

Concrete canCollide rejects a disabled/missing body, missing shape or a present
GameObject owner whose enabled field is false. Primary shape takes precedence
over secondary. Equal nonzero signed 16-bit groups bypass masks: positive groups
allow, negative groups deny. Otherwise both category/mask intersections must be
nonzero. Native absent-source handling is a caller adapter; the original direct
method is invoked with a valid source body. Shape/owner/body construction and
custom collision-test overrides are outside this recovered view.

Three ARM64 binaries (standalone oracle, packaged repository and Studio library)
each match 2,008 original-instruction comparisons: 832 force, 750 avoidance and
426 collision filter requests, with zero mismatches. Comparisons include 4,524
force-buffer records, 4,535 contributions, 220 adjustments, 181 turn limits,
275 early gates and 3,377 registry keys. Cases cover strict boundaries, duplicate
members crossing original deque blocks, vector growth, prefix appends, optional
diagnostic buffers, signed group limits, physical/owner/shape gates, coincident
positions and 384 positive randomized force/avoidance pairs.

Original force/avoidance, map/deque/vector, canCollide and its concrete virtual
implementation execute. Allocator instructions run with supplied heap storage;
IEEE/libm imports are modeled. Caller fixtures supply actor fields, physical
fields, scene membership and immutable first path destination. These comparisons
do not recover those producers or claim a complete physics engine.

ASan/UBSan loads actual Crypt BRES/DWLD, verifies geometry/graph against gold and
replays all 2,008 cases. Eight atomic malformed/storage rejection checks and one
empty-registry early-gate check pass. Full object, motion, FindPath, waypoint,
world-route and graph-search corpora also pass against the current host library.
Prior complete ARM64 comparisons are inherited evidence for those unchanged
modules, rather than fresh full-corpus results for the new Android binaries.

Android's startup probe exercises all eight Crypt floors: 16 contributions,
eight adjusted directions and eight turn limits, FNV state `09d1d79d2612c945`.
The same installed APK passes ten movement/display/lifecycle cases and retains
all five earlier 64-floor-pair probes. Both APKs build, carry only ARM64/x86_64
native binaries with 16 KiB load/ZIP alignment and bundle the existing 126 assets.
Neither APK includes the original engine. Actor movement still uses the existing
supported-floor adapter; avoidance currently runs only in the startup probe.

Next are GameObject capability/radius/obstacle and physical-field producers,
cache invalidation and the character controller, speed/root motion and pursuit.
Full scene progression, rendering fidelity, UI/audio/saves, the complete asset
bundle and physical modern ARM64 testing remain unfinished. The goal stays active.
