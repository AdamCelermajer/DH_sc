"""Static Act 2 transition-scope inventory for Atlas round 2.

This module reads the generated round-1 indexes and original scene/MGP sources.
It does not execute scripts, procedural rules, or transitions. Every emitted
work row starts TODO so root can release the package scope before dossier work.
"""
from __future__ import annotations

import hashlib
import json
import re
import xml.etree.ElementTree as ET
from collections import Counter, defaultdict, deque
from pathlib import Path

from common import physical_path, read_json, source_site, stable_id, write_json


PACKAGE = "act2"
START_SCENE = "003_darkwood.mlx"
KNOWN_ACT1_BACKLINKS = {
    "001_swamp.mlx": "The v21.8 handoff scopes Act 1 as Swamp and Witch Cave before the Darkwood load.",
    "002_swamp_witchcave.rule.xml": "The v21.8 handoff scopes Act 1 as Swamp and Witch Cave before the Darkwood load.",
}
EVIDENCE = {
    "scene_and_rules": "Original Android scene .mlx/.rule.xml source; direct Module.mgp values and every procedural <elem gameplay=...> candidate are retained.",
    "module_exit": "Original Android MGP GameObject gametype=TriggerZoneExitLevel and exact levelName property; nested Module.mgp values are recursively followed.",
    "script_exit": "Round-1 campaign-references.json Script_ChangeLevel reference joined to campaign-relations.json destination_level and levels#group1.",
    "native_consumer": "IDA pseudocode-all.c TriggerZoneExitLevel::DeclareProperties 0x0039c8f8 binds levelName; TriggerZoneExitLevel::Update 0x0039c33c gates touch/condition/local-host and exposes the travel prompt. Script_ChangeLevel::Execute is 0x0045cba0.",
}


def _case_child(root: Path, rel: str) -> Path | None:
    cur = root
    for part in Path(rel.replace("\\", "/")).parts:
        if not cur.is_dir():
            return None
        child = next((p for p in cur.iterdir() if p.name.casefold() == part.casefold()), None)
        if child is None:
            return None
        cur = child
    return cur if cur.is_file() else None


def _module_rel(value: str) -> str | None:
    match = re.search(r"(?:^|/)(?:iphone/)?3d/modules/(.+)$", value.replace("\\", "/"), re.I)
    return match.group(1) if match else None


def _source_roots(ctx):
    inputs = Path(ctx.inputs)
    return [
        ("android", inputs / "assets-extra/android/data/3d/modules"),
        ("original-cache", inputs / "assets-extra/android/original-cache/data/3d/modules"),
        ("ios", inputs / "assets-extra/ios/data/3d/modules"),
    ]


def _module_variants(ctx, raw_path: str, rule_base: str | None = None):
    rel = _module_rel(raw_path) or raw_path.replace("\\", "/")
    if rule_base:
        base_rel = _module_rel(rule_base) or rule_base.replace("\\", "/")
        rel = f"{base_rel.rstrip('/')}/mgp/{rel.lstrip('/')}"
    found = []
    for platform, root in _source_roots(ctx):
        path = _case_child(root, rel)
        if path:
            found.append({"platform": platform, "path": str(path), "relative": rel})
    return found


def _scene_source(ctx, scene):
    source = str(scene.get("source") or "")
    if source:
        candidate = physical_path(ctx, source)
        if candidate.is_file():
            return candidate
    fallback = Path(ctx.inputs) / "assets-extra/android/data/scene" / str(scene.get("file", ""))
    return fallback if fallback.is_file() else None


def _parse_scene_xml(source, scene_name):
    """Recover only audited lexical/tag-closure defects in source XML in memory."""
    raw = source.read_bytes()
    text = raw.decode("utf-8", "replace")
    repairs = []
    quoted, count = (text, 0)
    if scene_name in {"006_basement.rule.xml", "007_crypt_01.rule.xml", "010_el_lightning.rule.xml",
                      "014_el_earth.rule.xml", "020_crypt_02.rule.xml", "023_watertemple.rule.xml",
                      "024_el_water.rule.xml", "030_el_air.rule.xml", "032_el_fire.rule.xml"}:
        quoted, count = re.subn(r"(\b[A-Za-z_:][\w:.-]*\s*=\s*)(true|false)(?=\s|/?>)", r'\1"\2"', text, flags=re.I)
    if count:
        text = quoted
        repairs.append("quoted_unquoted_boolean_attributes")
    if scene_name in {"004_root_den.rule.xml", "006_basement.rule.xml", "019_light_house_02.rule.xml", "020_crypt_02.rule.xml"}:
        normalized = re.sub(r"</?Forceblock\b", lambda m: "</ForceBlock" if m.group(0).startswith("</") else "<ForceBlock", text)
        if normalized != text:
            text = normalized
            repairs.append("normalized_ForceBlock_case_variants")
    if scene_name in {"006_basement.rule.xml", "020_crypt_02.rule.xml"}:
        before = text
        # Exact source shape: one redundant nested ForceBlock close between the
        # authored path list and the enclosing Path. Preserve the remaining tree.
        text = re.sub(r"(</ForceBlock>\s*)</ForceBlock>(\s*</Path>\s*</RootRule>)", r"\1\2", text, count=1)
        if text != before:
            repairs.append("removed_source_surplus_ForceBlock_close")
    if scene_name == "019_light_house_02.rule.xml":
        before = text
        text = re.sub(r"</RootRule>\s*</rules>\s*</RootRule>\s*</rules>\s*$", "</RootRule>\n</rules>\n", text, count=1)
        if text != before:
            repairs.append("removed_duplicate_trailing_root_closures")
    try:
        return ET.fromstring(text), repairs, None
    except ET.ParseError as exc:
        return None, repairs, str(exc)


def _scene_module_roots(ctx, scene):
    source = _scene_source(ctx, scene)
    if source is None:
        return [], [{"status": "TODO", "reason": "scene_source_missing", "scene": scene.get("file")}], []
    try:
        root, repairs, parse_error = _parse_scene_xml(source, str(scene.get("file", "")))
    except OSError as exc:
        return [], [{"status": "TODO", "reason": "scene_parse_failed", "scene": scene.get("file"), "detail": str(exc)}], []
    if root is None:
        return [], [{"status": "TODO", "reason": "scene_parse_failed", "scene": scene.get("file"), "detail": parse_error}], []
    modules = []
    if root.tag.casefold() == "rules":
        folder = root.get("folder", "")
        for elem in root.findall(".//elem"):
            gameplay = elem.get("gameplay")
            if gameplay:
                modules.append({
                    "raw_path": gameplay,
                    "rule_base": folder,
                    "conditional": True,
                    "rule_source": str(source),
                    "rule_list": elem.getparent().get("name") if hasattr(elem, "getparent") else None,
                })
    else:
        for obj in root.findall(".//GameObject"):
            if obj.get("gametype") == "Module" and obj.get("mgp"):
                modules.append({"raw_path": obj.get("mgp"), "rule_base": None, "conditional": False,
                                "scene_source": str(source), "object": obj.get("name")})
    return modules, [], [{"scene": scene.get("file"), "source": source_site(ctx, source), "repairs": name,
                          "status": "DONE", "confidence": "LIKELY" if name in {"removed_source_surplus_ForceBlock_close", "removed_duplicate_trailing_root_closures"} else "PROVEN",
                          "confidence_note": ("Recovered tree association after narrow close-tag repair; authored attribute values remain separately evidenced."
                                              if name in {"removed_source_surplus_ForceBlock_close", "removed_duplicate_trailing_root_closures"}
                                              else "Lexical normalization preserves exact source attribute values."),
                          "claim_scope": "static_module_candidate_extraction"} for name in repairs]


