# Source Deflector and ordered force composition

New production files: particle_deflector_v1.hpp/.cpp,
particle_force_scene_v2.hpp/.cpp and particle_bound_forces_v4.hpp/.cpp.
Reuse existing particle_cloud_models_v1, particle_random_v1 and engine-math.
No renderer/CMake/app files were modified.

The whole PDeflector constructor6349a0 borrows its actual force parameters and
copies the current source matrix65 bytes into retained history. Whole
SParticle100 apply6338e0 preserves finite-plane crossing and rectangle tests,
source friction630abc including logf/expf blend, restitution variation, three
optional source Euler draws, moving-plane inheritance, .3 normal separation,
matrix identity-marker stores, and final history copy even with zero particles.
Only position/velocity fields are written; other raw100 bytes survive.

Actual SForce type2 data has seven float words. Source CDeflectorForceSceneNode
C1 631050 copies them into source fields144..15c; parameter names from whole
serialize6301f8 are bounce, variation, chaos, friction, inheritVelocity, width,
length. The same retained scene stores these parameters once; per-emitter
proxy/model history borrows those fields. No copied mutable parameter authority.
Unknown reached force families fail explicitly. Gravity's existing supported
directional/zero-falloff domain remains unchanged.

Create one shared ParticleForceWorldOwnerV4 for the SAME retained resource Scene.
Supply owner/context/node_world callback calling actual
source_fx_node_world_matrix_v4. This preserves its source-produced marker; do
not infer identity from float equality. It retains one typed force world65 per
node across emitters, applying actual outer68 times authored node world68 once.
Its initial node matrix is read at actual proxy construction. Deliver a source
outer before construction only if that producer has already executed.

For each emitter retain a separate ParticleBoundForcesV4 and call:

```
forces.decode(bres, same_scene, actual_emitter_instance, shared_force_world, error);
forces.update_outer(actual_outer68, error);
forces.apply(same_raw100, count, actual_dt, same_PSRandom_word, error);
```

Call this at the original ordered force stage before Motion and Spin apply.
Bindings preserve the actual emitter URI vector order, including duplicates;
missing source names do not bind, but a found unsupported receiver fails.
The proxy interface contains only destructor/apply, not an init or reset slot.
Cloud timeline reset does not reconstruct proxy history. New resource/actual
rebind constructs a new owner; repeated decode on one owner is rejected.
Stop frame callbacks before destroying proxy owners; release them before the
shared force world/Scene/asset/service leases. No owner escapes its Scene lease.

Actual BashDown cache SHA21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483:
debris instance31884 binds Deflector then Gravity; dust instance31992 count0.
Deflector data30752 is (.27,0,0,.5,1,1000,1000). Gravity data30780 is (5,0,0).
Do not add forces to dust. Zero bounce variation consumes no random draw;
nonzero variation and optional chaos retain original PSRandom ordering.

Evidence: reference/particle-deflector-v1/{original.asm,parameter-names.json,
imports.json,original-fixtures.bin,differential.json}. Original constructor and
whole apply versus compiled ARM64 O2 passed160 byte-exact comparisons over raw
particles, history/current matrix65 and RNG. Libm calls use the existing explicit
oracle projections; historical Bionic libm parity is not claimed.
Actual packaged BashDown native cache test passed on isolated emulator5554;
receipt port/level-world/reports/android-native-owner-tests/particle-bound-forces-v4.
It covers authored debris2/dust0, shared node identity/parameters, positive finite
collision, history updates, source ordering and missing-producer failure.
This is subsystem validation, not an APK/live mixed-FX draw claim.

Remaining integration belongs to CharacterFxForceFactoryV4 and its retained
resource/typed outer68 owner. Connect create/decode and the existing V3 cloud
force callback; preserve first-particle-node completion and actual camera/GPU
providers owned by the mixed resource adapter. Attribute serialize/deserialize
dispatch beyond the decoded authored constructor fields remains a separate
reached service, not a successful empty callback supplied here.
