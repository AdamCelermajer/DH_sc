# B048 DROPSOUND: item drop and pickup sound (Preview 15)

Status: investigation complete (no original payload exists locally); cue wiring implemented; focused test
green; EXE logs verified on the drop and pickup path. Silence is the correct output until the WAVs are
supplied (section 4). Not an audible fix.
Branch `p15/dropsound`, worktree `DH_wt/dropsound`, build `DH_wt/build-dropsound`.

## 1. Verdict on the original samples (exhaustive search)

The selected original WAV payloads for every item drop and pickup cue are **not present** in any
container available to this worktree. The cue names, uids and ordinals all exist; the bytes do not.
No sound was substituted or synthesized.

Cue chain (observed in the data, not guessed):

| Event (original) | IDA | Table field | Source ordinal | XML uid | Selected file (sounds.xml) |
|---|---|---|---|---|---|
| Drop, gold stack | InitAgain 0x3ec0f0 Play3D row+4 | GoldStack1-3 AudioDrop | 62 `DropGold` | 151 | `sfx_drop_gold.wav` |
| Drop, potion | same | Potion AudioDrop | 63 `DropPotion` | 149 | `sfx_drop_potion.wav` |
| Drop, weapon (Knife, LongSword, Sword, Quarterstaff, two-hand, Dagger, Ring) | same | AudioDrop | 64 `DropSword` | 150 | `sfx_drop_weapon.wav` |
| Drop, armor (Belt, Boot*, Chest*, Glove*, Helm, Shield, bag row 16) | same | AudioDrop | 61 `DropArmor` | 148 | `sfx_drop_armor.wav` |
| Pickup, gold stack | Interact 0x3ed144 Play3D row+8 | GoldStack AudioPickup | 155 `PickupGold` | 155 | `sfx_pickup_gold.wav` |
| Pickup, potion | same | Potion AudioPickup | 156 `PickupPotion` | 153 | `sfx_pickup_potion.wav` |
| Pickup, weapon | same | AudioPickup | 157 `PickupWeapon` | 154 | `sfx_pickup_weapon.wav` |
| Pickup, armor | same | AudioPickup | 154 `PickupArmor` | 152 | `sfx_pickup_armor.wav` |

Ordinal to uid comes from the live sound table (`sdd_dungeon_hunter_2_iphone_pyarray.bin`, 638 rows,
checked by `handoff/audio-v38/.../source-bindings-v38/ledger.json`: `DropGold` 62 -> uid 151, status
`missing`). The loot table is `loot_audiovisual_pyarray.bin` (29 rows, names Axe..Warhammer in index
order; `AudioDrop`, `AudioPickup`, `Visual`). Item to row: ItemTable `AudioVisualID`
(`game-data/items.cpp` field list).

Also present: `PotionDrink` (uid 147) and `MenuPotion` (uid 8) use `sfx_potion_drink.wav`. They are
drink cues, not pickup/drop, and are not wired here.

Quirks in the original table, kept as is: Ring (row 21) plays pickup 155 (PickupGold); Bag (row 1) and
DLCItem (row 9) play drop 62 (DropGold); Helm (row 16) plays drop 61 and pickup 154 (its visual name is a bag).

### Search evidence (all empty for drop/pickup payloads)

1. `.local-inputs` file names: every `*.wav`, `*.vxn`, `*.ogg`, `*.mp3`, `*.pcm`, `*.snd`, `*.bin`,
   archive (zip, apk, obb, tar, 7z, pak): 7787 audio/archive paths. Name match for `sfx_drop*`,
   `sfx_pickup*`, `sfx_potion*`, `*drop*`, `*pick*`, `*potion*`, `*gold*`, `*loot*`, `*coin*`: none.
   Names present on disk from sounds.xml: 498 unique; 284 of those are absent on disk, including all
   eight drop/pickup WAVs and `sfx_potion_drink.wav`.
2. Archive contents (346 zip/apk/tar.gz/pak entries scanned by entry name, `unzip -Z1`/`tar -tzf`):
   no drop/pickup/potion entry. Sanity check: the preview-14 and preview-13-rc2 packages list 275
   audio entries each, so the scan reads real listings.
