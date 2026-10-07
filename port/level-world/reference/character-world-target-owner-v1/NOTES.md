# Borrowed World targeting/controller candidate V1

The candidate connects an actual retained World intrusive Registry8 and fresh
actor lookup to V6 target predicates, actual design AI faction/radius kernels,
the same life/property/Scene borrows, and genuine controller/Character/GameObject
LookAt bodies. It owns only an immutable AI type projection and diagnostics.
It owns no inventory, Save, Session, timer, VM, target list or encounter relations.

GetTargetPosition at 0x3935dc selects cached position +184 only when node +180
and enable byte +80 are both nonzero, otherwise position +160. Original/O2
comparison passes 1,024 cases covering every enable byte and null/non-null nodes.
Cmd_LookAt retains source global/lock/forced gates before any target lookup.
GameObject LookAt performs native heading arithmetic; the adapter publishes that
result to the borrowed actor and runtime controller heading fields.

Production integration remains incomplete. model_renderer ObjectActor owns real
properties/life/Aggro/identities and authored room placements, but no genuine
source World Registry8, flags520, disabled81, visibility8a, interactive415,
zoned/in_zone, target-node/cache enable owner is currently exposed. Its current
prince_look_service explicitly substitutes actor world position for the unfinished
NPC target-node cache. This candidate requires exact source borrows rather than
silently adopting that substitute. Registry/search projections and their selected
center fields must be refreshed before an authored callback; intrusive pointers,
actor/Scene, AI tables and command state must remain live through synchronous
callbacks. No vector relocation, actor removal or GL replacement may occur during
Search. These are borrowing/lifetime requirements, not a reconstructed World
registration/lifecycle implementation.

Whole original AI_IsEnemy (0x3d574c) and AI_IsFriend (0x3d511c) remain required
providers. Their captured bodies first refresh target GetHandle/GetObject and
check object kind +f4; invalid faction IDs reach assertion machinery before table
lookup. Enemy additionally has noncharacter virtual-interaction and player-pair
branches. Cached AiTables faction arithmetic alone does not implement these
ordered wrappers. The host test explicitly supplies that native arithmetic as a
wrapper-boundary fixture and separately verifies the missing production provider
is rejected. Dead-friendly interaction also requires original AI_IsFriend;
there is no !enemy substitution. Missing move/stop continuations fail. Visibility
and zone filters remain the actual source Search fields, never derived from
rendering visibility or authored room names.

Frozen FX hookup is exact: V4/CharAI authored `fx_` animation events call
CharacterMeshFxOwnerV1::animation_event(event, actual owner world position).
Executed source global/target PlayFX calls require original set/name resolution
and the same retained owner's play_set with real position, rotation and nullable
anchor. The frozen owner admits mesh type0 one-step nonredirected sets; particle,
light, other families and required constructors remain explicit. Scene phase,
manager phase and draw_parts retain the frozen owner's ordering and same Scene
lifetime. No no-op FX is supplied by this targeting candidate.

The private host proof uses actual cached Character/AI rows and borrowed native
life/property storage. Scene containers, world topology and source flags are
explicit test input, not evidence of a live renderer connection. Sanitizer and
optimized receipts separately identify that boundary.
