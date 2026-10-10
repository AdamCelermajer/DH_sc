# ANDROIDCACHE report: Android device data folder vs our staged assets

Agent: androidcache (read-only on inputs). Date: 2026-10-10.

## 1. Inventory

Source: `C:/Users/adamc/Downloads/dungeonhunter2/com.gameloft.android.GAND.GloftD2SS/files` (the game root is `.../files`; there is no `version.txt` or `shaders.state` at the folder root, they are in `files/data/`).

| Item | Value |
|---|---:|
| Files | 6833 (6690 under `data/`, 123 under `res/`, 20 at `files/` root) |
| Bytes | 648,357,710 (about 618 MiB) |
| `data/3d` | 5495 files, about 251 MB |
| `data/sounds` | 264 files (244 wav, 17 vxn, sounds.xml, sounds_he.xml, .nomedia), about 250 MB |
| `data/menus` | 32 files (30 swf, 2 other), about 54 MB |
| `data/text`, `pydata`, `scripts`, `scene`, `gfx`, `tweaker` | 334, 217, 226, 107, 2, 3 files |

Earlier listing check: `Dungeon-Hunter-2-HD-v1-0-2-cache.zip` has 6833 file entries with the same paths and total bytes as the folder. `cache_list.txt` is identical to both (6833 paths, all sizes equal). So the listing was never incomplete. What was missing was the file content: our staged assets did not contain these files.

## 2. Version

`files/data/version.txt` contains `1.0.0B` (6 bytes, no newline). This is not labelled v1.0.2. The APK (`Dungeon-Hunter-2-HD-v1-0-2.apk`, 225 entries) carries no game data files (none of the missing names are in it), so there is no direct hash match against the APK. Treat this folder as a 1.0.0B-labelled data set. The data for the missing items is not in any v1.0.2 source we hold, so this may be the best available source, but the version mismatch should be kept in mind.

## 3. Diff against what we have

Method: each device file was matched by basename and size against the 89,236 staged files in DH_sc (all of `.local-inputs`, coordination and port trees), then SHA256 streamed. Classification in `.local-inputs/claude-preview15/androidcache/classification.tsv`.

| Class | Files | Bytes | Meaning |
|---|---:|---:|---|
| IDENTICAL | 3392 | 501,725,658 | staged copy with same name, size and SHA256 |
| DIFFERENT | 2 | 590 | `dh2_settings.savegame`, `oconf.bar`: same name and size as other copies, different content; device or iOS config, not game assets |
| NEW | 3439 | 146,631,462 | no staged file with the same name and size (content we did not have) |

NEW by top directory:

| Dir | Files total | Identical | New | Bytes new |
|---|---:|---:|---:|---:|
| data/3d | 5495 | 2213 | 3282 | about 248 MB total (most of it new) |
| root (saves, intro videos, `d_o_w_n_l_o_a_d_e_d.txt`) | 153 | 66 | 85 | 78,142,969 total |
| data/scene | 107 | 37 | 70 | 493,645 total |
| data/tweaker | 3 | 1 | 2 | 12,481 total |
| data/sounds, menus, pydata, text, scripts, gfx | 1,075 | 1,075 | 0 | all identical |

Caveat: NEW means "no staged file with that basename and size". A file could exist under another name, but no staged copy was found under its name. Not every NEW file has been individually confirmed absent.

## 4. The missing assets from the tracker and surveys

