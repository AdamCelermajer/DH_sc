"""Conservative source-name links for the packed iPad Lua scripts.

This resolver associates exact Lua GetPyOID/GetPyCst operands with the iPad
package's own serialized name and constant streams.  It does not use Android
table IDs, an iPhone executable, or assume that a stored ordinal is the value
returned by an iPad native API.
"""
from __future__ import annotations

import copy
import re
import struct

import package_index
from common import stable_id


_RESOLVER = "ios_semantics.v1"
_NAME_SUFFIXES = ("_pyarraynames.bin", "_pystructnames.bin")
_CONSTANT_SUFFIX = "_pycst.bin"
_MAX_GROUPS = 65_536
_MAX_ENTRIES = 100_000
_MAX_STRING = 1_048_576
_MAX_TOTAL_ENTRIES = 1_000_000
_NESTED_PROVIDER = re.compile(
    r'''^\s*(GetPyOID|GetPyCst)\s*\(\s*(["'])([^"'\\]+)\2\s*,\s*(["'])([^"'\\]+)\4\s*\)\s*;?\s*$''')
_PRIOR_FIELDS = ("status", "reason", "reason_code", "confidence", "rule", "resolves_to",
                 "candidate_note", "candidates", "work_status")


def _logical(asset):
    paths = asset.get("logical_paths") or []
    return paths[0] if len(paths) == 1 and isinstance(paths[0], str) else None


def _asset_id(asset):
    return asset.get("id") or stable_id("asset", asset.get("path", ""))


def _mapped_ios_asset(asset):
    return (asset.get("source") == "ios-packed"
            and asset.get("mapping_confidence") == "LIKELY"
            and _logical(asset) is not None)


def _ios_asset(asset):
    return (_mapped_ios_asset(asset)
            and _logical(asset).startswith("data/pydata/"))


def _literal_arguments(call):
    """Return exact, unescaped literal values in positional argument order."""
    args = call.get("arguments") or []
    if [arg.get("index") for arg in args] != list(range(1, len(args) + 1)):
        return None
    values = []
    for arg in args:
        literals = arg.get("literals") or []
        if (len(literals) != 1 or literals[0].get("type") != "string"
                or arg.get("expression") != literals[0].get("raw")):
            return None
        raw = literals[0].get("raw", "")
        if not isinstance(raw, str) or "\\" in raw or len(raw) < 2:
            return None
        values.append(literals[0].get("value"))
    return values if all(isinstance(value, str) for value in values) else None


def _parse_constant_groups(data: bytes):
    """Parse bounded namespace/member/signed-value groups; reject partial data."""
    at = 0

    def word():
        nonlocal at
        if at + 4 > len(data):
            raise ValueError("truncated u32")
        value = struct.unpack_from("<I", data, at)[0]
        at += 4
        return value

    def name():
        nonlocal at
        size = word()
        if size > _MAX_STRING or size > len(data) - at:
            raise ValueError("invalid string length")
        raw = data[at:at + size]
        at += size
        return raw.split(b"\0", 1)[0].decode("utf-8", "strict")

    group_count = word()
    if group_count > _MAX_GROUPS:
        raise ValueError("invalid group count")
    groups = []
    total = 0
    for group_index in range(group_count):
        namespace = name()
        entry_count = word()
        if entry_count > _MAX_ENTRIES or total + entry_count > _MAX_TOTAL_ENTRIES:
            raise ValueError("invalid entry count")
        entries = []
        for ordinal in range(entry_count):
            key = name()
            value = struct.unpack("<i", struct.pack("<I", word()))[0]
            entries.append({"name": key, "value": value, "ordinal": ordinal})
        groups.append({"namespace": namespace, "group": group_index, "entries": entries})
        total += entry_count
    if at != len(data):
        raise ValueError("trailing bytes")
    return groups


