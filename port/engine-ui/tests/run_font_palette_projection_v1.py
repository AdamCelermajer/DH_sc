import pathlib
import hashlib
import json
import subprocess
import tempfile

root = pathlib.Path(__file__).resolve().parents[3]
assets = root / "port/android-native/app/src/main/assets/source-data/font-palette"
manifest = json.loads((assets / "PROVENANCE.json").read_text(encoding="utf-8"))
for name, expected in manifest["files"].items():
    payload = (assets / name).read_bytes()
    if len(payload) != expected["size_bytes"] or hashlib.sha256(payload).hexdigest() != expected["sha256"]:
        raise SystemExit(f"Source cache fixture provenance mismatch: {name}")
source = pathlib.Path(__file__).with_name("font_palette_projection_v1.cpp")
constants = root / "port/pydata-constants/constants.c"
with tempfile.TemporaryDirectory(prefix="dh2-font-palette-") as directory:
    directory = pathlib.Path(directory)
    object_file = directory / "constants.o"
    binary = directory / "font-palette-projection"
    subprocess.run(["gcc", "-c", str(constants), "-o", str(object_file)], check=True)
    subprocess.run(["c++", "-std=c++17", str(source), str(object_file), "-o", str(binary)], check=True)
    subprocess.run([str(binary), str(assets)], check=True)
