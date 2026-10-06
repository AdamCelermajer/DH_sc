# Same retained Level and GSLevel current-global contract

`CanonicalLevelContextV1` is the loader's single retained logical game-Level
receiver for the presently connected source fields. It is distinct from the
LevelConfig and Module objects. Its stable identity is the native receiver's
address, never a selected-level name, source index, module ID or registry key.
Root must adopt this SAME receiver in its application/runtime composition.
Creating a separate Kill-only receiver or copying gate150 is not this contract.

The receiver retains the selected Level source request and its provider lease.
A matching `LevelPreparationV1::Borrow` may attach later, retaining original
assets and fixed/generated provenance. SWAMP and SWAMP_02 identities remain
separate even with shared geometry. Attaching prepared source does not publish
a world or claim gameplay readiness.

## Actual constructor field producer

The original LevelC1 is `0x3f3128`, symbol
`_ZN5LevelC1EPKcijjjbbii`. Direct source stores initialize:

| Field | Native storage / borrowed input | Original store | Value |
|---|---|---|---:|
| config38 | Level receiver uintptr field | 0x3f319c | 0 |
| music11c | Level receiver int32 field | 0x3f3244 | -1 |
| safezone120 | Level receiver int32 field | 0x3f3248 | -1 |
| ambient124 | Level receiver int32 field | 0x3f324c | -1 |
| word150 | SAME receiver's KillLevel16::loot_gate150 | 0x3f3270 | 0 |

There is only one writable word150. `source_word150()` references exactly the
field returned by `kill_level()`. That KillLevel16 view lives inside this same
receiver, with identity equal to the receiver and ABI reserved field zero.
The constructor value is a captured declaration, not a loot suppression policy.
Root still supplies real DropLoot, XP, quest, death and AI effects when reached.

The original ARM prefix executes through the word150 store across eight
poison/input cases. EventManager, LuaScript and std::string constructors are
declared fixture boundaries in this bounded oracle. The native field values
match those actual direct stores. Full LevelC1 event/script/table/online/save
construction and subsequent Level initialization are not implemented or
accepted by this field checkpoint. Their state/continuations must extend this
same canonical receiver rather than create duplicate Level authorities.

## LevelConfig binding

Link `dh2_loader_canonical_level_context`. `level->config_fields()` pins this
same Level and returns direct pointers accepted by root's exact
`LevelConfigPublicationBorrowV2` contract:

```cpp
auto fields = level->config_fields();
dh2::world::LevelConfigPublicationBorrowV2 publication{};
publication.level_owner = fields.level_owner;
publication.config38 = fields.config38;
publication.music11c = fields.music11c;
publication.safezone120 = fields.safezone120;
publication.ambient124 = fields.ambient124;
// Supply the SAME actual Arrays::Sounds lease/catalog and canonical config lookup.
publication.arrays_owner = actual_arrays_lease;
publication.sound_names = actual_sound_names;
publication.config = actual_canonical_config_lookup;
// Use root's real level_config_publication_v2 at its original InitPost point.
```

No sound IDs or config identities are invented. The loader does not implement
root's publication helper. All four supplied root contract headers were read
and matched to the exact announced SHA256s before pinning; the copies under
`vendor/level-context-contracts-20261005` are contract evidence, not a standalone
root subsystem or override include path. Whole root dependencies and source
helpers stay owned by root.

## GSLevel current global, Application getter and Kill binding

Original `Application::GetCurrentLevel` at 0x31f594 reads
`GSLevel::s_level` at 0x9a2638 via GOT cell 0x9967fc. It does not read an
Application member. `CanonicalGSLevelGlobalSlotV1` borrows the ONE authoritative
retained global storage and pins its `globals_owner` provider. Root must pass
that actual global, never a new Application-owned slot or a loader mirror.
The loader owns no singleton, parallel slot or publication policy.

Source GSLevel Ctor at 0x386190 allocates the original 0x1ac Level and calls
LevelC1 at 0x386200. After that call, the SAME pointer is written to GSLevel
field+34 at 0x386214 and s_level at 0x386218. Dtor executes actual virtual Level
destruction at 0x386118, clears field+34 at 0x386120, then clears s_level at
0x386130. Those instructions are captured in gslevel-current-original.asm;
the full Ctor/Dtor were not executed by this checkpoint. Root owns their
remaining source lifecycle and publication/destruction ordering. The shared
native lease is an adapter lifetime choice, not proof of original destruction.
Use the SAME receiver for state-owned Level fields and the global projection.
Prepared source attachment does not publish a candidate to that global.

Six executions of the original Application getter, including null and unmapped
Application arguments and changed/null Level globals, returned the actual
s_level value. The original GOT cell is unchanged; its exact R_ARM_RELATIVE
addend matches the original global symbol at the oracle's ELF base zero.
No Application or getter implementation was mocked.

For each `kill_current_level` request, call
`borrow_current_canonical_level_v1(actual_slot, scoped_borrow, error)`.
Return service failure if the GSLevel global slot/lease is unavailable. Otherwise
return0 with `response.pointer = reinterpret_cast<uintptr_t>(scoped_borrow.kill_level())`.
A genuinely empty s_level global produces null, preserving Kill's existing explicit
unsafe-null-Level result; it never creates a default Level. Keep the borrow
alive throughout Kill and its synchronous callbacks. Query again on each source
request; retain earlier borrows until their last use instead of caching a scalar
or releasing a still-borrowed Level on a slot change. Asynchronous quest effects
remain root-owned and must retain their real required Level/event lifetimes.

Source loading, LevelConfig fields, Kill's gate read and quest Level identity
thus share the same receiver. Field mutations remain live through a held borrow.
A current-Level change affects later queries without invalidating existing
borrowed fields. These are lifetime adapter choices, not proof of original
GSLevel construction/publication/destruction ordering or full world restoration.

Provider leases must not own this Level or its enclosing aggregation. Returned
borrows must not be stored in the GSLevel global owner/Level they pin. Root should use
separate provider lifetimes and scoped call ownership, avoiding shared_ptr cycles.
Slot/receiver access stays sequential on the owning runtime thread.

## Verification

Thirteen Level-context checks pass in normal host, ASan/UBSan/leak and Android
x86_64 executions; ARM64 compiles/links. They check the original stores, one
native identity and writable gate, live config/gate fields, actual-global query
semantics, null/missing slots, lifetime across slot changes, rejected source
identity, immutable-source attachment, atomic invalid input and cycle release.
GSLevel global/provider and service lifetimes in these probes are declared fixtures.
No live Kill, loot award, quest, full Level constructor or level commit is claimed.

The tested map APK remains unchanged, and emulator5590 still shows its prior
map. Main/menu worktrees and emulators5554/5580 were untouched. This is a field
and identity handoff for root integration, not complete SWAMP acceptance.

## Replacement scope

This replaces the earlier Application-member wording/API in handoff
9d944cd1680bb71c. The old CanonicalApplicationLevelSlotV1 name is removed;
use CanonicalGSLevelGlobalSlotV1 { globals_owner, s_level }. Application is a
reader of GSLevel::s_level. The retained Level, source fields and KillLevel16
projection remain the same receiver. Main must rebuild consumers of the changed
header; no live Kill/loot/quest acceptance is added by this correction.
