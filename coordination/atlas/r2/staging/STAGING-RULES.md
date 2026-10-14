# Package B source and staging rules

This is the package-B source investigation used before classifying reference rows. It records the consumers that determine whether an original path is resolvable and the exact staging-script contract. It is a static source audit; the ledger generator does not change game behavior.

## Port path consumers

`port/windows-foundation/content_paths.cpp` (SHA-256 `B5C1D517CFC194987D4E912A5535B0FE37CC184AE5F7AE4698EAF5B7ADEEAEEB`) is the current frozen content resolver. `normalize_content_uri` at line 70 converts backslashes, accepts the artist `q:/data/...` drive form by removing its drive prefix, rejects absolute/traversing paths, strips URI fragments, and removes path components named `iphone`, `ps3`, `debug`, or `old`. `find_case_path` at line 50 resolves each path component case-insensitively and rejects multiple case-folded matches as ambiguous. `content_path_candidates` at line 99 tries the exact and normalized URI, owner-relative paths, `original-cache/` forms, the flattened PyData `.bin` alias, original level search folders, and type-specific basename folders. It also maps `.dae`/`.xml` to `.bdae` and tries the iPhone `pvr2_` texture form. `resolve_content_path` at line 162 checks those candidates in order and keeps AssetCatalog containment checks authoritative.

Audio has a separate exact-path consumer. `features/audio/feature_audio.hpp:29` documents `AudioFilesystem` as `root/data/sounds/filename` with no basename or suffix fallback. `audio_source_cue_submission_v1.cpp:39` constructs that URI from the exact `sounds.xml` filename. Therefore content-path aliases do not establish an audio alias, and a repeated filename prefix such as `sfx_sfx_` must remain exact unless original bytes or a different consumer rule prove otherwise.

## Staging script contract

`coordination/v21.8/stage_assets_v21_8.py` (SHA-256 `10326BFE350D8A6620A797AEBBB0B0C431008AEAF655F0E66F004F78C185767F`) reads `source_root`, `destination_root`, `groups`, and optional per-group `group_destinations`. Each selected relative path is copied to `<group override or destination_root>/<same relative path>`. The source root is singular per selection JSON. The script rejects a case-insensitive duplicate source path across groups, rejects a selected destination already in the base manifest, rejects missing/non-regular/reparse source files, verifies each existing base file against its manifest size and SHA-256, refuses an existing output folder, and rewrites a new manifest after copying.

The current comparison package is `.local-inputs/windows-source-clock-v21.8-staged2/manifest.json`; the staging directory and its manifest are read-only. Its paths are package-relative (for example `assets/data/...` and `audio-assets/data/sounds/...`). Any package-B dry preflight must use scratch inputs/output and stop before the copy phase so no staged package is changed.

## Evidence limits

The path resolver is evidence for the port's accepted lookup rules, not proof that an original site executes. A serialized field that only looks like a resource name remains an unresolved/dynamic site until its source consumer establishes that role. A file found by a broad suffix search remains a candidate until an exact authored link, manifest path, or implemented resolver rule connects it.