def _parse_module(ctx, variant):
    try:
        return ET.parse(variant["path"]).getroot()
    except (ET.ParseError, OSError):
        return None


def _reachable_modules(ctx, scene):
    roots, frontier, source_repairs = _scene_module_roots(ctx, scene)
    queue = deque(roots)
    visited = set()
    exits = []
    module_rows = []
    while queue:
        item = queue.popleft()
        variants = _module_variants(ctx, item["raw_path"], item.get("rule_base"))
        if not variants:
            frontier.append({"status": "TODO", "reason": "module_source_missing", "raw_path": item["raw_path"],
                             "rule_base": item.get("rule_base"), "scene": scene.get("file"),
                             "conditional": item.get("conditional", False)})
            continue
        for variant in variants:
            key = (variant["platform"], variant["path"].casefold())
            if key in visited:
                continue
            visited.add(key)
            module_rows.append({**variant, "conditional": item.get("conditional", False), "status": "TODO"})
            root = _parse_module(ctx, variant)
            if root is None:
                frontier.append({"status": "TODO", "reason": "module_parse_failed", "source": variant["path"],
                                 "platform": variant["platform"], "scene": scene.get("file")})
                continue
            for obj in root.findall(".//GameObject"):
                if obj.get("gametype") == "Module" and obj.get("mgp"):
                    queue.append({"raw_path": obj.get("mgp"), "rule_base": None,
                                  "conditional": item.get("conditional", False), "included_from": variant["path"]})
                if obj.get("gametype") == "TriggerZoneExitLevel" and obj.get("levelName"):
                    exits.append({
                        "kind": "trigger_zone_exit_level",
                        "target_name": obj.get("levelName"),
                        "source_platform": variant["platform"],
                        "source_path": variant["path"],
                        "source_object": obj.get("name"),
                        "entrypoint_id": obj.get("entrypointID"),
                        "fasttravel_id": obj.get("fasttravel"),
                        "conditional_module_choice": bool(item.get("conditional", False)),
                        "activation_condition": obj.get("activate_cond"),
                        "deactivation_condition": obj.get("deactivate_cond"),
                        "status": "TODO",
                    })
    return module_rows, exits, frontier, source_repairs


