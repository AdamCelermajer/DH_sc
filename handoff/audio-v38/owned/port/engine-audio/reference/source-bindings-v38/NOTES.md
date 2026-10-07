# Genuine source Sounds bindings V38

The original executable registers **two different sound schemas**. `sounds_pyarray.bin` is the legacy multi-table stream whose final183 rows are `Arrays::Sounds_bak`. The live `Arrays::Sounds` table is instead registered against `sdd_dungeon_hunter_2_iphone_pyarray.bin`, with its corresponding names stream. The cached generated stream has638 rows,575 direct sounds and63 events. Source IDs are indices in this638-row order. They are not XML UIDs, legacy183-row indices, or labels inferred from XML.

`audit_sound_autogen_v38.py` executes original ARM instructions with bounded Unicorn fixtures. `PyDataArrays::C1` at4be550 produces the registration calls:4c0d80 passes the generated records filename and `Arrays::Sounds::read4b5690`;4c0da0 passes the generated names filename and `readNames4b313c`. `Sounds_bak` registration is independently retained in `legacy-registration.asm` and `registration.json`. Registration callees are observed services; the source caller instructions produce their arguments.

The whole original records reader consumes5,108 bytes using1,277 word reads. Its638 runtime rows have stride12: vtable at+0, UID at+4, event flag at+8. `SoundAutoGen::read502af4` consumes exactly the two serialized words. Every UID/event pair matches the generated stream, and every runtime vtable matches `SoundAutoGen`. `readNames4b313c` consumes15,457 bytes and produces the same638 unique names. Eight whole original `GetMemberIDByString<Sounds>37ba84` lookups cover first/last rows, chest/drop/pickup, an event, a weapon and an absent name.

Whole original `VoxSoundManager::C1` at36c7b0 selects `<resource-root>/data/sounds/sounds.xml` for both the zero and nonzero optional-zip settings. The nonzero setting calls the filesystem with `data/sounds/sounds.zip`, then loads the same XML. This constructor has no `sounds_he.xml` selection branch. The fixture supplies root storage, string operations, allocation, filesystem and engine endpoints, and records the XML argument before the XML parser service; it does not pretend that stubbed parser output is a real initialized soundpack.

## Integration

New production files are `port/engine-audio/audio_source_bindings_v38.hpp/.cpp`. `AudioSourceBindingsV38::records_uri` and `names_uri` specify the exact registered generated files. Call `load(records,size,names,size,error)` once on the producer/control thread. `rows()` returns `std::vector<AudioSoundAutoGenV34>` directly accepted by `VoxAudioBridgeV34::bind_source_autogen`. `row(source_id)` looks up the source ordinal; `source_id(exact_name)` uses the source name order. Do not call it with a soundpack UID or the legacy table.

Loading validates stream extents, matching counts, UID/event ranges, complete unique names and embedded terminators. A failed load leaves the existing owner unchanged. The parser does not create missing clips, bypass game phase/driver gates, or replace XML filenames.

## Exact availability ledger

`ledger.json` joins **serialized UID/event pairs** to XML numeric UIDs. Names are included for diagnostics only. All638 targets exist in `sounds.xml`. Availability is282 done,2 partial,354 missing, where these statuses mean all/some/none of the referenced exact filenames exist in this archive; they do not mean implemented producer, audible output or accepted gameplay.

| Source ID | Source name | XML UID | Exact filename | Cache |
|---|---|---|---|---|
|33|ChestOpen|162|sfx_chest_opening.wav|missing|
|61|DropArmor|148|sfx_drop_armor.wav|missing|
|62|DropGold|151|sfx_drop_gold.wav|missing|
|63|DropPotion|149|sfx_drop_potion.wav|missing|
|64|DropSword|150|sfx_drop_weapon.wav|missing|
|154|PickupArmor|152|sfx_pickup_armor.wav|missing|
|155|PickupGold|155|sfx_pickup_gold.wav|missing|
|156|PickupPotion|153|sfx_pickup_potion.wav|missing|
|157|PickupWeapon|154|sfx_pickup_weapon.wav|missing|
|478..482|five elemental staff source names|31,29,30,33,32|sfx_mage_staff_elemental_thunder_22.wav|present|

The last row is genuine: all five authored XML UID records point to the same thunder clip. It must not be changed to element-specific filenames merely from their labels. The partial events are source270 `env_impact_metal_metal` (eventUID3) and source308 `preload_sfx` (eventUID56): XMLUID41 references `sfx_impactl_metal_1_22.wav`, absent exactly as spelled. Existing nearby names are not silently substituted.

## Validation and boundaries

`proof.json` contains original ELF, stream and XML hashes plus source registration/read/path evidence. `original-runtime-pairs.bin` is captured from actual original runtime rows. The new loader compiles with `-Wall -Wextra -Werror` for ARM64 and x86_64 at the existing Android API23 target, without indentation suppression. `run-host.ps1` compiles bounded O1/O2 ASan+UBSan fixtures, each passing1,576 checks: all638 bindings/name lookups, malformed input and transaction failures, all102 original source listener/emitter property cases from the parent's authority proof, and all five original Listener rows bytewise against captured ARM gold. Listener updates receive explicit position/front/up vectors, preserve existing velocity, and use each actual row's distance/rolloff fields. Invalid vector updates and truncated/corrupt table loads preserve the preceding owner. The fixture makes no level-based listener selection. Source/gold hashes are recorded in native/host validation JSON; build products live under `.local-inputs/audio-v38`.

V34 and frozen handoffs were not changed. No emulator, ADB, APK build/launch, shared native application, renderer or CMake was touched. Root owns event-family producer integration, native audio control/focus/output wiring, full V38 handoff, and guarded audible acceptance.
