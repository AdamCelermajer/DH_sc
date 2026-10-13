"""Resolve narrowly evidenced game-file paths from table and BDAE references.

This module maps logical file URIs to exact entries already present in the
Android/iPad inventories. It does not equate files across platforms merely
because their logical paths match.
"""
from __future__ import annotations

import copy
import re
from collections import defaultdict


_AI_EVIDENCE = (
    "Android IDA pseudocode: CharAI::StepCreateScript 0x3cf04c reads AITable "
    "Script length at row+40 and name at row+44, calls CharAI::SetScriptByName "
    "0x3ceeb0 for nonempty names, then sets scripted=1. SetScriptByName sends "
    "non-__ names to AISExternal and preserves the authored name. The external "
    "owner assigns path data/scripts/ai/ (port/level-world/reference/"
    "character-script-owner/NOTES.md, Stage 2). LuaManager::AddFile "
    "0x37b23c joins that path and the name, appending .luac when no .lua "
    "substring exists; it returns false on a missing stream and suppresses a "
    "duplicate load per LuaScript. Source: .local-inputs/ida-apk-export-"
    "2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003c/003cf04c.c, "
    "003c/003ceeb0.c, and 0037/0037b23c.c."
)
_TEXTURE_EVIDENCE = (
    "Android IDA pseudocode: SceneManager::SetMaterialRimLightEffects "
    "0x359d08 concatenates data/3d/textures/ with its authored texture-name "
    "argument and passes the resulting URI to CTextureManager::getTexture. "
    "Its gate requires a GL_Diffuse/GL_Xray material with a RimLight parameter; "
    "the scene-node overload 0x35a188 iterates eligible materials. Source: "
    ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/"
    "pseudocode/0035/00359d08.c and 0035/0035a188.c. The BDAE string-to-"
    "parameter association remains a source-role inference; this resolver "
    "claims only the exact logical URI/path mapping, not runtime material use."
)

_PLATFORM_BY_SOURCE = {
    "android": "android",
    "android-extra": "android",
    "original-cache": "android",
    "port-runtime-original-cache": "android",
    "ios": "ios",
    "ios-packed": "ios",
    "ios-glive": "ios",
}


def _logical(value):
    if not isinstance(value, str) or not value:
        return None
    value = value.replace("\\", "/")
    while value.startswith("./"):
        value = value[2:]
    value = value.lstrip("/")
    if value.casefold().startswith("data/"):
        value = value[5:]
    return value.casefold()


def _platform(asset):
    return _PLATFORM_BY_SOURCE.get(str(asset.get("source", "")).casefold())


def _receipt(asset, platform, logical_path, mapping):
    receipt = {
        "id": asset.get("id"),
        "path": asset.get("path"),
        "source": asset.get("source"),
        "platform": platform,
        "logical_path": logical_path,
        "mapping": mapping,
        "mapping_confidence": asset.get("mapping_confidence") or (
            "PROVEN" if mapping == "exact-source-relative-path" else "LIKELY"),
        "sha256": asset.get("sha256"),
        "size": asset.get("size"),
    }
    for key in ("decode_recipe", "decode_confidence", "package_version"):
        if asset.get(key) is not None:
            receipt[key] = asset[key]
    return receipt


def _asset_index(assets):
    index = defaultdict(list)
    seen = set()
    for asset in assets or []:
        platform = _platform(asset)
        if platform is None:
            continue
        aliases = []
        relative = _logical(asset.get("relative"))
        if relative:
            aliases.append((relative, "exact-source-relative-path"))
        for alias in asset.get("logical_paths") or []:
            logical = _logical(alias)
            if logical:
                aliases.append((logical, "package-map-logical-path"))
        for logical, mapping in aliases:
            key = (platform, logical, asset.get("id"), asset.get("path"))
            if key in seen:
                continue
            seen.add(key)
            index[(platform, logical)].append((asset, mapping))
    return index