3. Original Android cache listing `dh2_video_research/cache_list.txt` (7088 entries, 265 sound
   entries): sfx_* one-shots present but no drop/pickup/potion. Includes `sounds_he.xml` (variant
   with the same drop names, no payload).
4. `audio-v34/cache/` (261 assets + sounds.xml): no drop/pickup/potion entries.
5. APK `Dungeon-Hunter-2-HD-v1-0-2.apk` (230 entries): assets are `data.save` only; the only media
   are `res/raw/raw_000.ogg` and `res/raw/igli.bin`. Sound payloads are not in the APK.
6. Combined banks: all 17 `m_*.vxn` files have 2 segments each (VoxN `Segm` count = 2) and are
   music/ambience beds. sounds.xml binds the drop/pickup uids 148-155 only to `.wav` names, so no
   VXN can supply them.
7. Other names: `sounds_pyarraynames.bin` (label table) contains `DropGold`, `PickupGold`, etc. as
   labels only; no file names. The `.ogg` name strings for the same cues (`sfx_drop_gold.ogg`,
   `sfx_pickup_gold.ogg`, ...) exist as literals in `ida-apk-export-2026-10-07/libraries/classes.dex/
   strings.jsonl` (addresses 0x2c0c4-0x2d204) but no `.ogg` payload for them exists in any listing.
8. Sound tables (`sounds_pyarray.bin`, 57 copies) carry uid/event rows only.

Conclusion: the original drop/pickup WAV (or OGG) payloads are absent from every local input. Silence is
the only faithful output until a package supplies them.

## 2. Changes (this worktree)

- `features/loot/runtime_world_item_adapter_v1.{hpp,cpp}`: `WorldItemSoundEventV1` (drop, pickup),
  `set_sound_observer`. Notifies after every world-item publish (`publish_at_position`,
  `publish_inventory_drop`) and after a pickup commits (both success exits). The observer cannot change
  the outcome (exceptions swallowed).
- `features/loot/world_item_sound_v1.{hpp,cpp}`: `world_item_sound_ordinal_v1` maps the item's
  ItemAudioVisualTable row to its AudioDrop or AudioPickup ordinal. -1 or out-of-range is silent.
- `features/loot/world_item_sound_v1_tests.cpp` + CMake target `world_item_sound_v1` (ctest), reads the
  Preview 14 package loot table: GoldStack 62/155, Potion 63/156, Knife 64/157, Belt 61/154, plus
  out-of-range, unbound, and synthetic -1 branches.
- `features/audio/runtime_audio_host_v1.{hpp,cpp}`: `submit_world_item_sound`. Resolves ordinal to
  bindings row to catalog sound to `data/sounds/<file>`, probes the selected sample through
  `load_sample_actual_xml_uid`. An absent file (error names the exact URI) returns
  `WorldItemSoundStatusV1::asset_missing` and submits nothing. Otherwise it submits through the existing
  `submit_source_sound` (null target, as Play3D passes 0).
- `features/audio/runtime_session_audio_v1.{hpp,cpp}`: forwarder.
- `main.cpp` (`bindDeathRewards`, after `deathRewards.bind`): sets the observer, logging
  `World item sound uid=N event=drop|pickup source=O item=I status=submitted|asset_missing` and, when
  missing, `cue uid=N asset missing: silent (data/sounds/<file>)`. Silent source cases log
  `status=silent`.

## 3. Tests

Build: `p14_build.ps1 -Name dropsound -Test` (incremental after fixes; `build-dropsound` log in
`.local-inputs/claude-preview15/dropsound/build4.log`, `BUILD_EXIT=0`).

- New `world_item_sound_v1` ctest: PASS.
  `PASS world item sound ordinals: drop/pickup rows GoldStack 62/155, Potion 63/156, Knife 64/157, Belt 61/154; out-of-range, unbound and -1 branches silent`
- Full ctest (112 tests): 111 pass; `session_skill_binding` fails. This is the known worktree
  `.local-inputs` junction failure named in the brief (not related to this change).
- Not covered by an isolated test: the adapter observer calls and the live `submit_world_item_sound`.
  They are verified only in the EXE (section 5). Reason: the live host needs the native output.