def _constant_data(ctx, asset):
    """Use the package index's scoped path and header recipe, then validate all bytes."""
    raw = package_index._source_file(ctx, asset).read_bytes()
    attempts = [("identity:bounded-pycst-groups", raw)]
    shifted = package_index._subtract_header(raw)
    if shifted is not None and shifted != raw:
        attempts.append(("subtract-first-four-bytes(+1,+2,+3,+4):bounded-pycst-groups", shifted))
    errors = []
    for recipe, candidate in attempts:
        try:
            groups = _parse_constant_groups(candidate)
        except (ValueError, UnicodeDecodeError, struct.error) as exc:
            errors.append(f"{recipe}: {exc}")
            continue
        return groups, recipe, len(raw), len(candidate)
    raise ValueError("; ".join(errors))


def _base_reference(call, script, kind, args, reason=None):
    value = args[1] if args and len(args) > 1 else None
    namespace = args[0] if args else None
    return {
        "id": stable_id("ios-semantic-reference", call.get("id"), kind),
        "site": f"{call.get('site', script.get('site', ''))}:{call.get('line', 0)}:{call.get('name', '')}",
        "name": value if value is not None else call.get("expression", ""),
        "kind": "symbolic_reference",
        "reference_kind": kind,
        "source_platform": "ios",
        "source_asset_id": call.get("source_asset_id"),
        "status": "UNRESOLVED",
        "reason": reason,
        "work_status": "DONE",
        "confidence": "LIKELY",
        "rule": "Exact iPad Lua provider operand is associated only with a validated iPad serialized source group; native lookup semantics remain unverified.",
        "resolves_to": [],
        "candidates": [],
        "context": {
            "script": call.get("site"),
            "script_id": call.get("script_id"),
            "call_id": call.get("id"),
            "call": call.get("name"),
            "call_mode": call.get("call_mode"),
            "line": call.get("line"),
            "namespace": namespace,
            "argument_index": 2,
            "logical_source_path": call.get("logical_source_path"),
            "source_asset_id": call.get("source_asset_id"),
            "source_decode_recipe": call.get("source_decode_recipe"),
        },
        "ios_semantic_resolution": {
            "resolver": _RESOLVER,
            "native_contract_status": "UNVERIFIED",
            "native_id_verified": False,
            "evidence": {
                "package_version": call.get("package_version"),
                "script_source_sha256": call.get("source_sha256"),
                "script_decoded_sha256": call.get("decoded_sha256"),
                "call_expression": call.get("expression"),
                "call_line": call.get("line"),
                "namespace": namespace,
                "name": value,
                "limitation": "The iPad package proves stored names/values and their serialized ordinals. Android IDA is a separate native build and no iPhone/iPad native contract is asserted.",
            },
        },
    }


def _finish(ref, status, reason, evidence, targets=(), candidates=()):
    ref["status"] = status
    if status == "UNRESOLVED":
        ref["reason"] = "OTHER"
        ref["reason_code"] = "OTHER"
    else:
        ref["reason"] = None
        if evidence.get("reason_code"):
            ref["reason_code"] = evidence["reason_code"]
        else:
            ref.pop("reason_code", None)
    if status == "AMBIGUOUS":
        ref["resolves_to"] = [{**target, "candidate_status": "AMBIGUOUS_CANDIDATE"}
                               for target in targets]
    else:
        ref["resolves_to"] = list(targets)
    ref["candidates"] = list(candidates)
    ref["candidate_note"] = reason if status in ("AMBIGUOUS", "UNRESOLVED") else None
    ref["ios_semantic_resolution"]["evidence"].update(evidence)


def _restore_owned_rows(rows):
    restored = []
    for row in rows:
        semantic = row.get("ios_semantic_resolution") or {}
        if semantic.get("resolver") != _RESOLVER:
            restored.append(row)
            continue
        prior = semantic.get("prior_fields")
        if isinstance(prior, dict):
            for key in _PRIOR_FIELDS:
                if key in prior:
                    row[key] = copy.deepcopy(prior[key])
                else:
                    row.pop(key, None)
        row.pop("ios_semantic_resolution", None)
        if row.get("reference_kind") not in ("ios_getpyoid_source_name", "ios_getpycst_stored_constant"):
            restored.append(row)
    return restored


