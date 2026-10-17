# IDA assembly listing: libDungeonHunter2.so (ARM)

Full IDA disassembly listing of the game binary, from the user's upload
`/mnt/project-files/ida.zip` (member `libDungeonHunter2.so.asm`).

- Input SHA256 `36498EB8180FFB74759E6305E9596DB999F18583D460F3B8534ABCB6022F5E80`,
  matching the `libDungeonHunter2.so` inside the same zip (checked). The IDA header
  says ELF for ARM, GNU C++, armeabi-v7a.
- 3,374,997 lines, 31,020 function banners (`; =============== S U B R O U T I N E`).
- Split into `asm_chunk_000.asm` to `asm_chunk_018.asm` at function boundaries.
  Concatenating the chunks reproduces the original byte for byte. CRLF line endings
  are kept.
- Companion IDA files from the same run: `libDungeonHunter2.so.til` (type library)
  and `libDungeonHunter2.so.id2` (IDA id file). Binary, not text.

## Format

There is no address column. A function starts with a banner, then its prototype
comment, then a label in column 0. The label is `sub_<ADDR>` for unnamed functions
and the mangled symbol otherwise. Functions that IDA collapsed for library code appear
as `[0000000C BYTES: COLLAPSED FUNCTION name ...]` lines.

To find a function by address, use the helper, which maps the address to its label
(Hex-Rays name for `sub_` functions, Ghidra symbol for named ones):

```sh
python3 tools/decomp_asm.py --address 0x4f2750
python3 tools/decomp_asm.py --name _ZN7Structs19CharacterProperties4readEP11IStreamBase
```

Plain search also works, for example
`grep -n "^_ZN7Structs19CharacterProperties4readEP11IStreamBase" recovered/native/ida-asm/*.asm`.

## Not in this folder

- `libDungeonHunter2.so.i64` (two IDA databases, 158 MB and 388 MB): larger than
  GitHub's 100 MB file limit. They would need Git LFS. Only IDA can read them.
- `armeabi-v7a.rar` (95 MB) and the `.so` files for `libStormGLOFT` and
  `libnativeinterface`: not IDA output and not extracted here.
- `ida.zip` itself.
- `_ZrsRSiRN6glitch4core8vector3dIiEE.asm`: a single-function excerpt, already inside
  the full listing.
