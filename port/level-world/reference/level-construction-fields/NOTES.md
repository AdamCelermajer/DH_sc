# Level constructor identity fields

This slice reconstructs only the three LevelList-selected fields and the raw
constructor difficulty projection. It does not construct the rest of a Level,
run GSLevel, or publish a current-Level object to the native game.

## Recovered source behavior

`GSLevel::Ctor` at ELF `0x386190` calls the complete `Level` C1 constructor at
`0x3f3128`. The C1 constructor initializes `Level+0x3c` and `+0x40` to `-1`,
stores its final signed difficulty argument at `+0x118`, and initializes the
random flag at `+0xe8` to zero before scanning `Arrays::LevelList`.

The LevelList scan is an ordinal walk over the source runtime's 72-byte
`LevelDeclaration` records. For each row it:

1. copies the LevelFile C string at row `+0x20` into a 1024-byte local buffer;
2. calls `ToLowerCase(buffer, 0, -1)`, which lowercases ASCII `A` through `Z`;
3. runs case-sensitive `strstr(Level+0x10c, lowered_LevelFile)`.

The first match stores the row ordinal at `Level+0x3c`, the source `IsRandom`
byte from row `+0x14` at `Level+0xe8`, and the signed Hub word from row `+0x10`
at `Level+0x40`. The incoming filename/haystack is not lowercased. A no-match
or empty-list scan retains `-1`, `-1`, and false. An empty row needle follows
the actual `strstr` rule and matches the beginning of any filename.

The C2 constructor variant at `0x3f34c0` contains the same scan at a relocated
span and is separately replayed by the runner. The production GSLevel path
reaches C1. The runner starts immediately before the constructor scan with the
constructor's pre-scan field values prepared; it does not run unrelated Lua,
online, savegame, allocation, or whole-Level constructor work. It executes the
actual C1 and C2 scan instructions, the complete original `ToLowerCase` body,
and the source PLT calls for `strcpy`, `strlen`, and `strstr`. Those three libc
functions are bounded host dependencies in the oracle.

## Port boundary

`level_construction_fields::initialize` consumes the root-owned normalized
`data::LevelTables` rows and returns a named projection for `+0x3c`, `+0x40`,
`+0xe8`, and `+0x118`. It has no ARM memory overlay and is not wired to a live
GSLevel/Level owner. A caller must retain and keep the table and filename
stable through the call.

The source uses unbounded `strcpy` into its 1024-byte buffer. The port rejects
any LevelFile that would overflow that buffer, and rejects embedded NULs,
before writing output. These are port-only safety guards; they do not claim
source-equivalent handling of malformed inputs. The native cache parser
already validates its strings and the current 51-row LevelList fits the bound.

## Cache result and tests

The original cache has 51 LevelList entries. `GOTHICUS_CRYPT_01` is ordinal 23
with LevelFile `007_crypt_01.rule.xml`; it selects ordinal 23, Hub 2, and
IsRandom true. The file `003_darkwood.mlx` appears at ordinals 4, 5, 6, and 35;
the source constructor therefore selects ordinal 4 whenever that filename is
found in the incoming path.

Run:

```powershell
python port/level-world/tests/run_level_construction_fields.py
```

Latest report: `port/level-world/build/level-construction-fields/validation.json`.
It passes 12 host assertions and 7 input comparisons against both original
constructor scan spans (14 source-span comparisons total), including the
actual Crypt cache row, a duplicate LevelFile, substring selection, mixed
case, case-sensitive haystack behavior, empty table, empty needle, raw
difficulty preservation, and fail-closed malformed-input guards.
