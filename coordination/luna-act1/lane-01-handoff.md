# Lane 01 handoff — Startup data and tables

**Status:** coherent source delivery; root owns integrated build/runtime acceptance. No build, test, emulator, or gameplay verification was run here.

V124's first genuine startup failure was downstream of `FaeryListTable`: IDA `Arrays::FaeryListTable::read` `0x4bc12c` invokes `Structs::FaeryList::read` `0x4eab9c`, which reads a count-delimited int32 vector. The registration is now `[i]` (previously `ii`). Canonical `faeries_pyarray.bin` has four lists ending at `0x64`; `FaeryTable` begins there with count 16. Its `iiiiSii` shape matches `Structs::Faery::read` `0x50637c` and consumes through EOF `0x2bb`. This corrects V124's apparent `FaeryTable` string length `234881024` at `0x55` without padding or cap changes.

The coherent registration corrections now also encode:

- `AnimTable`: `i[biiibib[i]iiib]i` and `CharAnimTable`: both count-delimited integer arrays. IDA readers `Structs::AnimTpl` `0x4ef2f0`, `AnimStep` `0x4ec9f0`, and `CharAnim` `0x4ef64c`; bool reads are one byte (`0x429c1c`, `0x4db89c`).
- `FootstepEffectTable`: `iS[i][i]`, per `Structs::FootstepEffect::read` `0x506660`.
- `ItemBonusAttrMonopoly`: `[i]`, per `Structs::ItemBonusAttrList::read` `0x4ea954`.
- `LootTable`: `ii[iiiiiiii][iiiiiiii][i]`, per `Structs::Loot::read` `0x4fe794` and `LootEntry::read` `0x4fec08`.
- `ItemTypeList`: `[b]`, per `Structs::ItemTypeListList::read` `0x4dd2c4` (count plus signed-byte vector); the old `ib` consumed the wrong boundaries and made the following Loot vector count appear as 1792.
- `SkillListTable`: `[i]`, per `Structs::SkillList::read` `0x4ea830`; `SkillTable` remains `ib[i]ibiiSbiiSiii`, per `Structs::Skill::read` `0x4ebeb0`.
- `CharSoundsTable`: `[i][i][i][i]bb`, per `Structs::CharSounds::read` `0x4eade4`. `Listeners` is `iiiifi`, preserving its float32 field per `Structs::Listener::read` `0x4ed978`; the reader stores raw bits in a distinct float32 field kind.

Passive parsing directly from `C:/Users/adamc/Downloads/Dungeon-Hunter-2-HD-v1-0-2-cache.zip` consumed every registered data and names stream exactly to EOF (70 unique stream files; 71 data registrations and 71 names registrations, with no trailing bytes). This includes the 699-byte Faery stream, 284,104-byte loot stream, skills, and sounds. This validates wire consumption against the canonical cache only; it does not prove gameplay or runtime startup.

`source_process_arrays_v101.cpp` retains contextual `data/<uri>`, group, and byte-offset diagnostics. The 1,000,000 string/count guard is unchanged. No `source_process_pydata_files_v101.hpp` order change or shared wiring request is needed. Runtime model metadata was not available to this worker; dispatch was explicitly Luna/high.
