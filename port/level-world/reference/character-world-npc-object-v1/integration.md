# Source NPC object, PF and room ownership

`character_world_npc_object_v1.hpp/.cpp` borrows the sole retained
`actor::RuntimeState.object` PFObject, shared world geometry/obstacle registry,
actual NPC physical pointer and one `WorldNpcObjectFieldsV1`. Create those fields
once with the original ObjectBase/GameObject constructor values; do not derive
them from actor presentation strings. Initialize the PFObject using the existing
`dh2_nav_object_defaults`. Its null floor is represented by `UINT_MAX`.

Set `WorldNpcPhysicalServicesV1.update_pf` to call the same object owner's
`update_pf()`, returning its error on failure. The whole source UpdatePFObject
returns immediately while its PF floor is null. This genuinely completes the
InitPhysical tail before InitFinal's PFWorld::InitObject; it does not pretend
InitFinal, obstacle insertion or movement completed. With a nonnull floor the
helper invokes ordered real virtual IsPFObstacle/weight/extent services, the
existing actual PFWorld obstacle kernel, then reloads the physical pointer and
uses its game-unit radius or the actual absolute AABB. Required queries fail
only when reached. PF obstacle enable observes the physical pointer captured
before the weight/extent callbacks, as the source does.

For the recovered Character vtable compose services with
`character_pf_services_v1(services)`: whole source methods 3a2ee8, 3a2ef0 and
3a2efc return true, radius 50 and strength 20. This keeps the actual physical,
visual, AABB and lifecycle providers unchanged. `init_pf_object(position,aabb)`
implements only InitFinal's PFWorld::InitObject call, borrowing the same PFObject,
actual position and AABB, full maximum XY extent, and source static84. The existing
backend member named flying receives this static argument (object flag 1); it
does not establish PF flying capability. Spawn/disabled gates and the subsequent
debug-name, visual, light and LookAt-node tail must still be supplied by InitFinal.

The physical same-graph host now loads actual `crypt.bdae` and `crypt01.dwld`.
All eleven actors find source floors and each inserts exactly one obstacle with
the same actor identity. After actual body release, explicit UpdatePFObject
removes every obstacle and preserves the source empty floor-map keys. This is
verification of the callable UpdatePF removal branch, not a claim that the full
GameObject destructor dispatch has been integrated.

Renderer composition order (all owners must outlive their callbacks):
1. Retain one ObjectFields and one actor RuntimeState.object; load CPO1 defaults.
2. Borrow `native_floor->collision_world` and the world-owned ObstacleRegistry.
3. Set physical to the same physical owner's actual NativeBody when assigned;
   absolute_aabb borrows the same body projection bounds.absolute_box.
4. Wrap those services with character_pf_services_v1; construct the Object helper.
5. Physical update_pf calls that helper. Constructor-null PF correctly returns
   before querying the still-unassigned physical pointer.
6. At the reached InitFinal PF call invoke init_pf_object with actual actor
   position and actual bounds; complete the required visual/light/node tail,
   then invoke update_pf in original order. Do not skip that tail by joining the
   two calls into an invented InitFinal success.

`CharacterWorldNpcPhysicalV1` now owns the actual primary shape and original
saved filter halfwords from its real BodyConfig. `enable_filter()` and
`disable_filter()` write native shape filters then invoke real World::Refilter,
with source +26 constructor zero/idempotence. The recovered NPC factory creates
one primary shape and leaves the source secondary null. Never synthesize a
secondary shape from the body's unrelated linked list. Exceptions retain the
reached source prefix and return the error. Before World teardown call release
while same FSM/AI/VM/collision owners still exist.

`WorldNpcObjectServicesV1.method` must compose actual VisualObject SyncVisibility,
native physical enable_filter/disable_filter, and same-actor Character::Stop.
Enabled/Disabled/SetEnable/SetVisible/ZoneEntered/ZoneExited preserve original
ordering. Missing Stop is required on Character::Disabled even with no physical
body; don't accept an empty callback. Source SetEnable(true) does nothing when
constructor +8a already equals1. Source room entry writes +2f0/update85 and
does not write +80. ObjectBase/GameObject constructors do not initialize +80;
`read_visible()` reports unavailable until a real property-default or SetVisible
producer. `CharacterWorldNpcPropertiesV1` loads the additive CPO1 reference using
the actual DACT digest and eleven initialization keys, then initializes these same
fields before room entry. Source DeclareProperties declares visible default 1 and
static default 0; LoadDefaultProperties invokes the real bool default store. The
canonical Crypt XML has no overrides for either property. CPO1 records that exact
absence, not a replacement mutable property store. Do not call Enabled arbitrarily
to create visibility. Do not replace +80 with CPU renderer scene visibility. Queue and
collision services must call read_visible and propagate unavailable.

`character_world_npc_room_v1.hpp/.cpp` supplies genuine RoomZone AddObject,
RemoveObject and AddInitialObject over the original separate room-owned list
nodes. Borrow actual room absolute AABB, actual Character position, shared World
Debug owner/files and actual previous room lookup. Init does source inclusive XY
bounds checks (Z unused), actual isTracingRoomZoneInit Debug prefix, removal from
the previous owner when +2ef says assigned, +2f4/+2ef writes, same ZoneEntered,
then list append. AddObject deduplicates; RemoveObject erases first match and
does not clear source object's room/assigned fields. The list owns no Character.
Do not substitute the World handle registry, PF obstacle lists or AI queue.
Actual RoomZone construction/DACT association and ObjectManager no-room list
dispatch still require their recovered production producers. Full DisableZoning
and EnableZoning are not implemented by this bounded helper.

Validation: current direct sources strict ARM64 compile; O1/O2 ASAN+UBSAN object
and room host checks; same eleven Crypt VM/FSM/property/life/native-body graph
physical host with skeleton/slime/ghost assets and real native filter changes;
whole original ARM lifecycle/PF (145 calls), filter (16 calls) and room ownership
(52 calls). The original audit explicitly supplies Visual/Stop/Refilter/PF and
allocator dependency fixtures; it is not a native differential or proof of full
NPC AI. Existing native PF algorithms retain their separate original differential
receipts. Frozen shared-library dependency hashes are recorded and do not imply
current-source attribution. Renderer/CMake untouched.
