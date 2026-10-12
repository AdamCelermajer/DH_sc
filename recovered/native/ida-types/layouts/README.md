# IDA local type layouts (libDungeonHunter2.so)

Export of the local type library of `libDungeonHunter2.so.i64` (the database under `lib/armeabi-v7a/ida/`), generated headlessly with IDA 9.3 `idat.exe` and `export_ida_types.py`.

## Files

- `types.json`: every numbered local type (ordinal, name, kind, size, and for structs/unions the member offset, size, type, base-class, vftable and gap flags; enum values for enums).
- `types.h`: forward declarations plus the definitions IDA prints for each type. Some definitions are stubs because the source type is only a forward declaration.
- `export_ida_types.py`: the IDAPython script used for the export.
- `forward-decl-diagnostic.txt`: the diagnostic run that showed why most classes have no members.

## Counts

- Local types exported: 2467 (0 errors).
- Kinds: struct 470, union 62, typedef 115, enum 16, unresolved/forward-only 1804.
- Types with member layouts: 526.
- `glitch::` names: 437. Of these, 0 have member layouts.
- `glitch::scene::` names: 107. Of these, 0 have member layouts.

## Caveats

- The database has only forward declarations for the glitch classes, including `glitch::scene::ISceneNode`, `glitch::video::IVideoDriver`, `glitch::video::CMaterial` and `glitch::io::IAttributes`. The earlier `libDungeonHunter2.so.h` export has the same stubs (for example `struct glitch::video::IVideoDriver;` with no body). So member offsets, sizes and vtable slots such as `IVideoDriver` owner at `+0xd4` and slot `+0x34` cannot be read from the type library. They need struct recovery from the decompiled code or another source.
- The database was built with IDA 9.3. IDA 9.4 is not installed on this PC. The ida-pro-mcp server on `127.0.0.1` refused connections, so the export used headless IDA 9.3 instead.
- The export ran on a scratch copy of the database; the original `.i64` was not modified.
