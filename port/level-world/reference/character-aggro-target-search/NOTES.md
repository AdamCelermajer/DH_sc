# Character aggro TargetList search

This bounded port reconstructs the candidate-search portion of
`CharAI::_UpdateAggro`; it does not claim to implement enemy AI or to be wired
into the Android game.

## Recovered source call

`CharAI::_UpdateAggro()` at ELF `0x3cf3f0` constructs
`TargetList(owner, 0x7fffffff, 2, 1)`. The tuple selects the all-relationship
sentinel path, Character-only objects, and closest-first sorting. It then
calls `SearchEff` with the selected AI row's view radius and a `2π` cone. That
cone does not narrow the search direction. The sentinel path still checks
`owner.Character+0x1314 >= candidate.Character+0x1310`; it bypasses the
narrower dead, enemy, player, and group checks.

Search walks the current circular room/object lists. For each candidate it
applies the Character-only filter, visibility, `Character::IsZonable()` and
the source `+0x2ee/+0x2f0` gate, `Character::IsInteractive(candidate, owner)`,
the adjusted-distance range test, and closest ordering. The adjusted distance
subtracts both candidate interaction radius and `CharAI::AI_GetMeleeRadius()`.
The service boundary supplies the actor-owned virtual results and borrowed
room/actor identities. The normalized records in the public header are adapter
projections, not binary overlays of game classes.

For filter 2, each accepted `TargetInfo` stores the source GameObject pointer
in its first slot and sets the Character flag. The separately resolved
Character pointer is used for Character-specific gates. In particular, a
consumer that classifies candidates or raises events must use
`TargetInfo.object_identity`, not the port-only `character_identity` field.
The parent integration should preserve this identity distinction.

The caller consumes closest candidates in pop order, then applies its own
friend/neutral/enemy classification and event 7/8/9 paths. The no-enemy
convergence is a separate branch: it tests `[CharAI+0x40]` and, when nonzero,
raises event `0xc` with that pointer as payload. Neither classification nor
these event paths are implemented by this search adapter.

## Evidence and validation

`original-functions.json` pins the complete SHA-256 of the recovered ARM32
library, 11 function instruction ranges, and both relevant vtable ranges.
These include `TargetList` construction/search/validation, the caller,
closest comparator, candidate virtual methods, and the exact Character and
GameObject virtual slots. The runner verifies every recorded byte range before
compiling.

Run from the repository root:

```powershell
python port/level-world/tests/run_character_aggro_target_search_host.py
```

The host fixtures check candidate filtering, borrowed service call order,
range boundary and radius subtraction, a full-circle search, closest pop
order, and adapter error handling. The current report is
`port/level-world/build/character-aggro-target-search/validation.json`.
This is host validation against synthetic registries plus pinned source
hashes. It is not ARM differential execution, live Android actor integration,
relationship classification, aggro storage mutation, script execution, or
controller movement.

The existing melee TargetList adapter uses a different tuple (flags 1,
object filter 0). It is neither modified nor reused here. Adapter capacity,
pointer validation, service errors, and opaque object lifetime remain explicit
port boundaries; a service failure after earlier candidates does not roll back
those service-side effects.