def enumerate_scope(ctx):
    out = Path(ctx.out)
    scene_rows = [r for r in read_json(out / "data/levels.json", []) if r.get("record_type") == "scene_file"]
    tables = read_json(out / "data/tables.json", [])
    level_rows = [r for r in tables if r.get("table") == "levels#group1"]
    campaign_refs = read_json(out / "data/campaign-references.json", [])
    campaign_rels = read_json(out / "data/campaign-relations.json", [])
    scene_by_file = {str(r.get("file", "")).casefold(): r for r in scene_rows}
    scene_by_id = {r.get("scene_id"): r for r in read_json(out / "data/campaign.json", [])}
    campaign_by_scene = {str(r.get("scene", "")).casefold(): r for r in read_json(out / "data/campaign.json", [])}
    level_by_name = {}
    for row in level_rows:
        fields = row.get("fields") or {}
        target = scene_by_file.get(Path(str(fields.get("LevelFile", ""))).name.casefold())
        level_by_name.setdefault(str(row.get("name", "")).casefold(), []).append((row, target))
    rel_by_ref = {r.get("source_reference_id"): r for r in campaign_rels if r.get("relation") == "destination_level"}
    refs_by_scene = {}
    for ref in campaign_refs:
        if ref.get("command") == "Script_ChangeLevel":
            refs_by_scene.setdefault(ref.get("scene_id"), []).append(ref)

    start = scene_by_file.get(START_SCENE.casefold())
    if not start:
        raise RuntimeError(f"Start scene {START_SCENE} is absent from round-1 levels.json")
    pending = deque([start])
    reached = {start.get("id") or start.get("scene_id"): start}
    edges = []
    modules_seen = set()
    module_sources = {}
    frontier = []
    source_repairs = []
    while pending:
        scene = pending.popleft()
        modules, exits, missing, repairs = _reachable_modules(ctx, scene)
        source_repairs.extend(repairs)
        for module in modules:
            key = (module["platform"], module["path"].casefold())
            if key not in modules_seen:
                modules_seen.add(key)
            module["scene"] = scene.get("file")
            module["scene_id"] = scene.get("id")
            module_sources[(scene.get("id"), module["platform"], module["path"].casefold())] = module
        frontier.extend(missing)
        sources = [(e, None) for e in exits]
        for ref in refs_by_scene.get(scene.get("id"), []):
            rel = rel_by_ref.get(ref.get("id"))
            targets = []
            if rel:
                level_row = next((r for r in level_rows if r.get("id") == rel.get("target", {}).get("id")), None)
                if level_row:
                    target = scene_by_file.get(Path(str((level_row.get("fields") or {}).get("LevelFile", ""))).name.casefold())
                    if target:
                        targets.append((level_row, target))
            if not targets:
                targets = level_by_name.get(str(ref.get("name", "")).casefold(), [])
            edge = {
                "kind": "script_change_level",
                "target_name": ref.get("name"),
                "source_scene": scene.get("file"),
                "source_scene_id": scene.get("id"),
                "source_reference_id": ref.get("id"),
                "script_name": ref.get("script_name"),
                "script_index": ref.get("script_index"),
                "command_index": ref.get("command_index"),
                "field_offset": ref.get("field_offset"),
                "script_names_source": scene.get("source_names"),
                "script_commands_source": scene.get("source_commands"),
                "evidence_relation": rel.get("relation") if rel else None,
                "status": "TODO",
            }
            if len(targets) == 1:
                edge["target_level_row_id"] = targets[0][0].get("id")
                edge["target_scene"] = targets[0][1].get("file")
                edge["target_scene_id"] = targets[0][1].get("id")
            elif len(targets) > 1:
                edge["target_candidates"] = [{"level_row_id": t[0].get("id"), "scene": t[1].get("file"),
                                               "scene_id": t[1].get("id")} for t in targets]
                edge["resolution"] = "AMBIGUOUS"
            else:
                edge["resolution"] = "UNRESOLVED_TARGET"
            sources.append((edge, targets[0][1] if len(targets) == 1 else None))
        for edge, unused in sources:
            if edge["kind"] == "trigger_zone_exit_level":
                edge["source_scene"] = scene.get("file")
                edge["source_scene_id"] = scene.get("id")
                candidates = level_by_name.get(str(edge.get("target_name", "")).casefold(), [])
                if len(candidates) == 1 and candidates[0][1]:
                    edge["target_level_row_id"] = candidates[0][0].get("id")
                    edge["target_scene"] = candidates[0][1].get("file")
                    edge["target_scene_id"] = candidates[0][1].get("id")
                    target_scene = candidates[0][1]
                elif len(candidates) > 1:
                    edge["target_candidates"] = [{"level_row_id": t[0].get("id"), "scene": t[1].get("file") if t[1] else None,
                                                   "scene_id": t[1].get("id") if t[1] else None} for t in candidates]
                    edge["resolution"] = "AMBIGUOUS"
                    target_scene = None
                else:
                    edge["resolution"] = "UNRESOLVED_TARGET"
                    target_scene = None
            elif edge.get("target_scene"):
                target_scene = scene_by_file.get(str(edge["target_scene"]).casefold())
            else:
                target_scene = None
            edge["source_site"] = (edge.get("source_path") if edge["kind"] == "trigger_zone_exit_level"
                                    else f"campaign-references.json:{edge.get('source_reference_id')}")
            edge["confidence"] = "PROVEN"
            edge["claim_scope"] = "authored_source"
            if edge.get("kind") == "trigger_zone_exit_level":
                edge["association_confidence"] = "LIKELY" if edge.get("conditional_module_choice") else "PROVEN"
            else:
                edge["association_confidence"] = "PROVEN"
            edges.append(edge)
            if target_scene:
                target_key = target_scene.get("id") or target_scene.get("scene_id")
                if target_key not in reached:
                    reached[target_key] = target_scene
                    pending.append(target_scene)

    levels = []
    for scene in sorted(reached.values(), key=lambda r: (str(r.get("file", "")).casefold(), r.get("id", ""))):
        file_name = str(scene.get("file", ""))
        if file_name.casefold() in KNOWN_ACT1_BACKLINKS:
            candidate = "act_1_backlink"
            membership_reason = KNOWN_ACT1_BACKLINKS[file_name.casefold()]
            membership_confidence = "LIKELY"
        elif file_name.casefold() == START_SCENE.casefold():
            candidate = "act_2_inferred_start"
            membership_reason = "User-requested post-Act-1 root; boundary remains an inference pending owner confirmation."
            membership_confidence = "LIKELY"
        else:
            candidate = "unknown_act_membership"
            membership_reason = "Source reachability proves a possible level connection, not the narrative act boundary."
            membership_confidence = "GUESS"
        levels.append({
            "scene_id": scene.get("id"),
            "scene": file_name,
            "source": scene.get("source"),
            "level_script": (scene.get("config") or {}).get("scriptFile"),
            "script_source_names": campaign_by_scene.get(file_name.casefold(), {}).get("source_names"),
            "script_source_commands": campaign_by_scene.get(file_name.casefold(), {}).get("source_commands"),
            "script_count": campaign_by_scene.get(file_name.casefold(), {}).get("script_count"),
            "actor_candidate_count": len(scene.get("module_spawn_candidates", [])),
            "existing_atlas_bucket": scene.get("act"),
            "membership_candidate": candidate,
            "membership_evidence": membership_reason,
            "membership_confidence": membership_confidence,
            "membership_claim_scope": "narrative_act_boundary",
            "source_transition_edge_count": sum(1 for e in edges if e.get("source_scene_id") == scene.get("id")),
            "status": "TODO",
        })
    # Android is the canonical authored source for this atlas. Preserve the
    # original-cache copies as per-edge provenance instead of inflating the
    # authored edge count with byte-identical duplicate declarations.
    grouped = {}
    for edge in edges:
        source_path = str(edge.get("source_path", ""))
        logical_source = re.sub(r".*?/3d/modules/", "3d/modules/", source_path.replace("\\", "/"), flags=re.I)
        if edge["kind"] == "script_change_level":
            key = (edge["kind"], edge.get("source_reference_id"))
        else:
            key = (edge["kind"], edge.get("source_scene_id"), logical_source.casefold(),
                   edge.get("source_object"), edge.get("target_name"), edge.get("entrypoint_id"),
                   edge.get("fasttravel_id"), edge.get("activation_condition"),
                   edge.get("deactivation_condition"), edge.get("conditional_module_choice"))
        if key not in grouped:
            grouped[key] = dict(edge)
            grouped[key]["source_variants"] = []
            grouped[key]["edge_id"] = stable_id("r2-act2-edge", *key)
        grouped[key]["source_variants"].append({
            "platform": edge.get("source_platform", "android"),
            "source_path": edge.get("source_path") or edge.get("source_site"),
            "conditional_module_choice": edge.get("conditional_module_choice"),
        })
    edges = list(grouped.values())
    for edge in edges:
        edge["source_variants"] = sorted(edge["source_variants"],
                                         key=lambda v: (str(v.get("platform", "")), str(v.get("source_path", ""))))
    edges.sort(key=lambda e: (str(e.get("source_scene", "")).casefold(), str(e.get("kind")),
                              str(e.get("source_site", "")), str(e.get("target_name", "")).casefold()))
    frontier.append({
        "status": "TODO",
        "reason": "other_transition_mechanisms_not_closed",
        "detail": "The scope follows authored TriggerZoneExitLevel.levelName values, direct and rule-candidate MGP includes, and serialized Script_ChangeLevel commands. Fast-travel menu membership, every Lua/native route, opaque rule conditions, and cross-platform differences are not yet proven closed.",
        "evidence_to_acquire": ["FastTravelDestination/LevelList source consumers", "Lua/native level-loading call sites", "iOS and original-cache exit-zone source variants"],
    })
    missing_castle = next((r for r in frontier if r.get("reason") == "module_source_missing"
                           and str(r.get("raw_path", "")).casefold() == "gc02_corner_ws_01.mgp"), None)
    if missing_castle:
        frontier.append({
            "status": "TODO",
            "reason": "possible_ios_only_source_candidate",
            "scene": missing_castle.get("scene"),
            "raw_path": "data/3d/modules/castle/mgp/gc02_corner_ws_01.mgp",
            "detail": "No exact case-insensitive match was found in the explicit Android, Android original-cache, or extracted iOS data roots. The packed iOS logical-path index has not yet been joined for this filename; membership and any exit declarations remain unknown.",
        })
    for row in frontier:
        row["confidence"] = "GUESS"
        row["claim_scope"] = "transition_scope_completeness"
    frontier.sort(key=lambda r: (str(r.get("reason", "")), str(r.get("raw_path", "")).casefold(),
                                 str(r.get("source", "")).casefold()))
    scope = {
        "schema": "atlas-r2-act2-scope-v1",
        "package": PACKAGE,
        "scope_label": "act_2_inferred",
        "start_scene": START_SCENE,
        "stopping_rule": "Traverse every statically authored TriggerZoneExitLevel.levelName in direct MGP modules and every procedural <elem gameplay> module candidate, recurse nested Module.mgp includes, and include decoded Script_ChangeLevel targets. Keep branch, conditional, duplicate, and return edges. Stop when no newly resolved scene remains; unresolved modules/targets and other transition mechanisms remain explicit TODO frontier rows.",
        "evidence": EVIDENCE,
        "counts": {"levels": len(levels), "source_edges": len(edges), "frontier_rows": len(frontier),
                   "module_source_variants": len(modules_seen),
                   "edge_source_variants": sum(len(e.get("source_variants", [])) for e in edges),
                   "actor_candidates": sum(r.get("actor_candidate_count", 0) for r in levels),
                   "known_act1_backlink_levels": sum(1 for x in levels if x["membership_candidate"] == "act_1_backlink"),
                   "unknown_act_membership_levels": sum(1 for x in levels if x["membership_candidate"] == "unknown_act_membership")},
        "levels": levels,
        "edges": edges,
        "module_sources": sorted(module_sources.values(), key=lambda m: (str(m.get("scene", "")).casefold(),
                                                                          str(m.get("platform", "")),
                                                                          str(m.get("path", "")).casefold())),
        "frontier": frontier,
        "source_parse_repairs": sorted(source_repairs, key=lambda r: str(r.get("scene", "")).casefold()),
        "all_rows_initial_state": "TODO",
    }
    def canonicalize(value):
        if isinstance(value, dict):
            return {k: canonicalize(v) for k, v in value.items()}
        if isinstance(value, list):
            return [canonicalize(v) for v in value]
        if isinstance(value, str) and re.match(r"^[A-Za-z]:[\\/]", value):
            try:
                return source_site(ctx, Path(value))
            except (ValueError, OSError):
                return value
        return value
    return canonicalize(scope)


