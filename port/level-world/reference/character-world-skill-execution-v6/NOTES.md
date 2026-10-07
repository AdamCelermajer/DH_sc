# Same-world source skill execution bridge

`CharacterWorldSkillExecutionV6` composes the actual V6 native mutation binding
with `CharacterWorldSkillCombatV6`. It borrows the retained readonly target/mana
binding, real Gear/FSM/SkillTables, world registration, shared CF context and
shared combat RNG. It creates no replacement actor, properties, life, settings,
trophy manager or random stream. The native binding shim only intercepts
original `_SkillCombatRoll` address `0x3b9fbc`; readonly calls keep their original
owner and behavior.

Every actor refresh must borrow the same resolved properties and life registered
in the world. Combat facts must identify those exact properties and the actual
FSM current state. Combo, invulnerability, network ID and push-death borrow the
sole constructor-backed `CharacterConstructorCombatFieldsV1` owner. Existing
logical life fields are mirrors after source writes; legacy combat stores must
be synchronized into those source fields before subsequent execution. UI or
animation labels must not become FSM facts.

The original ctor establishes combo halfword0 at Character+14d0,
invulnerability byte0 at +14f0, ObjectBase network ID-1 at +110 and lifecycle0 at
+11c. FSM ctor zeros its word+3c, including push-death byte+3f (Character+53b).
The controller pointer is an actual borrowed source field. A missing producer
leaves Hit unavailable; a borrowed field whose value is zero preserves the
source null. Lethal branches still require genuine Cmd_Kill.

The host proof uses real cache design/skill/loot/trophy tables and explicit
world/application/aggro fixtures. It exercises real source HP and reciprocal
threat prefixes, real TrophyManager capture and later missing FX rejection. It
does not claim VM dispatch, renderer connection or complete skill application.
The runner hashes frozen shared dependencies and does not attribute them to
current source.

`world_aggro_event_v1` reproduces CharAI::OnAggro3d20c8: actual Debug
load/query `isTracingCharAIEvents`, then selected AIS virtual+38 if present.
Verified inherited AISDefault3dbea4 is complete `bx lr`. AISPlayer3ddb70 has
substantial counters/world/music/timer continuation and requires a real backend.
Selected method identities must come from the actual retained AIS selector.

Application IsSavedOptionOn320e14 has a distinct source policy: absent key=>0;
present descriptor type(+18)==0 and current value==maximum(+c)=>1, otherwise0.
It is not a nonzero-value test. Borrow the existing OwnedHudSettingsV1 owner.

Remaining required paths include positive hit FX, status reactions, cancellation
of sneaking, scrolling text, sound, AI combat tails, lethal controller/Kill, DoT
dictionary/FX and full achievement UI/storage. An unavailable reached provider
returns failure while preserving source mutation prefixes.
