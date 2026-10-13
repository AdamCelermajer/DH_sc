"""Exact source-derived coverage for the script projection families."""
import json
from pathlib import Path


def coverage_check(ctx):
    """Compare source-derived scripts/functions/calls/script refs by exact ID set."""
    from scripts import expected_ids

    expected = expected_ids(ctx)
    errors = []
    for key, filename in (("scripts", "scripts.json"),
                          ("functions", "functions.json"),
                          ("calls", "calls.json"),
                          ("references", "script-references.json")):
        path = Path(ctx.out) / "data" / filename
        saved = json.loads(path.read_text(encoding="utf-8")) if path.is_file() else []
        actual = {row.get("id") for row in saved}
        wanted = set(expected[key])
        missing = sorted(wanted - actual)
        extra = sorted(actual - wanted)
        if missing or extra:
            errors.append(f"{key}: source coverage differs (missing={len(missing)}, extra={len(extra)}; "
                          f"missing_ids={missing[:5]}, extra_ids={extra[:5]})")
    return errors
