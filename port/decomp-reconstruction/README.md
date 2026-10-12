# Decompiled-function reconstruction

Readable, compilable C++ rebuilt function by function from the Ghidra
pseudocode of `libDungeonHunter2.so` (`recovered/native/decompiled/`). The
target is code that builds and runs on Windows (MinGW-w64 or MSVC) and on the
host; Android builds come later through the existing app projects.

This tree is deliberately separate from the live-gameplay reconstruction
(`port/level-world`, `port/game-data`, `port/engine-*`, `port/android-native`,
`port/desktop-native`). Classes that work owns are not queued here; see
`tools/decomp_categories.py` for the exact rule.

## Layout

```
<category>/
  queue.csv            functions assigned to this category (generated, do not edit)
  ledger.csv           one row per finished function (elf_address,symbol,status,source_file,notes)
  NOTES.md             class layouts, open questions, upstream-library findings
  include/<category>/  public headers; deps/ holds minimal stand-ins for other categories' types
  src/                 reconstructed .cpp/.c
  tests/               one executable per file; return non-zero on failure
```

Ledger `status` values:

- `reconstructed`: the body is rebuilt from the pseudocode, compiles, and matches its control flow and side effects.
- `upstream`: the function is unmodified code from a known open-source library; `notes` names the library, version and upstream function.
- `partial`: compiles, but part of the behaviour is stubbed or uncertain; `notes` says which part.
- `skipped`: deliberately not rebuilt (`notes` says why, such as a compiler-generated thunk or a platform that is not a target).

## Tools

```sh
python3 tools/decomp_extract.py --category core-platform --remaining
python3 tools/decomp_extract.py --category core-platform --owner Console      # one class, unfinished only
python3 tools/decomp_extract.py --category core-platform --next 20 --max-bytes 600
python3 tools/decomp_extract.py --address 0x30eea0
python3 tools/decomp_extract.py --category glitch-core-io --match "glitch::io::CXMLReader" --list   # address, size, name
python3 tools/decomp_vtable.py Console           # vtable byte offset -> method, for (*obj + 0x98)(...) calls
tools/decomp_build.sh host core-platform         # fast build + tests
tools/decomp_build.sh windows core-platform      # MinGW-w64 build, tests run under Wine
python3 tools/decomp_progress.py                 # ledger totals per category
```

Original strings and symbols are in `recovered/native/symbols/libDungeonHunter2.so/`
(`strings.csv`, `symbols.csv`, `vtables-001.json`). Upstream Irrlicht 1.8.5, the
ancestor of `glitch::`, is vendored at `port/irrlicht-prototype/upstream/irrlicht-1.8.5/`
for naming reference.

## Rules for reconstructed code

- Write idiomatic C++17: real classes and structs with named members, real
  pointer types, no `undefined4`/`param_1` names. Keep the original 32-bit
  offset of each member as a comment (`// +0x40`) so layouts stay checkable,
  but never do pointer arithmetic on raw offsets; the code must work on 64-bit.
- Each function starts with a comment giving its original address and mangled
  symbol, e.g. `// 0x30ef50 _Z8ObjToStrPcRN6glitch4core8vector3dIfEE`.
- Keep original names (classes, methods, globals) from the demangled symbols.
- Platform code: prefer the original's Win32 variants (`FileSystemWin32`,
  `TouchScreenWin32`, ...). Where only an Android/iOS variant exists, write
  portable C++ or `#ifdef _WIN32` with Winsock/Win32.
- Do not paste pseudocode. Do not invent behaviour that is not in it; mark
  `partial` instead.
