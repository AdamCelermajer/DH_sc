"""Focused fixtures for the iPad source-name resolver."""
from __future__ import annotations

import copy
import struct
import tempfile
from pathlib import Path
from types import SimpleNamespace

import ios_semantics
from common import stable_id


def _word(value):
    return struct.pack("<I", value & 0xFFFFFFFF)


def _string(value):
    raw = value.encode("utf-8")
    return _word(len(raw)) + raw


def _offset_header(data):
    return bytes((data[i] + i + 1) & 0xFF for i in range(4)) + data[4:]


def _names(*groups):
    data = bytearray()
    for group in groups:
        data += _word(len(group))
        for value in group:
            data += _string(value)
    return _offset_header(bytes(data))


def _constants(groups):
    data = bytearray(_word(len(groups)))
    for namespace, entries in groups:
        data += _string(namespace) + _word(len(entries))
        for name, value in entries:
            data += _string(name) + struct.pack("<i", value)
    return _offset_header(bytes(data))


def _fixture():
    temp = tempfile.TemporaryDirectory()
    inputs = Path(temp.name)
    ipa = inputs / "claude-preview15" / "ipa"
    data_root = ipa / "extracted" / "Payload" / "dungeonhunter2iPad.app" / "data"
    data_root.mkdir(parents=True)
    mapping = {
        "data/file000001.dat": "data/pydata/animations_dictionary_pyarraynames.bin",
        "data/file000002.dat": "data/pydata/tie_a_pyarraynames.bin",
        "data/file000003.dat": "data/pydata/tie_b_pyarraynames.bin",
        "data/file000004.dat": "data/pydata/broken_pyarraynames.bin",
        "data/file000005.dat": "data/pydata/ai_pycst.bin",
        "data/file000006.dat": "data/pydata/broken_pycst.bin",
        "data/file000007.dat": "data/scripts/fixture.luac",
    }
    (data_root / "file00000-plantext.dat").write_bytes(
        "".join(f"{physical}:{logical}\n" for physical, logical in mapping.items()).encode("utf-8"))
    (ipa / "index_map.tsv").write_text(
        "".join(f"{physical}\t{logical}\n" for physical, logical in mapping.items()), encoding="utf-8")
    payloads = {
        "data/file000001.dat": _names(["idle", "hit", "Shared", "Shared"]),
        "data/file000002.dat": _names(["tie_a", "tie_b"]),
        "data/file000003.dat": _names(["tie_c", "tie_d"]),
        "data/file000004.dat": _names(["broken_a", "broken_b"]),
        "data/file000005.dat": _constants([("AIStates", [("Skill", 6)])]),
        "data/file000006.dat": b"\x00\x00\x00\x00bad pycst",
        "data/file000007.dat": b"fixture Lua source is represented in the packed call ledger",
    }
    for physical, payload in payloads.items():
        (data_root / Path(*physical.split("/")[1:])).write_bytes(payload)

    assets = []
    for physical, logical in mapping.items():
        asset_id = stable_id("asset", f".local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/{physical}")
        assets.append({
            "id": asset_id,
            "path": f".local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/{physical}",
            "source": "ios-packed",
            "mapping_confidence": "LIKELY",
            "logical_paths": [logical],
            "package_relative": physical,
            "relative": Path(physical).name,
            "decode_recipe": "subtract-first-four-bytes(+1,+2,+3,+4):names-only-groups" if "pyarraynames" in logical else "preserve-raw",
            "name_group_count": 1 if logical.endswith("animations_dictionary_pyarraynames.bin") else
                                1 if logical.endswith("tie_a_pyarraynames.bin") or logical.endswith("tie_b_pyarraynames.bin") or logical.endswith("broken_pyarraynames.bin") else None,
            "name_count": 4 if logical.endswith("animations_dictionary_pyarraynames.bin") else
                          2 if logical.endswith("tie_a_pyarraynames.bin") or logical.endswith("tie_b_pyarraynames.bin") or logical.endswith("broken_pyarraynames.bin") else None,
            "package_version": "fixture-iPad",
        })
    # The invalid group-count metadata is deliberate and must block use of that stream.
    next(asset for asset in assets if asset["logical_paths"][0].endswith("broken_pyarraynames.bin"))["name_group_count"] = 7

    script_asset = next(asset for asset in assets if asset["logical_paths"][0] == "data/scripts/fixture.luac")
    script = {
        "id": "script-fixture", "site": script_asset["path"], "platform": "ios",
        "source": "ios-package", "source_asset_id": script_asset["id"],
        "logical_path": "data/scripts/fixture.luac", "source_sha256": "src", "decoded_sha256": "dec",
    }
    calls = []

    def add_call(name, namespace, value, *, call_id=None, source_asset_id=None, mode="global"):
        args = []
        for index, item in enumerate((namespace, value), 1):
            raw = f'"{item}"'
            args.append({"index": index, "expression": raw,
                         "literals": [{"type": "string", "value": item, "raw": raw}]})
        calls.append({"id": call_id or f"call-{len(calls)}", "script_id": script["id"],
                      "source_asset_id": source_asset_id or script_asset["id"],
                      "site": script_asset["path"], "line": len(calls) + 1,
                      "name": name, "call_mode": mode,
                      "expression": f'{name}("{namespace}", "{value}")',
                      "arguments": args, "logical_source_path": script["logical_path"],
                      "source_decode_recipe": "fixture-validated", "source_sha256": "src",
                      "decoded_sha256": "dec", "package_version": "fixture-iPad"})
        return calls[-1]

    add_call("GetPyOID", "AnimDict", "idle")
    add_call("GetPyOID", "AnimDict", "hit")
    add_call("GetPyOID", "AnimDict", "Shared")
    add_call("GetPyOID", "UnknownNamespace", "idle")
    for name in ("tie_a", "tie_b", "tie_c", "tie_d"):
        add_call("GetPyOID", "TieNS", name)
    add_call("GetPyOID", "BrokenNS", "broken_a")
    add_call("GetPyOID", "BrokenNS", "broken_b")
    add_call("GetPyCst", "AIStates", "Skill")
    add_call("GetPyCst", "AIStates", "Missing")
    android_asset_id = "android-script-asset"
    assets.append({"id": android_asset_id, "source": "android", "mapping_confidence": "LIKELY",
                   "logical_paths": ["data/scripts/fixture.luac"], "path": ".local-inputs/assets-extra/android/data/scripts/fixture.luac"})
    add_call("GetPyOID", "WrongPlatform", "idle", source_asset_id=android_asset_id)
    packed = {"script_records": [script], "script_calls": calls}
    ctx = SimpleNamespace(root=Path("."), inputs=inputs, out=inputs / "out")
    return temp, ctx, assets, packed


