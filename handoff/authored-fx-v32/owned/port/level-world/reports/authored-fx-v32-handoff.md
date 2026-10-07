# Authored FX resource continuation V32

V32 closes all seven failures reproduced in the actual 45-resource V6 census. It is an independent factory; V6 and shared renderer/application/CMake files were not edited by this session. The frozen tree is `handoff/authored-fx-v32` and its manifest is authoritative for integration.

## Behavior and original evidence

- Dark Queen second-form projectile: original initializer `656450` stores SpinAxisType=2, takes the zero SpinAxis/variation fallback at `656868`, and the original spin model `64e3c8` takes velocity from the same particle's +0xc..+0x14. V32 initializes this domain and reuses the existing exact spin model. DirectionType remains 1. AxisType1 still requires another initializer continuation.
- Dragon attack01: the failure was the artificial thirty-particle limit in V1 billboard apply, not a newly required force model. V32 retains its same-vector sort, RNG/clock/force path and bounds computation through the factory's existing 16-bit quad capacity (16,383 particles). Original `651eec` has no thirty-particle check. Real source sort/BBox and compiled ARM64 outputs agree byte for byte through n=256, including n=31/45 and equal-distance particle identities.
- Dragon attack02/03 and intimidate01/02/03: the kind1 `Circle01` geometry is not a triangle payload. Original `CColladaDatabase::constructGeometry` at `60e634` returns a null IMesh for every one of these actual declarations. The instance/node branches at `61aeb8` and `61b2f4` skip creating its mesh child. V32 retains the original graph, node transforms/tracks and raw five-word declaration `(0,15,3,0,0)`; it emits the four actual kind0 meshes and both source particle clouds. It does not manufacture a Circle mesh or report an empty backend as a drawable mesh. Other kind1 layouts and kind2+ remain explicit required continuations.

The original constructor's five null results were executed against relocated original-cache bytes. Initializer proof uses explicit parameter-registry callbacks and exits before original driver/render initialization. Camera, driver8 and outer placement in resource tests are explicit fixtures. Imported float/libm services in the instruction oracle are modeled as in the existing original-code proof harness. No legacy instructions or Unicorn are dependencies of production code.

## Validation

- Fresh V6 baseline: 38/45, 738 frames; exact same seven failures.
- V32: 45/45 through each authored segment end plus 2,000 ms, 5,541 CPU frames. All seven formerly rejected resources produce their real mesh and positive particle clouds, including delayed intimidate emitters.
- Original vs O2 ARM64: 240 sort/BBox cases, 160 velocity-axis spin cases, five real geometry null-return cases; byte identity.
- O1 Linux host runs use ASan, UBSan, leak detection and strict FP arithmetic. Packet audit: 3,341,558 checks, 15,428 mesh packets, 2,437 particle packets. Skin/scalar audit: 5,665 checks, five moving skinned resources, two same-model spin tracks.
- Strict ARM64 and x86_64 compilation: ten production TU compiles with Wall/Wextra/Werror.
- See `authored-fx-v32-source-v32.json` for original-gold host replay and parameter-cell/negative-domain checks.

## Integration

Add these five new production TUs to their existing libraries:

1. level-world: `character_authored_resource_v32.cpp`, `authored_fx_mesh_graph_v32.cpp`, `authored_fx_nonrender_geometry_v32.cpp`.
2. engine-animation: `particle_resource_init_v32.cpp`, `particle_billboard_v32.cpp`.

Include `character_authored_resource_v32.hpp` and replace the sole retained factory member's V6 type with `CharacterAuthoredResourceFactoryV32`. Its constructor and `factory()` contract are unchanged. Leave the old factory compiled if other users need it. V32 has distinct receiver/Impl symbols and calls existing V3 runtime, V4 force owner and V5 transforms. The old V1 billboard basis/corners/vertices and V1 spin implementation remain linked. Do not add duplicate copies of existing dependency TUs already built by the project.

The manifest pins exact owned files, inherited local source/header dependencies, host snapshot libraries, original library, cache resources, receipts and gold fixtures. Frozen inherited dependencies are reference snapshots; integrate owned V32 files against a compatible existing closure. Resource metadata, skinning, material/profile, texture matrices, source identities, outer68, queue-facing interfaces and cloud clocks retain their existing implementations.

## Remaining boundaries

No unsupported case remains in this 45-resource census. This is not an exhaustive cache-wide census: GNPS, coronas, emitter type2, direction types0/2, spin axis1 initialization, other geometry declarations, and existing material/animation guards outside this actual domain remain required. Hardware-skin driver selection, renderer GPU acceptance, actual gameplay FX timing, lighting/camera authorities and audible/visible live acceptance are separate integration tests. Neither emulator nor ADB nor the loader worktree was accessed.
