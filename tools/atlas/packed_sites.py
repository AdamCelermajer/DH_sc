"""Static reference and script-call sites in validated iPad package resources.

All package reads go through :mod:`package_index` and stay in memory. The rows
retain physical package paths, raw/decoded hashes, and logical aliases so a
source site can be traced back to its numbered DAT without claiming that the
iPad loader or native bindings match Android.
"""
from __future__ import annotations

from collections import Counter, defaultdict
import hashlib
from pathlib import Path, PurePosixPath
import re

import package_index
import scripts
import sites
from common import physical_path, stable_id


_PACKAGE_MARKER = "/dungeonhunter2ipad.app/data/"
_XML_SITE_SUFFIXES = {".xml", ".lightset_xml", ".mlx", ".mgp", ".mvp", ".mgx", ".mvx"}
_SITE_SUFFIXES = _XML_SITE_SUFFIXES | {".bdae", ".luac", ".lua"}
_FILE_REFERENCE_KINDS = {
    ".bdae": "model_or_effect", ".wav": "sound", ".vxn": "sound",
    ".ogg": "sound", ".mp3": "sound", ".luac": "script",
    ".lua": "script", ".pyscript": "script", ".mlx": "level",
    ".mgp": "model", ".mgx": "model", ".mvx": "model",
    ".mvp": "model", ".png": "image", ".dds": "image",
    ".tga": "image", ".jpg": "image", ".jpeg": "image",
    ".swf": "ui_movie", ".fnt": "font", ".ttf": "font",
    ".otf": "font", ".xml": "data", ".bin": "data",
}


class _DecodedPath:
    """Path facade: physical path for source identity, logical suffix for readers."""

    def __init__(self, ctx, asset: dict, logical_path: str, data: bytes):
        self._physical = physical_path(ctx, asset["path"])
        self._data = data
        self.suffix = PurePosixPath(logical_path).suffix

    def __fspath__(self):
        return str(self._physical)

    def __str__(self):
        return str(self._physical)

    def read_bytes(self):
        return self._data


def _asset_id(asset: dict) -> str:
    return str(asset.get("id") or stable_id("asset", asset.get("path", "")))


def _logical_path(asset: dict) -> str | None:
    paths = asset.get("logical_paths") or []
    return str(paths[0]).replace("\\", "/") if paths else None


def _is_package_asset(asset: dict) -> bool:
    path = str(asset.get("path", "")).replace("\\", "/").lower()
    source = str(asset.get("source", "")).lower()
    return source in {"ios-packed", "ios-package", "ipad-packed"} or _PACKAGE_MARKER in path


def _enriched_assets(ctx, assets: list[dict]) -> list[dict]:
    """Attach package_index metadata when the caller passed pre-enrichment rows."""
    rows = [dict(asset) for asset in assets if isinstance(asset, dict) and _is_package_asset(asset)]
    if not rows:
        return []
    required = ("package_relative", "logical_paths", "decode_recipe", "decoded_sha256",
                "decode_confidence", "mapping_confidence")
    if any(not all(key in row for key in required) for row in rows):
        updates = {row["path"]: row for row in package_index.enrich(ctx, rows) if row.get("path")}
        for row in rows:
            row.update(updates.get(row.get("path"), {}))
    for row in rows:
        row.setdefault("id", _asset_id(row))
    return rows


def _same_name_native_candidates(ctx, names: set[str]) -> dict[str, list[dict]]:
    """Return Android IDA registrations as unproven same-name candidates."""
    registrations = scripts._ida_registration_map(Path(ctx.inputs))
    wanted = {name.casefold() for name in names}
    found: dict[str, list[dict]] = defaultdict(list)
    for public_name, entries in registrations.items():
        key = str(public_name).casefold()
        if key not in wanted:
            continue
        for entry in entries:
            found[key].append({
                "public_name": public_name,
                "handler": entry.get("handler"),
                "address": entry.get("address"),
                "binding": entry.get("binding"),
                "evidence_path": ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c",
                "confidence": "INFERENCE",
                "candidate_note": "Same public name appears in the Android APK IDA registration; this does not prove an iPad binding or matching native behavior.",
            })
    return found


