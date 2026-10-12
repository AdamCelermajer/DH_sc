# Integrated build loader repair V115

Finite repair of actual act1-native-build.log errors, based on current shared source. Main owns integrated compilation/runtime; no independent checks or emulator were run.

- RetainedLevelModuleGraphV1::candidate_owner_v50 exposes a const reference to its existing input_.candidate_owner. The Root connection keeps its original ownership-equivalence check. No new owner or alias is fabricated.
- LevelSavegameOwnerV1::validate_checkpoint restores the earlier loader-save-return validation contract on the existing owner, guarding ready/cache/busy/released and forwarding the same mutable fields and cache. The returned delivery boolean remains separate from original valid output.
- LevelSavegameRuntimeV1 forwards through the existing runtime and exact cache/fields identity check. Existing source_save_checkpoint_v83 and its implementation are preserved; no competing save method is introduced.

Apply loader-api-repair-v115.patch at the shared repository root. Five existing files changed, no new translation unit or CMake change.

Runtime dependency remains explicit: Main must bind LevelSavegameApplicationV1.validate_checkpoint to genuine original463288 filename/cache/INFO behavior. Missing callback returns a required-service failure. This patch does not fabricate checkpoint validity or claim the callback is implemented. Original source reference: .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0046/00463288.c. The existing candidate/loader-save-return/HANDOFF.md describes the callback sequence.

Terminal callback repair is delegated separately and will be included in the same finite handoff.

## Complete validation producer and terminal fix

The validation producer dependency above is now implemented by checkpoint-producer-v115.patch. Apply it AFTER loader-api-repair-v115.patch, then terminal-loading-callable-build-repair.patch. No new translation unit/CMake entry. Includes one new header level_validate_checkpoint_v115.hpp.

Original463288: capture mode0c; query actual online, conditional actual hosting and manager719; derive checkpoint filename; probe original primary then .bak without reading/flushing; neither exists returns delivered true/original valid false and leaves cache/filename unchanged. Present file changes SAME cache filename, executes original _cacheFile(NULL) reload, loads INFO into SAME fields.loaded_row2c, compares actual row, restores normal filename using freshly read mode0c, then reloads SAME normal cache. No replacement save/cache/queue. Initial new-game can reach this path whenever actual save_ec is positive in state34; it is not Continue-only.

Native enrollment is confined to existing source_campaign_save_objects_v86.cpp. It uses the same retained provider with weak World, verifies actual LevelEC/cache/fields, and uses existing App FileManager. New source_exists_v115 uses actual lstat under the same transport mutex with existing regular-file/path policy. Existence does not flush jobs. Cache reload reuses existing read callback (including actual matching-job flush at read/open) and registrations, releases old cached bytes before reads, preserves loaded INFO fields when cache/file is genuinely absent. Malformed payload still reports existing parser errors; never silently fabricates validity.

Terminal fix distinguishes optional std::function availability from mandatory captured lambdas, retaining existing failure latch/order. Child completed finite repair and holds. Root alone validates all patches through integrated build/runtime. No independent build, tests, syntax checks or emulator were run.
