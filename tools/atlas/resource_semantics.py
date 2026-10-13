"""Resolve source-backed animation and named-sound resource semantics.

This pass preserves reference IDs and only emits native event/source sound
targets when the original handler and its admission rule are known. Marker
names that lack an owner remain explicitly unresolved.
"""
from __future__ import annotations

from collections import Counter, defaultdict
from copy import deepcopy
import json
from pathlib import Path
import xml.etree.ElementTree as ET

from common import stable_id


_EVENT_EVIDENCE = (
    "Android IDA CharAI::_OnAnimEvent at 0x3d4434; recovered source note "
    "port/level-world/reference/character-melee-animation-event-v1/NOTES.md; "
    "port/level-world/character_melee_animation_event_v1.cpp. The caller "
    "queries animation step index/count before prefix or state dispatch."
)
_SKILL_STATE_EVIDENCE = (
    "port/level-world/character_skill_state_v4.cpp:38-45: query current "
    "state after ev_/an_/fx_/sfx_ prefix dispatch; do_skill reaches skill Use "
    "only in state 6 after isTracingCharAI_AnimEvent. State 5 has the separate "
    "CanRangeAttack gate and admits do_skill only on the ranged-projectile path."
)
_SPELL_EVIDENCE = (
    "Android CharAI::_OnAnimEvent at 0x3d4434 admits do_spell only in state 7 "
    "after isTracingCharAI_AnimEvent; port/windows-foundation/features/"
    "faery_menu/source_faery_ability_binding_v1.cpp:258-286 verifies the "
    "same state-7 gate before Player Spell Use."
)
_ATTACK_EVIDENCE = (
    "Android CharAI::_OnAnimEvent at 0x3d4434 queries CanRangeAttack in state 5. "
    "If true, only attack_ranged/attack_mainhand/do_skill reach projectile "
    "spawn; if false, attack_mainhand/offhand call the selected AIS OnAttack "
    "virtual at index-1. Evidence: port/level-world/reference/"
    "character-melee-animation-event-v1/NOTES.md and "
    "port/level-world/character_melee_animation_event_v1.cpp:31-44."
)
_INTERACT_EVIDENCE = (
    "Android CharAI::_OnAnimEvent at 0x3d4434 admits interact only in state 13, "
    "then checks isTracingCharAI_AnimEvent before its native interaction "
    "operation; port/level-world/character_melee_animation_event_v1.cpp:40-44."
)
_PREFIX_EVIDENCE = (
    "Android CharAI::_OnAnimEvent at 0x3d4434 dispatches ev_ to virtual 0x94, "
    "an_ to object animation, and fx_ to the exact ordered mesh-FX set lookup; "
    "port/level-world/character_melee_animation_event_v1.cpp:24-31."
)
_SOUND_EVIDENCE = (
    "Android IDA CharAI::_OnAnimEvent at 0x3d4434 handles exact sfx_ prefix; "
    "CharAI named-sound leaf 0x3d4434..0x3d4598 queries animation step/count, "
    "strips exactly four bytes and case-sensitively scans the 638-name "
    "Arrays::Sounds table. Evidence: port/engine-audio/reference/"
    "event-families-v38/handoff.md:31,44,48; "
    "port/engine-audio/audio_source_bindings_v38.cpp; "
    "port/windows-foundation/features/audio/canonical_source_audio_adapter.cpp:17-23."
)
_SDD_EVIDENCE = (
    "port/engine-audio/reference/source-bindings-v38/NOTES.md: Arrays::Sounds "
    "registers the 638-row sdd_dungeon_hunter_2_iphone records/name streams; "
    "each row's ordinal is source_id and its UID/event pair joins sounds.xml "
    "through port/engine-audio/reference/source-bindings-v38/ledger.json."
)

