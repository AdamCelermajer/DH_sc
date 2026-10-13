"""Focused checks for source event/sound semantic resolution."""
from __future__ import annotations

import json
from pathlib import Path
import sys
from tempfile import TemporaryDirectory
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parent))
import resource_semantics
from common import read_json


def row(name, index, uid, event=0, table="sdd_dungeon_hunter_2_iphone"):
    return {"id": f"sound-{index}", "table": table, "index": index, "name": name,
            "fields": {"uid": uid, "isEvent": event}, "status": "RESOLVED"}


def ref(name, rule, *, rid="r1", platform=None, attribute=None, source=".local-inputs/assets-extra/android/data/3d/test.bdae"):
    context = {"file": source, "original_event": name if rule.startswith("BRES_EVENT") else None}
    if attribute:
        context["attribute"] = attribute
    result = {"id": rid, "name": name, "kind": "sound" if "SFX" in rule or rule == "XML_ATTRIBUTE" else "animation event",
              "rule": rule, "context": context, "status": "TODO", "work_status": "TODO",
              "resolves_to": [], "candidates": []}
    if platform:
        result["source_platform"] = platform
    return result


def asset(name, source, path=None, logical=None, relative=None):
    if relative is None:
        relative = name if source == "audio-cache" else f"sounds/{name}"
    if path is None and source == "audio-cache":
        path = f".local-inputs/audio-v34/cache/{name}"
    return {"id": f"{source}-{name}", "source": source,
            "path": path or f".local-inputs/assets-extra/{source}/data/sounds/{name}",
            "relative": relative, "sha256": "a" * 64, "size": 100,
            "logical_paths": [logical] if logical else []}


def coverage_check(ctx):
    """Audit resolver coverage and typed targets in the generated ledger."""
    refs = read_json(Path(ctx.out) / "data/references.json", []) or []
    errors = []
    candidates = []
    for ref_row in refs:
        context = ref_row.get("context") or {}
        rule = ref_row.get("rule")
        selected = rule in {"BRES_EVENT_CALLBACK", "BRES_EVENT_SFX_PREFIX"}
        selected = selected or (rule == "XML_ATTRIBUTE" and ref_row.get("kind") == "sound" and
                                str(context.get("attribute", "")).casefold() in resource_semantics._XML_SOUND_ATTRIBUTES)
        if selected:
            candidates.append(ref_row)
            sem = ref_row.get("resource_semantics") or {}
            if sem.get("resolver") != "resource_semantics.v1":
                errors.append("missing resource semantic disposition: " + str(ref_row.get("id")))
                continue
            targets = ref_row.get("resolves_to") or []
            if sem.get("role") == "animation_event" and ref_row.get("status") == "RESOLVED":
                target = next((t for t in targets if t.get("type") == "native_event"), None)
                if not target or not all(target.get(key) for key in ("id", "name", "source_handler", "address", "evidence", "conditions")):
                    errors.append("incomplete native event target: " + str(ref_row.get("id")))
            if sem.get("role") == "unresolved_event_marker":
                if ref_row.get("status") != "UNRESOLVED" or not ref_row.get("reason") or not sem.get("reason"):
                    errors.append("unknown event marker lacks explicit unresolved reason: " + str(ref_row.get("id")))
            for target in targets:
                if target.get("type") != "source_sound":
                    continue
                if target.get("source_id") is None or target.get("table") != "sdd_dungeon_hunter_2_iphone":
                    errors.append("source sound target lacks exact SDD identity: " + str(ref_row.get("id")))
                if target.get("missing_filenames") and (ref_row.get("status") != "UNRESOLVED" or ref_row.get("reason") != "FILE_MISSING"):
                    errors.append("missing sound member is not retained as FILE_MISSING: " + str(ref_row.get("id")))
                for platform, receipts in (target.get("asset_files_by_platform") or {}).items():
                    if platform not in {"android", "ios"}:
                        errors.append("sound file assigned to unknown platform: " + str(ref_row.get("id")))
                    if any(not receipt.get("logical_uri", "").startswith("data/sounds/") for receipt in receipts):
                        errors.append("sound receipt lost full physical URI namespace: " + str(ref_row.get("id")))
    return errors


