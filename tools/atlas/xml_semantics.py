"""Resolve XML-local template/prototype names without cross-source guesses.

The resolver treats a declaration as an XML element explicitly named Template
or Prototype and carrying a name attribute. References are matched only within
the same physical source resource. Package-backed iPad XML is read through the
validated package_index decoder.
"""
from collections import Counter, defaultdict
from copy import deepcopy
import re
import xml.etree.ElementTree as ET

import package_index
from common import physical_path, stable_id


_DECLARATIONS = {
    "template", "objecttemplate", "charactertemplate", "gametemplate",
    "prototype", "objectprototype", "characterprototype",
}
_REFERENCE_ATTRIBUTES = {
    "_templatename", "template", "prototype", "prototypename",
    "object_template", "objecttemplate", "prototype_name",
}
_SAFE_SELECTOR = re.compile(r"^[A-Za-z_][A-Za-z0-9_.-]*$")
_TEMPLATE_SELECTOR_EVIDENCE = (
    "Recovered original PropertyMap::SetTemplate 0x513fec stores the class-map "
    "template name; LoadTemplate 0x51419c is only assertion9, with no external "
    "template XML parser recovered (port/level-world/reference/canonical-object-factory-v1/renderer-integration.md; "
    "original-functions.json entries 0x513fec and 0x51419c). The native caller "
    "chain is ObjectManager InitProperties -> optional SetTemplate -> "
    "LoadDefaultProperties -> LoadOverrides (port/level-world/reference/actor-initialization/NOTES.md). "
    "Port source details: canonical_property_map_v1.cpp:59,134-150; "
    "canonical_object_factory_v1.cpp:12,29-30."
)
_XML_DECLARATION_EVIDENCE = (
    "Exact XML declaration and reference occur in the same validated physical "
    "source resource; this establishes source identity only, not runtime use."
)


def _asset_lookup(assets):
    by_id = {}
    by_path = defaultdict(list)
    by_logical = defaultdict(list)
    for asset in assets:
        if asset.get("id"):
            by_id[asset["id"]] = asset
        path = str(asset.get("path", "")).replace("\\", "/")
        if path:
            by_path[path].append(asset)
        if asset.get("source") == "ios-packed":
            for logical in asset.get("logical_paths") or []:
                by_logical[str(logical).replace("\\", "/")].append(asset)
    return by_id, by_path, by_logical


def _source_asset(ref, lookup):
    by_id, by_path, by_logical = lookup
    context = ref.get("context") or {}
    asset_id = context.get("source_asset_id") or ref.get("source_asset_id")
    if asset_id:
        return by_id.get(asset_id)
        return None
    source_path = context.get("source_path") or context.get("file") or ref.get("source_path")
    if source_path:
        source_path = str(source_path).replace("\\", "/")
        matches = by_path.get(source_path, [])
        if len(matches) == 1:
            return matches[0]
    logical = context.get("logical_source_path") or ref.get("logical_source_path")
    if logical:
        logical = str(logical).replace("\\", "/")
        matches = by_logical.get(logical, [])
        if len(matches) == 1:
            return matches[0]
    return None


def _read_source(ctx, asset):
    """Read an indexed XML resource; package resources keep decoder validation."""
    source = str(asset.get("source", "")).lower()
    if source in {"ios-packed", "ios-package", "ios_package"} or asset.get("package_relative"):
        return package_index.read_resource(ctx, asset)
    path = asset.get("path")
    if not path:
        raise FileNotFoundError("asset has no physical source path")
    return physical_path(ctx, str(path)).read_bytes()


def _logical_source(ref, asset):
    context = ref.get("context") or {}
    logical = context.get("logical_source_path") or ref.get("logical_source_path")
    if logical:
        return str(logical).replace("\\", "/")
    paths = asset.get("logical_paths") or []
    if len(paths) == 1:
        return str(paths[0]).replace("\\", "/")
    return str(asset.get("path") or context.get("file") or ref.get("site") or "").replace("\\", "/")


def _parse_definitions(raw, logical_path, asset_id, platform):
    try:
        root = ET.fromstring(raw)
    except (ET.ParseError, UnicodeError, ValueError):
        return None
    result = []
    for ordinal, element in enumerate(root.iter()):
        if element.tag.rsplit("}", 1)[-1].casefold() not in _DECLARATIONS:
            continue
        name = element.attrib.get("name")
        if not isinstance(name, str) or not name:
            continue
        target_id = stable_id("xml-template", logical_path, asset_id, ordinal)
        result.append({
            "id": target_id,
            "type": "xml_template",
            "name": name,
            "source_file": logical_path,
            "source_asset_id": asset_id,
            "source_platform": platform,
            "element_index": ordinal,
            "definition_tag": element.tag.rsplit("}", 1)[-1],
        })
    return result


