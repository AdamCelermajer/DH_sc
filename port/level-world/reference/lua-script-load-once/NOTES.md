# Per-script Lua file execution cache

`lua_script_load_once.{hpp,cpp}` implements the narrow `LuaScript::Load` cache
boundary: for one caller-supplied VM identity it remembers exact, already
resolved path strings whose source bytes have successfully executed in that
VM. A cache hit returns the source-success result without validating/touching
the bytes or calling the VM loader. A miss requires retained bytes and a real
synchronous loader callback; the path is inserted only after that callback
returns zero. Failure keeps any Lua mutations and error text produced by the
VM, and the path remains retryable. The helper rejects reentry and VM reset
during the loader callback.

The source `LuaScript::Load(const char*)` wrapper is ELF `0x37b574`. It forwards
to `LuaManager::AddFile(LuaScript*, const char*)` at `0x37b23c`. AddFile first
forms the full path from the script's current directory and requested path,
then checks the exact full path in that LuaScript's loaded-path set at `+0x80`.
The hit branch returns true before loading/executing the file. On a miss it
uses a separate LuaManager-wide path-to-`StreamBuffer*` map for bytes, calls
the target `sfc::script::lua::Instance::loadFile`, then inserts the path into
the per-LuaScript set only on success. The helper starts **after** source path
resolution and receives the resolved key; the shared byte cache and complete
AddFile path/extension logic are deliberately outside its implementation.

This distinction matters for `skills/_commons.luac`: the file starts by
resetting global skill callbacks and creates its local `SKILLS` registry.
Executing its bytes again in one VM erases prior registrations. The host test
executes the recovered 11,291-byte file in the float32 Lua runtime, registers
and runs a skill callback, then sends a duplicate path with no bytes or loader
service. The callback runs again, proving the duplicate shortcut preserved
the existing registry. The same path executes in a replacement VM after an
explicit reset. Separate cases cover a new path, syntax failure, runtime
failure after a global side effect, retry, path/data alias rejection, VM
mismatch, callback reentry, and reset during an active callback.

Run from the repository root:

```powershell
python port/level-world/tests/run_lua_script_load_once_host.py
```

The runner compiles the existing float32 `port/adam-script-runtime` Lua units
with the focused helper and fixture, verifies the recovered `_commons` hash,
and writes `port/level-world/build/lua-script-load-once-host/validation.json`.
This is host evidence only. It is not wired into the live Android Session or
the source `LuaManager` byte-cache owner, and it does not claim full
`LuaManager::AddFile` reconstruction.

The game's proprietary original code and script are evidence inputs. This
note records symbols, byte ranges, and hashes only; it does not reproduce the
original function bodies or script text. Applicable third-party runtime
notices remain with the Lua runtime source distribution.