def _decoded_matches_android(ctx, logical_path: str, decoded_hash: str, decoded_bytes: int) -> bool:
    """Require current byte equality to a retained Android source counterpart."""
    root = package_index._android_pair_root(ctx)
    if root is None or not logical_path.startswith("data/"):
        return False
    counterpart = root.joinpath(*logical_path.split("/")[1:])
    if not counterpart.is_file() or counterpart.stat().st_size != decoded_bytes:
        return False
    digest = hashlib.sha256(counterpart.read_bytes()).hexdigest()
    return digest == decoded_hash


def _contextualize_reference(ref: dict, asset: dict, logical_path: str, source_hash: str,
                             decoded_hash: str, decode_recipe: str, package_version: str) -> dict:
    row = dict(ref)
    row["source_asset_id"] = _asset_id(asset)
    row["source_path"] = asset.get("path")
    row["logical_source_path"] = logical_path
    row["source_sha256"] = source_hash
    row["decoded_sha256"] = decoded_hash
    row["source_decode_recipe"] = decode_recipe
    row["package_version"] = package_version
    row["source_platform"] = "ios"
    row["producer_role_confidence"] = "CROSSCHECKED" if asset.get("_decoded_matches_android") else "LIKELY"
    row["producer_role_note"] = (
        "Logical file association is from the embedded package map; no iPad loader admission or native consumer was recovered."
        if row["producer_role_confidence"] == "LIKELY"
        else "Decoded bytes exactly match the retained Android source counterpart; this confirms byte identity, not iPad runtime admission."
    )
    context = dict(row.get("context") or {})
    context.update({
        "source_asset_id": row["source_asset_id"],
        "source_path": row["source_path"],
        "logical_source_path": logical_path,
        "source_sha256": source_hash,
        "decoded_sha256": decoded_hash,
        "source_decode_recipe": decode_recipe,
        "package_version": package_version,
    })
    row["context"] = context
    return row


def _literal_references(script_record: dict, call: dict, call_id: str,
                        logical_path: str, source_hash: str, decoded_hash: str,
                        decode_recipe: str, package_version: str) -> list[dict]:
    """Capture typed file/name arguments without assigning table-ID semantics."""
    result = []
    call_name = str(call.get("name", ""))
    for argument in call.get("arguments", []):
        arg_index = argument.get("index")
        for literal in argument.get("literals", []):
            if literal.get("type") != "string":
                # Numeric literals stay attached to the call expression. They
                # are deliberately not resolved against Android pydata tables.
                continue
            value = str(literal.get("value", ""))
            if not value:
                continue
            ext = PurePosixPath(value.replace("\\", "/")).suffix.lower()
            is_include = call_name.casefold() in {"include", "require", "dofile"} and arg_index == 1
            if ext in _FILE_REFERENCE_KINDS:
                kind = _FILE_REFERENCE_KINDS[ext]
                reference_kind = "file_name"
            elif is_include:
                kind = "script"
                reference_kind = "script_name"
            elif re.search(r"(?:anim|sound|audio|fx|effect|projectile|skill|script|model|dialog|cutscene|prop|spawn|target|zone|trap|quest|music|play|load|template|actor|character)", call_name, re.I):
                kind = "named_reference"
                reference_kind = "name"
            else:
                continue
            site = f'{script_record["site"]}:{literal.get("line", call.get("line", 0))}:literal@{literal.get("offset", 0)}'
            ref = {
                "id": stable_id("ios-script-ref", call_id, arg_index, literal.get("offset", literal.get("line")), value, reference_kind),
                "site": site,
                "name": value,
                "kind": kind,
                "reference_kind": reference_kind,
                "rule": "String literal in a validated iPad Lua call argument; raw source name only.",
                "status": "TODO",
                "work_status": "TODO",
                "reason": None,
                "candidate_note": "Preserved as a source name. Do not assign a numeric/table ID or claim runtime resolution from this literal alone.",
                "candidates": [],
                "resolves_to": [],
                "confidence": "LIKELY",
                "context": {
                    "script": script_record["site"],
                    "script_id": script_record["id"],
                    "call_id": call_id,
                    "call": call_name,
                    "call_mode": call.get("mode"),
                    "argument_index": arg_index,
                    "argument_expression": argument.get("expression"),
                    "literal_type": "string",
                    "literal_line": literal.get("line"),
                    "literal_offset": literal.get("offset"),
                    "source_path": script_record["site"],
                    "source_asset_id": script_record["source_asset_id"],
                    "logical_source_path": logical_path,
                    "source_sha256": source_hash,
                    "decoded_sha256": decoded_hash,
                    "source_decode_recipe": decode_recipe,
                    "package_version": package_version,
                    "numeric_table_resolution": "not_attempted",
                },
                "source_asset_id": script_record["source_asset_id"],
                "source_path": script_record["site"],
                "logical_source_path": logical_path,
                "source_sha256": source_hash,
                "decoded_sha256": decoded_hash,
                "source_decode_recipe": decode_recipe,
                "package_version": package_version,
                "source_platform": "ios",
                "producer_role_confidence": "LIKELY",
                "producer_role_note": "The API/name suggests a resource role, but the iPad loader admission and native consumer are unrecovered.",
                "port_status": "UNKNOWN",
                "implementation": None,
            }
            result.append(ref)
    return result


