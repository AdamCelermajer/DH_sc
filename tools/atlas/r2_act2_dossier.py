"""Static, evidence-linked Act 2 readiness dossier builder for Atlas round 2."""
from __future__ import annotations

import hashlib
import json
import re
import xml.etree.ElementTree as ET
from collections import Counter, defaultdict
from pathlib import Path

from common import physical_path, read_json, source_site, stable_id, write_json

_STAGING_INDEX_CACHE = {}


def _source_indexes(ctx):
    out = Path(ctx.out)
    levels = [r for r in read_json(out / "data/levels.json", []) if r.get("record_type") == "scene_file"]
    actors = read_json(out / "data/actors.json", [])
    tables = read_json(out / "data/tables.json", [])
    campaign = read_json(out / "data/campaign.json", [])
    campaign_refs = read_json(out / "data/campaign-references.json", [])
    assets = read_json(out / "data/assets.json", [])
    return {
        "levels": {str(r.get("file", "")).casefold(): r for r in levels},
        "actors": {str(r.get("name", "")).casefold(): r for r in actors},
        "actors_by_id": {r.get("id"): r for r in actors},
        "tables": tables,
        "table_by_id": {r.get("id"): r for r in tables},
        "table_by_name": defaultdict(list, {k: [] for k in []}),
        "table_by_table_index": {(r.get("table"), r.get("index")): r for r in tables},
        "campaign": {str(r.get("scene", "")).casefold(): r for r in campaign},
        "campaign_refs": campaign_refs,
        "assets": assets,
        "asset_by_rel": defaultdict(list),
    }


def _source_receipt(ctx, raw_path):
    if not raw_path:
        return {"path": raw_path, "status": "UNRESOLVED", "reason": "No source path was indexed."}
    try:
        path = physical_path(ctx, str(raw_path))
        if not path.is_file():
            return {"path": str(raw_path), "status": "UNRESOLVED", "reason": "Indexed source path is absent from this input tree."}
        raw = path.read_bytes()
        return {"path": source_site(ctx, path), "sha256": hashlib.sha256(raw).hexdigest(),
                "bytes": len(raw), "status": "RESOLVED", "confidence": "PROVEN",
                "claim_scope": "exact_original_source_file_receipt"}
    except (OSError, ValueError):
        return {"path": str(raw_path), "status": "UNRESOLVED", "reason": "Source file could not be read for a deterministic receipt."}


def _prep_indexes(ix):
    for row in ix["tables"]:
        ix["table_by_name"][str(row.get("name", "")).casefold()].append(row)
    for row in ix["assets"]:
        ix["asset_by_rel"][str(row.get("relative", "")).replace("\\", "/").casefold()].append(row)
    ix["refs_by_scene"] = defaultdict(list)
    for row in ix["campaign_refs"]:
        ix["refs_by_scene"][row.get("scene_id")].append(row)
    return ix


def _asset_paths(ix, raw_path):
    if not raw_path:
        return []
    rel = str(raw_path).replace("\\", "/")
    if rel.casefold().startswith("data/"):
        rel = rel[5:]
    key = rel.casefold()
    return [{"path": r.get("path"), "source": r.get("source"), "sha256": r.get("sha256"),
             "status": r.get("status"), "confidence": "PROVEN", "claim_scope": "source_file_presence"}
            for r in ix["asset_by_rel"].get(key, [])]


def _table_link(ix, actor, field_name):
    ref = (actor.get("field_links") or {}).get(field_name) or {}
    target = ix["table_by_id"].get(ref.get("target_id"))
    if not target:
        return {"reference": ref or None, "target": None, "status": "UNRESOLVED"}
    return {"reference": ref, "target": target, "status": "RESOLVED"}


def _script_source_index(ctx):
    roots = [Path(ctx.inputs) / "assets-extra/android/data/scripts",
             Path(ctx.inputs) / "assets-extra/android/original-cache/data/scripts",
             Path(ctx.inputs) / "assets-extra/ios/data/scripts"]
    by_stem = defaultdict(list)
    all_paths = []
    for root in roots:
        if not root.is_dir():
            continue
        for path in root.rglob("*.luac"):
            by_stem[path.stem.casefold()].append(path)
            all_paths.append(path)
    return by_stem, all_paths


def _lua_chain(ctx, stem, script_index, native_names):
    by_stem, _ = script_index
    start = str(stem or "").strip()
    if not start or start.casefold() in {"monster", "__npc__", "none", "-1"}:
        return {"root": start or None, "status": "NOT-NEEDED", "reason": "generic_or_empty_AI_script"}
    queue = [start]
    visited = {}
    while queue:
        current = queue.pop(0)
        if current.casefold() in visited:
            continue
        candidates = sorted(by_stem.get(current.casefold(), []), key=lambda p: str(p).casefold())
        if not candidates:
            visited[current.casefold()] = {"stem": current, "status": "UNRESOLVED",
                                           "reason": "no exact AI Lua filename in indexed original sources"}
            continue
        # Android-authored source is canonical; alternate copies remain provenance.
        path = next((p for p in candidates if "assets-extra\\android\\data\\scripts" in str(p).casefold()), candidates[0])
        raw = path.read_bytes()
        text = raw.decode("utf-8", "replace")
        includes = re.findall(r"\bInclude\s*\(\s*['\"]([^'\"]+)['\"]\s*\)", text, re.I)
        calls = sorted({name for name in native_names
                        if re.search(rf"\b{re.escape(name)}\s*\(", text)}, key=str.casefold)
        selected = []
        for line_no, line in enumerate(text.splitlines(), 1):
            if re.search(r"phase|form|health|\bhp\b|dying|summon|attack_flag|musicStarted|DoSkill|SetAIState|SetState",
                         line, re.I):
                if len(selected) < 80:
                    selected.append({"line": line_no, "text": line.strip(), "confidence": "PROVEN",
                                     "claim_scope": "original_lua_source_line"})
        visited[current.casefold()] = {
            "stem": current,
            "source": source_site(ctx, path),
            "sha256": hashlib.sha256(raw).hexdigest(),
            "line_count": len(text.splitlines()),
            "includes": includes,
            "native_calls_matched": calls,
            "phase_and_behavior_evidence_lines": selected,
            "confidence": "PROVEN",
            "claim_scope": "original_lua_source",
        }
        queue.extend(includes)
    return {"root": start,
            "status": "RESOLVED" if visited and all(x.get("status") != "UNRESOLVED" for x in visited.values()) else "PARTIAL",
            "files": [visited[k] for k in sorted(visited)],
            "include_edges": [{"from": v.get("stem"), "to": child, "confidence": "PROVEN",
                               "claim_scope": "original_lua_include"}
                              for v in visited.values() if v.get("includes") for child in v["includes"]]}


