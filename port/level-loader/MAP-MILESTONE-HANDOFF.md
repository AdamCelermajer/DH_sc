# Generic loader map milestone — review draft

The isolated loader now reads original fixed source graphs, assembles module
geometry/materials/navigation and renders Swamp map meshes. The complete loader
goal remains active. No merge, shared integration or runtime factory ABI is
approved by this draft.

## Source and evidence

The core changes are under `port/level-loader`: owned XML/source preparation,
compiled resource-path policy, fixed map assembly, visual transform projection
and UserProperties decoding. Standalone CMake reuses unchanged ZIP, BRES,
scene/math and floor/navigation kernels. Original-engine captures and
differential receipts distinguish checked behavior from pending semantics.

Coverage attempts all 51 original level-table rows: 16 fixed definitions
assemble, 29 require original procedural generation and six remain blocked by
original malformed XML. Swamp retains 19 XML documents, nine modules, 50
Character declarations and five OpenableContainer declarations. Conditions are
retained; these counts do not establish the active object population.

Swamp assembly contains 369 map mesh instances, 16 native floors and 626 nav
triangles. Original ARM comparisons pass 138 visual-transform cases and 89
UserProperties cases. Swamp retained-owner checks pass ASan/UBSan. Standalone
and preview targets build for ARM64 and x86_64.

The private Android preview submits 384 draws and three textures. All nine
module views, three reloads, failed-load retention and Home/resume are captured.
`reports/swamp-map-preview.json` binds the APK and screenshots;
`reports/map-preview-checkpoint.json` binds native sources, embedded canonical
ZIP and both packaged ABIs. `reports/swamp-map-overview.png` shows the map.

All 16 assembled fixed definitions also have native frames and screenshots in
`reports/fixed-map-preview-coverage.json`; separate pixel checks confirm
nonblank map viewports and their visible identities. The remaining 35 rows
retain their source blockers. `SWAMP_02` is rule-driven
(`022_swamp2.rule.xml`), remains a separate identity and is not verified by this
fixed-level milestone.

## Ownership and integration boundary

`FixedSourcesV1::Borrow` retains raw documents/declarations and dependency links.
`FixedMapV1::Borrow` retains those sources, BDAE bytes/views, assembled scene,
materials, module records and native navigation. Borrows outlive mutable facades
and the ZIP reader. Failed candidates preserve prior published snapshots.
Native navigation query scratch is mutable and requires sequential use.

`INTERFACE-PROPOSAL.md` still requires agreement. Main supplies real object
factories, template/condition services, event registration and restoration.
Main owns AI/path algorithms, combat/skills, animation execution, loot,
inventory, quests, dialogue/cinematic execution and persistent save state.
Loader owns declaration preparation, resource retention and level lifecycle.
Menu owns loading presentation. Runtime identity and transition/save ordering
must follow original evidence before these boundaries are connected.

A later, separate internal preparation milestone adds
`FixedDeclarationsV1::Borrow`: 2,331 declaration occurrences from all 16 fixed
definitions compare with independent original XML reads. Each occurrence
retains raw attributes/nested elements and module context; names and separate
template fields remain unchanged. This is not the shared factory/save ABI.
`reports/swamp-fixed-declarations.json` contains the full Swamp source index.
`reports/fixed-declarations-host.json` binds the new native probe and sources;
the older map/preview checkpoint hashes describe their original milestone.

## Private preview changes

`android/loader_preview.cpp`, `LoaderPreviewActivity.java` and the private app's
CMake/Gradle/manifest select an inspection target with `local.dh2.loader`.
Only `DH2_Loader_API37` / `emulator-5590` is used. Renderer camera/shader are
inspection adapters; original lighting/effects are not reconstructed here.
Unclassified helper geometry remains explicit. These app settings are private
verification scaffolding and must not overwrite the main app's integration.

The managed worktree contains inherited uncommitted changes. Review core files
against `../baseline-snapshot.json`; do not merge or stage the entire worktree.
The main checkout and other emulators have not been modified.

## Pending acceptance

Swamp mobs and chests still need real factories, original template resolution,
condition decisions and visible rendering. Entry/exits, other object types,
event/save services, chapter return visits and transitions remain incomplete.
Procedural rules, remaining parser/caller policy and full original scene policy
also remain pending. A passing map preview does not establish any of these.
