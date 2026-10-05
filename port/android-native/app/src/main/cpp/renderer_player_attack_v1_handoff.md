# Source player attack wiring

Include `character_world_player_attack_owner_v1.hpp` and
`character_world_attack_geometry_v1.hpp` with the other renderer headers.
Add these declarations to the retained `PlayerSkillsRuntime` (root already
owns the first four fields; do not add duplicate animation AI storage):

```cpp
dh2::character::AttackState64 attack_fields;
dh2::character::ControllerAttackState32 attack_controller{};
std::unique_ptr<dh2::character::skills::CharacterWorldPlayerAttackOwnerV1> attack_owner;
std::unique_ptr<dh2::character::skills::CharacterWorldAttackGeometryV1> attack_geometry;
std::map<std::uintptr_t,std::unique_ptr<dh2::character::skills::CharacterAttackEmptyInventoryV1>> attack_npc_inventories;
static int attack_inventory(void*,std::uintptr_t,dh2::character::skills::WorldAttackInventoryBorrowV1*);
static int attack_kind(void*,std::uintptr_t,std::int32_t*);
static int attack_backend(void*,const dh2::character::AttackRequest32*,dh2::character::AttackResponse16*);
static void bind_attack(PlayerSkillsRuntime&);
```

Include `renderer_player_attack_v1.inc` after `prince_event`/`player_target`
definitions and before player startup. Forward-declare
`std::string source_player_attack_v1(int);` before `player_attack`, then replace
its entire nearest/reach/direct-event body with `return source_player_attack_v1(supplied_target);`.
Call `PlayerSkillsRuntime::bind_attack(*player_skills_runtime)` after publishing
the initialized retained runtime, before any command or attack animator call.
Do not call it on retained GL reload: inventory/Target/FSM/property owners remain
the same. Root's animator begin/end uses the SAME `t.attack_fields`.
Link the two world attack owner/geometry source units and existing
`character_ai_attack.cpp`, `character_attack_geometry.cpp`,
`character_world_ai_can_attack_v1.cpp` (existing target search/bindings units
are already linked). No APK/build/install was performed by this handoff.

The NPC query backing implements only the genuine ItemInventory constructor
period: selected byte+2e=0 (3ff26c), two sets of nine null cells
(3ff288..304). It is not an equipment mutation owner. If NPC source initialization
equips anything, replace that borrow with its actual resulting inventory before
activation; do not retain an empty projection over a mutated source inventory.
Character kind matches the existing registered-world Character kind0 domain.
Source targetability flags/byte415 require the independent original audit; this
adapter uses the existing world query unchanged. No forced targetability flags.

The ordinary Character/type0/interaction8 melee path is complete over actual
source radius/position queries. Non-character/other-interaction fallback remains
explicitly required; `AI_IsInInteractionRange` also reads actual interaction
position and target+2e8 before its own geometry, which this handoff does not fake.
Ranged command redirect and online packet queue remain named required backends.
The source cached projectile shortcut supplies CanRangeAttack without touching
inventory. Diagnostic sites perform actual source DebugSwitches queries with
recovered literal case. SetAttack uses the existing complete same-machine
`prince_event(c354,target)` route; command completion alone does not claim an
accepted FSM transition, hit, damage or whole combat tail.

Source SetTarget clears its actual TargetOwner16.word14d0. Root must connect
that source scalar to its sole constructor combat field on the existing target
store boundary, rather than leaving a duplicate combo authority. Raw OOI+14a4
and signed type+14a8 remain this SAME attack field owner; root HUD projection
must borrow it rather than copy AI+40 as a replacement raw target.
