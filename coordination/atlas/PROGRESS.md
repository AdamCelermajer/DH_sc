# Original Data Atlas progress

Phase: **MAPPED - final view and checker verification**. Processed rows: **84,855/84,855 (100.00%)**.

Processed = DONE, UNRESOLVED(reason), or NOT-NEEDED(reason); resolved != ported != runtime verified.

| Category | Rows | Processed | TODO | States |
| --- | --- | --- | --- | --- |
| scripts | 219 | 219 | 0 | {"UNRESOLVED": 215, "DONE": 4} |
| functions | 288 | 288 | 0 | {"UNRESOLVED": 257, "DONE": 31} |
| calls | 7982 | 7982 | 0 | {"DONE": 4170, "UNRESOLVED": 3812} |
| references | 60106 | 60106 | 0 | {"DONE": 60106} |
| assets | 6733 | 6733 | 0 | {"DONE": 6733} |
| levels | 190 | 190 | 0 | {"DONE": 175, "UNRESOLVED": 15} |
| actors | 448 | 448 | 0 | {"DONE": 448} |
| bosses | 26 | 26 | 0 | {"DONE": 26} |
| tables | 8863 | 8863 | 0 | {"DONE": 8863} |


## Inventory category counts

### assets

```json
{
  "asset_files": 6733,
  "by_source": {
    "android": 6040,
    "android-extra": 18,
    "audio-cache": 265,
    "ios": 256,
    "original-cache": 152,
    "sound-table": 2
  }
}
```

### campaign

```json
{
  "by_source": {
    "android": 65
  },
  "candidate_string_references": 9208,
  "category": "SERIALIZED LEVEL CAMPAIGN SCRIPTS",
  "commands": 12610,
  "decode_failures": 0,
  "decoded_script_files": 65,
  "missing_pairs": 2,
  "notes": [
    "Commands are decoded source records and are not executed.",
    "Exact table-name matches are candidate references; field semantics remain TODO."
  ],
  "script_file_rows": 67,
  "scripts": 1278,
  "work_status": "TODO"
}
```

### platforms

```json
{
  "availability": {
    "ANDROID_OR_RECOVERED_ONLY": 6288,
    "BOTH": 17,
    "IOS_ONLY": 239
  },
  "by_format": {
    ".bdae": {
      "ANDROID_OR_RECOVERED_ONLY": 2889,
      "BOTH": 3
    },
    ".bin": {
      "ANDROID_OR_RECOVERED_ONLY": 219
    },
    ".lightset_xml": {
      "ANDROID_OR_RECOVERED_ONLY": 40
    },
    ".luac": {
      "ANDROID_OR_RECOVERED_ONLY": 219
    },
    ".mgp": {
      "ANDROID_OR_RECOVERED_ONLY": 629
    },
    ".mgx": {
      "ANDROID_OR_RECOVERED_ONLY": 250
    },
    ".mlx": {
      "ANDROID_OR_RECOVERED_ONLY": 400
    },
    ".mvp": {
      "ANDROID_OR_RECOVERED_ONLY": 598
    },
    ".mvx": {
      "ANDROID_OR_RECOVERED_ONLY": 253
    },
    ".state": {
      "ANDROID_OR_RECOVERED_ONLY": 1
    },
    ".tga": {
      "ANDROID_OR_RECOVERED_ONLY": 242
    },
    ".txt": {
      "ANDROID_OR_RECOVERED_ONLY": 23
    },
    ".vxn": {
      "ANDROID_OR_RECOVERED_ONLY": 3,
      "BOTH": 14
    },
    ".wav": {
      "ANDROID_OR_RECOVERED_ONLY": 244,
      "IOS_ONLY": 239
    },
    ".xml": {
      "ANDROID_OR_RECOVERED_ONLY": 41
    },
    "extensionless": {
      "ANDROID_OR_RECOVERED_ONLY": 237
    }
  },
  "differing_content": 15,
  "paths": 6544,
  "policy": "Use both supplied partial caches, as clarified by the owner; record the platform of every resolution."
}
```

### references

```json
{
  "inventory_only": false,
  "reference_todo": 0,
  "reference_work_todo": 0,
  "references": 60106,
  "references_by_kind": {
    "actor_name_candidate": 46,
    "anim": 4429,
    "animated effect": 553,
    "animation event": 521,
    "animation_dictionary": 854,
    "character": 12050,
    "class": 298,
    "data file": 230,
    "effect set": 575,
    "effect_set": 370,
    "faction": 519,
    "faery": 29,
    "item": 2178,
    "item power": 3643,
    "level_name_candidate": 9,
    "loot": 1141,
    "model": 8022,
    "model component": 1245,
    "named_reference": 1939,
    "numeric_reference": 294,
    "projectile": 150,
    "script": 1164,
    "serialized_string": 7727,
    "skill": 268,
    "sound": 3257,
    "source_sound_ordinal": 202,
    "symbolic_reference": 2544,
    "texture": 5849
  },
  "sound_ledger_rows": 638,
  "table_rows_todo": 0,
  "table_rows_unresolved": 1400,
  "table_rows_work_todo": 0,
  "tables": 8863
}
```