def _function_display(call: dict) -> str:
    mode = call.get("mode")
    name = str(call.get("name", "<unknown>"))
    receiver = call.get("receiver")
    if mode in ("object_method", "alias_method") and receiver:
        return f"{receiver}:{name}"
    if mode == "qualified_function" and receiver:
        return f"{receiver}.{name}"
    if mode == "dynamic":
        return "<dynamic>"
    return name


def extract(ctx, assets: list[dict], tables: list[dict] | None = None) -> dict:
    """Extract references and Lua call sites from validated iPad package bytes.

    `tables` is accepted to match the atlas extractor interface, but is not used
    to resolve numeric Lua values. iPad table names and IDs remain names-only
    until their package schema and the iPad's own consumers are established.
    """
    del tables
    package_assets = _enriched_assets(ctx, assets)
    # Validate the package's embedded map and TSV transcription once. The
    # package reader rechecks aliases on each call, so below we pass an
    # equivalent validated physical path with the alias removed; its strict
    # type validator still selects the format from the mapped logical suffix.
    logical_by_package = package_index._read_mapping(ctx)
    eligible = []
    for asset in package_assets:
        logical = _logical_path(asset)
        suffix = PurePosixPath(logical or asset.get("relative", "")).suffix.lower()
        if logical and suffix in _SITE_SUFFIXES:
            eligible.append((asset, logical, suffix))

    refs: list[dict] = []
    script_records: list[dict] = []
    script_calls: list[dict] = []
    script_nodes: dict[str, dict] = {}
    gaps: list[dict] = []
    coverage = {
        "source": "ios-package",
        "package_version": "Dungeon Hunter 2 HD iPad CFBundleVersion 1.0.0",
        "eligible_asset_files": len(eligible),
        "eligible_by_suffix": dict(Counter(suffix for _, _, suffix in eligible)),
        "decoded_by_suffix": Counter(),
        "processed_by_suffix": Counter(),
        "references_by_kind": Counter(),
        "script_files": 0,
        "script_plaintext_files": 0,
        "script_bytecode_files": 0,
        "script_calls": 0,
        "script_literals": 0,
        "script_call_nodes": 0,
        "xml_files": 0,
        "bdae_files": 0,
        "package_asset_rows": len(package_assets),
    }
    package_version = coverage["package_version"]

    for asset, logical, suffix in eligible:
        metadata_raw_hash = str(asset.get("sha256") or "")
        package_relative = str(asset.get("package_relative") or "").replace("\\", "/")
        try:
            if list(asset.get("logical_paths") or []) != [logical] or logical_by_package.get(package_relative) != logical:
                raise ValueError("logical alias does not match the validated embedded package map")
            read_asset = dict(asset)
            read_asset["logical_paths"] = []
            read_asset["relative"] = logical
            raw = package_index._source_file(ctx, read_asset).read_bytes()
            raw_hash = hashlib.sha256(raw).hexdigest()
            decoded = package_index.read_resource(ctx, read_asset)
            decoded_hash = hashlib.sha256(decoded).hexdigest()
            fmt, decode_recipe, decode_confidence, classified = package_index._classify([], logical, raw)
            if classified is None or classified != decoded:
                raise ValueError("package_index reader and typed classifier disagree")
            expected_hash = asset.get("decoded_sha256")
            mismatch = []
            if metadata_raw_hash and raw_hash != metadata_raw_hash:
                mismatch.append({"field": "sha256", "metadata": metadata_raw_hash, "observed": raw_hash})
            if expected_hash and decoded_hash != expected_hash:
                mismatch.append({"field": "decoded_sha256", "metadata": expected_hash, "observed": decoded_hash})
            if mismatch:
                gaps.append({
                    "source_asset_id": _asset_id(asset),
                    "source_path": asset.get("path"),
                    "logical_path": logical,
                    "suffix": suffix,
                    "source_sha256": raw_hash,
                    "decoded_sha256": decoded_hash,
                    "metadata_hash_mismatches": mismatch,
                    "reason_code": "PACKED_ASSET_METADATA_HASH_MISMATCH",
                    "reason": "Current package bytes pass the bounded type validator, but stored asset hashes disagree; current observed hashes are used for sites.",
                    "package_version": package_version,
                })
            asset = dict(asset)
            asset["_decoded_matches_android"] = _decoded_matches_android(ctx, logical, decoded_hash, len(decoded))
        except Exception as exc:
            gaps.append({
                "source_asset_id": _asset_id(asset),
                "source_path": asset.get("path"),
                "logical_path": logical,
                "suffix": suffix,
                "source_sha256": metadata_raw_hash,
                "raw_magic_hex": asset.get("raw_magic_hex"),
                "decode_recipe": asset.get("decode_recipe"),
                "reason_code": "PACKED_RESOURCE_NOT_VALIDATED",
                "reason": str(exc),
                "package_version": package_version,
            })
            continue

        coverage["decoded_by_suffix"][suffix] += 1
        facade = _DecodedPath(ctx, asset, logical, decoded)
        current_refs = []
        if suffix in _XML_SITE_SUFFIXES:
            coverage["xml_files"] += 1
            coverage["processed_by_suffix"][suffix] += 1
            current_refs = sites.xml_refs(ctx, facade)
        elif suffix == ".bdae":
            coverage["bdae_files"] += 1
            coverage["processed_by_suffix"][suffix] += 1
            current_refs, event_errors = sites.binary_refs(ctx, facade)
            for error in event_errors:
                gaps.append({
                    "source_asset_id": _asset_id(asset),
                    "source_path": asset.get("path"),
                    "logical_path": logical,
                    "source_sha256": raw_hash,
                    "decoded_sha256": decoded_hash,
                    "reason_code": "BRES_REFERENCE_DECODE_FAILED",
                    "reason": error.get("reason"),
                    "package_version": package_version,
                })
        elif suffix in (".luac", ".lua"):
            fmt = str(fmt or "")
            coverage["script_files"] += 1
            if "bytecode" in fmt.casefold():
                coverage["script_bytecode_files"] += 1
                gaps.append({
                    "source_asset_id": _asset_id(asset),
                    "source_path": asset.get("path"),
                    "logical_path": logical,
                    "source_sha256": raw_hash,
                    "decoded_sha256": decoded_hash,
                    "reason_code": "LUA_BYTECODE_CALLS_NOT_DECOMPILED",
                    "reason": "Lua bytecode passed the package type validator, but this extractor indexes source calls only and does not decompile bytecode.",
                    "package_version": package_version,
                })
                continue
            try:
                text = decoded.decode("utf-8-sig")
            except UnicodeDecodeError as exc:
                gaps.append({
                    "source_asset_id": _asset_id(asset),
                    "source_path": asset.get("path"),
                    "logical_path": logical,
                    "source_sha256": raw_hash,
                    "decoded_sha256": decoded_hash,
                    "reason_code": "LUA_SOURCE_UTF8_DECODE_FAILED",
                    "reason": str(exc),
                    "package_version": package_version,
                })
                continue
            calls, local_names, defined_names, params = scripts._parse_script(text)
            site = str(asset.get("path"))
            script_id = stable_id("script", "ios-package", site, decoded_hash)
            lines = len(text.splitlines())
            call_literal_sites: dict[int, list[str]] = defaultdict(list)
            for parsed_call in calls:
                display = _function_display(parsed_call)
                parsed_call_id = stable_id("script-call", "ios-package", site,
                                           parsed_call.get("line"), parsed_call.get("_start"), display)
                for argument in parsed_call.get("arguments", []):
                    for literal in argument.get("literals", []):
                        if literal.get("offset") is not None:
                            call_literal_sites[literal["offset"]].append(parsed_call_id)
            literal_sites = []
            for token_kind, raw_token, line_no, start, end in scripts._tokens(text):
                if token_kind not in ("string", "number"):
                    continue
                value = raw_token[1:-1] if token_kind == "string" and len(raw_token) >= 2 else raw_token
                literal_sites.append({
                    "type": token_kind,
                    "raw": raw_token,
                    "value": value,
                    "line": line_no,
                    "offset": start,
                    "end_offset": end,
                    "call_ids": sorted(set(call_literal_sites.get(start, []))),
                })
            coverage["script_literals"] += len(literal_sites)
            record = {
                "id": script_id,
                "site": site,
                "name": PurePosixPath(logical).name,
                "platform": "ios",
                "source": "ios-package",
                "format": "lua-source",
                "bytes": len(decoded),
                "raw_bytes": len(raw),
                "lines": lines,
                "call_sites": len(calls),
                "engine_call_sites": 0,
                "literal_site_count": len(literal_sites),
                "literal_sites": literal_sites,
                "logical_path": logical,
                "logical_paths": [logical],
                "source_asset_id": _asset_id(asset),
                "source_path": site,
                "source_sha256": raw_hash,
                "decoded_sha256": decoded_hash,
                "source_decode_recipe": decode_recipe,
                "package_version": package_version,
                "mapping_confidence": asset.get("mapping_confidence", "LIKELY"),
                "decode_confidence": decode_confidence,
                "classification": {
                    "local_names": sorted(local_names),
                    "script_functions": sorted(defined_names),
                    "parameters": sorted(params),
                },
                "status": "TODO",
                "work_status": "TODO",
                "confidence": "PROVEN",
                "candidate_note": "Validated plaintext source; iPad runtime load order and native binding contracts remain unverified.",
            }
            script_records.append(record)
            coverage["script_plaintext_files"] += 1
            coverage["processed_by_suffix"][suffix] += 1

            for call in calls:
                display = _function_display(call)
                node_mode = str(call.get("mode", "unknown"))
                node_id = stable_id("ios-script-function", node_mode, display)
                if node_id not in script_nodes:
                    script_nodes[node_id] = {
                        "id": node_id,
                        "name": display,
                        "mode": node_mode,
                        "platform": "ios",
                        "namespace": "ios-package",
                        "status": "TODO",
                        "confidence": "PROVEN",
                        "call_count": 0,
                        "script_ids": [],
                        "native_function_candidates": [],
                        "candidate_note": "Call target is statically recognized from validated Lua source; no iPad native function is asserted.",
                    }
                node = script_nodes[node_id]
                node["call_count"] += 1
                if script_id not in node["script_ids"]:
                    node["script_ids"].append(script_id)
                call_id = stable_id("script-call", "ios-package", site, call.get("line"), call.get("_start"), display)
                call_row = {
                    "id": call_id,
                    "script_id": script_id,
                    "function_id": node_id,
                    "site": site,
                    "line": call.get("line"),
                    "source_offset": call.get("_start"),
                    "name": display,
                    "call_mode": node_mode,
                    "receiver": call.get("receiver"),
                    "alias_target": call.get("alias_target"),
                    "expression": call.get("expression"),
                    "arguments": call.get("arguments", []),
                    "engine_candidate": node_mode in ("global", "object_method", "qualified_function", "alias_method", "dynamic"),
                    "status": "TODO",
                    "confidence": "PROVEN",
                    "source_asset_id": _asset_id(asset),
                    "source_path": site,
                    "logical_source_path": logical,
                    "source_sha256": raw_hash,
                    "decoded_sha256": decoded_hash,
                    "source_decode_recipe": decode_recipe,
                    "package_version": package_version,
                }
                script_calls.append(call_row)
                coverage["script_calls"] += 1
                if call_row["engine_candidate"]:
                    record["engine_call_sites"] += 1
                refs.extend(_literal_references(record, call, call_id, logical,
                                                raw_hash, decoded_hash, decode_recipe, package_version))
            continue

        for ref in current_refs:
            refs.append(_contextualize_reference(ref, asset, logical, raw_hash,
                                                 decoded_hash, decode_recipe, package_version))

    candidate_names = {
        node["name"].split(":")[-1].split(".")[-1]
        for node in script_nodes.values()
        if node["mode"] in ("global", "object_method", "qualified_function", "alias_method")
    }
    native_candidates = _same_name_native_candidates(ctx, candidate_names)
    for node in script_nodes.values():
        if node["mode"] not in ("global", "object_method", "qualified_function", "alias_method"):
            continue
        base = node["name"].split(":")[-1].split(".")[-1]
        node["native_function_candidates"] = native_candidates.get(base.casefold(), [])
        if node["native_function_candidates"]:
            node["native_candidate_confidence"] = "INFERENCE"
            node["native_candidate_note"] = "Same-name Android IDA registrations are research candidates only; platform/version and behavior equivalence are unproven."
    coverage["script_call_nodes"] = len(script_nodes)
    coverage["references"] = len(refs)
    # Count the final result exactly once. XML/BDAE refs and Lua literal refs
    # are all included here; updating per-file and again here double-counted
    # non-script sites.
    coverage["references_by_kind"].update(ref.get("kind", "unknown") for ref in refs)
    coverage["references_by_kind"] = dict(coverage["references_by_kind"])
    coverage["script_records"] = len(script_records)
    coverage["script_nodes"] = len(script_nodes)
    coverage["gaps"] = len(gaps)
    coverage = {key: dict(value) if isinstance(value, Counter) else value
                for key, value in coverage.items()}
    for node in script_nodes.values():
        node["script_ids"].sort()
    script_records.sort(key=lambda row: row["logical_path"].casefold())
    script_calls.sort(key=lambda row: (row["site"].casefold(), row["line"] or 0, row["source_offset"] or 0))
    refs.sort(key=lambda row: (row.get("site", "").casefold(), row.get("id", "")))
    return {
        "references": refs,
        "script_nodes": sorted(script_nodes.values(), key=lambda row: (row["name"].casefold(), row["id"])),
        "script_records": script_records,
        "script_calls": script_calls,
        "coverage": coverage,
        "gaps": gaps,
    }
