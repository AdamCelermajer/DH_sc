# Existing exact audio asset audit V39

No missing exact authored filenames were recovered. This audit checked283 unique missing filenames from the proven638-row source binding ledger against212 existing DH2 original/derived archive central directories and the matching loose-asset inventory. There were zero exact matches and zero archive errors. It did not rename a sample, infer a suffix, substitute a similar clip, edit a cache, or change frozen V38.

## Original supplied archives

| Existing file | Bytes | SHA256 | Audio members |
|---|---:|---|---:|
|Downloads/Dungeon-Hunter-2-HD-v1-0-2-cache.zip|433189197|3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679|261|
|Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip|433189197|3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679|261|
|Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2.apk|10269872|32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200|0|

Both supplied cache copies are cryptographically identical. The provided APK contains no WAV/VXN files or separate plausible nested audio asset archive. Twenty-two derived APK/source archives contain a nested `dh2-original-cache.zip`; each nested payload was read sequentially in1MiB chunks and its SHA256 equals the same inspected original cache hash above. They therefore provide no additional sample version.

The search included `sfx_chest_opening.wav`, all eight genuinely authored loot filenames (`sfx_drop_armor/gold/potion/weapon.wav` and `sfx_pickup_armor/gold/potion/weapon.wav`), and every missing filename referenced by the source sound/event rows, covering the missing animation families. The parent's request mentioned nine loot drop/pick files, but the genuine XML/source ledger has eight distinct drop/pick filenames. No ninth label or filename was invented.

## Evidence and limits

`audit_archives.py` uses `rg --files --hidden --no-ignore` first. Downloads content inspection is restricted to names/paths identifying the supplied Dungeon Hunter2 archives. Workspace inspection covers `.local-inputs`, `port`, and `session-contributions` when present; unrelated third-party renderer/physics sources and ZIP parser fixtures are excluded. `report.json` lists each inspected archive path, version hint from its filename, sizes and exact central-directory listing SHA256. Original supplied archives have full file hashes. Small derivative archives also have full file hashes; larger derived outer APKs carry the central listing digest instead of a whole outer file hash. All22 plausible nested cache payloads have full payload hashes.

`remaining_exact_missing` retains all283 filenames. No extraction or codec check was needed because no new exact sample was found. This result is limited to the existing provided files; it does not claim the clips cannot exist in another game release or an additional cache. No network, emulator, ADB, application runtime or packaging was used.
