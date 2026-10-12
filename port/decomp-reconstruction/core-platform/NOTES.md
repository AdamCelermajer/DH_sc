# core-platform notes

## Status

224 ledger rows (103 reconstructed, 49 partial, 72 skipped). 455 queue rows remain.
Host and Windows (MinGW under Wine) builds pass with 8 of 8 tests.

Not yet started: Console (43 functions), slim XML parse/write (23), ADevice (17,
JNI-bound), TouchScreenIPhone/PS3 (32), PerfCounters (9), Array2d (4), GCSingleton (3),
and most of `<global>` (the Java/JNI helpers, gameswf and Box2D code, FUN_* stubs).
TouchScreenBase and TouchScreenWin32 are finished in this pass.

## Layouts inferred

- `TimerUtil`: `unsigned remaining_` at +0x00, `bool paused_` at +0x04. The
  constructor sets paused, `Start` clears the flag and stores the duration,
  `Update` subtracts `Application::GetDt()` and clamps at zero.
- `FsFileHandle` (gameswf file callbacks): `size_` at +0x4, `pos_` at +0x8.
  Slot +0x0 is never read by these callbacks. Partial until a caller confirms it.
- `Device::Resolution`: width 960 at +0x0, height 640 at +0x4 (from
  `GetScreenResolution`).
- `ISerializable`: polymorphic base, Serialize/Deserialize are empty.

## Decisions

- Allocators keep malloc/free. Out-of-memory with assert level 2 calls
  `abort()` where the original writes to address 0. `CMutex` for the memory
  mutex is `std::mutex`.
- Tagged global `operator new/delete` overloads (glitch hint, swf MemHint, and
  MemoryHintState) are defined in `src/memory.cpp` and declared in `memory.hpp`.
  They are extra overloads, not replacements of the default operators.
- `sys::printNoFormat` passes the text as `"%s"`, not as a printf format string
  (the original's `printf(text)` misbehaves on '%'). Marked partial.
- Stand-in globals and functions (`ApplicationGetDt`, `DeviceFeatureFlags`,
  `SHARP_DEVICES`, `HTC_DEVICES`, `Is_In_Multiplayer_Mode`) are declared in
  `deps/application.hpp`; each test provides its own definition. Owned by the
  engine and game categories.

## Open questions

- `ConfigFile::XOR` and `ConfigFile::Decode`: the XOR body is a tail call the
  decompiler could not follow; Decode needs the key and obfuscation tables
  (`INTERNAL_KEY_INDEX`, `PARAMS`). Both skipped.
- `FileSystemBase::decodeObfuscatedData`: the decompiled signature shows three
  arguments, but the mangled name has four. Partial.
- `strrstr`: the original tail-calls an unrecovered routine for non-empty
  needles. Implemented as last-occurrence search, unverified. Partial.
- `StrToObj(vector<Point3D<float>>&)`: the original only logs a debug line;
  replaced with stdout. Partial.
- `Color255To01`, `DBG_GetLoadingStepName`, `GetPreferredLanguages`: not
  recovered (tail call to `__aeabi_fdiv`, a string table, a locale table).
- `tolower` is the Android libc function; not rebuilt.
- `threadfun2/3` wrap Savegame calls from another category; skipped.
- `IStreamBase`, `CFileSystem` and the Savegame lookups are not declared
  locally. `FileSystemBase` existence, path and savefile checks, and the
  constructors/destructors that chain into the glitch base are skipped.

## FileSystemWin32 (added in the second pass)

- Member layout: root `+0x2c`, copy of RES_PATH `+0x130`, resources `+0x234`,
  data `+0x338`, savefiles `+0x43c`. Each buffer is 0x104 bytes (the original's
  paths are `char[260]`). The earlier header assumed 0x208 and was corrected.
- Files are opened with stdio, not glitch::io::CFileSystem. Open modes are
  inferred: read-only is "rb", a write handle is "wb", and readwrite is "r+b".
- `_FileHandle` read/peek decode the first four bytes when the obfuscated flag
  (`+0x24` of the original file object) is set, using FileSystemBase's decoder.
- `RES_PATH` is a stand-in global (`"./"`) defined in filesystem_win32.cpp.
  The platform layer should own it at integration time.
- Not reconstructed: the wide-char `makeFullPath` overload, the uncompiled-data
  path in `openFile`, the trace-switch logging in delete/backup, and the Savegame
  flush in `openSavefile`. `getFiles` uses `<filesystem>` instead of the Win32
  finder.

## Still open (core-platform)

- slim XML parse/write (`parse`, `parseLabel`, `writeNode`, `findChild` and friends).
- PerfCounters (9), Array2d<rnd::Tile*> (4), ADevice (17, JNI-bound), Console (43).
- Remaining FileSystemBase members (`doesFileExists` and related, `createAndOpenFile`).

## TouchScreenBase layout (inferred from the decompiled handlers)

- Slot records are 0x30 bytes, at index i: `+0x00` mark (written as 0/1/2 by the
  handlers of slot i-1; the original writes one record past the touched slot),
  `+0x08` begin point, `+0x0c` current point, `+0x10` previous point, `+0x18`
  double time of the last sample, `+0x20` valid, `+0x24` swipe, `+0x28` touched,
  `+0x2c` id. Nine records are kept because the handlers write slot i+1.
- `+0x188` double `time_now_` (set by `update`), `+0x190` active count,
  `+0x194` the queue vector (16 entries of 12 bytes: type, x, y, id), `+0x1a0`
  head, `+0x1a4` tail, `+0x1a8` orientation (2 or 3 mirrors X), `+0x1ac`/`+0x1ae`
  width and height, `+0x1b0` float scale (1.0).
- Queue type values: 0 began, 1 moved, 2 ended (cancelled also uses 2).
- Vtable (from `decomp_vtable.py`): +0x8..+0x14 are the four bounds (pure in the
  base, supplied by TouchScreenWin32), +0x18/+0x1c the getDimension overloads,
  +0x20..+0x2c the touch handlers, +0x50 hasTouch.
- `Rct<short>` and `glitch::core::rect<float>` are stand-ins in deps/. The
  original `Rct` begins with a vtable pointer that the stand-in omits.

## TouchScreenWin32 notes

- Base class is now `TouchScreenBase`. The `+0x1b4` secondary interface vtable
  (an event receiver) is not recovered; it is a placeholder member.
- `onEvent` maps mouse event codes 0/3/6 to began/ended/moved. The original
  calls `touchBegan` with two arguments; the id is passed as 0 here.

## Touch decisions and open items

- `isRegionPressed` (0x33aebc) is not implemented. Its pseudocode mixes the rect
  argument with constants and returns a float from a tail call, so it needs the
  ARM listing before it can be rebuilt.
- `getTouchPoint` and `getFirstTouchPoint` return the current point and keep
  partial status. Their null-path check has not been checked against the listing.
- `touchEnded` (0x33aeb8) is a 4-byte ELF symbol but its decompiled body is
  complete and identical to `touchCancelled`. Marked partial.
- Tests live in `tests/touch_screen_test.cpp` (queue order, coalescing, swipe,
  displacement, region queries, timeout release, id list).
