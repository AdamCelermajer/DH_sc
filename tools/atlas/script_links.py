"""Conservative Lua constant-flow links for data-bearing script APIs.

This module adds auditable edges from a symbolic argument (for example
``PlayFX(TELEPORT_FX)``) to an exact PyData row only when the source contains a
single simple binding to ``GetPyOID`` and the native consumer proves that the
API argument uses the same table namespace. It does not execute Lua or guess
branch outcomes.
"""
from __future__ import annotations

import copy
import hashlib
import json
import re
from pathlib import Path

from scripts import _tokens

# Native consumers establish these first-argument namespaces. In particular,
# _PlayAnim and _RegisterAnim index Arrays::AnimDict; _PlayFX checks the first
# value against Arrays::AnimatedEffectTable; the shared Lua SpawnSkillProjectile
# helper prepends SKILL_ID, so the authored first argument reaches the native
# projectile slot checked against Arrays::ProjectileTable.
_API_NAMESPACE = {
    "PlayAnim": (1, "AnimDict", "animations_dictionary#group0", "0x003ba388"),
    "RegisterAnim": (1, "AnimDict", "animations_dictionary#group0", "0x003b70d8"),
    "PlayFX": (1, "AnimatedEffectTable", "effect_sets", "0x003923cc"),
    # _commons.luac prepends SKILL_ID, so authored argument 1 reaches native
    # argument 1 (the projectile row) after the hidden skill slot is removed.
    "SpawnSkillProjectile": (1, "ProjectileTable", "projectiles#group0", "0x003b99d0"),
    "ApplyPropClass": (1, "ClassTable", "character_classes#group0", "0x003babdc"),
    "CreateBuff": (1, "ClassTable", "character_classes#group0", "0x003b86a8"),
    "ApplyBuff": (1, "ClassTable", "character_classes#group0", "0x003babdc"),
}
_API_STRING_MEMBER = {
    # These handlers call Arrays::GetMemberIDByString<Arrays::Sounds> on arg 0.
    "PlaySound": (1, "Sounds", "0x0037e578"),
    "PlayMusic": (1, "Sounds", "0x0037e420"),
    "PlaySound3D": (1, "Sounds", "0x0039212c"),
}

_GET_PYOID = re.compile(
    r'^\s*GetPyOID\s*\(\s*([\"\'])([^\"\']+)\1\s*,\s*([\"\'])([^\"\']+)\3\s*\)\s*;?\s*$'
)
_IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z_0-9]*$")
_STRING = re.compile(r"^([\"'])([^\"']*)\1$")
_NUMBER = re.compile(r"^-?\d+(?:\.\d+)?$")
_RESOLVER_ID = "script_links.v1"


def _edge_id(ref, edge_type, target):
    key = json.dumps([ref.get("id"), edge_type, target], ensure_ascii=False, separators=(",", ":"))
    return "script-link:" + hashlib.sha256(key.encode("utf-8")).hexdigest()[:20]


def _store_trace(ref, payload):
    ref["symbolic_trace"] = {**payload, "resolver": _RESOLVER_ID}


