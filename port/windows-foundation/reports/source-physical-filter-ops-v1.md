# Character source physical filter operations

## Evidence before implementation

The original v1.0.3 reference video is
`.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`.
At 285.2 s, a Bogwomp corpse lies on the floor at screen left while the player
and selected living Bogwomp continue the encounter. At 286.0 s, the living
target remains engaged and an on-screen `MISS` appears; at 286.8 s, the player
is still in combat beside the same visible corpse. These are direct visual
observations of the aftermath and continued fight. They do not by themselves
prove collision admission or identify the corpse's filter values. Video version
is v1.0.3; the recovered ELF/source below is v1.0.2.

IDA pseudocode and ARM disassembly in
`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/0046ece8.c`
and `0046ec6c.c`, plus
`port/level-world/reference/character-state/NOTES.md`, establish the physical
operations:

- `PhysicalObject::setFilter` at `0x46ece8` writes `(group, category, mask)` to
  the primary shape, then calls `b2World::Refilter` at `0x7e7afc`. It reads the
  secondary shape afterward and changes/refilters it only when the fourth bool
  is true. It clears owner byte `+0x26` after those operations. The Dead Focus
  caller `CSDead::OnFocus` at `0x3c4d50` first checks Character `physical2dc`
  and calls `(0, 0x51c, 3, false)`, so only primary is changed on this state
  transition.
- `PhysicalObject::resetFilter` at `0x46ec6c` restores the saved category,
  mask, and group words at owner `+0x20/+0x22/+0x24` into primary, refilters,
  then does the same for secondary if present, and finally clears byte `+0x26`.
  `CSDead::OnBlur` at `0x3c499c` null-checks the same Character `physical2dc`
  before calling it. `CSKnockedBack::OnBlur` at `0x3c48e0` and its `OnEvent`
  at `0x3c5ab0` also call `resetFilter`; KnockedBack `OnFocus` at `0x3c4a48`
  calls `setFilter(0,0x51c,3,false)` only when machine flag bit `8` is set.
- The caller path is `SM_SetDeadState` at `0x3c58c8` or a registered state
  transition through `_SetState` at `0x3c1938`; `_SetState` runs outgoing Blur
  and incoming Focus around the owner state assignment. The death selector
  chooses forced state dispatch or the existing event route. This filter API
  applies only after that state owner has admitted the transition; it does not
  choose death, remove a body, alter the PF obstacle owner, or change collision
  gameplay dispatch.

The recovered source therefore calls for two reusable operations with distinct
semantics: exact source `setFilter(group,category,mask,applySecondary)` and
`resetFilter()`. The former must not be implemented as zero-filter disable or
as a restore; the latter must always use the owner’s original saved filter.
The modern API validates the registered current actor at the pool boundary and
the assigned receiver/body/world at the physical owner. It keeps health, target,
action, transform, pin/mass, PF ownership and gameplay callbacks outside this
primitive.

## Implementation and verification

Added typed operations on `OriginalActorPhysical` and current-owner-checked
operations on `PlayableActorBodies`. Existing enable/disable behavior remains
the `native_physical_filter_v1` path. Source SetFilter changes and refilters
primary first; its optional secondary path is independent. Reset restores the
captured initial filter to each present shape in source order.

Focused verification is defined in
`tests/source_physical_filter_ops_tests.cpp`: actual Knight player and Lizard
enemy `ActorState` bodies in one isolated `PlayableActorBodies`/`NativeWorld`
fixture; exact filter words; repeat set/reset; `applySecondary` true/false;
disable → SetFilter → enable and disable → enable saved-filter interactions;
stale/missing owner and absent-body rejection; and rejection from inside actual
`NativeWorld::ShouldCollide` delivery. It also snapshots HP, target, action,
position, destination, native XY, mass, and pin state around each filter
operation. This fixture is not a `CombatSession` and has no RNG owner to inspect;
the filter methods have no RNG input or source call. The current character-body
owner creates one primary shape and has no secondary shape, so the bool’s
present-secondary branch cannot be exercised with this character fixture. The
exact conditional and ordering are source-derived; this gap is recorded rather
than manufacturing a second shape in the character object.

The test fixture uses actual cached character properties, AI rows, authored
bounds/body plans and Box2D bodies. `tests/run_source_physical_filter_ops_tests.ps1`
compiled current `OriginalActorPhysical`, `PlayableActorBodies`, and
`NativeWorld` sources with `-Wall -Wextra -Werror` in a private directory after
hash-checked copies of the prebuilt dependency archives.

Result: **PASS**. Exact filter data, repeated operations, disable/enable
interactions, stale/missing/absent owner rejection, and a mutation attempt from
inside actual `NativeWorld::ShouldCollide` delivery all passed. HP, target,
action, transform, destination, native XY, mass, and pin stayed unchanged. Log
SHA-256: `68BA2190A48E7E2EB1269AE705D8F0E0CFDAE7A63A130A34A46C5FD31E74435C`;
executable SHA-256:
`956D0875050B6005085D70AE96EA285A71879A9407971B9298CE97801E27A54B`.
This is an isolated native body/pool verification, not a production
`CombatSession` or UI capture. No full production Dead-state integration or
playable-feature claim is made by this primitive report.
