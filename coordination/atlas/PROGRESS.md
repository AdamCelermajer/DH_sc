# Original Data Atlas progress

Phase: **COVERAGE VERIFIED; MOB SEMANTICS PARTIAL**. Processed rows: **86,309/86,309 (100.00%)**.

**Mob/boss semantics remain PARTIAL.** Joined profiles cover every character row, but unresolved Lua expressions, native contracts and runtime conditions remain. Resume with the named profile gaps in [mob profiles](general/mobs/index.md); root owns further source investigation.

Processed = DONE, UNRESOLVED(reason), or NOT-NEEDED(reason); resolved != ported != runtime verified.

| Category | Rows | Processed | TODO | States |
| --- | --- | --- | --- | --- |
| scripts | 219 | 219 | 0 | {"UNRESOLVED": 215, "DONE": 4} |
| functions | 288 | 288 | 0 | {"UNRESOLVED": 257, "DONE": 31} |
| calls | 7982 | 7982 | 0 | {"DONE": 4170, "UNRESOLVED": 3812} |
| references | 61521 | 61521 | 0 | {"DONE": 61521} |
| assets | 6733 | 6733 | 0 | {"DONE": 6733} |
| levels | 190 | 190 | 0 | {"DONE": 175, "UNRESOLVED": 15} |
| actors | 448 | 448 | 0 | {"DONE": 448} |
| bosses | 65 | 65 | 0 | {"DONE": 65} |
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
    "Exact table-name matches are candidate references; unknown field semantics retain UNRESOLVED disposition."
  ],
  "script_file_rows": 67,
  "scripts": 1278,
  "work_status": "DONE"
}
```

### character-sounds

```json
{
  "bytes_consumed": 98,
  "bytes_total": 11295,
  "field_schema": "[i][i][i][i]bb",
  "following_registered_tables_bytes": 11197,
  "next_registered_table": "Listeners",
  "next_registered_table_rows": 5,
  "rows": 3,
  "source": ".local-inputs/assets-extra/android/data/pydata/sounds_pyarray.bin"
}
```

### mobs

```json
{
  "fully_certified_profiles": 0,
  "meaning": "Joined lookup coverage; none certifies every gameplay rule or dynamic dependency.",
  "profiles": 448,
  "roles": {
    "BOSS_FLAGGED": 59,
    "BOSS_NAME_CANDIDATE": 6,
    "MOB_AI_TYPE_4": 218,
    "OTHER_ACTOR": 165
  }
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
  "case_variant_paths": 0,
  "differing_content": 15,
  "identical_content_different_name_groups": 0,
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
  "references": 61521,
  "references_by_kind": {
    "actor_name_candidate": 46,
    "anim": 4429,
    "animated effect": 553,
    "animation event": 521,
    "animation_dictionary": 854,
    "character": 13199,
    "class": 298,
    "data file": 402,
    "effect set": 575,
    "effect_set": 370,
    "faction": 519,
    "faery": 29,
    "item": 2178,
    "item power": 3643,
    "level": 84,
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
    "sound": 3267,
    "source_sound_ordinal": 202,
    "symbolic_reference": 2544,
    "texture": 5849
  },
  "sound_ledger_rows": 638,
  "table_rows_todo": 0,
  "table_rows_unresolved": 1313,
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
    "statuses": {
      "RESOLVED": 424,
      "UNRESOLVED": 4359
    },
    "todo": 0,
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
    "character": 13199,
    "data file": 328,
    "model": 6013,
    "script": 506,
    "sound": 1542,
    "texture": 5849
  },
  "event_decode_errors": [],
  "reference_sites": 27958,
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

### views