def _frontier_key(row):
    return (str(row.get("reason", "")), str(row.get("scene", "")).casefold(),
            str(row.get("source", row.get("raw_path", ""))).replace("\\", "/").casefold(),
            str(row.get("raw_path", "")).replace("\\", "/").casefold())


def _module_key(row):
    return (row.get("scene_id"), str(row.get("platform", "")), str(row.get("relative", "")).casefold())


def _new_frontier_id(row):
    return stable_id("r2-act2-frontier-row", *_frontier_key(row))


def _scope_expansion(baseline, current, initial_rows):
    old_levels = {r.get("scene_id") for r in baseline.get("levels", [])}
    old_edges = {r.get("edge_id") for r in baseline.get("edges", [])}
    old_modules = {_module_key(r) for r in baseline.get("module_sources", [])}
    old_frontiers = {_frontier_key(r) for r in baseline.get("frontier", [])}
    added_levels = [r for r in current["levels"] if r.get("scene_id") not in old_levels]
    added_edges = [r for r in current["edges"] if r.get("edge_id") not in old_edges]
    added_modules = [r for r in current["module_sources"] if _module_key(r) not in old_modules]
    new_frontiers = [r for r in current["frontier"] if _frontier_key(r) not in old_frontiers]
    repair_scenes = {r.get("scene") for r in current.get("source_parse_repairs", [])}
    old_frontier_rows = [r for r in initial_rows if r.get("row_type") == "frontier"]
    old_frontier_details = baseline.get("frontier", [])
    resolved_frontiers = []
    for row, detail in zip(old_frontier_rows, old_frontier_details):
        if detail.get("reason") == "scene_parse_failed" and detail.get("scene") in repair_scenes:
            resolved_frontiers.append({**detail, "step0_progress_id": row["id"], "status": "TODO",
                                       "resolution_status": "DONE",
                                       "resolution_note": "Narrow in-memory source repair recovered authored module declarations; original source bytes remain unchanged."})
    rows = []
    rows.extend({"id": r["scene_id"], "label": r["scene"], "row_type": "level", "state": "TODO"} for r in added_levels)
    rows.extend({"id": r["edge_id"], "label": f"{r.get('source_scene')} -> {r.get('target_name')}",
                 "row_type": "source_edge", "state": "TODO"} for r in added_edges)
    rows.extend({"id": f"{r.get('scene_id')}:{r.get('platform')}:{r.get('relative')}", "label": r.get("path"),
                 "row_type": "module_source", "state": "TODO"} for r in added_modules)
    rows.extend({"id": _new_frontier_id(r), "label": r.get("reason"), "row_type": "frontier", "state": "TODO"}
                for r in new_frontiers)
    return {
        "schema": "atlas-r2-act2-scope-expansion-v1", "package": "C-act2",
        "act_boundary": "INFERENCE; reachability does not prove narrative Act 2 membership.",
        "baseline_scope": "act2/scope.json", "current_scope": "act2/scope-current.json",
        "baseline_counts": baseline["counts"], "current_counts": current["counts"],
        "added_levels": added_levels, "added_edges": added_edges, "added_module_sources": added_modules,
        "new_frontiers": new_frontiers, "resolved_frontiers": resolved_frontiers,
        "source_parse_repairs": current.get("source_parse_repairs", []),
        "all_new_rows_initial_state": "TODO", "rows": rows,
    }


def _package_c_ledger(ctx, scope, expansion, initial_rows):
    out = Path(ctx.out) / "r2"
    a = read_json(out / "package-a-progress.json", {})
    b = read_json(out / "package-b-progress.json", {})
    a_ready = bool(a.get("total")) and a.get("processed") == a.get("total") and not a.get("status_counts", {}).get("TODO", 0)
    b_ready = bool(b.get("total")) and b.get("processed") == b.get("total") and not b.get("status_counts", {}).get("TODO", 0)
    cross_ready = a_ready and b_ready
    rows = [dict(r) for r in initial_rows] + [dict(r) for r in expansion["rows"]]
    repair_by_scene = {}
    for r in scope.get("source_parse_repairs", []):
        repair_by_scene.setdefault(r.get("scene"), []).append(r)
    initial_scope = read_json(out / "act2/scope.json", {})
    initial_frontier = initial_scope.get("frontier", [])
    initial_frontier_by_id = {}
    for idx, detail in enumerate(initial_frontier, 1):
        initial_frontier_by_id[f"frontier-{idx:03d}"] = detail
    current_by_key = {_frontier_key(r): r for r in scope["frontier"]}
    new_frontier_by_id = {_new_frontier_id(r): r for r in expansion["new_frontiers"]}
    for row in rows:
        typ, ident = row.get("row_type"), row.get("id")
        row["work_id"] = stable_id("r2-c-work-row", typ, ident, row.get("label"))
        if typ == "level":
            row["state"] = "DONE" if cross_ready else "TODO"
            if not cross_ready:
                row["reason"] = "Awaiting complete package A native and package B staging cross-joins."
        elif typ in {"source_edge", "module_source"}:
            row["state"] = "DONE"
        elif typ == "frontier":
            detail = new_frontier_by_id.get(ident) or initial_frontier_by_id.get(ident)
            if detail and detail.get("reason") == "scene_parse_failed" and detail.get("scene") in repair_by_scene:
                row.update({"state": "NOT-NEEDED", "evidence_scene": detail.get("scene"),
                            "resolution_reason": "Resolved by narrow in-memory source extraction; see scope-expansion.json source_parse_repairs."})
            else:
                current = current_by_key.get(_frontier_key(detail)) if detail else None
                if current:
                    row.update({"state": "UNRESOLVED", "reason": current.get("detail", current.get("raw_path", "Current transition frontier remains unresolved; see scope-current.json."))})
                else:
                    row.update({"state": "UNRESOLVED", "reason": "Current transition frontier remains unresolved; see scope-current.json."})
    counts = Counter(r.get("state", "TODO") for r in rows)
    total = len(rows)
    processed = sum(n for state, n in counts.items() if state != "TODO")
    progress = {"schema": "atlas-r2-package-progress-v1", "package": "C-act2", "total": total,
                "processed": processed, "unique_work_ids": len({r.get("work_id") for r in rows}),
                "status_counts": dict(sorted(counts.items())),
                "scope_files": ["act2/scope-current.json", "act2/scope-expansion.json", "act2/dossier.json",
                                "act2/coverage.json", "act2/blocker-reach.json"],
                "content_scope_levels": len(scope["levels"]),
                "content_scope_note": "Current reachable scene dossier count; level membership is not proven narrative Act 2.",
                "cross_join_ready": {"package_A_native": a_ready, "package_B_staging": b_ready},
                "rows": rows}
    return progress


