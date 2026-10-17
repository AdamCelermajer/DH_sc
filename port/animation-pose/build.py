#!/usr/bin/env python3
"""Build the bounded absolute transform preview component on a host."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parent
SOURCES=[ROOT/'pose.cpp',ROOT/'../animation-values/values.cpp',ROOT/'../skin-payloads/skin.cpp',ROOT/'../asset-payloads/payloads.cpp',
         ROOT/'../scene-payloads/scene.cpp',ROOT/'../engine-resources/resources.cpp',ROOT/'../engine-math/math.cpp']
def main():
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path,default=ROOT/'build')
    p.add_argument('--report',type=Path,default=ROOT/'build-validation.json');a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=True);binary=a.output/('pose-host.dll'if os.name=='nt'else'pose-host.so')
    subprocess.run([os.environ.get('CXX','c++'),'-std=c++17','-O2','-Wall','-Wextra','-Werror',
                    '-fPIC','-fno-exceptions','-fno-rtti','-fno-fast-math','-ffp-contract=off',
                    '-shared',*map(str,SOURCES),'-lm','-o',str(binary)],check=True)
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    headers=[p.with_suffix('.hpp')for p in SOURCES]
    result={'host_build':True,'complete_engine':False,'original_animator_equivalence':False,
            'source_sha256':{str(p.resolve().relative_to(ROOT.parent)):sha(p)for p in [*SOURCES,*headers]},
            'artifact_sha256':{binary.name:sha(binary)}}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
