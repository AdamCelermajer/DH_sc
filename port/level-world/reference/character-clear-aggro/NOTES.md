# ClearAggro Lua wrapper and ownership discovery

NEW original-only discovery; no frozen production files are changed. `tests/character_clear_aggro_original.py` executes original instructions and saves `source-probes.json`. Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`; eight-function manifest SHA256 `828ef4a1e85e3cd493879a44dfc85730b27ea9d1d4f81339a69f0133fe39a1fb`.

## Lua callback

The actual global/method registration is `Character::_ClearAggro` at `0x3b8648`, 96 bytes, function SHA256 `0b3b77e4d0150abb135db50c152dba9803d47127a0c8d7a3f024e347a58ceed3`. It obtains the source Arguments vector count using the 144-byte Value stride. Zero arguments return immediately; this is **not ClearAllAggro**. It accepts only first Value types `2` or `7`, ignores additional arguments, and calls actual `Value::getUserData` at `0x31b5a0`. That method reads type at `+4` and identity at `+0x6c`; accepted null identity remains null. The wrapper captures Character's embedded AI (`owner+0x3c8`) before conversion and tail-branches to `AI_ClearAggro`. ReturnValues is untouched: no results are added.

## Complete native ownership choreography to preserve

`CharAI::AI_ClearAggro` at `0x3d6d68`, 388 bytes, SHA256 `d59c80b01af4f6b38ff7a70f4eb21a3382a04edab27f4f6b51394c221f568040`:

1. Capture the receiver and input Character. Null input returns immediately.
2. Search the receiver's outgoing map at AI `+0x7c` (`Character+0x444`), using unsigned Character pointer ordering. If the relation is absent, skip reciprocal removal and OnDeAggro, but still reach the target ownership check.
3. For an existing relation, erase that outgoing node through original RB erase `0x3d5d9c` / rebalance `0x336004`. Its count is decremented and the original allocator deallocation service invoked.
4. Search the captured target's incoming map at Character `+0x45c`. Its key is the **currently loaded** receiver owner at AI `+4`. Erase that reciprocal node when found. Missing reciprocal relation does not suppress notification.
5. Call captured target's embedded AI virtual `+0x3c` (`OnDeAggro`) with the receiver's reloaded owner. Outgoing and reciprocal removal precede that synchronous notification. Its return is ignored.
6. At the common tail, reload receiver owner and captured target's current target (`Character+0x408`). If unequal, return. Equality applies even when no outgoing relation existed.
7. Call target embedded `AI_SetTarget(nullptr,false)` at `0x3d6890`, then reload target controller (`Character+0x378`) and call `v2Controller::Cmd_Stop` at `0x40559c`.

Notification may synchronously replace the receiver owner or clear the captured target's current target, thereby suppressing the later SetTarget/Stop tail. Replacing the target controller before that tail changes the controller identity actually dispatched. Callback/provider lifetimes therefore cannot be collapsed into precomputed flags. The actual Cmd_Stop body separately applies source forced/global-blocked/locked gates and owner virtual dispatch; this discovery treats that complete downstream body as a service boundary, not automatically accepted movement.

## Original instruction evidence

The new probe passes **1,792 original-only cases / 200 ordered notifications and tail calls**. It covers Value types `0..8`, counts `0/1/3`, null/non-null identities, outgoing and incoming relation presence independently, target ownership, and three synchronous mutation modes. All ReturnValues sentinel bytes remain unchanged.

Original SetAggro creates maps through actual map insertion/rebalance instructions, including unrelated keys on both sides of the removed key. Original erase/rebalance executes for every removal. A partial relation fixture removes only one side through actual erase before the tested call, proving that absent outgoing leaves an incoming-only relation untouched. The probe records state at each callback, showing removal before OnDeAggro, source owner reload suppression, and controller replacement at the subsequent Stop. Allocation, IsPlayer and notification/SetTarget/Stop bodies are explicit services. There is no native/ARM64 parity or whole-world ownership claim in this report.

Reproduce from repository root with configured Python/Unicorn dependencies:

```powershell
python port/level-world/tests/character_clear_aggro_original.py
```

## Existing aggro primitive reuse

`port/game-data/aggro.cpp::dh2_aggro_apply(...,aggro_clear)` already implements the source outgoing/reciprocal storage removal and exposes notification/target-clear/Stop request bits. Its existing isolated instruction proof is useful. For a new synchronous coordinator, invoke that storage operation with static facts **not predicting the target-clear tail**, deliver OnDeAggro when requested, then evaluate the actual live receiver owner versus target current target, execute genuine target clear, reload the controller and invoke its real source command. Calling the old primitive once with precomputed `aggro_target_targets_owner` and then blindly consuming all request bits would miss the observed source reentry/reload effects. Native storage bounds/alias validation must stay explicit; arbitrary aligned pointers do not prove source object lifetime.

The VM bridge must be scoped to the existing same-VM callback capability and real source-object projection. This report does not add a fake successful ClearAggro closure to production. The prior host target-pipeline audit retains its explicitly labelled temporary suffix fixture until a separately proved native wrapper/coordinator is available.
