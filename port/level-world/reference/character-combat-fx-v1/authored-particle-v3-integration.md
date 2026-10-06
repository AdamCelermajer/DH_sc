# Authored particle factory V3

Use `CharacterAuthoredParticleFxFactoryV3` from
`character_authored_particle_fx_v3.hpp` in the retained combat FX runtime.
Its constructor and `factory()` are identical to BloodV2. Keep the SAME actual
camera/driver, resource bytes, manager, actor rotation and source event time.
V2's source and behavior are preserved; this is a separate successor.

Link new production units:

- level-world/character_authored_particle_fx_v3.cpp
- engine-animation/particle_box_v2.cpp
- engine-animation/particle_cloud_runtime_v2.cpp
- engine-animation/particle_resource_init_v2.cpp
- engine-animation/material_color_v3.cpp

The existing engine-animation/material_color.cpp remains required. Source
models, generation/emission, random, forces, billboard and scene dependencies
are the same as V2.

Actual FIRE `spell_dh2_hurt_fire.bdae` has descriptor3/mode0, box0 size
(200,20,20), capacity20, and a nested `GL_pcloud-emitter`. Traversal uses the
complete serialized scene forest and child order. Material `_1_-_Defaultjh`
binds its actual ProfileCOMMON effect. BirthRate uses the existing scalar28
owner. Material channel86 uses uchar4 (not FX77 alpha-only) values at source
frame keys0,2,4,13,14,16,18,20. Whole source6268b0 interpolation accumulates
two weighted products from zero; the source next key is adjacent. Source
SColor conversion5cad38 writes float4 with exact0x3b808081 factor and source
byte order. The immutable default is black; the actual animation produces
orange/yellow and returns to black. No white diffuse fallback is supplied.

Read dynamic `DiffuseColor` from
`source.part.material_table->at(source.material).color` in the GPU submission.
Do not reapply the immutable BRES default over the sampled runtime material.
The metadata retains the exact BRES/effect/technique and same world emitter.
`camera_offset_word`, `rendering_layer` borrow constructor-backed source node
values: original ISceneNode C1 stores0 at599404/+128 and599420/+12c; actual
getters5974e4/5974f4 return those words. No layer/camera-offset setters are
introduced in this resource domain. Future source mutations need the same
resource owner setter, not a second queue property authority.

Proof receipts: `engine-animation/reports/particle-box-v2-original.json`
(500 original ctor/transform/generate cases),
`engine-animation/reports/material-color-v3-original.json` (600 original
full-color cases including200 actual FIRE keys), and
`level-world/reports/character-authored-particle-fx-v3-host.json` (both O1/O2
ASan/UBSan, actual blood79/80+FIRE124 initialization/simulation/color/bake and
source completion). Camera is a host fixture. These tests do not claim GL,
live cast completion, texture descriptor or SceneManager ordering parity.

Unsupported particle families/descriptors/modes, external material bindings,
other shader profiles, other material animation parameters/sampler types and
other reached source services remain explicit errors. This successor removes
the blood name/capacity assumption by composing the real reached box branch,
not by accepting unknown shapes or fabricated shader outputs.