_BARE_EVENTS = {
    "do_skill": {
        "source_handler": "CharAI::_OnAnimEvent -> SkillState/AI Use",
        "address": "0x3d4434",
        "role": "state-gated skill or ranged-projectile marker",
        "conditions": "state 6 + isTracingCharAI_AnimEvent reaches skill Use; state 5 first queries CanRangeAttack and the true branch admits do_skill only for the ranged projectile path",
        "evidence": _SKILL_STATE_EVIDENCE,
    },
    "do_spell": {
        "source_handler": "CharAI::_OnAnimEvent -> Player Spell Use",
        "address": "0x3d4434",
        "role": "state-7 spell-use marker",
        "conditions": "current state must be 7 and isTracingCharAI_AnimEvent must pass; the player spell owner must be source-ready",
        "evidence": _SPELL_EVIDENCE,
    },
    "attack_mainhand": {
        "source_handler": "CharAI::_OnAnimEvent -> AIS OnAttack or ranged projectile",
        "address": "0x3d4434",
        "role": "state-5 attack marker",
        "conditions": "current state 5; CanRangeAttack selects projectile for accepted ranged/mainhand/do_skill names, otherwise OnAttack virtual 0xa8 uses step index minus one",
        "evidence": _ATTACK_EVIDENCE,
    },
    "attack_offhand": {
        "source_handler": "CharAI::_OnAnimEvent -> AIS OnAttack(offhand)",
        "address": "0x3d4434",
        "role": "state-5 offhand attack marker",
        "conditions": "current state 5 and CanRangeAttack false; OnAttack virtual 0xa8 receives offhand=true and step index minus one",
        "evidence": _ATTACK_EVIDENCE,
    },
    "attack_ranged": {
        "source_handler": "CharAI::_OnAnimEvent -> ranged projectile",
        "address": "0x3d4434",
        "role": "state-5 ranged projectile marker",
        "conditions": "current state 5 and CanRangeAttack true; projectile spawn uses the selected projectile ID",
        "evidence": _ATTACK_EVIDENCE,
    },
    "interact": {
        "source_handler": "CharAI::_OnAnimEvent -> AIS interaction operation",
        "address": "0x3d4434",
        "role": "state-13 interaction marker",
        "conditions": "current state 13 and isTracingCharAI_AnimEvent passes",
        "evidence": _INTERACT_EVIDENCE,
    },
}

_PREFIXES = {
    "ev_": ("CharAI::_OnAnimEvent -> AIS event virtual 0x94", "character_ai_event", _PREFIX_EVIDENCE),
    "an_": ("CharAI::_OnAnimEvent -> object animation lookup", "object_animation_trigger", _PREFIX_EVIDENCE),
    "fx_": ("CharAI::_OnAnimEvent -> exact CharacterMeshFx set lookup", "mesh_fx_trigger", _PREFIX_EVIDENCE),
}
_XML_SOUND_ATTRIBUTES = {"sound", "music", "ambient_music", "ambiant_music", "idle_sound"}
_AUDIO_SOURCES = {"android", "android-extra", "original-cache", "port-runtime-original-cache", "ios", "ios-packed", "ios-glive", "audio-cache"}
_PLATFORM_BY_SOURCE = {
    "android": "android", "android-extra": "android", "original-cache": "android",
    "port-runtime-original-cache": "android", "audio-cache": "android",
    "ios": "ios", "ios-packed": "ios", "ios-glive": "ios",
}


def _norm(value):
    return str(value or "").replace("\\", "/").lstrip("./")


def _source_platform(ref, context):
    platform = str(ref.get("source_platform") or context.get("source_platform") or "").casefold()
    if platform:
        return platform
    source_file = _norm(context.get("file") or context.get("source_path") or ref.get("site"))
    folded = source_file.casefold()
    if "/ios/" in folded or "claude-preview15/ipa/" in folded:
        return "ios"
    if "/android/" in folded or ".local-inputs/assets-extra/android/" in folded:
        return "android"
    return "unknown"


