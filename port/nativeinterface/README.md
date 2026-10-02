# Reconstructed `libnativeinterface` source

This is new, buildable C source reconstructed from the exact supplied ARM32
`libnativeinterface.so`. It is **not the studio's original source**. This small
library implements Samsung Zirconia license helpers, not the game's rendering,
combat, AI, world simulation, audio, or networking engine. Building it for ARM64
does not make the whole game run on current Android.

Input SHA-256: `180b582cbb7e7004c94fe771f8c8013dad4c74a9317ee4baff08c542da4ff0da`.
The two ARM variants in the supplied APK should be inspected independently before
using this source as a replacement for any binary with a different hash.

The reconstruction exports all ten defined function symbols from the inspected
ELF. JNI method declarations were checked against recovered
`com/samsung/zirconia/NativeInterface.smali`. Five 1024-byte lookup tables were
recovered byte for byte from the supplied binary; their text is third-party
material and is not relicensed by this project.

| Function | Recovered behavior |
|---|---|
| `SHA1Reset`, `SHA1Input`, `SHA1Result`, `SHA1ProcessMessageBlock`, `SHA1PadMessage` | SHA-1 with the original 104-byte fixed-width context, incremental state, and overflow/corruption flags. `SHA1Result` returns 1 on success. |
| `doPassphraseTest` JNI | SHA-1 of the first identity truncated to 31 modified UTF-8 bytes and of the full second identity; sum each hexadecimal digest's nibble values; return the corresponding bounded text fragment from one of five tables. |
| `checkLicenseFile` JNI / `CheckLicenseFile` | Read the first 20 file bytes; compare them with SHA-1 of the selected fragment followed by both full identity strings. Missing and shorter files fail. Additional file bytes are ignored. |
| `storeLicenseKey` JNI | Write the first 20 caller-supplied key bytes and a 20-byte metadata digest. Return success if the file opened, matching the supplied binary's behavior even if a subsequent write fails. No license retrieval or entitlement service is supplied. |
| `checkLicenseFile2` JNI | The supplied ELF function contains exactly `movs r0, #1; bx lr` and returns true without reading arguments. This behavior already exists in the supplied binary; this reconstruction does not establish whether it was vendor-authored or subsequently patched. The actual `checkLicenseFile` validator remains intact. |

## Deliberate differences from unsafe original behavior

The supplied `storeLicenseKey` calls `GetStringUTFChars` and then calls `wcslen`
on that byte pointer. ARM32 Android uses 32-bit `wchar_t`, so this scans groups of
four bytes, potentially reading beyond the JNI allocation. The resulting count
is then passed as a **byte count** to SHA-1. The reconstruction uses the defined,
zero-padded-buffer case: `ceil(strlen(metadata) / 4)` bytes are hashed. This was
matched against the original instructions with explicitly padded test buffers.
There is no promised equivalence to arbitrary memory contents beyond an original
JNI string allocation. The trailing metadata digest is not consulted by the
supplied `checkLicenseFile` implementation.

The reconstruction rejects null required arguments and key arrays shorter than
20 bytes and handles allocation failures. The supplied binary could dereference
null pointers or read beyond those arrays. Its process-global JNI string pointers
are replaced with per-thread pointers to prevent concurrent calls from using a
different caller's path or identity. The non-JNI `CheckLicenseFile` returns false
outside its wrapper rather than attempting `fopen(NULL)`. These changes are
explicit safety improvements, not recovered original behavior.

For portable host checks, this import opens the 20/40-byte license data in
binary mode (`rb`/`wb`); these modes are equivalent to the original Android
`r`/`w` modes but avoid Windows text-mode newline conversion. The semantic
test also creates its temporary file in the test working directory.

The SHA-1 algorithm and file comparison are preserved for compatibility; they
are not proposed as a new authentication design. No new entitlement bypass,
replacement license server, real license/key file, or licensing patch is included.

## Build and validate

Linux host prerequisites: GCC and a complete JDK (tested with JDK 17).

```bash
bash build.sh host
# Optional if the JDK is not on PATH:
DH2_JDK_DIR=/path/to/jdk bash build.sh host
```

This builds the core and JNI shared libraries and runs semantic C tests plus
desktop JVM JNI tests for all four recovered method signatures. Output is in
`build/host/` by default. Tests use synthetic identities and temporary files.

Android ARM64 build (tested with NDK r29, minimum API 26):

```bash
bash build.sh android /path/to/android-ndk-r29
```

The output is `build/arm64-v8a/libnativeinterface.so`, with 16 KiB load-segment
alignment and exactly the original ten function exports. To use the alternative
CMake entry point:

```bash
cmake -S . -B build/android \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/android-ndk-r29/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-26
cmake --build build/android
```

CMake configuration is supplied as a build entry point. The compiler-based
`build.sh` route is the route validated in the recovery environment.

On Windows, the CMake/Ninja host semantic test and NDK r29 ARM64 build were
also rerun from the imported source. The host test passed; the rebuilt ARM64
ELF is AArch64 with 16 KiB `PT_LOAD` alignment. These checks did not load this
library inside the game on Android.

Differential and static ELF checks require Python with `unicorn` and `pyelftools`:

```bash
python tests/differential_arm32.py /path/to/original/libnativeinterface.so \
  build/host/libnativeinterface-core.so --report build/differential-results.json
python tests/check_arm64_elf.py /path/to/original/libnativeinterface.so \
  build/arm64-v8a/libnativeinterface.so --report build/arm64-results.json
```

Differential validation executes the original ARM32 instructions in Unicorn and
compares their outputs with reconstructed C. Passed cases: 15 SHA-1 inputs,
100 passphrase pairs, 72 license-file cases, 72 JNI license checks, 4 JNI fragment
returns, and 8 original JNI stores with padded metadata. The emulator models libc
and JNI dispatch; it is not Android device execution. Desktop JVM smoke tests
separately exercise real JNI marshaling. No device or game-engine integration
test has been performed for this library.

To reproduce the table header from the hash-matched supplied binary:

```bash
python tools/extract_tables.py /path/to/original/libnativeinterface.so passphrase_tables.h
```

Use this source only with authorization appropriate to the supplied software.
Recovered game resources and third-party lookup text retain their existing
rights; this folder does not assert a license over the original software.