| Asset (as requested) | Status | Device path | Bytes | SHA256 |
|---|---|---|---:|---|
| `level_up.bdae` (I026, FX 135) | FOUND, NEW | `data/3d/interface/level_up.bdae` (lowercase on disk) | 29,292 | 47e297936e527a4b54fc60666315be5e87128945bb730d981df8c6b322896942 |
| `sfx_level_up.wav` | found, already staged | `data/sounds/sfx_level_up.wav` | 439,048 | IDENTICAL to `.local-inputs/audio-v34/cache/sfx_level_up.wav` |
| `minimapcameras.bdae` (MAP) | FOUND, NEW | `data/3d/camera/minimapcameras.bdae` | 2,600 | a4f4e09f8e4f7ba862d60b1114f5c483fce13df8589e93ea72560daf5e8b7757 |
| `minimapcamerashud.bdae` (MAP, related) | FOUND, NEW | `data/3d/camera/minimapcamerashud.bdae` | 1,688 | 3b9d641e28a34369115285a42913bba8488e4b68437ffa5b8e5a487a1ec76952 |
| `camera_minimap_idle.bdae` (MAP, related) | FOUND, NEW | `data/3d/camera/animations/common/camera_minimap_idle.bdae` | 1,424 | d7a7183495888f8bf1e8584cd6900643db98c805591852753eb970f444770221 |
| `menu_miniMap` | FOUND inside `dqhud.swf` (CWS, zlib-compressed). `dqhud.swf` is IDENTICAL to our staged copy | `data/menus/dqhud.swf` | 10,720,682 | 3ab455733367acf657df6eaaec39c93fed34cbcf519255e02cf488c07a96f2bb |
| `loadanims.swf`, `loadanims_droid.swf`, `loadanims_i9000.swf` (loading art) | found, already staged | `data/menus/` | 1,563 / 1,456 / 1,456 | IDENTICAL |
| loading tips fonts (arkham_bold/reg, nanumgothic, uncadis, wqy-zenhei, japanese, Fontin SmallCaps, sct_font_3.fnt) | found, already staged | `data/*.ttf`, `data/menus/sct_font_3.fnt` | | all IDENTICAL |
| `sfx_drop_{armor,potion,weapon,gold}.wav` (uid 148-151) | NOT FOUND | | | |
| `sfx_pickup_{armor,potion,weapon,gold}.wav` (uid 152-155) | NOT FOUND | | | |
| `sfx_potion_drink.wav` (uid 8 MenuPotion, uid 147 PotionDrink) | NOT FOUND | | | |
| `sfx_static_ball_{spawn,hurt,killed,attack}.wav` (uid 231-234, bank 5) | NOT FOUND (whole StaticBall group) | | | |
| `sfx_lizardman_attack_1/2.wav`, `sfx_lizardman_hurt.wav`, `sfx_lizardman_die.wav` (uid 282-285, bugs B028) | NOT FOUND | | | |

Searched: device `files/` tree, the APK entry list, `.local-inputs/claude-preview15/ipa/extracted` (iOS), and the full DH_sc tree by name. None of the drop, pickup, potion, StaticBall, or lizard WAVs exists anywhere we can reach.

`sounds.xml` cross-check (578 entries): 254 resolve to a file on the device. 324 reference files that are not on the device. These include every bank-5 monster SFX (zombie, skeleton, balrog, spider, slime, drake, dark queen, dragon, etc.), bank-3 item, door, chest and trap SFX (DropX, PickupX, ChestOpen, SpikeTrap etc.), and bank-1 music that names `.vxn` files which are present. All 264 WAV/VXN/XML files that are on the device are IDENTICAL to our staged copies, so the sound folder adds nothing new.

VXN banks (17 files): these are level music and ambience banks (IMA ADPCM, per existing handoff notes). They are IDENTICAL to our staged copies. Searching the bytes for `sfx_lizardman`, `sfx_static_ball`, `sfx_drop_`, `sfx_pickup_` and `sfx_potion_drink` returned nothing. I did not decode the VXN Data chunks, so an SFX inside a VXN cannot be fully excluded, but no name-level evidence exists.

Other cross-check: 36 distinct asset filenames cited in `coordination/claude-preview14/*.md`, `coordination/claude-preview15/*.md` and `docs/BUGS-AND-IMPLEMENTATION.md`: 30 on device, 6 not. The 6 not on device are `sfx_potion_drink.wav`, `sfx_lizardman_attack_1.wav`, `structnames.bin` (not a real device name), `original-campaign.xml` (generated, staged elsewhere), and the two reference-video filenames (not game assets).

Correction to the map survey: `coordination/claude-preview14/MAP-survey.md` says `menu_miniMap` is absent from every shipped SWF. That is wrong. Searching compressed bytes missed it. After decompressing the CWS `dqhud.swf`, the string is present. Our staged `dqhud.swf` is IDENTICAL to the device copy, so the target is already available to the port.