def _asset_ref_for_actor(ix, actor):
    model = _table_link(ix, actor, "ModelFile")
    model_row = model.get("target") or {}
    model_path = (model_row.get("fields") or {}).get("Path")
    anim = _table_link(ix, actor, "AnimTable")
    anim_row = anim.get("target") or {}
    anim_states = {}
    dictionary = {}
    for row in ix["tables"]:
        if row.get("table") == "animations_dictionary#group0":
            dictionary[row.get("index")] = row
    for state, value in (anim_row.get("fields") or {}).items():
        if isinstance(value, int) and value >= 0:
            row = dictionary.get(value)
            if row:
                path = (row.get("fields") or {}).get("Path")
                anim_states[state] = {"index": value, "name": row.get("name"), "path": path,
                                      "asset_candidates": _asset_paths(ix, path), "confidence": "PROVEN",
                                      "claim_scope": "animation_table_row"}
            else:
                anim_states[state] = {"index": value, "status": "UNRESOLVED",
                                      "reason": "no matching animation dictionary row"}
    sounds = _table_link(ix, actor, "Sounds")
    sound_profile = sounds.get("target") or {}
    loot = _table_link(ix, actor, "Loot")
    return {
        "model": {"table_link": model, "path": model_path, "asset_candidates": _asset_paths(ix, model_path),
                  "status": "RESOLVED" if model_path else "UNRESOLVED", "confidence": "PROVEN",
                  "claim_scope": "character_model_dictionary_row",
                  "reason": None if model_path else "ModelFile dictionary link has no usable Path; source model resolution is unknown."},
        "animation": {"table_link": anim, "state_rows": anim_states,
                      "status": "RESOLVED" if anim_states else "UNRESOLVED",
                      "confidence": "PROVEN", "claim_scope": "character_animation_table_rows",
                      "reason": None if anim_states else "AnimTable link has no indexed animation state rows; required animation paths remain unknown."},
        "sound": {"table_link": sounds, "profile": sound_profile,
                  "status": "RESOLVED" if sound_profile else "UNRESOLVED",
                  "confidence": "PROVEN", "claim_scope": "character_sound_profile_row",
                  "reason": None if sound_profile else "Sounds link has no indexed sound profile row; sound assets remain unknown."},
        "loot": {"table_link": loot, "row": loot.get("target"),
                 "status": "RESOLVED" if loot.get("target") else "UNRESOLVED",
                 "confidence": "PROVEN", "claim_scope": "character_loot_table_row",
                 "reason": None if loot.get("target") else "Loot link has no indexed loot-table row; drops remain unknown."},
    }


def _scene_placements(ctx, scene, scope, ix):
    modules = [m for m in scope["module_sources"] if m.get("scene_id") == scene.get("scene_id")
               and m.get("platform") == "android"]
    found = []
    seen = set()
    parsed_module_keys = set()
    def module_key(value):
        normalized = str(value or "").replace("\\", "/")
        match = re.search(r"(?:^|/)3d/modules/(.+)$", normalized, re.I)
        if match:
            normalized = match.group(1)
        normalized = re.sub(r"^.*?/modules/", "", normalized, flags=re.I)
        return normalized.casefold()
    for module in modules:
        path = physical_path(ctx, str(module.get("path", "")))
        try:
            module_bytes = path.read_bytes()
            module_sha256 = hashlib.sha256(module_bytes).hexdigest()
            root = ET.parse(path).getroot()
        except (ET.ParseError, OSError):
            continue
        parsed_module_keys.add(module_key(module.get("relative") or module.get("path")))
        for object_index, obj in enumerate(root.findall(".//GameObject"), 1):
            if obj.get("gametype", "").casefold() != "character":
                continue
            props = dict(obj.attrib)
            name = props.get("name")
            charprops = props.get("charpropsname") or props.get("char_template")
            actor = ix["actors"].get(str(charprops or "").casefold())
            key = (module.get("relative", "").casefold(), str(name or "").casefold())
            if key in seen:
                continue
            seen.add(key)
            found.append({
                "placement_id": stable_id("r2-act2-placement", scene.get("scene_id"), module.get("relative"), name),
                "object_name": name,
                "source_scene": scene.get("scene"),
                "source_scene_id": scene.get("scene_id"),
                "source_module": module.get("path"),
                "source_module_relative": module.get("relative"),
                "source_sha256": module_sha256,
                "source_anchor": f"GameObject[{object_index}] name={name!r}",
                "source_anchor_confidence": "PROVEN",
                "module_conditional_choice": bool(module.get("conditional")),
                "charpropsname": props.get("charpropsname"),
                "char_template": props.get("char_template"),
                "char_template_pydata": props.get("char_template_pydata"),
                "ai_state": props.get("ai_state"),
                "char_group": props.get("char_group"),
                "char_group_role": props.get("char_group_role"),
                "spawn_prob": props.get("spawn_prob"),
                "auto_spawn": props.get("auto_spawn"),
                "activate_cond": props.get("activate_cond"),
                "deactivate_cond": props.get("deactivate_cond"),
                "actor_id": actor.get("id") if actor else None,
                "actor_name": actor.get("name") if actor else charprops,
                "actor_link_status": "RESOLVED" if actor else "UNRESOLVED",
                "source_confidence": "LIKELY" if module.get("conditional") else "PROVEN",
                "association_claim_scope": "scene_module_membership",
                "field_confidence": "PROVEN",
                "field_claim_scope": "authored_MGP_GameObject_attribute",
                "port_runtime_status": "UNVERIFIED",
                "port_runtime_blocker": "No Windows production-path spawn/admission run was performed.",
                "status": "DONE" if actor else "UNRESOLVED",
                "reason": None if actor else "No exact actor-table row matched charpropsname/char_template.",
            })
    # Round-1's authored-definition pass may retain a character when its MGP
    # parser could not recover the complete document. Keep that evidence as a
    # separate likely association instead of silently treating the file as empty.
    source_level = ix["levels"].get(str(scene.get("scene", "")).casefold(), {})
    for old in source_level.get("module_spawn_candidates", []):
        module = str(old.get("module", "")).replace("\\", "/")
        if module_key(module) in parsed_module_keys:
            continue
        object_name = old.get("object_name")
        key = (module_key(module), str(object_name or "").casefold())
        if key in seen:
            continue
        seen.add(key)
        actor = ix["actors"].get(str(old.get("charpropsname") or old.get("actor_name") or "").casefold())
        found.append({"placement_id": stable_id("r2-act2-placement", scene.get("scene_id"), module, object_name),
                      "object_name": object_name, "source_scene": scene.get("scene"),
                      "source_scene_id": scene.get("scene_id"), "source_module": old.get("module"),
                      "source_module_relative": module, "module_conditional_choice": True,
                      "source_sha256": _source_receipt(ctx, old.get("module")).get("sha256"),
                      "source_anchor": f"recovered GameObject name={object_name!r}",
                      "source_anchor_confidence": "LIKELY",
                      "charpropsname": old.get("charpropsname"), "char_template": old.get("template"),
                      "char_template_pydata": None, "ai_state": old.get("ai_state"),
                      "char_group": old.get("char_group"), "char_group_role": old.get("char_group_role"),
                      "spawn_prob": old.get("spawn_prob"), "auto_spawn": None,
                      "activate_cond": None, "deactivate_cond": None,
                      "actor_id": actor.get("id") if actor else old.get("actor_id"),
                      "actor_name": actor.get("name") if actor else old.get("actor_name"),
                      "actor_link_status": "RESOLVED" if actor else "UNRESOLVED",
                      "source_confidence": "LIKELY", "association_claim_scope": "round1_static_definition_recovery",
                      "field_confidence": "PROVEN", "field_claim_scope": "round1_MGP_source_candidate",
                      "port_runtime_status": "UNVERIFIED",
                      "port_runtime_blocker": "Recovered source candidate only; no Windows production-path spawn/admission run was performed.",
                      "status": "DONE" if actor else "UNRESOLVED",
                      "reason": None if actor else "Round-1 source candidate has no exact CharacterTable row join."})
    return found


