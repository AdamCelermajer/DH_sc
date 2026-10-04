# Original UI, inventory and player-skill engine milestone

The new checkpoint builds the original UI frame/input system, authoritative inventory storage and retained player-skill ownership into the native engine. It passes the existing connected player/status scene on the visible emulator. **The full menu, inventory screens and gameplay skill actions are not connected in Android yet.**

## Checkpoint

- APK: `port/android-native/build/checkpoints/dh2-native-source-player-ui-136e924a.apk`
- SHA256: `136e924a7f9f9d73bca38d6c55f05778b0b8df42e7b0bb5821f13a0f1c8be70f`
- Size: 541,045,846 bytes; one APK with the complete original cache, 6,833 cache files plus selected decoded assets, 771 top-level packaged assets.
- Native libraries: 18 ELF64 DSOs across ARM64 and x86_64; minimum LOAD alignment 16 KiB. No ARM32 runtime compatibility layer.
- Actual compiler/source snapshot: `dh2-native-source-player-ui-136e924a-source.zip`, SHA256 `7d829c4d2f61bb6651f6c66c53aef98ebfb9d1616bd6ee661f24182960dea502`; 1,119 captured repository inputs. Assets remain inside the APK rather than duplicating the complete cache in the source ZIP.
- Final PASS receipt: `port/android-native/reports/native-source-player-ui-136e924a-checkpoint-validation.json`.

## Consequential source changes

The UI library now selects the recovered root/sprite/button scheduler and setter-history observers. It uses one actual GameSWF core, with typed history/frame ownership installed before graph construction. Fresh observer ownership per legacy facade load, generation-pinned input/status sessions, ordered ActionScript callbacks and safe last-player/tag-loader recreation are centrally enabled. The versioned direct-core viewport fixture installs actual owners before creating its root; its prior cases/gold are retained.

The game-data library now includes `FreshInventoryOwnedV4`: one authoritative heap item/vector/slot graph and two equipment sets, with source split/merge/delete/equip/unequip/auto-equip/capacity/gold behavior. The prior immutable versions remain preserved. Real item descriptions, powered gear effects and modular weapon resource ownership remain further required producers.

The world library now includes seven V2 player-skill implementation files. `CharacterPlayerSkillsV2` owns one V2 script session/VM/aliases/timers/properties and skill instances, with pinned source tables and saved skill rows. It executes source player InitVCB and original initialization/vitals/configuration/update order. Three real authored classes and 19 initialized skills run through this authority. Forty-two original SkillInfo cases reproduce 9,408 property words and 84 localized strings; live saved-row changes and close-time ownership are tested. The controlled timer-ID fixture proves same-owner lookup; it does not establish authored gameplay cooldown production.

## Verification

- Main central host: 138 suites PASS, ASan/UBSan/LSan zero; report `port/level-world/reports/native-source-player-ui-main-linked-host-audit-v1.json`.
- Input/frame gold: 6,000 cursor and 4,800 frame/drag comparisons with zero mismatches. Cursor gold calls the actual UI DSO with explicitly original-derived libm service fixtures.
- Inventory: 1,720 original-derived mutation steps, 10,828 ordered effect requests, 60 prior loot regressions, 78,265 host checks.
- Actual ARM64/x86_64 Android libraries: PASS with compiler/source hashes, defined exports and ELF/page-alignment inspection in `native-source-player-ui-library-build-v1.json`.
- Gradle all-in-one APK build: SUCCESS. Packaged stripped libraries are byte-matched to the actual build output and live installed APK.
- Emulator-5554/API37: native complete-cache mount and six actual file reads; default 3D player/HUD; real touch movement; authored damage driving retained HUD; resume retaining player/HUD; developer panel open/close. All five cases PASS. Enemy attack selection in the damage scenario is explicit debug control, not full automatic AI acceptance.
- Final visible preview: living player, full HP/mana, panel closed, enemy AI temporarily held for inspection. Screenshot and scoped log in `native-source-player-ui-136e924a-live-v1/final-player-preview.*`.

The initial candidate smoke invocation correctly rejected the previously installed APK hash. Installing this actual candidate and rerunning produced the accepted PASS; the failed attempt remains preserved separately.

## Remaining work

Retained original edit-text/HTML/filter/cache rendering, item presentation/gear/Skin resource effects, actual skill-use/Buff/cooldown gameplay, and Android caller connections are under active development. The visible scene remains the existing status-HUD prototype. Full menus, full enemy AI, remaining levels/campaign, audio and campaign save/load remain incomplete. Physical ARM64 devices have not been tested. The complete reconstruction goal remains active.
