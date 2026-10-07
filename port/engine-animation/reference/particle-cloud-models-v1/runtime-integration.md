# Actual blood cloud runtime integration

`ParticleCloudRuntimeV1` borrows one `ParticleEmissionOwner`, one registered
`ParticleCloudModelsV1`, and the source PSRandom seed. The caller retains those
owners for the resource lifetime. Register model storage before initializing
the authored parameter tail. Construct the sphere through
`dh2_particle_sphere_construct_v1(center, authored RadiusLength, 0)`.

The selected descriptor3/mode0 whole Mixin constructor produces seed123456789,
context bytes4/5/20 zero, and null Color streams. Leaf context construction
alone does not initialize those flags. GLES2 driver type8 supplies white via
`particle_scene_color_v1`; no invented ambient default is needed.

Initialize requires the real render-buffer initializer. Each `update` receives
source absolute scene seconds, the same emitter world matrix, actual source
color, and ordered same-scene gravity bindings. Compose the FX root once with
each authored emitter/force matrix. The update owns no second particle array.
It performs source generation, ordered new-particle model initialization,
life/compaction, color, size, force, motion, spin, then mandatory render apply.
The render adapter uses `dh2_particle_billboard_apply_v1` for source distance
sorting and bounds; camera view supplies `basis_v1`; `vertices_v1` emits four
ordered vertices. `uv_v1` supplies source startup UV values. Source indices
are `(0,1,2,0,2,3)+4*particle`; resource material and GPU buffer ownership belong
to the retained resource adapter. No empty render callback is a production
provider.

Authored animation sampling remains separate from cloud simulation. Use the
authored segment bounds for manager timeline duration, rather than the last
BirthRate sampler key. End sampling must not simulate a second cloud frame or
invent a generation-disable write. Completion stays with the source resource
query/manager integration.

Available domain covers actual blood Life/Size, sphere emitter, nonzero .3
direction variation, axis0 Spin, real directional gravity with zero falloff,
source color fallback, actual capacity30 sorting/bounds/spun billboards. Other
spin-axis variations and point/falloff force branches explicitly fail when
reached. Missing material/camera/GPU services are caller obligations.

Validation reports: models1000 original ARM comparisons; basis/corners500;
whole render-sort/bounds500; whole generation/update graph200. Graph original
instructions include generation65317c and update6532cc with a reserved vector
fixture and explicit allocator-cleanup boundary. Host O1/O2 ASAN+UBSAN replay
2200 gold cases plus two unavailable-provider checks and both actual blood
resources. Only host corner trig comparison allows4 ULP; ARM64 comparisons
are byte exact. Host camera is an explicit fixture, not a live-game claim.