def _campaign_detail(ctx, ix, scene, campaign):
    if not campaign:
        return {"status": "UNRESOLVED", "reason": "no campaign record indexed for this scene",
                "source_chains": [], "command_counts": {}, "quest_dialogue_commands": []}
    scripts = campaign.get("scripts", [])
    command_counts = Counter()
    selected = []
    select_names = {"Script_StartDialog", "Script_WaitDialog", "Script_ExecScript", "Script_ChangeLevel",
                    "Script_SetAIState", "Script_SetActorPosition", "Script_SpawnCharacter", "Script_CompleteQuest",
                    "Script_CONSOLE", "Script_PlayLevelMusic", "Script_PlaySound", "Script_OpenDoor", "Script_CloseDoor"}
    for script in scripts:
        for command in script.get("commands", []):
            name = command.get("name")
            command_counts[name] += 1
            if name in select_names:
                selected.append({"script_index": script.get("index"), "script_name": script.get("name"),
                                 "command_index": command.get("index"), "command": name,
                                 "fields": command.get("fields", {}),
                                 "source_status": command.get("decode_status"),
                                 "port_status": (command.get("native_mapping") or {}).get("port_status", "UNKNOWN"),
                                 "port_evidence": (command.get("native_mapping") or {}).get("port_path_anchors", []),
                                 "confidence": "PROVEN", "claim_scope": "decoded_campaign_command"})
    source_chain = [{"path": campaign.get("script_file"), "source": campaign.get("source_commands"),
                     "names": campaign.get("source_names"), "command_stream": campaign.get("source_commands"),
                     "source_file_receipts": [_source_receipt(ctx, value) for value in
                                              [campaign.get("script_file"), campaign.get("source_commands"), campaign.get("source_names")]
                                              if isinstance(value, str) and value],
                     "script_count": campaign.get("script_count"), "command_count": campaign.get("command_count"),
                     "confidence": "PROVEN", "claim_scope": "decoded_campaign_script_bank"}]
    refs = ix["refs_by_scene"].get(scene.get("scene_id"), [])
    story_refs = [{"id": r.get("id"), "command": r.get("command"), "script_name": r.get("script_name"),
                   "script_index": r.get("script_index"), "command_index": r.get("command_index"),
                   "field_offset": r.get("field_offset"), "value": r.get("value"), "kind": r.get("kind"),
                   "status": r.get("status"), "site": r.get("site"), "confidence": r.get("confidence"),
                   "claim_scope": "campaign_serialized_reference"}
                  for r in refs if r.get("command") in select_names]
    script_tokens = {str(s.get("name") or "").casefold() for s in scripts if s.get("name")}
    quest_rows = []
    for row in ix["tables"]:
        if row.get("table") != "v2quests#group0":
            continue
        authored_scripts = [str(x) for x in ((row.get("fields") or {}).get("Scripts") or []) if x]
        normalized = {x.casefold() for x in authored_scripts}
        suffixes = {x.rsplit(".", 1)[-1].casefold() for x in authored_scripts}
        if script_tokens & (normalized | suffixes):
            quest_rows.append({"quest_id": row.get("id"), "quest_name": row.get("name"),
                               "table": row.get("table"), "fields": row.get("fields"),
                               "source_file": row.get("source_file"), "source_path": row.get("source_path"),
                               "status": row.get("status"), "work_status": row.get("work_status"),
                               "confidence": "PROVEN", "claim_scope": "exact_quest_script_name_join"})
    return {"status": "DONE", "source_chains": source_chain,
            "script_names": [s.get("name") for s in scripts],
            "script_count": campaign.get("script_count"), "command_count": campaign.get("command_count"),
            "command_counts": dict(sorted(command_counts.items())),
            "quest_dialogue_commands": selected,
            "source_reference_rows": story_refs,
            "source_reference_count": len(story_refs),
            "quest_source_rows": quest_rows,
            "quest_script_join_status": "RESOLVED" if quest_rows else "UNRESOLVED",
            "quest_script_join_reason": None if quest_rows else "No exact campaign script name matched a v2quest Script field; absence of a join does not prove the scene has no quest behavior.",
            "dialogue_text_decode": {"status": "UNRESOLVED",
                                     "reason": "Campaign command payloads provide dialog IDs/fields; these are not asserted as rendered text without a verified string-table join."}}


def _lua_native_links(native_scope, lua_files, page_manifest=None):
    sources = {str(r.get("source", "")).replace("\\", "/").casefold() for r in lua_files}
    page_manifest = page_manifest or {}
    matches = []
    for row in (native_scope or {}).get("native_rows", []):
        sites = []
        for call in row.get("all_original_call_sites", []):
            if str(call.get("source", "")).replace("\\", "/").casefold() in sources:
                sites.append({"call_id": call.get("call_id"), "name": call.get("name"), "script": call.get("script"),
                              "line": call.get("line"), "expression": call.get("expression"),
                              "function_id": call.get("function_id")})
        if sites:
            page = page_manifest.get(str(row.get("name", "")).casefold(), {})
            matches.append({"native_name": row.get("name"), "ledger_function_id": row.get("ledger_function_id"),
                            "contract_state": page.get("status", "UNAVAILABLE"),
                            "contract_page": page.get("path"), "contract_page_sha256": page.get("sha256"),
                            "scope_ledger_state": row.get("state"),
                            "current_port_semantic_status": page.get("current_port_semantic_status", row.get("current_port_semantic_status", "UNKNOWN")),
                            "current_port_difference": page.get("current_port_difference"),
                            "frozen_atlas_port_status_reference_only": row.get("atlas_port_status_reference_only", "UNKNOWN"),
                            "current_port_evidence": page.get("current_port_evidence", row.get("current_port_identifier_matches", [])),
                            "call_sites": sites, "confidence": "PROVEN",
                            "claim_scope": "package_A_current_callsite_and_contract_page_cross_join"})
    return matches


