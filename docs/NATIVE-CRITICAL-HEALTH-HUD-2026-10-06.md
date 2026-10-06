# Critical health HUD checkpoint

Installed and frozen APK: `port/android-native/build/checkpoints/dh2-native-critical-health-v7-55164f78.apk`.
SHA256: `55164f7836de89b7740887e0e34e830acd8d6717ec1033ff7f38b94aa7c7b97a`.
Detailed evidence: `port/android-native/reports/critical-health-v7/checkpoint.json`.

## Change

The actual cached `dqhud_droid.swf` HurtCorners sprite31 has 103 health-index frames. Its unnamed child sprite30 contains the original 22-frame pulse. Previously the HUD was advanced only at load, and this separately drawn warning kept its centered 480x320 stage bounds.

The retained HUD now advances only the original pulse child with the existing application tick and actual movie cadence. The outer health lookup remains source-owned. A draw-scoped transform maps this same blood artwork to the actual full display, with its original matrices restored after drawing. This does not replace the artwork, invent a sine wave, or advance unrelated menu callbacks.

## Verification

- ARM64 and x86_64 APK build succeeded; embedded original cache is 433,189,197 bytes. No ARM32 engine ABI packaged.
- Native component tests passed: 66 pulse advances and three complete cycles; layout has 4,253 checks over 480x320, 2400x1080, 1024x768, and 1080x2400. These compile new HUD code against the prior camera APK libraries with explicit texture/render fixtures.
- Current installed APK on visible emulator5554: normal joystick movement and actual monster damage produced low health. The live original pulse reached opacity0.648438 and1.000000; reviewed screenshots show warning at all four display edges, without the old central rectangle.
- A separate normal-combat run accepted the actual potion press: health42265/42265, source health frame99, warning alpha0; potion stock5→4. Reviewed screenshot confirms warning disappears.
- Successful combat/recovery process logs contain no DH2Native error. The latest APK was left running in a fresh Crypt world reached through the main-menu StartGame/SinglePlayer flow, with enemy AI enabled.

The first capture attempt's developer enemy-AI toggle did not stop canonical NPC damage; the player died and its potion was correctly rejected as unavailable. This attempt is retained as evidence, rather than being counted as recovery acceptance. The separate successful recovery is in `12-recovery-after.log` and `.png`.

## Limits and next integration

This checkpoint accepts critical-health display and animation only. Lethal skill Hit8/Cmd_Kill remains unconnected to the actual current-Level/drop/XP/quest services. Whole Level.Init/current GSLevel publication, live chest loot and the full first Swamp chapter remain incomplete. No physical device has been tested.

The recovered catch_up=false movie schedule advances one source frame per application tick when due. A22/30-second loop is nominal at sufficient frame rate; slow emulator frames stretch its live duration. Direct development-world startup currently attempts camera loading before Android surface resize and fails; the normal main-menu Play path was used for acceptance.

TriggerZoneV22 was independently hash-checked and forwarded to the loader chat so it can continue the actual Swamp object prefix. Its deterministic zero-backed network storage is explicitly a modern memory-safety correction, with positive global services still required.