## 5. Verifier script and EXE evidence

Build under test: `DH_wt/build-dropsound/dh-foundation.exe` (branch `p15/dropsound`, pre-commit tree).
Baseline: `.local-inputs/windows-source-clock-v19-preview-14-rc1/dh-foundation.exe` (p14/integrate).
Jobs and args live in `.local-inputs/claude-preview15/dropsound/runs/`. Run with
`port/windows-foundation/tools/quiet_run.ps1` (hidden desktop).

Scenario: the preview-13-rc2 `r200` run args (Lizard victim killed at frame 148), with the preview-14-rc1
assets, `--audio --audio-assets <preview-13-rc2>/audio-assets`, `--pickup-frame 190`, `--frames 200`.
The drop depends on `--combat-seed`: seed 1234 gives `spawned=0` in both EXEs, so the seed was swept.

Results (quiet batches, log lines quoted):

| Job | Seed | Death reward | Sound lines (after build) | Pickup |
|---|---|---|---|---|
| seed2-before (rc1 EXE) | 2 | spawned=1 store=1 | none | `gold=0->2` |
| seed2 (new EXE) | 2 | spawned=1 store=1 | `World item sound uid=151 event=drop source=62 item=1 status=asset_missing`; `cue uid=151 asset missing: silent (data/sounds/sfx_drop_gold.wav)`; `World item sound uid=155 event=pickup source=155 item=1 status=asset_missing`; `cue uid=155 asset missing: silent (data/sounds/sfx_pickup_gold.wav)` | `picked=1 gold=0->2` |
| seed8 (new EXE) | 8 | spawned=2 store=2 | drop armor uid=148 source=61; drop potion uid=149 source=63; pickup armor uid=152 source=154; each with a matching `cue ... asset missing: silent` | `PlateHelm01 picked=1 stacks=4->5` |
| seed3..9 (new EXE) | | spawned=0 (seed 2 and 8 drop) | - | - |

Visual check: the final-frame capture `seed2/cap.ppm` is byte-identical to `seed2-before/cap.ppm`
(`cmp` equal, 2594176 bytes), so the change adds no drawn difference. The item pickup gameplay outcome
is identical to the baseline (same gold and stack change).

Verifier commands (after the package is updated, the same jobs must show `status=submitted` instead
of `asset_missing`, and the file must be audible):
- `quiet_run.ps1 -JobsFile runs/jobs_seed.json -Parallel 4` (seeds 2..9 with the new EXE), then grep
  `World item sound` and `cue uid=`.
- Expected now: `status=asset_missing` for uid 148, 149, 151, 152, 155 (and 150, 153, 154 when those drops
  occur); no silent substitution.

Not verified: the `submitted` branch (sample present, Play3D submitted) was not run live, because no
original sample exists to load. It reuses the `submit_source_sound` path that the attack swoosh uses. No
audibility was checked.

## 4. Package files required

No real sample was available, so nothing was staged. To make these cues audible the package must
supply these exact original files under `data/sounds/` (same names as sounds.xml, the 22 kHz `_22`
variants are not used by these uids):

- `sfx_drop_armor.wav` (uid 148), `sfx_drop_potion.wav` (149), `sfx_drop_weapon.wav` (150),
  `sfx_drop_gold.wav` (151)
- `sfx_pickup_armor.wav` (152), `sfx_pickup_potion.wav` (153), `sfx_pickup_weapon.wav` (154),
  `sfx_pickup_gold.wav` (155)
- Optional for the drink cue (not in this change): `sfx_potion_drink.wav` (uid 8, 147)

Add them to `features/audio/source-subset-manifest.json` with size and sha256 only when a real copy is
found; no synthesized or substituted file may be added.

## 6. Open risks / not verified

- No audible verification is possible: the samples are absent, so the logs are the only evidence.
- Reference video: the drop and pickup SFX cannot be read from video frames; no audio track was
  analysed. Visual drop/pickup timing was already recorded in DROPS-survey.md (B5).
- The live asset probe depends on the loader error text containing the exact URI (same convention as the
  existing Lizard missing-sample test).
