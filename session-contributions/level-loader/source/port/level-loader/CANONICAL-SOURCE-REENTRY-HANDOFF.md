# Direct canonical source-file callback guards

The original retained source traversal must stay sequential within one file occurrence. A parser, constructor or source-release continuation can call back into loader code. CanonicalCachedFileV1 previously accepted a recursive step: a declared parser continuation could advance the original child walk before its outer call finished. The focused host reproduction exited 1 with `recursive direct file delivery was accepted` before this fix.

CanonicalCachedFileV1 now rejects and latches recursive delivery, retains the first diagnostic and reached source/factory prefix, and catches standard delivery exceptions into the same failure path. Scope guards clear the active dispatch marker even on exceptions. Owner/source discard is rejected during delivery or recursive discard, before callbacks can release active walk storage or execute cleanup twice. Successful owner cleanup still precedes XML release; failed cleanup continues to retain sources and supports explicit retry of its unfinished tail.

These are new native adapter access guards. They do not assert that the original ARM implementation had recursive-call protection, change the original XML traversal or factory order, or implement thread synchronization. Access remains sequential.

## Layer and integration

Layer after canonical-module-files-handoff-a0bc82ccd24918ca.zip, SHA256 b58e6746bcdd8e9099907a572a8814ac4e747b22358bf7ce96f0cc270ae67761, and its documented canonical source/SAME-Level prerequisites. Adopt canonical_cached_file_v1.hpp/.cpp together and rebuild the loader, Module relay, and any consumer embedding this file receiver. The bundled full CMake and standalone probes are for reproduction; update production target wiring surgically and retain the actual existing engine owner targets.

The guards apply to direct MLX source loading as well as MGP/MVP files routed through CanonicalModuleFilesV1. Nested loading of a different occurrence remains valid. The Module relay retains its independent guard for module-wide field/URI consistency and global candidate cleanup ordering.

## Verification

Four new direct-file checks cover recursive parser delivery and no replay; a standard parser exception and retained prefix until actual cleanup; attempted cleanup during active delivery; and recursive owner/source cleanup after the unfiltered original SWAMP definition fails at its first LevelConfig construction gate. Positive traversal uses the declared Player-only source fixture solely to verify original Player exclusion and source lifetime gates.

The existing 12 canonical source-composition, 14 canonical source-adapter and 14 Module relay checks are rebuilt and executed with this fix. The latter still reaches OpenableContainer in the original first SWAMP gameplay file and fails honestly where this probe supplies no construction services. No unsupported actor is skipped to obtain a success result.

Run tools/build_canonical_source_reentry.py for host, sanitizers, x86_64 and arm64-v8a, then tools/run_canonical_source_reentry_checks.py using the bundled Python runtime. Host and ASAN/UBSAN/leak execution, Android x86_64 execution on only emulator-5590/DH2_Loader_API37, and ARM64 compile results are recorded in reports/canonical-source-reentry-checks.json. Native test files use /data/local/tmp/dh2-loader-source-reentry-v1; no APK installation or other emulator mutation occurs.

Full Module InitPost, actual LevelConfig/class construction, spawning, complete scene publication, campaign/save restoration, and visibly populated SWAMP remain pending. This patch resolves a reproduced loader failure; it does not satisfy the full generic-loader acceptance goal.