def _staging_rows(ctx):
    root = Path(ctx.out) / "r2"
    progress = read_json(root / "package-b-progress.json", {})
    rows = []
    for relative in progress.get("classification_files", []):
        # package-b-progress stores paths relative to ctx.out/r2's parent
        # (for example r2/staging/classification/...), not relative to r2.
        path = Path(ctx.out) / relative
        try:
            data = read_json(path, {}) if path.is_file() else {}
        except Exception:
            continue
        if data.get("act") != "act_2_inferred":
            continue
        for row in data.get("rows", []):
            rows.append({**row, "classification_file": str(relative).replace("\\", "/"),
                         "classification_act": data.get("act")})
    return rows


def _staging_crossrefs(rows, names, paths, kind_filter=None):
    name_set = {str(n).casefold() for n in names if n}
    path_set = {item for p in paths if p for item in _path_equivalents(p)}
    cache_key = id(rows)
    indexes = _STAGING_INDEX_CACHE.get(cache_key)
    if indexes is None:
        by_name = defaultdict(list)
        by_path = defaultdict(list)
        for row in rows:
            if row.get("name"):
                by_name[str(row.get("name")).casefold()].append(row)
            path_values = list(_nested_path_values(row))
            if Path(str(row.get('name') or '')).suffix.casefold() in {'.bdae','.wav','.ogg','.mp3','.vxn','.dds','.png','.lua','.luac'}:
                path_values.append(row['name'])
            for value in path_values:
                for normalized in _path_equivalents(value):
                    by_path[normalized].append(row)
        indexes = (by_name, by_path)
        _STAGING_INDEX_CACHE.clear()
        _STAGING_INDEX_CACHE[cache_key] = indexes
    by_name, by_path = indexes
    candidate_rows = {}
    for name in name_set:
        for row in by_name.get(name, []):
            candidate_rows[id(row)] = row
    for path in path_set:
        for row in by_path.get(path, []):
            candidate_rows[id(row)] = row
    matches = []
    for row in candidate_rows.values():
        if kind_filter and str(row.get("kind", "")).casefold() not in kind_filter:
            continue
        exact_name = str(row.get("name", "")).casefold() in name_set
        row_paths = {item for value in (_nested_path_values(row)) for item in _path_equivalents(value)}
        if Path(str(row.get('name') or '')).suffix.casefold() in {'.bdae','.wav','.ogg','.mp3','.vxn','.dds','.png','.lua','.luac'}:
            row_paths.update(_path_equivalents(row['name']))
        exact_path = bool(path_set & row_paths)
        if exact_name or exact_path:
            matches.append((row, "exact_resource_path" if exact_path else "exact_resource_name"))
    unique = {}
    for r, association in matches:
        key = r.get("reference_id") or r.get("id")
        unique[key] = {"reference_id": key, "act": r.get("classification_act"),
                       "kind": r.get("kind"), "name": r.get("name"),
                       "classification": r.get("classification"), "reason": r.get("reason"),
                       "evidence": r.get("evidence"), "source_candidates": r.get("source_candidates", []),
                       "port_candidates": r.get("port_candidates", []), "selection_files": r.get("selection_files", []),
                       "classification_file": r.get("classification_file"),
                       "association_method": association, "confidence": "PROVEN",
                       "claim_scope": "package_B_current_classification_and_copy_selection_cross_join"}
    return [unique[k] for k in sorted(unique, key=lambda x: str(x))]


def _normalize_resource_path(value):
    value = str(value or "").replace("\\", "/").casefold()
    if "/assets-extra/" in value:
        value = "assets-extra/" + value.split("/assets-extra/", 1)[1]
    if value.startswith("assets-extra/"):
        value = value[len("assets-extra/"):]
        for platform in ("android/", "ios/"):
            if value.startswith(platform):
                value = value[len(platform):]
                break
    if "/.local-inputs/" in value:
        value = value.split("/.local-inputs/", 1)[1]
    value = value.lstrip("/")
    if value.startswith('assets/data/'):
        value = value[len('assets/'):]
    while value.startswith("./"):
        value = value[2:]
    return value


def _path_equivalents(value):
    normalized = _normalize_resource_path(value)
    result = {normalized} if normalized else set()
    if normalized.startswith("data/"):
        result.add(normalized[5:])
    else:
        result.add("data/" + normalized)
    return result


def _nested_path_values(value):
    if isinstance(value, dict):
        for key, child in value.items():
            if isinstance(child, str) and ("path" in key.casefold() or "file" in key.casefold() or key.casefold() in {"destination", "source_relative", "uri"}):
                yield child
            else:
                yield from _nested_path_values(child)
    elif isinstance(value, list):
        for child in value:
            yield from _nested_path_values(child)


def _page_manifest(ctx):
    progress = read_json(Path(ctx.out) / "r2/package-a-progress.json", {})
    pages = {str(row.get("name", "")).casefold(): dict(row) for row in progress.get("page_manifest", []) if row.get("name")}
    refined = read_json(Path(ctx.out) / "r2/natives/scope-refined.json", {})
    for row in refined.get("rows", []):
        page = pages.setdefault(str(row.get("name", "")).casefold(), {"name": row.get("name")})
        manual = row.get("manual_contract") or {}
        contract=row.get('contract') or {}
        if manual.get("port_status") or contract.get('port_status'):
            page["current_port_semantic_status"] = manual.get("port_status") or contract.get('port_status')
            page["current_port_difference"] = manual.get("port_difference") or contract.get('port_reason')
            page["current_port_evidence"] = manual.get("port_evidence") or contract.get('port_evidence',[])
            if not manual.get('port_difference') and page.get('path'):
                spec=Path(ctx.out)/page['path']
                if spec.is_file():
                    match=re.search(r'\*\*Concrete behavioral difference:\*\* ([^\n]+)',spec.read_text(encoding='utf-8'))
                    if match:page['current_port_difference']=match.group(1)
        page["refined_row_state"] = row.get("state")
    return pages


def _resource_names_paths(resource):
    names = set()
    for key, value in (resource or {}).items():
        if isinstance(value, dict):
            if value.get("name"):
                names.add(str(value["name"]))
            if value.get("table_link"):
                link = value["table_link"]
                for sub in (link.get("reference"), link.get("target")):
                    if isinstance(sub, dict) and sub.get("name"):
                        names.add(str(sub["name"]))
            if key == "state_rows":
                for state in value.values():
                    if isinstance(state, dict) and state.get("name"):
                        names.add(str(state["name"]))
        elif isinstance(value, list):
            names.update(str(x) for x in value if isinstance(x, str))
    paths = sorted({p for p in _nested_path_values(resource) if Path(str(p)).suffix.casefold() in
                    {".bdae", ".ogg", ".wav", ".mp3", ".mp4", ".bin", ".dds", ".png", ".lua", ".luac"}})
    return sorted(names, key=str.casefold), paths


