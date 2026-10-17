import argparse
from pathlib import Path
import shutil
import subprocess

tests = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default=shutil.which("g++") or "g++")
parser.add_argument("--output-dir", type=Path, default=tests / "build/merchant-item-details-gate-v1")
args = parser.parse_args()
compiler = shutil.which(args.compiler) or args.compiler
args.output_dir.mkdir(parents=True, exist_ok=True)
binary = args.output_dir.resolve() / "merchant_item_details_gate_v1.exe"
subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
                str(tests / "merchant_item_details_gate_v1.cpp"), "-o", str(binary)],
               check=True)
subprocess.run([str(binary)], check=True)
