"""Generate read-only asset fixtures with independent B112 save/capture directories.

Run with py -3 -I; execute jobs.json only through quiet_run.ps1.
"""
import argparse
import json
from pathlib import Path

p = argparse.ArgumentParser()
p.add_argument("--workspace", required=True, type=Path)
p.add_argument("--exe", required=True, type=Path)
p.add_argument("--stage", required=True)
args = p.parse_args()
inputs = args.workspace / ".local-inputs"
source = inputs / "v20.1/root/initial-runtime/npc/run.args"
assets = inputs / "claude-v20.1/fix100/assets"
assert source.is_file() and assets.is_dir() and args.exe.is_file()
lines = source.read_text().splitlines()
removed = {"--assets", "--position", "--capture", "--capture-frames", "--frames",
           "--game-save", "--save", "--space-key-interval", "--window-size"}
base, i = [], 0
while i < len(lines):
    if lines[i] in removed:
        i += 2
    else:
        base.append(lines[i])
        i += 1
jobs = []
for dims in ("1280,720", "1024,768"):
    for case, pos, interact in (
        ("quest-near", "-2427,6663,257", True),
        ("quest-distant", "-2900,6500,257", False),
        ("merchant-near", "-10900,12000,255", True),
        ("merchant-distant", "-10900,11400,255", False),
    ):
        name = f"{args.stage}-{case}-{dims.replace(',', 'x')}"
        out = inputs / "v20.1/b112/runtime" / name
        out.mkdir(parents=True, exist_ok=False)
        config = base + ["--assets", str(assets), "--position", pos,
                         "--frames", "650" if interact else "400",
                         "--window-size", dims, "--capture", str(out / "last.ppm"),
                         "--capture-frames", "260,340,580" if interact else "260,340",
                         "--game-save", str(out / "gameplay.save"),
                         "--save", str(out / "profile.save")]
        if interact:
            config += ["--space-key-interval", "300:3", "--space-key-interval", "550:3"]
        run_args = out / "run.args"
        run_args.write_text("\n".join(config) + "\n")
        jobs.append(dict(name=name, exe=str(args.exe), args=["--startup-config", str(run_args)],
                         cwd=str(out), log=str(out / "run.log"), timeoutSec=100))
dest = inputs / "v20.1/b112" / f"jobs-{args.stage}.json"
dest.write_text(json.dumps(jobs, indent=2))
print(dest)
