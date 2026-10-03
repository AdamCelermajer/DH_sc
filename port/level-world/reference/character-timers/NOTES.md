# Source character timers

`character_timers.hpp/.cpp` projects original CharTimers Update 0x3db640,
Start 0x3dbe24, slot lookup 0x3dbd70 and control routines into native 64-bit
objects. Exact captured code, original ELF binding and callback order are in
`../character-timer-discovery/NOTES.md` and its 30-function manifest.

Timer32 has no ARM vptr: ID/repeat/duration/elapsed/active/paused/event/user_ref
are logical source fields. TimerStore32 borrows caller storage and owner
identity; TimerServices32 provides synchronous expiry and optional allocation.
The actor caller should reserve the original 20 slots. Start returns lowest
inactive ID, appends within capacity otherwise, and invokes optional Grow only
outside Update. Grow must preserve existing timers/count/owner and leave the
store unchanged on failure. During expiry the caller must not relocate/shrink
slots or alter owner/update_depth; Start returns -3 if it would need growth.
This explicit native boundary excludes original callback reallocation with a
retained old slot pointer. It does not queue callbacks or invent stale-pointer
recovery. Timer callbacks may safely Stop, Pause, Start in spare storage or
reuse the current one-shot slot. Parent owns the AI event-routing adapter.

Update receives integer dt_ms and the truthy ScriptManager+0x30 source gate.
It snapshots count, skips inactive/paused timers, wraps unsigned elapsed,
subtracts duration for nonzero repeat, decrements only positive repeat, and
deactivates zero-repeat before callback. Event -1 becomes 0x29. The callback
receives Timer32*, not user_ref. Pausing during callback does not interrupt
that slot's existing catch-up loop. TimeLeft exposes raw elapsed/duration.

`tests/character_timers_differential.py` converts the hash-bound original gold
to `character-timers-reference.bin`; `tests/character_timers.cpp` replays it.
Host g++ -O1 with ASan/UBSan and -Werror passes all 1,856 original update cases
and four synchronous reentry fixtures, plus malformed atomic guards, 64-bit
owner/user_ref, one-shot Start reuse and allocator growth outside Update.
`../../reports/character-timers-host-sanitizers.json` binds original gold,
reference, current source and audited host executable hashes. This proof is
host native source parity; ARM64 and packaged-library execution remain next.
