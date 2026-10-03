# Original floor position and direction validation

`navigation_motion.cpp` reconstructs PFWorld::ValidatePosition (`0x525d84`),
ValidateDirection (`0x525944`), floor/room/world GetFloorHeightAt, Point3D's
epsilon equality, normalization/angle arithmetic and glitch::line2d intersection
(`0x525310`). The capture identifies 56 original routines including the prior
world collision chain. The original engine is used only for local comparisons;
the compiled Android functions contain reconstructed C++, without ARM32 code.

## Position and height rules

Height queries use inclusive XYZ world/room bounds and visit rooms/floors in
caller order. Ordinary room traversal skips type mask `0x03000000`; inclusive
mode does not. An explicit cached-floor query bypasses that room filter. Floor
height calls the existing selector/octree/collision chain and returns the first
successful floor. This is not the supported-floor adapter's nearest-height
policy. Its normal is the unnormalized reverse cross product of the hit
triangle's B-A and C-A vectors, preserving original single-float operation order.
Misses preserve the caller's supplied height and normal values.

ValidatePosition first compares the object's current XYZ with the requested
position using `abs(current-requested) < float(0x38d1b717)` on all three axes.
Equality returns true immediately without updating the requested point, cached
floor/room, normal or obstacle-parent service. Otherwise it queries the cached
floor, then the cached room, then the world. A hit must satisfy the floor's
capability bits. Unless the policy bypass is enabled, it also requires the
strict inequality `maximum_height_delta > abs(hitHeight-requested.z)`.

Accepted movement requests the original obstacle-parent service before storing
the new room/floor, writes the hit height into the requested position and copies
the position/normal to the object. A failed query restores the prior position
and returns false. A capability or height rejection also restores that position
but returns true. With no object, the function simply performs the world height
query and updates Z on success. These different return values are preserved.

## Floor-boundary direction rules

ValidateDirection starts with a world collision at the current position and
requires a traversable source floor. It normalizes the supplied 3D direction by
dividing each component by its length, then probes ten units ahead. A traversable
hit keeps the original direction, including its magnitude. The radius argument
is present in the original overload but is not used by this body.

If the forward probe fails or its floor is not traversable, the original routine
tests the source triangle's AB, AC and BC edges in order against the XY movement
segment. This uses a separate finite-segment intersection routine with strict
parallel epsilon `0x358637bd` and inclusive parameter endpoints. A hit selects
the edge direction; an angle at least pi/2, or an unordered angle, reverses it.
The slide direction is normalized and probed ten units ahead. A missing slide
floor rejects the direction, leaving the supplied vector unchanged. Successful
slides preserve its original 3D magnitude and set Z from the planar edge vector.
There is no capability test on the successful slide floor. If no triangle edge
intersects, the original function returns true with the input vector unchanged.
Zero vectors retain the original division/NaN/sign behavior; normalization has
no zero guard here. This is floor-boundary sliding, not movable-obstacle collision.

## Native adapters and evidence

MotionObject owns position/normal and stable cached room/floor IDs; UINT_MAX
represents null pointers. MotionPolicy supplies the original world height limit
and bypass flag as caller facts. Finite caller positions/directions, nonnegative
radius/limit, valid IDs and well-formed scene/selector storage are modern caller
contracts. Malformed requests are rejected before output/state mutations.

The original `_ChangeObstacleParentList` call is an observer in this comparison.
PositionResult reports its required service and whether object flag 4 plus a
floor change requires backend relocation. The original deque/map obstacle-parent
backend is not implemented by these functions. Full PFObject InitObject,
InitObstacle, capabilities and actor radius producers remain to be recovered.

All three ARM64 binaries match 2,353 original comparisons: 528 height queries,
573 position requests, 768 direction requests and 484 segment tests. All 2,860
floor-query IDs/coordinates and ordering are compared through independent
original floor/selector/octree/collision execution. The corpus includes cached
priority, absent actors/cache pointers, special-inclusive traversal, strict
height/equality boundaries, denied source capabilities, zero directions,
corner probes, 171 slide attempts (149 successful, 22 rejected), 264 parent
service requests and 64 backend-change notifications. IEEE arithmetic and libm
imports are comparison fixtures; NaN payload/sign is not a portable libm claim.

ASan/UBSan loads the actual Crypt BRES/DWLD, checks its graph and geometry against
gold, and replays all results and object/normal updates. It also rejects six
malformed requests atomically. Its query count refers to gold records; query
order is observed by the instruction differential. Prior path/search code is
unchanged; current host regressions replay its complete corpora and the emulator
retains all prior graph/world/FindPath probes.

The live Android motion probe tests all 64 ordered floor pairs, with cached
source room/floor IDs, capability 0, object flags 8, radius 36 and a development
height limit of 1,000. All 64 position requests are accepted and all 64 direction
requests succeed, matching checksum `d60ad48039cc85c6`. It does not move actors.
Characters still use the supported-floor movement adapter. Original dynamic
obstacle lists/forces, InitObject producers, cache lifecycle, character controller
speed/root motion and moving enemy pursuit remain pending, as do full game flow,
original GPU parity, full assets and physical ARM64 testing.