def _inventory(logical_path, asset_index):
    grouped = {"android": [], "ios": []}
    wanted = _logical(logical_path)
    for platform in grouped:
        for asset, mapping in asset_index.get((platform, wanted), []):
            grouped[platform].append(_receipt(asset, platform, logical_path, mapping))
    for platform in grouped:
        grouped[platform].sort(key=lambda row: (str(row.get("source") or ""),
                                                str(row.get("path") or ""),
                                                str(row.get("id") or "")))
    return grouped


def _distinct_hashes(receipts):
    return {row.get("sha256") for row in receipts if row.get("sha256")}


def _conflicting_receipts(receipts):
    return len(_distinct_hashes(receipts)) > 1


def _platform_summary(grouped):
    present = [platform for platform in ("android", "ios") if grouped[platform]]
    hashes = {platform: _distinct_hashes(grouped[platform]) for platform in present}
    identical = (len(present) == 2 and len(hashes["android"]) == 1
                 and hashes["android"] == hashes["ios"])
    return {
        "present": present,
        "cross_platform_identity": (
            "raw-bytes-identical" if identical else "not-established"),
        "identity_note": (
            "Raw SHA-256 receipts match on both platforms."
            if identical else
            "A shared logical URI establishes file availability only. Raw bytes "
            "differ or decoded iPad bytes were not compared; semantic equivalence "
            "is not asserted."),
    }


def _replace_file_target(ref, target):
    old = ref.get("resolves_to") or []
    retained = [entry for entry in old
                if not (isinstance(entry, dict)
                        and entry.get("type") in {"source asset file", "file"})]
    retained.append(target)
    ref["resolves_to"] = retained


def _finish(ref, logical_path, grouped, rule, evidence, consumer_gate,
            role_confidence, source_rule=None):
    conflicts = [platform for platform in ("android", "ios")
                 if _conflicting_receipts(grouped[platform])]
    platform_info = _platform_summary(grouped)
    receipts = grouped["android"] + grouped["ios"]
    ref["file_resolution"] = {
        "resolver": "file_semantics.v1",
        "logical_path": logical_path,
        "source_rule": source_rule or (ref.get("file_resolution") or {}).get("source_rule") or ref.get("rule"),
        "rule": rule,
        "evidence": evidence,
        "consumer_gate": consumer_gate,
        "reference_role_confidence": role_confidence,
        "platform_availability": platform_info,
        "asset_files": receipts,
        "native_equivalence": "not-asserted",
    }
    ref["rule"] = rule
    ref["confidence"] = role_confidence
    ref["work_status"] = "DONE"
    ref["candidates"] = []
    ref["candidate_note"] = None

    if not receipts:
        ref["status"] = "UNRESOLVED"
        ref["reason"] = "FILE_MISSING"
        ref["reason_code"] = "FILE_MISSING"
        ref["candidate_note"] = (
            "The exact logical path has no asset in either indexed platform "
            "inventory. The Android path construction rule is proven; this "
            "inventory does not establish file availability.")
        ref["candidates"] = []
        ref["resolves_to"] = [entry for entry in (ref.get("resolves_to") or [])
                              if not (isinstance(entry, dict) and entry.get("type") in
                                      {"source asset file", "file"})]
        return
    if conflicts:
        ref["status"] = "AMBIGUOUS"
        ref["reason"] = None
        ref["reason_code"] = None
        ref["candidate_note"] = (
            "Multiple assets within at least one platform map to the exact "
            "logical path with different raw hashes; platform variants across "
            "Android and iPad are retained together.")
        ref["file_resolution"]["diagnostic"] = "CONFLICTING_SAME_PLATFORM_HASHES"
        ref["file_resolution"]["conflicting_platforms"] = conflicts
        ref["candidates"] = receipts
        ref["resolves_to"] = [entry for entry in (ref.get("resolves_to") or [])
                              if not (isinstance(entry, dict) and entry.get("type") in
                                      {"source asset file", "file"})]
        for ordinal, receipt in enumerate(receipts):
            ref["resolves_to"].append({
                "type": "source asset file", "path": logical_path,
                "asset_files": [receipt], "candidate_ordinal": ordinal,
                "raw_sha256": receipt.get("sha256"),
            })
        return

    ref["status"] = "RESOLVED"
    ref["reason"] = None
    ref["reason_code"] = None
    _replace_file_target(ref, {
        "type": "source asset file",
        "path": logical_path,
        "asset_files": receipts,
        "platform_availability": platform_info,
        "native_equivalence": "not-asserted",
    })


