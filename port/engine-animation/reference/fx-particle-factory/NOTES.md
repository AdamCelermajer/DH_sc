# Owned particle context and generation subset

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. This stage supplies concrete native context/generation storage, hash registry, retained parameter leases and the exact recovered emitter initialization prefix. It does not supply a usable particle cloud or renderer.

## Correct constructor identities and fields

Original context constructor64d1d0 registers **AnimationDatabase**, with storage at context+58 (hex). The earlier BBox interpretation was incorrect and was rejected by actual constructor/registry differential execution. BBox is separately registered by the complete render-data model. The context is5c bytes; +58 is a four-byte source pointer storage cell, not an inline bounding box. The source context constructor leaves that cell untouched. A native database-pointer mapping is required before a real provider interprets it; the present projection preserves its explicit source32 seed and never dereferences it as a native object.

The source zeros words+8/+c/+10/+14/+18/+1c, words+24/+28/+2c, maproot+34/count+40, words+48/+4c/+50, and individual bytes+21/+30/+54. It initializes map end links and vtable. Other bytes retain allocation contents. `ParticleContextSeed92` is a diagnostic constructor-memory projection; vtable/map process addresses normalize to zero during comparisons. All other bytes are compared, including untouched padding and AnimationDatabase's seeded cell. This projection is distinct from native class layout.

Source generation constructor6545d0 initializes BirthRate1.0f, MaxParticles1 and two remaining words0; registration order is AnimationDatabase→BirthRate→MaxParticles. Native generation fields have genuine stable native storage. A BirthRate lease retains that owner after callers release it. The native source hash operates on signed bytes, `seed ^= byte + 9e3779b9 + (seed<<6) + (seed>>2)`, with32-bit overflow. The map uses hash identity exclusively; unique insertion retains the first pointer, including null. Missing lookup inserts null. Word setters write only nonnull storage. A proven real collision pair is hex`2553482e34475a5f2e` and`6e79284d705a7e2c4a`, both hash`c66e9677`.

Registry providers may supply a shared model owner with their real storage; parameter leases then retain that model through the registry owner. A provider that supplies only a borrowed pointer must keep it alive through all owner/parameter leases. No cyclic ownership or arbitrary process-pointer interpretation is reconstructed.

## Exact FX77 resource inputs

FX283 `zombie_spawn_fx.bdae`:17,288 bytes, SHA256 `b54e3b05ba684eca654c8a58b8ebd5e84f7cad25929e0b5a60405f66dfdc772e`. Original getEmitter60e468 and name lookup61a7f4 establish root+78 count/root+7c table and exact0x90 stride.

The authored rows are `gl_pcloud_debris-emitter` and `gl_pcloud_dust-emitter`. Both record+50 mode0, descriptor at record+54 has type3 and flags0. Native decoder deep-copies the complete BRES backing,144-byte record,36-byte descriptor and exact shape words; it preserves scalar bits and file offsets, without normalization or pointer widening. Original-derived report binds every copied range/hash. Native BRES validation uses genuine `resources.cpp`. The bounded decoder rejects other descriptor/mode/shape domains explicitly and leaves output unchanged on failure.

Original node factory634c64 executes mode0/descriptor3→634ce8, calls node virtual+f4 `initParticleSystem(driver,true)` and returns the allocated receiver. The probe executes both actual branches with required graph-constructor and initialization providers. It does not treat those services as completed production backends.

`initialize_particle_generation` reproduces the recovered source prefix of656450: EmitterType, then type0 RadiusLength/Width/Height, type1 RadiusLength, or type2 RadiusLength/Height, then MaxParticles(record+18), then BirthRate(record+20). Both current FX rows author MaxParticles20 and BirthRate0. Shape/type providers are required real receiver services. Missing providers fail explicitly; delivered provider failures preserve the already applied prefix. Provider diagnostic failure propagation is a native boundary—the original setters return void. Malformed input is rejected before side effects. Repeated calls remain allowed.

The original differential executes656450 through the BirthRate setter, redirecting656500 to the actual epilogue656c88 before Life and remaining models. The cloud factory return is an explicit preconstructed context/generation fixture; other-model storage is a declared borrowed provider fixture. This is prefix parity, not full init parity.

## Complete constructor capture and required next backends

The separate probe executes createPCloudSystem655314 and BOTH entire mixin constructors654ec4/654a74 with a raw allocator seeded0xcd, source hash/STL and modeled single-thread allocator locks. Each source cloud allocates0x1dc, zeroes first0x180, constructs context at+180, and registers41 owned slots. The source AnimationDatabase cell at+1d8 retains0xcd. The complete field buffers and all registry storage offsets are captured and hash-bound.

Typed constructor call order is context→generation→size→color→emitter→motion→spin→life, with source inline force-model setup between motion/spin and inline render-data setup after life. True selects billboard color/normal/position/UV bakers; false selects null-color/generic bakers. This new native module supplies context and generation only. Required next bodies include size/color/emitter/motion/force/spin/life/render-data constructor/storage implementations, true-cloud simulation virtuals, particle buffers/bakers, material and shader factories, renderer driver and real attachment receivers. AnimationDatabase requires a genuine native resource identity and lifetime mapping. None of these missing operations may return fabricated success.

Release order for the eventual full graph must follow source owned model teardown, with context/parameter storage alive through callbacks; retained model leases must protect borrowed animation bindings. Current leases prove stable generation and explicitly owned provider storage, rather than claiming that complete teardown hierarchy.

## Proofs and integration

Original↔NDK29 O2:1,850 comparisons,160 constructor-memory cases,1,410 hashes,7,200 registry commands and40 real-FX initialization prefixes; zero mismatches. ASan/UBSan replays the same gold through an actual isolated DSO, plus11 ownership checks and4 malformed/provider contract checks; zero findings. Complete source constructor probe:2 cases ×41 owned slots; node factory probe:2 actual FX branches. IEEE word setters preserve bits.

Add `particle_factory.cpp` to the animation library linked to genuine engine-resources. Host target `particle_factory_audit` links `tests/particle_factory.cpp` against that actual DSO; argv1=`reference/fx-particle-factory/particle-factory-fixtures.bin`, argv2=actual zombie_spawn_fx BDAE. No math wrappers. Current isolated host script compiles resources.cpp directly into its private DSO; it neither rebuilds shared DSOs nor mutates Android artifacts. Frozen particle_parameter/material_color proofs remain unchanged.
