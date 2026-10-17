import pathlib
import subprocess
import tempfile

source = pathlib.Path(__file__).with_name("skill_ui_binding_v1.cpp")
with tempfile.TemporaryDirectory(prefix="dh2-skill-ui-") as directory:
    binary = pathlib.Path(directory) / "skill-ui-binding"
    subprocess.run(["c++", "-std=c++17", str(source), "-o", str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
print('{"validation":"PASS","checks":4}')
