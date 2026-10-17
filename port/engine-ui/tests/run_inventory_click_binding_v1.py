import pathlib
import subprocess
import tempfile

root = pathlib.Path(__file__).resolve().parents[3]
source = pathlib.Path(__file__).with_name("inventory_click_binding_v1.cpp")
with tempfile.TemporaryDirectory(prefix="dh2-inventory-click-") as directory:
    binary = pathlib.Path(directory) / "inventory-click-binding"
    subprocess.run(["c++", "-std=c++17", str(source), "-o", str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
print('{"validation":"PASS","checks":13}')