### scripts

```json
{
  "calls": {
    "by_mode": {
      "global": 6166,
      "imported_script_function": 832,
      "local_function": 257,
      "lua_builtin": 137,
      "object_method": 498,
      "qualified_function": 8,
      "script_function": 84
    },
    "done": 4170,
    "engine_candidates": 6672,
    "port_status": {
      "MISSING": 919,
      "PARTIAL": 4915,
      "PORTED": 2148
    },
    "total": 7982,
    "unresolved": 3812
  },
  "engine_call_sites": 6672,
  "engine_function_names": 288,
  "engine_function_rows": {
    "done": 31,
    "total": 288,
    "unresolved": 257
  },
  "known_answer_tests": {
    "swampking_core.luac": "STATIC_SOURCE_ASSERTIONS_PASS",
    "witch.luac": "STATIC_SOURCE_ASSERTIONS_PASS"
  },
  "native_addresses": 498,
  "native_binding_matches": 271,
  "port_status_candidates": {
    "MISSING": 67,
    "PARTIAL": 174,
    "PORTED": 47
  },
  "script_file_format_counts": {
    "lua-source": 219
  },
  "script_lines": 27037,
  "script_readiness": {
    "missing": 919,
    "partial": 4083,
    "ported": 1670,
    "unknown": 0
  },
  "script_references": {
    "by_kind": {
      "named_reference": 1939,
      "numeric_reference": 294,
      "script": 3,
      "sound": 3,
      "symbolic_reference": 2544
    },
    "todo": 4783,
    "total": 4783
  },
  "script_rows": {
    "done": 4,
    "total": 219,
    "unresolved": 215
  },
  "scripts": 219,
  "scripts_by_platform": {
    "android": 219
  }
}
```

### sites

```json
{
  "by_kind": {
    "animation event": 521,
    "character": 12050,
    "data file": 156,
    "model": 6013,
    "script": 506,
    "sound": 1542,
    "texture": 5849
  },
  "event_decode_errors": [],
  "reference_sites": 26637,
  "source_files": {
    "bdae": 2918,
    "xml": 2335
  }
}
```

### table-sites

```json
{
  "by_kind": {
    "anim": 1894,
    "animated effect": 284,
    "class": 298,
    "effect set": 449,
    "faction": 519,
    "faery": 29,
    "item": 2178,
    "item power": 3643,
    "level": 84,
    "loot": 1141,
    "model": 2006,
    "model component": 1245,
    "projectile": 110,
    "script": 531,
    "skill": 55,
    "sound": 601
  },
  "by_table": {
    "ai#group0": 76,
    "ai_factions#group0": 16,
    "animations_dictionary#group0": 1447,
    "character_models_dictionary#group0": 116,
    "character_properties#group0": 448,
    "character_properties#group1": 3,
    "character_properties#group2": 4,
    "effects_dictionary#group0": 284,
    "faeries#group0": 4,
    "faeries#group1": 16,
    "game_objects_dictionary#group0": 85,
    "item_powers#group0": 121,
    "item_powers#group1": 937,
    "levels#group0": 33,
    "levels#group1": 51,
    "loot_audiovisual#group0": 29,
    "loot_table#group0": 1,
    "loot_table#group1": 3,
    "loot_table#group2": 186,
    "loot_table#group3": 1322,
    "loot_table#group4": 9,
    "loot_table#group5": 339,
    "projectiles_dictionary#group0": 28
  },
  "consumption": [
    {
      "consumed": 401412,
      "partial": true,
      "size": 401608,
      "suffix": 196,
      "table": "character_properties"
    },
    {
      "consumed": 5366,
      "partial": false,
      "size": 5366,
      "suffix": 0,
      "table": "character_models_dictionary"
    },
    {
      "consumed": 104357,
      "partial": false,
      "size": 104357,
      "suffix": 0,
      "table": "animations_dictionary"
    },
    {
      "consumed": 16770,
      "partial": false,
      "size": 16770,
      "suffix": 0,
      "table": "effects_dictionary"
    },
    {
      "consumed": 1557,
      "partial": false,
      "size": 1557,
      "suffix": 0,
      "table": "projectiles_dictionary"
    },
    {
      "consumed": 4144,
      "partial": false,
      "size": 4144,
      "suffix": 0,
      "table": "game_objects_dictionary"
    },
    {
      "consumed": 5071,
      "partial": false,
      "size": 5071,
      "suffix": 0,
      "table": "ai"
    },
    {
      "consumed": 636,
      "partial": false,
      "size": 636,
      "suffix": 0,
      "table": "ai_factions"
    },
    {
      "consumed": 699,
      "partial": false,
      "size": 699,
      "suffix": 0,
      "table": "faeries"
    },
    {
      "consumed": 282872,
      "partial": true,
      "size": 284104,
      "suffix": 1232,
      "table": "loot_table"
    },
    {
      "consumed": 54443,
      "partial": false,
      "size": 54443,
      "suffix": 0,
      "table": "item_powers"
    },
    {
      "consumed": 1022,
      "partial": false,
      "size": 1022,
      "suffix": 0,
      "table": "loot_audiovisual"
    },
    {
      "consumed": 5574,
      "partial": false,
      "size": 5574,
      "suffix": 0,
      "table": "levels"
    }
  ],
  "decode_errors": [],
  "reference_cells": 15067,
  "rows": 5558
}
```

