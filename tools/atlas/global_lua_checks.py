"""Read-only regression checks for global Android Lua link and semantic contracts."""
from __future__ import annotations

import copy
import json
import tempfile
from pathlib import Path
from types import SimpleNamespace

import lua_semantics
import script_links
from common import read_json


def _ok(name, passed, observed=None, expected=None):
    return {"name": name, "passed": bool(passed), "observed": observed, "expected": expected}


def _fixture(ctx, source, *, call="PlayFX", expression="OID", argument_index=1,
             platform="android", table_rows=None, call_mode="global"):
    site = ".local-inputs/assets-extra/android/data/scripts/check.lua"
    path = ctx.inputs / site.removeprefix(".local-inputs/")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(source, encoding="utf-8")
    scripts = [{"id": "script-1", "platform": platform, "site": site, "imports": {}}]
    calls = [{"id": "call-1", "script_id": "script-1", "name": call, "line": source.count("\n") + 1,
              "call_mode": call_mode, "arguments": [{"index": argument_index, "expression": expression}]}]
    refs = [{"id": "ref-1", "kind": "symbolic_reference", "status": "UNRESOLVED", "name": expression,
             "context": {"call_id": "call-1", "script_id": "script-1", "argument_index": argument_index}}]
    return refs, calls, scripts, table_rows or []


