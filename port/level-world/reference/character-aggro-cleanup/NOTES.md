# CharAI `AI_ClearAllAggro` cleanup ordering

This source slice captures the source-owned neighbor traversal and callback
order used by the Limbus state's `clear_all_aggro` service. It does not claim
that actor aggro tables or the renderer are wired to the kernel.

## Recovered call path and exact mutation order

`CSLimbus::OnFocus` (`0x3c2e58`) calls `CharAI::AI_ClearAllAggro()`
(`0x3d5fa8`) once on every normal return path. A zero Character byte at
`+0x530` skips the respawn-timer branch; a nonzero byte evaluates delay and
hosting gates and may start a timer. Both paths converge on the cleanup call
at `0x3c2ee0`. The original cleanup method reserves a temporary vector to the owner's outgoing count, then walks
the owner's outgoing `std::map<Character*, float>` in its map order.

For **every** outgoing peer, the method searches the peer CharAI's incoming
tree for the owner Character. If present, it erases that incoming entry. It
then appends the peer to the temporary callback vector whether or not the
incoming entry existed. After the complete peer traversal, it clears this
owner's outgoing tree if its count is nonzero. Finally, it calls every saved
peer's virtual slot `+0x3c` with the owner Character; that slot resolves to
`CharAI::OnDeAggro(owner Character)`. Thus unmirrored outgoing peers are
still dispatched, and peer incoming erasures happen before the owner map
clear.

The portable kernel reproduces this sequence. Its callbacks provide the exact
incoming lookup and conditional erase, owner outgoing clear, and peer
`OnDeAggro` notification. This notification must not be implemented by calling
`AI_ClearAggro`: that is a different operation with additional pair, target,
and controller effects. A self peer is valid; owner outgoing and incoming
maps are distinct stores. The original instruction oracle's existing
`self relation` and `remove self relation` cases in
`port/game-data/tests/aggro_differential.py` accept that relation.

## Exact virtual callback identity

The constructor at ELF `0x3cebf0` loads the `_ZTV6CharAI` address `0x966790`
through GOT entry `0x9990e4`, then adds 8 and writes the vptr at
`0x3cec0c`/`0x3cec10`. Those instructions are the 8 bytes
`082082e2002084e5`. Therefore the address point is `0x966798`, and vptr slot
`+0x3c` is ELF `0x9667d4`, full table offset `+0x44`. Its exact bytes
`14203d00` encode `0x3d2014`, `CharAI::OnDeAggro(Character*)`. Both the GOT
entry and callback entry have `R_ARM_RELATIVE` relocations (type 23).
The manifest records the original ELF SHA and these independently hashed
ranges; this is not an inference from a similarly named symbol.

`CharAI::OnDeAggro` performs its debug-switch work, then, when its selected
CharAIScript at AI `+0x1c` is present, forwards the same Character argument
through that script's vptr slot `+0x3c` at `0x3d2094`. The default AI consumer
at `0x3dbea8` is `bx lr`; AISMonster inherits that default slot. The player
consumer at `0x3dde48` has a nontrivial 668-byte body. These consumers were
inspected to identify the boundary, not reconstructed by this kernel.
Actual actor-owned AI selection and its consumers, including any separately
recovered Lua routes, still need a live adapter; no direct Lua invocation
from this notification is claimed.

## Lifetime and reentrancy boundary

Before the first source mutation, the adapter acquires a strong hold for every
outgoing peer and copies every identity in original map order. This is
port-specific safety preparation; the original builds a temporary vector of
raw `Character*` values. Each retained handle is passed to the incoming lookup,
conditional erase, and final peer callback. The kernel releases all holds in
reverse order after dispatch; a throwing callback also unwinds through the
lease owner. This prevents callbacks from using a freed actor if an earlier
callback changes the registry.

The incoming-map predicate is a read-only lookup; a positive result is
followed immediately by the separate source-equivalent conditional erase.
Source table mutation callbacks run in source order: conditional peer
incoming erases, owner map clear, then one virtual peer callback per outgoing
peer. Callback-time reentrancy uses its own current source state; this kernel
adds no artificial reentrancy lock. It copies `Services` and owner identity before callbacks so a
callback cannot change the remainder of this invocation's function/context
selection or owner.

The original operation has no error/rollback protocol. Validation, allocation,
or peer-hold acquisition failures happen before source aggro mutation. If a
mutation callback throws, the exception propagates, acquired actor holds are
released, and already-completed source mutations remain; the kernel does not
claim rollback. A throwing callback may itself have partially mutated its
table before throwing.

## Explicitly separate behavior

`CharAI::AI_ClearAggro(Character*)` (`0x3d6d68`) removes one owner outgoing
relation and the peer's mirrored incoming relation, notifies that peer via
`OnDeAggro(owner)` if the outgoing relation existed, then freshly reads the
peer's current target. If it still targets the owner, the source calls
`AI_SetTarget(nullptr, false)`, freshly reads the peer controller, and calls
`Cmd_Stop`. The fresh target read is after the notification; the fresh
controller read is after `AI_SetTarget`. Neither of those follow-up effects
is part of `AI_ClearAllAggro`, and the pair function is not the virtual callee.

`CharAI::AI_ClearAllAggroTowardMe(bool)` (`0x3d6abc`) is not this path.
`CharAI::OnTerminate()` invokes that separate routine with `false`; its
incoming-neighbor traversal and optional current-target clearing are not
implemented here. Nor does this adapter recover CharacterGroup ownership,
membership registration, automatic aggro acquisition, target selection,
combat, or revival FX.

## Host validation

Run:

```powershell
python port/level-world/tests/run_character_aggro_cleanup_host.py
```

The focused host test checks all-peer dispatch, source map order, mirrored
incoming-entry erasure before owner-map clear, callback-time source-list
mutation/reentry, actor holds for every peer, empty-map no-op, pre-mutation
hold failure, a thrown mutation callback without a rollback claim, self-peer
notification, and duplicate-key rejection. Seven top-level cases are checked;
the main ordered fixture has four notifications and the self fixture has one.
This host test does not execute the original ARM binary or test live actor
storage. The source contribution is one ordering kernel, with three primary
function-range references (caller, driver, notification boundary); verified
reference bytes are not a measure of complete reconstructed function bodies.
