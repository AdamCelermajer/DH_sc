# Owned original debug-switch callers

## Scope

The new Runtime reconstructs **three complete original caller bodies**:
DebugSwitches::load512B (`0x337888`), GetSwitch196B (`0x337a88`) and
SetSwitch236B (`0x337ddc`). It owns real switch/module maps rather than returning
invented query facts. The `_loadSwitches`932B body is reconstructed only for the
evidenced WSBD binary switch-loop paths, including version1 and version2 with a
nonpositive module count, bounded names and source short-file return. Positive
module lists, old versions/text/invalid magic remain explicit unsupported paths.
The manifest pins13 original ranges and9 exact literal computations; that is
not13 newly complete functions. Filesystem, save and STL/string dependency bodies
are external, with **zero new complete dependency-body claims**. Native wiring
is not claimed by this standalone unit.

Original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Caller order and owned maps

The loaded byte is shared DebugSwitches::s_loaded (`0x9a1d48`), independent of
which Runtime is selected. Load returns immediately when it is nonzero. It
otherwise writes1 **before** application/engine/filesystem reads and open. A
normal missing file or null filesystem proceeds to defaults. A valid opened file
is decoded into the original receiver's owned maps, then closed through the
captured filesystem. Afterwards it captures the global singleton once at0x337930,
retaining that owner for load then SetSwitch(false) for IsDeactivatingFlashMenus,
its Update variant, its Render variant; load then GetSwitch for ConnectToAlphaServer
and BetaServer. A provider changing the global selection during these phases does
not replace the captured owner; a nested missing-key trace reads the global anew.
The exact names/case and singleton/GOT fields are verified in the manifest.

GetSwitch finds the current borrowed key. On a miss it inserts false **before**
capturing/loading the global singleton and querying isTracingDebugSwitches. It
then reaccesses the current borrowed key and reads its byte. A provider changing
the key can therefore leave the first key inserted while the final lookup inserts
a different key. Existing keys skip tracing. Returned bool bytes are not coerced
to an unrelated actor state.

SetSwitch's missing branch traces **before** inserting the current key false. It
then reads that entry, compares the captured value and, on a change, writes the
byte before calling the real save provider. Missing false and unchanged values
do not save. This order matters when file/debug callbacks change globals or
borrowed key contents. Map insertion retains owned strings and values. The C++
maps replace the original STL representation, not the source ordering/actions.
Nested source calls terminate through the early loaded guard and insertion of
the tracing key. A defensive128-level port limit rejects unresolved recursion.

## Actual configuration

Actual DebugSwitches.savegame is665 bytes with SHA256
`51a3827f0109e16d1520e76d5b19736df3afe38375b91b955ac519f954234d6b`.
It contains magic0x44425357, version0x20000, module count0, switch count23.
Each name is a u32 length plus that many bytes, then a byte value; caller code
normalizes the parsed byte to0/1. The existing maintained persistence little
endian word primitive is reused. The reader consumes each item before tracing
isTracingDebugSwitchesFile and applying SetSwitch to the original receiver.
Names copied from file retain their content across providers. Later file bytes
are read freshly; a provider may mutate that live backing. The original read
buffer's NUL truncation is preserved while consuming all declared name bytes.

All23 final switch values match the file. Its5 true rows invoke5 real save calls
during loading. The original missing-key source tracing inserts its own false
keys before their later file rows arrive. isTracingChar_Stats is absent; its first
GetSwitch inserts false without saving. The caller's discarded result does not
permit an empty fake load/query provider.

Supported parsing also preserves source remaining<=11 short-file return, signed
nonpositive counts, duplicate key updates and unconsumed suffixes. Invalid/truncated
read boundaries retain earlier guard/map/save effects. Formats, positive module
lists and names>255 outside the bounded proof fail explicitly. This is not a
claim of full original save/profile/file-format compatibility.

## Real resource/error boundaries

Open returns a real retained stream identity plus live byte backing; missing
identity is an ordinary missing-file result. Close uses the captured filesystem
and returned file. Save must implement the real persistent dependency for the
supplied owner; a successful no-op is not valid. Native adapters may write a
dedicated mutable app configuration rather than the preserved original evidence
file. That adapter and the original136B save/writer body are not implemented here.

Runtime/global/application/engine/filesystem/backend/file ownership is retained
on one owning thread. Service records are captured at public entry. Providers
may change scalar global selections and borrowed key/file contents; active owners
and map controls cannot be destroyed/erased or arbitrarily externally reentered.
Internal source recursion is allowed, including the real save writer's original
load/query tracing through retained Runtime owners. This query may insert keys;
owned std::map insertion preserves the writer's retained iterators. Errors/throws retain preceding actions, without
added close/save/rollback. Providers manage open resources after port errors.
Allocator/C++ string unwinding is not an original exception-behavior claim.

## Verification

Host cases check owned map insertion, changed/unchanged saves, repeated loading,
all41 current contract branches, original short-file behavior, supported versions,
unknown domains, provider failure/throw at open/save/close and retained partial
effects. The ARM comparison executes actual complete load/GetSwitch/SetSwitch
instructions and accepted original loader branches. Named primitives, native
strings/map find/index, memory-stream virtuals and save dependency fixtures remain
explicit. Original instructions perform guard/byte stores, nested ordering and
key rereads. Entire final maps and ordered real resource requests are compared,
including all actual cache rows. The48 comparisons include a save provider changing
an unread file byte and signed negative module/switch counts; the maintained reader
observes the changed byte when it reaches that row. A save-time global-owner swap
during the first default reset verifies retained ownership for subsequent defaults
and server queries. No soft-float import is modeled.

```powershell
python port/level-world/tests/run_debug_switches_runtime_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files
```
