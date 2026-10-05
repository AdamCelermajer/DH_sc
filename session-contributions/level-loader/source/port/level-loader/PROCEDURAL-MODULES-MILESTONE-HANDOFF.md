# Generated module projection and private map inspection

This milestone adds original generated module overrides, native geometry and
selected MGP/MVP source assembly, and generated definitions to the visible
private preview. The generic loader goal remains active. Runtime mobs/chests,
condition evaluation, gameplay templates, campaign saves and shared factory
integration remain incomplete.

## Original evidence and scope

`tests/procedural_modules_original.py` executes original ARM Generate,
`Tile::SaveAsModuleXML` at `0x491d90` and
`Tile::SetModuleMVXProperties` at `0x491518`, including recursive naming,
float32 position calculations, file callbacks supplying unchanged MVX bytes,
original XML parsing and selected MGP/MVP path construction. It first checks
each generated hierarchy against the earlier original layout receipt.

Module construction, property registration/default loading, PropertyMap writes,
property serialization and destruction are explicit recorded service boundaries.
The oracle records attempted setters, including null values, and generated
non-null overrides. It does **not** prove the full original PropertyMap output,
default values, XML byte serialization or complete Level loading.

All 35 definitions at seeds 0/1 complete original projection. Native optimized
and ASan/UBSan comparisons match all 70 runs and 533 module occurrences.
Room names use traversal order and a generated index. Position strings use
the original float32 operations and six decimal places. The MVX reader selects
the first exact Module/GameObject and copies only scale, xrefmax, xrefobject,
dae, fog_color and is_solid. It retains parser diagnostics and ignores the
original parser return in this specific original caller. It does not invent
rotation or copy every MVX property. Selected gameplay/visual filenames remain
original room-list selections, with the original target/folder path construction.

