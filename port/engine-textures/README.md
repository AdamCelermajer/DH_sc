# Native texture source reconstruction

This module reconstructs DH2 HD 1.0.2 texture-header and PVRTC decoding behavior as portable C++17. It also reads the cache's ordinary uncompressed TGA images. Android compiles this source directly for ARM64 and x86_64. The APK contains no original game executable, ARM32 interpreter, translated engine, or Unicorn dependency.

This is reconstructed source, not the original Gameloft development source. It is a texture subsystem and a foundation for the native renderer, not a playable rebuilt game. The sibling [Android project](../android-native/README.md) demonstrates native decoding, RGBA uploads and aspect-preserving display.

## Evidence

| Check | Result | Report |
|---|---|---|
| Full owner-supplied cache texture corpus | 242 images decoded: 17 PVRTC 2bpp, 217 PVRTC 4bpp, eight ordinary TGA | [Cache audit](reports/cache-audit.json) |
| Original ARM32 header routines against compiled Android ARM64 source | 360 comparisons match, including all 234 real PVR headers, raw/wrapped headers, format mapping, mip/cube/volume flags and malformed inputs | [Differential report](reports/differential.json) |
| Original ARM32 PVRTC against compiled Android ARM64 source | 11 pixel cases match byte for byte; four complete real 64x64 images, a miniature using real 2bpp payload words, and deterministic synthetic fixtures | Same report |
| Bounds and orientation under host ASan/UBSan | TGA channel/origin conversion, output guards, misaligned PVRTC input, truncation, capacity checks and 25,000 random inputs pass | [Build/test record](reports/build-validation.json) |
| Native Android GLES upload and resolution checks | Seven cases pass in portrait and landscape on an x86_64 API 37 emulator | [Emulator report](../android-native/reports/emulator-smoke.json) |

The instruction oracle models the caller's virtual file, logging, imported libc/compiler helpers and Android TLS canary. It executes the actual original routines and the actual compiled ARM64 routines. Unknown executed imports fail. Unicorn is a development test dependency only. The report records both binary hashes; 980 original instruction addresses were observed, not complete coverage of every original error branch.

The real 1024x1024 2bpp images are decoded in full by the native corpus audit and one is uploaded in the emulator. Original-versus-rebuilt byte equivalence was checked on smaller 2bpp fixtures, not those complete large images. No physical ARM64 phone or 16 KiB-page device has been tested; native libraries have 16 KiB ELF load alignment.

## Reconstructed behavior

`textures.cpp` validates legacy 52-byte PVR v2 headers with the optional eight-byte `BTEXpvr\0` prefix and reproduces the observed texture-description values. `pvrtc.cpp` implements Morton block addressing, endpoint decoding, modulation, interpolation and RGBA output from the original ARM instructions. [Captured assembly](reference/original-functions.asm) and [symbol addresses and hashes](original-functions.json) identify the input engine version. The capture contains literal pools and the format jump table as well as instructions.

The legacy decoder differs from the newer PowerVR SDK decoder. In particular, its 2bpp mode uses the stored checkerboard modulation without the newer horizontal/vertical mode-bit rewrite. Its transparent B endpoint also modifies A's blue expansion while leaving B's blue unreplicated. The reconstructed source deliberately preserves these observable results; the modern SDK decoder is not used by the app.

The APIs use checked values and borrowed byte views rather than reproducing the ARM32 class layout. Keep the input bytes alive and unchanged until decoding finishes. Zero dimensions and dimensions above the configured limits are rejected; the original descriptor accepted the two tested zero-dimension cases. Those intentional safety differences are recorded separately from equivalence checks.

The display path supports non-mipped 2D PVRTC 2/4bpp with power-of-two dimensions, and true-color TGA type 2 with 24/32-bit pixels. It rejects RLE/colormapped TGA and unimplemented texture features. Cube, volume and mip metadata are described but their image decoding and GPU resource lifecycle are not implemented. Cache materials, UV transforms, scene hierarchy and game UI layout remain separate work.

## Build and reproduce

Host checks, using CMake 3.22+, a C++17 compiler and Python 3.11+:

```sh
cmake -S . -B build/host -DCMAKE_BUILD_TYPE=Debug
cmake --build build/host
ctest --test-dir build/host --output-on-failure
python tests/audit_cache.py --cache /path/to/original-cache.zip \
  --library build/host/libdh2_engine_textures.so --report reports/cache-audit.json
```

For ASan/UBSan, use a separate Debug build with `-fsanitize=address,undefined -fno-omit-frame-pointer` in C++ flags and `-fsanitize=address,undefined` in the shared/executable linker flags, then run CTest.

After building the Android project, run the original-instruction check with `pyelftools` and `unicorn` installed:

```sh
python tests/differential.py --engine /path/to/libDungeonHunter2.so \
  --library /path/to/unstripped/arm64-v8a/libdh2_engine_textures.so \
  --cache /path/to/original-cache.zip --report reports/differential.json
```

The original ELF must match the SHA-256 in `original-functions.json`. `tools/capture_reference.py` additionally requires Capstone and regenerates the assembly evidence from that exact ELF. The current test harness reuses `../engine-resources/tests/cpu.py` without modifying that module.
