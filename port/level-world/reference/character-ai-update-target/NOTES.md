# CharAI::_UpdateTarget producer

New `character_ai_update_target.{hpp,cpp}` implements the complete ordering of
the original 556-byte caller at `0x3cb908`. This is one caller orchestration;
FSM/virtual/getter/sight/range/event dependencies remain owning providers.
No full source body is claimed for those callees. Exact original ranges and
primary vtable bytes are pinned in `original-functions.json`.

## Reuse and scope

The producer reuses `character::set_target::State` and OwnerFacts. Current target
`+0x40`, independent last target `+0x44`, alive byte `+0x48` and sight byte `+0x49`
are the same actor-owned values used by the recovered setter. It does not invoke
the setter when the source writes current/last directly. Existing
`game-data::dh2_ai_target_update` remains a snapshot-only API: its documented
fixed callbacks cannot represent live owner/target replacement or repeated
Interactive queries. That frozen API is unchanged.

Source sight must bind `character_ai_sight::evaluate_object`, preserving its live
owner propagation, borrowed position/radius lifetimes and repeated owner reads.
Melee/close/ranged queries are exact full-body service boundaries. They are not
replaced by copied distances or generic reach values. `Character::RaiseEvent`
owns its real event/controller relay. This module is not a complete AI pipeline
and is not wired into native/Android/CMake.

## Exact source sequence

1. Fresh owner `SM_IsAwaitingToSpawn` (`0x3c0230`, state17), then fresh owner
   `SM_IsInLimbus` (`0x3c01c0`, state0 only: subtraction returns zero at state1).
   Either true returns. No current target also returns without touching last.
2. Capture current target and fresh owner for target virtual `+0x88`
   IsInteractive(owner). False captures the next fresh owner, directly clears
   target+40 and last+44, then sends event12 with null payload and returns.
   True rereads target and returns if a callback cleared it.
3. Fresh owner GetCharAIId (`0x3a2fec`) is a real call even though its result is
   unused. Reload current target for virtual `+0x34` IsDead. Cache
   `(raw_dead XOR 1) & 255`. This is **not** boolean negation of an arbitrary raw
   return. Compare the current alive snapshot by truthiness, not byte equality:
   previous nonzero→computed zero sends10; previous zero→computed nonzero sends11.
   Event owner and target payload are fresh. Only after event return, capture
   current target then write the cached computed byte48. Thus a death callback
   may clear the target while the original byte write still happens.
4. If the captured target is nonnull, call sight on the original AI and that
   target. Cache its **full raw word**. Read old byte49 after the sight callback;
   false→true sends13, true→false sends12. Event callbacks again precede the
   store. Capture current target, then store only the low8 bits to byte49.
5. Continue only with that target nonnull and cached **full** sight word nonzero.
   Repeat Interactive on current target with fresh owner. False returns without
   clearing anything. Fresh owner virtual `+0x124` CanRangeAttack selects:
   true→CloseRange(original AI,fresh current target), true gives16;
   else Range(original AI,fresh current target), true gives15, else14.
   false→MeleeRange(original AI,fresh current target), true gives17, else14.
   Final event captures current target and fresh owner after range callback.

The source original AI pointer is fixed through all calls. Owner pointers and
current-target values are repeatedly loaded at the exact sites; computed alive
and sight results survive event-driven replacements. Last target is otherwise
independent and unchanged. No object handle conversion or stored visibility
byte check exists in this caller; those belong to its owning callees.

## Vtable and state evidence

Primary table address point is `_ZTV9Character + 8`, not the symbol start.
Byte offsets `+0x34`, `+0x88`, `+0x124` resolve respectively to Character::IsDead
`0x3a2ed4`, IsInteractive `0x3a4870`, and CanRangeAttack `0x3a4d3c`.
Base GameObject has its own IsDead/IsInteractive implementations, so no
Character-only cast is invented for a target GameObject. CanRangeAttack accepts
resolved property+`0x1078 != -1`, otherwise delegates to EquipSet::HasRangeWeapon.
This dependency is preserved as a service, not reduced to a guessed boolean.

## Lifetime, failure and verification

State, services, output, owners, identities and callback backing remain live on
one thread, including retired owner/target objects after replacement. Identity
keys are stable; callbacks may change target/snapshot/owner bindings but must
not rewrite keys, destroy state, overwrite services/output or reenter this State.
Independent nested state/output is permitted. Missing, failed or throwing services
stop at the boundary, retaining all completed events/mutations and prior writes.
No cleanup or rollback event is invented. Alignment and alias guards reject
known invalid entry overlap before effects; a newly borrowed owner overlapping
control/output is rejected at its next source use. If discarded GetCharAIId
clears the target, the original next instruction would dereference null. The
port reports invalid_source_fact at that unsafe boundary; it does not fabricate
a no-target early return or claim the source crash body is reproduced.

The host tests cover callback retarget/clear, fresh owners, changing snapshots,
noncanonical dead/sight words, independent last target, all four range events,
service/exception failure phases and alias bounds. The runner executes the
existing sight kernel in a host composition case that changes the owner during
the target-position callback, checks the fresh tail radius getter and later
event ownership. Other caller fixtures explicitly supply callee boundaries.
The runner executes the
entire original caller with explicitly modeled callee returns/side effects and
compares call sequence, snapshots at each call, final owner, current/last target
and byte writes. Six additional original FSM leaf cases execute GetState and
both named predicates, proving state1 does not satisfy Limbus. No sight/range
library or native event backend is claimed executed by this caller oracle.
