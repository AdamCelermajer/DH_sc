# `Character::CanUpdate()` source decision

This unit reconstructs the complete source decision body
`Character::CanUpdate()` at ELF `0x3a52a4`. The function-index record is 316
bytes: 77 ARM instructions (308 bytes) and 8 mapped data bytes at the tail.
The original symbol and range hash are pinned in
[`original-functions.json`](original-functions.json). `Character::Update()`
dispatches through virtual slot `+0x148` at `0x3abf60`; the `Character`
vtable entry resolves to `0x3a52a4` before the larger update/scheduler body
proceeds.

## Recovered order

1. Capture `Character+0x2d8` once. If present, clear the current root
   scene-node byte at `+0x200` before calling `GetOnline()`.
2. If online, call Character vtable slot `+0x54` (`IsRemotelyUpdated`). A
   truthy result skips the culling, `IsDead`, and respawn checks and proceeds
   to the final enable/return path.
3. Offline objects and online objects not handled remotely share the next
   branch. If a Visual was captured, capture `Character+0x418`, call
   `PlayerManager::GetLocalPlayer(0, true)`, and compare that captured word
   with the returned player-info `+0x660` Character pointer.
4. On a mismatch, reread `Character+0x2d8`, then its root node. If root
   `+0x118` or Character byte `+0x2fc` is nonzero, call
   `ObjectBase::TestCullingBeforeUpdate(this, this+0x12c)`. A zero result
   returns false unless Character byte `+0x1480` is nonzero.
5. The shared non-remote path calls Character vtable slot `+0x34` (`IsDead`).
   If dead and the current-visibility byte `+0x80` is zero, call `CanRespawn`; a zero
   result returns false.
6. At the final path, if a Visual was captured and the current Character
   visibility byte `+0x80` is nonzero, reread the root pointer from the captured
   Visual and set its `+0x200` byte to one. Return true.

`+0x80` is the ObjectBase current-visibility byte, not a scheduler enable
flag. `GameObject::SetVisible(bool)` at ELF `0x38b0f0` writes zero for false;
true copies the object's enabled byte `+0x8a` into `+0x80`. The eligibility
kernel consumes the resulting raw byte and does not reconstruct that setter.

These byte tests use source zero/nonzero truthiness without normalizing raw
service results. The test runner executes the original ARM function itself
for 26 deterministic offline/online, culling, death, respawn, callback
mutation, and scene-flag fixtures. It compares the host kernel's result,
service order/arguments, and scene flag effects against those executions.
The separate host-only guard suite also covers a misaligned Visual pointer,
initial/fresh/late Visual-root aliasing, result-storage overlap, source byte
domains, and same-Character recursive entry. The failure suite covers all six
provider operations as both errors and exceptions plus a missing provider.
The guard and failure suites pass under WSL AddressSanitizer and
UndefinedBehaviorSanitizer.

## Explicit boundary

This is the predicate only. It does not reconstruct the enclosing
`Character::Update()` body, state-machine gating, script loading, concurrent
AI caps/eviction, timers, or engine object/scene ownership. Services are
fixture providers for the listed existing engine calls. Production CMake
compilation is verified separately; native frame invocation and real
owner/service wiring remain pending.

Run from the DH_sc worktree:

```powershell
python port/level-world/tests/run_character_update_eligibility_host.py `
  --original-elf ..\test_strategy\libDungeonHunter2.so
```
