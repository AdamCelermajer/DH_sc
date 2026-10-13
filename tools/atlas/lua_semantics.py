"""Source-backed resolution for Lua references and duplicate semantic tokens.

This module does not execute Lua or infer targets from similar names. It joins
exact call-ledger literals to Android's native table/constant providers, the
atlas's already-proven script-link edges, or an exact indexed Lua source file.
"""
from __future__ import annotations

import copy
import re
import struct


_RESOLVER = "lua_semantics.v1"
_FIELD_ARRAY = re.compile(
    r"process_members_CharacterProperties_v121\[\]\s*=\s*\{([^}]+)\}",
    re.DOTALL,
)
_C_STRING = re.compile(r'"((?:[^"\\]|\\.)*)"')


def _store(ref, rule, evidence, *, status, reason=None, resolves_to=None):
    ref["status"] = status
    ref["work_status"] = "DONE"
    ref["confidence"] = "PROVEN"
    ref["reason"] = (rule if status == "NOT-NEEDED" else reason)
    ref["reason_code"] = ("OTHER" if status == "NOT-NEEDED" else reason)
    ref["resolves_to"] = list(resolves_to or [])
    ref["rule"] = rule
    ref["candidates"] = []
    ref.pop("candidate_note", None)
    ref["semantic_resolution"] = {
        "resolver": _RESOLVER,
        "rule": rule,
        "evidence": evidence,
    }


def _unquote(value):
    if not isinstance(value, str) or len(value) < 2 or value[0] not in "\"'" or value[-1] != value[0]:
        return None
    body = value[1:-1]
    # This resolver only accepts plain exact literals. Escape interpretation
    # requires Lua's lexer rules and is deliberately left unresolved here.
    return None if "\\" in body else body


def _literal_args(call):
    """Return source strings only when every requested argument is one literal."""
    result = []
    for arg in call.get("arguments", []):
        literals = arg.get("literals") or []
        if (len(literals) != 1 or literals[0].get("type") != "string"
                or arg.get("expression") != literals[0].get("raw")):
            return None
        result.append((arg.get("index"), literals[0].get("value"), literals[0].get("raw")))
    return result


def _pycst_groups(ctx):
    """Decode PyDataConstants in the Android loader's exact registration order."""
    root = ctx.inputs / "assets-extra/android/data/pydata"
    manifest = ctx.root / "port/android-native/app/src/main/cpp/source_process_pydata_files_v101.hpp"
    source = manifest.read_text(encoding="utf-8")
    match = re.search(r"process_pydata_constants_v101\[\]\s*=\s*\{(.*?)\};", source, re.DOTALL)
    if not match:
        raise ValueError("Android PyDataConstants registration order is absent")
    filenames = re.findall(r'\{"pydata/([^\"]+_pycst\.bin)"', match.group(1))
    paths = [root / name for name in filenames if (root / name).is_file()]
    groups, sources = {}, {}

    class SourceNameStop(Exception):
        pass

    def decode(path):
        raw = path.read_bytes()
        at = 0

        def word():
            nonlocal at
            if at + 4 > len(raw):
                raise ValueError(f"truncated Android {path.name}")
            value = struct.unpack_from("<I", raw, at)[0]
            at += 4
            return value

        def name():
            nonlocal at
            size = word()
            # The recovered source reader stops the current file at a name
            # length >= 256, preserving only prior complete assignments.
            if size >= 256:
                raise SourceNameStop
            if size > len(raw) - at:
                raise ValueError(f"invalid string in Android {path.name}")
            value = raw[at:at + size].split(b"\0", 1)[0].decode("utf-8", "strict")
            at += size
            return value

        decoded = {}
        try:
            count = word()
            if count > 65536:
                raise ValueError(f"invalid group count in Android {path.name}")
            for _ in range(count):
                group, entries = name(), word()
                if entries > 100000:
                    raise ValueError(f"invalid entry count in Android {path.name}")
                values = decoded.setdefault(group, {})
                for _ in range(entries):
                    key, value = name(), struct.unpack("<i", struct.pack("<I", word()))[0]
                    values[key] = value
        except SourceNameStop:
            return decoded
        if at != len(raw):
            raise ValueError(f"trailing bytes in Android {path.name}")
        return decoded

    for path in paths:
        site = ".local-inputs/" + path.relative_to(ctx.inputs).as_posix()
        for group, entries in decode(path).items():
            for key, value in entries.items():
                groups.setdefault(group, {})[key] = value
                sources[(group, key)] = site
    return groups, sources


