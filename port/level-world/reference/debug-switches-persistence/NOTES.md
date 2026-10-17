# Original debug configuration persistence callers

## Scope

This unit reconstructs two complete caller bodies: DebugSwitches::_saveSwitches
at0x337b4c/520 bytes and DebugSwitches::save at0x337d54/136 bytes. It consumes
live owned maps, including the maintained DebugSwitches Runtime, rather than a
serialization snapshot. Standard string/map ownership replaces the original
STL ABI. Filesystem, stream-write and allocation bodies are dependencies; there
are zero new complete dependency-body claims and no native wiring claim.

## Save and wire order

Save reads application→engine→filesystem and captures that filesystem. A null
filesystem returns normally. It opens DebugSwitches.savegame with mode1; an
ordinary null result returns. A real stream runs the writer, then closes through
the captured filesystem even if providers changed the application selection.

The writer's null-stream path performs no writes or tracing. Otherwise it writes
little endian u32 magic0x44425357 and version0x20000; reads/writes the module
map's count; then traverses its live ordered nodes, writing each name and byte.
It next reads/writes the switch count and captures its first live node. Empty
switch maps return without tracing. For a nonempty switch map, it captures the
global tracing Runtime once at0x337c28 and retains it through all iterations.
Each iteration calls that owner's load, constructs isTracingDebugSwitchesFile,
calls GetSwitch and destroys the temporary string BEFORE reading the current
node's name. It writes the captured name pointer/length, then rereads the same
retained node's byte after the string writer returns and writes that byte. The
next node is read after the byte writer returns. GetSwitch's result is discarded.

Names are a little endian u32 byte length followed by exactly that many bytes,
without an extra NUL. Empty names and embedded NUL bytes retain their lengths.
The raw node byte is written without bool normalization. Maps are ordered by
their byte string keys. Modules precede switches.

Count capture is separate from traversal. Tracing/provider insertion may increase
the emitted rows after the header count was written. Insertion after the current
node can be visited later; insertion before it is not replayed. The maintained
writer preserves this source behavior and does not repair headers or stop at a
captured count. For example, an initial map containing only A writes count1, then
the source missing-key tracing inserts two later keys and emits three rows.

## Ownership and adapters

Owner borrows the two real map controls from Runtime::switches/modules. Controls,
identity and current nodes remain live on one owning thread; insertion/value
changes are allowed but erasure, key mutation and destruction are forbidden.
Captured map iterators stay valid under std::map insertion. Borrowed string bytes
and streams/backends remain live through synchronous calls. Services are captured
at public entry. The real source save→writer→load/query recursion is allowed.
Other arbitrary reentry into the same writer is outside the adapter contract.

Typed open/word/string/byte/close services must perform real stream effects.
They are not successful no-ops. Port errors/throws retain preceding writes and
map effects and stop without extra close, rollback or repair; the backend owns
remaining resource cleanup. Standard C++ allocator/unwinding behavior is not a
claim about original STL failure behavior. Native file backend wiring is external.

The actual665B cache configuration remains unchanged evidence. A native adapter
must retain each opened read stream's backing across nested saves to the writable
configuration path. It cannot destroy the active read view while replacing a
file. The source Runtime requires this retained view, and this persistence caller
does not add a snapshot of the maps or alter the source save order.

## Verification

The 44 host cases and 48 original ARM comparisons cover actual emitted bytes,
modules, raw/empty names, count capture,
insertion, fresh node byte reads, captured tracing owner/filesystem, source nested
loading and saves, and failures/throws across every nontrivial write/save phase.
The original ARM runner executes the complete writer/save callers and original
load/GetSwitch/SetSwitch callers for nested recursion. It builds retained original
tree nodes and executes the original next-node traversal. Primitive stream writers,
map allocation/find/index and standard string allocation are explicitly modeled
dependencies. No soft-float helper is modeled, and unknown imports fail.

```powershell
python port/level-world/tests/run_debug_switches_persistence_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files
```