def _render_coverage_markdown(coverage):
    counts = coverage["counts"]
    lines = ["# Conservative source and capability coverage", "",
             f"Scenes: **{counts['scenes']}**; required capability rows: **{counts['required_capabilities']}**; verified playable support: **{counts['verified_playable_fraction']['numerator']}/{counts['verified_playable_fraction']['denominator']} (0%)**.", "",
             "Static asset links and authored source references do not count as production capability. All capability rows remain unverified in the Windows runtime and visual path.", "",
             "| Capability | Rows | Staging review complete | Native contract review complete | Runtime verified | Playable supported |", "| --- | ---: | --- | --- | --- | --- |"]
    groups = {}
    for row in coverage["required_capabilities"]:
        groups.setdefault(row["capability"], []).append(row)
    for cap, rows in sorted(groups.items()):
        lines.append(f"| {cap} | {len(rows)} | {all(r['staging_status']=='REVIEW_COMPLETE' for r in rows)} | {all(r['native_contract_status']=='REVIEW_COMPLETE' for r in rows)} | {all(r['production_path_status']=='VERIFIED' for r in rows)} | {sum(r['playable_supported'] for r in rows)} |")
    lines.extend(["", f"Source edges: {counts['source_edges']}; unresolved transition frontiers: {counts['unresolved_frontier_rows']}; linked Character rows: {counts['unique_entity_rows']}; resolved entity-resource relationships: {counts['resolved_entity_resource_links']}; model-file candidates: {counts['model_asset_file_links']}.", ""])
    metrics=coverage.get('source_availability_metrics',{})
    lines.extend(['## Static availability, separate from playability','',
                  '| Inventory | Explicit denominator | Source or staging states |','| --- | ---: | --- |'])
    for name,key in [('Linked or candidate logical media paths','resource_file_dependencies'),('Distinct extracted AI native call sites','in_scope_ai_native_call_dependencies')]:
        metric=metrics.get(key,{})
        states=metric.get('status_counts') or metric.get('port_semantic_status_counts') or {}
        lines.append(f'| {name} | {metric.get("denominator",0)} | {states} |')
    lines.extend(['','A staged file or implemented callback does not prove a working scene. Parent table keys and diagnostic filename candidates cannot clear a physical media gap. These metrics are not averaged into a gameplay-support percentage.',''])
    return "\n".join(lines)


def _render_blocker_reach(dossier, scope, a_ready, b_ready):
    scenes = sorted((r["scene"] for r in dossier["levels"]), key=str.casefold)
    native_reach = defaultdict(set)
    native_details = {}
    staging_reach = defaultdict(set)
    staging_details = {}
    for level in dossier["levels"]:
        scene = level["scene"]
        for actor_row in level.get("entity_rows", []):
            entity = next((e for e in dossier.get("entities", []) if e.get("actor_id") == actor_row.get("actor_id")), {})
            for link in entity.get("package_A_native_callsite_specs", []):
                key = (link.get("native_name"), link.get("current_port_semantic_status", "UNKNOWN"))
                native_reach[key].add(scene)
                native_details[key] = link
        for ref in level.get("readiness", {}).get("staging", {}).get("references", []):
            classification = str(ref.get("classification", "UNRESOLVED"))
            if classification not in {"RESOLVED_STAGED", "NOT_NEEDED"}:
                key = (ref.get("reference_id"), classification)
                staging_reach[key].add(scene)
                staging_details[key] = ref
    candidates = []
    for (name, semantic), affected in native_reach.items():
        if semantic == "PORTED":
            continue
        link = native_details[(name, semantic)]
        candidates.append({"blocker": f"native:{name}:{semantic}", "blocked_scenes": sorted(affected, key=str.casefold),
                           "evidence_scope": "package_A_current_native_semantics_and_page_manifest",
                           "evidence_ids": [name], "evidence_notes": sorted({str(x) for x in link.get("current_port_evidence", [])}) or ["No linked current port evidence was recorded."],
                           "contract_page_state": link.get("contract_state"), "port_semantic_status": semantic})
    for (ref_id, classification), affected in staging_reach.items():
        detail = staging_details[(ref_id, classification)]
        candidates.append({"blocker": f"resource:{ref_id}:{classification}", "blocked_scenes": sorted(affected, key=str.casefold),
                           "evidence_scope": "package_B_current_classification_and_copy_selection",
                           "evidence_ids": [ref_id], "evidence_notes": sorted({str(detail.get("reason") or classification)}),
                           "classification": classification, "classification_file": detail.get("classification_file")})
    if scope["frontier"]:
        affected = sorted({r.get("scene") for r in scope["frontier"] if r.get("scene")} or scenes, key=str.casefold)
        candidates.append({"blocker": "unresolved_transition_source_frontiers", "blocked_scenes": affected,
                          "evidence_scope": "authored_transition_scope_completeness",
                          "evidence_ids": sorted({str(r.get("id") or r.get("reason")) for r in scope["frontier"]}),
                          "evidence_notes": sorted({str(r.get("reason")) for r in scope["frontier"]})})
    # Rank by actual cross-level reach first; one runtime-proof requirement is
    # included once, with unique evidence notes rather than repeated per scene.
    candidates.sort(key=lambda r: (-len(r["blocked_scenes"]), r["blocker"].casefold()))
    rows = []
    for rank, row in enumerate(candidates, 1):
        rows.append({"rank": rank, **row, "blocked_scene_count": len(row["blocked_scenes"])})
    rows.append({"rank": len(rows) + 1, "blocker": "Windows_production_runtime_verification", "blocked_scene_count": len(scenes),
                 "blocked_scenes": scenes, "evidence_scope": "per_scene_readiness_and_source_linked_dependencies",
                 "evidence_ids": [r.get("scene_id") for r in dossier["levels"]],
                 "evidence_notes": ["No production-path runtime evidence; all 39 scene capabilities remain unverified."]})
    lines = ["# Cross-level blocker reach", "", f"Ranked source and capability blockers across {len(scenes)} reachable scenes. High reach indicates implementation leverage, not proof of gameplay behavior.", "",
             "| Rank | Blocker | Affected scenes | Evidence note |", "| ---: | --- | ---: | --- |"]
    for row in rows:
        lines.append(f"| {row['rank']} | {row['blocker']} | {row['blocked_scene_count']} | {'; '.join(row['evidence_notes'])} |")
    return {"schema": "atlas-r2-act2-blocker-reach-v1", "scene_count": len(scenes),
            "ranking_policy": "Rank actual A semantic and B classification blocker IDs by affected-scene reach; runtime proof is recorded once after source blockers. Static links do not imply runtime support.",
            "blockers": rows}, "\n".join(lines) + "\n"