def _fixture_link_checks():
    checks = []
    with tempfile.TemporaryDirectory(prefix="atlas-lua-check-") as root:
        ctx = SimpleNamespace(inputs=Path(root))
        unique = [{"id": "row-fx", "table": "effect_sets", "index": 7, "name": "Fx",
                   "source_path": ".local-inputs/test.bin"}]
        source = 'local OID = GetPyOID("AnimatedEffectTable", "Fx")\nPlayFX(OID)\n'

        refs, calls, scripts, rows = _fixture(ctx, source, table_rows=unique)
        linked = script_links.resolve(ctx, refs, rows, calls, scripts)
        checks.append(_ok("unique exact AnimatedEffectTable row resolves", len(linked[0].get("derived_edges", [])) == 1,
                          linked[0].get("derived_edges"), "one PROVEN effect_sets row edge"))
        resolved_owner = copy.deepcopy(linked[0])
        resolved_owner["status"] = "RESOLVED"  # models the semantic resolver promoting this same row
        revalidated = script_links.resolve(ctx, [resolved_owner], rows, calls, scripts)[0]
        checks.append(_ok("owned edge survives a chained resolved-reference rerun",
                          revalidated.get("status") == "RESOLVED" and
                          len([edge for edge in revalidated.get("derived_edges", [])
                               if edge.get("resolver") == "script_links.v1"]) == 1,
                          revalidated.get("derived_edges"), "one revalidated owned edge"))
        stale = script_links.resolve(ctx, [resolved_owner], rows + [dict(rows[0], id="duplicate", index=8)], calls, scripts)[0]
        checks.append(_ok("owned edge is revalidated against duplicate rows",
                          not any(edge.get("resolver") == "script_links.v1"
                                  for edge in stale.get("derived_edges", [])) and
                          stale.get("symbolic_trace", {}).get("reason") == "NO_EXACT_OR_AMBIGUOUS_TABLE_ROW",
                          stale.get("symbolic_trace", {}).get("reason"), "stale edge removed"))
        foreign = copy.deepcopy(refs[0])
        foreign["status"] = "RESOLVED"
        foreign["derived_edges"] = [{"id": "foreign-edge", "resolver": "other_resolver", "status": "PROVEN"}]
        untouched = script_links.resolve(ctx, [foreign], rows, calls, scripts)[0]
        checks.append(_ok("foreign resolved annotation is preserved", untouched == foreign,
                          untouched.get("derived_edges"), "original foreign edge retained"))

        refs, calls, scripts, animation_rows = _fixture(ctx,
            'local ID = GetPyOID("AnimDict", "Walk")\nRegisterAnim(ID)\n', call="RegisterAnim",
            expression="ID", table_rows=[{"id": "row-anim", "table": "animations_dictionary#group0",
                                           "index": 3, "name": "Walk"}])
        result = script_links.resolve(ctx, refs, animation_rows, calls, scripts)[0]
        checks.append(_ok("unique exact AnimDict row resolves", len(result.get("derived_edges", [])) == 1,
                          result.get("symbolic_trace", {}).get("status"), "PROVEN animation row edge"))

        for label, table_rows, reason in (
            ("missing exact row stays unresolved", [], "NO_EXACT_OR_AMBIGUOUS_TABLE_ROW"),
            ("duplicate exact rows stay unresolved", unique + [dict(unique[0], id="row-fx-duplicate", index=8)],
             "NO_EXACT_OR_AMBIGUOUS_TABLE_ROW"),
        ):
            refs, calls, scripts, _ = _fixture(ctx, source, table_rows=table_rows)
            result = script_links.resolve(ctx, refs, table_rows, calls, scripts)[0]
            checks.append(_ok(label, not result.get("derived_edges") and
                              result.get("symbolic_trace", {}).get("reason") == reason,
                              result.get("symbolic_trace", {}).get("reason"), reason))

        refs, calls, scripts, _ = _fixture(ctx,
            'local OID = GetPyOID("ClassTable", "Fx")\nPlayFX(OID)\n', table_rows=unique)
        result = script_links.resolve(ctx, refs, unique, calls, scripts)[0]
        checks.append(_ok("wrong namespace does not cross-link", not result.get("derived_edges") and
                          result.get("symbolic_trace", {}).get("reason") == "NAMESPACE_MISMATCH",
                          result.get("symbolic_trace", {}).get("reason"), "NAMESPACE_MISMATCH"))

        refs, calls, scripts, _ = _fixture(ctx, source, argument_index=2, table_rows=unique)
        # The call carries its authored first argument while the reference claims arg 2.
        calls[0]["arguments"] = [{"index": 1, "expression": "OID"}, {"index": 2, "expression": "OID"}]
        result = script_links.resolve(ctx, refs, unique, calls, scripts)[0]
        checks.append(_ok("wrong documented argument index is ignored", not result.get("symbolic_trace") and
                          not result.get("derived_edges"), result.get("symbolic_trace"), "no resolver annotation"))

        refs, calls, scripts, _ = _fixture(ctx, source, table_rows=unique, platform="ios")
        result = script_links.resolve(ctx, refs, unique, calls, scripts)[0]
        checks.append(_ok("Android native contracts do not classify iOS calls",
                          not result.get("symbolic_trace") and not result.get("derived_edges"),
                          result.get("symbolic_trace"), "no Android-derived annotation"))

        negative_sources = [
            ("branch assignment remains unknown", 'local OID = "Fx"\nif enabled then\n OID = GetPyOID("AnimatedEffectTable", "Fx")\nend\nPlayFX(OID)\n', "MULTIPLE_WRITES"),
            ("function-local assignment remains unknown", 'local OID = "Fx"\nfunction update()\n OID = GetPyOID("AnimatedEffectTable", "Fx")\nend\nPlayFX(OID)\n', "MULTIPLE_WRITES"),
            ("multiple assignment invalidates every LHS", 'local OID = GetPyOID("AnimatedEffectTable", "Fx")\nOID, other = "Fx", "Fx"\nPlayFX(OID)\n', "MULTIPLE_WRITES"),
            ("escaped GetPyOID strings remain unknown", 'local OID = GetPyOID("AnimatedEffectTable", "F\\x")\nPlayFX(OID)\n', "UNSUPPORTED_ESCAPED_GETPYOID_LITERAL"),
        ]
        for label, fixture_source, reason in negative_sources:
            refs, calls, scripts, _ = _fixture(ctx, fixture_source, table_rows=unique)
            result = script_links.resolve(ctx, refs, unique, calls, scripts)[0]
            checks.append(_ok(label, not result.get("derived_edges") and
                              result.get("symbolic_trace", {}).get("reason") == reason,
                              result.get("symbolic_trace", {}).get("reason"), reason))

        # Imports are followed only when a single non-local global declaration
        # appears in an explicitly projected source. Imported locals are invisible.
        caller_site = ".local-inputs/assets-extra/android/data/scripts/caller.lua"
        common_site = ".local-inputs/assets-extra/android/data/scripts/common.lua"
        (ctx.inputs / caller_site.removeprefix(".local-inputs/")).parent.mkdir(parents=True, exist_ok=True)
        (ctx.inputs / caller_site.removeprefix(".local-inputs/")).write_text("PlayFX(OID)\n", encoding="utf-8")
        (ctx.inputs / common_site.removeprefix(".local-inputs/")).write_text(
            'OID = GetPyOID("AnimatedEffectTable", "Fx")\n', encoding="utf-8")
        refs, calls, scripts, _ = _fixture(ctx, "PlayFX(OID)\n", table_rows=unique)
        scripts[0].update(site=caller_site, imports={"common": common_site})
        result = script_links.resolve(ctx, refs, unique, calls, scripts)[0]
        checks.append(_ok("explicit imported global alias resolves", bool(result.get("derived_edges")),
                          result.get("symbolic_trace", {}).get("status"), "one PROVEN row edge"))
        for local_declaration in (
            'local OID = GetPyOID("AnimatedEffectTable", "Fx")\n',
            '\tlocal\tOID = GetPyOID("AnimatedEffectTable", "Fx")\n',
            'local OID\nOID = GetPyOID("AnimatedEffectTable", "Fx")\n',
            'local OID;\nOID = GetPyOID("AnimatedEffectTable", "Fx")\n',
        ):
            (ctx.inputs / common_site.removeprefix(".local-inputs/")).write_text(local_declaration, encoding="utf-8")
            result = script_links.resolve(ctx, refs, unique, calls, scripts)[0]
            checks.append(_ok("imported local alias stays invisible", not result.get("derived_edges"),
                              result.get("symbolic_trace", {}).get("reason"), "no row edge"))

    # Direct lexical fixtures guard against counting only the last identifier
    # in a tuple assignment, and against admitting nested/branch assignments.
    assignment_cases = {
        "both tuple-assignment names are invalidated": (
            'local x = "Fx"\nx, y = "Fx", "Fx"\n', {"x", "y"}),
    }
    for label, (source, names) in assignment_cases.items():
        observed = script_links._assignments(source)
        passed = all(observed.get(name) and observed[name][-1].get("multiple_lhs") for name in names)
        checks.append(_ok(label, passed, {name: observed.get(name) for name in names}, "tuple write recorded for each LHS"))

    for label, source in (
        ("if scope is not admitted", 'if ok then\n X = "Fx"\nend\n'),
        ("function scope is not admitted", 'function f()\n X = "Fx"\nend\n'),
        ("loop scope is not admitted", 'for i=1,2 do\n X = "Fx"\nend\n'),
    ):
        assignment = script_links._assignments(source).get("X", [{}])[0]
        checks.append(_ok(label, assignment.get("scope_depth", 0) > 0 and not assignment.get("rhs"),
                          assignment, "nested assignment is recorded but not admitted"))
    return checks