def _annotate_nested_literals(refs, semantic_refs):
    """Attach exact nested provider evidence to existing literal-reference IDs."""
    query_by_key = {}
    for semantic in semantic_refs:
        context = semantic.get("context") or {}
        evidence = semantic.get("ios_semantic_resolution", {}).get("evidence", {})
        key = (context.get("script_id"), context.get("line"),
               context.get("call"), context.get("namespace"), semantic.get("name"))
        query_by_key.setdefault(key, []).append((semantic, evidence))

    for ref in refs:
        context = ref.get("context") or {}
        if ref.get("source_platform") != "ios" or not context.get("script_id"):
            continue
        match = _NESTED_PROVIDER.fullmatch(str(context.get("argument_expression") or ""))
        if not match:
            continue
        api, namespace, name = match.group(1), match.group(3), match.group(5)
        ref_name = ref.get("name")
        if ref_name not in (namespace, name) or namespace == name:
            continue
        key = (context.get("script_id"), context.get("literal_line"), api, namespace, name)
        candidates = query_by_key.get(key, [])
        if len(candidates) != 1:
            continue
        provider, provider_evidence = candidates[0]
        if not ref.get("ios_semantic_resolution"):
            ref["ios_semantic_resolution"] = {"resolver": _RESOLVER,
                                             "prior_fields": {field: copy.deepcopy(ref[field])
                                                              for field in _PRIOR_FIELDS if field in ref},
                                             "native_contract_status": "UNVERIFIED",
                                             "native_id_verified": False,
                                             "evidence": {}}
        semantic = ref["ios_semantic_resolution"]
        semantic["native_contract_status"] = "UNVERIFIED"
        semantic["native_id_verified"] = False
        semantic["evidence"].update({
            "nested_provider": api,
            "nested_provider_call_id": provider.get("context", {}).get("call_id"),
            "provider_reference_id": provider.get("id"),
            "provider_status": provider.get("status"),
            "namespace": namespace,
            "name": name,
            "provider_evidence": copy.deepcopy(provider_evidence),
            "match_basis": "Exact parsed GetPyOID/GetPyCst(namespace,name) expression at the same iPad script and line; original reference ID retained.",
        })
        if ref_name == namespace:
            ref.update(status="NOT-NEEDED", reason="OTHER", reason_code="OTHER",
                       work_status="DONE", confidence="LIKELY",
                       rule="Exact nested provider expression establishes this string as its first namespace argument, not an asset name.",
                       resolves_to=[], candidates=[], candidate_note="Syntactic provider namespace literal; iPad native namespace contract remains unverified.")
            semantic["evidence"]["literal_role"] = "provider_namespace"
            continue
        if api == "GetPyOID" and provider.get("status") == "RESOLVED":
            target = copy.deepcopy((provider.get("resolves_to") or [None])[0])
            if target is not None:
                ref.update(status="RESOLVED", reason=None, confidence="LIKELY", work_status="DONE",
                           rule="Exact nested iPad GetPyOID source query resolves to the same validated stored name group/ordinal.",
                           resolves_to=[target], candidates=[], candidate_note=None)
                ref.pop("reason_code", None)
                semantic["evidence"]["literal_role"] = "stored_source_name"
                continue
        if api == "GetPyCst" and provider.get("status") == "RESOLVED":
            ref.update(status="NOT-NEEDED", reason="OTHER", reason_code="OTHER",
                       work_status="DONE", confidence="LIKELY",
                       rule="Exact nested iPad GetPyCst pair is a stored constant label, not an asset name.",
                       resolves_to=[], candidates=[], candidate_note="The validated iPad constant row is recorded on its provider reference.")
            semantic["evidence"]["literal_role"] = "stored_constant_name"


def _collect_queries(packed):
    script_records = packed.get("script_records") or []
    scripts = {row.get("id"): row for row in script_records if row.get("id")}
    queries = []
    for call in packed.get("script_calls") or []:
        api = call.get("name")
        if api not in ("GetPyOID", "GetPyCst"):
            continue
        script = scripts.get(call.get("script_id"))
        if not script or script.get("platform") != "ios":
            continue
        queries.append((call, script, api))
    return sorted(queries, key=lambda item: (str(item[0].get("site")), item[0].get("line") or 0,
                                             item[0].get("source_offset") or 0, item[0].get("id", "")))