def run(ctx):
    """Regenerate C dossier outputs; never rewrite the frozen Step 0 scope."""
    from r2_act2_dossier import build_dossier, render_markdown, write_coverage

    root = Path(ctx.out) / "r2"
    out = root / "act2"
    scope = enumerate_scope(ctx)
    baseline = read_json(root / "act2/scope.json", {})
    if not baseline:
        raise RuntimeError("Frozen Step 0 Act 2 scope is missing")
    inventory = read_json(root / "initial-inventory.json", {})
    initial_rows = inventory.get("packages", {}).get("C", [])
    if len(initial_rows) != 457:
        raise RuntimeError(f"Frozen Step 0 C inventory changed: expected 457 rows, found {len(initial_rows)}")
    expansion = _scope_expansion(baseline, scope, initial_rows)
    retained = [r for r in expansion["resolved_frontiers"]]
    if len(retained) != 11:
        raise RuntimeError(f"Expected 11 resolved Step 0 parser-frontier rows, found {len(retained)}")
    write_json(out / "scope-current.json", scope)
    _write_scope_report(out / "scope-current.md", scope)
    write_json(out / "scope-expansion.json", expansion)

    dossier = build_dossier(ctx, scope)
    (out / "dossier.md").write_text(render_markdown(dossier, scope, expansion), encoding="utf-8", newline="\n")
    level_dir, entity_dir = out / "levels", out / "entities"
    level_dir.mkdir(parents=True, exist_ok=True)
    entity_dir.mkdir(parents=True, exist_ok=True)
    level_manifest = []
    for level in dossier["levels"]:
        level_json = level_dir / f"{level['scene_id']}.json"
        write_json(level_json, level)
        lines = [f"# {level['scene']}", "", f"Scene ID: `{level['scene_id']}`; membership: **{level['membership']['candidate']}** ({level['membership'].get('confidence', 'GUESS')}).", "",
                 "## Authored placements", "", f"{level['entities']['placement_count']} static Character definitions across {len(level['entities']['actor_ids'])} unique Character rows. These are not runtime spawn counts.", "",
                 "## Source chains", "", f"Transitions: {level['transitions']['status']}; quests/dialogue: {level['quest_dialogue'].get('status', 'UNRESOLVED')}; boss candidates: {len(level['boss_phases']['candidates'])}; music: {level['music'].get('status', 'UNRESOLVED')}; item/loot: {level['items_loot'].get('status', 'UNRESOLVED')}.", "",
                 f"Package B ledger: {level['readiness']['staging']['ledger_completion_status']}; Package A ledger: {level['readiness']['native_contracts']['ledger_completion_status']}; Windows runtime: {level['readiness']['port_runtime']['status']}; visual runtime: {level['readiness']['visual_runtime']['status']}.", "",
                 "Actor resource and AI details are sharded under `../entities/`. Source evidence is static and does not claim runtime behavior.", ""]
        (level_dir / f"{level['scene_id']}.md").write_text("\n".join(lines), encoding="utf-8", newline="\n")
        level_manifest.append({"scene_id": level["scene_id"], "scene": level["scene"],
                               "details_path": f"levels/{level['scene_id']}.json",
                               "sha256": hashlib.sha256(level_json.read_bytes()).hexdigest(),
                               "membership_candidate": level.get("membership", {}).get("candidate")})
    entity_manifest = []
    for entity in dossier["entities"]:
        actor_id = str(entity["actor_id"])
        entity_json = entity_dir / f"{actor_id}.json"
        write_json(entity_json, entity)
        lines = [f"# {entity['actor_name']} (`{actor_id}`)", "", f"Character source: {entity.get('source_table', 'unresolved')}; confidence: {entity.get('source_confidence', 'GUESS')}; scope: {entity.get('claim_scope', 'authored_source')}.", "",
                 "## Resources", ""]
        for name, resource in entity.get("resources", {}).items():
            lines.append(f"- **{name}**: {resource.get('status', 'UNRESOLVED')} — {resource.get('reason', 'see JSON for source links')}")
        lines.extend(["", "AI and native callsite links are static source cross-references. Windows production behavior remains unverified.", ""])
        (entity_dir / f"{actor_id}.md").write_text("\n".join(lines), encoding="utf-8", newline="\n")
        entity_manifest.append({"actor_id": actor_id, "actor_name": entity.get("actor_name"),
                                "details_path": f"entities/{actor_id}.json",
                                "sha256": hashlib.sha256(entity_json.read_bytes()).hexdigest(),
                                "status": entity.get("status", "UNRESOLVED")})
    # Keep the aggregate index comfortably below the per-file ceiling; detailed
    # actor/AI/resource evidence lives in content-addressed per-entity shards.
    dossier_index = {**dossier, "levels": level_manifest, "entities": entity_manifest}
    write_json(out / "dossier.json", dossier_index)

    a = read_json(root / "package-a-progress.json", {})
    b = read_json(root / "package-b-progress.json", {})
    a_ready = bool(a.get("total")) and a.get("processed") == a.get("total") and not a.get("status_counts", {}).get("TODO", 0)
    b_ready = bool(b.get("total")) and b.get("processed") == b.get("total") and not b.get("status_counts", {}).get("TODO", 0)
    coverage = write_coverage(out / "coverage.json", dossier, scope, b_ready, a_ready)
    (out / "coverage.md").write_text(_render_coverage_markdown(coverage), encoding="utf-8", newline="\n")
    blocker, blocker_md = _render_blocker_reach(dossier, scope, a_ready, b_ready)
    write_json(out / "blocker-reach.json", blocker)
    (out / "blocker-reach.md").write_text(blocker_md, encoding="utf-8", newline="\n")
    progress = _package_c_ledger(ctx, scope, expansion, initial_rows)
    write_json(root / "package-c-progress.json", progress)
    return scope


