"""Replay the reached original bodies after Main restores process admission.
No GPU/final deletion or real renderer ownership is asserted by these fixtures.
Never overwrites frozen source results; --out must be a new directory.
"""
from pathlib import Path
import argparse, sys, struct, json, hashlib
ap=argparse.ArgumentParser()
ap.add_argument('--loader',type=Path,required=True)
ap.add_argument('--engine',type=Path,required=True)
ap.add_argument('--python-site',type=Path,required=True)
ap.add_argument('--out',type=Path,required=True)
a=ap.parse_args()
assert hashlib.sha256(a.engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert not a.out.exists(), 'Preserve previous proof outputs; supply a new directory'
a.out.mkdir(parents=True)
sys.path.insert(0,str(a.python_site))
sys.path.insert(0,str(a.loader/'tests'))
sys.path.insert(0,str(a.loader.parent/'game-data/tests'))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
b=a.loader;e=a.engine;r=a.out
here=Path(__file__).resolve().parent
# Preserve copied original definitions verbatim; adapt only imports/output paths.
s=(here/'dh2-lightpoint-dtor-proof-v55.py').read_text()
exec(s[s.index('class Dtor(Parser):'):],globals())
s=(here/'dh2-light-reset-proof-v55.py').read_text()
exec(s[s.index('class Reset(Parser):'):],globals())
s=(here/'dh2-lightset-assignment-v55.py').read_text()
exec(s[s.index('class Assign(Parser):'):],globals())
assert json.loads((r/'original-lightpoint-dtor-v55.json').read_text())['validation']=='PASS'
assert json.loads((r/'original-lightset-reset-v55.json').read_text())['validation']=='PASS'
assert json.loads((r/'original-lightset-assignment-v55.json').read_text())['validation']=='PASS'
print('PASS: original C1/Dtor, real IReferenceCounted.drop, ResetLightSets, assignment bodies. No runtime/GPU readiness claim.')