def _assignments(text):
    """Return uncomplicated whole-line assignments, preserving source lines."""
    found = {}
    lines = text.splitlines()
    tokens = _tokens(text)
    local_names = set()
    block_depth = []
    depth = 0
    pending_loop_do = False
    for token in tokens:
        block_depth.append(depth)
        value = token[1]
        if value in ("function", "if", "repeat"):
            depth += 1
        elif value in ("for", "while"):
            depth += 1
            pending_loop_do = True
        elif value == "do":
            if pending_loop_do:
                pending_loop_do = False
            else:
                depth += 1
        elif value == "end":
            depth = max(0, depth - 1)
            pending_loop_do = False
        elif value == "until":
            depth = max(0, depth - 1)
    # Imported snippets share their global environment, but a local declaration
    # shadows that spelling for the whole chunk. Record declarations even when
    # they have no initializer (`local X; X = ...`). A conservative whole-file
    # name gate is used for imports, avoiding a cross-include local leak.
    for index, token in enumerate(tokens):
        if token[1] != "local" or index + 1 >= len(tokens):
            continue
        cursor = index + 1
        if tokens[cursor][1] == "function":
            if cursor + 1 < len(tokens) and tokens[cursor + 1][0] == "id":
                local_names.add(tokens[cursor + 1][1])
            continue
        while cursor < len(tokens) and tokens[cursor][0] == "id":
            local_names.add(tokens[cursor][1])
            cursor += 1
            if cursor >= len(tokens) or tokens[cursor][1] != ",":
                break
            cursor += 1
    # Count every simple identifier LHS, including assignments inside
    # conditional/inline statements and unsupported expressions. For a tuple
    # assignment, invalidate every identifier on its comma-separated LHS;
    # recording only the final name would leave earlier names falsely unique.
    for eq_index, token in enumerate(tokens):
        if token[1] != "=" or eq_index == 0 or (eq_index + 1 < len(tokens) and tokens[eq_index + 1][1] == "="):
            continue
        lhs = []
        cursor = eq_index - 1
        if tokens[cursor][0] == "id" and (cursor == 0 or tokens[cursor - 1][1] not in (".", ":")):
            lhs.append(cursor)
            cursor -= 1
            while cursor >= 1 and tokens[cursor][1] == "," and tokens[cursor - 1][0] == "id":
                lhs.append(cursor - 1)
                cursor -= 2
        if not lhs:
            continue
        lhs.reverse()
        multiple_lhs = len(lhs) > 1
        for index in lhs:
            name, line_no = tokens[index][1], tokens[index][2]
            source_line = lines[line_no - 1]
            m = re.fullmatch(r"\s*(?:local\s+)?([A-Za-z_][A-Za-z_0-9]*)\s*=\s*(.*?)\s*;?\s*", source_line)
            whole_line_assignment = bool(m and m.group(1) == name)
            # Lua indentation is formatting, not scope. The token block depth
            # is the scope gate; the whole-line regexp below proves syntax.
            top_level = block_depth[index] == 0
            rhs = m.group(2) if not multiple_lhs and top_level and whole_line_assignment else ""
            if not (_GET_PYOID.fullmatch(rhs) or _IDENTIFIER.fullmatch(rhs) or _STRING.fullmatch(rhs) or _NUMBER.fullmatch(rhs)):
                rhs = ""
            found.setdefault(name, []).append({"line": line_no, "rhs": rhs, "source": source_line.strip(),
                                               "scope_depth": block_depth[index],
                                               "multiple_lhs": multiple_lhs,
                                               "whole_line_assignment": whole_line_assignment,
                                               "local_declared_in_file": name in local_names})
    return found


def _site_path(ctx, site):
    prefix = ".local-inputs/"
    if not isinstance(site, str) or not site.startswith(prefix):
        return None
    path = Path(ctx.inputs) / site[len(prefix):]
    return path if path.is_file() else None


def _included_sources(ctx, script, calls_by_script):
    """Read only explicit Include targets and imports already proven by scripts.py."""
    paths = set()
    for value in (script.get("imports") or {}).values():
        for site in value if isinstance(value, list) else [value]:
            path = _site_path(ctx, site)
            if path:
                paths.add(path)
    for call in calls_by_script.get(script.get("id"), []):
        if call.get("name") != "Include":
            continue
        args = call.get("arguments") or []
        if not args:
            continue
        expr = args[0].get("expression", "")
        match = _STRING.fullmatch(expr)
        if not match:
            continue
        target = match.group(2).replace("\\", "/")
        script_path = _site_path(ctx, script.get("site"))
        if not script_path:
            continue
        data_root = Path(ctx.inputs) / "assets-extra" / script.get("platform", "android") / "data" / "scripts"
        candidates = [script_path.parent / target, data_root / target]
        expanded = []
        for candidate in candidates:
            expanded.append(candidate)
            if not candidate.suffix:
                expanded.extend([Path(str(candidate) + ".luac"), Path(str(candidate) + ".lua")])
        for candidate in expanded:
            if candidate.is_file():
                paths.add(candidate)
                break
    return sorted(paths)