def _validated_name_streams(ctx, assets):
    streams = []
    errors = {}
    for asset in assets:
        logical = _logical(asset)
        if (not _ios_asset(asset) or not logical.lower().endswith(_NAME_SUFFIXES)):
            continue
        try:
            groups = package_index.read_name_groups(ctx, asset)
            if (asset.get("name_group_count") is not None
                    and len(groups) != asset.get("name_group_count")):
                raise ValueError("stored group count does not match validated payload")
            if (asset.get("name_count") is not None
                    and sum(len(group) for group in groups) != asset.get("name_count")):
                raise ValueError("stored name count does not match validated payload")
            streams.append({"asset": asset, "logical_path": logical, "groups": groups})
        except (OSError, ValueError, UnicodeError, struct.error) as exc:
            errors[_asset_id(asset)] = {"source_asset_id": _asset_id(asset), "logical_path": logical,
                                        "reason": str(exc), "decode_recipe": asset.get("decode_recipe")}
    return streams, errors


def _namespace_families(queries, streams):
    """Infer a file family only when repeated exact calls give a unique lead."""
    names_by_namespace = {}
    for call, script, api in queries:
        if api != "GetPyOID":
            continue
        args = _literal_arguments(call)
        if args and len(args) == 2:
            names_by_namespace.setdefault(args[0], set()).add(args[1])

    result = {}
    for namespace, names in names_by_namespace.items():
        support = {}
        for name in names:
            locations = []
            for stream in streams:
                for group_index, group in enumerate(stream["groups"]):
                    for ordinal, stored_name in enumerate(group):
                        if stored_name == name:
                            locations.append((stream, group_index, ordinal))
            if len(locations) == 1:
                stream, group_index, _ = locations[0]
                path = stream["logical_path"]
                support[path] = support.get(path, 0) + 1
        ranked = sorted(support.items(), key=lambda item: (-item[1], item[0]))
        if ranked and ranked[0][1] >= 2 and (len(ranked) == 1 or ranked[0][1] > ranked[1][1]):
            result[namespace] = {"status": "LIKELY", "logical_path": ranked[0][0],
                                 "support": ranked, "query_name_count": len(names)}
        elif (len(ranked) > 1 and ranked[0][1] == ranked[1][1]
              and ranked[0][1] >= 2):
            result[namespace] = {"status": "AMBIGUOUS", "logical_path": None,
                                 "support": ranked, "query_name_count": len(names)}
        elif ranked:
            result[namespace] = {"status": "UNRESOLVED", "logical_path": None,
                                 "support": ranked, "query_name_count": len(names)}
        else:
            result[namespace] = {"status": "UNRESOLVED", "logical_path": None,
                                 "support": [], "query_name_count": len(names)}
    return result


def _name_candidates(streams, name, logical_path=None):
    candidates = []
    for stream in streams:
        if logical_path is not None and stream["logical_path"] != logical_path:
            continue
        asset = stream["asset"]
        for group_index, group in enumerate(stream["groups"]):
            for ordinal, stored_name in enumerate(group):
                if stored_name == name:
                    candidates.append({
                        "type": "ios_source_name",
                        "source_asset_id": _asset_id(asset),
                        "source_file": stream["logical_path"],
                        "group": group_index,
                        "ordinal": ordinal,
                        "name": name,
                        "confidence": "LIKELY",
                        "native_id_verified": False,
                    })
    return candidates


def _constant_streams(ctx, assets):
    streams = []
    errors = {}
    for asset in assets:
        logical = _logical(asset)
        if not (_ios_asset(asset) and logical.lower().endswith(_CONSTANT_SUFFIX)):
            continue
        try:
            groups, recipe, raw_size, decoded_size = _constant_data(ctx, asset)
            streams.append({"asset": asset, "logical_path": logical, "groups": groups,
                            "recipe": recipe, "raw_size": raw_size, "decoded_size": decoded_size})
        except (OSError, ValueError, UnicodeError, struct.error) as exc:
            errors[_asset_id(asset)] = {"source_asset_id": _asset_id(asset), "logical_path": logical,
                                        "reason": str(exc), "decode_recipe": "preserve-raw"}
    return streams, errors


