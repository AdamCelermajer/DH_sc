# Full Prince bank host integration

This proof executes the production `libdh2_engine_animation.so` and its genuine `libdh2_scene_materials.so` dependency under ASan/UBSan. It does not link duplicate animation implementations into the test or substitute the quaternion/math algorithms.

Inputs are the staged Prince bank manifest, its 116 hash-checked BDAE resources, and the authored 35-node Prince modular scene. The 158 registration requests must exactly match `reference/prince-registration/probe.json`, which records the original ARM32 producer. The fixture stores resource paths, original clip-bound words, all registration IDs, and the 17 coordinator first-occurrence mappings. The report binds every input, the fixture, both DSOs, the test executable, source files and build/replay commands.

The test keeps 116 canonical Players alive while registering all 158 occurrences. Canonical resource identity is the clip ID, so repetitions borrow the same Player. `set_default(1111)` is separate and must preserve both occurrence and game-ID map counts. `compiled_inputs()` must retain synthetic engine IDs 0 through 157. The compiler uses the actual template Player and `TransformMismatchBehavior::retain` with the authored scene.

After compilation, all borrowed Players and registration objects are destroyed. Every compiled target is sampled at every occurrence's original start and end. It checks surrounding storage canaries, float3 fourth-word retention, and complete output/cursor retention for mode1 bindings without a default. All 158 compiled bounds and the 17 original first indices are checked. Source clip 1138 has no tracks and raw bounds `0x7fffffff/0x80000000`; the compiled raw bounds preserve those words. The legacy Player's convenience start/end values are therefore not used as source bounds.

Current replay: 116 distinct resources, 158 compiled occurrences, 83 union targets, 26,228 samples. These include 8,046 accessor samples, 17,598 default samples and 584 retained samples. The 83 targets comprise 28 position, 28 quaternion and 27 scale handlers; component/angle tracks execute through their raw interpreters even when the first union handler is a full-vector handler. Two targets remain unbound to the Prince scene. Zero ASan/UBSan/leak findings.

Reproduce from the repository root on Windows using Python with WSL and the existing sanitizer CMake build:

```powershell
python port/engine-animation/tests/prince_bank_integration.py --build-dir /home/adampalace/dh2-world-build
```

This is a full-bank native integration and memory/lifetime proof. The source registration order, 17 mappings and raw clip bounds are original-derived; the complete bank's sampled poses are not compared with original ARM32 instructions here. It makes no GPU, actor event/clock, equipment, original manager ownership, APK or live gameplay parity claim. The smaller actual-factory component corpus retains the separate 9,376 original-pose comparisons.