def _character_property_fields(ctx):
    """Read the Android-IDA-derived registered field order from the source handoff."""
    path = ctx.root / "port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp"
    source = path.read_text(encoding="utf-8")
    match = _FIELD_ARRAY.search(source)
    if not match:
        raise ValueError("CharacterProperties source field array is absent")
    names = [bytes(item, "utf-8").decode("unicode_escape") for item in _C_STRING.findall(match.group(1))]
    if len(names) != 224:
        raise ValueError(f"CharacterProperties field count changed: expected 224, found {len(names)}")
    return {name: index for index, name in enumerate(names)}, ".repo/port/android-native/app/src/main/cpp/source_process_compiled_members_v121.hpp"


def _target_row(rows_by_key, table, name):
    matches = rows_by_key.get((table, name), [])
    return matches[0] if len(matches) == 1 else None


def _mark_not_needed(ref, rule, evidence):
    _store(ref, rule, evidence, status="NOT-NEEDED", reason="OTHER")


def _android_source_call(ref, calls_by_id, scripts_by_id):
    context = ref.get("context") or {}
    call = calls_by_id.get(context.get("call_id"))
    script = scripts_by_id.get(context.get("script_id") or (call or {}).get("script_id"))
    return call, script if script and script.get("platform") == "android" else None


