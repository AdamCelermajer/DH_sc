# Reconstructed engine math checkpoint

This directory is buildable C++ reconstructed directly from the supplied `libDungeonHunter2.so` ARM instructions. It covers **20 original physical function starts**: the 19 named bodies in the `vector3d<float>` and `quaternion` groups, plus their matrix-to-Euler dependency. Other mathematical classes and inlined operations remain outside this checkpoint. This library is one engine component, not the complete original engine or a playable game.

`math.hpp` supplies explicit port types and C-linkage entry points. These are independently reconstructed declarations, not original studio headers. `original-functions.json` maps each entry point to its original mangled symbol, ELF address, size and machine-code hash. `reference/original-functions.asm` preserves all 6,816 original instruction bytes as annotated listings.

## Recovered behavior

| Area | Included behavior |
| --- | --- |
| Vector normalization | Sequential float rounding; zero-length vectors remain unchanged |
| Vector division | Component division and in-place division; signed zero/infinity/NaN behavior retained |
| Vector rotation | XY, YZ and XZ rotation around a center; input angle is degrees, trig uses double then converts to float |
| Direction angles | Original yaw/pitch calculation, degree wrapping and intermediate float/double conversions |
| Quaternion construction | Euler radians, angle/axis radians, and the compiler-specialized clone's exact literal constants |
| Quaternion normalization | Skip when squared length equals exactly one; original zero-quaternion NaNs retained |
| Quaternion products | Original reversed Hamilton-product operand order and vector transformation signs |
| Matrix output | Both orientations, identity-hint byte, and the value-returning overload's transposed output |
| Matrix/Euler conversion | All matrix-to-quaternion branches, normalization, Euler conversions and original branch thresholds |
| Interpolation | Original sign adjustment, linear/SLERP/orthogonal branches, 0.05 threshold and branch-specific normalization |

The observed vector/quaternion layouts are three/four adjacent floats. The matrix stores sixteen floats followed by a byte at offset 64, inferred as an identity hint from its initialization/use. The port fixes these layouts with static assertions and leaves the matrix's three padding bytes untouched.

Do not substitute a generic math library without accounting for these conventions. In particular, `dh2_quat_matrix(q, out)` and `dh2_quat_matrix_value(out, q)` intentionally produce transposed results relative to each other, following the two original overloads.

## Build and check

The Python build script was tested with the available host C/C++ compiler and Android NDK r29 on Linux. It produces a host library, a test dependency library, and, when an NDK is supplied, an ARM64 Android library with 16 KiB load-segment alignment. The ARM64 component imports only libc/libm/libdl; it does not require `libc++_shared.so`.

From the reconstruction workspace root:

```sh
python -m pip install -r port/engine-math/requirements.txt
python port/engine-math/build.py --ndk /absolute/path/to/android-ndk-r29
python port/engine-math/tests/differential.py \
  --original /absolute/path/to/libDungeonHunter2.so
```

The original engine must have SHA-256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The harness refuses a different binary and verifies each selected function's original address and instruction hash before execution.

Example use after linking the component:

```cpp
#include "math.hpp"
dh2::math::Quaternion rotation;
dh2_quat_from_euler(&rotation, 0.0f, 0.5f, 0.0f);
dh2::math::Matrix4f matrix{};
dh2_quat_matrix(&rotation, &matrix);
```

Pointers must reference valid objects. Output objects must be separate from inputs except for the declared mutating operations. The API documents reconstruction interfaces; it does not promise compatibility with existing callers in the ARM32 engine binary.

## Recorded validation

The final run passed **21,477 original-ARM32 versus compiled-ARM64 comparisons**, with zero mismatches. It executed all **1,704 ARM instruction addresses** in the twenty original function ranges. Tests include randomized finite values, fixed axes, signed zero, normalization underflow/overflow, selected infinities/NaNs, matrix branches, interpolation thresholds, alias cases for mutating operations, and angles that round to exactly 360 during wrapping.

All non-NaN floats compare bit for bit; NaNs compare by classification because their sign/payload can vary across CPUs and runtimes. Tests also check output guards, unmodified inputs, matrix hint/padding, restored stack pointers, and pointer return values. ARM64 code and all data/stack pointers are placed above 4 GiB. Results and source/binary hashes are in `reports/engine-math-validation.json` at the workspace root.

The engine's actual original ARM32 instructions run in Unicorn. Imported `__aeabi` arithmetic and libm calls use a host C/libm dependency model; the compiled ARM64 routines use their native floating-point instructions and the same libm model for external trig/square-root calls. No vector/quaternion engine routine is mocked. This verifies the sampled engine algorithms under that dependency model; it does not verify the historical Android runtime's particular libm implementation. Full instruction-address coverage is not exhaustive input or path-combination coverage.

No Android device load, renderer integration, performance measurement or gameplay validation has been performed for this module. The rest of the native engine is still being reconstructed.

## Decompiler finding

Sixteen of the nineteen recovered vector/quaternion pseudocode bodies contain `WARNING: Subroutine does not return` at floating-point helper calls. For example, the normalization pseudocode stops at its first `__aeabi_fmul`, whereas the assembly continues through squared-length accumulation, branching and scaling. These exports remain useful indexes, but cannot serve as complete implementations. This checkpoint was translated from full assembly instead. Future decompilation should correct external helper return metadata and calling conventions, then rerun affected analysis before relying on inferred high-level control flow.

The materials retain the provenance and rights treatment documented in the workspace's `RIGHTS.md`; no original studio-source or game-asset license is inferred.
