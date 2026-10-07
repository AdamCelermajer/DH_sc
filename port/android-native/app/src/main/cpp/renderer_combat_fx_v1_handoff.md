This handoff owns only source FX/presentation resources. Gameplay target, health,
state, rotation and time are borrowed from the existing player/world authority.

Add the `character_combat_fx_runtime_v1.hpp` header and compile that unit plus
`character_combat_hit_fx_v1.cpp`. Keep frozen four FX owner units unchanged.
Include `renderer_combat_fx_draw_v1.inc` after existing `upload`, `release`,
`Draw` definitions; include `renderer_combat_fx_runtime_v1.inc` after the player
runtime definition and draw adapter. Retain:

```cpp
std::unique_ptr<dh2::character::skills::CharacterCombatFxRuntimeV1> combat_fx;
std::map<std::uintptr_t,const dh2::actor::RotationState*> combat_fx_rotations;
static bool combat_fx_asset(void*,const char*,std::vector<std::uint8_t>&,std::string&);
static int combat_fx_rotation(void*,std::uintptr_t,float[3]);
static bool combat_fx_remaining(void*,dh2::fx::MeshFxRequestV1&,std::string&);
static void bind_combat_fx(PlayerSkillsRuntime&);
int combat_fx_application(const dh2::character::skills::SkillApplyRequestV6&,const dh2::data::CombatResult&);
int combat_fx_frame(std::int32_t source_absolute_ms,std::int32_t source_app_dt);
```

`combat_fx_rotation` copies the same target Character's actual source
RotationState.rotation: player `prince_runtime.rotation.rotation`, NPC the
RotationState retained by CharacterWorldNpcSceneBridgeV1 after real InitPost.
The player borrow is registered by bind_combat_fx. Immediately after that call,
register each actual NPC rotation pointer in combat_fx_rotations by its same
World identity. This map owns no rotations; the scene bridge/runtime is sole
authority. Clear and re-register aliases after replacement, before any hit.
No fallback to zero, initial placement, render matrix or another actor heading.
`combat_fx_remaining` currently reports reached required anchor/floor operations.
Fixed-rotation, null-anchor HitFX does not require floor-normal queries; a
reached different authored branch must fail until its actual provider is bound.

Construct after same World/Effects/Debug/player CPU Scene exists, then route
application service7 through `combat_fx_application`: return1 means provider0,
return0 means other service, negative is explicit failure. `fx_` authored events
use the retained owner's animation_event with the genuine full prefix and
actual registered owner. Never additionally spawn a development hit sprite.

On every source scene frame call `combat_fx_frame(actual_absolute_ms,actual_dt)`
after scene/actor transforms update and before render submission. It preserves
Scene timeline then VisualFXManager update/return-pool ordering, retains real
draw parts and uploads original geometry/materials/textures. Submit each
`combat_fx_draws` batch through the existing material-aware `submit` lambda:
`submit(batch,scene::multiply(projection,batch.placement))`. Use source material
blend/alpha/depth semantics already in that renderer; do not force opaque FX.

Before CPU Scene replacement or world destruction destroy `combat_fx`. On
ordinary GL reload release `combat_fx_draws/combat_fx_images` while context is
valid and clear retained parts/mapping; re-create the owner against the new
actual CPU Scene after native gameplay restore. On lost-context reset clear GL
IDs without deleting through a dead context. Destroy source manager before its
Scene, Debug/files, asset/platform and world handles die. This is the same stale
borrow class as the previously fixed floor reload, so retained-reload branch
must explicitly rebind this owner.

Current limitation: actual blood79/80 assets contain a particle emitter and a
GNPS cloud. The frozen mesh manager rejects their constructor family. This
handoff is stable for supported source mesh sets and preserves the blood
failure; it must receive the particle-capable source extension before claiming
accepted service7 for a positive blood hit. Metadata type0/oneStep is not proof
of mesh-only content. Actual positive-resource test currently records that
required failure, not a successful GPU or blood animation claim.