def run(ctx=None):
    checks = []
    corpus_ctx = ctx
    with TemporaryDirectory(prefix="atlas-resource-semantics-") as temp:
        root = Path(temp)
        inputs = root / "inputs"
        (inputs / "audio-v34/cache").mkdir(parents=True)
        (inputs / "audio-v34/cache/sounds.xml").write_text(
            '<soundpack><sounds><sound uid="380" label="QueenAttack" filename="sfx_dark_queen_attack.wav" />'
            '<sound uid="162" label="ChestOpen" filename="sfx_chest_opening.wav" />'
            '<sound uid="137" label="sfx_skill_rogue_arc_burst_shot" filename="sfx_skill_rogue_arc_burst_shot.wav" />'
            '<sound uid="99" label="Footstep" filename="footstep.wav" />'
            '<sound uid="401" label="RandomHit1" filename="hit_1.wav" />'
            '<sound uid="402" label="RandomHit2" filename="hit_2.wav" />'
            '</sounds></soundpack>', encoding="utf-8")
        ledger_path = root / "port/engine-audio/reference/source-bindings-v38/ledger.json"
        ledger_path.parent.mkdir(parents=True)
        ledger_path.write_text(json.dumps([
            {"source_id": 161, "source_name": "QueenAttack", "uid": 380, "event": 0,
             "status": "done", "assets": [{"uid": 380, "filename": "sfx_dark_queen_attack.wav", "exists": True}]},
            {"source_id": 33, "source_name": "ChestOpen", "uid": 162, "event": 0,
             "status": "missing", "assets": [{"uid": 162, "filename": "sfx_chest_opening.wav", "exists": False}]},
            {"source_id": 542, "source_name": "sfx_skill_rogue_arc_burst_shot", "uid": 137, "event": 0,
             "status": "done", "assets": [{"uid": 137, "filename": "sfx_skill_rogue_arc_burst_shot.wav", "exists": True}]},
            {"source_id": 90, "source_name": "Footstep", "uid": 99, "event": 0,
             "status": "done", "assets": [{"uid": 99, "filename": "footstep.wav", "exists": True}]},
            {"source_id": 244, "source_name": "RandomHit", "uid": 44, "event": 1,
             "status": "partial", "target": {"type": "random", "value": "401,402"},
             "assets": [{"uid": 401, "filename": "hit_1.wav", "exists": True},
                        {"uid": 402, "filename": "hit_2.wav", "exists": False}]},
        ]), encoding="utf-8")
        ctx = SimpleNamespace(root=root, inputs=inputs)
        tables = [row("QueenAttack", 161, 380), row("ChestOpen", 33, 162),
                  row("sfx_skill_rogue_arc_burst_shot", 542, 137), row("Footstep", 90, 99),
                  row("RandomHit", 244, 44, event=1)]
        tables.extend([row("Repeated", 91, 100), row("Repeated", 92, 101)])
        assets = [asset("sfx_dark_queen_attack.wav", "android"),
                  asset("sfx_dark_queen_attack.wav", "ios"),
                  asset("sfx_skill_rogue_arc_burst_shot.wav", "android"),
                  asset("footstep.wav", "audio-cache"), asset("hit_1.wav", "ios")]
        refs = [
            ref("do_skill", "BRES_EVENT_CALLBACK", rid="skill"),
            ref("do_spell", "BRES_EVENT_CALLBACK", rid="spell"),
            ref("attack_mainhand", "BRES_EVENT_CALLBACK", rid="attack"),
            ref("attack_offhand", "BRES_EVENT_CALLBACK", rid="offhand"),
            ref("ev_step_left", "BRES_EVENT_CALLBACK", rid="step"),
            ref("an_camera/cam_shake_horiz", "BRES_EVENT_CALLBACK", rid="camera"),
            ref("closed", "BRES_EVENT_CALLBACK", rid="unknown"),
            ref("sfx_QueenAttack", "BRES_EVENT_SFX_PREFIX", rid="queen"),
            ref("sfx_sfx_skill_rogue_arc_burst_shot", "BRES_EVENT_SFX_PREFIX", rid="double-prefix"),
            ref("sfx_ChestOpen", "BRES_EVENT_SFX_PREFIX", rid="known-missing"),
            ref("sfx_RandomHit", "BRES_EVENT_SFX_PREFIX", rid="random-event"),
            ref("Footstep", "XML_ATTRIBUTE", rid="xml-label", attribute="sound"),
            ref("Repeated", "XML_ATTRIBUTE", rid="xml-ambiguous", attribute="sound"),
            ref("footstep", "XML_ATTRIBUTE", rid="xml-case", attribute="sound"),
            ref("sfx_QueenAttack", "XML_ATTRIBUTE", rid="xml-exact", attribute="idle_sound"),
            ref("Footstep", "XML_ATTRIBUTE", rid="ios-xml-label", attribute="sound",
                source=".local-inputs/assets-extra/ios/data/test.xml"),
            ref("QueenAttack", "BRES_EVENT_SFX_PREFIX", rid="ios-event", platform="ios",
                source=".local-inputs/assets-extra/ios/data/3d/test.bdae"),
            ref("do_skill", "BRES_EVENT_CALLBACK", rid="unknown-platform-event", source=""),
            ref("sfx_QueenAttack", "BRES_EVENT_SFX_PREFIX", rid="unknown-platform-sound", source=""),
        ]
        next(item for item in refs if item["id"] == "ios-event")["context"]["original_event"] = "sfx_QueenAttack"
        before = json.dumps(refs, sort_keys=True)
        once = resource_semantics.resolve(ctx, refs, tables, assets)
        twice = resource_semantics.resolve(ctx, once, tables, assets)
        by_id = {item["id"]: item for item in once}
        by_id_twice = {item["id"]: item for item in twice}

        def check(name, condition, detail=None):
            checks.append({"name": name, "passed": bool(condition), "detail": detail})

        check("input refs remain untouched", json.dumps(refs, sort_keys=True) == before)
        check("stable IDs and idempotent output", all(by_id[k] == by_id_twice[k] for k in by_id))
        skill = by_id["skill"]["resolves_to"][0]
        check("do_skill keeps both state admission branches", skill["type"] == "native_event" and
              "state 6" in skill["conditions"] and "state 5" in skill["conditions"])
        check("do_spell state gate retained", "state must be 7" in by_id["spell"]["resolves_to"][0]["conditions"])
        check("attack main/offhand remain distinct", by_id["attack"]["resolves_to"][0]["role"] !=
              by_id["offhand"]["resolves_to"][0]["role"] or "offhand" in by_id["offhand"]["resolves_to"][0]["role"])
        check("ev_ prefix resolves through AIS virtual", "virtual 0x94" in by_id["step"]["resolves_to"][0]["source_handler"])
        check("an_ camera callback is not treated as a file URI", by_id["camera"]["status"] == "RESOLVED" and
              by_id["camera"]["resolves_to"][0]["type"] == "native_event" and
              "cam_shake_horiz" in by_id["camera"]["resolves_to"][0]["name"])
        check("unknown callback retains explicit reason", by_id["unknown"]["status"] == "UNRESOLVED" and
              by_id["unknown"]["reason"] == "OTHER" and
              by_id["unknown"]["resource_semantics"]["reason_code"] == "UNKNOWN_EVENT_MARKER" and
              "owner/admission" in by_id["unknown"]["candidate_note"])
        queen = by_id["queen"]["resolves_to"][0]
        check("BRES sound maps exact suffix to source SDD row", queen["type"] == "source_sound" and
              queen["source_id"] == 161 and queen["name"] == "QueenAttack" and queen["original_event"] == "sfx_QueenAttack")
        check("physical sound path retains Android/iOS union", set(queen["asset_files_by_platform"]) == {"android", "ios"} and
              all(row["logical_uri"] == "data/sounds/sfx_dark_queen_attack.wav" for values in queen["asset_files_by_platform"].values() for row in values))
        check("only one leading event prefix is removed", by_id["double-prefix"]["resolves_to"][0]["source_id"] == 542)
        check("catalog-known missing file stays unresolved", by_id["known-missing"]["status"] == "UNRESOLVED" and
              by_id["known-missing"]["reason"] == "FILE_MISSING" and "sfx_chest_opening.wav" in
              by_id["known-missing"]["resolves_to"][0]["missing_filenames"])
        random_target = by_id["random-event"]["resolves_to"][0]
        check("source random event retains both exact member files", random_target["is_event"] and
              {member["uid"] for member in random_target["members"]} == {401, 402} and
              "hit_2.wav" in random_target["missing_filenames"] and
              random_target["asset_files_by_platform"]["ios"][0]["logical_uri"] == "data/sounds/hit_1.wav")
        check("XML member label matches source name exactly", by_id["xml-label"]["status"] == "RESOLVED" and
              by_id["xml-label"]["resolves_to"][0]["source_id"] == 90)
        check("XML label case mismatch is not normalized", by_id["xml-case"]["status"] == "UNRESOLVED" and
              by_id["xml-case"]["reason"] == "OTHER" and
              by_id["xml-case"]["resource_semantics"]["reason_code"] == "UNKNOWN_SOUND_LABEL")
        check("XML sfx-looking label is not prefix-stripped", by_id["xml-exact"]["status"] == "UNRESOLVED")
        ios_xml = by_id["ios-xml-label"]["resolves_to"][0]
        check("iOS bare sound label keeps Android row inference explicit", by_id["ios-xml-label"]["confidence"] == "LIKELY" and
              ios_xml.get("platform_association", "").startswith("iOS authored sound label") and
              ios_xml["native_equivalence"] == "not-asserted")
        check("duplicate exact XML sound labels remain ambiguous", by_id["xml-ambiguous"]["status"] == "AMBIGUOUS" and
              len(by_id["xml-ambiguous"]["candidates"]) == 2)
        ios = by_id["ios-event"]["resolves_to"][0]
        check("iOS event association states Android inference only", ios.get("platform_association", "").startswith("Android named-sound handler inference") and
              ios["native_equivalence"] == "not-asserted" and by_id["ios-event"]["confidence"] == "LIKELY")
        unknown_platform_event = by_id["unknown-platform-event"]["resolves_to"][0]
        check("unknown source platform keeps Android event evidence likely only",
              by_id["unknown-platform-event"]["confidence"] == "LIKELY" and
              unknown_platform_event["source_platform"] == "unknown" and
              unknown_platform_event["native_evidence_platform"] == "android" and
              unknown_platform_event["native_equivalence"] == "not-asserted" and
              unknown_platform_event.get("platform_association", "").startswith("Android native-handler evidence"))
        unknown_platform_sound = by_id["unknown-platform-sound"]["resolves_to"][0]
        check("unknown source platform sound association is not proven",
              by_id["unknown-platform-sound"]["confidence"] == "LIKELY" and
              unknown_platform_sound["source_platform"] == "unknown" and
              unknown_platform_sound["native_equivalence"] == "not-asserted" and
              unknown_platform_sound.get("platform_association", "").startswith("Android named-sound handler inference"))

        case_ref = ref("sfx_QueenAttack", "BRES_EVENT_SFX_PREFIX", rid="file-case")
        case_out = resource_semantics.resolve(ctx, [case_ref], tables,
            [asset("SFX_Dark_Queen_Attack.wav", "ios")])[0]
        case_target = case_out["resolves_to"][0]
        check("case-only physical path stays missing with diagnostic candidate", case_out["status"] == "UNRESOLVED" and
              case_out["reason"] == "FILE_MISSING" and len(case_target["case_mismatch_candidates"]) == 1 and
              "not accepted" in case_out["candidate_note"])

    if corpus_ctx is not None and (Path(corpus_ctx.out) / "data/references.json").is_file():
        errors = coverage_check(corpus_ctx)
        checks.append({"name": "generated corpus rows have terminal source semantic evidence",
                       "passed": not errors, "detail": errors[:12] if errors else None})
    return {"checks": checks, "passed": all(item["passed"] for item in checks),
            "check_count": len(checks), "failed": [item for item in checks if not item["passed"]]}


if __name__ == "__main__":
    result = run()
    print(json.dumps(result, indent=2, ensure_ascii=False))
    raise SystemExit(0 if result["passed"] else 1)
