# Retained player scene record

This stage connects the actual player gameplay backing to the native script
object registry. It does not establish a complete original player AI session or
live monster event/FSM integration.

The renderer's private PlayerCombat owns shared PropertyState and
CombatActorState records. Existing gameplay reads and writes use those same
records through accessors. CharacterScriptObjects retains a player record named
PlayerCharacterPrince with identity 0x100000001, the existing gameplay identity.
Its properties and life pointers are the actual KnightPlayerBase backing, not
separate script copies. The player's current raw game position is copied from
the native actor after actor initialization and each actual actor frame.

The world registers this player before creating the eleven original monster
Init sessions. The player and monster records survive GL context recreation;
their owned target records remain valid through Lua close finalizers. This adds
the twelfth native scene record. It does not implement currently unsupported
object methods or arbitrary projectile identities.

## Host proof

`../../tests/character_player_objects.cpp` executes actual cache-backed
KnightPlayerBase/Crypt_Skeleton property initialization and an original monster
Init session. It resolves the player through the real Lua object bridge,
sets/gets the target, observes live player life/property/position changes, and
checks ownership after external references are released. A real Lua close
finalizer reads then clears the retained player target.

`../../reports/character-player-scene-main-linked-host-audit.json` binds 36
passing actual main CMake audits to the compiled inputs and libraries. The new
player composition has 453 checks. ASan/UBSan/LeakSanitizer report zero findings.
Its original Lua target operations are a bounded composition proof; this does
not execute the complete original OnEnemySpotted or player AI implementation.

## Android checkpoint

`../../../android-native/build/checkpoints/dh2-native-player-scene-d57c6e01.apk`
is 26,212,515 bytes, SHA256
`d57c6e01dac7d82aa725a8a349359d88cb06b4fcf1c8b3a75a4fb0580bb21790`.
Repository and Android Studio ARM64/x86_64 builds passed. The APK bundles 263
prototype assets and eight ELF64 libraries per ABI with 16 KiB alignment; it
contains no original ARM32 engine.

`../../../android-native/reports/native-player-scene-d57c6e01-checkpoint-validation.json`
binds five passing emulator suites to this exact installed APK: original
monster initialization and scene records, movement, rotation/pause/resume,
the recovered Prince animation bank, and stationary/moving combat. Eleven
monster records and the player's identity, CharAI, health backing and position
are retained after rotation. Portrait/landscape screenshots show a textured
Crypt and player with prototype controls.

The immutable build capture is
`.local-inputs/native-player-scene-build-capture.zip` relative to the repository:
411 source/build inputs, SHA256
`c8d1202e7c6de15ef3432ef13b81ea32c1882a09e0b39eda5337b76b5dbc3ee2`.
Build inspection binds this capture to the actual host/compiler inputs.

The prototype enemy controller remains a development adapter. Newly compiled
original-derived state, targeting, damage and timer kernels are not all wired
into a complete live original AI loop. Campaign, quests, inventory, skills,
loot, original menus, audio, saves and all-game asset packaging remain. The
API 37 x86_64 emulator is verified; a physical ARM64 phone is not.
