import hashlib
import json
from pathlib import Path

root = Path(__file__).resolve().parents[4]
manifest = json.loads((root / "coordination/v20/GATE-TELEPORT-FX-assets.json").read_text())
projection = json.loads((root / "port/game-data/reference/effects-tables/original-reader-projection.json").read_text())

assert projection["names"][0][132] == manifest["source_table"]["set_name"]
assert projection["dictionary_paths"][56] == manifest["source_table"]["step"]["uri"]
set132 = projection["sets"][132]
assert len(set132["steps"]) == manifest["source_table"]["step_count"] == 1
step = set132["steps"][0]
for source_field, projection_field in (
    ("OrientWithAnchor", "orient_with_anchor"),
    ("ScaleWithAnchor", "scale_with_anchor"),
    ("SelfIllum", "self_illum"),
    ("PlayTime", "play_time"),
    ("Loop", "loop"),
    ("subobject", "subobject"),
):
    assert step[projection_field] == manifest["source_table"]["step"][source_field]
assert step["file"] == manifest["source_table"]["step"]["dictionary_index"]
assert step["speed_bits"] == 1065353216  # IEEE-754 1.0f, exact source table value.

for row in manifest["required_original_files"]:
    for key in ("source_path", "staged_path"):
        path = root / row[key]
        content = path.read_bytes()
        assert len(content) == row["bytes"], f"size mismatch: {path}"
        assert hashlib.sha256(content).hexdigest().upper() == row["sha256"], f"hash mismatch: {path}"
    assert hashlib.sha256((root / row["source_path"]).read_bytes()).digest() == \
           hashlib.sha256((root / row["staged_path"]).read_bytes()).digest()
    print(f"PASS | exact source/stage asset {row['uri']} ({row['bytes']} bytes)")

# The persistent BlueDoor model is a visual candidate, separate from set 132's
# guaranteed dependency closure. Pin its original files without asserting a
# pixel-to-node match with the v1.0.3 footage.
gate = manifest["observed_persistent_gate_candidate"]
assert gate["source_object"] == "_prim_BlueDoor"
assert gate["resolved_model_uri"].endswith("go_swamp_door_ruins_fire.bdae")
for row in gate["original_dependencies"]:
    for key in ("source_path", "staged_path"):
        path = root / row[key]
        content = path.read_bytes()
        assert len(content) == row["bytes"], f"size mismatch: {path}"
        assert hashlib.sha256(content).hexdigest().upper() == row["sha256"], f"hash mismatch: {path}"
    assert hashlib.sha256((root / row["source_path"]).read_bytes()).digest() == \
           hashlib.sha256((root / row["staged_path"]).read_bytes()).digest()
    print(f"PASS | exact source/stage gate candidate {row['uri']} ({row['bytes']} bytes)")

print("PASS | set 132 projection matches the finite I028 asset manifest")