Original inputs are bound to cache SHA256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`
and engine ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

## Native ownership and assembly

`procedural_modules_v1` retains the complete source/rule graph, generated
hierarchy, captured MVX documents, setter attempts and selected module overrides.
It publishes only checked candidates and retains previous output after failure.
Raw XML source ownership survives ZIP reader/facade teardown.

`procedural_map_sources_v1` is an explicit private inspection adapter. It creates
an owned derived Level document containing authored rule-root attributes and
generated overrides, then uses the existing FixedSources/FixedMap stages to
resolve original MGP/MVP dependencies, geometry, materials and navigation.
The complete original graph remains retained through a strong provenance owner.
This document is **not** claimed to be the original complete serialized XML,
an agreed factory ABI or a campaign save. Null setter attempts stay in the
module plan; they are not converted into invented property defaults.

Both host variants attempt all 35 definitions at both seeds: 66 map assemblies,
three original no-layout outcomes and one explicit missing dependency. Their
generated module names, position strings, xrefobject and dae are also compared
with the original module projection. Geometry indices, material ranges,
navigation, declaration retention, repeated preparation, source lifetimes and
failed-candidate retention are checked. The refactored FixedSources stage also
passes the existing 16 fixed map assemblies in both host variants.

`reports/procedural-declaration-types.json` inventories 7,543 retained
declaration occurrences across the 66 assembled selected runs: 17 types,
including Characters, containers, doors, triggers, checkpoints, spawn points,
quest zones, scenery and sound emitters. Reused declarations and both seeds
contribute separate occurrences; these counts are not unique active objects.

Known tested seed outcomes:

| Definition | Seed | Result |
|---|---:|---|
| DESERT_CAVE_02 | 0 | Original generator produces no layout |
| ICY_CAVERN_02 | 0 and 1 | Original generator produces no layout |
| VOID_MAZE_03 | 1 | Missing authored MGP dependency |

The missing URI is
`data/iphone/3d/modules/void_maze/mgp/vm011_corner_voidmaze_ne_00_01.mgp`.
Compiled XML path resolution still applies the verified original prefix/search
policy. This is not bypassed by silently discarding the gameplay placements.
These results cover the two tested seeds, not every possible generator state.

## Visible private preview

The app remains `local.dh2.loader`, AVD `DH2_Loader_API37`, serial
`emulator-5590`, with its own visible window and private build outputs. The
other sessions' checkouts, APKs, emulators and windows are untouched.

The picker exposes all 51 original level definitions: 16 fixed and 35 generated.
Known seed failures are labeled. Seed 0/1 selects generator-local state only;
it does not create a competing game save system. Preparation failure retains
the active map, source owner and GL resources. Successful switching releases
the previous candidate after the new buffers/textures are prepared. Reload
uses the active identity, definition and seed. Camera controls are inspection
controls and are independent of original map transforms.

The lighthouse asset explicitly binds primitive symbol `ColorMaterial` to
suffixed target material IDs. The preview follows its existing resolved
serialized instance binding instead of requiring symbol/target ID equality.
`reports/generated-material-binding-inputs.json` records the unchanged BRES
fields. This is a private renderer correction, not a shared scene-library edit
or a claim of complete original material/shader parity.

The final visible audit records 82 rendered map/seed cases across 50 original
definitions. Four failures match the three original no-layout outcomes and the
one missing MGP dependency above. ICY_CAVERN_02 does not render at either tested
seed; the other definitions render at least one tested seed. Nine small
overviews are checked with an additional focused module capture. This is map
geometry inspection, not complete authored scene or gameplay acceptance.

`reports/generated-preview-coverage.json` records cold preview attempts,
native frame counts, visible labels and screenshots for every definition and
both procedural seeds. Where an authored overview is tiny, a separate module-0
focus capture verifies the actual geometry without changing its placement.
`reports/generated-picker-checks.json` records in-process Swamp return seed
switching, reload, failed-load retention, cave/crypt/fixed Swamp switching and
camera controls. These are inspection transitions, not campaign transitions.
All 12 picker checks pass. Reload and failed-load retention compare viewport
pixels exactly. Fixed chapter-1 SWAMP is left visible.

`reports/procedural-modules-checkpoint.json` binds the final current sources,
four standalone build variants, native receipts, APK libraries, catalog and
visible checks. Earlier preview/layout checkpoints are historical milestones.
The final counts and verification status must be read from this checkpoint;
a submitted frame alone does not establish complete scene fidelity. The
executed cold-audit source is retained as
`reports/generated-preview-audit-source.py` and bound to its recorded hash.
The current audit tool also distinguishes a completed audit from a resumable
partial audit so a repeat cannot silently archive images and skip every case.

The private map stage replaces selected source-root TRS with the projected
module TRS. Complete original root-transform composition still requires
verification, particularly for assets whose source root has a different scale.
Focused captures establish that geometry renders; they do not settle that
original placement policy. Original lighting/effects and helper visibility also
remain unresolved.

MVP/MGP scenery declarations such as Decor and AnimatedDecor are retained but
not instantiated by this map-only renderer. Some authored modules therefore
show primarily their ground geometry. Rendering their module mesh is not proof
that all authored scenery or the complete level is visible.

Two emulator processes exited during earlier audits; their cause
is not established by the app logs. Only this owned AVD was relaunched. The
audit now saves a hash-bound per-case checkpoint and can resume completed
cases after a device interruption. The launcher now uses detached/process-group
flags and an accepted breakaway-from-job flag, recorded in its private launch
receipt. This does not prove the cause of the earlier exits or establish
long-duration emulator stability.

## Reproduction

Run from the private checkout with bundled Windows Python. Substitute the
canonical cache path and supplied original ELF path. `--probe` paths in these
scripts are Linux paths to the private WSL builds.

```text
python port/level-loader/tests/procedural_modules_original.py --engine <original-ELF> --dependency-root ../dependencies --cache <canonical-cache> --original port/level-loader/reports/procedural-layout-original.json --out port/level-loader/reports/procedural-modules-original.json
python port/level-loader/tests/procedural_modules_differential.py --probe <host-xml-probe> --cache <canonical-cache> --original port/level-loader/reports/procedural-modules-original.json --out port/level-loader/reports/procedural-modules-host.json
python port/level-loader/tests/procedural_modules_differential.py --probe <host-sanitizers-probe> --cache <canonical-cache> --original port/level-loader/reports/procedural-modules-original.json --out port/level-loader/reports/procedural-modules-sanitizers.json
python port/level-loader/tests/procedural_map_coverage.py --probe <host-xml-map-probe> --cache <canonical-cache> --inventory port/level-loader/reports/fixed-map-level-coverage.json --out port/level-loader/reports/procedural-map-host-coverage.json
python port/level-loader/tests/procedural_map_coverage.py --probe <host-sanitizers-map-probe> --cache <canonical-cache> --inventory port/level-loader/reports/fixed-map-level-coverage.json --out port/level-loader/reports/procedural-map-sanitizers.json
python port/level-loader/tools/build_preview.py
python port/level-loader/tools/preview_device.py install
python port/level-loader/tools/audit_generated_preview.py
python port/level-loader/tools/audit_generated_picker.py
python port/level-loader/tools/check_visible_preview.py
python port/level-loader/tools/capture_procedural_modules_checkpoint.py
```

The visible audit intentionally launches/stops only this private app. It checks
the AVD before every audit and explicitly targets its serial. The picker audit
leaves fixed chapter-1 SWAMP visible. Repeated cold audits archive the previous
APK's evidence instead of mixing its screenshots with a new APK. Both native
Android ABIs compile; runtime rendering is verified on this x86_64 emulator.

## Integration still required

Review `INTERFACE-PROPOSAL.md` with the main session before shared integration.
The main session must provide original property/default/template processing,
generic factory creation and rendering for all authored object types, condition
and event services, persistent save restoration and release/lifecycle services.
Loader data retains dialogue/cinematic/quest references; the main session owns
execution and gameplay effects. Menu presentation remains with the menu session.
The generated room index is not an agreed persistence key. SWAMP and SWAMP_02
remain separate identities even when sharing the swamp BDAE and placement pools.

Chapter-1 SWAMP currently retains 50 Character and five OpenableContainer
declarations. They remain uninstantiated and unrendered. Swamp return seed 0
assembles 11 modules, 497 mesh instances, 60 Character and two OpenableContainer
declarations. No object is silently promoted from a conditional declaration
to an active runtime object. No shared merge, push or integration edit occurs
in this milestone.
