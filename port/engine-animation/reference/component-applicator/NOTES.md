# Concrete node component contribution and application

The original ARM32 ELF is SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. `original-functions.json` captures 41 actual functions, including the factory, seven lazy singleton producers, six concrete float component classes and the original node setters. `reference/original-functions.asm` preserves their instructions.

## Factory numeric identity

`CColladaDatabase::getAnimationTrackEx` at `0x611ae0` loads channel type from `animation+0x10 -> channel+8`, subtracts one, and dispatches through its branch table. With no offsets (`animation+0x1c=0`):

| Channel type | Factory branch | Singleton | Concrete handler |
| --- | --- | --- | --- |
| 2 | `0x611ce8 -> 0x611f54` | `0x610454` | Position X |
| 3 | `0x611d0c -> 0x611f04` | `0x610610` | Position Y |
| 4 | `0x611d30 -> 0x611f64` | `0x6107cc` | Position Z |
| 6, 7, 8, 9 | `0x611d78 -> 0x611f44` | `0x6100dc` | Shared quaternion/Euler handler, outside this adapter |
| 11 | `0x611dc0 -> 0x611f24` | `0x610b44` | Scale X |
| 12 | `0x611de4 -> 0x611f14` | `0x610d00` | Scale Y |
| 13 | `0x611e08 -> 0x611f7c` | `0x610ebc` | Scale Z |

The audit executes the factory and actual singleton initialization, then checks their real vtable size/contribution entries and presence of apply methods. Only `__cxa_guard_acquire`, `__cxa_guard_release`, and `__aeabi_atexit` are service fixtures. Seven actual singleton initializations execute. Type6..9 returns size function `0x60f07c` and contribution `0x61333c`; these differ from every scale component handler. The preliminary assumption that ScaleX/Y/Z meant numeric7/8/9 was incorrect and was corrected before the final source/corpus/report handoff.

## Full vector values and setters

| Type | getValueSize | getBlendedValue | applyValue | applyBlendedValue wrapper |
| --- | --- | --- | --- | --- |
| 2 | `0x60efe0` | `0x626df8` | `0x623d90` | `0x629b14` |
| 3 | `0x60efc8` | `0x6270a4` | `0x623d48` | `0x62949c` |
| 4 | `0x60efb0` | `0x627350` | `0x622cb8` | `0x628e24` |
| 11 | `0x60ef4c` | `0x6278a8` | `0x6238f4` | `0x62d0b4` |
| 12 | `0x60ef34` | `0x62a034` | `0x622c40` | `0x62ca3c` |
| 13 | `0x60ef1c` | `0x62a2e0` | `0x622bf8` | `0x62c3c4` |

Every size is 12 bytes. Contributions start with +0XYZ for count0; count1 copies all 12 bytes and ignores weight; larger counts sum each XYZ component in slot order using separate f32 multiply/add, including zero weights. The port reuses the already recovered `dh2_animation_blend_vector3` kernel.

Direct apply and blended apply pass the entire XYZ vector to a single node virtual setter. Position uses vtable byte offset `+0xa4`, scale uses `+0x94`. **There is no virtual getter and no live other-axis merge.** Thus two consecutive concrete PositionX/PositionY calls both overwrite XYZ; the second complete vector wins. Other-axis values must come from the actual upstream slot/default/accessor producer. This applicator cannot manufacture them from the current node.

The audit installs the actual node setters in those two virtual slots and makes every other node virtual slot fail immediately:

- `0x59712c`: position at node `+0xac/+0xb0/+0xb4`; OR dirty bit8 at `+0x11c`.
- `0x5970c4`: scale at node `+0xc8/+0xcc/+0xd0`; OR dirty bit2 at `+0x11c`.

The original setters interleave loads/stores in the order X, Y, dirty, Z, and preserve all unrelated dirty bits. The native API proves the resulting local values/flags under a disjoint, single-threaded caller contract; it does not expose the historical ARM32 object layout or promise observable individual store interleaving.

## Native boundary

