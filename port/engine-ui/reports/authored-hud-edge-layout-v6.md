# Actual-cache HUD edge layout V6 validation

PASS: 12 cases covering all four authored HUD styles at 480×320, 2400×1080 and 1024×768, with three consecutive whole authored `onPush` refreshes per case. Frozen receipt: `authored-hud-edge-layout-v6.json`.

The test loads the actual bundled `dqshared_droid.swf` and `dqhud_droid.swf`, uses real append semantics for `NativeSkillGetEquipedSkillsIDs`, and dynamically returns the selected HUD style. Native skill details, localization, texture uploads and fonts are explicitly test platform boundaries; it does not claim production player or GPU validation.

Checks cover actual-instance identity, unchanged scale/skew, left/right mirroring, player/portrait/pause/potion, movement/action/list or fixed skill groups, caption/music and style-3 dpad. Every currently instantiated graph matrix is compared bit-for-bit after successful operations, failed operations, exceptions and actual vector display submissions. Duplicate deadzones are visited by actual display-list instance rather than name lookup. The current joystick stick displacement survives the public receiver-geometry wrapper unchanged.

Positive input checks use actual GameSWF shape traversal to find portrait/pause/potion points, then test the public HUD geometry wrapper at their relocated positions. Separated old positions must miss. The 3:2 zero-offset case naturally has no old-position exclusion. The authored attack clip has no mouse command handler; this test proves its matrix relocation and does not invent a successful shape-handler result.

Host command is `.local-inputs/run_hud_edge_v6.py`. It compiles current HUD, edge helper, source facade and input connection against the frozen host GameSWF DSO. The current facade intentionally interposes the older DSO facade, so ASan ODR detection is disabled for that fixture arrangement; address, leak and undefined checks remain enabled. The test installs no font platform; its explicit test-only font receiver rejects flushing if reached. This is a scoped host graph/input validation, not an APK or emulator receipt. No production files, emulator input, installation or app lifecycle were changed.
