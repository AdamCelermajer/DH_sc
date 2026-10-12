# pydata-accessors notes

## Done
- GetMemberIDByString family (135 functions): `src/member_ids.cpp`, `include/pydata-accessors/member_names.hpp`.
  - Arrays (68): loop over `m_memberNames[size]`, first match wins, else -1.
  - Structs (67): records `m_dataNames` with a per-class count. The 18 single-name and 8 two-name
    variants read fixed slots and are equivalent to a loop over `count` names (count 1 or 2).
    Loop variants read the name at record +0x14 with stride 0x18 and a literal count (3..224).
    The stride/offset is a layout detail of the original; the port stores names as a plain
    `const char* const*` table, so the record layout is not reproduced.
- Name tables are owned by the PyData loader (port/pydata-names); this category only reads them.
- Test: `tests/member_ids_test.cpp` (duplicates, empty and short tables, count limits).

## Remaining (about 2066 queued)
- `finalizeNames` (70), `readNames` (67), `skipNames` (34): need the IStreamBase/StreamReader
  interface and CustomFree/MemoryHintState equivalents. Not started.
- `GetNewScriptCmdDataInstance<T>` (80): allocates a ScriptCmd with a per-class vtable pointer.
  Needs a decision on vtable representation.
- `read`/`finalize` per class (about 774) and destructors (about 1035, including 3 variants each
  for virtual dtors with property/ClassFunc hierarchies). Not started.

## Second pass (name tables, ScriptCmd)
- deps: `include/pydata-accessors/deps/stream_reader.hpp` holds IStreamBase, StreamReader::readAs<T>
  (little-endian), readStringEx, MemoryHintState (malloc) and CustomFree (free).
- NameTable is now owning: `char** names` plus `int size`. Tests build tables from `char` arrays.
- finalizeNames/readNames/skipNames (Arrays only, 171 functions): all bodies are one shared shape.
  skipNames is the same body as readNames. Checked by normalising identifiers and whitespace
  against the ItemTypeList reference; all 171 match, so all are `reconstructed`.
  Layout: count (u32), must equal size, names array allocated, then per name a u32 length
  and that many bytes plus a NUL.
- GetNewScriptCmdDataInstance<T> (80): one class per command, deriving from `Structs::ScriptCmd`
  (virtual dtor replaces the vtable word). Stored words are modelled as zero-initialised
  `m_wN` members. Original allocation sizes (8..0x28 bytes) are not reproduced: only stored
  words exist, and 64-bit sizes differ. PlayEffect stores a link-time address (0x97ac48) in
  m_w3; it is left null and marked `partial`.
- Not done: Structs::X::finalize (343) and Structs::X::read (310) need per-class field layouts;
  Arrays::X::read (51); destructors (about 1035). Structs read bodies use fixed offsets
  such as `this+0x380` with readAs<int>.
