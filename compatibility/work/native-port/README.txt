DUNGEON HUNTER 2 — INDEPENDENT ARM64 PORTING PROTOTYPE
Target requested: Samsung Galaxy Z Fold7, One UI 8.5.
Work performed: 2026-10-01.

STATUS

This is a reproducible research checkpoint. It contains four manually translated
ARM64 functions and a native binary that exercises only those functions. It is
not an APK, a replacement game engine, or a verified playable port. Do not put
libdh2_port_prototype.so into the game as libDungeonHunter2.so.

The previous experimental APK still contains the original ARM32 engine. This
checkpoint does not make that APK run on an ARM64-only system.

WHAT CHANGED

Four small functions were translated directly from the user's original ARM32
library to ARM64 assembly, independently of ZettaBridge:

  luaO_log2          -> dh2_log2_u32
  luaO_int2fb        -> dh2_int2fb_u32
  luaO_fb2int        -> dh2_fb2int_u32
  IGUIElement::isPointInside -> dh2_ui_contains_point_legacy32

The first three preserve 32-bit integer behavior, including overflow. The GUI
function accepts true 64-bit pointers but reads coordinates at the original
32-bit-layout offsets. It does not reconstruct the C++ object's other fields,
embedded pointers, inheritance, virtual methods, or ownership rules.

The exact four original instruction sequences are in audit/original-routines.txt.
The 31,021 nonempty defined function symbols, representing 31,018 distinct
symbol values, are listed in audit/functions.csv. Aliases and wrappers mean this
is not a count of independent original source functions or a progress metric.
The engine has 32 DWARF compilation units, covering supporting license/network
code, STLport and compiler support. Its full game source/types were not recovered.
audit/engine-inventory.json records these units and native dependencies.

VALIDATION

45,330 comparisons passed with zero mismatches. Unicorn emulated the actual
original ARM32 instructions and the compiled ARM64 replacements on the host.
The checks include small integers, seeded random 32-bit values, shift/overflow
boundaries, signed rectangle edges, and ARM64 data addresses above 4 GiB.

This does not prove behavior for every possible input or integration context.
There was no Android installation/load test, graphics test, game launch, phone
test or performance benchmark. No faster-frame-rate claim is made.

build/libdh2_port_prototype.so is a freestanding AArch64 ELF shared object, with
no imported functions or DT_NEEDED dependencies. It was compiled without an
Android SDK/NDK. Its load segments use 16 KiB alignment and its RELRO extent ends
on a 16 KiB boundary. Static alignment is not proof of successful Android loading.

REPRODUCE

Requires a Python environment able to install the pinned requirements. The
original APK and its game assets are not downloaded by these scripts.

  python -m pip install -r requirements.txt
  python extract_input.py /path/to/Dungeon-Hunter-2-HD-v1-0-2.apk
  python build.py
  python verify_translations.py --original input/libDungeonHunter2.so
  python audit_native.py --original input/libDungeonHunter2.so

extract_input.py and verify_translations.py pin the original input hash. A
different APK or engine must be analyzed independently before porting it.
build.py uses Zig's bundled assembler/linker for a no-libc, no-framework probe.
Zig's aarch64-linux-musl target selects the freestanding ELF output; this library
does not use musl and is not an Android runtime implementation.

REMAINING WORK FOR A REAL FOLD7 BUILD

1. Reconstruct and port the reachable engine and its global data, preserving
   32-bit pointer/structure semantics or explicitly converting all related
   object layouts, vtables, callbacks and serialized state together.
2. Rebuild or adapt the old runtime/library interfaces and JNI boundaries for
   ARM64. Keep the Java and native signatures and ownership rules consistent.
3. Port the GLES and existing texture/shader patches; the supplied
   libStormGLOFT.so is ARM32 and performs address-sensitive runtime hooks.
4. Integrate the completed engine into a signed ARM64 APK, with storage import,
   permissions, lifecycle, audio and foldable display behavior verified.
5. Run actual launch, rendering, input, sound, save/reload and gameplay tests on
   the target device. The current CPU-emulation checks cannot replace them.

The required dependencies are not a normal Gradle/Maven list that can simply be
updated: much of the runtime and game is already linked into compiled code.
Manual translation is an available technique, but the remaining data and ABI
work determines whether a complete port behaves correctly.

CACHE

See CACHE-PLACEMENT.txt. The user can keep the archive on their device; no
upload or splitting is needed to continue native-code analysis. The cache's
exact internal directory layout and texture formats have not been inspected.
No new cache importer is included in this checkpoint.
