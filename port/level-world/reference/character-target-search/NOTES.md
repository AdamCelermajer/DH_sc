# Source target search: melee caller domain

The new `character_target_search.hpp/.cpp` reproduces the complete circular target-search traversal and heap for the actual `AI_DoMeleeAttack` caller: character filter flags `1`, game-object filter `0`. It borrows the real ordered room/object registry and object identity keys. It does not acquire targets through a nearest-enemy approximation or a physics broadphase. Other generic character-filter bits, rectangular searches, backup lists and arbitrary resort operations are captured/source boundaries rather than implemented exports.

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. This directory contains 31 primary routine captures plus four helper captures under `helpers/`, each bound to original instruction bytes. The original-only discovery report is `discovery-probes.json`; the O2 ARM64 and ASan/UBSan reports are `../../reports/character-target-search-arm64-differential.json` and `../../reports/character-target-search-host-audit.json`.

## Call chain and traversal

`TargetList::Search(float,float)` at `0x3d0020` obtains `ObjectManager` through the actual context global, then constructs a stack `RoomObjectList` over manager `+0x80`. `SearchEff` at `0x4a3428` obtains the heading vector first through `GetLookAtVec 0x393ae4`: `sin(rotation+0x174), -cos(rotation+0x174), 0`. It then obtains a pointer to the owner's selected target position through `GetTargetPosition 0x3935dc`. That method uses game XYZ `+0x160`, or cached target XYZ `+0x184` when the visual pointer `+0x180` and byte `+0x80` permit it. The pointer choice is retained; the pointed-to coordinate words are read for each candidate.

`Search 0x4a2f34` clears the existing heap through actual priority-queue pops, obtains the reference-character melee radius, then invokes registry Reset. The actual `RoomObjectList` vtable is `0x969d20`; its Reset/AtEnd/Next/Get/GetChar slots point to `0x4a18fc/0x4a17fc/0x4a18ec/0x4a1840/0x4a1cfc`. Room and object containers are intrusive doubly-linked-list storage, traversed through the next links. Each room carries an object-list sentinel. `_ValidateCurrent 0x4a18a8` skips empty object lists and advances rooms until the room-list sentinel.

The native projection needs only the next links and sentinel identities. Each room head is read when entering the room; each current entry's next link is reread after synchronous candidate callbacks. Null entries, self entries, duplicates, and empty rooms remain in traversal order. GetChar executes before null/self/invisibility rejection. The original CharHandle constructor/Get are captured, but their deeper handle-manager resolution is an explicit synchronous service in this coordinator.

## Filters and numeric gates

In source order, Search gets the object and resolves its character handle, rejects null/self/invisible (`object+0x8a`), invokes IsZonable, then rejects zonable objects that are zoned (`+0x2ee`) and outside their zone (`+0x2f0==0`). It invokes `IsInteractive(candidate,referenceObject)` next.

The flags1 character branch executes `_IsCharacterValid 0x4a1ab8`: a null reference character accepts; otherwise signed reference word `+0x1314` must be at least candidate word `+0x1310`. These words are named by offset in the API because their higher-level semantic producer is not recovered here. IsDead runs before `referenceAI.AI_IsEnemy(candidate)`. A living enemy nonplayer accepts. A player enemy accepts only when the reference character is not also a player. The native services preserve this short-circuit order and exact owner/target arguments. The noncharacter branch executes `_IsGameObjectValid 0x4a1950` filter0: `GetInteractionType(candidate,referenceObject)==8`.

For an accepted candidate, GetInteractionRadius runs before selecting its target-position pointer. The exact float computation is `sqrt((dx*dx+dy*dy)+dz*dz) - candidateInteractionRadius - referenceMeleeRadius`, using separate f32 operations. An adjusted distance strictly greater than search radius rejects; equality accepts. The melee caller genuinely supplies search radius zero, so this is an overlap/radius gate, not a center-distance-zero requirement.

`Point3D::angle 0x313058` calls `angleCos 0x312f40`: `dot/(length(delta)*length(look))`, then `acosf`, with no clamp or zero-vector special case. Search clears the returned float sign bit. The cone rejects only when `cone < f32(pi)` and `cone < angle`. Cone equality with pi disables angular rejection; zero-length vectors and unordered NaNs follow the original comparisons. Source debug assertion policy is a fixture boundary; this export does not reproduce debug log/assert notifications.

## Exact TargetInfo heap and reentry

Original TargetInfo has a 20-byte stride: object pointer, adjusted distance, angle, character-classification bit0, trailing zero. ARM64 `Target24` keeps the same fields with a 64-bit identity. Constructor `0x4a2730` maps sort0 to `_sortNoSort 0x38d568`, sort1 to `_sortClosest 0x38d570`, sort2 to `_sortFrontal 0x38d5b4`. No-sort always returns false, including classification. Closest/frontal first prefer character classification, then compare the distance/angle with strict greater-than. No secondary identity or stable-order key exists.

Push uses `0x4a2440 → 0x4a1f30 → 0x4a1e2c`. Pop uses `0x38fb18 → 0x38fa54 → 0x38f7cc → 0x4a24e4`. The adjust-heap path chooses the right child on comparator ties. The native implementation reproduces this algorithm; sorting the final records would not preserve source tie order. Original deque map/node allocation and all heap instructions execute in the oracle with bounded allocator storage fixtures (six original records per 120-byte node).