def _resolve_ai_script(ref, tables, asset_index):
    context = ref.get("context") or {}
    if (context.get("field") != "Script" or context.get("table") != "ai#group0"
            or ref.get("kind") != "script"):
        return
    stem = ref.get("name")
    if not isinstance(stem, str) or not re.fullmatch(r"[A-Za-z0-9_-]+", stem):
        return
    if stem.startswith("__"):
        return  # builtin script policy is not an external file path
    try:
        index = int(context.get("index"))
    except (TypeError, ValueError):
        return
    matching_rows = [row for row in (tables or [])
                     if row.get("table") == "ai#group0" and row.get("index") == index
                     and row.get("fields", {}).get("Script") == stem]
    if len(matching_rows) != 1:
        return
    logical_path = f"data/scripts/ai/{stem}.luac"
    _finish(ref, logical_path, _inventory(logical_path, asset_index),
            "AI.Script stem -> data/scripts/ai/<stem>.luac",
            _AI_EVIDENCE,
            "Android external-AI selection requires a nonempty non-__ Script; "
            "StepCreateScript selects AISExternal and LuaManager::AddFile must "
            "find the path in its resource provider. Missing files fail the load.",
            "PROVEN")
    ref["reference_kind"] = ref.get("reference_kind") or "script_file"


def _bdae_logical_source(ref):
    context = ref.get("context") or {}
    source = context.get("logical_source_path")
    if not source:
        source = context.get("file") or ref.get("logical_source_path")
    source = str(source or "").replace("\\", "/")
    marker = "/data/"
    if marker in source:
        source = "data/" + source.rsplit(marker, 1)[1]
    return source


def _resolve_bdae_texture(ref, asset_index):
    context = ref.get("context") or {}
    prior = ref.get("file_resolution") or {}
    own_prior = prior.get("resolver") == "file_semantics.v1"
    if (ref.get("kind") != "texture"
            or (ref.get("rule") != "BINARY_FILE_STRING"
                and not (own_prior and prior.get("source_rule") == "BINARY_FILE_STRING"))):
        return
    name = ref.get("name")
    if (not isinstance(name, str) or not name.casefold().endswith(".tga")
            or "/" in name or "\\" in name or name in {".", ".."}):
        return
    source_path = _bdae_logical_source(ref)
    if not source_path.casefold().startswith("data/3d/") or not source_path.casefold().endswith(".bdae"):
        return
    if "byte_offset" not in context or "NUL-terminated file string" not in str(context.get("evidence", "")):
        return
    logical_path = f"data/3d/textures/{name}"
    _finish(ref, logical_path, _inventory(logical_path, asset_index),
            "BDAE material texture basename -> data/3d/textures/<basename>",
            _TEXTURE_EVIDENCE,
            "The original path consumer loads this URI only for a GL_Diffuse or "
            "GL_Xray material that exposes a RimLight parameter; scene-node "
            "overload 0x35a188 iterates materials. BDAE string-to-parameter "
            "association is not established by the raw-string site alone.",
            "LIKELY", source_rule="BINARY_FILE_STRING")
    ref["reference_kind"] = "texture_file"


def resolve(ctx, refs, tables, assets):
    """Return copied refs enriched with exact, source-backed file URI targets.

    The inputs are read-only ledgers. Existing IDs and unrelated semantic
    targets are retained. Any exact receipt in the supplied Android/iPad cache
    union establishes file availability; the Android native consumer proof does
    not imply iPad native equivalence.
    """
    del ctx  # assets/tables carry the input identities needed for this pass
    result = copy.deepcopy(refs or [])
    asset_index = _asset_index(assets)
    for ref in result:
        _resolve_ai_script(ref, tables, asset_index)
        _resolve_bdae_texture(ref, asset_index)
    return result