def _event_target(name, spec, platform):
    target = {
        "type": "native_event",
        "id": stable_id("native-event", name, spec["source_handler"], platform),
        "name": name,
        "source_platform": platform,
        "source_handler": spec["source_handler"],
        "address": spec.get("address"),
        "role": spec["role"],
        "conditions": spec["conditions"],
        "evidence": spec["evidence"],
        "native_evidence_platform": "android",
        "native_equivalence": "not-asserted",
    }
    if platform == "ios":
        target["platform_association"] = (
            "Android-handler inference attached to an iOS source event string; "
            "the iOS string does not establish Android native-ID equivalence."
        )
    elif platform != "android":
        target["platform_association"] = (
            "Android native-handler evidence associated by exact source marker; "
            "the source platform is unknown, so native-ID equivalence is not established."
        )
    return target


def _source_sound_target(row, members, grouped_files, missing, event_name=None, platform="android"):
    name = row.get("name")
    source_id = row.get("index")
    target = {
        "type": "source_sound",
        "id": stable_id("source-sound", source_id, name, platform),
        "name": name,
        "source_platform": platform,
        "table": row.get("table"),
        "source_id": source_id,
        "uid": (row.get("fields") or {}).get("uid"),
        "is_event": bool((row.get("fields") or {}).get("isEvent")),
        "evidence": _SDD_EVIDENCE,
        "native_equivalence": "not-asserted",
        "members": members,
        "physical_uri_prefix": "data/sounds/",
        "asset_files_by_platform": grouped_files,
        "missing_filenames": missing,
    }
    if event_name is not None:
        target["original_event"] = event_name
        target["source_handler"] = "CharAI::_OnAnimEvent named-sound leaf"
        target["source_handler_address"] = "0x3d4434"
        target["handler_evidence"] = _SOUND_EVIDENCE
        target["native_evidence_platform"] = "android"
        if platform == "ios":
            target["platform_association"] = (
                "Android named-sound handler inference attached to an iOS source event string; "
                "the iOS string does not establish Android native-ID equivalence."
            )
        elif platform != "android":
            target["platform_association"] = (
                "Android named-sound handler inference attached by exact source event string; "
                "the source platform is unknown, so native-ID equivalence is not established."
            )
    elif platform == "ios":
        target["platform_association"] = (
            "iOS authored sound label joined by exact source text to the Android SDD row; "
            "the text does not establish iOS native ID equivalence."
        )
    elif platform != "android":
        target["platform_association"] = (
            "Source sound label joined by exact text to the Android SDD row; "
            "the source platform is unknown, so native-ID equivalence is not established."
        )
    return target


def _index_assets(assets):
    exact = defaultdict(list)
    folded = defaultdict(list)
    for asset in assets or []:
        source = str(asset.get("source", "")).casefold()
        platform = _PLATFORM_BY_SOURCE.get(source)
        if platform is None or source not in _AUDIO_SOURCES:
            continue
        values = [asset.get("relative", ""), *(asset.get("logical_paths") or [])]
        for value in values:
            rel = _norm(value)
            if not rel:
                continue
            if rel.startswith("data/"):
                logical = rel
            elif rel.startswith("sounds/"):
                logical = "data/" + rel
            elif source == "audio-cache" and "/" not in rel:
                # The owner's audio-v34 cache is the exact supplemental
                # Android sound corpus. Its catalog entries are flat, so the
                # catalog filename itself supplies the data/sounds role.
                logical = "data/sounds/" + rel
            else:
                continue
            key = (platform, logical)
            if asset not in exact[key]:
                exact[key].append(asset)
            folded[(platform, logical.casefold())].append(asset)
    return exact, folded


def _receipt(asset, platform, uri):
    return {key: asset.get(key) for key in
            ("id", "path", "relative", "source", "sha256", "size", "logical_paths",
             "mapping_confidence", "package_version") if asset.get(key) is not None} | {
                 "platform": platform, "logical_uri": uri}


