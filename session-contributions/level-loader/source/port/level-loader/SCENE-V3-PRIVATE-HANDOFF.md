# Verified private loader scene-V3 composition

The exact frozen retained scene/root/visibility/animator owners now build and
run in this loader worktree. All three original swamp chest BDAEs pass normal
host, ASan/UBSan/leak and Android x86_64 execution; ARM64 compiles/links. The
test checks real same-root membership, visibility propagation, authored
animation, actual track removal with retained sampled pose, ordinary removal
after prior detach, and the reached PF-failure cleanup prefix. Asset reads and
PF remain declared boundary providers. There are no scene/visibility/animator
success stubs. This does not instantiate authored OpenableContainer factories
at SWAMP placements or render a complete level.

Provenance: connected-owner archive29bdb6fb..., its exact PropertyMap include
supplement, and scene-V3 archive93318b2b... form a new immutable184-file layer.
Missing lifecycle header85cd574b... was captured explicitly as current source;
main subsequently supplied immutable archive40989e6c..., verifying those exact
bytes plus11 byte-identical existing dependency headers. No conflicts or prior
owner overwrites occurred. The supplement and comparisons are retained.

Frozen VisualV1 uses std::runtime_error without including <stdexcept>. This
probe supplies -include stdexcept only to that TU, keeping archived bytes
unchanged. Root was informed to add the missing standard include in its source.
The probe links actual existing transform/navigation/physical producers. Every
reached project source/header is hashed and included in the dependency receipt;
no whole gameplay/runtime readiness follows from the standalone link.

Reproduce from repository root using the original supplied cache:

```
python -B port/level-loader/tools/extract_scene_v3_chest_assets.py
python -B port/level-loader/tools/build_scene_v3_probe.py host
python -B port/level-loader/tools/build_scene_v3_probe.py sanitizers
python -B port/level-loader/tools/build_scene_v3_probe.py x86_64
python -B port/level-loader/tools/build_scene_v3_probe.py arm64-v8a
python -B port/level-loader/tools/run_scene_v3_checks.py
```

Host commands use Ubuntu WSL and CMake/Ninja. Android commands use the existing
SDK/NDK29.0.14206865. The runner explicitly checks DH2_Loader_API37 on5590 before
each adb operation. Its native executable/assets live in a separate temporary
device directory. The map APK is unchanged; main5554/menu5580 are untouched.
Loader5590 was confirmed absent by process command lines before its visible AVD
was relaunched with preserved data. No other emulator was restarted.

Production integration must reuse the actual canonical owner target and SAME
scene manager, not link this test's standalone copy into a second world. Use
RetainedSceneVisualConnectionV3 for each real canonical base with its actual
asset/PF/world providers. Candidate rejection calls discard_failed; successful
attachment detaches through source SetVisualObject before discard_unattached.
Resource-domain limits (optimized static/subscene branches and draw-list
registration), complete ModuleInitPost, all-class construction, restoration and
whole SWAMP acceptance remain pending. Earlier dependency-blocked report is
historical; scene-v3-private-dependency-resolved.json records its resolution.
