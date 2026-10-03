# Serialized ItemTable

The native `items.hpp/.cpp` module decodes the actual 1,322 ItemTable rows in the supplied `loot_table_pyarray.bin`. Its allocation-free record decoder matched original ARM32 instructions against optimized ARM64 in 1,578 cases: every cache row plus 256 boundary records. The full-file native loader, owned strings, identifier lookup and combat-query bridge passed ASan/UBSan with 1,326 original identifier results, 1,322 range-capability results from the genuinely loaded original ItemTable, 168 native boundary checks and zero findings.

The original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. [original-functions.json](original-functions.json) binds 29 captured readers, lookup and query routines; [original-functions.asm](original-functions.asm) contains their instructions. [record-fixtures.bin](record-fixtures.bin), SHA256 `5aa7b428b2031dd88c1b533e47e06bada2b372c92211240bbd64d88a73119db9`, stores the exact source outputs.

## Actual file producer and section order

The supplied Android ELF's `PyDataArrays` constructor at `0x4be550` registers callbacks for the literal `loot_table_pyarray.bin` and `loot_table_pyarraynames.bin` strings. [registration-probe.json](registration-probe.json) records those arguments while the actual constructor runs; only its registration/map services are observers. This binds the selected file to the original producer rather than choosing a cache file by its name or schema alone. The constructor and reload dispatcher are captured under [registration](registration/original-functions.asm).

The first four data callbacks execute in this order:

| Reader | Original address | Exact cache start/end | Serialized leading contents |
| --- | --- | --- | --- |
| DropTilePriorityTable | `0x4ba4fc` | 0 / 44 | Table count, then per-row count and four-byte priority entries |
| InventoryTable | `0x4ba3c8` | 44 / 54 | Table count, then two-byte inventory values |
| ItemList | `0x4ba27c` | 54 / 6486 | Table count, then per-row entry count; entries are signed item ID32, probability16, quantity8 |
| ItemTable | `0x4ba12c` | 6486 / 229006 | Table count, then 1,322 Item records |

The matching name readers are `0x4b53f4`, `0x4b5258`, `0x4b4a54` and `0x4b4724`; their original name-stream offsets are 0/27, 27/66, 66/3737 and 3737/30502. Each name section has a four-byte count followed by length-prefixed byte strings. Schema block0 contains the 15 ItemBase fields and block1 contains the 38 Item fields, including inherited fields. Additional subclass schemas and later loot tables remain in the original files and are not interpreted by this module.

Native traversal of the preceding three data/name sections serves to locate ItemTable. It bounds-checks their sizes and advances through their exact serialized layout; it does not expose or reconstruct their owning runtime tables.

## Item record layout

`ItemTable::read` allocates rows with an eight-byte array header (element size164 and count), installs each row's vtable and initializes the two string pointers before virtual read+`0x0c`. `Structs::Item::read` at `0x4fc068` calls `ItemBase::read` at `0x4ec47c`. All fields are consumed from the stream, rather than synthesized from defaults.

| Original byte offset | Meaning and serialized producer |
| --- | --- |
| `0x00` | Original vtable pointer; not serialized |
| `0x04`, `0x08` | IconName byte length, allocated pointer; unsigned length32 followed by exact bytes and an added runtime NUL |
| `0x0c`..`0x18` | PickUpType, DistType, SwooshSoundFX, SwooshFX; signed words |
| `0x1c` | Stackable; one serialized byte, followed by three runtime padding bytes |
| `0x20`..`0x40` | Nine class EquipValue float words; read as raw IEEE32 bytes |
| `0x44`, `0x48` | Material and ModularModule; signed words |
| `0x4c`, `0x50` | Name byte length and allocated pointer; unsigned length32 followed by exact bytes and runtime NUL |
| `0x54`, `0x58` | AudioVisualID and Type; signed words |
| `0x5c`..`0xa0` | BaseStat, BaseProp, BaseElement, EquipmentSlottingType, Value, GoldValueMultiplier, requirements and Param1..6; signed words |

The serialized field order reads Material/ModularModule **before** Name, although the schema lists Name before those fields. Treating schema order as serialization order would shift the actual record. There are no inline vectors in Item itself; the relevant vector readers belong to the leading sections described above.

