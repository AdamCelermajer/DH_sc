"""Resolve authored CharacterTemplate references with source-gated evidence.

The authored ``char_template`` is a template-name key.  The sibling
``char_template_pydata`` is a table selector.  They have different targets and
must never be joined to one another by string equality.
"""
from copy import deepcopy
import xml.etree.ElementTree as ET

import package_index
from common import physical_path


_TABLE = "character_templates#group0"
_CHARACTER_TABLE = "character_properties#group0"
_TEMPLATE_ALIAS = "Charater_Templates"
_CHARACTER_ALIAS = "CharacterTable"
_EVIDENCE = (
    "Original GetCharPropertiesId 0x3b3d38 returns cached properties ID first; "
    "otherwise nonempty char_template takes precedence over charpropsname "
    "(port/level-world/canonical_character_family_v4.cpp:40-47; "
    "canonical_property_declarations_v1.inc:68-69). The source table is "
    "CharacterTemplateTableV78: ordered Name/CharInfo rows and exact name lookup "
    "(port/game-data/character_templates_v78.cpp:19-21,29-34,42-47). "
    "Android actor population admits char_template only with the explicit "
    "Charater_Templates selector and binds its authored name through that "
    "table (port/windows-foundation/actor_population.cpp:61-80)."
)
_ALIAS_EVIDENCE = (
    "The exact authored PyData selector is checked by the population consumer "
    "against Charater_Templates / CharacterTable before accessing the respective "
    "CharacterTemplateTableV78 / CharacterTable (port/windows-foundation/"
    "actor_population.cpp:61-80,144-170; "
    "port/level-world/reference/character-game-design/registered_names.inc). "
    "This resolves the table selector only, not an authored row ID."
)


def _path_key(path):
    return str(path or "").replace("\\", "/")


def _asset_index(assets):
    by_path = {}
    by_id = {}
    by_logical = {}
    for asset in assets or []:
        if asset.get("id"):
            by_id[asset["id"]] = asset
        path = _path_key(asset.get("path"))
        if path:
            by_path.setdefault(path, []).append(asset)
        for logical in asset.get("logical_paths") or []:
            by_logical.setdefault(_path_key(logical), []).append(asset)
    return by_path, by_id, by_logical


def _asset_for(ref, lookup):
    by_path, by_id, by_logical = lookup
    context = ref.get("context") or {}
    asset_id = context.get("source_asset_id") or ref.get("source_asset_id")
    if asset_id:
        return by_id.get(asset_id)
    path = _path_key(context.get("file") or ref.get("source_path"))
    found = by_path.get(path, [])
    if len(found) == 1:
        return found[0]
    logical = _path_key(context.get("logical_source_path") or ref.get("logical_source_path"))
    found = by_logical.get(logical, [])
    return found[0] if logical and len(found) == 1 else None


def _platform(ref, asset):
    explicit = str(ref.get("source_platform") or
                   (ref.get("context") or {}).get("source_platform") or
                   (asset or {}).get("source") or "").lower()
    path = _path_key((ref.get("context") or {}).get("file") or
                     (asset or {}).get("path"))
    if explicit in {"android", "ios", "ipad", "ios-packed", "ios-package", "ios_package"}:
        return "ios" if explicit.startswith("ios") or explicit == "ipad" else "android"
    if "/android/" in path or "/android-extra/" in path:
        return "android"
    if "/ios/" in path or "dungeonhunter2ipad.app/" in path.lower():
        return "ios"
    return None


def _read_source(ctx, asset):
    if str(asset.get("source", "")).lower() in {"ios-packed", "ios-package", "ios_package"} or asset.get("package_relative"):
        return package_index.read_resource(ctx, asset)
    path = asset.get("path")
    if not path:
        raise FileNotFoundError("source asset has no physical path")
    return physical_path(ctx, str(path)).read_bytes()


