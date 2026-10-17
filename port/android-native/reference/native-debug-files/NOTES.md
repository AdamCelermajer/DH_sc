# Native Debug filesystem adapter

Backend owns the source Debug Runtime, loaded guard, application/engine/filesystem
projections, callback services and real retained file resources. Identity values
are actual stable owned addresses. Bind `globals()` and `services()` to the native
Character level Runtime. Keep Backend alive across scene/window recreation.
Its noncopyable ownership prevents address/identity changes during source calls.

`initialize` takes the existing absolute directory supplied by Android
`Context.getFilesDir`. It creates only `DebugSwitches.savegame`. Optional unchanged
bundled bytes are installed using exclusive file creation when absent; an existing
file survives installation and recreation. Installation is an explicit native
asset step outside original Debug load/save, with no source counter effects. An
installation write failure is reported and can leave its created partial file;
it does not run source load or overwrite an existing save on retry.

The read provider obtains an actual binary file and retains its byte image and
OS handle. Nested source saves truncate/rewrite the real named file while every
already opened read image remains live. Source close calls real fclose. Closed
controls/bytes and resources left open after errors remain owned until Backend
destruction. Lifetime cleanup is separate from source close counters. The source
parser's unsupported old-format/positive-module branches remain unsupported.

The save callback runs the frozen source persistence caller over the Runtime's
live ordered maps. Word writes encode little-endian uint32, byte writes preserve
the raw low byte, and strings encode uint32 length plus exact bytes without a NUL.
The adapter flushes completed typed writes so partial effects are observable in
the real file. There are no successful no-op writes or save callbacks. An ordinary
ENOENT open miss returns the source null stream; other OS failures become explicit
port failures. This error classification is a native dependency contract, not a
newly reconstructed original filesystem implementation.

The original save/writer/load/query recursion is allowed. Arbitrary caller reentry,
global owner reselection, destruction, clearing the shared guard or moving backing
during an active call is forbidden. Errors retain source guard/map/file changes,
without extra close, rollback or parser repair. Source counters record attempted
opens/saves and successful operations separately; bytes_written counts bytes
accepted by stdio, with flush errors reported separately. No original OS stream,
allocator or additional caller/dependency body claim is made.

The 20-case host test uses the actual unchanged 665-byte, 23-entry cache configuration.
It verifies five real saves during original loading, exact final bytes, retained
read images, persistent custom switches across new Backend owners, missing files,
strict path/owner/filesystem checks, malformed/truncated/unsupported/oversized
inputs, real directory-as-file IO errors and ordinary ENOENT write-open misses.
A two-row configuration saves its first true row through the real writer, then
fails parsing the second row. The successful file/map effects remain, its read
handle remains owned, and no extra source close or rollback is added.
Tests use fresh generated directories and preserve the original cache. Explicit
loaded=1 fixtures isolate write error/miss branches; they are not native guard
producers. Android ownership/build wiring remains root-owned.

```powershell
python port/android-native/tests/run_native_debug_files_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files
```