`component_applicator.hpp/.cpp` exports three C helpers: `dh2_animation_component_blend`, `dh2_animation_component_apply`, and `dh2_animation_component_apply_blended`. `ComponentNodeState` is a native 28-byte value snapshot containing position, scale, and owner dirty flags. Supported numeric tags are only2/3/4 and11/12/13. All IEEE words are accepted. Counts are bounded0..65536, output/state must be aligned and disjoint from input spans, count1 permits null weights, and malformed calls reject before mutation. Types6..10 reject rather than dispatch through the wrong concrete handler.

The C++ `apply_component` and `apply_blended_component` bridges operate on genuine borrowed `scene::Node` local translation/scale. Because the graph has no original dirty member, the owner explicitly supplies dirty storage. The bridges preserve node identity, unrelated local properties and existing world matrices. The owner requests `scene::update_world` separately. The host functional fixture performs two ordered component writes and an actual parent/child world update; it verifies the second complete vector wins and parent scale/translation reaches the final world translation.

Caller dispatch must use the actual compiled union handler selected by the original factory/compiler, rather than the latest incoming channel type. Compatibility, defaults, registration, raw accessors and component/Euler conversion are owned by the separate raw compiler recovery. These concrete helper tests establish none of those producers and do not justify treating Euler7/8/9 as scale.

## Evidence

`component-applicator-arm64-differential.json` executes original instructions and the optimized ARM64 shared library: 10 factory identity checks, six size checks, 1,872 cases, 624 contributions, 624 direct applies, 624 blended applies, 624 position setter calls, 624 scale setter calls, zero other virtual/getter calls, zero mismatches. The corpus includes ordered prior-state propagation, counts0/1/2/3/7/31 and random counts, negative/non-normalized weights, signed zero, subnormals, overflow, infinities and NaNs. Copied words compare exactly; 68 arithmetic NaN payload differences compare by classification, with no finite-word tolerance.

`component-applicator-host-audit.json` replays the original-derived binary corpus under ASan/UBSan: all1,872 kernel cases, 1,248 C++ node applications, 14 atomic malformed/type/alias rejections, one real graph world-update fixture, zero findings. Reports bind the original ELF, function manifest, gold corpus, executable/oracle and current relevant source hashes.

No complete original pose compiler, animation set, quaternion/Euler6..9 conversion, full gameplay frame, GPU output or packaged APK parity is claimed. The initial bounded kernel task changed no renderer, CMake, raw sampler, old report, asset, Android build or emulator state.

## Shared-library integration follow-up

The separately authorized CMake integration adds `component_applicator.cpp` to `dh2_engine_animation` and an actual DSO-linked `component_applicator_audit` target. `tests/component_applicator_dso.cpp` includes the gold replay test but links no production sources directly. Seven runtime `dladdr` checks require component C exports, both graph bridges and the weighted vector kernel to reside in `libdh2_engine_animation.so`, and `scene::update_world` in `libdh2_scene_materials.so`. Executable dynamic symbols independently show the kernel imports remain undefined until shared-library resolution.

`component-applicator-dso-host-audit.json` binds both actual loaded DSOs and the executable by SHA256. Its reusable runner `tests/component_applicator_dso_host.py` obtains source/header inputs from the actual Ninja compile commands and object dependency records, asserts their hashes stay unchanged across the CMake build and replay, and binds the CMake cache and command/output hashes. The actual sanitized DSO replay passes the same1,872 cases, 1,248 graph applications, 14 guards and world-update fixture with zero ASan/UBSan findings. It also records the then-current raw sampler and angle interpreter sources compiled into the DSO; this corpus does not claim to validate those additional exports.

During the authorized CMake handoff, the parent's angle interpreter dependency was added to the production DSO and both direct compiled-transform audit targets. The parent's new direct `component_transforms_audit` target was added with explicit sinf/cosf/acosf/sqrtf wrapping. These target definitions were coordinated with their respective owners; no raw sampler, angle helper, actor or renderer source was edited here.
