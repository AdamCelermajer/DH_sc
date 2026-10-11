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

def verify_fx_set(row):
    index = row["set_index"]
    assert projection["names"][0][index] == row["set_name"]
    source_set = projection["sets"][index]
    assert len(source_set["steps"]) == row["step_count"] == 1
    source_step = source_set["steps"][0]
    expected = row["step"]
    assert projection["dictionary_paths"][expected["dictionary_index"]] == expected["uri"]
    assert source_step["file"] == expected["dictionary_index"]
    for source_field, projection_field in (
        ("OrientWithAnchor", "orient_with_anchor"),
        ("ScaleWithAnchor", "scale_with_anchor"),
        ("SelfIllum", "self_illum"),
        ("PlayTime", "play_time"),
        ("Loop", "loop"),
        ("subobject", "subobject"),
    ):
        assert source_step[projection_field] == expected[source_field]
    assert source_step["speed_bits"] == 1065353216
    print(f"PASS | exact EffectsTables row {index}: {row['set_name']}")

prison = manifest["prisoner_cage_transition"]
for row in prison["transition_cues"]:
    verify_fx_set(row)

for row in manifest["required_original_files"]:
    for key in ("source_path", "staged_path"):
        path = root / row[key]
        content = path.read_bytes()
        assert len(content) == row["bytes"], f"size mismatch: {path}"
        assert hashlib.sha256(content).hexdigest().upper() == row["sha256"], f"hash mismatch: {path}"
    assert hashlib.sha256((root / row["source_path"]).read_bytes()).digest() == \
           hashlib.sha256((root / row["staged_path"]).read_bytes()).digest()
    print(f"PASS | exact source/stage asset {row['uri']} ({row['bytes']} bytes)")

# The Witch/Swamp fire-door family is separate from the P1 prisoner-cage spill.
# Pin its original files without asserting a pixel-to-node match.
gate = manifest["related_fire_door_asset_family"]
assert gate["source_object"] == "_prim_BlueDoor"
assert gate["resolved_model_uri"].endswith("go_swamp_door_ruins_fire.bdae")

def verify_assets(rows, label):
    for row in rows:
        for key in ("source_path", "staged_path"):
            path = root / row[key]
            content = path.read_bytes()
            assert len(content) == row["bytes"], f"size mismatch: {path}"
            assert hashlib.sha256(content).hexdigest().upper() == row["sha256"], f"hash mismatch: {path}"
        assert hashlib.sha256((root / row["source_path"]).read_bytes()).digest() == \
               hashlib.sha256((root / row["staged_path"]).read_bytes()).digest()
        print(f"PASS | exact source/stage {label} asset {row['uri']} ({row['bytes']} bytes)")

verify_assets(gate["original_dependencies"], "related fire-door")
verify_assets(prison["cage_model_dependencies"], "prisoner-cage model")
for row in prison["transition_cues"]:
    verify_assets(row["dependencies"], f"set {row['set_index']} transition")

print("PASS | exact set 132, 119, and 120 projections and finite I028 asset manifest")