### table_sites

```json
{
  "by_kind": {
    "anim": 1894,
    "animated effect": 284,
    "class": 298,
    "effect set": 449,
    "faction": 519,
    "faery": 29,
    "item": 2178,
    "item power": 3643,
    "loot": 1141,
    "model": 2006,
    "model component": 1245,
    "projectile": 110,
    "script": 531,
    "skill": 55,
    "sound": 601
  },
  "by_table": {
    "ai#group0": 76,
    "ai_factions#group0": 16,
    "animations_dictionary#group0": 1447,
    "character_models_dictionary#group0": 116,
    "character_properties#group0": 448,
    "character_properties#group1": 3,
    "character_properties#group2": 4,
    "effects_dictionary#group0": 284,
    "faeries#group0": 4,
    "faeries#group1": 16,
    "game_objects_dictionary#group0": 85,
    "item_powers#group0": 121,
    "item_powers#group1": 937,
    "loot_audiovisual#group0": 29,
    "loot_table#group0": 1,
    "loot_table#group1": 3,
    "loot_table#group2": 186,
    "loot_table#group3": 1322,
    "loot_table#group4": 9,
    "loot_table#group5": 339,
    "projectiles_dictionary#group0": 28
  },
  "consumption": [
    {
      "consumed": 401412,
      "partial": true,
      "size": 401608,
      "suffix": 196,
      "table": "character_properties"
    },
    {
      "consumed": 5366,
      "partial": false,
      "size": 5366,
      "suffix": 0,
      "table": "character_models_dictionary"
    },
    {
      "consumed": 104357,
      "partial": false,
      "size": 104357,
      "suffix": 0,
      "table": "animations_dictionary"
    },
    {
      "consumed": 16770,
      "partial": false,
      "size": 16770,
      "suffix": 0,
      "table": "effects_dictionary"
    },
    {
      "consumed": 1557,
      "partial": false,
      "size": 1557,
      "suffix": 0,
      "table": "projectiles_dictionary"
    },
    {
      "consumed": 4144,
      "partial": false,
      "size": 4144,
      "suffix": 0,
      "table": "game_objects_dictionary"
    },
    {
      "consumed": 5071,
      "partial": false,
      "size": 5071,
      "suffix": 0,
      "table": "ai"
    },
    {
      "consumed": 636,
      "partial": false,
      "size": 636,
      "suffix": 0,
      "table": "ai_factions"
    },
    {
      "consumed": 699,
      "partial": false,
      "size": 699,
      "suffix": 0,
      "table": "faeries"
    },
    {
      "consumed": 282872,
      "partial": true,
      "size": 284104,
      "suffix": 1232,
      "table": "loot_table"
    },
    {
      "consumed": 54443,
      "partial": false,
      "size": 54443,
      "suffix": 0,
      "table": "item_powers"
    },
    {
      "consumed": 1022,
      "partial": false,
      "size": 1022,
      "suffix": 0,
      "table": "loot_audiovisual"
    }
  ],
  "decode_errors": [],
  "reference_cells": 14983,
  "rows": 5474
}
```

### views

```json
{
  "backup_scene_rows": 30,
  "boss_candidates": 26,
  "campaign_scene_rows": 43,
  "character_properties_actor_rows": 448,
  "debug_scene_rows": 32,
  "scene_level_rows": 106
}
```

## Resume

Run `powershell -NoProfile -ExecutionPolicy Bypass -File tools/atlas/regen.ps1` from this worktree.
Use `atlas.ps1 check` to validate current files. Read the category reports for evidence and unresolved ownership.
The inventory checkpoint preserves TODO before resolution. Later regeneration replaces the generated rows deterministically.
