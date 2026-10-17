#!/usr/bin/env python3
"""Compile and run the platform-independent Android preview layout policy test."""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parents[1]
POLICY = HERE / 'src/local/dh2/sourceviewer/ResponsiveLayoutPolicy.java'
TEST = HERE / 'tests/ResponsiveLayoutPolicyTest.java'


def resolve_tool(argument: Path | None, name: str) -> Path:
    if argument:
        return argument.resolve(strict=True)
    java_home = os.environ.get('JAVA_HOME')
    candidate = Path(java_home) / 'bin' / (name + ('.exe' if os.name == 'nt' else '')) if java_home else None
    if candidate and candidate.is_file():
        return candidate
    found = shutil.which(name)
    if found:
        return Path(found)
    raise FileNotFoundError(f'{name} not found; set JAVA_HOME or pass --{name}')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--javac', type=Path)
    parser.add_argument('--java', type=Path)
    parser.add_argument('--classes', type=Path,
                        default=HERE / 'build/responsive-layout-policy-test/classes')
    args = parser.parse_args()
    javac, java = resolve_tool(args.javac, 'javac'), resolve_tool(args.java, 'java')
    args.classes.mkdir(parents=True, exist_ok=True)
    subprocess.run([str(javac), '-Xlint:-options', '-source', '8', '-target', '8',
                    '-d', str(args.classes.resolve()), str(POLICY), str(TEST)], check=True)
    subprocess.run([str(java), '-cp', str(args.classes.resolve()),
                    'local.dh2.sourceviewer.ResponsiveLayoutPolicyTest'], check=True)


if __name__ == '__main__':
    main()