```json
{
  "actor_act_membership_rows": {
    "processed": 448,
    "states": {
      "INFERENCE": 48,
      "UNRESOLVED": 400
    },
    "todo": 0,
    "total": 448
  },
  "boundary_rows": {
    "label": "INFERENCE",
    "processed": 5,
    "todo": 0,
    "total": 5,
    "unresolved": 5
  },
  "category": "ACT-ORGANISED GENERATED VIEWS",
  "inventory": {
    "asset_reference_rows": {
      "processed": 61521,
      "states": {
        "AMBIGUOUS": 2,
        "RESOLVED": 30204,
        "UNRESOLVED": 31315
      },
      "todo": 0,
      "total": 61521
    },
    "backup_scene_rows": {
      "processed": 30,
      "states": {
        "DONE": 30
      },
      "todo": 0,
      "total": 30
    },
    "boss_candidate_actor_rows": {
      "definition": "BOSS/MIDBOSS name markers or explicitly named encounter actors; candidates only",
      "processed": 65,
      "states": {
        "PROVEN": 59,
        "UNRESOLVED": 6
      },
      "todo": 0,
      "total": 65
    },
    "boss_flag_actor_rows": {
      "rule": "CharacterProperties.AI -> AI.Flags bit 4; native HUD predicate at port/level-world/canonical_character_hud_v62.cpp:14.",
      "status": "PROVEN",
      "total": 59
    },
    "campaign_numeric_references": {
      "processed": 1426,
      "states": {
        "RESOLVED": 1426
      },
      "todo": 0,
      "total": 1426
    },
    "campaign_scene_rows": {
      "processed": 43,
      "states": {
        "DONE": 30,
        "UNRESOLVED": 13
      },
      "todo": 0,
      "total": 43
    },
    "campaign_string_references": {
      "processed": 7782,
      "states": {
        "UNRESOLVED": 7782
      },
      "todo": 0,
      "total": 7782
    },
    "character_properties_actor_rows": {
      "processed": 448,
      "states": {
        "RESOLVED": 448
      },
      "todo": 0,
      "total": 448
    },
    "debug_scene_rows": {
      "processed": 32,
      "states": {
        "DONE": 30,
        "UNRESOLVED": 2
      },
      "todo": 0,
      "total": 32
    },
    "demo_scene_rows": {
      "processed": 1,
      "states": {
        "DONE": 1
      },
      "todo": 0,
      "total": 1
    },
    "dialogue_rows": {
      "processed": 55,
      "states": {
        "UNRESOLVED": 55
      },
      "todo": 0,
      "total": 55
    },
    "levels_table_rows": {
      "processed": 84,
      "states": {
        "RESOLVED": 84
      },
      "todo": 0,
      "total": 84
    },
    "mob_actor_rows_ai_type_4": {
      "rule": "CharacterProperties AI row Type == 4; this is an AI category count, not a runtime spawn count.",
      "status": "DONE",
      "total": 283
    },
    "module_character_candidates": {
      "note": "Authored MGP candidates; conditional/random module selection and actor scripts are not executed.",
      "status": "INFERENCE",
      "total": 3437
    },
    "scene_level_rows": {
      "processed": 106,
      "states": {
        "DONE": 91,
        "UNRESOLVED": 15
      },
      "todo": 0,
      "total": 106
    },
    "script_reference_rows": {
      "processed": 4783,
      "states": {
        "RESOLVED": 424,
        "UNRESOLVED": 4359
      },
      "todo": 0,
      "total": 4783
    },
    "serialized_campaign_script_files": {
      "by_source": {
        "android": 65
      },
      "candidate_string_references": 9208,
      "commands": 12610,
      "decode_failures": 0,
      "decoded": 65,
      "missing_pairs": 2,
      "scripts": 1278,
      "states": {
        "DONE": 65,
        "UNRESOLVED": 2
      },
      "total": 67
    },
    "source_script_rows": {
      "processed": 219,
      "states": {
        "DONE": 4,
        "UNRESOLVED": 215
      },
      "todo": 0,
      "total": 219
    },
    "supplemental_name_marked_boss_candidates": {
      "rule": "Preserves all 26 name-marker candidates as notes; classification relies on the AI flag predicate where available.",
      "status": "DONE",
      "total": 26
    }
  },
  "notes": [
    "Counts are inventory, not completion claims.",
    "INFERENCE rows represent generated navigation/candidate memberships, not confirmed boundaries or runtime spawns.",
    "59 rows match the source-proven AI.Flags bit-4 HUD marker; 6 name-only candidates remain supplemental, and encounter placement is not inferred from the marker."
  ],
  "work_status": "DONE"
}
```

## Resume

Run `powershell -NoProfile -ExecutionPolicy Bypass -File tools/atlas/regen.ps1` from this worktree.
Use `atlas.ps1 check` to validate current files. Read the category reports for evidence and unresolved ownership.
The inventory checkpoint preserves TODO before resolution. Later regeneration replaces the generated rows deterministically.