def _resource_stage_evidence(staging_rows, family, resource):
    kinds = {
        "model": {"model", "model_component", "data_file"},
        "animation": {"animation_dictionary", "anim", "animation_event", "data_file"},
        "sound": {"sound", "source_sound_ordinal", "data_file"},
        "loot": {"loot", "data_file"},
    }
    names, paths = _resource_names_paths(resource)
    refs = _staging_crossrefs(staging_rows, names, paths, kinds.get(family))
    file_dependencies=[]
    for dep_path in paths:
        if Path(dep_path).suffix.casefold() not in {'.bdae','.wav','.ogg','.mp3','.vxn','.dds','.png','.mp4','.lua','.luac'}:
            continue
        file_refs=[r for r in _staging_crossrefs(staging_rows,[],[dep_path],kinds.get(family))
                   if r.get('classification')!='NOT_NEEDED']
        classes={r.get('classification') for r in file_refs}
        if not classes:
            dep_status='UNRESOLVED'
            reason='No exact Package B file classification; a typed parent key or diagnostic filename candidate does not establish media availability.'
        elif 'RESOLVED_STAGED' in classes:
            dep_status='RESOLVED_STAGED';reason=None
        elif classes <= {'ORIGINAL_EXISTS_NOT_STAGED'}:
            dep_status='ORIGINAL_EXISTS_NOT_STAGED';reason='Original file has an exact copy selection but is not in the frozen staged package.'
        else:
            dep_status='UNRESOLVED'
            reason='; '.join(sorted({str(r.get('reason')) for r in file_refs if r.get('reason')})) or 'Exact file classification remains ambiguous or unresolved.'
        file_dependencies.append({'path':dep_path,'status':dep_status,'reason':reason,'references':file_refs,
                                  'claim_scope':'logical_resource_path_staging_receipt; source-platform/native equivalence is not asserted'})
    if file_dependencies:
        classes={r['status'] for r in file_dependencies}
        status='RESOLVED_STAGED' if classes=={'RESOLVED_STAGED'} else 'ORIGINAL_EXISTS_NOT_STAGED' if classes<={'RESOLVED_STAGED','ORIGINAL_EXISTS_NOT_STAGED'} else 'UNRESOLVED'
        return {'status':status,'reason':None if status=='RESOLVED_STAGED' else 'At least one linked file requires staging or an exact classification; inspect file_dependencies.',
                'source_names':names,'source_paths':paths,'references':refs,'file_dependencies':file_dependencies}
    if not refs:
        return {"status": "UNRESOLVED", "reason": "No exact current Package B act_2_inferred classification row matched this linked resource name or path; source-table presence is not staging evidence.",
                "source_names": names, "source_paths": paths, "references": []}
    classifications = {str(r.get("classification", "UNRESOLVED")) for r in refs}
    if "RESOLVED_STAGED" in classifications:
        status = "RESOLVED_STAGED"
    elif "ORIGINAL_EXISTS_NOT_STAGED" in classifications:
        status = "ORIGINAL_EXISTS_NOT_STAGED"
    elif classifications == {"NOT_NEEDED"}:
        status = "NOT_NEEDED_STAGE_ONLY"
    elif "UNRESOLVED" in classifications or "AMBIGUOUS" in classifications:
        status = "UNRESOLVED"
    else:
        status = sorted(classifications)[0]
    return {"status": status, "reason": None if status == "RESOLVED_STAGED" else "; ".join(sorted({str(r.get("reason")) for r in refs if r.get("reason")})),
            "source_names": names, "source_paths": paths, "references": refs}


def _is_boss_candidate(actor, placement):
    name = " ".join(str(x or "") for x in [actor.get("name"), placement.get("charpropsname"),
                                             (actor.get("ai_profile") or {}).get("name"),
                                             actor.get("ai_script")]).casefold()
    return any(token in name for token in ("boss", "midboss", "demiboss", "champion", "queen", "king",
                                            "dragon", "madruk", "darklin", "sandworm", "root_troll", "treething"))


