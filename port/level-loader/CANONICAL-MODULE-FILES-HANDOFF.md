# Canonical Module source-file relay

CanonicalModuleFilesV1 connects the main owner's ModuleLevelLoadBorrowV1::load_file endpoint to CanonicalCachedFileV1. It reads the selected original MGP/MVP files and executes the retained canonical source factory/property traversal, preserving each file occurrence and the SAME Level's module_offset160/object_module_id18c context.

Layer this after canonical-level-module-fields-handoff-b8a188ec6939c773.zip (SHA256 f52a2dccd9055ea097c3c29f652c7666b43a50d95bd18d59e726e55e4537284d) and canonical-connected-source-handoff-fbed9d0d5b3d99e3.zip (SHA256 6605e1d9cb0432463ea46c57f624a790bd6830c3f7f69c3b5614342855d7aa0a). The scene V3 handoff is independent. Do not overwrite newer main owner sources with standalone vendor copies.

## Integration

Use the actual root CanonicalObjectManagerV1, CanonicalClassServicesV1 and candidate lease. The candidate lease must pin the manager and provider context, and must not itself own the relay or its returned borrow: keep them as siblings in the owning world aggregate to avoid a cycle.

Create one relay for the candidate's original source files, supplying the retained archive and SAME CanonicalLevelContextV1. Before calling each actual Module::Load, call begin_module with its diagnostic occurrence. Pass load_borrow() as that Module's Level load services. Module::Load writes the borrowed original 18c/160 fields, chooses its files, loops each until loaded, and resets fields on success. The relay does not implement selection, Random, Module construction, Module InitPost, field resets or publication.

The occurrence is diagnostic. The actual runtime module ID comes directly from SAME Level18c, and the offset comes from SAME Level160. Each source request retains their snapshot and the candidate lease for later initialization. Neither snapshot becomes a second Level field authority.

Each completed file is a distinct retained journal. The next callback starts a new file even when its URI/root equals the previous file; MGP and MVP or repeated modules must not collapse by filename. Pending delivery requires unchanged URI, root and original Level field bits. Failures retain reached attempts and latch, preventing factory/property replay. Access is sequential; delivery and cleanup reentry are rejected.

When discarding the candidate, call discard_after_owner_release with the real main-owner cleanup callback. The callback executes once successfully before any source journal is released. A failed callback preserves sources; a later source-release failure retries only the unfinished source tail. Held old Module borrows continue pinning their Level field storage after discard, and future borrows are empty. No manager Flush, scene cleanup, rollback or destructor behavior is fabricated by this relay.

Set DH2_LOADER_CANONICAL_OWNER_TARGET and DH2_LOADER_MODULE_OWNER_TARGET to the existing root targets before adding the loader directory. Production links these owners and uses their exported headers. The exact pinned Module load TU is compiled only for the standalone probe. Preserve the existing production CMake and add the new library/owner links surgically; the bundled full CMake is review/reproduction material.

## Evidence and limits

The checks run the exact pinned root module_load_v1 implementation with declared XML-field, position, parser-result and source-release fixtures. Positive calls use an explicitly generated Player-only fixture to verify the original Player exclusion, pending/completion and file lifetime gates; they do not construct actors.

An unfiltered original SWAMP first-module gameplay file, data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp, reaches OpenableContainer and fails at the canonical factory's empty prefix because this probe intentionally supplies no class construction providers. The retained failure contains the original XML and runtime placement context; retry does not parse or execute again and does not reset Level fields. This is evidence for the connection and its honest failure behavior, not proof a chest renders.

See reports/canonical-module-files-checks.json for exact runtime results and source hashes. Host, ASAN/UBSAN with leak detection, Android x86_64 on DH2_Loader_API37/emulator-5590 are execution checks; ARM64 is compile-only. No other emulator or APK is changed.

Reproduce from the private checkout using bundled Python: tools/build_canonical_module_files.py host, sanitizers, x86_64, arm64-v8a, then tools/run_canonical_module_files_checks.py. The runner reconstructs its explicitly named fixture and reads the original cache at its existing path. It validates the device AVD and cached archive hash before each operation and writes only native test files under /data/local/tmp/dh2-loader-module-files-v1.

Full Module construction/InitPost, LevelConfig/Level initialization, actual authored class construction, spawning, scene publication, and visibly rendered SWAMP mobs/chests remain pending. The complete loader goal is not satisfied by these checks.