def _catalog_rows(ctx):
    path = Path(ctx.inputs) / "audio-v34/cache/sounds.xml"
    if not path.is_file():
        return None
    try:
        root = ET.parse(path).getroot()
    except (OSError, ET.ParseError, UnicodeError):
        return None
    by_uid = defaultdict(list)
    for node in root.iter("sound"):
        try:
            uid = int(node.attrib.get("uid", "-1"))
        except ValueError:
            continue
        by_uid[uid].append(dict(node.attrib))
    return by_uid


def _read_audio_ledger(ctx):
    path = Path(ctx.root) / "port/engine-audio/reference/source-bindings-v38/ledger.json"
    try:
        rows = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}
    return {row.get("source_id"): row for row in rows if isinstance(row.get("source_id"), int)}


def _sound_members(row, catalog, ledger):
    fields = row.get("fields") or {}
    source_id = row.get("index")
    entry = ledger.get(source_id)
    if entry:
        members = []
        for item in (entry.get("assets") or []):
            try:
                member_uid = int(item.get("uid", -1))
            except (TypeError, ValueError):
                member_uid = -1
            xml_members = catalog.get(member_uid, []) if catalog is not None else []
            members.append({
                "uid": member_uid if member_uid >= 0 else item.get("uid"),
                "filename": item.get("filename"),
                "catalog_uid": entry.get("uid"),
                "source_status": entry.get("status"),
                "xml_catalog_rows": [{key: node.get(key) for key in
                    ("uid", "label", "filename", "format", "loop", "group", "bank", "priority")
                    if node.get(key) is not None} for node in xml_members],
            })
        return members
    try:
        uid = int(fields.get("uid", -1))
    except (TypeError, ValueError):
        return []
    xml_rows = catalog.get(uid, []) if catalog is not None else []
    return [{"uid": uid, "filename": item.get("filename"), "catalog_uid": uid,
             "catalog_label": item.get("label"), "source_status": "catalog-row"}
            for item in xml_rows]


def _sound_files(members, asset_index):
    exact, folded = asset_index
    grouped = {"android": [], "ios": []}
    missing = []
    case_mismatches = []
    for member in members:
        filename = member.get("filename")
        if not isinstance(filename, str) or not filename:
            continue
        uri = "data/sounds/" + filename
        found = False
        key = uri
        for platform in grouped:
            assets = exact.get((platform, key), [])
            if assets:
                found = True
                grouped[platform].extend(_receipt(asset, platform, uri) for asset in assets)
            else:
                folded_matches = folded.get((platform, key.casefold()), [])
                if folded_matches:
                    case_mismatches.extend(_receipt(asset, platform, uri) for asset in folded_matches)
        if not found:
            missing.append(filename)
    for platform in grouped:
        unique = {(item.get("id"), item.get("path"), item.get("logical_uri")): item
                  for item in grouped[platform]}
        grouped[platform] = [unique[key] for key in sorted(unique, key=lambda x: tuple(str(i or "") for i in x))]
    return grouped, sorted(set(missing)), case_mismatches


def _set_resolution(ref, targets, status, reason=None, role=None, detail=None, confidence="PROVEN"):
    old = ref.get("resolves_to") or []
    retained = [item for item in old if not (isinstance(item, dict) and item.get("type") in
                {"native_event", "source_sound", "sounds.xml label", "sounds.xml filename", "source asset file", "file"})]
    ref["resolves_to"] = retained + targets
    ref["status"] = status
    valid_reasons = {"FILE_MISSING", "NAME_NO_FILE", "PATH_CASE", "ID_OUT_OF_RANGE", "NOT_IN_CACHE", "OTHER"}
    ref["reason"] = reason if reason in valid_reasons else ("OTHER" if status == "UNRESOLVED" and reason else None)
    ref["reason_code"] = ref["reason"]
    ref["work_status"] = "DONE"
    ref["confidence"] = confidence
    ref["reference_kind"] = role
    ref["resource_semantics"] = {
        "resolver": "resource_semantics.v1",
        "role": role,
        "detail": detail,
        "reason_code": reason,
        "reason": detail if status == "UNRESOLVED" else None,
        "source_evidence": targets[0].get("evidence") if targets else None,
    }
    ref["candidates"] = []
    ref.pop("candidate_note", None)