def run_checks():
    temp, ctx, assets, packed = _fixture()
    try:
        calls = {row["id"]: row for row in packed["script_calls"]}
        script_id = packed["script_records"][0]["id"]

        def literal_ref(ref_id, call_id, name):
            call = calls[call_id]
            return {"id": ref_id, "name": name, "kind": "named_reference",
                    "reference_kind": "name", "source_platform": "ios", "status": "UNRESOLVED",
                    "reason": "OTHER", "reason_code": "OTHER", "work_status": "DONE",
                    "confidence": "LIKELY", "rule": "Fixture raw literal", "resolves_to": [],
                    "candidates": [], "candidate_note": "Raw fixture source name",
                    "context": {"script_id": script_id, "call_id": "outer-" + call_id,
                                "call": "RegisterAnim", "argument_index": 1,
                                "argument_expression": call["expression"],
                                "literal_line": call["line"]}}

        nested_refs = [
            literal_ref("orig-namespace", "call-0", "AnimDict"),
            literal_ref("orig-name", "call-0", "idle"),
            literal_ref("orig-constant-namespace", "call-10", "AIStates"),
            literal_ref("orig-constant-name", "call-10", "Skill"),
            literal_ref("orig-unknown-name", "call-8", "broken_a"),
        ]
        rows = ios_semantics.resolve(ctx, nested_refs, [], assets, packed)
        owned = {row["id"]: row for row in rows}
        by_call = {row["context"]["call_id"]: row for row in owned.values()}
        checks = []

        positive = by_call[calls["call-0"]["id"]]
        target = positive["resolves_to"][0]
        checks.append(("exact iPad name pair resolves to its own stored group/ordinal",
                       positive["status"] == "RESOLVED" and target["source_file"].endswith("animations_dictionary_pyarraynames.bin")
                       and target["group"] == 0 and target["ordinal"] == 0
                       and target["native_id_verified"] is False and positive["confidence"] == "LIKELY"))

        checks.append(("original nested GetPyOID namespace and name literals retain IDs with source roles",
                       owned["orig-namespace"]["status"] == "NOT-NEEDED"
                       and owned["orig-namespace"]["ios_semantic_resolution"]["evidence"]["literal_role"] == "provider_namespace"
                       and owned["orig-name"]["status"] == "RESOLVED"
                       and owned["orig-name"]["resolves_to"][0]["ordinal"] == target["ordinal"]))

        checks.append(("known nested GetPyCst pair removes duplicate raw-name gaps",
                       owned["orig-constant-namespace"]["status"] == "NOT-NEEDED"
                       and owned["orig-constant-name"]["status"] == "NOT-NEEDED"
                       and owned["orig-constant-name"]["ios_semantic_resolution"]["evidence"]["provider_status"] == "RESOLVED"))

        checks.append(("unresolved nested provider remains an explicit linked gap",
                       owned["orig-unknown-name"]["status"] == "UNRESOLVED"
                       and owned["orig-unknown-name"]["ios_semantic_resolution"]["evidence"]["provider_status"] == "UNRESOLVED"))

        duplicate = by_call[calls["call-2"]["id"]]
        checks.append(("duplicate exact names remain ambiguous source candidates",
                       duplicate["status"] == "AMBIGUOUS" and len(duplicate["resolves_to"]) == 2
                       and all(candidate["native_id_verified"] is False for candidate in duplicate["resolves_to"])))

        unknown = by_call[calls["call-3"]["id"]]
        checks.append(("a unique global match without family evidence stays unresolved",
                       unknown["status"] == "UNRESOLVED" and bool(unknown["candidates"])
                       and not unknown["resolves_to"]))

        tie = by_call[calls["call-4"]["id"]]
        checks.append(("a real repeated-evidence family tie is explicitly ambiguous",
                       tie["status"] == "AMBIGUOUS" and bool(tie["resolves_to"])
                       and tie["ios_semantic_resolution"]["evidence"]["namespace_family_status"] == "AMBIGUOUS"))

        broken = by_call[calls["call-8"]["id"]]
        checks.append(("bad declared name-group count blocks the source family",
                       broken["status"] == "UNRESOLVED"
                       and any(error["logical_path"].endswith("broken_pyarraynames.bin")
                               for error in broken["ios_semantic_resolution"]["evidence"]["name_stream_errors"])))

        constant = by_call[calls["call-10"]["id"]]
        ctarget = constant["resolves_to"][0]
        checks.append(("header-adjusted bounded pycst decode records the stored value only",
                       constant["status"] == "RESOLVED" and ctarget["stored_value"] == 6
                       and ctarget["native_id_verified"] is False
                       and constant["ios_semantic_resolution"]["evidence"]["source_decode_recipe"].startswith("subtract-first-four-bytes")
                       and constant["ios_semantic_resolution"]["evidence"]["stored_value_is_native_return"] is False))

        missing_constant = by_call[calls["call-11"]["id"]]
        checks.append(("unknown stored constant stays unresolved",
                       missing_constant["status"] == "UNRESOLVED"
                       and missing_constant["reason_code"] == "OTHER"))

        wrong = by_call[calls["call-12"]["id"]]
        checks.append(("wrong-platform source asset cannot resolve through iPad data",
                       wrong["status"] == "UNRESOLVED"
                       and wrong["ios_semantic_resolution"]["evidence"]["reason_code"] == "IOS_SOURCE_MISMATCH"))

        dynamic_packed = copy.deepcopy(packed)
        dynamic = dynamic_packed["script_calls"][0]
        dynamic["arguments"][1]["expression"] = "dynamicName"
        dynamic["arguments"][1]["literals"] = []
        dynamic_rows = ios_semantics.resolve(ctx, [], [], assets, dynamic_packed)
        dynamic_row = next(row for row in dynamic_rows if row["context"]["call_id"] == dynamic["id"])
        checks.append(("dynamic provider operand remains unresolved",
                       dynamic_row["status"] == "UNRESOLVED"
                       and dynamic_row["ios_semantic_resolution"]["evidence"]["reason_code"] == "DYNAMIC_ARGUMENTS"))

        rerun = ios_semantics.resolve(ctx, rows, [], assets, packed)
        checks.append(("stable IDs and deep-copy output make resolution idempotent",
                       len(rerun) == len(rows) and [row["id"] for row in rerun] == [row["id"] for row in rows]
                       and rows == ios_semantics.resolve(ctx, nested_refs, [], assets, packed)))

        # Direct malformed/trailing-byte checks ensure the parser never accepts a prefix.
        try:
            ios_semantics._parse_constant_groups(_word(0) + b"trailing")
            trailing_rejected = False
        except ValueError:
            trailing_rejected = True
        try:
            ios_semantics._parse_constant_groups(_word(1))
            truncated_rejected = False
        except ValueError:
            truncated_rejected = True
        checks.append(("constant parser rejects trailing and truncated bytes",
                       trailing_rejected and truncated_rejected))
        return {"passed": all(passed for _, passed in checks),
                "checks": [{"name": name, "passed": passed} for name, passed in checks],
                "scope": "Temporary mapped iPad pack fixture; no source or game files changed."}
    finally:
        temp.cleanup()


def run(ctx=None):
    del ctx
    result = run_checks()
    return [{"name": check["name"], "passed": check["passed"],
             "detail": result["scope"]} for check in result["checks"]]


if __name__ == "__main__":
    result = run_checks()
    for check in result["checks"]:
        print(("PASS" if check["passed"] else "FAIL") + " " + check["name"])
    if not result["passed"]:
        raise SystemExit(1)