def _constant_candidates(streams, namespace, name):
    candidates = []
    for stream in streams:
        for group in stream["groups"]:
            if group["namespace"] != namespace:
                continue
            for entry in group["entries"]:
                if entry["name"] == name:
                    asset = stream["asset"]
                    candidates.append({
                        "type": "ios_stored_constant",
                        "source_asset_id": _asset_id(asset),
                        "source_file": stream["logical_path"],
                        "group": group["group"],
                        "ordinal": entry["ordinal"],
                        "namespace": namespace,
                        "name": name,
                        "stored_value": entry["value"],
                        "confidence": "LIKELY",
                        "native_id_verified": False,
                    })
    return candidates


def resolve(ctx, refs, tables, assets, packed):
    """Append stable iPad source associations to the reference ledger.

    ``tables`` is accepted to keep the atlas module contract uniform. It is
    deliberately unused: Android table rows cannot establish iPad IDs.
    The function deep-copies inputs and replaces only its own prior rows, so it
    is safe to run repeatedly during atlas regeneration.
    """
    del tables
    result = _restore_owned_rows(copy.deepcopy(refs))

    queries = _collect_queries(packed or {})
    ios_assets = [asset for asset in assets if _mapped_ios_asset(asset)]
    assets_by_id = {_asset_id(asset): asset for asset in ios_assets}
    streams, name_errors = _validated_name_streams(ctx, ios_assets)
    family_map = _namespace_families(queries, streams)
    constant_streams, constant_errors = _constant_streams(ctx, ios_assets)

    for call, script, api in queries:
        # Call and script metadata must point back to the same admitted iPad
        # packed asset. A mismatched source is preserved as an unresolved row.
        asset_id = call.get("source_asset_id")
        source_asset = assets_by_id.get(asset_id)
        valid_source = (source_asset is not None and script.get("source") == "ios-package"
                        and script.get("source_asset_id") == asset_id
                        and script.get("logical_path", "").startswith("data/scripts/"))
        args = _literal_arguments(call)
        ref_kind = "ios_getpyoid_source_name" if api == "GetPyOID" else "ios_getpycst_stored_constant"
        ref = _base_reference(call, script, ref_kind, args,
                              reason="IOS_SOURCE_MISMATCH" if not valid_source else None)
        if not valid_source:
            _finish(ref, "UNRESOLVED", "Call and script do not share a validated iPad package source asset.",
                    {"reason_code": "IOS_SOURCE_MISMATCH"})
            result.append(ref)
            continue
        if call.get("call_mode") != "global":
            _finish(ref, "UNRESOLVED", "The source call is not an exact global provider call.",
                    {"reason_code": "CALL_MODE_UNVERIFIED"})
            result.append(ref)
            continue
        if not args or len(args) != 2:
            _finish(ref, "UNRESOLVED", "Both provider arguments are not exact plain string literals.",
                    {"reason_code": "DYNAMIC_ARGUMENTS"})
            result.append(ref)
            continue

        namespace, name = args
        if api == "GetPyOID":
            family = family_map.get(namespace, {"status": "UNRESOLVED", "logical_path": None,
                                                "support": [], "query_name_count": 0})
            evidence = {
                "reason_code": None,
                "source_mapping": "iPad package logical path plus repeated exact namespace/name calls",
                "namespace_family_status": family["status"],
                "namespace_family_support": family["support"],
                "namespace_query_name_count": family["query_name_count"],
                "name_stream_errors": list(name_errors.values()),
            }
            if family.get("status") != "LIKELY":
                other = _name_candidates(streams, name)
                candidates = other[:100]
                status = "AMBIGUOUS" if family.get("status") == "AMBIGUOUS" else "UNRESOLVED"
                reason = ("Namespace-to-file-family mapping has tied source evidence; no global name match is promoted."
                          if status == "AMBIGUOUS" else
                          "No repeated exact iPad source pairs establish a names-stream family for this namespace.")
                evidence["reason_code"] = "NAMESPACE_FAMILY_AMBIGUOUS" if status == "AMBIGUOUS" else "NAMESPACE_FAMILY_UNKNOWN"
                _finish(ref, status, reason, evidence,
                        targets=candidates if status == "AMBIGUOUS" else (), candidates=candidates)
                result.append(ref)
                continue
            candidates = _name_candidates(streams, name, family["logical_path"])
            evidence.update({"source_family": family["logical_path"],
                             "family_association_confidence": "LIKELY",
                             "name_stream_validation": "package_index.read_name_groups complete bounded parse",
                             "native_contract_status": "UNVERIFIED"})
            if len(candidates) == 1:
                target = {**candidates[0], "namespace": namespace}
                evidence.update({"matched_logical_family": family["logical_path"],
                                 "source_group": target["group"], "stored_ordinal": target["ordinal"],
                                 "native_id_verified": False,
                                 "association_meaning": "Exact source name association; ordinal is a stored position only."})
                _finish(ref, "RESOLVED", None, evidence, targets=[target])
            elif candidates:
                evidence["reason_code"] = "DUPLICATE_STORED_NAME"
                _finish(ref, "AMBIGUOUS", "The exact name occurs more than once in its inferred iPad family.",
                        evidence, targets=candidates[:100], candidates=candidates[:100])
            else:
                broken = [row for row in name_errors.values()
                          if family["logical_path"] == row.get("logical_path")]
                evidence["reason_code"] = "SOURCE_GROUP_VALIDATION_FAILED" if broken else "NAME_NOT_IN_FAMILY"
                evidence["invalid_source_groups"] = broken
                _finish(ref, "UNRESOLVED",
                        "The exact name is absent from the inferred source family." if not broken else
                        "The inferred source family failed package name-group validation.", evidence)
        else:
            candidates = _constant_candidates(constant_streams, namespace, name)
            evidence = {
                "reason_code": None,
                "constant_source_validation": "bounded namespace/member/signed-value parse with complete consumption",
                "constant_stream_errors": list(constant_errors.values()),
                "native_contract_status": "UNVERIFIED",
                "stored_value_is_native_return": False,
            }
            if len(candidates) == 1:
                target = candidates[0]
                stream = next(stream for stream in constant_streams
                              if _asset_id(stream["asset"]) == target["source_asset_id"])
                evidence.update({"source_decode_recipe": stream["recipe"],
                                 "raw_bytes": stream["raw_size"], "decoded_bytes": stream["decoded_size"],
                                 "serialized_value_confidence": "PROVEN",
                                 "package_logical_mapping_confidence": "LIKELY",
                                 "link_confidence": "LIKELY",
                                 "stored_value": target["stored_value"],
                                 "stored_group": target["group"], "stored_ordinal": target["ordinal"],
                                 "source_file": target["source_file"],
                                 "native_id_verified": False})
                _finish(ref, "RESOLVED", None, evidence, targets=[target])
            elif candidates:
                evidence["reason_code"] = "DUPLICATE_STORED_CONSTANT"
                _finish(ref, "AMBIGUOUS", "The exact namespace/member pair occurs in multiple iPad constant groups.",
                        evidence, targets=candidates[:100], candidates=candidates[:100])
            else:
                evidence["reason_code"] = "CONSTANT_PAIR_NOT_STORED"
                _finish(ref, "UNRESOLVED", "The exact namespace/member pair is absent from validated iPad constant streams.", evidence)
        result.append(ref)

    semantic_refs = [row for row in result
                     if (row.get("ios_semantic_resolution") or {}).get("resolver") == _RESOLVER
                     and row.get("reference_kind") in ("ios_getpyoid_source_name", "ios_getpycst_stored_constant")]
    _annotate_nested_literals(result, semantic_refs)

    # Duplicate IDs indicate malformed call input, not a harmless repeat.
    owned = [row for row in result if (row.get("ios_semantic_resolution") or {}).get("resolver") == _RESOLVER]
    seen = set()
    for row in owned:
        if row["id"] in seen:
            raise ValueError(f"duplicate iOS semantic reference id: {row['id']}")
        seen.add(row["id"])
    return result
