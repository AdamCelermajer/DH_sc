# Limbus focus respawn timer policy

This source slice reconstructs the normal-flow decision in
`CSLimbus::OnFocus` (`0x3c2e58`). It joins the existing signed
`Character::GetRespawnDelay` adapter to the native Character Coordinator's
timer storage, without creating a parallel timer implementation.

## Recovered source order

OnFocus first stores zero in the Character word at `+0x520`, then invokes the
Character vtable entry at `+0x40`. The vtable points to
`GameObject::SetVisible(false)` (`0x38b0f0`). Only after visibility is changed
does the source read `Character+0x530`, its Limbus respawn-timer gate. This is
not the separate `Character+0x1481` byte checked by `Character::InitSpawned`
and `GetRespawnDelay`.

With `+0x530 == 0`, OnFocus makes no delay, suppression, RespawnTime property,
online, host or timer query. It still reaches `CharAI::AI_ClearAllAggro()` on
the normal return path.

With `+0x530 != 0`, it calls `GetRespawnDelay()` once and tests its signed
result `> 0`. A nonpositive result skips all player-manager and timer work.
For a positive result it reads the online byte at PlayerManager `+5`. A zero
byte schedules directly. A nonzero byte calls the separate
`PlayerManager::IsLocalPlayerHosting()` source helper and schedules only when
that helper returns nonzero. The helper queries online byte+5 itself and,
when online, checks `OnlineGameState+0x34`: modes 3 and 4 use the matching
live-host predicate. Other modes fall through to a fresh online-byte query;
offline then counts as hosting, while online checks local player zero's
`IsHost`. Its internal reads and mode routing stay behind that call-through
service; they are not the same value as the outer byte+5 gate and are not
cached by this producer.

When scheduling, OnFocus calls `GetRespawnDelay()` a second time after the
manager decision and passes the second result's low 32 bits to
`CharTimers::TMR_Start(duration, 0, 0x2f, nullptr)`. It ignores the returned
timer ID and does not re-test the second delay. Therefore if the second read
changes to zero or negative after the positive gate, those exact bits still
reach the timer service. Each getter invocation independently reads
`Character+0x1481`; when that suppression byte is nonzero the getter returns
zero without looking up property 11. Otherwise it reads RespawnTime property
ID 11 and applies signed arithmetic shift right by 8 followed by 32-bit
multiplication by 1000 with ARM wrap semantics.

Every completed normal source branch calls `CharAI::AI_ClearAllAggro()` once,
after any optional timer request. The adapter does not add cleanup when a
port-only callback reports failure or throws; those paths preserve earlier
effects but stop before further synthetic source actions.

## Implementation and validation

`character_limbus_respawn.cpp` exposes this branch policy through narrow
callbacks. It uses the existing `character_respawn::get_delay` implementation
for both getter calls. Its host fixture calls the real
`dh2::character::Coordinator::start_timer`, which exercises the existing timer
store with event `0x2f`, repeat `0`, and `user_ref == 0`.

Run from the repository root:

```powershell
python port/level-world/tests/run_character_limbus_respawn_host.py
```

The 14 host cases cover the ordered prefix and delayed `+0x530` read,
independent `+0x1481` reads, property ID 11, signed positive/zero/negative
first delays, offline and online-host routes, a changed second delay,
truthy-negative host result, one-shot event arguments, ignored negative timer
ID, failure/exception boundaries, known property-service/result alias
rejection, normal-path final aggro cleanup and reuse of Coordinator storage.
This is host source validation only: live Android actor fields, native
PlayerManager hosting, event `0x2f` gameplay delivery and revive presentation
are not wired or verified by this slice.

Exact original function addresses and byte hashes are in
[`original-functions.json`](original-functions.json).
