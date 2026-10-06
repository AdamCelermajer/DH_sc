# Whole collision lifecycle receiver

Compile character_collision_lifecycle_v1.cpp and native_physical_filter_v1.cpp
with level-world. Include their headers and renderer_player_collisions_v1.inc
after the retained PlayerSkillsRuntime definition. Strict bothABI compilePASS.

Members/declarations:
```
b2Shape* collision_primary{};
b2Shape* collision_secondary{};
std::uint8_t collision_filter_disabled{};
std::unique_ptr<dh2::character::CharacterCollisionLifecycleV1> collisions;
static dh2::physical::NativeBody* collision_physical(void*);
static int collision_method(void*,dh2::character::WorldNpcObjectMethodV1,std::string&);
static void bind_collisions(PlayerSkillsRuntime&);
int enable_collisions_source(std::uintptr_t);
```
Initialize primary from the actual single shape returned by the existing
player create_character factory ONCE after allocation. Secondary is the source
PhysicalObject constructor-null field; disabled byte26 starts0. Reset these
only when the original physical owner is recreated, never on GL/menu reload.
Borrow existing prince_initial_filter as source saved20/22/24; never replace
with current temporary death/disabled shape filters. Bind once after actual
player registration; retained level reload dynamically rebinds geometry.

event_service's exact ai_event_helper case operation394a3c, subjectCharacter:
```
return t.enable_collisions_source(q->subject);
```
Return0 delivered matches AIEventServices. Do not independently deliver FSM
event3f; dh2_character_ai_event does so only AFTER successful whole helper.
Missing actor/world/filter/nav leaves the original executed prefix visible.

Source394a3c order: optional actualPhysicalObject enableFilter46ebe4;
actual virtualb4; only iftrue virtualb8 thenbc; actual PFWorld.InitObstacle
528234(enabled1,weight,extent), EVEN when physical pointer is null. Character
virtual leaves3a2ee8/3a2ef0/3a2efc are literaltrue/50/20 (not gameplay defaults).
Disable3949b0 identical with physicaldisable46eb70 and enabled0.

Native filter helper restores exact saved halfwords to primary then secondary
and calls real Box2D Refilter after each; final byte26 publishes only after
successful synchronous delivery. Disable writes all three halfwords0. Source
already-disabled/enabled branch still writes canonical final byte1/0. No
blanket filter success. Existing NPC physical owner already implements these
source functions; NPC lifecycle should delegate to it instead of another
filter owner. CharacterCollisionLifecycleV1 can borrow any actor's SAME
PFObject/registry/geometry and WorldNpcObjectServicesV1.

Host/original differential checks are pending. Current WSL environment did
not service preceding host commands; no broad restart performed. Source
function extraction is private inspect_collision_enable_v1.py; recovered
navigation InitObstacle already has original differential coverage.