def _source_properties(ctx, ref, asset, cache):
    """Read the exact GameObject indexed by the XML reference's element_index."""
    context = ref.get("context") or {}
    if isinstance(context.get("source_properties"), dict):
        return context["source_properties"]
    if asset is None or context.get("element", "").casefold() != "gameobject":
        return None
    ordinal = context.get("element_index")
    if not isinstance(ordinal, int) or ordinal < 0:
        return None
    key = (asset.get("id"), asset.get("path"))
    if key not in cache:
        try:
            root = ET.fromstring(_read_source(ctx, asset))
            cache[key] = [dict(node.attrib) if node.tag.rsplit("}", 1)[-1].casefold() == "gameobject" else None
                          for node in root.iter()]
        except (OSError, ValueError, KeyError, IndexError, ET.ParseError):
            cache[key] = None
    elements = cache[key]
    return elements[ordinal] if elements is not None and ordinal < len(elements) else None


def _rows(tables, table, platform):
    """Use Android-decoded ordinals only; other platforms need their own table."""
    if platform != "android":
        return []
    return [row for row in (tables or []) if row.get("table") == table
            and str(row.get("source_file", "")).replace("\\", "/").find("/android/") >= 0]


def _ios_name_groups(ctx, assets, table, cache):
    """Read the iPad names-only stream for an exact logical PyData table path."""
    logical = f"data/pydata/{table}_pyarraynames.bin"
    matches = [asset for asset in assets or []
               if asset.get("source") == "ios-packed"
               and logical in (asset.get("logical_paths") or [])]
    if len(matches) != 1:
        return None
    asset = matches[0]
    key = asset.get("id") or asset.get("path")
    if key not in cache:
        try:
            cache[key] = package_index.read_name_groups(ctx, asset)
        except (OSError, ValueError, KeyError):
            cache[key] = None
    groups = cache[key]
    return (asset, groups) if groups is not None else None


def _unresolved(ref, note, evidence=None):
    ref.update(status="UNRESOLVED", reason="OTHER", reason_code="OTHER",
               work_status="DONE", confidence="PROVEN", resolves_to=[], candidates=[])
    ref["candidate_note"] = note
    for key in ("reference_kind", "referenced_table", "referenced_index", "table_target",
                "template_link_confidence"):
        ref.pop(key, None)
    if evidence:
        ref["schema_evidence"] = evidence