def _write_scope_report(path, scope):
    rows = [[r["scene"], r["scene_id"], r["membership_candidate"], r["existing_atlas_bucket"], r["status"]]
            for r in scope["levels"]]
    edges = [[r.get("source_scene"), r.get("kind"), r.get("target_name"), r.get("target_scene", "UNRESOLVED"),
              r.get("source_site"), r.get("status")] for r in scope["edges"]]
    def table(headers, data):
        esc = lambda v: str(v).replace("|", "\\|").replace("\n", " ")
        return "| " + " | ".join(headers) + " |\n| " + " | ".join("---" for _ in headers) + " |\n" + "\n".join(
            "| " + " | ".join(esc(v) for v in row) + " |" for row in data)
    text = ["# Act 2 inferred source-transition scope", "",
            f"Initial inventory: **{scope['counts']['levels']} scene files**, **{scope['counts']['source_edges']} source edges**, "
            f"**{scope['counts']['frontier_rows']} TODO frontier rows**. Every row starts TODO.", "",
            "## Evidence and limits", "",
            "Visual evidence: this is static source inventory with no direct visual counterpart; no gameplay animation or transition timing is claimed.", "",
            f"Logic evidence: {scope['evidence']['native_consumer']}", "",
            "All source edges record authored targets only. Conditional module alternatives and command branches are retained without claiming execution. Known Act 1 backlinks are identified separately; the remaining destination act memberships are unknown. The closure does not claim that all reachable scenes belong to Act 2.", "",
            "## Level rows", "", table(["Scene", "Canonical scene ID", "Membership candidate", "Existing Atlas bucket", "State"], rows), "",
            "## Transition edges", "", table(["Source scene", "Source kind", "Target LevelList name", "Resolved scene", "Source site", "State"], edges), "",
            "## Stopping rule", "", scope["stopping_rule"], "",
            "## Frontier", ""]
    for row in scope["frontier"]:
        text.append(f"- **TODO — {row.get('reason')}**: {row.get('detail', row.get('raw_path', row.get('scene', 'source unresolved')))}")
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_text("\n".join(text) + "\n", encoding="utf-8", newline="\n")


