# core-platform notes

## Status

131 ledger rows (58 reconstructed, 16 partial, 57 skipped). 548 queue rows remain.
Host and Windows (MinGW under Wine) builds pass with 6 of 6 tests.

Not yet started: Console (43 functions), FileSystemWin32 (39), TouchScreen*,
Accelerometer*, PerfCounters, slim, ADevice, Array2d, GCSingleton, _JNIEnv and
most of `<global>` (the Java/JNI helpers, gameswf and Box2D code, FUN_* stubs).

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
