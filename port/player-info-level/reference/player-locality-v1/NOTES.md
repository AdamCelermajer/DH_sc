# Borrowed Player locality V1

Source base: `46a8d1c80a6a58396f403f466c42b71f752a8203`, with Adam's
`791e961b12233100b303038c961666834f4beb9d` initialization/manager evidence.
The exact original ELF identity and complete function hashes are pinned in
`original-functions.json`. No original binary or byte payload is included.

## Sole owners and source order

The module borrows the existing `player_manager_host_level::PlayerRegistry`
and `PlayerInfoProjection*` identities. It does not add a registry, player,
save, VM, property sheet, RNG, timer, or network owner. The native caller owns
canonical Character+660 and CNetPlayerInfo member+1a0 fields. Every required
field/provider read is fresh at the original call site.

`GetPlayerByCharacter` first reads Online+5. Its offline branch walks the
internal-ID registry in ascending order, reading each PlayerInfo+660. The first
match wins, including a null Character queried directly; a miss returns the
same embedded manager+8 PlayerInfo. `IsLocalPlayer(null)` returns zero without
lookup or provider calls. Otherwise it passes lookup flag zero, selects that
exact PlayerInfo, and dispatches its virtual+50. The inherited CNet leaf is
explicitly reconstructed; offline is never substituted for locality.

The network branch reads game-state+24, acquires Matching and calls virtual+64,
then acquires NetPlayerManager/IsInitialized. MatchingLocal virtual+64 is
**IsInRoom**, not IsHost. It requires genuine room/GameSession services if
reached. The manager's ID vector is read initially and freshly after misses.
Each candidate is probed through GetPlayerByInternalID. A hit reloads the
vector's matched index and tailcalls `_GetNetPlayerInfo`, a distinct source
callee. These network providers remain explicit mandatory contracts.

`CNetPlayerInfo::IsLocal` acquires Matching, calls the actual IsServer kernel,
then reads PlayerInfo member+1a0. A server with a negative info member returns
one immediately. Every other path reacquires Matching and dispatches virtual
GetMemberId, comparing it with the captured info member. Changing the singleton
or info field during that second acquisition cannot replace the captured value.

IsServer reads active byte **+0xc**. Zero returns false without member calls.
Otherwise GetMemberId is read once for the negative gate, then freshly a second
time, then GetServerMemberId is read and compared with the second value. Local
member leaves read the actual +3638/+363c fields.

## Constructor projections and fallback distinction

`construct_matching_local_fields` projects only three genuine reached stores:
base active+c=0, Local member3638=-1, and server363c=-2. The original Local
constructor subsequently calls Reset. `reset_matching_local_ids` projects its
server=-2 then member=-1 stores; the whole pinned Reset has no active+c store.
These helpers are not complete CMatching/Local constructors or Reset: member
records, NetStruct assignments, mutexes, collections and transport lifecycle
effects are not fabricated. Full providers are required when those are needed.

Source PlayerInfo.Reset calls CNetPlayerInfo.Reset at 373c0c; its member+1a0
store is -1 at 80f294, with conditional SetChanged delivery. PlayerInfo then
stores Character+660=0 at 373c24. PlayerManager's constructor creates its
embedded fallback and empty tree/vector. An empty manager's genuine fallback
can therefore return local because -1 equals -1, while its Character remains
null. `Result.registered` is false for this fallback and true only for an
actual canonical registry hit. Network lookup has a distinct route and does
not establish registration in the offline manager tree.

GetMatching borrows the actual singleton/provider storage. An existing singleton
returns unchanged. Provider0 writes1 before mandatory Local construction. Exact
allocation sizes and GL modes are passed to the constructor provider. Fresh
provider reads after construction preserve the original Local/BT/GL(false)/
GL(true) chain. Unsupported provider values leave the source null singleton;
subsequent query use fails explicitly rather than manufacturing an owner.

## Failures and bounded proof

Missing reached callbacks, thrown callbacks, invalid projections and failed
delivery stop with the reached result/trace prefix. No source effect is rolled
back, no cleanup is invented, and no failed read grants an association. Query
results are caller-owned and must be disjoint from all canonical fields and
provider-owned storage. One owning thread holds the borrowed registry/identity
graph stable during a query; callbacks may mutate the fresh field/provider
values as the source does. The 4,096-entry limit and alias/projection guards
are explicit port boundaries, not new original conditions.

The original replay executes complete locality/lookup/GetMatching callers and
both Local member leaves. Online/room/network and allocation/derived-constructor
callees are declared fixtures. Base constructor scalar stores execute with 32
member-record constructor fixtures. The Local constructor prefix and Reset ID
window are bounded projections, not full-body implementations. Tests compare
exact values, selected identities, source call/field traces, fallback routes and
fresh mutation behavior, plus missing/failed/throwing service prefixes.

The runner links the actual central selected `dh2_level_world` shared library
and existing host-level TU, with no staged or directly compiled module copy.
It discovers actual Ninja object/header/CMake inputs, snapshots them, performs
a clean selected rebuild, compares original results and verifies unchanged
hashes/commands/results. This host gate does not prove native wiring, profile
load, Player registration, full Matching construction or gameplay.

```
C:/Python313/python.exe port/player-info-level/tests/run_player_locality_v1_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf .local-inputs/libDungeonHunter2.so --output C:/Users/noamc/Documents/Codex/.checks/locality-selected
```
