# Original Data Atlas progress

Phase: **INVENTORY TODO**. Processed rows: **0/57,233 (0.00%)**.

Processed = DONE, UNRESOLVED(reason), or NOT-NEEDED(reason); resolved != ported != runtime verified.

| Category | Rows | Processed | TODO | States |
| --- | --- | --- | --- | --- |
| scripts | 219 | 0 | 219 | {"TODO": 219} |
| functions | 314 | 0 | 314 | {"TODO": 314} |
| calls | 7982 | 0 | 7982 | {"TODO": 7982} |
| references | 32460 | 0 | 32460 | {"TODO": 32460} |
| assets | 6731 | 0 | 6731 | {"TODO": 6731} |
| levels | 190 | 0 | 190 | {"TODO": 190} |
| actors | 448 | 0 | 448 | {"TODO": 448} |
| bosses | 26 | 0 | 26 | {"TODO": 26} |
| tables | 8863 | 0 | 8863 | {"TODO": 8863} |


## Inventory category counts

### assets

```json
{
  "asset_files": 6731,
  "by_source": {
    "android": 6040,
    "android-extra": 18,
    "audio-cache": 265,
    "ios": 256,
    "original-cache": 152
  }
}
```

### references

```json
{
  "inventory_only": true,
  "reference_todo": 32460,
  "references": 32460,
  "references_by_kind": {
    "anim": 2446,
    "animation event": 521,
    "character": 12050,
    "data file": 156,
    "effect set": 83,
    "model": 6013,
    "named_reference": 1935,
    "numeric_reference": 292,
    "script": 509,
    "sound": 2606,
    "texture": 5849
  },
  "sound_ledger_rows": 638,
  "table_rows_todo": 8863,
  "tables": 8863
}
```

### scripts

```json
{
  "calls": {
    "by_mode": {
      "global": 6998,
      "local_function": 257,
      "lua_builtin": 137,
      "object_method": 498,
      "qualified_function": 8,
      "script_function": 84
    },
    "engine_candidates": 7504,
    "todo": 7982,
    "total": 7982
  },
  "engine_call_sites": 7504,
  "engine_function_names": 314,
  "engine_function_rows": {
    "done": 0,
    "todo": 314,
    "total": 314
  },
  "known_answer_tests": {
    "swampking_core.luac": "INVENTORIED_TODO",
    "witch.luac": "INVENTORIED_TODO"
  },
  "native_addresses": 498,
  "native_binding_matches": 271,
  "port_status_candidates": {
    "MISSING": 42,
    "PARTIAL": 272
  },
  "script_file_format_counts": {
    "lua-source": 219
  },
  "script_lines": 27037,
  "script_references": {
    "by_kind": {
      "named_reference": 1939,
      "numeric_reference": 294,
      "script": 3,
      "sound": 3
    },
    "todo": 2239,
    "total": 2239
  },
  "script_rows": {
    "done": 0,
    "todo": 219,
    "total": 219
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

### views

```json
{
  "actor_act_membership_rows": {
    "todo": 448,
    "total": 448
  },
  "boundary_rows": {
    "todo": 5,
    "total": 5
  },
  "category": "ACT-ORGANISED GENERATED VIEWS",
  "inventory": {
    "asset_reference_rows": {
      "todo": 2872,
      "total": 2872
    },
    "backup_scene_rows": {
      "todo": 30,
      "total": 30
    },
    "boss_candidate_actor_rows": {
      "definition": "BOSS/MIDBOSS name markers or explicitly named encounter actors; candidates only",
      "todo": 26,
      "total": 26
    },
    "boss_candidates_by_custom_core_ai_link": {
      "classification": "candidate; must confirm table link",
      "todo": 26,
      "total": 26
    },
    "campaign_scene_rows": {
      "todo": 44,
      "total": 44
    },
    "character_properties_actor_rows": {
      "todo": 448,
      "total": 448
    },
    "debug_scene_rows": {
      "todo": 32,
      "total": 32
    },
    "dialogue_rows": {
      "todo": 55,
      "total": 55
    },
    "levels_table_rows": {
      "todo": 84,
      "total": 84
    },
    "scene_level_rows": {
      "todo": 106,
      "total": 106
    },
    "script_reference_rows": {
      "todo": 2239,
      "total": 2239
    },
    "source_script_rows": {
      "todo": 219,
      "total": 219
    }
  },
  "notes": [
    "Counts are inventory, not completion claims.",
    "Rows remain TODO pending owner integration/acceptance.",
    "Boss count is only a candidate count from explicit custom-core links."
  ],
  "work_status": "TODO"
}
```

## Resume

Run `powershell -NoProfile -ExecutionPolicy Bypass -File tools/atlas/regen.ps1` from this worktree.
Use `atlas.ps1 check` to validate current files. Read the category reports for evidence and unresolved ownership.
The inventory checkpoint preserves TODO before resolution. Later regeneration replaces the generated rows deterministically.
