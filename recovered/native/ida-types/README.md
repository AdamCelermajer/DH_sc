# IDA type export (ida 2.zip)

Exported from the IDA session on the user's PC, 2026-10-12. The large databases and listings are
not in the repo (GitHub rejects files over 100 MB). The full disassembly is already in
`recovered/native/ida-asm/` (`asm_chunk_*.asm`), and `ida-asm/` also holds the `.til` and `.id2` files.

## In this folder

- `libDungeonHunter2.so.h`: the IDA local-types C header (188 KB). It holds forward declarations for
  about 1,870 types and definitions for about 430 more. Those definitions are `std::` internals,
  libc and pthread structs, and SIMD types. **It has no glitch:: class bodies**, so it does not give the
  layouts of glitch::scene, glitch::gui or glitch::video classes.
- `_ZrsRSiRN6glitch4core8vector3dIiEE.asm`: the listing of one function, `operator>>(std::istream&,
  glitch::core::vector3d<int>&)`. It calls `std::istream::operator>>(int&)` for each component.

## Not in the repo

- `libDungeonHunter2.so.asm` (191 MB): duplicates the chunked listing in `ida-asm/`.
- `libDungeonHunter2.so.lst` (244 MB) and `libDungeonHunter2.so.i64` (388 MB): too big for GitHub.
- `libDungeonHunter2.so.map` (11 MB): symbol and segment map. Its names are already in
  `recovered/native/symbols/`.
- `libDungeonHunter2.so_20261012173718.i64` (388 MB): same as above.
- `libnativeinterface.so` (11 KB), `libStormGLOFT.so` (908 KB), and `libDungeonHunter2.so` (16 MB):
  the original binaries. The repo does not track them, and they were not pushed here.

## What the type library has

`libDungeonHunter2.so.til` (already in `ida-asm/`) names 105 `glitch::scene::` classes, including
`CBatchMesh`, `CColladaBinaryFileLoader`, `CMeshBuffer` and `CSkinnedMeshSceneNode`, and 452 glitch
names in all. Member layouts are stored in IDA's binary type-library format, which this repo cannot
decode. The C header does not carry them either.

## How to get the glitch layouts

The layouts need IDA on the user's PC. Cloud threads cannot run it, so use a Remote Control session:
in IDA 9.4 (or through ida-mcp), dump the local types that carry glitch:: names, with member offsets,
to a C header, and commit it here. Start with `glitch::scene::CBatchMesh`, `CColladaBinaryFileLoader`,
`IGUIElement` and `ISceneNode`, which the glitch-scene threads are blocked on.
