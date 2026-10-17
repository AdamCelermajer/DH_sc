# `Character::Update` lazy-script and concurrent-AI slice

This source kernel covers one bounded region of the original
`Character::Update()` body, not the full function. Its entry is
`0x3ac34c`; the ordinary inline block ends at `0x3ac570`. The level-29 branch
reaches the cap tail at `0x3aca48`, and the shared map-search/insertion return
blocks at `0x3aca98..0x3accf4`. The full `Character::Update()` symbol is 3776
bytes, but this unit does not implement its other phases.

The code enters here only after the enclosing `SM_GetState()!=0` branch at
`0x3abf84`. The earlier `Character+0x520` test at `0x3ac2b8` is a distinct
part of `Character::Update()` and is not included. The bounded block itself
reads the state twice: state 12 skips it, then state 2 skips it; otherwise a
nonzero byte `+0x1480` or nonzero active-AIS association at `+0x3e4` skips
lazy loading. This field overlaps embedded `CharAI+0x1c`, so the port keeps
its identity full-width even though the ARM32 instruction reads one word. No
service call occurs before those gates.

## Recovered order

1. Call `Application::GetCurrentLevel()` and read the returned Level's `+0x3c`
   OID. The original dereferences the Level without a null check; a null
   provider result is therefore an adapter error, not a source fallback.
2. Read `Character::s_concurrentAI`, the shared `std::map<int,Character*>`.
   For OID 29, the reached tail at `0x3aca54` compares its size with `0x18`;
   eviction occurs only when size is **greater than 24**. Other OIDs compare
   with 8 at `0x3ac3a8`; eviction occurs only when size is **greater than 8**.
   Both paths choose `begin()` (the smallest signed key).
3. For an eviction, call the victim's controller `Cmd_Kill(nullptr,true)`, then
   call `Character::UnLoadScriptProcess(iterator,true)`. The latter is a typed
   service boundary here; its original body erases the map iterator/decrements
   size before forwarding to the victim CharAI unload body. The kernel does
   not recreate that outer unload body's tree surgery.
4. Call the current Character's `CharAI::LoadNInitScriptProcess(true)`. A zero
   result returns from this lazy-load section. A nonzero result is followed by
   `IsMonster`, `IsMiniBoss`, and `IsBoss`, in that order; non-monsters,
   mini-bosses, and bosses do not enter the concurrent map.
5. Read `GetOnline()` and its byte at `+5`. A nonzero byte skips insertion.
   Then reread Character byte `+0x3ec`; zero skips insertion.
6. Read `Timer::getRealTime()` and insert into the same ordered map with the
   raw 32-bit clock word interpreted as a signed `int32_t` key. If that key
   already exists, the source still writes the current Character pointer into
   the returned node's mapped-value field at `0x3ac56c`; it therefore replaces
   that existing mapped Character without changing map size.

The direct app/map GOT references are recorded in
[`original-functions.json`](original-functions.json). The map is a process-wide
container, not an actor-owned queue. The separate `CharAI::IncUpdateQueue`
`180ms` scheduler remains another subsystem.

## Test boundary

The host kernel uses typed providers for CurrentLevel, controller kill, outer
Character unload, LoadNInit, the three class predicates, GetOnline's byte, and
the raw real-time clock. The runner executes the original ARM scheduler block
and reached cap/insertion branches against observing fixtures. It intercepts
the `std::map` allocator/tree helpers and listed dependency bodies; it does not
claim those dependency bodies are rebuilt by this module.

The high-level map projection is a port-owned fixed array of signed-sorted
entries with an explicit byte extent. It is **not** the original red-black
tree and does not recreate the original allocator or iterator layout. The ARM
oracle intercepts insertion helpers, and the host projection models their
observable map order, eviction, timestamp insertion, and duplicate-key
replacement. Native integration must supply stable actor references and an
array adapter; it must not cast `std::map` storage into this projection.

Before dereferencing facts, the kernel checks alignment, address-range overflow,
the caller-declared exact entry-backing extent, and overlap among Character,
map descriptor, Services, output Result, declared service-context extent,
entry backing, and every mapped Character projection. Host coverage includes
15 malformed-pointer/alias/reentry cases; array extent is part of the adapter
contract because this host map view is not an allocator-aware tree. Same-
Character or same-map callback reentry is rejected as port protocol. The
provider context's declared extent covers its projection object; any backing
allocations referenced from it must also remain live and disjoint by caller
contract. These guards cannot prove arbitrary input pointers are mapped, so
all borrowed storage still must be valid and retained for the synchronous call.

The source retains the begin iterator while calling victim `Cmd_Kill`, then
passes it to outer `UnLoadScriptProcess`; therefore the adapter rejects a map
mutation during `Cmd_Kill` before passing a stale iterator onward. The source
side effect from a completed kill is retained if the next provider fails.
Likewise, an unload provider may have erased the victim before reporting an
adapter error; the kernel does not roll that source effect back. Successful
unload must remove exactly the begin entry before the current actor's
LoadNInit call. Provider changes to ordinary actor facts are read at the later
source gates; actor identity and embedded CharAI projection addresses remain
stable.

The caller serializes other updates to this shared map. Adapter errors preserve
any already completed command, erase, or provider effects. Focused host tests
exercise three such effect cases: a successful kill and erase remain visible
when an unload adapter reports failure after mutation; a LoadNInit effect is
not rolled back when its adapter reports failure; and a map mutation during
Cmd_Kill preserves the kill but rejects the now-stale source begin iterator.

The maintained runner is
`port/level-world/tests/run_character_update_script_scheduler_host.py`. Its
latest checked run passed 18 host/ARM behavior comparisons with 127 distinct
original block PCs and no mismatches, plus 15 pointer/alias/reentry guards and
three adapter-effect cases. It also passed one WSL AddressSanitizer/UBSan run
over the host fixture. ARM map allocator/tree helpers and named dependency
bodies remain intercepted as described above.

Native frame invocation and real owner/service wiring remain pending. This is
not a full `Character::Update()` frame. It does not reconstruct map allocation, full
`Character::UnLoadScriptProcess`, AIS destruction, source frame eligibility,
`+0x520` handling, room/zoning ownership, or the enclosing state-zero branch.
