"""Focused source-gate checks for character_template_semantics."""
from copy import deepcopy
from pathlib import Path
from types import SimpleNamespace

import character_template_semantics as semantics


def _ref(name, attribute, props, platform="android", row="row-a"):
    return {
        "id": row, "name": name, "kind": "character", "rule": "XML_ATTRIBUTE",
        "status": "UNRESOLVED", "reason": "OTHER", "reason_code": "OTHER",
        "work_status": "DONE", "confidence": "PROVEN", "resolves_to": [],
        "candidates": [], "source_platform": platform,
        "context": {"attribute": attribute, "element": "GameObject",
                    "element_index": 0, "source_properties": props,
                    "file": f".local-inputs/assets-extra/{platform}/data/scene.mgp"},
    }


def run(ctx=None):
    ctx = ctx or SimpleNamespace(inputs=Path("."))
    props = {"gametype": "Character", "char_template": "WolfPack",
             "char_template_pydata": "Charater_Templates",
             "charpropsname": "IgnoredByTemplate", "charpropsname_pydata": "CharacterTable"}
    tables = [
        {"table": "character_templates#group0", "index": 0, "name": "WolfPack",
         "source_file": ".local-inputs/assets-extra/android/data/pydata/character_templates_pyarray.bin"},
        {"table": "character_properties#group0", "index": 5, "name": "Wolf",
         "source_file": ".local-inputs/assets-extra/android/data/pydata/character_properties_pyarray.bin"},
    ]
    tests = []

    def record(name, passed, detail=None):
        tests.append({"name": name, "passed": bool(passed), "detail": detail})

    def run_one(ref, table_rows=tables):
        return semantics.resolve(ctx, [ref], table_rows, [])[0]

    authored = _ref("WolfPack", "char_template", props)
    resolved = run_one(authored)
    record("authored char_template resolves to exact Android template row",
           resolved["status"] == "RESOLVED" and resolved["resolves_to"][0]["index"] == 0
           and resolved["referenced_table"] == "character_templates#group0")
    record("stable IDs and caller input are preserved", resolved["id"] == authored["id"]
           and authored["status"] == "UNRESOLVED")
    record("resolution is idempotent", semantics.resolve(ctx, [resolved], tables, []) == [resolved])

    alias = run_one(_ref("Charater_Templates", "char_template_pydata", props))
    record("char_template_pydata resolves as a table selector, never as the template row key",
           alias["status"] == "RESOLVED" and alias["reference_kind"] == "pydata_table"
           and alias.get("referenced_index") is None
           and alias.get("referenced_table") == "character_templates#group0"
           and alias["resolves_to"][0].get("type") == "table")
    cp_alias = run_one(_ref("CharacterTable", "charpropsname_pydata", props))
    record("char_template precedence blocks the competing CharacterTable selector",
           cp_alias["status"] == "UNRESOLVED" and "takes precedence" in cp_alias["candidate_note"])

    non_character = dict(props, gametype="Prop")
    gated = run_one(_ref("WolfPack", "char_template", non_character))
    record("non-Character gametype is rejected", gated["status"] == "UNRESOLVED"
           and "gametype=Character" in gated.get("candidate_note", ""))
    missing_gate = run_one(_ref("WolfPack", "char_template", {"char_template": "WolfPack"}))
    record("missing source selector gate stays unresolved", missing_gate["status"] == "UNRESOLVED")
    wrong_key = run_one(_ref("Other", "char_template", props))
    record("reference spelling must equal the exact source key", wrong_key["status"] == "UNRESOLVED")
    unsupported_alias = run_one(_ref("WolfPack", "char_template",
                                     dict(props, char_template_pydata="DifferentTable")))
    record("unregistered PyData alias does not select a row", unsupported_alias["status"] == "UNRESOLVED")

    original_names = semantics._ios_name_groups
    try:
        semantics._ios_name_groups = lambda _ctx, _assets, table, _cache: (
            {"id": "ios-names", "path": "ios/file000373.dat"},
            [["WolfPack"]] if table == "character_templates" else [["Wolf"]])
        ipad = run_one(_ref("WolfPack", "char_template", props, "ios"))
        ipad_target = ipad["resolves_to"][0]
        record("iPad exact source-name link is scoped to its names stream, with native ID unverified",
               ipad["status"] == "RESOLVED" and ipad["confidence"] == "LIKELY"
               and ipad_target["type"] == "ios_source_name" and ipad_target["ordinal"] == 0
               and ipad_target["source_asset_id"] == "ios-names"
               and ipad_target["native_id_verified"] is False
               and "table" not in ipad_target and "index" not in ipad_target
               and "referenced_table" not in ipad)
        ipad_alias = run_one(_ref("Charater_Templates", "char_template_pydata", props, "ios"))
        ipad_alias_target = ipad_alias["resolves_to"][0]
        record("iPad table alias is a source-scoped table target, not an Android table node",
               ipad_alias["status"] == "RESOLVED" and ipad_alias["confidence"] == "LIKELY"
               and ipad_alias_target["type"] == "ios_table_source"
               and ipad_alias_target["source_asset_id"] == "ios-names"
               and ipad_alias_target["native_id_verified"] is False
               and "table" not in ipad_alias_target and "referenced_table" not in ipad_alias)
        ios_direct_cp = run_one(_ref("CharacterTable", "charpropsname_pydata",
                                     dict(props, char_template="", charpropsname="Wolf"), "ios"))
        record("iPad CharacterTable table alias uses its own names-only group zero",
               ios_direct_cp["status"] == "RESOLVED"
               and ios_direct_cp["resolves_to"][0]["type"] == "ios_table_source"
               and ios_direct_cp["resolves_to"][0]["group"] == 0
               and ios_direct_cp["resolves_to"][0]["native_id_verified"] is False)
        unknown = run_one(_ref("WolfPack", "char_template", props, "other"))
        record("unknown platform cannot borrow the iPad names group",
               unknown["status"] == "UNRESOLVED" and not unknown.get("resolves_to"))
    finally:
        semantics._ios_name_groups = original_names

    direct_cp = dict(props, char_template="", charpropsname="Wolf")
    cp_resolved = run_one(_ref("CharacterTable", "charpropsname_pydata", direct_cp))
    record("CharacterTable selector resolves only for direct charpropsname path",
           cp_resolved["status"] == "RESOLVED"
           and cp_resolved["referenced_table"] == "character_properties#group0")
    duplicate = run_one(_ref("WolfPack", "char_template", props), tables + [dict(tables[0], index=1)])
    record("duplicate table names remain ambiguous", duplicate["status"] == "AMBIGUOUS"
           and len(duplicate["candidates"]) == 2 and len(duplicate["resolves_to"]) == 2
           and all(t["type"] == "table-row" for t in duplicate["resolves_to"]))
    absent = run_one(_ref("Unknown", "char_template", dict(props, char_template="Unknown")))
    record("missing exact table row remains unresolved", absent["status"] == "UNRESOLVED")

    original_reader = semantics._read_source
    try:
        xml = b'<Level><GameObject gametype="Character" char_template="WolfPack" char_template_pydata="Charater_Templates"/></Level>'
        semantics._read_source = lambda _ctx, _asset: xml
        source_ref = {"id": "xml", "name": "WolfPack", "rule": "XML_ATTRIBUTE",
                      "status": "UNRESOLVED", "context": {"attribute": "char_template",
                      "element": "GameObject", "element_index": 1,
                      "file": ".local-inputs/assets-extra/android/data/scene.mgp"}}
        source_asset = {"id": "source", "source": "android",
                        "path": ".local-inputs/assets-extra/android/data/scene.mgp"}
        source_out = semantics.resolve(ctx, [source_ref], tables, [source_asset])[0]
        record("exact source XML element attributes, not unrelated reference context, gate the link",
               source_out["status"] == "RESOLVED")
    finally:
        semantics._read_source = original_reader

    return {"passed": all(test["passed"] for test in tests), "tests": tests,
            "scope": "Source identity and table-selector links only; no runtime spawn or visual behavior asserted."}