def build_dossier(ctx, current_scope):
    ix = _prep_indexes(_source_indexes(ctx))
    native_scope = read_json(Path(ctx.out) / "r2/natives/scope.json", {})
    native_progress = read_json(Path(ctx.out) / "r2/package-a-progress.json", {})
    staging_progress = read_json(Path(ctx.out) / "r2/package-b-progress.json", {})
    native_ready = bool(native_progress.get("total")) and native_progress.get("processed") == native_progress.get("total")
    staging_ready = bool(staging_progress.get("total")) and staging_progress.get("processed") == staging_progress.get("total")
    staging = _staging_rows(ctx)
    native_pages = _page_manifest(ctx)
    source_snapshots = []
    for rel in ["r2/package-a-progress.json", "r2/natives/scope.json", "r2/natives/scope-refined.json", "r2/package-b-progress.json"]:
        path = Path(ctx.out) / rel
        if path.is_file():
            source_snapshots.append({"path": rel, "sha256": hashlib.sha256(path.read_bytes()).hexdigest()})
    for rel in sorted(set(staging_progress.get("classification_files", []))):
        path = Path(ctx.out) / rel
        if path.is_file():
            source_snapshots.append({"path": rel, "sha256": hashlib.sha256(path.read_bytes()).hexdigest()})
    script_index = _script_source_index(ctx)
    native_names = {str(r.get("name")) for r in native_scope.get("native_rows", []) if r.get("name")}
    dossiers = []
    entity_rows = {}
    for scene in current_scope["levels"]:
        source_level = ix["levels"].get(str(scene.get("scene", "")).casefold(), {})
        campaign = ix["campaign"].get(str(scene.get("scene", "")).casefold())
        campaign_detail = _campaign_detail(ctx, ix, scene, campaign)
        placements = _scene_placements(ctx, scene, current_scope, ix)
        actor_ids = []
        for placement in placements:
            actor_id = placement.get("actor_id")
            if actor_id and actor_id not in entity_rows:
                actor = ix["actors_by_id"].get(actor_id, {})
                linked = _asset_ref_for_actor(ix, actor)
                ai_script = (actor.get("ai_profile") or {}).get("fields", {}).get("Script") or actor.get("ai_script")
                chain = _lua_chain(ctx, ai_script, script_index, native_names)
                entity_rows[actor_id] = {
                    "actor_id": actor_id, "actor_name": actor.get("name"),
                    "source_table": actor.get("source_table"), "character_properties_fields": actor.get("fields"),
                    "field_links": actor.get("field_links"), "AI": {"profile": actor.get("ai_profile"),
                        "script": ai_script, "source_chain": chain,
                        "confidence": "PROVEN", "claim_scope": "character_AI_profile_row"},
                    "resources": linked,
                    "package_B_resource_crossrefs": _staging_crossrefs(
                        staging, [actor.get("name")],
                        [linked.get("model", {}).get("path")]
                        + [r.get("path") for r in linked.get("animation", {}).get("state_rows", {}).values() if r.get("path")]),
                    "package_B_resource_dependencies": {family: _resource_stage_evidence(staging, family, linked.get(family, {}))
                                                         for family in ("model", "animation", "sound", "loot")},
                    "package_A_native_callsite_specs": _lua_native_links(native_scope, chain.get("files", []), native_pages),
                    "trophy_and_skill_fields": {"skills": actor.get("skills"),
                                                  "trophy": (actor.get("ai_profile") or {}).get("fields", {}).get("Trophy")},
                    "source_confidence": "PROVEN", "claim_scope": "character_properties_and_linked_tables",
                    "source_port_snapshot": {"source_atlas_status": actor.get("status"),
                                              "source_atlas_work_status": actor.get("work_status"),
                                              "production_runtime_status": "UNVERIFIED",
                                              "reason": "Source table linkage does not prove resource loading or actor behavior in the Windows production path."},
                    "status": "DONE" if actor.get("field_links") else "UNRESOLVED",
                }
            if actor_id:
                actor_ids.append(actor_id)
        actor_by_id = ix["actors_by_id"]
        bosses = []
        for placement in placements:
            actor = actor_by_id.get(placement.get("actor_id"), {})
            if not actor or not _is_boss_candidate(actor, placement):
                continue
            ai = entity_rows.get(actor.get("id"), {}).get("AI", {})
            files = (ai.get("source_chain") or {}).get("files", [])
            phase_lines = [line for f in files for line in f.get("phase_and_behavior_evidence_lines", [])
                           if re.search(r"phase|form|health|\bhp\b|dying|summon|hasChangedForm|BOSS_MUSIC", line.get("text", ""), re.I)]
            bosses.append({"actor_id": actor.get("id"), "actor_name": actor.get("name"),
                           "placement_id": placement.get("placement_id"),
                           "boss_candidate_confidence": "LIKELY", "claim_scope": "name_AI_profile_candidate",
                           "phase_table": [{"source": f.get("source"), "line": row.get("line"),
                                            "authored_expression": row.get("text"), "confidence": "PROVEN",
                                            "claim_scope": "original_Lua_source_line"}
                                           for f in files for row in f.get("phase_and_behavior_evidence_lines", [])
                                           if re.search(r"phase|form|health|\bhp\b|dying|summon|hasChangedForm|BOSS_MUSIC", row.get("text", ""), re.I)],
                           "phase_table_status": "RESOLVED" if phase_lines else "UNRESOLVED",
                           "phase_table_reason": None if phase_lines else "No explicit phase/health/form transition line matched in the exact AI include closure.",
                           "source_lua_closure": ai.get("source_chain"),
                           "port_runtime_status": "UNVERIFIED",
                           "port_runtime_blocker": "Original Lua evidence and native callsites do not establish runtime boss phase behavior in the Windows build.",
                           "package_A_native_specs": _lua_native_links(native_scope, files, native_pages),
                           "package_A_status": "DONE" if native_ready else "TODO" if native_scope.get("native_rows") else "PENDING_SCOPE"})
        outgoing = [e for e in current_scope["edges"] if e.get("source_scene_id") == scene.get("scene_id")]
        incoming = [e for e in current_scope["edges"] if e.get("target_scene_id") == scene.get("scene_id")]
        config = source_level.get("config") or {}
        music_name = config.get("music")
        safezone_name = config.get("safezone_music")
        music_rows = []
        for authored_name in dict.fromkeys(x for x in (music_name, safezone_name) if x):
            for row in ix["table_by_name"].get(str(authored_name).casefold(), []):
                music_rows.append({"authored_name": authored_name, "table": row.get("table"), "index": row.get("index"),
                                   "uid": row.get("uid", (row.get("fields") or {}).get("uid")), "id": row.get("id"), "fields": row.get("fields"),
                                   "source": row.get("source"), "confidence": "PROVEN",
                                   "claim_scope": "exact_round1_table_row_name_match"})
        music_refs = _staging_crossrefs(staging, [music_name, safezone_name], [], {"sound", "music", "audio", "data_file"})
        music_classes = {str(r.get("classification")) for r in music_refs}
        if "RESOLVED_STAGED" in music_classes:
            music_stage_status = "RESOLVED_STAGED"
        elif "ORIGINAL_EXISTS_NOT_STAGED" in music_classes:
            music_stage_status = "ORIGINAL_EXISTS_NOT_STAGED"
        elif music_refs and music_classes == {"NOT_NEEDED"}:
            music_stage_status = "NOT_NEEDED_STAGE_ONLY"
        else:
            music_stage_status = "UNRESOLVED"
        music = {"level_music": music_name, "safezone_music": safezone_name,
                 "source_scene": scene.get("source"), "confidence": "PROVEN", "claim_scope": "LevelConfig_authored_value",
                 "source_table_rows": music_rows,
                 "source_table_match_status": "RESOLVED" if music_rows else "UNRESOLVED",
                 "source_table_reason": None if music_rows else "No exact authored music name row matched the round-1 table index.",
                 "package_B_sound_classifications": music_refs,
                 "package_B_sound_status": music_stage_status,
                 "package_B_sound_reason": None if music_stage_status == "RESOLVED_STAGED" else "; ".join(sorted({str(r.get("reason")) for r in music_refs if r.get("reason")})) or "No exact current Package B sound/music/audio classification matched the authored music event name.",
                 "runtime_music_support": "UNVERIFIED_STATIC_SOURCE_ONLY"}
        quest_dialogue = {**campaign_detail,
                          "source_transition_conditions": sorted({e.get("activation_condition") for e in outgoing if e.get("activation_condition")}),
                          "port_status_summary": Counter(c.get("port_status", "UNKNOWN") for s in (campaign or {}).get("command_readiness", []) for c in [s]),
                          "runtime_dialogue_support": "UNVERIFIED_STATIC_SOURCE_ONLY"}
        loot_summaries = []
        for actor_id in sorted(set(actor_ids)):
            entity = entity_rows[actor_id]
            loot = entity["resources"]["loot"]
            row = loot.get("row") or {}
            if row:
                loot_summaries.append({"actor_id": actor_id, "actor_name": entity.get("actor_name"),
                                       "loot_table": row.get("name"), "table": row.get("table"),
                                       "fields": row.get("fields"), "confidence": "PROVEN",
                                       "port_runtime_status": "UNVERIFIED",
                                       "port_runtime_blocker": "Static loot-table linkage does not verify the Windows drop path.",
                                       "claim_scope": "character_Loot_field_link"})
        names = [entity_rows[x].get("actor_name") for x in sorted(set(actor_ids))]
        paths = [m.get("path") for m in current_scope["module_sources"] if m.get("scene_id") == scene.get("scene_id")]
        b_crossrefs = _staging_crossrefs(staging, names, paths)
        Afiles = [f for actor_id in set(actor_ids) for f in (entity_rows[actor_id].get("AI", {}).get("source_chain") or {}).get("files", [])]
        dossier = {
            "schema": "atlas-r2-act2-level-dossier-v1",
            "scene_id": scene.get("scene_id"), "scene": scene.get("scene"),
            "membership": {"candidate": scene.get("membership_candidate"), "confidence": scene.get("membership_confidence", "GUESS"),
                           "evidence": scene.get("membership_evidence"), "claim_scope": "narrative_act_boundary"},
            "source_scene": {**_source_receipt(ctx, scene.get("source")), "path": scene.get("source"), "level_config": config,
                             "direct_modules": source_level.get("direct_objects", []),
                             "procedural_module_candidates": source_level.get("procedural_lists", []),
                             "source_confidence": "PROVEN", "claim_scope": "scene_file_authored_declarations"},
            "transitions": {"outgoing": outgoing, "incoming": incoming,
                            "unresolved_frontiers": [f for f in current_scope["frontier"] if f.get("scene") == scene.get("scene")],
                            "status": "DONE" if outgoing or incoming else "NOT-NEEDED",
                            "confidence": "PROVEN", "claim_scope": "authored_transition_source"},
            "entities": {"placement_count": len(placements), "unique_actor_count": len(set(actor_ids)),
                         "placements": placements, "actor_ids": sorted(set(actor_ids)),
                         "status": "DONE" if placements else "UNRESOLVED",
                         "source_confidence": "LIKELY" if any(p.get("module_conditional_choice") for p in placements) else "PROVEN",
                         "claim_scope": "MGP_static_Character_definitions_not_runtime_spawns"},
            "entity_rows": [{"actor_id": x, "actor_name": entity_rows[x].get("actor_name"),
                              "details_path": f"entities/{x}.json", "status": "LINKED_TO_ENTITY_SHARD",
                              "confidence": "PROVEN", "claim_scope": "scene_actor_id_to_detailed_entity_record"}
                             for x in sorted(set(actor_ids))],
            "quest_dialogue": quest_dialogue,
            "boss_phases": {"candidates": bosses, "status": "DONE" if bosses else "NOT-NEEDED",
                             "reason": None if bosses else "No actor name/AI profile in this static placement set matched the conservative boss-candidate heuristic."},
            "music": music,
            "items_loot": {"actor_loot_rows": loot_summaries,
                            "status": "DONE" if loot_summaries else "NOT-NEEDED",
                            "reason": None if loot_summaries else "No linked actor loot row in the current placement set."},
            "readiness": {"source_semantics": "Static source-linked inventory; direct authored attributes PROVEN, procedural membership LIKELY.",
                          "staging": {"package": "B", "ledger_completion_status": "DONE" if staging_ready else "TODO",
                                      "processed": staging_progress.get("processed", 0), "total": staging_progress.get("total", 0),
                                      "cross_reference_count": len(b_crossrefs), "references": b_crossrefs,
                                      "cross_reference_status": "MATCHED" if b_crossrefs else "UNRESOLVED",
                                      "cross_reference_reason": None if b_crossrefs else "No exact current Package B classification matched the scene's actor names or module source paths."},
                          "native_contracts": {"package": "A", "ledger_completion_status": "DONE" if native_ready else "TODO" if native_scope.get("native_rows") else "PENDING_SCOPE",
                                               "processed": native_progress.get("processed", 0), "total": native_progress.get("total", 0),
                                               "AI_lua_call_matches": _lua_native_links(native_scope, Afiles, native_pages)},
                          "port_runtime": {"status": "UNVERIFIED", "reason": "Static atlas extraction does not execute level loading, actor AI, quest/dialogue, music, boss phases or loot in the Windows production path."},
                          "visual_runtime": {"status": "NOT_RUN", "reason": "This source-only dossier makes no gameplay footage or rendered-state claim."}},
            "status": "DONE",
        }
        dossiers.append(dossier)
    return {"schema": "atlas-r2-act2-dossiers-v1", "package": "C-act2",
            "source_scope": "act2/scope-current.json", "level_count": len(dossiers),
            "cross_join_source_snapshots": source_snapshots,
            "levels": dossiers,
            "entities": list(entity_rows.values()),
            "entity_count": len(entity_rows),
            "confidence_policy": {"PROVEN": "exact source field/table/IDA or line evidence", "LIKELY": "source association or inferred boundary", "GUESS": "unknown membership/completeness only"},
            "support_policy": "Asset/source presence, staging-reference status, native contract state, Windows production support and runtime/visual verification are distinct fields; asset presence alone never increments playable support."}


