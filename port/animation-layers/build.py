#!/usr/bin/env python3
"""Build diagnostic absolute-pose layers on a host."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parent
SOURCES=[ROOT/'layers.cpp',ROOT/'../animation-mixing/mixing.cpp',ROOT/'../animation-pose/pose.cpp',ROOT/'../animation-values/values.cpp',ROOT/'../skin-payloads/skin.cpp',ROOT/'../asset-payloads/payloads.cpp',
         ROOT/'../scene-payloads/scene.cpp',ROOT/'../engine-resources/resources.cpp',ROOT/'../engine-math/math.cpp']
def main():
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path,default=ROOT/'build')
    p.add_argument('--report',type=Path,default=ROOT/'build-validation.json')
    for key in ('first','second','model'):p.add_argument('--'+key,type=Path)
    a=p.parse_args()
    fixtures=(a.first,a.second,a.model)
    if any(fixtures) and not all(fixtures):p.error('--first, --second and --model must be used together')
    a.output.mkdir(parents=True,exist_ok=True);binary=a.output/('layers-host.dll'if os.name=='nt'else'layers-host.so')
    subprocess.run([os.environ.get('CXX','c++'),'-std=c++17','-O2','-Wall','-Wextra','-Werror',
                    '-fPIC','-fno-exceptions','-fno-rtti','-fno-fast-math','-ffp-contract=off',
                    '-shared',*map(str,SOURCES),'-lm','-o',str(binary)],check=True)
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    headers=[p.with_suffix('.hpp')for p in SOURCES]
    result={'host_build':True,'complete_engine':False,'original_animator_equivalence':False,
            'source_sha256':{str(p.resolve().relative_to(ROOT.parent)):sha(p)for p in [*SOURCES,*headers]},
            'artifact_sha256':{binary.name:sha(binary)}}
    if all(fixtures):
        safety=a.output/'layers-safety'
        subprocess.run([os.environ.get('CXX','c++'),'-std=c++17','-O1','-g',
            '-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',
            *map(str,SOURCES),str(ROOT/'tests/safety.cpp'),'-lm','-o',str(safety)],check=True)
        subprocess.run([str(safety.resolve()),*map(str,fixtures)],check=True)
        result['safety']={'address_sanitizer':True,'undefined_behavior_sanitizer':True,
            'corruption_truncation_probes':3000,'seed':20261002,
            'fixture_sha256':{key:sha(path)for key,path in zip(('first','second','model'),fixtures)},
            'test_sha256':sha(ROOT/'tests/safety.cpp')}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