def resolve(ctx, refs, tables, calls, scripts):
    """Resolve Lua references with exact source-backed table/schema contracts.

    Input rows are copied. The function is deterministic and idempotent, so the
    caller can compose it after ``script_links.resolve`` without altering the
    reference IDs or the original call/source ledgers.
    """
    result = copy.deepcopy(refs)
    calls_by_id = {row.get("id"): row for row in calls if row.get("id")}
    scripts_by_id = {row.get("id"): row for row in scripts if row.get("id")}
    rows_by_key = {}
    for row in tables:
        if row.get("name") is not None:
            rows_by_key.setdefault((row.get("table"), row.get("name")), []).append(row)
    refs_by_call = {}
    refs_by_file_line = {}
    for ref in result:
        context = ref.get("context") or {}
        if context.get("call_id"):
            refs_by_call.setdefault(context["call_id"], []).append(ref)
        refs_by_file_line.setdefault((context.get("script"), context.get("line")), []).append(ref)

    field_ids, field_source = _character_property_fields(ctx)
    constants, constants_source = _pycst_groups(ctx)

    # Promote exact, already-proven call edges to the outer symbolic reference.
    # The edge's target is revalidated against this invocation's source rows.
    for ref in result:
        if not _android_source_call(ref, calls_by_id, scripts_by_id)[1]:
            continue
        if ref.get("kind") != "symbolic_reference" or ref.get("status") == "RESOLVED":
            continue
        edges = [edge for edge in ref.get("derived_edges", [])
                 if edge.get("resolver") == "script_links.v1"
                 and edge.get("status") == "PROVEN"
                 and edge.get("confidence") == "PROVEN"]
        valid = []
        for edge in edges:
            table, name = edge.get("table"), edge.get("name")
            if edge.get("type") == "native-string-member":
                if edge.get("namespace") != "Sounds":
                    continue
                table = "sdd_dungeon_hunter_2_iphone"
            row = _target_row(rows_by_key, table, name)
            if row and (edge.get("type") == "native-string-member" or
                        row.get("index") == edge.get("index") and row.get("id") == edge.get("target_row_id")):
                valid.append((edge, row))
        unique = {(row.get("table"), row.get("index"), row.get("id")) for _, row in valid}
        if len(unique) != 1:
            continue
        edge, row = valid[0]
        target = {"type": "table-row", "table": row["table"], "index": row["index"],
                  "name": row["name"], "row_id": row["id"]}
        ref["reference_kind"] = "table_row"
        ref["referenced_table"] = row["table"]
        ref["referenced_index"] = row["index"]
        _store(ref,
               ("Exact Lua constant-flow trace and Android native API namespace identify one source table row."
                if edge.get("type") == "table-row" else
                "Exact Lua constant-flow trace and native Sounds member lookup identify one source sound-table row."),
               {"edge": edge, "source_row_id": row["id"], "source_path": row.get("source_path")},
               status="RESOLVED", resolves_to=[target])

    # Literal first operands also occur without a symbolic extractor row (for
    # example StopSound). The native wrappers prove the same Sounds lookup.
    sound_contracts={'PlaySound':'0x0037e578','PlayMusic':'0x0037e420','StopSound':'0x0037e730'}
    for ref in result:
        if not _android_source_call(ref, calls_by_id, scripts_by_id)[1]:
            continue
        context=ref.get('context') or {}
        address=sound_contracts.get(context.get('call'))
        if not address or context.get('argument_index')!=1 or ref.get('status')=='RESOLVED':
            continue
        call=calls_by_id.get(context.get('call_id'),{})
        script=scripts_by_id.get(call.get('script_id'),{})
        if call.get('call_mode')!='global' or script.get('platform')!='android':continue
        arg=next((a for a in call.get('arguments',[]) if a.get('index')==1),{})
        literals=arg.get('literals') or []
        if len(literals)!=1 or literals[0].get('type')!='string' or arg.get('expression')!=literals[0].get('raw'):
            continue
        name=_unquote(arg['expression'])
        if name is None:continue
        row=_target_row(rows_by_key,'sdd_dungeon_hunter_2_iphone',name)
        if not row:continue
        ref.update(reference_kind='table_row',referenced_table=row['table'],referenced_index=row['index'])
        _store(ref,'Exact first-string Android sound API operand -> unique Sounds source row.',
            {'native_wrapper':address,'native_lookup':'Arrays::GetMemberIDByString<Arrays::Sounds>',
             'namespace':'Sounds','name':name,'source_row_id':row['id'],
             'call_id':call['id'],'script':script['site'],'line':call['line']},
            status='RESOLVED',resolves_to=[{'type':'table-row','table':row['table'],
                'index':row['index'],'name':name,'row_id':row['id']}])

    # Resolve GetPyStruct's exact CharacterProperties field using its proven
    # 224-member registered schema. This is a schema member, not a data row.
    for ref in result:
        if not _android_source_call(ref, calls_by_id, scripts_by_id)[1]:
            continue
        if ref.get("kind") != "symbolic_reference" or ref.get("status") == "RESOLVED":
            continue
        context = ref.get("context") or {}
        if context.get("call") != "GetPyStruct" or context.get("argument_index") != 2:
            continue
        call = calls_by_id.get(context.get("call_id"))
        args = _literal_args(call or {})
        if not args or [(i, value) for i, value, _ in args] != [(1, "CharacterProperties"), (2, _unquote(ref.get("name", "")))]:
            continue
        field = _unquote(ref.get("name", ""))
        if field not in field_ids:
            continue
        ref["reference_kind"] = "table_field"
        _store(ref,
               "GetPyStruct exact namespace/member strings -> Android registered CharacterProperties field order.",
               {"native_wrapper": "pseudocode-all.c LuaScript::_GetPyStruct 0x0037f4a8 -> PyDataArrays::GetOID 0x004bd640",
                "registration": "source_process_array_registration_v101.hpp:234 (CharacterProperties, 224, getter 0x4af110)",
                "schema_source": field_source, "field_index": field_ids[field], "field_count": 224,
                "call_id": call.get("id"), "script": call.get("site"), "line": call.get("line")},
               status="RESOLVED", resolves_to=[{"type": "table-field", "table": "CharacterProperties",
                                                  "index": field_ids[field], "name": field}])

    # Resolve exact GetPyCst namespace/member pairs from the actual Android
    # constants blob. The generic native getter consumes a group and member;
    # every exact pair is a numeric constant, not a resource row.
    for ref in result:
        if not _android_source_call(ref, calls_by_id, scripts_by_id)[1]:
            continue
        if ref.get("kind") != "symbolic_reference" or ref.get("status") == "RESOLVED":
            continue
        context = ref.get("context") or {}
        if context.get("call") != "GetPyCst" or context.get("argument_index") != 2:
            continue
        call = calls_by_id.get(context.get("call_id"))
        args = _literal_args(call or {})
        value_name = _unquote(ref.get("name", ""))
        if not args or len(args) != 2 or args[1][1] != value_name:
            continue
        namespace = args[0][1]
        value = constants.get(namespace, {}).get(value_name)
        if value is None:
            continue
        ref["reference_kind"] = "enum_constant"
        _mark_not_needed(ref,
                         "GetPyCst resolves this exact namespace/member as a source numeric constant, not an external resource.",
                         {"native_wrapper": "pseudocode-all.c LuaScript::_GetPyCst 0x0037f354 -> PyDataConstants::getConstant 0x004c4bdc",
                          "source_cache": constants_source.get((namespace, value_name)), "namespace": namespace, "name": value_name,
                          "signed_value": value, "call_id": call.get("id"), "script": call.get("site"),
                          "line": call.get("line")})

    # A callback method string is terminally classified only for exact method
    # names whose native AISExternal callers are present in Android IDA.
    callback_contracts = {
        "OnDied": {"native_dispatch": "AISExternal::OnDied 0x003dd440", "dispatch_key": "OnDied",
                    "registration": "LuaScript::_AddToVFTable 0x0037ec70; CharAI event 2 uses callback slot +0x24"},
        "OnEndOfAnim": {"native_dispatch": "AISExternal::OnEndOfAnim 0x003dccd0", "dispatch_key": "OnEndOfAnim",
                         "registration": "LuaScript::_AddToVFTable 0x0037ec70; CharAI events 0x22/0x23 use callback slot +0x98"},
        "OnInitFinal": {"native_dispatch": "AISExternal::OnInitFinal 0x003dce44", "dispatch_key": "OnInitFinal",
                         "registration": "LuaScript::_AddToVFTable 0x0037ec70"},
        "OnTargetDied": {"native_dispatch": "AISExternal::OnTargetDied 0x003dce24", "dispatch_key": "OnTargetDied",
                          "registration": "LuaScript::_AddToVFTable 0x0037ec70"},
        "OnTargetInCloseRange": {"native_dispatch": "AISExternal::OnTargetInCloseRange 0x003dcdb0", "dispatch_key": "OnTargetInCloseRange",
                                  "registration": "LuaScript::_AddToVFTable 0x0037ec70; AISExternal::InitVCB 0x003dcec8 derives bit 0x10 from IsInVFTable and gates this callback"},
        "OnTerminate": {"native_dispatch": "AISExternal::OnTerminate 0x003dce34", "dispatch_key": "OnTerminate",
                         "registration": "LuaScript::_AddToVFTable 0x0037ec70"},
    }
    # The Android external-AI bridge dispatches these exact keys through
    # AISExternal::On* wrappers. Registration alone is not proof that a
    # callback will run: InitVCB and each caller retain their source gates.
    for key, address in {
        "OnMasterInMeleeRange": "0x003dccf4", "OnMasterInCloseRange": "0x003dcd10",
        "OnMasterInRangedRange": "0x003dcd2c", "OnMasterOutOfRange": "0x003dcd48",
        "OnMasterInSight": "0x003dcd64", "OnMasterOutOfSight": "0x003dcd74",
        "OnMasterDied": "0x003dcd84", "OnTargetInMeleeRange": "0x003dcd94",
        "OnTargetInRangedRange": "0x003dcdcc", "OnTargetOutOfRange": "0x003dcde8",
        "OnTargetInSight": "0x003dce04", "OnTargetOutOfSight": "0x003dce14",
        "OnInitPost": "0x003dce54", "OnUpdate": "0x003dce64",
        "OnInit": "0x003dcea4", "OnCombatResults": "0x003dd250",
        "OnProjectileHit": "0x003dd2a0", "OnEnemySpotted": "0x003dd2f4",
        "OnNeutralSpotted": "0x003dd344", "OnFriendSpotted": "0x003dd394",
        "OnRevived": "0x003dd3f0",
    }.items():
        callback_contracts.setdefault(key, {
            "native_dispatch": f"AISExternal::{key} {address}", "dispatch_key": key,
            "registration": "LuaScript::_AddToVFTable 0x0037ec70; AISExternal::InitVCB 0x003dcec8 and callback-specific callers gate delivery",
        })
    for ref in result:
        if not _android_source_call(ref, calls_by_id, scripts_by_id)[1]:
            continue
        if ref.get("kind") != "symbolic_reference" or ref.get("status") == "RESOLVED":
            continue
        context = ref.get("context") or {}
        call = calls_by_id.get(context.get("call_id"))
        api = context.get("call")
        arg_index = context.get("argument_index")
        if api == "RegisterSkill" and isinstance(arg_index, int) and 1 <= arg_index <= 7:
            script = scripts_by_id.get((call or {}).get("script_id"), {})
            if ((call or {}).get("call_mode") not in ("global", "imported_script_function") or script.get("platform") != "android"
                    or not str(script.get("site", "")).replace("\\", "/").startswith(".local-inputs/assets-extra/android/data/scripts/skills/")):
                continue
            args = call.get("arguments", [])
            arg = next((entry for entry in args if entry.get("index") == arg_index), None)
            name = ref.get("name", "")
            key = {1: "update", 2: "check", 3: "pre", 4: "skill", 5: "post", 6: "delayed", 7: "info"}[arg_index]
            if not arg or arg.get("expression") != name or not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", name):
                continue
            ref["reference_kind"] = "skill_callback"
            _mark_not_needed(ref, "RegisterSkill positional callback argument follows the Android _commons.luac callback schema.",
                {"helper_source": ".local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:41-49",
                 "callback_field": key, "argument_index": arg_index, "call_id": call.get("id"),
                 "script": script.get("site"), "line": call.get("line")})
            continue
        if api == "RegisterSkillEx" and arg_index == 1:
            script = scripts_by_id.get((call or {}).get("script_id"), {})
            arg = next((entry for entry in (call or {}).get("arguments", []) if entry.get("index") == 1), {})
            expression = (arg.get("expression") or "").strip()
            known_fields = {"update", "cleanup", "check", "pre", "skill", "post", "delayed", "delayed_check", "info"}
            field = r"(?:update|cleanup|check|pre|skill|post|delayed|delayed_check|info)"
            simple_callback_table = re.fullmatch(
                rf"\{{\s*{field}\s*=\s*(?:[A-Za-z_][A-Za-z_0-9]*|nil)"
                rf"(?:\s*,\s*{field}\s*=\s*(?:[A-Za-z_][A-Za-z_0-9]*|nil))*\s*,?\s*\}}",
                expression, re.DOTALL)
            fields = re.findall(rf"(?:\{{|,)\s*({field})\s*=", expression) if simple_callback_table else []
            if ((call or {}).get("call_mode") not in ("global", "imported_script_function") or script.get("platform") != "android"
                    or not str(script.get("site", "")).replace("\\", "/").startswith(".local-inputs/assets-extra/android/data/scripts/skills/")
                    or not simple_callback_table or not fields or not set(fields) <= known_fields):
                continue
            ref["reference_kind"] = "skill_callback_table"
            _mark_not_needed(ref, "RegisterSkillEx argument is an authored callback table using the helper's declared field schema.",
                {"helper_source": ".local-inputs/assets-extra/android/data/scripts/skills/_commons.luac:52-78",
                 "callback_fields": fields, "call_id": call.get("id"), "script": script.get("site"), "line": call.get("line")})
            continue
        if api != "AddToVFTable" or arg_index != 1:
            continue
        args = _literal_args(call or {})
        key = _unquote(ref.get("name", ""))
        proof = callback_contracts.get(key)
        if not args or len(args) != 2 or args[0][1] != key or not proof:
            continue
        ref["reference_kind"] = "callback_method"
        _mark_not_needed(ref,
                         "AddToVFTable key is the exact native-dispatched Lua callback name; it does not name a data resource.",
                         {"registration_call": call.get("expression"), "registration_api": proof["registration"],
                          "native_dispatch": proof["native_dispatch"],
                          "lua_dispatch": f'LuaScript::Call(..., "{proof["dispatch_key"]}")',
                          "ida_source": ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c",
                          "script": call.get("site"), "line": call.get("line")})

    # Numeric/boolean operands at documented sound API positions are playback
    # parameters. Preserve their value and call location as evidence, while
    # keeping them out of the resource-reference graph. DoSkill slots are
    # intentionally excluded: they select entries in the actor's SkillTree.
    scalar_contracts = {
        ("PlayMusic", 2): ("number", "pseudocode-all.c LuaScript::_PlayMusic 0x0037e420", "music playback scalar"),
        ("PlaySound", 2): ("boolean", "pseudocode-all.c LuaScript::_PlaySound 0x0037e578", "sound playback boolean"),
        ("PlaySound", 3): ("number", "pseudocode-all.c LuaScript::_PlaySound 0x0037e578", "sound playback scalar"),
        ("PlaySound", 4): ("boolean", "pseudocode-all.c LuaScript::_PlaySound 0x0037e578", "sound playback boolean"),
        ("StopSound", 2): ("number", "pseudocode-all.c LuaScript::_StopSound 0x0037e730", "sound stop duration"),
        ("TargetListSearch", 2): ("number", "pseudocode-all.c GameObject::_TargetListSearch 0x0038fcf0", "target search geometry parameter"),
    }
    for ref in result:
        if not _android_source_call(ref, calls_by_id, scripts_by_id)[1]:
            continue
        if ref.get("kind") == "symbolic_reference" and ref.get("status") == "RESOLVED":
            continue
        context = ref.get("context") or {}
        contract = scalar_contracts.get((context.get("call"), context.get("argument_index")))
        if not contract or ref.get("kind") not in ("numeric_reference", "symbolic_reference", "named_reference"):
            continue
        call = calls_by_id.get(context.get("call_id"))
        arg = next((entry for entry in (call or {}).get("arguments", [])
                    if entry.get("index") == context.get("argument_index")), None)
        if not arg:
            continue
        raw_value = arg.get("expression", "").strip()
        expected_type, native_binding, scalar_role = contract
        if expected_type == "boolean":
            if raw_value.lower() not in ("true", "false"):
                continue
        else:
            if not re.fullmatch(r"[+-]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?", raw_value):
                continue
            try:
                numeric_value = float(raw_value)
            except (TypeError, ValueError):
                continue
            if not numeric_value == numeric_value:
                continue
        ref["reference_kind"] = "native_scalar_argument"
        _mark_not_needed(ref,
                         f"Exact native API position consumes this literal as a {scalar_role}.",
                         {"native_binding": native_binding, "argument_index": context.get("argument_index"),
                          "scalar_type": expected_type, "literal_value": raw_value,
                          "call_id": call.get("id"), "script": call.get("site"), "line": call.get("line")})

    # Mark duplicate nested string mentions as NOT-NEEDED only when a sibling
    # expression from that same source call now has a proven outcome.
    for ref in result:
        if ref.get("kind") != "named_reference" or ref.get("status") == "RESOLVED":
            continue
        context = ref.get("context") or {}
        expression = context.get("argument_expression")
        if not expression or context.get("literal_type") != "string":
            continue
        siblings = refs_by_call.get(context.get("call_id"), [])
        parent = next((row for row in siblings
                       if row.get("kind") == "symbolic_reference"
                       and row.get("name") == expression
                       and row.get("status") == "RESOLVED"
                       and row.get("semantic_resolution", {}).get("resolver") == _RESOLVER), None)
        if parent:
            _mark_not_needed(ref,
                             "Nested literal is already represented by its exact, resolved enclosing Lua expression.",
                             {"parent_reference_id": parent.get("id"), "expression": expression,
                              "literal": ref.get("name"), "call_id": context.get("call_id"),
                              "script": context.get("script"), "line": context.get("line")})
            continue
        # GetProp wrappers contain a nested GetPyStruct call. The outer lexical
        # literal rows are duplicate mentions when the inner member reference
        # on this exact source line resolves through the schema above.
        if expression.startswith(("GetPyStruct(", "GetPyCst(")):
            expected_inner_call = "GetPyStruct" if expression.startswith("GetPyStruct(") else "GetPyCst"
            line_refs = refs_by_file_line.get((context.get("script"), context.get("line")), [])
            member = next((row for row in line_refs
                           if row.get("kind") == "symbolic_reference"
                           and row.get("context", {}).get("call") == expected_inner_call
                           and (calls_by_id.get(row.get("context", {}).get("call_id")) or {}).get("expression") == expression
                           and row.get("status") in ("RESOLVED", "NOT-NEEDED")
                           and row.get("semantic_resolution", {}).get("resolver") == _RESOLVER), None)
            if member:
                _mark_not_needed(ref,
                                 f"Outer API lexical token duplicates the exact {expected_inner_call} lookup on this source line.",
                                 {"member_reference_id": member.get("id"), "expression": expression,
                                  "literal": ref.get("name"), "script": context.get("script"),
                                  "line": context.get("line")})

    return result
