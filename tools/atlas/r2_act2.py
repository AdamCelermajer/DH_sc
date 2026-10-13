"""Static Act 2 transition-scope inventory for Atlas round 2.

This module reads the generated round-1 indexes and original scene/MGP sources.
It does not execute scripts, procedural rules, or transitions. Every emitted
work row starts TODO so root can release the package scope before dossier work.
"""
from __future__ import annotations

import json
import re
import xml.etree.ElementTree as ET
from collections import Counter, deque
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


def _scene_module_roots(ctx, scene):
    source = _scene_source(ctx, scene)
    if source is None:
        return [], [{"status": "TODO", "reason": "scene_source_missing", "scene": scene.get("file")}]
    try:
        root = ET.parse(source).getroot()
    except (ET.ParseError, OSError) as exc:
        return [], [{"status": "TODO", "reason": "scene_parse_failed", "scene": scene.get("file"), "detail": str(exc)}]
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
    return modules, []


def _parse_module(ctx, variant):
    try:
        return ET.parse(variant["path"]).getroot()
    except (ET.ParseError, OSError):
        return None


def _reachable_modules(ctx, scene):
    roots, frontier = _scene_module_roots(ctx, scene)
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
    return module_rows, exits, frontier


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
    while pending:
        scene = pending.popleft()
        modules, exits, missing = _reachable_modules(ctx, scene)
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
        elif file_name.casefold() == START_SCENE.casefold():
            candidate = "act_2_inferred_start"
            membership_reason = "User-requested post-Act-1 root; boundary remains an inference pending owner confirmation."
        else:
            candidate = "unknown_act_membership"
            membership_reason = "Source reachability proves a possible level connection, not the narrative act boundary."
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


def run(ctx):
    out = Path(ctx.out) / "r2/act2"
    scope = enumerate_scope(ctx)
    write_json(out / "scope.json", scope)
    progress_rows = ([{"row_type": "level", "id": r["scene_id"], "label": r["scene"], "state": "TODO"} for r in scope["levels"]]
                     + [{"row_type": "source_edge", "id": r.get("source_reference_id") or r.get("source_site"),
                         "label": f"{r.get('source_scene')} -> {r.get('target_name')}", "state": "TODO"} for r in scope["edges"]]
                     + [{"row_type": "module_source", "id": f"{r.get('scene_id')}:{r.get('platform')}:{r.get('relative')}",
                         "label": r.get("path"), "state": "TODO"} for r in scope["module_sources"]]
                     + [{"row_type": "frontier", "id": f"frontier-{i:03d}", "label": r.get("reason"), "state": "TODO"}
                        for i, r in enumerate(scope["frontier"], 1)])
    progress = {"schema": "atlas-r2-package-progress-v1", "package": "C-act2",
                "total": len(progress_rows), "processed": 0, "status_counts": {"TODO": len(progress_rows)},
                "scope_files": ["act2/scope.json"], "rows": progress_rows}
    write_json(Path(ctx.out) / "r2/package-c-progress.json", progress)
    _write_scope_report(out / "scope.md", scope)
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
    try:
        scope = enumerate_scope(ctx)
        tests.append({"name": "transition_scope_rebuilds", "passed": True,
                      "detail": f"{scope['counts']['levels']} levels; {scope['counts']['source_edges']} authored edges"})
        if scope["start_scene"] != START_SCENE:
            errors.append("start scene mismatch")
        if not scope["levels"] or any(row.get("status") != "TODO" for row in scope["levels"]):
            errors.append("all initial level rows must remain TODO")
        if any(row.get("status") != "TODO" for row in scope["edges"]):
            errors.append("all initial source edge rows must remain TODO")
        if any(row.get("status") != "TODO" for row in scope["module_sources"]):
            errors.append("all initial module-source rows must remain TODO")
        if any(row.get("status") != "TODO" for row in scope["frontier"]):
            errors.append("all initial frontier rows must remain TODO")
        progress_path = Path(ctx.out) / "r2/package-c-progress.json"
        if progress_path.is_file():
            progress = read_json(progress_path)
            if progress.get("processed") != 0 or progress.get("status_counts") != {"TODO": progress.get("total")}:
                errors.append("package progress must report every initial row as TODO")
            if progress.get("total") != (len(scope["levels"]) + len(scope["edges"])
                                          + len(scope["module_sources"]) + len(scope["frontier"])):
                errors.append("package progress row count does not match the enumerated scope")
        if not any(row.get("reason") == "other_transition_mechanisms_not_closed" for row in scope["frontier"]):
            errors.append("other transition-mechanism frontier missing")
        tests.append({"name": "uncertainties_are_explicit", "passed": not errors,
                      "detail": "Act membership, non-command exits, and unresolved source paths stay visible."})
    except Exception as exc:  # report source/inventory gaps through atlas check
        errors.append(str(exc))
    return {"passed": not errors, "tests": tests, "errors": errors}
