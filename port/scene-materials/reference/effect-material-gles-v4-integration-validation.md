# Shared material/program integration validation

Read-only review of `renderer_effect_scene_v4.inc` found the intended shared
source-key cache and per-material pass/value borrows wired. The test compiles
the actual production draw, program-connection, reflection and typed-profile
includes and calls the native collection, directory, comparison, source
transparent comparator and whole heapsort. It uses two distinct retained
copies of the pinned FIRE BRES and the genuine packaged FIRE PVRTC atlas
through the source texture lifecycle adapter. Actual GL active uniforms supply
names, types, locations and counts; normalization/global partition are native
source kernels. Matrix values come from actual BRES effect defaults.

Standalone run on root-authorized5554:292 checks, three EGL contexts, one
created program per shared source key, four required reflected semantics,
27 equality and eight ordering calls per context. The equal-distance queue
contains both distinct materials. Equal values compare equal; current color
changes compare unequal. Missing producers preserve the last immutable
snapshot while returning failure. FIRE atlas readback is nonblank; changing
current color changes pixels; drawing A after B reproduces A. Dropping one
material preserves the shared GL program; live release deletes it once;
lost-context discard performs no deletion into a replacement context.

Receipt: `reports/effect-material-gles-v4/receipt.json`, including production
include, asset fixture, shader pack and source hashes. Strict standalone
NDK x86_64 compilation passes with warnings treated as errors.

Limits: fullscreen geometry, source node priority/distance and current color
mutations are explicit fixtures. This is actual GLES/cache/directory/draw
acceptance, not full authored simulation, SceneManager registration, user
skill success or the entire integrated renderer. The existing57-check texture
suite is reused for transport definitions but is not rerun by this test.

After the combined APK is ready, root may run:
`python port/scene-materials/tools/run_effect_material_apk_linked_v4.py --run`.
It uses the canonical APK-linked runner, extracts that APK's current libraries
and records their hashes; it changes only isolated `/data/local/tmp` test files.
The current combined APK-linked run also passed292 checks with identical
three-context/reflection/queue/pixel coverage. APK SHA256:
`f51c4c26fd58560520735e7145c075565c28abc378154abb3c0f5fd9760fbd4f`.
Receipt: `port/level-world/reports/android-native-owner-tests/effect-material-gles-v4-linked/receipt.json`.
It includes actual extracted library hashes and the same explicit fixture
limits. No main renderer, CMake, app lifecycle or input edits are
performed by either test runner.
