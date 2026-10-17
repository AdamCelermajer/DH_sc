# Character respawn outer gates

This host-only kernel reconstructs the outer `Character::CanRespawn()` and
`Character::GetRespawnDelay()` decisions. It binds the already reconstructed
`CharAI::GroupInfo::CanRespawn()` kernel for the optional group branch instead
of reimplementing its member/status logic.

## Source behavior

`Character::CanRespawn() const` (`0x3a5248`) reads the byte at Character
`+0x1481` first and returns false when it is nonzero. `Character::InitSpawned`
(`0x3b379c`) writes 1 to that byte; constructors initialize it to 0. The
byte's exact engine-facing label is not recovered, so the adapter calls it the
**InitSpawned suppression byte** rather than assigning a broader semantic.
With the byte clear, CanRespawn requests CharacterProperties property 11,
`RespawnTime`, and rejects a raw value `<= 0`. If the Character has no
GroupInfo pointer (`Character+0x3fc`), it returns true after those two gates.
Otherwise it delegates using the Character state ID to the existing group
kernel.

`Character::GetRespawnDelay() const` (`0x3a4bac`) also checks the same byte
before reading property 11. When the byte is set it returns zero. Otherwise
its ARM sequence does a signed arithmetic shift right by 8 and multiplies the
result by 1000. The port reproduces ARM32 low-32-bit `MUL` wrap explicitly;
it does not clamp negative delays, round negative values toward zero, or infer
eligibility from the converted delay. Thus raw `255` permits a no-group actor
through CanRespawn while GetRespawnDelay returns 0 ms, and raw `-257` converts
to `-2000` ms.

The property resolver callback is invoked with the exact ID 11 only after the
InitSpawned gate. For CanRespawn, a nonpositive raw result returns before the
optional GroupInfo pointer is read. A null GroupInfo pointer returns true for
any Character state once the outer gates pass, matching the source branch.
When a group exists, the kernel calls
`dh2::character_group::can_respawn()` directly, preserving its state-first
read ordering, member callback order, and status update.

## Boundaries left outside

This slice does not create or attach GroupInfo, own its member vectors, or
implement `OnDied`/`OnEnemySpotted` producers. It consumes a caller-owned raw
resolved property value through a property-ID callback and does not integrate
the 224-field property sheet itself. `CSLimbus::OnFocus` (`0x3c2e58`) timer
eligibility is separate: it tests `+0x530`, positive GetRespawnDelay, and the
PlayerManager local-host policy before starting one-shot event `0x2f`; those
timer/hosting facts are deliberately not synthesized here. Existing native
timer storage is in `character_timers.{hpp,cpp}` and runtime composition is
`character_coordinator.{hpp,cpp}`.

The outer result is committed only after property and optional group queries
succeed. A failed property/group adapter call is a protected port error; the
source routines themselves do not expose this failure protocol.
The actual outer result is checked against group header and consumed member
storage, because the delegated leaf receives a separate local result and
cannot validate that outer alias. Group headers also cannot overlap outer
Facts or Services; consumed member ranges cannot overlap the outer arguments
or wrap the address space. These are port input protections, not extra source
respawn predicates. Header range checks compare addresses without reading
group fields. Member-range checks run only on the leaf's Idle/status-1 path;
suppressed/nonpositive-property and unsupported-state paths retain their
no-read behavior. Consumed storage and invoked callback contexts remain caller-owned
and must stay live through return. Callback effects or exceptions do not have
a rollback protocol.

## Host validation

Run:

```powershell
python port/level-world/tests/run_character_respawn_outer_host.py
```

The host test verifies the suppression/property/group read boundaries, the
no-group branch, reuse of the current group kernel, property ID 11, Q8.8
conversion, arithmetic negative shift, and signed 32-bit multiplication wrap
at `INT32_MIN` and `INT32_MAX`. Its 29 cases additionally check raw suppression
byte 255, poisoned unread group pointers, output aliases over group/member
storage, member address wrap, Facts/Services output aliases, and property/group
failure result preservation. Limbus and Idle/status-2 cases use poisoned
member ranges to verify those unused fields remain unread. The group-header alias regression failed before
the outer boundary repair and passes with it. The test does not call the original ARM binary,
resolve a live Character property sheet, wire the Limbus timer/hosting policy,
or mutate a native GroupInfo object.

The pinned function byte ranges and exact coverage are recorded in
[`original-functions.json`](original-functions.json).
