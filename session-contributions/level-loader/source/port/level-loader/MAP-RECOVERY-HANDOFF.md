# Map recovery before runtime factories

This loader work fixes source-level map failures independently of canonical
actor/container construction. It changes only the attached loader worktree.
The original cache, original ELF, main emulator 5554 and menu emulator are untouched.

## Result and selection policy

`LevelPreparationV1` now follows the original `Level::_LoadProcess` failure route.
After one original generation attempt returns false, it replaces the first
resource-basename character with `x`, keeps the prefix before its first dot,
appends `_BACKUP.mlx`, and loads the authored backup. There is no invented seed
retry. The rule source and failed layout attempt remain retained in the prepared
borrow. Requested identity, definition and seed remain unchanged; the selected
backup is separately exposed through `Borrow::resolution()`.

Original ELF evidence: generation call at 0x3f7fac; failure branch at 0x3f7fb8;
suffix at 0x8c6b38; continuation to LoadFile at 0x3f729c. The cache contains
`x17_red_desert_cave_02_backup.mlx` and `x27_icy_cavern_02_backup.mlx`.

Two exact reference repairs are applied after original module projection:

| Absent original reference | Confirmed existing resource |
| --- | --- |
| `void_maze/mgp/vm011_corner_voidmaze_ne_00_01.mgp` | `void_maze/mgp/vm01_corner_voidmaze_ne_00_01.mgp` |
| `void_maze/mvp/vm01_x_voidmaze_nswe_00` | `void_maze/mvp/vm01_x_voidmaze_nswe_00.mvp` |

The overlay applies only if the authored resource is absent and the exact
replacement exists. Each repair records tile, property, authored URI, resolved
URI and reason in `ProceduralModulePlanV1::reference_repairs`. Authored XML,
original setter attempts, generation choices and module placements stay intact.
`repair_known_references=false` and `allow_original_backup=false` retain the
prior raw source path for original-reference comparisons. Both flags default
to true for the normal preparation facade; low-level original module projection
remains unchanged.

Unknown missing resources remain explicit failures. A missing/unreadable backup
is classified `original_backup_unavailable`; unsuccessful candidates retain the
previous prepared source. A failed repair leaves its module plan unchanged.
The backup route has ten stages; fixed and generated routes retain four and
eleven stages respectively.

## Static decor inspection

Void Maze's base module asset contains tiny detail meshes; its actual platform
meshes are authored `AnimatedDecor` declarations. The private preview now adds
their rigid meshes at retained source poses through `StaticDecorInspectionV1`.
Original declarations remain owned, and derived names never become save IDs.
`FixedMapV1::prepare_geometry_inspection` shares visual assembly but does not
create navigation: `navigation_prepared()` is false and the world is unsewn.
Normal `prepare` retains its existing floor build and post-load route.

This is explicitly static source inspection. Runtime conditions/templates,
unsupported class selectors, particle-only resources and skinned decor meshes
remain recorded as provider skips, never silently discarded runtime objects.
Missing resources and malformed data still fail the supported geometry path.
No animations, class defaults or gameplay visibility are claimed. The main
session confirmed no reusable complete AnimatedDecor owner exists yet.

The inspection renderer also checks a `pvr2_` filename variant when an exact
authored texture is absent. This resolves the supplied cache's compressed alpha
textures, records `TEXTURE_SOURCE_ALIAS`, preserves original material references
and retains real decode failures. Exact authored resources always take priority.

## Verification receipts

All current receipts are tied to the final sources/binaries in the checkpoint.
Historical receipts remain separately named for provenance.

- `loader-current-coverage.json`: all 344 source-assembly cases pass: 16 fixed
  maps; seeds 0..7 for all 35 procedural identities; seeds 0..31 for Desert Cave
  02 and Icy Cavern 02. 53 original backups and two reference repairs.
- `loader-current-original-mode-coverage.json`: all 86 frozen baseline cases
  still match with overlay and backup selection disabled.
- `loader-current-host.json` and `loader-current-sanitizers.json`: 20 recovery
  checks plus 14 lifecycle checks pass on final native builds. Includes every
  backup cancellation boundary, injected read failure, retry after discard,
  prior-map retention, retained provenance, both reference corrections,
  authored resource precedence and repair rollback. Address/undefined-behavior
  sanitizers run with leak detection; normal/sanitizer semantics match.
- `decor-inspection-coverage.json`: all 86 final-source native decor cases pass,
  including retained source/geometry after facade/cache teardown and failed
  inspection retaining the previous borrow. `decor-inspection-sanitizers.json`
  repeats 13 key cases including all six Void Maze variants and recovery areas.
- `map-recovery-native-android.json`: all 86 decor cases plus 20 recovery and
  14 lifecycle checks pass natively on Android x86_64. All six Void Maze cases
  contain substantial platform meshes. This is independent of host execution.
- The final source compiles for Android ARM64 and x86_64.
- `loader-recovery-preview.json`: final APK fixed/generated loads, reload,
  failed-load pixel retention and module focus.
- `map-recovery-preview.json`: final APK rendering audit over all 86 seed-0/1
  requests, source-selection markers and actual SurfaceView pixel checks.

The preview labels identify original backups and repaired references, and the
native log records `SOURCE_BACKUP_SELECTED` / `SOURCE_REFERENCE_REPAIR`.
Rendering receipts establish visible inspection geometry, not original lighting
parity or gameplay. The emulator is restored to SWAMP after the map audit.

## Remaining boundary

Castle 02 declares absent `gc02_corner_ws_01.mgp` in path list index 2. Its actual
root rule only uses indices 1, 4, 0 and 3, so this entry was never selected in the
bounded seed tests. No matching Castle 02 replacement is established; choosing
the Castle 01 gameplay variant would change chapter content. This unused source
defect is documented rather than substituted. Original generation rules are
not rewritten to avoid intended backup selection.

This checkpoint does not construct mobs/chests, evaluate runtime conditions,
execute quests/dialogue/cinematics, restore campaign state, or publish a live
game world. Canonical actor/container factory, registered templates/properties,
ObjectManager IDs, conditions/events and restoration remain main-owned provider
gaps recorded in OWNER-CONTEXT-REQUIREMENTS.md. SWAMP and SWAMP_02 still retain
separate level identities. The full loader goal remains blocked on those providers.

## Integration

Apply this incremental source/evidence handoff after the immutable
`source-pipeline-handoff-bd48491b34de1b76.zip` and
`level-preparation-handoff-8e58f2de9e440b61.zip`. Recompile consumers of the updated
request/provenance structs. The standalone loader CMake adds reference-repair and static-decor helpers
and their probes; shared engine source files are not changed.

The archive excludes the supplied cache/ELF, generated APK, campaign data and
diagnostic cache copies. It carries SHA-256 checked current sources and receipts.
No merge, push or public release is performed by this checkpoint.