def _trace(ctx, script, calls_by_script, expression, use_line):
    source = _site_path(ctx, script.get("site"))
    if not source:
        return None, {"status": "UNRESOLVED", "reason": "SCRIPT_SOURCE_MISSING"}
    text = source.read_text(encoding="utf-8-sig", errors="replace")
    assignments = _assignments(text)
    bindings = []
    current = expression.strip()
    seen = set()
    site = script.get("site")
    while _IDENTIFIER.fullmatch(current):
        if current in seen:
            return None, {"status": "UNRESOLVED", "reason": "BINDING_CYCLE", "bindings": bindings}
        seen.add(current)
        options = [x for x in assignments.get(current, []) if x["line"] < use_line]
        all_options = assignments.get(current, [])
        # Reassignment, a later write, or multiple in-scope candidates means
        # lexical/runtime behavior could change the value; retain as unknown.
        if all_options and (len(all_options) != 1 or len(options) != 1):
            reason = "MULTIPLE_WRITES" if len(all_options) > 1 else "BINDING_AFTER_USE"
            return None, {"status": "UNRESOLVED", "reason": reason, "variable": current, "bindings": bindings,
                          "writes": [{"line": x["line"], "expression": x["rhs"]} for x in all_options]}
        if len(all_options) != 1 or len(options) != 1:
            # A global imported literal may be consumed without a local copy.
            imports = _included_sources(ctx, script, calls_by_script)
            imported = []
            for path in imports:
                imported_text = path.read_text(encoding="utf-8-sig", errors="replace")
                candidate = _assignments(imported_text).get(current, [])
                # A global binding only: imported locals are not visible.
                for item in candidate:
                    line_text = imported_text.splitlines()[item["line"] - 1].lstrip()
                    if (item.get("scope_depth") == 0 and not item.get("local_declared_in_file")
                            and not re.match(r"local\s+", line_text)
                            and item.get("rhs")):
                        imported.append((path, item, candidate))
            if len(imported) != 1 or len(imported[0][2]) != 1:
                reason = "MULTIPLE_WRITES" if len(all_options) > 1 else (
                    "BINDING_AFTER_USE" if all_options and not options else "MISSING_OR_AMBIGUOUS_BINDING")
                return None, {"status": "UNRESOLVED", "reason": reason, "variable": current, "bindings": bindings,
                              "writes": [{"line": x["line"], "expression": x["rhs"]} for x in all_options]}
            path, item, _ = imported[0]
            source_site = ".local-inputs/" + path.relative_to(Path(ctx.inputs)).as_posix()
        else:
            item = options[0]
            path = source
            source_site = site
        bindings.append({"variable": current, "source": source_site, "line": item["line"], "expression": item["rhs"], "text": item["source"]})
        if not item["rhs"]:
            if item.get("multiple_lhs"):
                reason = "MULTIPLE_ASSIGNMENT_LHS"
            elif item.get("scope_depth", 0) > 0:
                reason = "SCOPED_BINDING_NOT_ADMITTED"
            elif item.get("whole_line_assignment"):
                reason = "UNSUPPORTED_BINDING_EXPRESSION"
            else:
                reason = "INLINE_OR_UNSUPPORTED_ASSIGNMENT"
            return None, {"status": "UNRESOLVED", "reason": reason, "variable": current,
                          "bindings": bindings, "source": source_site, "line": item["line"]}
        current = item["rhs"].strip().rstrip(";").strip()
        # Follow exact GetPyOID calls as symbolic table/name pairs.
        query = _GET_PYOID.fullmatch(current)
        if query:
            if "\\" in query.group(2) or "\\" in query.group(4):
                return None, {"status": "UNRESOLVED", "reason": "UNSUPPORTED_ESCAPED_GETPYOID_LITERAL",
                              "expression": current, "bindings": bindings}
            return {"kind": "pydata", "namespace": query.group(2), "name": query.group(4)}, {"status": "TRACED", "bindings": bindings}
        literal = _STRING.fullmatch(current)
        if literal:
            if "\\" in literal.group(2):
                return None, {"status": "UNRESOLVED", "reason": "UNSUPPORTED_ESCAPED_STRING", "expression": current,
                              "bindings": bindings}
            return {"kind": "string", "value": literal.group(2)}, {"status": "TRACED", "bindings": bindings}
        if _NUMBER.fullmatch(current):
            return {"kind": "number", "value": current}, {"status": "TRACED", "bindings": bindings}
    query = _GET_PYOID.fullmatch(current)
    if query:
        if "\\" in query.group(2) or "\\" in query.group(4):
            return None, {"status": "UNRESOLVED", "reason": "UNSUPPORTED_ESCAPED_GETPYOID_LITERAL",
                          "expression": current, "bindings": bindings}
        return {"kind": "pydata", "namespace": query.group(2), "name": query.group(4)}, {"status": "TRACED", "bindings": bindings}
    literal = _STRING.fullmatch(current)
    if literal:
        if "\\" in literal.group(2):
            return None, {"status": "UNRESOLVED", "reason": "UNSUPPORTED_ESCAPED_STRING", "expression": current,
                          "bindings": bindings}
        return {"kind": "string", "value": literal.group(2)}, {"status": "TRACED", "bindings": bindings}
    if _NUMBER.fullmatch(current):
        return {"kind": "number", "value": current}, {"status": "TRACED", "bindings": bindings}
    return None, {"status": "UNRESOLVED", "reason": "DYNAMIC_EXPRESSION", "expression": current, "bindings": bindings}


