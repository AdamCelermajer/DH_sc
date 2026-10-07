# Canonical NPC physical composition

New portable production modules are `character_world_physical_peers_v1`,
`character_world_peer_properties_v1`, `character_world_physical_character_v1`
and `character_world_npc_physical_lifecycle_v2`. The renderer include is
`renderer_npc_physical_v2.inc`; it changes no renderer translation unit.

One physical peer index belongs to one canonical `CharacterWorldRuntimeV1`.
It indexes actual `WorldObject.context` to borrowed identity, `NativeBody`,
same ObjectBase lifecycle fields and optional source type+f4. Add rejects
unregistered canonical owners. Unknown contexts fail. Owner-null is accepted
only as an explicitly registered original PhysicalObject+8 null owner, never
as an unknown-player/decor fallback. Enabled queries read `visible_written`
and the actual byte80 on every call. There is no copied enabled/contact state.

Replace both old player and decor `BodyOwner` callbacks: they cast every peer
to `BodyOwner` and retain invented owner_enabled1. Use
`CharacterWorldPhysicalReceiverV1` over the EXISTING player `prince_body` and
canonical player identity. Its Character services come from
`CharacterWorldPhysicalCharacterV1` borrowing the actual
`prince_state_owner.native_fsm()`, same Debug and actual Character RaiseEvent
endpoint. Result is original POCharacter bx lr. Begin/Persist/End require the
real AI-then-FSM event endpoint; absent endpoint fails after source Debug and
handle prefixes. Do not replace that endpoint with a counter or empty callback.

For all84 current DACT AnimatedDecor owners, preserve their existing canonical
ObjectActor identity (`0x100000002 + descriptor_index`). Register a
`WorldPhysicalBaseActorV1` in the SAME World/Registry8 and retain its source
handle. Borrow actual stable actor.position and constructor-null target node.
Do not create a Character script/health/property owner for decor. The new CPP1
sidecar binds actual DACT SHA and all84 XML records, read from room.visual MVP:
`reference/character-world-physical-peers-v1/crypt01-decor-properties.bin`.
Package it as a new worlds asset when integrating. `initialize_decor` produces
type+f4=0x14, static+84=1 and inherited visible default1; the actual cache has
no visible/static/activation-condition override on these records. Constructor
alone does not initialize byte80. Its receiver kind is base_physical: original
PhysicalObject Begin/Persist/End/Result are whole bx lr bodies, not providers
silently dropped. Use the same actual COLBOX/placement/decor-body config and
NativeWorld body. Current data has82 COLBOX bodies, two genuine absent bodies.
AnimatedDecor IsZonable3884f8 tailcalls MeetCondition38ab60. This derived query
is deliberately unavailable through the generic base-byte2ed shortcut.

For each NPC, retain one new immutable CharacterNpcBodyModel resource, one
WorldNpcObjectFieldsV1, AIS collision fields+bc/+c0, InitFinal fields, and light
output field. Borrow EXISTING m.runtime (including rotation/PF/position/bounds),
source_bounds_flat, machine, controller command view, session/object, ai_events,
common collision counter, common spawn RNG/network mode, actual floor graph
and obstacle registry. Borrow canonical WorldActor.target_node, not another
target-node field. The include validates same animation resources/factory and
SceneBinding before construction. It applies genuine CPO1 NPC property defaults
and the source PF constructor before first PF use. Collision clock/unknown AIS
override/CancelSneaking and real visibility/CharacterStop remain explicit
services. Collider type uses the actual peer registry automatically.

The retained V2 graph runs actual InitPhysical Debug order and allocation, the
existing source InitPost physical/scene/bounds projection, and whole supported
InitFinal spawn/disabled/PF/visual/light/node/VM tail. It borrows the same PF
object and commits bounds and rotation to m.runtime. No second HP, State56,
AIS pause, Save, inventory, position authority or RNG is created. It preserves
failure prefixes. It does not claim the unrecovered complete Character InitPost
or full NPC AI producer graph merely because these stages are composed.