def _definitions_for(ctx, ref, lookup, cache):
    asset = _source_asset(ref, lookup)
    if asset is None:
        return None, None
    asset_id = asset.get("id")
    if not asset_id:
        return None, None
    logical = _logical_source(ref, asset)
    cache_key = (str(asset_id), logical)
    if cache_key not in cache:
        try:
            raw = _read_source(ctx, asset)
            definitions = _parse_definitions(raw, logical, asset_id,
                                             ref.get("source_platform") or asset.get("source"))
            cache[cache_key] = ((definitions, None) if definitions is not None
                                else (None, "source XML is truncated or malformed"))
        except (OSError, ValueError, KeyError) as exc:
            cache[cache_key] = (None, str(exc))
    return cache[cache_key]


def _append_target(ref, target, evidence):
    ref["resolves_to"] = [target]
    ref["status"] = "RESOLVED"
    ref["reason"] = None
    ref["reason_code"] = None
    ref["work_status"] = "DONE"
    ref["confidence"] = "PROVEN"
    ref["reference_kind"] = "xml_template"
    ref["schema_evidence"] = evidence
    ref["candidates"] = []
    ref.pop("candidate_note", None)


def resolve(ctx, refs, tables, assets):
    """Return copied refs annotated with exact same-resource XML declarations.

    Existing IDs and unrelated edges remain intact. Character property
    resolution stays owned by references._resolve_charpropsname_sites, which
    applies the native gametype and char_template precedence gates.
    """
    del tables  # XML-local declarations do not select native numeric table IDs.
    result = deepcopy(refs or [])
    assets = list(assets or [])
    lookup = _asset_lookup(assets)
    cache = {}
    for ref in result:
        if ref.get("rule") != "XML_ATTRIBUTE":
            continue
        context = ref.get("context") or {}
        attribute = str(context.get("attribute", "")).casefold()
        # This native conditional table relation has its own admission gates.
        if attribute in {"charpropsname", "char_template", "char_template_pydata"}:
            continue
        if attribute not in _REFERENCE_ATTRIBUTES:
            continue
        name = ref.get("name")
        if not isinstance(name, str) or not name or not _SAFE_SELECTOR.fullmatch(name):
            continue
        source_asset = _source_asset(ref, lookup)
        source_platform = (ref.get("source_platform") or context.get("source_platform")
                           or ((source_asset or {}).get("source")))
        # The native Android consumer stores this value as a property-map
        # selector. It does not look up an XML declaration by this name.
        # Handle this role before parsing so a coincident local element cannot
        # turn the selector into a false resource edge.
        if (attribute == "_templatename"
                and str(context.get("element", "")).casefold() == "gameobject"
                and source_platform == "android"):
            ref.update(status="NOT-NEEDED", reason="OTHER", reason_code="OTHER",
                       work_status="DONE", confidence="PROVEN", resolves_to=[],
                       reference_kind="object_template_selector", candidates=[])
            ref["rule"] = "Android GameObject._templateName is an internal property-map selector, not a file or actor reference."
            ref["schema_evidence"] = _TEMPLATE_SELECTOR_EVIDENCE
            ref["candidate_note"] = (
                "This label is not a file or actor reference. External inheritance remains unsupported/unverified; "
                "no same-name XML declaration is interpreted as a lookup target.")
            ref["xml_semantics"] = {
                "resolver": "xml_semantics.v1",
                "scope": "original Android PropertyMap consumer",
                "role": "internal class property-map selector",
                "external_inheritance": "unsupported/unverified",
                "schema_evidence": _TEMPLATE_SELECTOR_EVIDENCE,
            }
            continue
        definitions, read_error = _definitions_for(ctx, ref, lookup, cache)
        if definitions is None:
            if read_error:
                ref["xml_semantics"] = {"source_read_error": read_error}
            continue
        matches = [item for item in definitions if item["name"] == name]
        ref["xml_semantics"] = {
            "resolver": "xml_semantics.v1",
            "scope": "same physical source resource",
            "definition_count": len(definitions),
            "match_count": len(matches),
        }
        if len(matches) == 1:
            _append_target(ref, matches[0], _XML_DECLARATION_EVIDENCE)
            ref["rule"] = "XML_ATTRIBUTE exact same-resource named template/prototype declaration"
            continue
        if len(matches) > 1:
            ref.update(status="AMBIGUOUS", reason=None, reason_code=None,
                       work_status="DONE", confidence="PROVEN", resolves_to=matches,
                       reference_kind="xml_template",
                       candidates=matches,
                       candidate_note="Multiple exact named declarations exist in this physical source resource.")
            ref["schema_evidence"] = _XML_DECLARATION_EVIDENCE
            continue
    return result


def summarize(refs):
    """Compact report counts for the XML resolver's observable outcomes."""
    selected = [r for r in (refs or []) if (r.get("xml_semantics") or {}).get("resolver") == "xml_semantics.v1"
                or r.get("reference_kind") == "object_template_selector"]
    return {
        "annotated_references": len(selected),
        "by_status": dict(Counter(r.get("status") for r in selected)),
        "by_attribute": dict(Counter(str((r.get("context") or {}).get("attribute", ""))
                                     for r in selected)),
        "unique_xml_template_targets": len({t.get("id") for r in selected
                                             for t in r.get("resolves_to", [])
                                             if t.get("type") == "xml_template"}),
    }