def resolve(ctx, refs, tables, assets):
    """Return copied refs with only source-gated template/table relationships."""
    result = deepcopy(refs or [])
    lookup = _asset_index(assets)
    source_cache = {}
    table_cache = {}
    ios_names_cache = {}
    for ref in result:
        if ref.get("rule") != "XML_ATTRIBUTE":
            continue
        context = ref.get("context") or {}
        attribute = str(context.get("attribute", "")).casefold()
        if attribute not in {"char_template", "char_template_pydata", "charpropsname_pydata"}:
            continue
        asset = _asset_for(ref, lookup)
        platform = _platform(ref, asset)
        props = _source_properties(ctx, ref, asset, source_cache)
        if props is None:
            _unresolved(ref, "Exact source GameObject properties or platform identity are unavailable; no row or table link is inferred.")
            continue
        if props.get("gametype") != "Character":
            _unresolved(ref, "Original lookup gate requires this exact source GameObject to have gametype=Character.", _EVIDENCE)
            continue

        if attribute == "char_template":
            authored_name = props.get("char_template", "")
            selector = props.get("char_template_pydata", "")
            if not authored_name or authored_name != ref.get("name"):
                _unresolved(ref, "The exact source element does not carry this nonempty authored char_template key.", _EVIDENCE)
                continue
            if selector != _TEMPLATE_ALIAS:
                _unresolved(ref, "The authored key is present, but its exact PyData table selector is missing or unsupported.", _EVIDENCE)
                continue
            if platform == "ios":
                names_source = _ios_name_groups(ctx, assets, "character_templates", ios_names_cache)
                names_asset, groups = names_source if names_source else (None, None)
                names = groups[0] if groups and len(groups) == 1 else []
                matches = [i for i, value in enumerate(names) if value == authored_name]
                _unresolved(ref, "The iPad names-only logical group supports a source-name association, but no native template row ID is verified; Android row IDs are not reused.", _EVIDENCE)
                if len(matches) == 1:
                    name_target = {"type": "ios_source_name",
                                   "source_asset_id": names_asset.get("id"),
                                   "source_file": names_asset.get("path"),
                                   "logical_source_path": "data/pydata/character_templates_pyarraynames.bin",
                                   "group": 0, "ordinal": matches[0], "name": authored_name,
                                   "native_id_verified": False}
                    ref.update(status="RESOLVED", reason=None, reason_code=None,
                               confidence="LIKELY", resolves_to=[name_target],
                               reference_kind="ios_source_name",
                               template_link_confidence="LIKELY_NAME_ASSOCIATION_NATIVE_ID_UNVERIFIED")
                elif len(matches) > 1:
                    targets = [{"type": "ios_source_name",
                                "source_asset_id": names_asset.get("id"),
                                "source_file": names_asset.get("path"),
                                "logical_source_path": "data/pydata/character_templates_pyarraynames.bin",
                                "group": 0, "ordinal": i, "name": authored_name,
                                "native_id_verified": False} for i in matches]
                    ref.update(status="AMBIGUOUS", confidence="LIKELY", resolves_to=targets,
                               candidates=targets, reference_kind="ios_source_name",
                               template_link_confidence="LIKELY_NAME_ASSOCIATION_NATIVE_ID_UNVERIFIED")
                continue
            if platform != "android":
                _unresolved(ref, "Source platform is unknown; the iPad names-only namespace cannot be assumed.", _EVIDENCE)
                continue
            cache_key = (_TABLE, "android")
            if cache_key not in table_cache:
                table_cache[cache_key] = _rows(tables, _TABLE, "android")
            matches = [row for row in table_cache[cache_key] if row.get("name") == authored_name]
            ref["schema_evidence"] = _EVIDENCE
            ref["reference_kind"] = "table_row"
            ref["referenced_table"] = _TABLE
            ref["rule"] = 'gametype="Character" char_template exact name in Android CharacterTemplateTableV78, gated by Charater_Templates selector.'
            if len(matches) == 1:
                row = matches[0]
                ref["resolves_to"] = [{"type": "table-row", "table": _TABLE,
                                       "index": row["index"], "name": row["name"],
                                       "schema_evidence": _EVIDENCE}]
                ref.update(status="RESOLVED", reason=None, reason_code=None,
                           work_status="DONE", confidence="PROVEN", candidates=[])
                ref.pop("candidate_note", None)
            elif len(matches) > 1:
                ref.update(status="AMBIGUOUS", reason=None, reason_code=None,
                           work_status="DONE", confidence="PROVEN")
                candidate_targets = [{"type": "table-row", "table": _TABLE,
                                      "index": r.get("index"), "name": r.get("name"),
                                      "schema_evidence": _EVIDENCE} for r in matches]
                ref["resolves_to"] = candidate_targets
                ref["candidates"] = candidate_targets
            else:
                _unresolved(ref, f"No exact authored row name {authored_name!r} exists in the Android CharacterTemplateTableV78 names stream.", _EVIDENCE)
            continue

        if attribute == "char_template_pydata":
            table_name = props.get("char_template_pydata", "")
            if not props.get("char_template") or table_name != ref.get("name"):
                _unresolved(ref, "This selector is associated only with a Character that has a nonempty authored char_template and an exact matching source value.", _ALIAS_EVIDENCE)
                continue
            if table_name != _TEMPLATE_ALIAS:
                _unresolved(ref, f"No recovered table mapping admits PyData alias {table_name!r}.", _ALIAS_EVIDENCE)
                continue
            if platform == "ios":
                names_source = _ios_name_groups(ctx, assets, "character_templates", ios_names_cache)
                names_asset, groups = names_source if names_source else (None, None)
                _unresolved(ref, "The iPad source has a decoded names-only CharacterTemplate group; the source table association is likely while its native ID namespace remains unverified.", _ALIAS_EVIDENCE)
                if groups and len(groups) == 1 and groups[0]:
                    table_target = {"type": "ios_table_source",
                                    "source_asset_id": names_asset.get("id"),
                                    "source_file": names_asset.get("path"),
                                    "logical_source_path": "data/pydata/character_templates_pyarraynames.bin",
                                    "group": 0, "name": table_name,
                                    "native_id_verified": False,
                                    "schema_evidence": _ALIAS_EVIDENCE}
                    ref.update(status="RESOLVED", reason=None, reason_code=None,
                               confidence="LIKELY", resolves_to=[table_target],
                               reference_kind="ios_table_source",
                               template_link_confidence="LIKELY_NAME_ASSOCIATION_NATIVE_ID_UNVERIFIED")
                continue
            if platform != "android":
                _unresolved(ref, "Source platform is unknown; the iPad names-only namespace cannot be assumed.", _ALIAS_EVIDENCE)
                continue
            table_target = {"type": "table", "table": _TABLE, "name": table_name,
                            "schema_evidence": _ALIAS_EVIDENCE}
            ref.update(status="RESOLVED", reason=None, reason_code=None,
                       work_status="DONE", confidence="PROVEN", resolves_to=[table_target], candidates=[],
                       reference_kind="pydata_table", referenced_table=_TABLE,
                       rule="Exact char_template_pydata selector Charater_Templates identifies the registered CharacterTemplateTableV78 source table; it is not a template row key.",
                       schema_evidence=_ALIAS_EVIDENCE)
            ref.pop("candidate_note", None)
            continue

        # charpropsname_pydata is a table selector, independent of the row key.
        if props.get("char_template"):
            _unresolved(ref, "char_template takes precedence over CharacterTable name lookup; the competing charpropsname table selector is not treated as an active source edge.", _ALIAS_EVIDENCE)
            continue
        selector = props.get("charpropsname_pydata", "")
        if not props.get("charpropsname") or selector != ref.get("name") or selector != _CHARACTER_ALIAS:
            _unresolved(ref, "The exact source element does not satisfy the CharacterTable selector and charpropsname admission gates.", _ALIAS_EVIDENCE)
            continue
        if platform == "ios":
            names_source = _ios_name_groups(ctx, assets, "character_properties", ios_names_cache)
            names_asset, groups = names_source if names_source else (None, None)
            _unresolved(ref, "The iPad source has a decoded names-only CharacterTable group; the source table association is likely while its native ID namespace remains unverified.", _ALIAS_EVIDENCE)
            if groups and groups[0]:
                table_target = {"type": "ios_table_source",
                                "source_asset_id": names_asset.get("id"),
                                "source_file": names_asset.get("path"),
                                "logical_source_path": "data/pydata/character_properties_pyarraynames.bin",
                                "group": 0, "name": selector,
                                "native_id_verified": False,
                                "schema_evidence": _ALIAS_EVIDENCE}
                ref.update(status="RESOLVED", reason=None, reason_code=None,
                           confidence="LIKELY", resolves_to=[table_target],
                           reference_kind="ios_table_source",
                           template_link_confidence="LIKELY_NAME_ASSOCIATION_NATIVE_ID_UNVERIFIED")
            continue
        elif platform != "android":
            _unresolved(ref, "Source platform is unknown; the iPad names-only namespace cannot be assumed.", _ALIAS_EVIDENCE)
            continue
        table_target = {"type": "table", "table": _CHARACTER_TABLE,
                        "name": selector, "schema_evidence": _ALIAS_EVIDENCE}
        ref.update(status="RESOLVED", reason=None, reason_code=None,
                   work_status="DONE", confidence="PROVEN", resolves_to=[table_target], candidates=[],
                   reference_kind="pydata_table", referenced_table=_CHARACTER_TABLE,
                   rule="Exact charpropsname_pydata selector CharacterTable identifies the CharacterTable source table; the authored charpropsname remains a separate row key.",
                   schema_evidence=_ALIAS_EVIDENCE)
        ref.pop("candidate_note", None)
    return result
