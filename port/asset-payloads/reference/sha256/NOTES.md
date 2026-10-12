# Owned SHA256 byte utility

`sha256.hpp/.cpp` is a new self-contained port utility. Existing port sources and CMake were inspected: digest metadata existed, but no reusable SHA256 implementation or linked crypto dependency was found. No alternative library was installed or copied. This implementation is based on the SHA256 definitions/constants/padding/initial state/compression in [NIST FIPS180-4](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.180-4.pdf), sections4.2.2,5.1.1,5.3.3 and6.2.2. NIST also publishes the [SHA256 abc/two-block worked example](https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdf) and [additional SHA2 test data](https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf). This is not a NIST validation/certification claim or reconstructed original-game function.

## API

```cpp
#include "../asset-payloads/sha256.hpp"
dh2::assets::Sha256Digest actual{}; // owned std::array<uint8_t,32>
if (!dh2::assets::sha256(dact_bytes, dact_size, actual)) {
    // malformed span; actual remains unchanged
}
// ActorInitializationDigest is the same std::array type; pass actual to CAI1.
```

The caller supplies a readable byte span whose lifetime covers the call. The utility cannot establish allocation/page validity. `nullptr,0` hashes the valid empty message. Null with a nonzero count, wrapping uintptr address spans and messages exceeding the SHA256 64-bit bit-length representation reject before reading or modifying output. SHA256 standard word arithmetic uses unsigned32 wrapping; byte loads and digest serialization are explicitly big-endian and support unaligned input. All message bytes are consumed before the owned digest is committed, so input/output overlap is supported. No allocation, global mutable state, borrowed result or platform crypto API is used.

The genuine actual current DACT bytes produce SHA `be44bd6d160973bf70a0cab61399a14d8bea6a7eade41790594cf6b781a04cca` (24336 bytes). This value was computed by the native implementation on an independently read actual DACT, then compared with Python hashlib. The caller should hash the bytes it actually loaded, rather than copying a digest asserted by CAI1/PAB1 metadata. Digest verification establishes byte identity; it does not itself prove full scene/XML/game-manager semantics.

## Isolated proof

`reports/sha256-host-audit.json` binds final hpp/cpp/tests, executable, SHV1 fixtures and actual payload origins/hashes. The O2 g++ ASan/UBSan/leak audit passes:

- 1104 expected digest comparisons: four fixed standard vectors (empty, abc, 56-byte two-block text, million-a), four byte patterns over273 lengths, and eight real payloads.
- Lengths0..260 cover block/padding edges55/56/63/64/119/120/127/128. Additional lengths511/512/513,1023/1024/1025,4095/4096/4097,65535/65536/65537 cover larger boundaries.
- Real payloads: current DACT, new CAI1, current PAB1, three canonical MGP XML resources, exact Prince animation template and original CharacterTable pyarray bytes. Cache ZIP SHA is checked before extraction; fixture manifest records each source path/ZIP member and hash.
- 3312 unaligned, read-only input checks; four input/output-overlap checks; six malformed-span atomic rejections; explicit valid nullptr/empty check. Total13268 checks; zero sanitizer diagnostics.

Expected values are Python hashlib outputs, with the four standard digest values also checked independently against fixed vectors. SHV1 stores labeled full byte messages and digest bytes; it is a test format only.

NDK29 clang compiled the final source independently at O2 for `aarch64-linux-android26` and `x86_64-linux-android26`, with `-fPIC -Wall -Wextra -Werror`. Both outputs are checked as ELF64 objects with matching machine types183/62. This is **compile suitability**, not optimized Android instruction replay, packaged or device execution proof. No original engine parity is claimed.

## Integration and reproduction

Root can add only `port/asset-payloads/sha256.cpp` to the chosen production target; it needs no extra dependency. Existing CMake/source/renderer/Android/APK files were left unchanged. New scratch outputs are under `.local-inputs/sha256/`.

```powershell
python port/asset-payloads/tests/sha256_host.py `
  --cache C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip `
  --output NEW-report-path.json
```

The script defaults to NDK `29.0.14206865` under LOCALAPPDATA/Android/Sdk; `--ndk` can supply that exact installation path. WSL/SDK access may require the approved elevated tool context. Existing fixture files must match byte-for-byte and existing output reports are not overwritten. It does not invoke any central build, CMake, Gradle, APK packaging or ADB operation. The native host test takes `reference/sha256/hashlib-fixtures.bin` and the actual DACT path.