Refresh the NPC canonical World borrow with target_enabled pointing at its
source fields.visible80 and cached_target_position at m.runtime.target_position.
V2 actor_phase computes an actual retained target-node absolute position when
nonnull, then calls the existing actor physical/path/rotation/subobjects phase
using SAME flags and commits source reached position prefixes to script object.
Caller must supply genuine scene/visual/camera/speed/movement policy services;
none is filled with empty success. Original scene sampling, NativeWorld Step,
actor phase and AI/FSM phase remain separate application-order responsibilities.

Death does not automatically invent Disabled or state10 from life.dead. Real
source death FSM focus/event delivery chooses filters/pin/Stop. `disabled()`
preserves visibility/filter prefixes and requires actual CharacterStop. The
source POCharacter state0/state10 filter runs over the same machine.

Before a real World unload: close all NPC graphs while scripts, controller,
World identities and peer contexts are alive; destroy player/decor bodies with
their receivers alive; remove peer registrations; clear canonical World only
after all End delivery; then release FSM/VM/controller/visual/model/floor owners
and NativeWorld. Never clear NativeWorld first and then destroy graph wrappers.

GL resource reload should retain NativeWorld, floor graph, source PFObject and
physical peer graph. Current reset_context clears NativeWorld and level's
unique_ptr floor graph, so it must be separated from source game-world lifetime
before retaining these borrows. Do not reset InitFinal.once1395/PF user or create
a second rotation to disguise that mismatch. A deliberate full game-world
recreation needs actual object lifecycle reconstruction rather than re-running
InitFinal over retained VM/state and stale PF storage.

Host audit uses current new modules and stable original NPC VM fixture, with
explicit application clock and moving-state-ID fixtures. Dependency snapshot
hashes are recorded without claiming current source attribution. Original
constructor/default store oracle is bounded, with whole allocator/XML/asset
factory explicitly outside its claim. Player missing AIS delivery is tested
as required; it is not called a completed player initialization.

## Renderer insertion order and player relay

After the existing canonical player/NPC registration in the player-skills
bootstrap, register the 84 decor base actors before allocating any bodies.
Construct one peer index over that existing World. Retain each decor receiver
and its base registration beside its existing ObjectActor; retain the player
receiver beside prince_body. Replace the old BodyOwner callbacks before
NativeWorld Step can deliver a heterogeneous contact. Register the actual
ground body with an explicit null source owner rather than a guessed player.
Initialize each retained NPC graph only after its machine/session/controller,
animation SceneBinding, actual floor and source object properties exist.
Call initialize_renderer_npc_physical_v2 with those borrows; retain the graph
even when initialization returns a required-provider failure, so its reached
body/PF prefix remains destructible. Close it before releasing any borrower.

Player WorldPhysicalCharacterBorrowV1.raise_event must call the same retained
PlayerSkillsRuntime.events through dh2_character_ai_event, with payload.value
set to the canonical peer identity and event equal to the supplied 37..3c.
The AI service must support collision routing (feature mask 3, as in the NPC
adapter) and forward ai_event_state_event to the actual same prince FSM.
The CharAI virtual relay slots bc/c0/c4/c8 are respectively 3d0dec/3d0e10/
3d0e34/3d0e58. Selected inherited AISDefault methods are 3dbf08 (Begin:
increment shared collision counter then OnCollided), 3dbfa0 (Persist:
OnCollided), 3dbf40 (End: decrement shared counter), and 3dbf68 (Result:
whole bx lr). Any different selected AIS target requires its actual override.
OnCollided requires the same player target/controller/AIS fields, application
frame/dt, World classification/relationship and actual CancelSneaking when
the source player-enemy branch reaches it. The existing renderer event_service
only accepts its literal 20/8c/98 slots and timer/cast branches; it does not
currently implement this required collision receiver. Do not declare player
contact delivery complete until that same-AIS adapter is connected.