Nine actual original same-list nested searches are in the gold. A callback reentering Search clears/repopulates the shared heap, while the outer stack-local traversal resumes and adds its remaining candidates. Native callbacks run synchronously with the same list and registry; there is no event queue, snapshot of the heap, or generic reentry suppression. The tested visibility mutation also changes a future candidate before the outer traversal reaches it.

## Native ownership and integration

The public exports are `dh2_target_list_init`, `dh2_target_search`, `dh2_target_pop`, in `dh2::target_search`. Init reproduces reference-object character classification. Search uses the borrowed `Registry8` room sentinel, each room's `Entry16` sentinel, and caller-owned `Target24` heap. Pop copies the current top then applies source heap removal. Sort must be 0, 1 or 2. Heap capacity is explicit and bounded to 65,536; exhaustion returns provider-domain error2 after the already-visible search effects.

Every borrowed registry/object/character projection must be aligned, live and accessible throughout synchronous calls, including nested calls. The owner adapter must refresh true source field projections and provide actual backend identity keys; it must not supply accepted hostility/interaction results as defaults. `Object48.has_target_position` represents the conjunction of the original pointer/byte condition, not new visual ownership. No memory is allocated/freed or object ownership assumed by this module. Intrusive list lifetimes remain caller-owned. Outputs/list/heap must be disjoint as documented by the checked APIs; arbitrary dangling but aligned pointers cannot be validated by this kernel.

Services resolve the actual character handle, owner/candidate player/dead/zonable virtuals, interaction predicates/type/radius, owner AI hostility, and reference melee radius. Request.subject/other contain backend identity keys; character resolution returns a borrowed `Object48*`. Boolean/integer results use Response.word and radius results use Response.number. Providers return0 on a valid response. Valid null/self/invisible records still receive the character-resolution call. Unaligned object/head projections now return2 before dereference or character-resolution dispatch; sanitizer fixtures verify that behavior. Entry guard1 is atomic; malformed provider/topology/capacity2 may retain earlier effects. An empty Pop returns2 without mutation.

Connect list-create/reset-sort/search/pop/destroy services of `character_ai_attack` to this explicit state/registry after the caller has genuine registry and interaction backends. List destroy relinquishes caller heap storage; this kernel has no allocator ownership. Keep actor registry ordering from the actual source producer. Generic flags2/4/8/10/20/40/80 and object filters1/3 require separate APIs and proofs before enabling those callers.

## Proof and reproduction

Final original/O2 ARM64 and sanitizer gold: 412 searches, 26,580 ordered callback records, 1,480 accepted output records, nine same-list reentries. Host additionally checks 2,884 atomic entry/output-alias rejections and 1,236 provider/malformed-projection failures. ASan/UBSan reports zero findings. The corpus covers multiple heap nodes, no-sort/closest/frontal ties, mixed character classifications, cone/heading boundaries, strict radius boundaries, living/dead/enemy/player/interaction/visibility/zoning gates, signed character-field gates, empty rooms, null/self/duplicate records, nonfinite/negative-zero words, alternate target positions and callback mutation/reentry.

Finite output words and all non-arithmetic record words are exact; arithmetic NaNs compare by unordered class. Imported sinf/cosf/acosf use caller libm fixtures, not a claim of bionic-libm implementation parity. The host gold includes 37 recorded import argument/result pairs; the sanitizer replay actually executes 5,875 fixture import calls. The source sqrt and all f32 operations execute natively.

From repository root, build `port/level-world/tools/build_character_target_search_oracle.ps1`. Then use the configured Python/PYTHONPATH to run:

```
python port/level-world/tests/character_target_search_differential.py --library .local-inputs/character-target-search-oracle/libcharacter_target_search.so
python port/level-world/tests/character_target_search_host.py
```

For a parent CMake main-world target, add only `character_target_search.cpp` to the production world library and `tests/character_target_search.cpp` to a `character_target_search_audit` executable linked to that library. Keep `-ffp-contract=off`. The test exports real sinf/cosf/sincosf/acosf fixture symbols as well as `__wrap_*` variants. Shared-DSO calls require exporting the real fixture symbols (`-Wl,--export-dynamic-symbol=sinf`, likewise cosf/sincosf/acosf); wrapping only the executable's calls does not interpose a DSO. Run the source/binary-bound optional replay:

```
python port/level-world/tests/character_target_search_host.py --main-linked /home/adampalace/dh2-world-build/character_target_search_audit
```

This saves a separate `character-target-search-main-linked-host-audit.json`; it makes no compiler provenance claim from binary hashes alone. No CMake, renderer, APK or live runtime changes were made by this task. The current b449 checkpoint remains separate.

Frozen SHA256 bindings:

| Artifact | SHA256 |
| --- | --- |
| production .cpp | `12f4d36eb7826b1d6d36de6908ed42a1f4b9179a422022b43fac8b114dc9f0a6` |
| production .hpp | `38ce6f1b5742642abe5e79e222330264d2a67933dddfa38bb81569b40b2e9c8a` |
| search-fixtures.bin | `f48d4de780870ec72570ae37d36831c87e1b7c5351bb8e5f0b29c72889ccd9f9` |
| isolated O2 ARM64 oracle | `ab9a633ee05a91cdeded0d752c35d33a303cc0907ea05eb5dcbb1a93a3bc12ef` |
| sanitizer executable | `bdf499a76e4cc37faa8dee59fe8ccbb772d2f2137b57e6f56d1e9f555b1b1cab` |