def check(ctx):
    errors = []
    tests = []
    root = Path(ctx.out) / "r2"
    out = root / "act2"
    try:
        scope = enumerate_scope(ctx)  # pure source rebuild; must never write progress or package artifacts
        saved_scope = read_json(out / "scope-current.json", {})
        equal_scope = all(saved_scope.get(k) == scope.get(k) for k in
                          ("counts", "levels", "edges", "module_sources", "frontier", "source_parse_repairs"))
        if not equal_scope:
            errors.append("saved current scope differs from the deterministic source rebuild")
        tests.append({"name": "source_scope_rebuild_is_pure_and_exact", "passed": equal_scope,
                      "detail": f"{scope['counts']['levels']} scenes, {scope['counts']['source_edges']} edges, {len(scope['module_sources'])} module rows, {len(scope['frontier'])} explicit frontiers"})

        baseline = read_json(root / "act2/scope.json", {})
        inventory = read_json(root / "initial-inventory.json", {})
        initial_rows = inventory.get("packages", {}).get("C", [])
        expansion = read_json(out / "scope-expansion.json", {})
        ledger = read_json(root / "package-c-progress.json", {})
        ledger_rows = ledger.get("rows", [])
        expansion_rows = expansion.get("rows", [])
        # Step 0's historical IDs are not unique: several authored source-edge
        # rows share source_site as their ID. Preserve these rows by their full
        # legacy identity and validate the append-only ledger as a multiset.
        legacy_key = lambda r: (r.get("row_type"), r.get("id"), r.get("label"))
        initial_keys = Counter(legacy_key(r) for r in initial_rows)
        expansion_keys = Counter(legacy_key(r) for r in expansion_rows)
        ledger_keys = Counter(legacy_key(r) for r in ledger_rows)
        expected_keys = initial_keys + expansion_keys
        expansion_ok = (len(initial_rows) == 457 and len(expansion_rows) == 214 and
                        len(expansion.get("added_levels", [])) == 2 and len(expansion.get("added_edges", [])) == 35 and
                        len(expansion.get("added_module_sources", [])) == 176 and len(expansion.get("new_frontiers", [])) == 1 and
                        len(expansion.get("resolved_frontiers", [])) == 11 and
                        expected_keys == ledger_keys and len(ledger_rows) == 671)
        if not expansion_ok:
            errors.append("scope expansion or append-only 671-row C ledger is incomplete")
        tests.append({"name": "step0_rows_retained_and_expansion_accounted", "passed": expansion_ok,
                      "detail": "457 frozen Step 0 rows retained; expansion has 214 rows; 11 repaired parser frontiers have terminal receipts."})

        a = read_json(root / "package-a-progress.json", {})
        b = read_json(root / "package-b-progress.json", {})
        a_ready = bool(a.get("total")) and a.get("processed") == a.get("total") and not a.get("status_counts", {}).get("TODO", 0)
        b_ready = bool(b.get("total")) and b.get("processed") == b.get("total") and not b.get("status_counts", {}).get("TODO", 0)
        counts = ledger.get("status_counts", {})
        cross_ready = ledger.get("cross_join_ready") == {"package_A_native": a_ready, "package_B_staging": b_ready}
        expected_todo = 0 if a_ready and b_ready else len(scope["levels"])
        progress_ok = (ledger.get("total") == 671 and ledger.get("processed") == sum(v for k, v in counts.items() if k != "TODO") and
                       counts.get("TODO", 0) == expected_todo and cross_ready and
                       all(r.get("state") != "TODO" for r in ledger_rows if r.get("row_type") != "level"))
        if a_ready and b_ready:
            progress_ok = progress_ok and ledger.get("processed") == ledger.get("total") and counts.get("TODO", 0) == 0
        if not progress_ok:
            errors.append("C completion ledger is inconsistent with current A/B cross-join readiness or still has unfinished non-level rows")
        tests.append({"name": "readiness_progress_matches_late_cross_joins", "passed": progress_ok,
                      "detail": f"C {ledger.get('processed', 0)}/{ledger.get('total', 0)}; package A ready={a_ready}; package B ready={b_ready}"})

        dossier = read_json(out / "dossier.json", {})
        required_sections = {"entities", "transitions", "quest_dialogue", "boss_phases", "music", "items_loot", "readiness"}
        level_ids = {r.get("scene_id") for r in scope["levels"]}
        level_manifest = dossier.get("levels", [])
        dossier_levels = []
        level_manifest_errors = []
        for manifest_row in level_manifest:
            details = out / manifest_row.get("details_path", f"levels/{manifest_row.get('scene_id')}.json")
            if not details.is_file() or hashlib.sha256(details.read_bytes()).hexdigest() != manifest_row.get("sha256"):
                level_manifest_errors.append(str(manifest_row.get("scene_id")))
                continue
            dossier_levels.append(read_json(details, {}))
        sections_ok = (dossier.get("level_count") == 39 and dossier.get("entity_count") == len(dossier.get("entities", [])) and
                       {r.get("scene_id") for r in dossier_levels} == level_ids and
                       all(required_sections <= set(r) for r in dossier_levels) and
                       not level_manifest_errors and
                       all(r.get("readiness", {}).get("port_runtime", {}).get("status") == "UNVERIFIED" and
                           r.get("readiness", {}).get("visual_runtime", {}).get("status") == "NOT_RUN" for r in dossier_levels))
        if not sections_ok:
            errors.append("one or more per-level readiness dossier sections or runtime limitation fields are missing")
        tests.append({"name": "all_scene_dossier_sections_present", "passed": sections_ok,
                      "detail": f"{len(dossier_levels)} scene dossiers; entity shards={dossier.get('entity_count', 0)}"})

        artifact_errors = []
        entity_by_id = {str(r.get("actor_id")): r for r in dossier.get("entities", [])}
        entity_contents = {}
        for actor_id, manifest_row in entity_by_id.items():
            ejson = out / manifest_row.get("details_path", f"entities/{actor_id}.json")
            emd = out / "entities" / f"{actor_id}.md"
            if not ejson.is_file() or not emd.is_file():
                artifact_errors.append(f"missing entity shard: {actor_id}")
                continue
            if hashlib.sha256(ejson.read_bytes()).hexdigest() != manifest_row.get("sha256"):
                artifact_errors.append(f"entity shard hash mismatch: {actor_id}")
                continue
            entity_contents[actor_id] = read_json(ejson, {})
        for level in dossier_levels:
            scene_id = str(level.get("scene_id"))
            json_path = out / "levels" / f"{scene_id}.json"
            md_path = out / "levels" / f"{scene_id}.md"
            if not json_path.is_file() or not md_path.is_file():
                artifact_errors.append(f"missing per-level artifact: {scene_id}")
            elif read_json(json_path, {}) != level:
                artifact_errors.append(f"per-level JSON differs from canonical dossier: {scene_id}")
            for entity_ref in level.get("entity_rows", []):
                actor_id = str(entity_ref.get("actor_id"))
                entity = entity_contents.get(actor_id)
                ejson = out / "entities" / f"{actor_id}.json"
                emd = out / "entities" / f"{actor_id}.md"
                if entity is None or not ejson.is_file() or not emd.is_file():
                    artifact_errors.append(f"missing or inconsistent entity shard: {actor_id}")
            for boss in level.get("boss_phases", {}).get("candidates", []):
                if (not boss.get("phase_table_status") or not boss.get("boss_candidate_confidence") or
                        (boss.get("phase_table_status") == "UNRESOLVED" and not boss.get("phase_table_reason"))):
                    artifact_errors.append(f"boss phase evidence lacks status/reason: {scene_id}/{boss.get('actor_id')}")
                for native in boss.get("package_A_native_specs", []):
                    if not native.get("contract_page") or native.get("contract_state") == "UNAVAILABLE":
                        artifact_errors.append(f"boss native cross-join lacks current A page: {scene_id}/{native.get('native_name')}")
            for entity_ref in level.get("entity_rows", []):
                entity = entity_contents.get(str(entity_ref.get("actor_id")), {})
                for family, dep in entity.get("package_B_resource_dependencies", {}).items():
                    if dep.get("status") == "UNRESOLVED" and not dep.get("reason"):
                        artifact_errors.append(f"unresolved {family} dependency has no reason: {entity.get('actor_id')}")
                    for ref in dep.get("references", []):
                        if not ref.get("classification") or not ref.get("classification_file"):
                            artifact_errors.append(f"B resource join missing classification file/state: {entity.get('actor_id')}/{family}")
                        if ref.get("classification") in {"UNRESOLVED", "AMBIGUOUS", "ORIGINAL_EXISTS_NOT_STAGED"} and not ref.get("reason"):
                            artifact_errors.append(f"B non-staged classification has no reason: {ref.get('reference_id')}")
        for level in dossier_levels:
            music = level.get("music", {})
            if music.get("package_B_sound_status") == "UNRESOLVED" and not music.get("package_B_sound_reason"):
                artifact_errors.append(f"unresolved music has no reason: {level.get('scene')}")
        if artifact_errors:
            errors.extend(artifact_errors[:20])
        tests.append({"name": "per_scene_entity_and_dependency_artifacts_complete", "passed": not artifact_errors,
                      "detail": f"Checked {len(dossier_levels)} scene shards, {len(entity_by_id)} entity shards, B dependency reasons, boss phase tables and A native links."})

        snapshot_errors = []
        for row in dossier.get("cross_join_source_snapshots", []):
            path = Path(ctx.out) / row.get("path", "")
            if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != row.get("sha256"):
                snapshot_errors.append(str(row.get("path")))
        if snapshot_errors:
            errors.append("A/B cross-join snapshot changed since C generation: " + ", ".join(snapshot_errors[:10]))
        tests.append({"name": "A_B_cross_join_snapshots_current", "passed": not snapshot_errors,
                      "detail": f"Verified {len(dossier.get('cross_join_source_snapshots', []))} A/B progress, scope and classification hashes."})

        coverage = read_json(out / "coverage.json", {})
        cov_counts = coverage.get("counts", {})
        coverage_ok = (cov_counts.get("scenes") == 39 and cov_counts.get("required_capabilities") == 234 and
                       cov_counts.get("verified_playable_fraction") == {"numerator": 0, "denominator": 234, "percent": 0.0} and
                       not any(r.get("playable_supported") for r in coverage.get("required_capabilities", [])) and
                       coverage.get("source_availability_metrics", {}).get("resource_file_dependencies", {}).get("denominator") is not None and
                       coverage.get("source_availability_metrics", {}).get("in_scope_ai_native_call_dependencies", {}).get("denominator") is not None and
                       all(r.get("staging_status") == "REVIEW_COMPLETE" and r.get("native_contract_status") == "REVIEW_COMPLETE"
                           for r in coverage.get("required_capabilities", [])))
        if not coverage_ok:
            errors.append("coverage metric is missing its conservative 234-row denominator or claims unsupported playability")
        tests.append({"name": "conservative_capability_denominator", "passed": coverage_ok,
                      "detail": "39 scenes × six capabilities; 0/234 playable-supported without runtime and visual proof."})

        files = [p for p in out.rglob("*") if p.is_file()]
        oversized = [str(p) for p in files if p.stat().st_size >= 40_000_000]
        if oversized:
            errors.append("generated files exceed 40 MB: " + ", ".join(oversized))
        tests.append({"name": "generated_file_size_limit", "passed": not oversized,
                      "detail": f"{len(files)} C artifacts; largest {max((p.stat().st_size for p in files), default=0)} bytes"})
        uncertainty_ok = (scope.get("start_scene") == START_SCENE and
                          any(r.get("reason") == "other_transition_mechanisms_not_closed" for r in scope.get("frontier", [])))
        if not uncertainty_ok:
            errors.append("start scene or explicit transition-completeness frontier is missing")
        tests.append({"name": "scope_uncertainties_remain_explicit", "passed": uncertainty_ok,
                      "detail": "Narrative boundary remains inferred; unresolved mechanism and source frontiers are recorded."})
    except Exception as exc:  # surface read/schema failures through Atlas check
        errors.append(str(exc))
    return {"passed": not errors, "tests": tests, "errors": errors}
