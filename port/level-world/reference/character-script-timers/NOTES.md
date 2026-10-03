# Native CharacterTimers to Lua adapter

`character_script_timers.hpp/.cpp` is an owned native adapter, not an original
object layout. It borrows a genuine TimerStore, TimerServices and VM. The
Character identity comes from TimerStore.owner and must remain stable. Bound
callback context must outlive the VM globals.

StartTimer/StopTimer/Trace use the separately original-verified source Value
projection and coercion callbacks in `script_game_bindings.c`. The adapter
invokes the actual native CharacterTimers kernels. Native malformed-storage,
capacity and growth-during-update diagnostic results become protected port
errors. They are never returned to Lua as negative timer IDs. This is explicit
native error handling, not original allocator/panic parity.

The production adapter does not create an AIS, publish an active script, load
commons, or implement LuaManager. Its caller must supply the actual ownership
and expiry dispatcher. Binding does not assert that ownership has been solved.

The composed host audit uses the actual shared native world and Lua libraries,
with compiler commands confirming ASan/UBSan on C and C++ units. Its real
TimerStore expiry enters complete `dh2_character_ai_event` event35, reads the
actual timer ID through an explicit source service, invokes the recovered
active-gated `dh2_character_ai_event_script_timer`, and calls the actual VM.
There are seven expiry/ID reads and six selected-AIS VM calls; the inactive
case reads the timer ID but does not invoke Lua. Locked/global-blocked gates
do not suppress this source timer branch.

The actual authored13535-byte AI common script loads and its empty OnTimer
executes. Additional owned Lua fixtures check one-shot/loop expiry, StopTimer,
wrong types, callback replacement reactivating the same native slot, returned
table `_this` conversion starting a timer, protected callback errors, exhausted
native storage and owner drift. Native timers clear one-shot active before
expiry callbacks; a callback can therefore reactivate that slot. The common
script's callback-table cleanup is a distinct mechanism.

`reports/character-script-timers-host-audit.json` binds sources, compiler
commands, common bytes and all resolved DSO bytes before/after replay. The
fixture's owner/AIS/callable projections are explicitly caller-owned. This
proves composed native kernels and VM services, not complete original AIS
construction, per-script alias ownership, game initialization or live renderer
integration. No physical ARM64 or full-game completion is claimed.