## 5. Staged extra assets

Copied (non-destructive, sha verified after copy) to `.local-inputs/assets-extra/android/`, with the same relative paths as the game data:

- `data/3d/interface/level_up.bdae` (needed by I026 level-up presentation, FX set 135)
- `data/3d/camera/minimapcameras.bdae` (MAP, minimap camera)
- `data/3d/camera/minimapcamerashud.bdae` (MAP, related)
- `data/3d/camera/animations/common/camera_minimap_idle.bdae` (MAP, related)

Manifest: `.local-inputs/assets-extra/android/MANIFEST.json`. The game folder and our existing staged folders were not modified.

## 6. Other valuable NEW content (prioritized for Preview 15 and later)

1. **Startup and cinematics**: `intro.mp4` (20.2 MB), `intro_jp.mp4`, `intro_kr.mp4` (root, NEW). Camera clips: `data/3d/camera/animations/` has 98 NEW files (852 KB): `cs_king_intro`, `cs_moth_intro`, `cs_swampking_intro`, `cs_swampking_outro`, `cs_swamp_intro`, `earth_temple_intro/outro`, `madruk_intro/end`, `witch_intro`, `dark_queen`, `common`. Swamp king intro cutscene actors are in `data/3d/characters/lizardman/animations/cs_swampking_intro_*` (about 20 NEW files), which is directly relevant to the lizard ambush.
2. **Saves as test fixtures** (copy only into new test folders, never modify): `dh2_000.savegame`, `dh2_000_0_000_041_level.savegame` (99 KB, level state), `dh2_000_0_single_level.checkpoint`, `dh2_000_single.checkpoint`, plus `.bak` copies. `DebugSwitches.savegame` and `dh2_settings.savegame` are also present.
3. **Later-act scenes**: 38 NEW `.mlx` files, including `003_darkwood`, `005_infectedvillage`, `009a_abbey`, `015_red_desert_hub`, `025_icy_hub`, `031_firetemple`, `036_underworld_hub`, `037_machine_rooms_01`, `038_dark_temple`, `039_dark_temple_bossroom_01`, plus `x*_backup.mlx` copies. The other 32 NEW scene files are `.rule.xml` and backup variants (not separately counted). Chests, vases and containers (Preview 15) and later acts need these.
4. **Characters and bosses (NEW)**: faeries 117 files (3.8 MB), dragon 19, madruk 45, lizardman 52, dark_queen 28, swampking 28, spider 25, infected 31, dk 27, statue 38, sharkman 25, goblin 39, root_troll 22, asassin_demon 28, witch 24, troll 18, moth 16, darkguardian 20, lighthouse 12, yeti 21, plants 20, prince 4.
5. **Level modules**: `data/3d/modules` has 1992 NEW files (mgp/mvp/mlx for later levels), useful when level transitions are implemented.
6. **Interface**: `data/3d/interface` has 249 NEW files (incl. `level_up.bdae`). The rest of the UI art is worth a separate pass for the quest and map panels.
7. **Tweaker data**: `data/tweaker/gamepad.tweaker_xml` and `player_light.tweaker_xml` (NEW).
8. **`d_o_w_n_l_o_a_d_e_d.txt`** (369 KB, 6574 lines): Gameloft's download list of game paths. Useful as the authoritative list of files the game loads.
9. **Not useful**: `res/` (123 files, UI pngs; check quickly only if needed), `.bin`/`.luac` in pydata/scripts (all IDENTICAL).

## 7. Open items and risks

- The 6 sound-table entries that are the core of B028 and the item feedback (lizard, drop/pickup, StaticBall) remain unavailable from every source we have. Keep them silent and logged, per the existing rule, unless a source with those WAVs turns up.
- `data/version.txt` says `1.0.0B`. Confirm which build produced this device folder before relying on file contents for v1.0.2 behaviour.
- Scratch files: `.local-inputs/claude-preview15/androidcache/` (`device_inventory.tsv`, `zip_inventory.tsv`, `classification.tsv`, `staged_name_hits.json`, and the scripts). All hashing used streamed reads, one Python process at a time.