def resolve(ctx, refs, tables, assets):
    """Annotate copied reference rows with evidenced native event/sound roles.

    This is safe to run repeatedly: the existing reference IDs and unrelated
    targets survive, while this resolver's targets and statuses are rebuilt
    from the supplied source tables and asset inventory.
    """
    result = deepcopy(refs or [])
    table_by_name = defaultdict(list)
    for row in tables or []:
        if row.get("table") == "sdd_dungeon_hunter_2_iphone":
            table_by_name[row.get("name")].append(row)
    catalog = _catalog_rows(ctx)
    ledger = _read_audio_ledger(ctx)
    asset_index = _index_assets(assets)

    for ref in result:
        rule = ref.get("rule")
        name = ref.get("name")
        context = ref.get("context") or {}
        if rule == "BRES_EVENT_CALLBACK":
            platform = _source_platform(ref, context)
            event_name = context.get("original_event") or name
            spec = _BARE_EVENTS.get(event_name)
            if spec is None and isinstance(event_name, str):
                for prefix, (handler, role, evidence) in _PREFIXES.items():
                    if event_name.startswith(prefix) and len(event_name) > len(prefix):
                        spec = {
                            "source_handler": handler, "address": "0x3d4434", "role": role,
                            "conditions": "The original event dispatcher matches this exact case-sensitive prefix after obtaining step index and count; the suffix is passed unchanged to the named owner lookup/callback.",
                            "evidence": evidence,
                        }
                        break
            if spec:
                target = _event_target(event_name, spec, platform)
                _set_resolution(ref, [target], "RESOLVED", role="animation_event",
                                detail="native callback marker; not a file URI",
                                confidence="PROVEN" if platform == "android" else "LIKELY")
            else:
                # The BRES event track proves this is an event-name payload,
                # but no global handler/owner gate has been established.
                ref["resource_semantics"] = {
                    "resolver": "resource_semantics.v1", "role": "unresolved_event_marker",
                    "original_event": event_name,
                    "reason_code": "UNKNOWN_EVENT_MARKER",
                    "reason": "No proven native handler and owner/admission gate for this exact marker in the available source handoff.",
                    "evidence": "port/engine-animation/event_track.cpp: load reads root+0x2c event record, bounded time keys and grouped NUL-terminated names; this proves serialized event-track structure, not the callback owner.",
                }
                ref["status"] = "UNRESOLVED"
                ref["reason"] = "OTHER"
                ref["reason_code"] = "OTHER"
                ref["work_status"] = "DONE"
                ref["candidates"] = []
                ref["candidate_note"] = ref["resource_semantics"]["reason"]
                ref["resolves_to"] = [item for item in (ref.get("resolves_to") or [])
                                       if not (isinstance(item, dict) and item.get("type") == "native_event")]
            continue

        is_bres_sound = rule == "BRES_EVENT_SFX_PREFIX"
        is_xml_label = (rule == "XML_ATTRIBUTE" and ref.get("kind") == "sound"
                        and str(context.get("attribute", "")).casefold() in _XML_SOUND_ATTRIBUTES)
        if not (is_bres_sound or is_xml_label):
            continue
        if is_bres_sound:
            original_event = context.get("original_event")
            if not isinstance(original_event, str) or not original_event.startswith("sfx_"):
                ref["resource_semantics"] = {"resolver": "resource_semantics.v1", "role": "unresolved_named_sound_marker",
                    "reason_code": "UNKNOWN_SOUND_MARKER",
                    "reason": "BRES sound rule lacks the exact authored sfx_ event string; no prefix repair was guessed."}
                ref["status"] = "UNRESOLVED"; ref["reason"] = "OTHER"; ref["reason_code"] = "OTHER"; ref["work_status"] = "DONE"
                continue
            label = original_event[4:]
            platform = _source_platform(ref, context)
        else:
            original_event = None
            label = name
            platform = _source_platform(ref, context)
        matches = table_by_name.get(label, []) if isinstance(label, str) else []
        if len(matches) != 1:
            ref["resource_semantics"] = {
                "resolver": "resource_semantics.v1", "role": "unresolved_sound_label",
                "catalog_label": label,
                "reason_code": "UNKNOWN_SOUND_LABEL" if not matches else "AMBIGUOUS_SOUND_LABEL",
                "reason": ("No exact case-sensitive source SDD row matches this label. Prefix or suffix edits are not inferred." if not matches else
                           "More than one exact source SDD row has this label."),
                "match_count": len(matches),
            }
            ref["status"] = "UNRESOLVED" if not matches else "AMBIGUOUS"
            ref["reason"] = "OTHER" if not matches else None
            ref["reason_code"] = ref["reason"]
            ref["work_status"] = "DONE"
            ref["candidate_note"] = ref["resource_semantics"]["reason"]
            ref["candidates"] = [{"id": row.get("id"), "table": row.get("table"), "index": row.get("index"), "name": row.get("name")}
                                 for row in matches]
            if matches:
                ref["resolves_to"] = [{"type": "table-row", "id": row.get("id"),
                    "table": row.get("table"), "index": row.get("index"),
                    "name": row.get("name"), "row_id": row.get("id")} for row in matches]
            continue
        source_row = matches[0]
        members = _sound_members(source_row, catalog, ledger)
        grouped, missing, case_mismatches = _sound_files(members, asset_index)
        target = _source_sound_target(source_row, members, grouped, missing, original_event, platform)
        target["catalog_evidence"] = _SOUND_EVIDENCE if is_bres_sound else _SDD_EVIDENCE
        target["case_mismatch_candidates"] = case_mismatches
        if not members:
            status, reason = "UNRESOLVED", "SOUND_CATALOG_MISSING"
        elif missing:
            status, reason = "UNRESOLVED", "FILE_MISSING"
        else:
            status, reason = "RESOLVED", None
        _set_resolution(ref, [target], status, reason,
                        role="named_animation_sound" if is_bres_sound else "xml_sound_label",
                        detail=("Exact sfx_ suffix -> exact source Sounds name -> source row UID/event -> sounds.xml member filename -> full data/sounds URI in the Android/iOS inventory union."
                                if is_bres_sound else
                                "Exact XML sound-label attribute -> exact source SDD row; no label normalization or prefix stripping."),
                        confidence="PROVEN" if platform == "android" else "LIKELY")
        if missing:
            if case_mismatches:
                ref["candidate_note"] = ("The exact case-sensitive physical URI is absent; case-folded paths are diagnostic candidates only and were not accepted as matches.")
            else:
                ref["candidate_note"] = "Source SDD/catalog member exists, but the exact case-sensitive physical URI is absent from both supplied platform inventories."
        elif case_mismatches:
            ref["candidate_note"] = "Case-folded path candidates exist, but exact path casing does not match the sounds.xml filename; no case correction was applied."
    return result


def summarize(refs):
    selected = [ref for ref in refs or []
                if (ref.get("resource_semantics") or {}).get("resolver") == "resource_semantics.v1"]
    return {
        "annotated_references": len(selected),
        "by_status": dict(Counter(ref.get("status") for ref in selected)),
        "by_role": dict(Counter((ref.get("resource_semantics") or {}).get("role") for ref in selected)),
        "native_event_targets": sum(any(t.get("type") == "native_event" for t in ref.get("resolves_to") or []) for ref in selected),
        "source_sound_targets": sum(any(t.get("type") == "source_sound" for t in ref.get("resolves_to") or []) for ref in selected),
        "missing_sound_members": sum(len(next((t.get("missing_filenames", []) for t in ref.get("resolves_to") or [] if t.get("type") == "source_sound"), [])) for ref in selected),
    }
