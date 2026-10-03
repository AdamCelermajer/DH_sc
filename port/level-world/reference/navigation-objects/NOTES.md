# PFObject initialization and obstacle-parent registry

Recovered C++ in `navigation_objects.cpp` implements the motion/obstacle view of
PFObject defaults, InitObject, InitObstacle, flying/swimming capability setters
and queries, PFWorld height-policy defaults and `_ChangeObstacleParentList`.
`dh2_nav_validate_object_position` attaches that parent backend to the previously
recovered position validation. It does not advance a character controller.

The original Vec3f initializer executes in the instruction comparison; constructor
normal is Vec3f_K `(0,0,1)`. Object defaults are capability mask 2, object flags 8,
radius 1, weight 1, extent 0, null user/room/floor and zero position. Path, embedded
edge and debug-string containers execute in the original constructor but are not
represented or compared by this 64-byte motion/obstacle projection. The native
PathObject module owns path storage separately. Original PFWorld height limit is
100, with bypass false; the startup probe deliberately supplies limit 1,000.

InitObject sets object flag 1 from its boolean, replaces user/position, clamps
radius to at least 1 and makes an ordinary world height query. Flag 1 preserves
the requested Z; normal and room/floor still update on a hit. A miss preserves
normal and cached room/floor, including stale cached IDs. InitObject does not
unregister an existing obstacle. This boolean differs from SetFlying, which sets
capability mask bit 1; SetSwimming sets capability mask bit 2.

InitObstacle requires nonnegative weight. Disabled bool or zero extent (including
negative zero) disables; negative nonzero extent remains enabled. Enable with
flag 4 already set only updates weight/extent. Otherwise it queries height if
there is no cached floor, then appends to the floor list and sets flag 4. A failed
uncached query returns without changing weight/extent or enabling registration.
Disable removes only the first matching member, clears flag 4 and writes positive
zero weight/extent. Map keys accessed during lookup persist when lists are empty.

Parent relocation runs only with flag 4 and a different target floor. It touches
the old map key, removes the first matching member if present and appends to the
target list. An absent old member prevents target lookup/append. Null floor is a
real map key. The service does not change the object's cached floor; the position
caller updates it after the service. Duplicate members and within-floor insertion
order are preserved. Native storage uses bounded arrays and stable 64-bit keys,
not original pointers or SGI tree/deque storage. The audit compares observable
map keys and complete ordered floor memberships, not storage-specific RB colors.

Each of three ARM64 binaries matches 1,093 original requests: 207 InitObject,
462 InitObstacle, 107 parent calls, 146 attached position requests, 81 flying and
88 swimming updates, one object-default and one world-policy case. All eight
object views, 9,598 registry member records, 2,188 map-key records and 477 floor
query IDs/coordinates/order are compared. The original map/deque instructions,
including block growth and both erase directions, execute with allocator storage
fixtures. Independent original selector/octree/collision oracles execute queries;
IEEE float imports are modeled. Source asset ownership and caller-supplied fields
remain fixtures, rather than recovered GameObject producers.

ASan/UBSan loads actual Crypt BRES/DWLD, checks graph/geometry against gold and
replays the full corpus. It also checks nine atomic malformed/storage rejections,
including a successful position whose required relocation exceeds bucket storage.
Full existing motion, FindPath, waypoint, world-route and graph-search corpora are
replayed against the current host library. Their original ARM64 instruction
comparisons are inherited evidence for unchanged source.

The live startup probe initializes and registers an object, then validates a
position for every ordered floor pair: 64 accepted, 64 registered, 56 relocations,
state FNV `22a98dfc4102db51`. It matches original state and memberships. It is not
used by actor movement yet. Force accumulation/AvoidObstacles, actor capability,
radius/obstacle producers, cache invalidation, speed/root motion, pursuing enemies,
full scene progression, UI/audio/saves, full assets and physical ARM64/GPU testing
remain unfinished.

The world smoke harness waits for the loaded status UI and a native draw whose
dimensions match the current viewport before taking screenshots. A draw log
alone could precede status-panel relayout: the captured failure showed Android's
BLAST buffer queue rejecting a 1080x2061 frame after resizing to 1080x1975, while
the subsequent screenshot rendered correctly. Geometry/image assertions remain
strict; the harness does not retry failing pixels, shader errors or app crashes.
