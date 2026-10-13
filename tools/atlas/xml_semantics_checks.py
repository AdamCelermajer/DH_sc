"""Focused positive and rejection checks for xml_semantics.resolve."""
from copy import deepcopy
from pathlib import Path
from types import SimpleNamespace

import package_index
import xml_semantics


def _ref(name="PlayerLight", attribute="prototype", element="GameObject",
         asset_id="asset-a", logical="data/scene/local.xml"):
    return {
        "id": "reference-stable-id", "name": name, "kind": "character",
        "rule": "XML_ATTRIBUTE", "status": "UNRESOLVED", "reason": "OTHER",
        "reason_code": "OTHER", "work_status": "DONE", "confidence": "PROVEN",
        "resolves_to": [], "candidates": [],
        "context": {"attribute": attribute, "element": element,
                    "source_asset_id": asset_id,
                    "logical_source_path": logical,
                    "file": ".local-inputs/source.xml"},
    }


def _asset(asset_id="asset-a", source="android"):
    return {"id": asset_id, "source": source,
            "path": ".local-inputs/source.xml",
            "logical_paths": ["data/scene/local.xml"] if source == "ios-packed" else []}


def run(ctx=None):
    tests = []

    def record(name, passed, detail=None):
        tests.append({"name": name, "passed": bool(passed), "detail": detail})

    ctx = ctx or SimpleNamespace(inputs=Path("."))
    original_reader = xml_semantics._read_source
    try:
        contents = {"asset-a": b'<Level><Template name="PlayerLight"/></Level>',
                    "asset-b": b'<Level><Template name="PlayerLight"/></Level>',
                    "asset-duplicate": b'<Level><Template name="PlayerLight"/><Prototype name="PlayerLight"/></Level>',
                    "asset-missing": b'<Level><Template name="Other"/></Level>',
                    "asset-broken": b'<Level><Template name="PlayerLight">'}
        xml_semantics._read_source = lambda _ctx, asset: contents[asset["id"]]
        refs = [_ref()]
        output = xml_semantics.resolve(ctx, refs, [], [_asset()])
        row = output[0]
        record("exact same-resource definition resolves with a typed target",
               row["status"] == "RESOLVED" and row["resolves_to"][0]["type"] == "xml_template"
               and row["resolves_to"][0]["source_asset_id"] == "asset-a")
        record("stable reference ID and input ledger are preserved",
               refs[0]["id"] == row["id"] == "reference-stable-id"
               and refs[0]["status"] == "UNRESOLVED")
        record("annotations are idempotent",
               output == xml_semantics.resolve(ctx, output, [], [_asset()]))

        foreign = xml_semantics.resolve(ctx, [_ref(asset_id="asset-missing")], [],
                                        [_asset("asset-a"), _asset("asset-missing")])[0]
        record("matching name in a foreign physical resource does not resolve",
               foreign["status"] == "UNRESOLVED" and not foreign.get("resolves_to"))
        duplicate = xml_semantics.resolve(ctx, [_ref(asset_id="asset-duplicate")], [],
                                          [_asset("asset-duplicate")])[0]
        record("duplicate same-resource declarations remain ambiguous",
               duplicate["status"] == "AMBIGUOUS" and len(duplicate["candidates"]) == 2
               and len(duplicate["resolves_to"]) == 2)
        missing = xml_semantics.resolve(ctx, [_ref(asset_id="asset-missing")], [],
                                        [_asset("asset-missing")])[0]
        record("missing exact declaration remains unresolved",
               missing["status"] == "UNRESOLVED")
        decorative = xml_semantics.resolve(ctx, [_ref(attribute="_templateName", asset_id="asset-a")], [],
                                           [_asset("asset-a")])[0]
        record("Android selector disposition precedes coincident local declaration",
               decorative["status"] == "NOT-NEEDED"
               and decorative["reference_kind"] == "object_template_selector"
               and "0x51419c" in decorative.get("schema_evidence", "")
               and decorative["xml_semantics"].get("external_inheritance") == "unsupported/unverified"
               and "not a file or actor reference" in decorative.get("candidate_note", ""))
        non_game = xml_semantics.resolve(ctx, [_ref(attribute="_templateName", element="Light")], [],
                                         [_asset("asset-missing")])[0]
        record("unproven non-GameObject template labels remain unresolved",
               non_game["status"] == "UNRESOLVED")
        ios_label = _ref(attribute="_templateName", asset_id="asset-ios")
        ios_label["source_platform"] = "ios"
        ios_out = xml_semantics.resolve(ctx, [ios_label], [],
                                       [_asset("asset-ios", "ios-packed")])[0]
        record("Android property-map proof does not classify iPad labels",
               ios_out["status"] == "UNRESOLVED"
               and "role" not in ios_out.get("xml_semantics", {})
               and "schema_evidence" not in ios_out.get("xml_semantics", {}))
        gated = _ref(attribute="charpropsname")
        gated.update(context={**gated["context"], "gametype": "Character", "char_template": "Named"})
        record("charpropsname gate and char_template precedence are left to owner resolver",
               xml_semantics.resolve(ctx, [gated], [], [_asset()])[0] == gated)
        malformed = xml_semantics.resolve(ctx, [_ref(attribute="prototype", asset_id="asset-broken")], [],
                                          [_asset("asset-broken")])[0]
        record("truncated XML cannot become an inferred declaration match",
               malformed["status"] == "UNRESOLVED"
               and malformed.get("xml_semantics", {}).get("source_read_error"))
    finally:
        xml_semantics._read_source = original_reader

    original_reader_resource = package_index.read_resource
    seen = []
    try:
        package_index.read_resource = lambda _ctx, asset: seen.append(asset["id"]) or b"<Level/>"
        xml_semantics._read_source(ctx, _asset("ios-asset", "ios-packed"))
        record("iPad packed XML reads go through validated package_index reader",
               seen == ["ios-asset"])
    finally:
        package_index.read_resource = original_reader_resource

    return {"passed": all(test["passed"] for test in tests),
            "tests": tests,
            "scope": "Static XML source identity only; no loader admission or runtime use asserted."}
