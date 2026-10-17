# DWARF in the reference binary: what it covers (verified 2026-10-16)

The reference `libDungeonHunter2.so` (15,938,284 bytes, SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`, the file under
`../lib/armeabi-v7a/ida/`) carries DWARF: `.debug_info` 0x142068, `.debug_line`, `.debug_loc`,
`.debug_str` and the rest. It has 82,983 `.symtab` and 32,572 `.dynsym` symbols.

The DWARF covers **32 compile units only**:
- 22 STLport units (`/tmp/ndk-toolchain/ndk-prebuilt-31319/sources/cxx-stl/stlport/...`),
- 7 units of the license-check and online layer (`P:/Template/Android/ALC/jni/...`: `ALicenseCheck.cpp`,
  `ConfigFile.cpp`, `LCXPlayer/src/Online/AndroidSocket.cpp`, `Common/GLXPlayerSocket.cpp`,
  `GLXPlayerSocketFactory.cpp`, `Porting.cpp`, ...),
- 3 NDK runtime units.

No game, glitch, scene, video or gameswf unit has DWARF. It does not resolve game or engine layout blockers.
Use it for the ALC/LCXPlayer classes and STLport layouts, citing `DW_AT_name` and `DW_AT_data_member_location`.

`reference-binaries/libDungeonHunter2-1.0.4-armeabi-v7a-dwarf.so` (SHA-256 `32d9452a…`, outside the
worktree, not committed) is a **different, later build** of the same source with the same 32-unit DWARF
(83,264 `.symtab` symbols, shifted non-uniformly). It adds no type coverage. Never put its addresses into
a ledger or a `// 0x` banner; every banner in this tree is keyed to the reference above. A note
(`V1.0.4-DEBUG-INFO-FINDING.md`, from another model) says this DWARF gives real types for
`CBatchMesh`, `CBatchDriver`, `SSharedStringData` and similar; that is not so.
