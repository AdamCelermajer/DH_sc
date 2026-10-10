# IPA analysis report (agent `ipa`, preview 15)

Input: `C:/Users/adamc/Downloads/Dungeon_Hunter_2_HD_1.0.0_ios_3.2/Dungeon_Hunter_2_HD_1.0.0_ios_3.2.ipa` (567.7 MB, 8069 zip entries, 758.9 MB uncompressed).
Extracted (stream-extract, nothing executed) to `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview15/ipa/extracted/` (7814 files, about 760 MB). Helper scripts: `.../claude-preview15/ipa/scripts/`.
Exports: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/assets-extra/ios/` with `MANIFEST.json` (256 files, 168.0 MB, sha256 per file, all verified).

## Verdict

1. **This is an iPad build**, not iPhone: `CFBundleIdentifier com.gameloft.dungeonhunter2iPad`, `UIDeviceFamily = [2]`, `CFBundleVersion 1.0.0`, `MinimumOSVersion 3.2`, built with iPhoneOS SDK 4.2. The iPhone-named files in it (`*_iphone*`, `LOGO_Gameloft_iPhone*`) are shared with the Android package (same sizes). There are no original iPhone HUD button positions in this IPA.
2. **The iOS data uses the same logical tree and the same asset formats as Android**, behind a simple packing layer:
   - 6145 logical paths map to 6145 packs `data/fileNNNNNN.dat`. The plaintext map is `data/file00000-plantext.dat` (`packfile:logicalpath` per line, CRLF).
   - 6140 of the 6145 logical names also exist in the Android cache list. The 5 iOS-only names are `shaders.pak`, `sounds/sounds.zip`, `menusgraphics.tga`, `menugraphics02.tga`, `menugraphics04.tga`.
   - Obfuscation: the first 4 bytes of most packs are XOR'd with a per-file key. The rest of the file is the same format as Android. The 4-byte magic is recoverable (`BRES` for .bdae, `PK\x03\x04` for the sounds zip). For 321 same-size files the only difference is those 4 bytes. For 41 .bdae files the only other difference is build-specific `-fxNNNNNNNN` ID strings.
   - Usable directly after the header fix: .bdae, .bin, .mvp/.mgp, .mlx/.mvx/.mgx, .luac, .lightset_xml, sounds WAV (PCM, same formats as the staged Android WAVs), `sounds.xml` (identical 578-uid catalog to Android).
   - Not directly usable: textures (PVRTC, see section 3), menu SWFs (different SWF version and compression), and the localization text tables (sizes differ).
3. **Missing assets:** 253 sound WAV files and 14 VOX banks (`.vxn`) referenced by `sounds.xml` were absent from the staged Android set and are present in the iOS `sounds.zip`. Three bdae assets (`level_up.bdae`, `minimapcameras.bdae`, `minimapcamerashud.bdae`) are also present. All exported; header restored where needed. No sound was synthesized or substituted.
4. **Game code:** the Mach-O is stripped. There are no `Character::LevelUp` or `CharAI::AI_BeginSkill` symbols. The symbol table has 901 entries: Box2D, STL template instances, ObjC imports, and a few game-library names (`item`, `channel`, `slim::Xml*`). Class names otherwise survive only in assert strings (for example `Characters/AI/CharAI.cpp`). The iOS binary does not help IDA at the function level; the Android `.so` (symbols present) remains the primary source.

## 1. Package contents

### 1.1 Identity (Info.plist)

| Key | Value |
|---|---|
| CFBundleIdentifier / Executable | com.gameloft.dungeonhunter2iPad / dungeonhunter2iPad |
| CFBundleVersion | 1.0.0 |
| UIDeviceFamily | [2] (iPad) |
| MinimumOSVersion | 3.2 (DTSDKName iphoneos4.2, Xcode 3.2.5) |
| Orientation | Landscape (left/right) |

### 1.2 Binary

| Item | Value |
|---|---|
| Format | thin Mach-O, ARMv7 (cputype 12, cpusub 9), 7,940,368 bytes |
| Segments | __TEXT 7.46 MB, __DATA 0.61 MB, __LINKEDIT 0.17 MB |
| Symbols | LC_SYMTAB present, 901 nlist entries (180 defined): Box2D, STL instances, ObjC imports, a few game-library names (`item`, `channel`, `slim::Xml*`). Game logic stripped |
| Useful strings | `Characters/AI/CharAI.cpp` source-path asserts, `PlayerLevelUp`, `cinematic_Tuto_levelUp` (script names) |

### 1.3 Package by directory

| Area | Entries | Size |
|---|---|---|
| `app/data/` game packs (`fileNNNNNN.dat`, 6146) plus index, key, fonts, `sounds/` | 6157 | 483.6 MB |
| `app/` root (movies, AIFF music, binary, plists, nibs, loose .png/.jpg) | 42 | 205.0 MB |
| `app/html/` (RES/ tree of jpg/txt/png/js/css, 1026 jpg, 486 txt) | 1562 | 33.9 MB |
| `app/GLIVE_RES/` | 50 | 1.1 MB |

### 1.4 Extensions in the logical tree (from `index_map.tsv`, 6145 names)

| Ext | Count | Ext | Count |
|---|---|---|---|
| .bdae | 2901 | .tga | 233 |
| .mgp | 629 | .luac | 219 |
| .mvp | 598 | .bin | 215 |
| .mlx | 400 | .lightset_xml | 40 |
| .mvx | 253 | .swf | 7 |
| .mgx | 250 | .sc/.symbols/.(lang) text | 37 each per language |

Other root assets: `Take the Lead.aif` and `chuskhor666.aif` (43 MB each, music), `DH2_{US,KR,JP}logo_iPad.mov` (38 MB each), `LOGO_Gameloft_iPhone4.mov`, `LOGO_Gameloft_iPhone.m4v`, 8 `DQ_TRAILER_SUB-*.srt` (no trailer video in the IPA), 8 `*.lproj` string stubs.

### 1.5 Sounds

- `data/sounds/sounds.xml` and `sounds_he.xml` are loose (plain XML). Catalog: 578 uids, 498 distinct filenames. Identical to the Android catalog (same uids and filenames, no extra or missing entries).
- `data/sounds/sounds.zip` = pack `data/file000787.dat` (292 MB, 511 members: 494 WAV, 17 VOX `.vxn`). Its first local header is XOR-obfuscated (`QM\x06\x08` instead of `PK\x03\x04`), which breaks `zipfile` for member 0 only. Reading with a patched header works (`scripts/patchzip.py`).
- WAV formats in the zip: 392 mono 32 kHz 16-bit PCM, 50 stereo 32 kHz 16-bit PCM, 36 mono 22.05 kHz 16-bit PCM, 16 stereo 32 kHz IMA ADPCM (format tag 17, the same 16 `m_*` music files that Android stages as ADPCM too). The staged Android WAVs use the same formats.

## 2. Format comparison with the Android data

Method: logical names from the iOS index compared with `assets/original-cache/` (raw Android tree, 543 files) and `cache_list.txt` (sizes, 6833 Android entries).

| Comparison (iOS logical names present on Android) | Count |
|---|---|
| Same byte size | 5969 of 6140 |
| Byte-identical to the raw Android copy | 0 (all raw copies differ at least in the header) |
| Differ only in the first 4 bytes (header XOR) | 321 |
| Differ in header plus `-fxNNNNNNNN` ID strings only | 41 (all .bdae) |
| Same size, other content differences | 14 (see below) |
| Different size | 167 |

Same-size, other content differences (14): `prince_modular.bdae` (133 bytes), `faeries_template_anim.bdae` and `faeries_02_celeste.bdae` (48 and 115 bytes), `envmap_swamp.tga`, `iphone_table_dh2.tga`, `atlas_weapons_dh2.tga`, `map_bottom.tga`, `map_top.tga`, `character_properties_pycst.bin` (2 bytes), `common_pyarray.bin` (8 bytes), `design_pyarray.bin` (10 bytes), and three Korean text tables (`gothicusb`, `icyhub`, `sewer`: 3, 2 and 685 bytes). These are the candidates for real 1.0.0-vs-Android build differences.

Different-size (167): 160 text tables (`.french` 37, `.german` 37, `.italian` 36, `.spanish` 36, `.korean` 5, `.japanese` 4, `.english` 2, `.sc` 2, `.symbols` 1), 5 SWF, 1 TGA, 1 bin. The text tables are a different build (not decoded).

Conclusion: the 1.0.0 gameplay data, models, animations and scripts are, for practical purposes, the same as the Android data. The only real content deltas are the localization tables, the textures and menus (format change), and the small `pydata` deltas above.

## 3. Textures, menus, movies

- **Textures:** the `.tga` logical names are PVRTC in the iOS pack, not TGA. Container: 8-byte wrapper (`CVH\` + `pvr\0`, obfuscated first 4 bytes) followed by a PVR v2 header with `PVR!` at offset 52, flags `0x8219` (PVRTC 4 bpp). Examples: `splash_final.tga` 1024x1024 4 bpp (524,348 bytes), `menugraphics03.tga` 512x512 (131,132 bytes). These are half the resolution of the Android TGA set (`splash_final.tga` 2048x1024). Not higher-res. Decoding needs PVRTC; the repo already has `b449-source-replay/port/engine-textures/pvrtc.cpp` (and `script-owner-source-replay/.../pvrtc.cpp`).
- **Menus (SWF):** signature XOR'd (`GYV` + version byte 0x0c). iOS `dqhud.swf` is 226,467 bytes; its length field equals the file size, so it appears to be uncompressed SWF v12. Android `dqhud.swf` is 10,720,682 bytes (zlib `CWS`, v7, 10.87 MB uncompressed). The iOS menus use a different authoring. `loadanims.swf` is 472 bytes on iOS and 1563 bytes on Android. Not decoded.
- **Movies:** no `intro.mp4` in the IPA. Candidates are `DH2_USlogo_iPad.mov`, `DH2_KRlogo_iPad.mov`, `DH2_JPlogo_iPad.mov` (45 s, 1024x768 H.264 + 48 kHz PCM, about 38 MB each) and `LOGO_Gameloft_iPhone4.mov` (7.3 s, 1024x768) and `LOGO_Gameloft_iPhone.m4v` (7.3 s, 480x270). Android `intro.mp4` is 50 s, 1280x720. Not copied (large, not in the missing list, logos rather than the intro cinematic).
- **Loading screen:** Android already has `splash_final*.tga` and `loadanims*.swf` staged. The iOS versions are lower-res PVRTC and a different SWF. Not copied.

## 4. Missing-asset check

| Asset (worker name) | iOS location | Result | Export |
|---|---|---|---|
| `level_up.bdae` (FX/interface, I026) | `data/3d/interface/level_up.bdae` = pack `file002215.dat`, 29,292 B | FOUND (size equals Android cache size) | yes, header restored |
| `sfx_level_up.wav` | in sounds.zip and Android staged | not missing | no |
| `sfx_drop_armor`, `_potion`, `_weapon`, `_gold` (uids 148-151, "DropArmor" etc., B048) | sounds.zip, PCM 16-bit mono 32 kHz | FOUND | yes |
| `sfx_pickup_armor`, `_potion`, `_weapon`, `_gold` (uids 152-155, B048) | sounds.zip | FOUND | yes |
| `sfx_potion_drink` (uid 147 "PotionDrink", uid 8 "MenuPotion") | sounds.zip, 434,136 B | FOUND | yes |
| `sfx_static_ball_attack/hurt/killed/spawn` (uids 231-234 StaticBall*) | sounds.zip | FOUND (all 4) | yes |
| `sfx_lizardman_attack_1`, `_attack_2` (uids 282-283) | sounds.zip, 35,946 B each | FOUND | yes |
| `sfx_lizardman_hurt` (uid 284) and `sfx_lizardman_die` (uid 285), B028 | sounds.zip, 59,896 B and 64,780 B | FOUND | yes |
| `minimapcameras.bdae` (B023) | `data/3d/camera/minimapcameras.bdae` = pack `file003480.dat`, 2,600 B | FOUND | yes, header restored |
| `minimapcamerashud.bdae` (B023) | pack `file005329.dat`, 1,688 B | FOUND | yes, header restored |
| Loading-screen art and `loadanims` | Android has them staged | not missing (iOS version is PVRTC, half-res) | no |
| Intro movie variants | no `intro.mp4`; logo `.mov`/`.m4v` only | not missing (see section 3) | no |

Totals of the sound catalog check: `sounds.xml` references 498 distinct filenames. 200 are staged on Android, 298 are not. Of those 298, 253 are in the iOS `sounds.zip` (exported: 239 WAV, 14 VOX) and 45 are in neither (below). The 58 zip members that no `sounds.xml` entry references are not exported.

Not found in any iOS source (45 names referenced by `sounds.xml`, not exported, nothing synthesized):
- `sfx_wolf_attack/die/howl/hurt/spawn.wav` (5 wolf cues)
- `sfx_demon_attack/die/howl/hurt/spawn/rain_attack.wav`, `sfx_demon_steam_burst_attack`, `sfx_dark_queen_*` (killed, critical_hurt, intimidate 1-3)
- `sfx_statue_attack/frost_attack/heat_attack.wav`, `sfx_assassin_bomb_throw/knife_throw1/laugh.wav`, `sfx_balrog_idle.wav`, `sfx_checkpoint_activated.wav`, `sfx_critical_hit.wav`, `sfx_faerie_ice.wav`, `sfx_trap_armed.wav`, `sfx_cultitst_group_chant_loop` (typo, the iOS file is `sfx_cultist_group_chant_loop.wav` in the zip, not referenced), `sfx_specter_attack_energyball_explosion`, `sfx_impact_metal_metal_2.wav`, `sfx_impactl_metal_1_22.wav`, `sfx_skill_mage_thunder_braid_add_2,wav` (comma typo in XML), `m_title_intro.wav`, `m_title_loop.wav`, `m_boss_hell_king.wav`, `m_combat_big_enemy.wav`, `m_cutscene_01.wav`, and the level-group `m_level_<name>.vxn` (7 names, the iOS banks use the `_sfx_*` names instead).
- Name quirks in the iOS zip: `sfx_cutscene_forbidden_seal_1.wav.wav` (double extension), `sfx_critical hit_1.wav` (space).

## 5. Exports

`C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/assets-extra/ios/` (256 files, 168,025,320 bytes):
- `data/sounds/*.wav` (239) and `data/sounds/*.vxn` (14), exact names from `sounds.xml`. Copied from `sounds.zip` as-is (no conversion; WAV is already PCM, the 16 IMA ADPCM WAVs were not exported because they are not missing).
- `data/3d/interface/level_up.bdae`, `data/3d/camera/minimapcameras.bdae`, `data/3d/camera/minimapcamerashud.bdae`: 4-byte header restored to `BRES` (`42 52 45 53 fe ff 00 00`, the Android header). Body unchanged.
- `MANIFEST.json`: per file `path`, `bytes`, `sha256`, `source` (iOS pack or zip member), `format`, `feature` (bug ID), `conversion`, `android_staged_present` (false for all). All hashes re-verified; all RIFF sizes consistent.

Not verified: the VOX banks (`.vxn`) and the three bdae bodies were not compared byte-for-byte to Android (no Android copy is staged for them; sizes match the Android cache list for the bdae). The engine's loading of these files is untested.

## 6. What the 1.0.0 build could add (prioritized)

1. **Sounds (done, needs engine check):** 239 WAV + 14 VOX fill B028 (Lizard cues, uids 284/285 point at `sfx_lizardman_hurt.wav`/`sfx_lizardman_die.wav`), B048 (drop/pickup/potion-drink), the StaticBall group, and 236 other `sounds.xml` cues whose WAV is absent from the staged Android set. The uid table is identical to Android, so no catalog change is needed.
2. **Level-up and minimap bdae (done):** `level_up.bdae` for I026 (the sound `sfx_level_up.wav` was already staged), and `minimapcameras*.bdae` for the B023 minimap camera rigs.
3. **Localization tables (not decoded):** 160 text tables differ in size from Android (French, German, Italian, Spanish, and some Korean/Japanese). iOS may carry different or fuller strings. Worth decoding for B-rows about text.
4. **Version deltas in `pydata` (not decoded):** `character_properties_pycst.bin`, `design_pyarray.bin`, `common_pyarray.bin` differ by 2 to 10 bytes. These may be balance or data changes between 1.0.0 and Android 1.0.2/1.0.3. Low effort to diff field-by-field; check if any stat or drop bug is in question.
5. **Menu SWFs (not decoded):** iOS `dqhud.swf` (226 KB, uncompressed v12) vs Android (10.7 MB, zlib v7). Could be an iPad HUD layout reference; not iPhone.
6. **Textures (low):** PVRTC at half resolution. No higher-res art. Only useful if an iPad-specific UI look is needed.
7. **Movies (low):** DH2 logo variants (US/KR/JP). Not the intro cinematic.

For the iPhone skill-button question: this IPA cannot answer it. The iPhone-suffixed files are shared with Android, and `data/pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin` (5,108 bytes, same on both) is the best lead for iPhone screen layout data. Not decoded here.

## 7. Limits and caveats

- Header restoration is verified on the 321 same-size pairs (XOR of the first 4 bytes) and on the 41 `-fxNNN` pairs, not on every file. For the 3 bdae exported, the restored header matches the Android header pattern.
- Only the first local member header of `sounds.zip` is obfuscated; the rest of the zip reads normally.
- Text table and SWF comparisons are size and header only.
- Work was done in `claude-preview15/ipa/`, `assets-extra/ios/` (as the brief's goal 4 asked), and the report. The input IPA was read-only.
