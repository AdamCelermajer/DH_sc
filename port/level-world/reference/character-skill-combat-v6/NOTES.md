# Active skill native combat V6 (candidate, not frozen)

V6 borrows one actual V6 player graph, its unchanged SessionV3, State56,
properties, timers, VM, actual GearV1 inventory, pinned design tables, shared
combat context/RNG, retained handle registry, and real target properties.
No alternate skill-script names or copied scratch property sheets are used.

The populated host composition executes all three starter Check callbacks,
native targeting, authored Use, actual Calculate, reciprocal retained-encounter
Aggro changes and HitFor HP subtraction. Pre stops at missing Cmd_LookAt before
UseMana; separately delivered Use is not a completed Pre-to-Use lifecycle.
An existing encounter relation is explicit initial storage input. Fresh
acquisition still requires the actual PlayerAIS OnAggro continuation.
The player-attacker HitFor path retains HP mutation then stops at required
global trophy-manager capture. Audio, AI combat/state reactions, trophy catalog
and unlocks, full Kill, online/co-op/player-target branches and external FX
owners remain required. No successful empty effect provider is accepted.
Full target application and full campaign are false.

AI_ReloadSkills uses the shared source-order coordinator: ascending non-null
script destruction, cell nulling after successful destruction, vector end reset,
same Save SG reload, configuration, then update. The V6 implementation owns
actual script strings/numeric payloads and releases their native allocations.
It preserves V3 public Session/state types but instantiates only V6 ownership.
ARM Arguments/Value pooled allocator byte layout is not claimed: native C++
ownership implements the recovered payload lifetime.

Save SG reload uses compiler-checked pointer-to-member access to the actual
PlayerSavegameV1 subobjects. It releases the old skill-vector allocation before
selection, retains the same Save identity, initializes exact selected rows,
then clears both slot maps in source order. No old SavedSkill view may survive.
Complete same-Save Load(mask8) is supplied by the parent-owned retained
PlayerSaveLoadOwnerV1, the sole profile authority. Slot=-1 alone never implies
a null retained stream. The populated SAN/O2 proof completes whole native reload
for all three actual player graphs with constructor-null profile and fresh slot,
then runs actual configure/update. It also publishes a retained nonnull profile
with slot=-1 and verifies the required section failure after Save reinitialization
and before configuration. Both builds pass 1,334 checks without sanitizer findings.

The differential proofs declare policy, allocation and external-boundary
observers separately from actual original instructions/native kernels. Host
proofs bind immutable historical DSO snapshots and check their hashes; they do
not attribute those libraries to moving current source. V3/V4/V5 frozen files
are unchanged.

Correction carried forward from V5: BashDown Pre has no executed global PlayFX
call. Its apparent call is commented source; its effect is authored animation
metadata. Required FX reachability must follow the executed source scripts and
animation callbacks, not that commented line. Frozen V4 NOTES remain unchanged.
