# Original actor rotation coordinator

ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. `original-functions.json` records the five captured function byte hashes; `original-functions.asm` preserves their instructions. This module reconstructs GameObject::UpdateRotation `0x393710` and emits its final visual synchronization request. The existing heading producer, visual quaternion services and native physics adapter remain separate stages.

## Native handoff

`dh2_actor_update_rotation(RotationState*, const RotationPolicy*, uint32_t* sync_visual)` returns0 on completion or1 for malformed caller storage. RotationState is24 bytes: Euler XYZ, heading target angle, last incremental turn direction, zero reserved word. These correspond to original fields+0x16c/+0x170/+0x174, +0x178 and+0x17c. RotationPolicy is16 bytes: source-derived rotation speed, unsigned GetDt milliseconds, visual-present bool and visual-with-rotation bool. State, policy and output storage must not overlap. Finite input and result arithmetic, boolean fields and reserved0 are required; rejection preserves every input/output byte.

Use `dh2_move_rotation_speed` to obtain the actual Character speed from flags and resolved property payload index47. Character::GetRotationSpeed `0x3a372c` returns exactly-1 when flags+0x520 has bit0x20; otherwise it calls PROPS_GetRotationSpeed `0x3de708`. The latter converts signed fixed256 property data to float, multiplies by1/256 then.01, adds1 and clamps below0. Character::IsUpdatingVisualWithRotation `0x3a2e68` returns the inverse of flag bit0x10. Application::GetDt `0x31f66c` supplies the unsigned word at App+0x8c; the coordinator does not consume GetDtScaled or ObjectManager's literal float1 argument.

The policy argument resolves the original virtual getters before entering this pure coordinator. Successful completion writes the final state and sets sync_visual to `visual_present && visual_with_rotation`. When set, the caller invokes the genuine visual SyncRotation service with final Euler XYZ. This request represents original tail call `0x393774 -> 0x472948`; no body transform is implied.

## Exact operations and branches

The original first calls virtual GetRotationSpeed through slot0xac at`0x393720`. Any negative finite speed copies heading+0x178 to current Z+0x174 and retains the turn flag; that branch does not call GetDt. The ordinary branch obtains GetDt at`0x393784` and calculates each single-precision operation separately:

```
rate = speed * float_bits(0x41490fdb)       // 4*pi
seconds = float(unsigned_dt_ms) * float_bits(0x3a83126f)
max_delta = rate * seconds
delta = heading - current_z
```

If delta is strictly greater than float pi `0x40490fdb`, subtract float2pi `0x40c90fdb` once (`0x393864`). Otherwise, if strictly less than negative pi `0xc0490fdb`, add2pi once (`0x393808`). This is one correction, not a repeated normalization, and current Z itself is never wrapped.

The original clears delta's sign bit before testing **strict** `abs(delta) < max_delta` at`0x393824`. That branch copies the unadjusted heading and retains the turn flag (`0x39382c`). At equality or above, a negative delta subtracts max_delta and writes turn0 (`0x393858/0x39385c`); every other delta adds max_delta and writes turn1 (`0x39388c/0x393890`). Therefore zero dt and an exactly zero delta still execute the increment branch and can change the turn flag. Snap preserves target signed zero, while incremental arithmetic follows the original float imports.

All branches then test visual pointer+0x2d8 and, when present, invoke virtual policy slot0x70. SyncRotation executes only for a true policy. No direct sleep, force, angular-velocity or physical-angle write occurs in UpdateRotation.

## Live frame placement

The verified frame sequence is early scene/timeline/root displacement, game-state dispatch, genuine PhysicalWorld Step, eligible Character timers/AI/FSM/animation scheduling, then GameObject path -> rotation -> subobjects. UpdateRotation is called at`0x38cccc`, after UpdatePath at`0x38ccc4` and before UpdateSubObjects at`0x38ccd4`. Detailed load/pause/clock gates are in `../frame-order/NOTES.md`.

CSMove focus policy0x23c1 selects neither visual nor physics rotation source but enables visual-with-rotation. Its GameObject angle is therefore produced by this coordinator and consumed by visual SyncRotation; UpdateSubObjects must preserve it under that policy. A character's fixed collision-body angle can remain independent. Do not copy the physical-body angle into the actor angle unless the actual source policy requests that path.

## Verification and boundary

`tests/actor_rotation_differential.py` runs the actual original coordinator and actual Character speed/property/visual-policy and Application dt getters against compiled ARM64 native code. Only VisualObject::SyncRotation is observed at its boundary to compare visual pointer, call count and final Euler angles. It is not replaced by a claimed scene/quaternion implementation. The oracle covers5,590 cases, including property extrema, sentinel flags, zero/full32-bit dt, strict snap boundaries, signed zeros, angles outside one revolution and4,096 seeded cases. All state and request words match exactly;15 malformed contracts preserve input/output bytes.

`tests/actor_rotation.cpp` replays the same5,590 original-derived records and15 atomic caller rejections under host ASan/UBSan with leak detection. Both audits pass. Golden SHA256 `b77af2fe9f0bbf99c75a0ea1442c4fe497baed7ec2123593b1f586d2d4be9fe7`. Current ARM64 library SHA256 `1c950f5718c165b9aae02ac0dfdfc5f2456cc33ee45243ac5f1b915fc0432a45`.

This establishes the original rotation coordinator for validated finite native callers. It does not reconstruct virtual getter side effects, all FSM state producers, heading acquisition, scene clock initialization, or the entire frame scheduler. Arithmetic overflow from arbitrary caller values is rejected atomically; normal source property values and authored dt are covered by the original oracle. No packaged APK or renderer/CMake integration was changed for this module.
