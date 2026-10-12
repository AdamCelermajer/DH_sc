# Projectile source linkage

Append the completed Projectile family to the existing native noncharacter source attachment. It uses the same target, include directories, source-existence checks, and source-path deduplication. No new canonical owner or library is created.

Copy catalog_projectiles_v99.hpp/.cpp, canonical_projectile_v99.hpp/.cpp, and projectile_resource_services_v99.hpp/.cpp from ../projectile-canonical-source-v99 into Main port/level-loader before adopting this patch. Keep Main property-map and resource/spawn enrollment changes; the separate property-map patch adds only the original inherited declarations.

The existing additional_parts_v101 seam accepts the family Part before the sole production catalog is created. CatalogProjectilesV99 retains the record and its release receipt. SourceReleaseJournalV69 invokes the record's genuine destroy_source and retire_storage_after_source_release_v91 callbacks; storage expiry does not invoke a native destructor.

Main owns the actual renderer/resource lenders, dynamic Spawn, Stop, physics, and integrated runtime. No compiler, build, patch-apply check, test, or emulator job was run. Full menu-to-SWAMP and repeated lifecycle acceptance remains open.
