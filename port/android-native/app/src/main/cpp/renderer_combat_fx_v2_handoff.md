The V2 handoff replaces the earlier mesh-only V1 presentation handoff. Keep the
four frozen V1 FX units unchanged. V2 retains their source state/pool kernels and
adds an actual particle constructor for the two original blood resources.

Include `character_combat_fx_runtime_v2.hpp` and
`character_blood_fx_resource_v2.hpp`. Add these translation units to the existing
native libraries, once each:

* level-world: `character_mesh_fx_owner_v2.cpp`,
  `character_blood_fx_resource_v2.cpp`, `character_combat_hit_fx_v2.cpp`,
  `character_combat_fx_runtime_v2.cpp`.
* engine-animation: `particle_random_v1.cpp`, `particle_resource_init_v1.cpp`,
  `particle_force_scene_v1.cpp`, `particle_scene_color_v1.cpp`,
  `particle_cloud_models_v1.cpp`, `particle_cloud_runtime_v1.cpp`,
  `particle_billboard_v1.cpp`. Existing `particle_factory.cpp`,
  `particle_emission.cpp` and parameter kernels must be linked as well.
* scene-materials: `particle_scene_v1.cpp`.

Include unchanged `renderer_combat_fx_draw_v1.inc` after Draw/upload/release,
then `renderer_combat_fx_runtime_v2.inc` after PlayerSkillsRuntime. Retain these
members and declarations, replacing earlier V1 declarations:

```cpp
// Factory must outlive manager; explicit reset manager first on reload.
std::unique_ptr<dh2::fx::CharacterBloodFxFactoryV2> combat_fx_particles;
std::unique_ptr<dh2::character::skills::CharacterCombatFxRuntimeV2> combat_fx;
std::map<std::uintptr_t,const dh2::actor::RotationState*> combat_fx_rotations;
static bool combat_fx_asset(void*,const char*,std::vector<std::uint8_t>&,std::string&);
static int combat_fx_rotation(void*,std::uintptr_t,float[3]);
static bool combat_fx_remaining(void*,dh2::fx::MeshFxRequestV1&,std::string&);
static bool combat_fx_camera(void*,float[16],float[3],std::string&);
static bool combat_fx_driver(void*,std::uint32_t&,std::string&);
static void bind_combat_fx(PlayerSkillsRuntime&);
int combat_fx_application(const dh2::character::skills::SkillApplyRequestV6&,const dh2::data::CombatResult&);
int combat_fx_frame(std::int32_t,std::int32_t);
```

Bind after the actual World/Effects/Debug/CPU Scene exists. Register each NPC's
same live `m.runtime.rotation` pointer by World identity after initialization;
player uses `prince_runtime.rotation`. Source HitFX uses the TARGET rotation16c.
No initial-position or zero-heading fallback. Route application service7 to
`combat_fx_application(q, same actual CombatResult)`: 1 handled maps provider0,
0 is another service, negative preserves `combat_fx->error()`.

The camera callback borrows root's `submitted_view` and `submitted_eye`, captured
from the SAME camera function's actual view/eye locals, with valid submitted
width/height. Run camera preparation before particle update in each frame so
the simulation/baker uses this frame's view and eye, rather than a stale previous
frame. The combined projection matrix is not a view substitute. Driver8 is the
verified original GLES2 literal; its `(driver&7)==0` source path produces white
scene color. Other drivers require the original lighting query and explicitly
fail here. The callback does not claim recovery of the original game camera.

Each frame after actor transforms and camera preparation call
`combat_fx_frame(actual source absolute milliseconds, actual App dt)`. It samples
the source animation, advances same cloud simulation, applies actual emitter
and gravity node transforms composed with the outer FX transform, then runs
the source manager completion/pool logic and syncs immutable GPU draw snapshots.
Submit `combat_fx_draws` with the existing material-aware submit lambda and
`scene::multiply(projection,batch.placement)`. Particle snapshots already carry
world positions, so their placement is identity. Original material and texture
UV/colors remain attached. No development hit sprite is spawned.

Before CPU scene/platform replacement destroy combat_fx FIRST, then factory,
clear borrowed rotations and release GL draws/images/retained snapshots while
the context is alive. Bind fresh owners and register fresh same-actor aliases
after restore. Lost-context paths clear GL IDs without deletes. Do this on the
retained gameplay reload branch too; a surviving raw scene borrow is invalid.

The original resources are exact `data/3D/FX/bloodsplat.bdae` and hero variant;
read through the existing cache provider. No stripped resource or authored seed
substitution. Source Mixin RNG123456789, capacity30, sphere radius, .3 directional
variation, gravity1/0/0 and actual 1000ms segment bounds are composed. Last scalar
key166ms is distinct from the segment end and must not set manager duration.

The host test verifies both actual resources, positive source simulation and
retained billboard data, expiry/pool/warm reuse with sanitizers. Live GPU upload,
material rendering and service7 end-to-end acceptance require the parent app
smoke check. Unsupported particle families, ambient branch, missing camera,
anchors/floor queries and source resource failures remain explicit errors.

Actual blood material `fx_particles_alpha` has diffuse-sampler and
transparent-sampler, both selecting the same atlas image0. Its source default
ProfileCOMMON shader multiplies Sampler0 RGBA by vertex color and DiffuseColor;
it does not separately sample transparent-sampler. The transparent sampler is
not mapped to AlphaMap, whose native shader multiplies RED. Source effect
defaults supply DiffuseColor and diffuse-sampler-matrix. The original vertex
shader uses matrix*vec4(UV,1,0), while the native shader uses w1. V2 bakes the
source UV equation into vertices and leaves the native texture matrix identity.
Blood tile U=.25 and hero tile U=.125, both scale=.125, are tested against the
actual assets. Active shader/material blend/depth configuration and live GPU
appearance still require the parent app check before pixel-equivalence claims.