The native `ItemRecord164` is a scalar projection with the same word positions. Original vtable/string pointer words0/2/20 are zero. String lengths remain at words1/19, and Stackable is preserved as its serialized byte with zero upper padding. No canonical boolean conversion occurs: byte255 stays255. `Item` owns both byte strings separately. Embedded NULs are preserved, and every float bit, including nonfinite values, is retained without arithmetic or conversion. The projection is not an ARM32 runtime object overlay.

The source storage allocator is an explicit zeroed caller fixture. Thus original pointer values and uninitialized boolean padding are normalized out of comparisons. Scalar fields, string lengths/content, stream consumption and actual lookup/query results are compared; allocator implementation and runtime address identity are not claimed.

## Typed lookup and actual query bridge

`GetMemberIDByString<ItemTable>` at `0x4ad5f8` visits names in order, calls `strcmp`, returns the first equal index and returns `-1` if absent. Native `item_id` uses the same NUL-terminated comparison and first-match behavior, including a test with an embedded NUL suffix. `item(table,id)` supplies a checked native row pointer; `item_type` reads word22, the original Type offset+`0x58`. `item_equip_value` exposes any of the nine float words using a bit-preserving copy.

The source range corpus calls `Character::CanRangeAttack` at `0x3a4d3c` with cached RangeProjectileID=-1 and real equipment references. Its `ItemInventory::HasRangedWeapon` and `ItemInstance::GetItem` instructions traverse the **genuinely loaded** ItemTable and test types4/5 for all 1,322 item IDs. The host test copies the decoded scalar words into the existing native query's borrowed `CombatItemRecord164` rows and compares those source results. It avoids casting between separately owned C++ types. No supplied capability boolean or invented equipment item participates in this proof.

This supersedes the earlier combat query's synthetic resolved item-row input with a genuine decoded-table bridge. Equipment/inventory construction, reference ownership, gear mutation and property-resolution timing remain separate caller responsibilities.

## Native malformed-input boundary

The allocation-free `dh2_item_decode_record` commits its scalar output, two borrowed text spans and consumed count only after all fields validate. Outputs must be aligned and mutually disjoint and cannot overlap the input. Inputs are bounded to8MiB, strings to65,536 bytes and table/vector counts to65,536. The full loader also checks schema/record/name dimensions, preserves prior output on failure and owns its successful strings independently of input lifetime.

These bounds/atomic failure semantics are native boundary behavior. The original stream readers may assert or proceed with bad input; no original undefined-read parity is claimed. Empty payload strings, raw byte strings, negative field words and all float bit patterns are valid within the limits. The host checks every truncation of the first complete record, malformed headers/schema/name streams, alias rejection, retained output and ownership after table move/source-buffer disposal.

## Evidence and reproduction

Authorized cache SHA256: `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.

- `loot_table_pyarray.bin`: 284104 bytes, SHA256 `4dd85c8656d60c38a0c651e999fd18a1ed4d936f7cfdfcec52a1dfb4b568847f`.
- `loot_table_pyarraynames.bin`: 39604 bytes, SHA256 `8ea5bc26e47d3b9aba1e1ab0fd3e96c4a989bbc2a6a647bf7c2b20071e8782ba`.
- `loot_table_pystructnames.bin`: 8858 bytes, SHA256 `ed6721327d6779e47e4def679ddeb796cf5da7ba0348d10996172e64d58bd323`.

Exact cache inputs are extracted read-only under `.local-inputs/items-discovery`. `tools/build_items_oracle.ps1` builds the isolated optimized ARM64 oracle. `tests/items_differential.py` executes the original table/record/name/query instructions and generates the immutable ITM1 corpus. `tests/items_host.py --rebuild` snapshots its compiler inputs before/after, builds the isolated host audit and binds all three cache files byte-for-byte. Reports are `reports/items-arm64-differential.json` and `reports/items-host-audit.json`.

The CMake host test source `tests/items.cpp` takes `<record-fixtures.bin> <loot-table-directory>` and links the item decoder plus the native combat-query module. ARM64 instruction comparisons cover record decoding; native full-file and identifier/query composition are proven by the sanitized host replay. This module makes no claim of complete loot generation, item powers, inventory lifecycle, original application frame or full combat parity.
