# CharAI master update

Original `_UpdateMaster()` is `0x3cc5a4`, 444 bytes. Its complete caller
orchestration is maintained in `character_ai_master_update.cpp`. The
ten-range manifest also pins supporting query/event bodies; those bodies
are explicit synchronous providers, not ten new rebuilt functions.

## Source order and mutable facts

- A null live master at AI+0x50 returns without queries or snapshot writes.
- Fresh owner GetCharAIId runs, with its return discarded. Master is reread
  before virtual vtable+0x34 IsDead. `(returned_word XOR 1) & 0xff` is the
  new alive byte; this is not Boolean negation for noncanonical words.
- Died/revived events `0x12/0x13` use fresh owner/master, and happen **before**
  storing the cached alive value into AI+0x54. Event changes to that byte
  are overwritten by the pending source store.
- A nonnull freshly reread master is passed to AI_IsInSight. Out/in sight
  events `0x14/0x15` happen before storing the low byte into AI+0x55.
  The full returned sight word stays cached for the later gate. For example,
  a result of `0x100` stores zero while still allowing the range path.
- Fresh master, current alive byte and cached full sight gate IsMyTurn.
  The named `IsMyTurn(CharAI const*)` implementation does not read incoming
  r1; the source call here sets only r0 to the captured AI identity.
- Fresh owner virtual+0x124 CanRangeAttack selects CloseRange then Range,
  or MeleeRange. Each range call rereads master. Tail RaiseEvent rereads
  both owner and master: `0x18` close, `0x17` ranged, `0x19` melee, `0x16` out.

There are no Limbus/AwaitingSpawn or Interactive queries in this body;
those belong to target update. This preserves independent source steps.

## Borrowing and port failures

`State` is a live port projection, not the original ARM layout. AI identity
is stable through the call. Callbacks may replace owner/master and edit
snapshot bytes. Source pointers passed to a callback are the captured
arguments of that call. The next callback rereads fields where original
instructions do so. One thread owns a state/output; same-state reentry is
outside the contract. Independent calls may nest.
Providers retain valid owner identities at actual getter/virtual dereference
sites and may not overwrite the service table or destroy borrowed storage.

Malformed arguments/overlap/alignment are rejected before output writes.
Provider exceptions/failures stop at that boundary and retain completed
effects. A master cleared by GetCharAIId would cause an original null vtable
dereference; the port reports `invalid_source_fact` after the query instead.
This is a bounds response, not invented game behavior.

## Reproducible component verification

Run `tests/run_character_ai_master_update_host.py` with the pinned original
ELF and a C++17 compiler. The runner verifies every manifest symbol, size
and original byte hash before executing original caller instructions.
It compares compiled source call order, captured subjects/peers/event IDs,
snapshot bytes observed at each call and final mutable owner/master state.

Root verification passed **54 original ARM comparisons**, **eight guard
cases** and **ten separate port failure cases**, zero mismatches. Fixtures
cover each range branch, noncanonical words, owner/master replacement at
every taken service, master clearing, events changing snapshots and cached
stores after delivery. Dead, sight, turn, range and event callee bodies are
observed fixture providers. They are not reconstructed by this test.

These are host component comparisons, not the original game running in an
Android emulator. Live companion behavior, concrete range/event service
ownership, native wiring and the full game remain separate integration gates.
