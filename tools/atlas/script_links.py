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
    "SpawnSkillProjectile": (1, "ProjectileDict", "projectiles_dictionary#group0", "0x003b99d0"),
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
    # Count every plain assignment token, including assignments inside
    # conditional/inline statements and unsupported expressions. They make a
    # binding ambiguous even when only one assignment has a simple RHS.
    for index, token in enumerate(tokens[:-1]):
        if token[0] != "id" or tokens[index + 1][1] != "=":
            continue
        if index + 2 < len(tokens) and tokens[index + 2][1] == "=":
            continue
        if index and tokens[index - 1][1] in (".", ":"):
            continue
        name, line_no = token[1], token[2]
        source_line = lines[line_no - 1]
        m = re.fullmatch(r"\s*(?:local\s+)?([A-Za-z_][A-Za-z_0-9]*)\s*=\s*(.*?)\s*;?\s*", source_line)
        top_level = source_line == source_line.lstrip()
        rhs = m.group(2) if top_level and m and m.group(1) == name else ""
        if not (_GET_PYOID.fullmatch(rhs) or _IDENTIFIER.fullmatch(rhs) or _STRING.fullmatch(rhs) or _NUMBER.fullmatch(rhs)):
            rhs = ""
        found.setdefault(name, []).append({"line": line_no, "rhs": rhs, "source": source_line.strip()})
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
        if len(all_options) != 1 or len(options) != 1:
            # A global imported literal may be consumed without a local copy.
            imports = _included_sources(ctx, script, calls_by_script)
            imported = []
            for path in imports:
                candidate = _assignments(path.read_text(encoding="utf-8-sig", errors="replace")).get(current, [])
                # A global binding only: imported locals are not visible.
                for item in candidate:
                    line_text = path.read_text(encoding="utf-8-sig", errors="replace").splitlines()[item["line"] - 1].lstrip()
                    if line_text == line_text.lstrip() and not line_text.startswith("local "):
                        imported.append((path, item, candidate))
            if len(imported) != 1 or len(imported[0][2]) != 1:
                return None, {"status": "UNRESOLVED", "reason": "MISSING_OR_AMBIGUOUS_BINDING", "variable": current, "bindings": bindings}
            path, item, _ = imported[0]
            source_site = ".local-inputs/" + path.relative_to(Path(ctx.inputs)).as_posix()
        else:
            item = options[0]
            path = source
            source_site = site
        bindings.append({"variable": current, "source": source_site, "line": item["line"], "expression": item["rhs"], "text": item["source"]})
        current = item["rhs"].strip().rstrip(";").strip()
        # Follow exact GetPyOID calls as symbolic table/name pairs.
        query = _GET_PYOID.fullmatch(current)
        if query:
            return {"kind": "pydata", "namespace": query.group(2), "name": query.group(4)}, {"status": "TRACED", "bindings": bindings}
        literal = _STRING.fullmatch(current)
        if literal:
            return {"kind": "string", "value": literal.group(2)}, {"status": "TRACED", "bindings": bindings}
        if _NUMBER.fullmatch(current):
            return {"kind": "number", "value": current}, {"status": "TRACED", "bindings": bindings}
    query = _GET_PYOID.fullmatch(current)
    if query:
        return {"kind": "pydata", "namespace": query.group(2), "name": query.group(4)}, {"status": "TRACED", "bindings": bindings}
    literal = _STRING.fullmatch(current)
    if literal:
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
        if isinstance(existing_edges, list):
            kept = [edge for edge in existing_edges if edge.get("resolver") != _RESOLVER_ID]
            if kept:
                ref["derived_edges"] = kept
            else:
                ref.pop("derived_edges", None)
        if (ref.get("symbolic_trace") or {}).get("resolver") == _RESOLVER_ID:
            ref.pop("symbolic_trace", None)
        if ref.get("kind") != "symbolic_reference" or ref.get("status") == "RESOLVED":
            continue
        context = ref.get("context") or {}
        call = calls_by_id.get(context.get("call_id"))
        script = scripts_by_id.get(context.get("script_id") or (call or {}).get("script_id"))
        if not call or not script:
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