def run(ctx):
    """Run corpus-wide and synthetic resolver checks without writing outputs."""
    data = ctx.out / "data"
    # Canonical files are the joined source ledgers. The `*-extra` projections
    # intentionally overlap them and must not be concatenated as new rows.
    references = read_json(data / "references.json", [])
    tables = read_json(data / "tables.json", [])
    calls = read_json(data / "calls.json", [])
    scripts = read_json(data / "scripts.json", [])
    input_snapshot = json.dumps(references, ensure_ascii=False, sort_keys=True, separators=(",", ":"))

    linked = script_links.resolve(ctx, references, tables, calls, scripts)
    linked_again = script_links.resolve(ctx, linked, tables, calls, scripts)
    semantic = lua_semantics.resolve(ctx, linked, tables, calls, scripts)
    semantic_again = lua_semantics.resolve(ctx, semantic, tables, calls, scripts)
    checks = [
        _ok("script-link resolver is deterministic and idempotent", linked == linked_again),
        _ok("Lua-semantic resolver is deterministic and idempotent", semantic == semantic_again),
        _ok("resolver inputs remain unchanged", input_snapshot == json.dumps(references, ensure_ascii=False,
             sort_keys=True, separators=(",", ":"))),
    ]
    constants, constant_sources = lua_semantics._pycst_groups(ctx)
    checks.append(_ok("registered Android cache groups include exact Elemental.Water provenance",
                      "Water" in constants.get("Elemental", {}) and ("Elemental", "Water") in constant_sources,
                      constant_sources.get(("Elemental", "Water")), "registered source cache path"))

    link_edges = [edge for row in linked for edge in row.get("derived_edges", [])
                  if edge.get("resolver") == "script_links.v1" and edge.get("status") == "PROVEN"]
    checks.extend([
        _ok("global ClassTable consumer rows linked", any(edge.get("table") == "character_classes#group0" for edge in link_edges),
            sum(edge.get("table") == "character_classes#group0" for edge in link_edges), "> 0"),
        _ok("global ProjectileTable consumer rows linked", any(edge.get("table") == "projectiles#group0" for edge in link_edges),
            sum(edge.get("table") == "projectiles#group0" for edge in link_edges), "> 0"),
        _ok("global effect consumers linked; animation contract covered by exact fixture",
            any(edge.get("table") == "effect_sets" for edge in link_edges),
            sorted({str(edge.get("table")) for edge in link_edges}), "effect_sets"),
        _ok("global schema, constant, callback, and scalar classes present",
            all(any(row.get("reference_kind") == kind and
                    row.get("semantic_resolution", {}).get("resolver") == "lua_semantics.v1"
                    for row in semantic) for kind in
                ("table_field", "enum_constant", "callback_method", "skill_callback", "skill_callback_table",
                 "native_scalar_argument")),
            sorted({str(row.get("reference_kind")) for row in semantic
                    if row.get("semantic_resolution", {}).get("resolver") == "lua_semantics.v1"}),
            "all listed semantic classes"),
    ])

    # Confirm coverage is genuinely global across the indexed script groups.
    successful = [row for row in semantic if row.get("semantic_resolution", {}).get("resolver") == "lua_semantics.v1"]
    script_groups = {str(row.get("context", {}).get("script", "")).split("/data/scripts/")[-1].split("/")[0]
                     for row in successful if "/data/scripts/" in str(row.get("context", {}).get("script", ""))}
    checks.append(_ok("semantic examples span AI, skills, and level scripts",
                      {"ai", "skills", "level"}.issubset(script_groups), sorted(script_groups),
                      ["ai", "skills", "level"]))
    checks.extend(_fixture_link_checks())
    return {"passed": all(check["passed"] for check in checks), "checks": checks,
            "counts": {"references": len(references), "calls": len(calls), "scripts": len(scripts),
                       "script_link_edges": len(link_edges), "semantic_resolutions": len(successful)}}