def render_markdown(dossier, current_scope, expansion):
    lines = ["# Act 2 inferred readiness dossier", "",
             f"Static source inventory covers **{dossier['level_count']} reachable scenes**, **{current_scope['counts']['source_edges']} authored transition edges**, and **{dossier['entity_count']} unique Character-table rows**. The narrative Act boundary remains INFERENCE.", "",
             "This dossier is static-source evidence. It does not claim that any scene, actor, boss, quest, sound, or loot path runs in the Windows production game. Original-source semantics, resource staging, port logic, and runtime/visual verification are recorded separately.", "",
             "## Scope expansion and parser repairs", "",
             f"Recovered **{len(expansion['added_levels'])} new scenes**, **{len(expansion['added_edges'])} new authored edges**, and **{len(expansion['added_module_sources'])} scene/platform module sources** beyond frozen Step 0. Eleven original malformed-rule frontiers now have narrow, in-memory repair receipts. The source bytes were not edited.", "",
             "Remaining transition frontiers are kept explicit:", ""]
    for row in current_scope["frontier"]:
        lines.append(f"- **{row.get('reason')}** (`{row.get('confidence','GUESS')}`): {row.get('detail', row.get('raw_path', row.get('scene', 'source unresolved')))}")
    lines.extend(["", "## Per-level inventory", "",
                  "Each linked JSON row carries the actor placements and Character rows, model/animation/sound/AI/loot source chains, campaign script and dialogue-command references, boss Lua evidence where present, B staging reference IDs, A native-contract source sites, and separate port/runtime status.", "",
                  "| Scene | Membership | Static placements | Linked actor rows | Scripts / commands | Boss candidates | Music | Loot | Port/runtime |",
                  "| --- | --- | ---: | ---: | ---: | ---: | --- | --- | --- |"])
    for level in dossier["levels"]:
        q = level["quest_dialogue"]
        lines.append(f"| {level['scene']} | {level['membership']['candidate']} ({level['membership']['confidence']}) | {level['entities']['placement_count']} | {level['entities']['unique_actor_count']} | {q.get('script_count',0)} / {q.get('command_count',0)} | {len(level['boss_phases']['candidates'])} | {level['music'].get('level_music') or 'unresolved'} | {len(level['items_loot']['actor_loot_rows'])} | UNVERIFIED / NOT RUN |")
    lines.extend(["", "## Readiness fractions", "",
                  "The conservative production-capability fraction counts a required per-level capability as supported only when source contract, package A/B cross-joins, frozen Windows production path, and integrated runtime/visual evidence are all present. This package is static-only and has not run those paths, so the verified-playable numerator is **0**. The explicit denominator is the count of required capability rows emitted in `coverage.json`; original asset presence does not increase it.", "",
                  "The separate static source-resolution fraction is reported in `coverage.json`. It measures source joins only and must not be read as playable coverage.", "",
                  "## Detailed artifacts", "",
                  "- `scope.json`: frozen Step 0 enumeration (preserved).",
                  "- `scope-expansion.json`: evidence-backed additions and source-repair receipts; new rows start TODO.",
                  "- `scope-current.json`: refreshed full transition closure for dossier joins.",
                  "- `dossier.json`: full per-level and per-entity evidence links.",
                  "- `levels/`: per-scene JSON and Markdown dossiers; actor details link to `entities/` shards.",
                  "- `entities/`: per-Character JSON and Markdown source-link records.",
                  "- `coverage.json` / `coverage.md`: explicit denominators, capability metric, and source-resolution counts.",
                  "- `blocker-reach.json` / `blocker-reach.md`: implementation order ranked by affected-scene reach.", ""])
    return "\n".join(lines)


