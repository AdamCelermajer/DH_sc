# CharSounds decoder handoff

Status: helper implemented and tested in isolation; atlas integration and generated mob-page verification remain with root.

## Original-source evidence

This is an internal data-decoding feature with no standalone visual counterpart, so no gameplay footage is required. The visible behavior it supports is the original actor-specific death, hurt, and material-impact sound selection during combat; animation-event and Lua/script audio remain separate producers and must keep their own reference paths.

The original registration in `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp:113-114` binds `sounds_pyarray.bin` and its names stream to `CharSoundsTable` using `[i][i][i][i]bb`. It is followed by the distinct `Listeners` registration at lines 115-116. The `sounds_pystructnames.bin` schema group 5 names the six serialized members `Die`, `Hurt`, `ImpactVsFlesh`, `ImpactVsMetal`, `IsFlesh`, and `IsMetallic`. IDA `Structs::CharSounds::read` at `0x4eade4` reads four count-prefixed signed-int vectors in this order, then two booleans at byte offsets 36 and 37. IDA `Arrays::CharSoundsTable::read` is `0x4b96bc`.

IDA `Character::GetCharSoundsId` at `0x3a3294` reads the actor's signed sound-table index and returns row `2` when it is negative or outside the table. `Character::GetCharSounds` at `0x3a32d0` then indexes the registered member array at a 40-byte row stride. The recovered original combat handler `0x3afee0` obtains sound rows for the involved characters, uses the target's `Die` vector when dead and `Hurt` when damage is positive, and for positive character-attacker damage selects the attacker's impact vector according to the target row's material flags. Its admission checks and random list selection are retained in the source handoff `character_combat_sound_v1.cpp`; this decoder records data only and does not duplicate those gameplay gates.

The sound IDs in these vectors are SoundAutoGen ordinals. The original registration/source binding at `0x4c0d80/0x4c0da0`, `Arrays::Sounds::read 0x4b5690`, and the V38 recovered-source handoff prove the distinct 638-row `sdd_dungeon_hunter_2_iphone` stream is the live `Sounds` table; each row ordinal is its source ID. The legacy `sounds_pyarray.bin` stream also contains later unrelated schemas, including `Listeners` and `Sounds_bak`; neither is the source namespace for these vector IDs. No mapping to XML UID or the legacy `Sounds_bak` labels is inferred.

The original bytes provide a direct known answer: `ArmoredHuman` index 1 has `Die=[28]`, `Hurt=[29,30,31]`, `ImpactVsFlesh=[120,121,122]`, `ImpactVsMetal=[117,118,119]`, `IsFlesh=false`, and `IsMetallic=true`. These resolve by ordinal to `CharacterDie`, `CharacterHurt1..3`, `ImpactMetalFlesh1..3`, and `ImpactMetal1..3` in the 638-row table. `Default` is index 2 with empty vectors and flags `(true,false)`; the native fallback is specifically the choice of row 2 for an invalid actor row index, not a claim that all actors use it.

## Helper/API and expected result

`tools/atlas/mob_sounds.py` exports `build(ctx, tables, references)`. It returns a JSON-serializable dictionary with:

- `rows`: three `sounds#group0` row records, with source field names and values preserved.
- `references`: one edge per vector member, pointing to `sdd_dungeon_hunter_2_iphone` at the same ordinal, including target name. Invalid/negative IDs are explicitly unresolved.
- `consumption`: the CharSounds section boundary and the following registered-table boundary. The helper consumes exactly 98 bytes for the three CharSounds rows, then verifies the next word matches five registered Listener names. The remaining bytes belong to later registered tables and are not interpreted as CharSounds.

This lets root add actor sound fields and reference edges to the atlas dossier without coupling the decoder to the root generator. Gameplay fidelity stays in the existing combat sound consumer: use the decoded vectors and flags under the source dead/damage/attacker gates, while animation- and script-authored audio continues to resolve from its own sites.

## Verification and gaps

Focused original-row assertions passed for `Default` and the non-default `ArmoredHuman`, all 10 ArmoredHuman IDs resolved to the expected 638-row table, the CharSounds boundary was exactly byte 98 with the next Listener count of 5, JSON serialization succeeded, and the reader rejected an out-of-bounds request. These were isolated source assertions only. No full atlas regeneration, generated mob/boss view review, executable/runtime test, or visual gameplay verification was performed by this helper task.

Remaining uncertainty is limited to version/cache variants: the helper currently reads the authoritative Android `assets-extra/android/data/pydata` stream selected by atlas context. If another platform's legacy CharSounds stream differs, compare its table before assuming byte-for-byte parity. Sound namespace and ordinal meaning are source-proven for this registered executable; asset availability and audible output remain separate atlas/runtime concerns.
