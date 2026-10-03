# Reconstructed aggression tables; captured target routines

Fourteen original routines are hash-identified in this capture. `aggro.cpp`
reconstructs Set/Add/Clear and Get/Highest/Count/Has/IsAggroed using caller-owned
native storage. It is compiled into the APK but is not yet connected to live
combat, target acquisition or enemy scheduling. `_UpdateTarget` and
`AISDefault::OnAttack` are captured evidence, not implemented routines.

The original outgoing tree header is CharAI `+0x7c`, with root `+0x80`, first
node `+0x84`, last node `+0x88` and count `+0x8c`. The incoming tree header is
`+0x94`, with count `+0xa4`. `AI_HasAggro` tests outgoing count; `AI_IsAggroed`
tests incoming count. These are opposite directions of the reciprocal relation.
Native tables use sorted 64-bit character identities and native pointers,
without copying the ARM32 node layout. Their caller must preserve character
ordering when equal positive threats are to match the original pointer tie.

The relocated PLT imports verify `0x30e2f8` as `__aeabi_fcmpgt`, `0x30eba4`
as `__aeabi_fadd`, and `0x30e3ac` as `__aeabi_fsub`. Highest starts at positive
zero and updates only for a strictly greater threat, so zero, negative and NaN
entries are retained but not selected. Equal positive threats select the first
entry in ascending character order.

Set rejects a player owner, dead owner or dead target. Otherwise it stores the
supplied float bits without clamping and updates both outgoing and reciprocal
incoming entries. A missing outgoing entry notifies the target's AI OnAggro
before insertion. Add returns Set's result minus the previous value when an
entry existed. Consequently, a gated Add can return negative previous threat
while leaving the entry unchanged; it does not simply return zero.

Clear removes both relations when the outgoing entry existed, then requests the
target's OnDeAggro. Independently, if that target currently targets this owner,
it requests target clearing and controller Stop, even for a missing relation.
Callback/target/controller backends remain service requests; the audit observes
them and checks their original order and arguments with nonmutating callbacks.

`tests/aggro_differential.py` executes original tree traversal, insertion,
rebalancing, erase, actual IsDead and all queries against compiled ARM64 source.
Its node allocator and IsPlayer query are caller fixtures. Imported IEEE
arithmetic is modeled: finite results and Set input bits compare exactly;
arithmetic NaNs compare by unordered class without sign/payload parity.
6114 cases include 675 IEEE boundaries, 5000 persistent-network operations,
13 explicit behaviors and 426 threat requests from original combat references.
The packaged ARM64 library is separately compared against the same records.
ASan/UBSan replay additionally checks atomic invalid-input/capacity failures.

Acquisition, decay, visibility/range rules, automatic target choice, pursuit,
combat callbacks, death cleanup and the complete AI/FSM still need separate
reconstruction and runtime integration.