def write_coverage(path, dossier, current_scope, staging_ready=False, native_ready=False):
    output_path = Path(path)
    capability_types = ["scene_transition", "actor_spawn_and_model", "AI_and_state", "quest_dialogue", "music_audio", "items_loot"]
    rows = []
    for level in dossier["levels"]:
        for cap in capability_types:
            rows.append({"scene_id": level["scene_id"], "scene": level["scene"], "capability": cap,
                         "required": True,
                         "source_evidence_status": "PROVEN" if cap in {"scene_transition", "AI_and_state", "quest_dialogue"} else "LIKELY",
                         "staging_status": "REVIEW_COMPLETE" if staging_ready else "REVIEW_PENDING",
                         "native_contract_status": "REVIEW_COMPLETE" if native_ready else "REVIEW_PENDING",
                         "staging_means": "Package B classification ledger reviewed; individual dependency states remain in source_availability_metrics.",
                         "native_means": "Package A contract ledger reviewed; current semantic support remains in source_availability_metrics.",
                         "production_path_status": "UNVERIFIED",
                         "runtime_visual_status": "NOT_RUN",
                         "playable_supported": False,
                         "reason": "No integrated runtime/visual verification in this static-source package."})
    asset_requirement_count = sum(1 for actor in dossier["entities"] for resource in actor["resources"].values()
                                  if isinstance(resource, dict) and resource.get("status") == "RESOLVED")
    asset_file_links = sum(1 for actor in dossier["entities"] for c in (actor["resources"].get("model", {}).get("asset_candidates", [])))
    resource_dependencies = {}
    native_dependencies = {}
    resource_extensions = {".bdae", ".ogg", ".wav", ".mp3", ".vxn", ".dds", ".png", ".mp4"}
    for actor in dossier["entities"]:
        for family, evidence in actor.get("package_B_resource_dependencies", {}).items():
            for dependency in evidence.get('file_dependencies',[]):
                dep_path=dependency['path']
                if Path(str(dep_path)).suffix.casefold() not in resource_extensions:
                    continue
                key = (family, _normalize_resource_path(dep_path))
                resource_dependencies[key] = dependency
        for link in actor.get("package_A_native_callsite_specs", []):
            for call in link.get("call_sites", []):
                key = (link.get("native_name"), call.get("call_id"), call.get("script"), call.get("line"))
                native_dependencies[key] = link
    for level in dossier["levels"]:
        for ref in level.get("music", {}).get("package_B_sound_classifications", []):
            resource_dependencies[("music", ref.get("reference_id"))] = {"status": ref.get("classification", "UNRESOLVED")}
    resource_counts = Counter(str(v.get("status", "UNRESOLVED")) for v in resource_dependencies.values())
    native_port_counts = Counter(str(v.get("current_port_semantic_status", "UNKNOWN")) for v in native_dependencies.values())
    native_contract_counts = Counter(str(v.get("contract_state", "UNAVAILABLE")) for v in native_dependencies.values())
    source_metrics = {
        "resource_file_dependencies": {
            "denominator": len(resource_dependencies), "status_counts": dict(sorted(resource_counts.items())),
            "resolved_staged": resource_counts.get("RESOLVED_STAGED", 0),
            "metric_scope": "distinct linked or candidate logical model/animation/sound/loot media paths; only exact file-level Package B receipts count as staged",
            "not_playability": "Source or staging availability does not establish Windows loading or gameplay support."},
        "in_scope_ai_native_call_dependencies": {
            "denominator": len(native_dependencies), "port_semantic_status_counts": dict(sorted(native_port_counts.items())),
            "contract_page_status_counts": dict(sorted(native_contract_counts.items())),
            "metric_scope": "distinct native call IDs in the extracted AI Lua include closure for current Act 2 reachable scenes",
            "not_playability": "A contract page marked DONE means a source contract dossier exists; port semantic status remains separately reported."}}
    result = {"schema": "atlas-r2-act2-coverage-v1", "metric_policy": {
                   "production_capability_metric": "Per-level required capability row is numerator-eligible only when original semantics, staging and native cross-joins, Windows production path, and real runtime/visual verification are all evidenced. Asset presence alone is excluded.",
                   "static_source_resolution_metric": "Resolved source table/asset relationships only; not gameplay or staging readiness.",
                   "source_availability_metrics": "Separate explicit denominators; do not combine or average these metrics into playable coverage."},
              "counts": {"scenes": len(dossier["levels"]), "required_capabilities": len(rows),
                         "verified_playable_supported": sum(1 for r in rows if r["playable_supported"]),
                         "verified_playable_fraction": {"numerator": 0, "denominator": len(rows), "percent": 0.0},
                         "unique_entity_rows": dossier["entity_count"], "resolved_entity_resource_links": asset_requirement_count,
                         "model_asset_file_links": asset_file_links,
                         "source_edges": current_scope["counts"]["source_edges"],
                         "unresolved_frontier_rows": len(current_scope["frontier"])},
              "required_capabilities": rows,
              "source_availability_metrics": source_metrics,
              "not_a_claim": "This fraction is conservative by design; no runtime evidence is supplied here, so no rows are called playable."}
    write_json(output_path, result)
    return result
