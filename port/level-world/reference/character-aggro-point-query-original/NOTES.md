# Original TargetList point-query replay

This fixture now executes the pinned ARM32 `TargetList::Search(Point3D const&,
float, Point3D const&, float, IObjectList&)` body at `0x4a2f34` (1,172 bytes),
along with the original `TargetList` constructor, Character validation,
GameObject filter, vector-angle helpers, priority-queue insertion/pop, and
`GameObject::GetTargetPosition`. It is distinct from the earlier host-only
TargetList adapter checks.

Run from the repository root:

```powershell
python port/level-world/tests/run_character_aggro_point_query_original.py
```

The runner verifies the whole original library SHA-256 and every source range
listed in `original-functions.json`. It independently executes the ARM import
thunks to verify the exact float arithmetic/comparison imports used by these
instructions. Arithmetic is rounded to IEEE binary32 after each operation.
The ARM corpus is written under
`port/level-world/build/character-aggro-point-query-original/`. The runner also
compiles the linked portable query against the same serialized actor/list
facts; the host executable, binary fixture, output, and comparison report are
under `port/level-world/build/character-aggro-point-query-differential/`.

The live source query is fed a borrowed, circular `IObjectList` fixture. Its
vtable methods read the list links at call time; the mutation case changes a
candidate's `next` pointer inside `Character::IsInteractive` and confirms that
the source loop follows the changed link. Source instructions decide the
visibility, Character filter, source zonable bytes, all-flags property gate,
range adjustment, angle threshold, insertion order, and closest-first pops.
Five focused cases cover those gates, heap growth past the six-entry first
block, the optional `GetTargetPosition` position, a negative interaction
radius, cone boundaries, live link mutation, and an empty list.

The source query's angular field is the computed `Point3D::angle`: a `0.5`
radian case accepts the aligned candidate and rejects perpendicular and
opposite candidates. The `2π` query used by `_UpdateAggro` takes the source
full-cone branch. Adjusted distance is the original 3D distance minus the
candidate interaction radius and the owner's `AI_GetMeleeRadius`; the source
range comparison is then applied.

## Supplied boundaries

The fixture supplies bounded storage at the original allocator/free entries,
the owner constructor's dynamic-character result, the borrowed `IObjectList`
virtual methods, and actor-owned `IsZonable`, `IsInteractive`, interaction
radius, and AI melee radius results. `DebugSwitches::Load/GetSwitch` are held at
the explicit no-active-diagnostic-switch boundary so debug rendering/logging
does not run. The point-query and candidate-filter instructions are not
short-circuited. The original float import identities are checked before their
results are modeled.

## Portable host differential

The runner serializes the same five actor and circular-list fixtures into a
binary input for `character_aggro_point_query_host.cpp`. That executable links
the production `character_aggro_target_search.cpp` and
`character_aggro_character_list.cpp`, enters through
`dh2_aggro_target_search_object_list`, and pops its actual `TargetList` heap.
The gate compares every popped object identity/index, distance and angle bits,
flags, reserved word, and the ordered actor-service/mutation trace against the
ARM results. All five cases pass exactly, including heap order, mutation during
`IsInteractive`, and the non-Character candidate that still receives
`IsZonable`/`IsInteractive` before being rejected before radius lookup.

The active diagnostic/debug-switch override path is explicitly excluded: both
oracle and comparison use the original no-active-diagnostic-switch boundary.
It needs real debug-switch providers before it can be tested.

This is not proof of live Android actor state or runtime manager enrollment.
The `IObjectList` fixtures exercise the source interface shape but are not
binary overlays or snapshots of Android `CharacterList` memory.
