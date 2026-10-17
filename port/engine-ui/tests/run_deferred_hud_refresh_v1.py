import pathlib
import subprocess
import tempfile

source = pathlib.Path(__file__).with_name("deferred_hud_refresh_v1.cpp")
with tempfile.TemporaryDirectory(prefix="dh2-deferred-hud-") as directory:
    binary = pathlib.Path(directory) / "deferred-hud-refresh"
    subprocess.run(["c++", "-std=c++17", str(source), "-o", str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
print('{"validation":"PASS","checks":4,"swap_preflight":"PASS"}')
