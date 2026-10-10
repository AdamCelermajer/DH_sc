# Act 1 two-scenario replay — 9 October 2026

First playable Act 1 remains **not accepted**. Source tests, compiled APK membership, and device acceptance are distinct below. Evidence timestamps are UTC in the action record; the client timezone is Asia/Jerusalem (UTC+3 on this date).

Candidate APK SHA-256: `79dfdd62ac0687827087335d76b010aa1b76a55cd7f36ec94972404436bbc9f3`. Both architectures compiled successfully in `flow-replay-combined-build.log` and this APK was installed on emulator-5554. Packaged-library hashes and every recorded input are in `.local-inputs/runtime-test-20261009/replay-actions.jsonl`; screenshots and logcat captures use the same directory.

Original application files were backed up before replay to `pre-replay-app-files.tar`, SHA-256 `955e6ea2d21af93070b150d3af7dbe90b1edb8b91d54b011d9d16e79fd8cdf57`. Empty-slot scenario preserves the prior slot-3 file as `files/dh2_003.savegame.replay-preserved`. The test-created `audit` persona is evidence, not a user profile or an accepted gameplay save. Preserve/restore original files after testing.

| Check | Current device evidence | Status |
|---|---|---|
| Occupied Warrior slot 0 precondition | `flow-existing-profile-before`: name A, Warrior; log confirms slot0, save_exists1, PCLS263 | observed |
| Existing character → Single Player | `flow-existing-after-single`: real loading artwork; local0/slot0 assignment succeeds | observed |
| Previous Stage24 shader/texture failures | Same existing-save run advances to Stage26 | passed this scenario |
| Swamp loaded/playable | `flow-existing-load-progress`: Stage26 texture/mip budget exhausted | failed; not playable |
| Saved Mage preview | `flow-creation-mage-slot2`: Galchou, Mage equipment/staff; log PCLS290 | observed |
| Empty-slot preview clears previous character | `flow-creation-empty-slot3`: Empty, no character; log save_exists0/Character none | observed |
| Name entry hides menu environment/avatar | `flow-creation-name-filled` | observed |
| Expected stone artwork in Name | Same screenshot shows flat background | failed; presentation remains open |
| Name → class chooser and Mage selection | `flow-creation-class-ready`, `flow-creation-mage-ready` | observed |
| Back from class chooser | `flow-creation-back-name`: selection scene lacks retained native root loan | failed |
| Class Confirm creates persona and assigns player | `flow-confirm-result`: creates audit/Mage slot3, then receives local_index3/selected_slot0 | failed |
| Movement/combat/chest/loot/XP/save-reload | Not reached by these runs | not_run |

The Confirm failure is now identifiable: the native argument projection was reversed. IDA NativeAssignSaveSlotToPlayer (0x43cf50) reads the top stack argument as save slot and the previous stack argument as local-player index. The slot0/slot0 path concealed this mistake. Do not describe the PlayerManager diagnostic/refactor candidate as a repair: it provided the evidence for the later ABI correction.

Independent active repairs: player-assignment ABI and genuine class-scene root lifetime (same lane); Name background source/asset presentation (separate lane); Stage26 aggregate texture/HUD/font accounting (separate lane). Root owns integration, combined APKs, save preservation and emulator replay. No new global audit is started. The user superseded former emulator RAM/time admission restrictions; emulator testing is authorized.

Pending source changes after this candidate require a new APK hash and device replay before any status above is upgraded.