def resolve(ctx, references, tables, calls, scripts):
    """Annotate symbolic references with proven constant-flow links.

    Existing resolved references are left intact. Rows whose flow is dynamic,
    ambiguous, missing, or maps to no exact table row remain UNRESOLVED with a
    concise trace diagnostic. ``tables`` must be the existing atlas table rows.
    """
    result = copy.deepcopy(references)
    calls_by_id = {x.get("id"): x for x in calls}
    calls_by_script = {}
    for call in calls:
        calls_by_script.setdefault(call.get("script_id"), []).append(call)
    scripts_by_id = {x.get("id"): x for x in scripts}
    rows_by_key = {}
    for row in tables:
        rows_by_key.setdefault((row.get("table"), row.get("name")), []).append(row)

    for ref in result:
        # Rebuild only annotations owned by this resolver. Other tools' edges
        # and the original reference row stay unchanged.
        existing_edges = ref.get("derived_edges")
        has_owned_annotation = isinstance(existing_edges, list) and any(
            isinstance(edge, dict) and edge.get("resolver") == _RESOLVER_ID for edge in existing_edges)
        has_owned_annotation = has_owned_annotation or (
            (ref.get("symbolic_trace") or {}).get("resolver") == _RESOLVER_ID)
        if isinstance(existing_edges, list):
            kept = [edge for edge in existing_edges if edge.get("resolver") != _RESOLVER_ID]
            if kept:
                ref["derived_edges"] = kept
            else:
                ref.pop("derived_edges", None)
        if (ref.get("symbolic_trace") or {}).get("resolver") == _RESOLVER_ID:
            ref.pop("symbolic_trace", None)
        if ref.get("kind") != "symbolic_reference" or (ref.get("status") == "RESOLVED" and not has_owned_annotation):
            continue
        context = ref.get("context") or {}
        call = calls_by_id.get(context.get("call_id"))
        script = scripts_by_id.get(context.get("script_id") or (call or {}).get("script_id"))
        if not call or not script or script.get("platform") != "android":
            continue
        api = (call.get("name") or "").split(":")[-1].split(".")[-1]
        contract = _API_NAMESPACE.get(api)
        string_contract = _API_STRING_MEMBER.get(api)
        if not contract and not string_contract:
            continue
        if contract:
            arg_index, expected_namespace, table_name, address = contract
        else:
            arg_index, expected_namespace, address = string_contract
            table_name = None
        if context.get("argument_index") != arg_index:
            continue
        argument = next((a for a in call.get("arguments", []) if a.get("index") == arg_index), {})
        trace, detail = _trace(ctx, script, calls_by_script, argument.get("expression", ref.get("name", "")), call.get("line", 0))
        evidence = {
            "api": api,
            "api_argument": arg_index,
            "native_consumer": address,
            "expected_namespace": expected_namespace,
            "bindings": detail.get("bindings", []),
        }
        if not trace:
            _store_trace(ref, {**evidence, **detail})
            continue
        if string_contract:
            if trace["kind"] != "string":
                _store_trace(ref, {**evidence, **detail, "status": "UNRESOLVED", "reason": "API_REQUIRES_SOUND_MEMBER_NAME", "terminal": trace})
                continue
            # Native code proves the value is submitted as a Sounds member
            # name. This edge does not claim that the label exists in the
            # supplied sound catalog or resolves to an audible file.
            ref.setdefault("derived_edges", []).append({
                "id": _edge_id(ref, "native-string-member", [expected_namespace, trace["value"]]),
                "resolver": _RESOLVER_ID,
                "type": "native-string-member",
                "namespace": expected_namespace,
                "name": trace["value"],
                "status": "PROVEN",
                "confidence": "PROVEN",
                "rule": "single Lua binding -> literal string -> native Sounds member lookup",
                "evidence": {**evidence, **detail, "terminal": trace},
            })
            _store_trace(ref, {**evidence, **detail, "status": "PROVEN", "terminal": trace})
            continue
        if trace["kind"] != "pydata":
            _store_trace(ref, {**evidence, **detail, "status": "UNRESOLVED", "reason": "API_NAMESPACE_REQUIRES_PYDATA_ROW", "terminal": trace})
            continue
        if trace["namespace"] != expected_namespace:
            _store_trace(ref, {**evidence, **detail, "status": "UNRESOLVED", "reason": "NAMESPACE_MISMATCH", "terminal": trace})
            continue
        candidates = rows_by_key.get((table_name, trace["name"]), [])
        if len(candidates) != 1:
            _store_trace(ref, {**evidence, **detail, "status": "UNRESOLVED", "reason": "NO_EXACT_OR_AMBIGUOUS_TABLE_ROW", "terminal": trace, "candidate_count": len(candidates)})
            continue
        row = candidates[0]
        # The edge is derived from an exact binding plus an explicit native
        # consumer/table-registration match. Keep the underlying reference's
        # status untouched; this module contributes a separately auditable edge.
        ref.setdefault("derived_edges", []).append({
            "id": _edge_id(ref, "table-row", [table_name, row.get("id")]),
            "resolver": _RESOLVER_ID,
            "type": "table-row",
            "table": table_name,
            "index": row.get("index"),
            "name": row.get("name"),
            "target_row_id": row.get("id"),
            "status": "PROVEN",
            "confidence": "PROVEN",
            "rule": "single Lua binding -> exact GetPyOID namespace/name -> native API argument namespace -> unique exact source table row",
            "evidence": {**evidence, **detail, "terminal": trace, "table_source": row.get("source_path")},
        })
        _store_trace(ref, {**evidence, **detail, "status": "PROVEN", "terminal": trace, "target_row_id": row.get("id")})
    return result
