# Native complete-cache APK milestone

Current checkpoint is `port/android-native/build/checkpoints/dh2-native-full-cache-ad2462c9.apk`, SHA256 `ad2462c92386bbb467744f2d7717f3b187ed5f048724b8bd9ad92a74cc7d5b0e`, **537,494,062 bytes**. It is installed on visible `emulator-5554`, Android API37. The player preview has been restarted with a living player, full HP/MP, development drawer closed and enemy AI disabled for inspection.

## Completed in this batch

- Entire supplied ZIP is stored inside the APK: **6,833 files**, **433,189,197 compressed archive bytes**, **648,357,710 decoded bytes**. The original archive hash is `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`. No external cache copy or extraction is needed.
- New native `ZipAssetPackV1` mounts an immutable directory, resolves original relative resource URIs, streams stored/deflated files, verifies local metadata and CRC, preserves outputs on failure, rejects ambiguous paths/duplicates and keeps positional-read backing alive. Only the requested decoded file and a bounded compressed chunk are allocated. Canonical cache files are byte-exact; this does not automatically decode their individual proprietary formats.
- Android `OriginalCacheAssetsV1` reads the ZIP directly through its uncompressed APK asset descriptor. Existing original UI resource lookup now reaches the full archive for uncatalogued original URIs. Six real startup reads verify skill script, audio, level geometry, loot data, menu and empty-resource bytes against canonical SHA256 values.
- Gradle bundles the exact archive as a generated asset, verifying its hash. On another development machine supply `-Pdh2OriginalCache=/path/to/the/cache.zip`. The unchanged original save/checkpoint files remain archive resources; this reader does not import them into a new campaign.
- Whole HUD formatting/SkillInfo source batch is integrated into actual central UI/world/runtime libraries: original parseEx/localization, temporary skill properties, owned VarArgs/string frames and a single-call Lua return observer. Runtime compiles the additive TU instead of compiling the old runtime twice.

## Verification

- Native filesystem: **all6,833 files** compare by decoded length/SHA256 to Python's independent ZIP reader, **21** corruption/domain/replacement guards, zero ASan/UBSan/LSan findings. Initial failed audit was a Windows TSV newline issue; corrected accepted receipt is `port/asset-payloads/reports/zip-asset-pack-v1-host-audit.json`.
- Main engine integration: **129 suites**, zero sanitizer findings. Formatting includes1,087 original/O2 cases; SkillInfo640 ordered cases; two original cache skills produce42 complete temporary sheets/9,408 words and84 localized Current/Next strings. That formatting proof's active VM is still explicitly a fixture; the agent's new actual player-session lifecycle is separate work awaiting handoff/integration.
- Real ARM64+x86_64 builds contain the new system. APK inspection verifies **18 ELF64 libraries**, at least16KiB LOAD alignment, **771 assets**, the complete uncompressed-in-APK cache ZIP and every existing770 asset. No ARM32 compatibility runtime is included.
- API37 emulator: installed APK hash matches checkpoint. Native directory mounts6,833 files and six actual probes pass. Existing connected player/status scene passes default view, real touch movement, authored damage/HUD correspondence, context resume and development drawer open/close. Damage is a controlled authored-animation scenario; full automatic AI is not inferred.
- Final checkpoint validation: `port/android-native/reports/native-full-cache-ad2462c9-checkpoint-validation.json`, SHA256 `bd93cee3259bf086b6d443123aa6cd576370ae886c25516692d2e0d1518f72e9`.
- Actual compiler/Ninja closure snapshot: `port/android-native/build/checkpoints/dh2-native-full-cache-ad2462c9-source.zip`, SHA256 `2877f5521a3b0c906654f9c6e04540a05a5f731d3b17c59a50697fe9c56492bf`. It captures1,070 source inputs and compiler evidence; assets remain in the checkpoint APK, avoiding a second large archive copy.

## Live limits and next connection

This is a whole asset-delivery subsystem and regression-verified player preview. **The new menu/inventory/skill screens are not yet live.** Merely bundling every level/audio/script does not make them playable. The visible scene retains the earlier player/status UI, including unfinished portrait/potion/skill actions. Physical ARM64 device testing and the complete campaign/audio/save experience remain unverified.

Separate substantial handoffs now include authoritative inventory V4 and input/frame/session V2. Next integrate source default startup/teardown across legacy movie callers, retained player skill publication, real item effects and original dynamic text fields, then promote their connected player/HUD/menu behavior together. The full native game goal remains active.
