# Source actor lighting boundary

`actor_lighting.hpp/.cpp` provides typed source light data and a bounded asset
adapter. It does not select a rendering technique for actors or tune the preview
renderer. Main and renderer are unchanged by this module.

Recovered source facts:

- `Character::InitLightSetAndMaterial` at `0x3b4834` initializes five true filter
  bits, selects `MonsterLight`, applies its LightSet, then applies material/base
  parameters/shadow/xray. `InitFinal` at `0x3b4978` selects `SceneLight` for its
  player branch and `MonsterLight` otherwise.
- `LightSetManager` constructor `0x40d7d4` initializes four sets of five null
  light pointers. The names PlayerLight/SceneLight/CameraLight/MonsterLight do
  not declare light colors. `GetLight` at `0x40c40c` indexes these exact slots.
- `ApplySettings` at `0x40c9a8` loops only four source slots. Enabled nonnull
  lights with existing shader parameters compact into `light0` onward. Disabled
  slots subsequently bind the corresponding dummy-off pointer, including null.
  A nonexistent shader parameter does not advance the compact index.
- `LightBase::DeclareProperties` at `0x40adf0` declares zero position,
  attenuation and RGB colors, radius two, false automatic flag. `InitPost` at
  `0x40b0c0` divides RGB by 255, attenuation Y by 1000 and Z by 1000000.
  `SyncData` at `0x40a8f8` publishes those values into the actual CLight.

The original cache `data/3d/light/swamp.lightset_xml` declares one PlayerLight,
white ambient/diffuse/specular RGB, attached to Player with offset (0,0,200),
attenuation (.265625,0,.132813), radius 3.5. The ES1.1 file instead declares
(.15625,0,.484375). Neither file supplies MonsterLight. Parsing a module does
not automatically publish its lights into named sets: actual source assignment
and attachment producers remain required.

The visible lizard body binds material `diffuse`, embedded effect
`#ProfileCOMMON_diffuse-fx1302961559_lizardman`, with empty external effectFile
and technique. Its authored vertex color is uniformly white. Other library
materials mentioning `GL_Diffuse_L1_VC_iPhone.bdae` are not its body binding.
The native runtime selection of the body's effective technique/preamble must
be recovered before enabling a lighting branch. A vertex COLOR stream alone
does not establish that a shader is unlit or that its colors contain lighting.

The supplied `shaders.pak` contains `ProfileCOMMON_emul_VS.glsl`. Its LIGHTING
branch uses unnormalized transformed normals, distance attenuation for ambient,
diffuse and specular, fixed eye vector (0,0,1), and clamps computed color. That
branch replaces `Color0 * DiffuseColor`; it does not subsequently multiply by
vertex color. The CPU evaluator is a readable finite-domain reference for that
equation, not ARM bit equivalence or a GLES precision/GPU proof. The
`GL_Diffuse_L1_iPhone_VS.glsl` shader has a different equation (unattenuated
ambient and vertex RGB multiplication), so the evaluator must not stand in for
it. The classifier distinguishes these known filenames and rejects unknown or
conditional macro environments.

Validation: `tests/actor_lighting_tests.cpp` reads the actual Swamp lightset,
checks unit conversion, missing MonsterLight, filter compaction, dummy ordering,
fifth-slot exclusion, absent shader parameters, unknown/conditional shader
selection, non-normalized normals, ambient attenuation and undefined inputs.
Compiled with native Clang C++17 and run successfully. No visual fidelity claim
or replacement MonsterLight constants are introduced.

Source evidence is the ignored local IDA export under
`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode`
at the addresses above, the original cache lightset, and the exact shader pack
at `.local-inputs/effect-cache-v4/shaders.pak`. Existing source light-name
recovery is in `port/level-world/light_set_name_owner_v3.hpp`; this foundation
adapter does not replace that canonical owner's lifetime or pointer storage.
