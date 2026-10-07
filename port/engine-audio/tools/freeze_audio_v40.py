"""Freeze NEW V40 packet after all owned work completes; never edits earlier handoffs."""
from pathlib import Path
import hashlib,json,re
root=Path(__file__).resolve().parents[3]
dest=root/'handoff/audio-v40'
if (dest/'manifest.json').exists(): raise SystemExit('V40 already frozen; use a new version')
owned=set()
for p in (root/'port/engine-audio/integration-v40').rglob('*'):
 if not p.is_file() or any(x in {'build','__pycache__','.git'} for x in p.parts):continue
 if p.suffix.lower() in {'.o','.class','.exe','.so','.a','.png','.wav','.vxn'}:continue
 owned.add(p.resolve())
for pattern in ['audio_*v40.*','tests/*v40*','tools/*v40*']:
 owned.update(p.resolve() for p in (root/'port/engine-audio').glob(pattern) if p.is_file())
dependencies=set();pending=list(owned)
while pending:
 p=pending.pop()
 if p.suffix not in {'.cpp','.hpp','.h','.inc'}:continue
 for include in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(errors='replace'),re.M):
  candidate=(p.parent/include).resolve()
  if not candidate.is_file():continue
  if candidate not in owned and candidate not in dependencies:
   dependencies.add(candidate);pending.append(candidate)
def rows(paths):
 return [dict(path=p.relative_to(root).as_posix(),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sorted(paths)]
prior=[root/'handoff'/v/'manifest.json'for v in ['authored-fx-v32','audio-v34','audio-v38','audio-audit-v39']]
result=dict(version=40,scope='Independent helpers, source/CPU proofs and captured prospective integration patches; not applied/shared build/device/audible acceptance',owned=rows(owned),dependencies=rows(dependencies),frozen_prior_handoffs=rows(prior),acceptance=dict(source_exact=True,cpu_decoded_output=True,shared_app_installed=False,actual_world_ready=False,actual_aaudio=False,all_categories_audible=False))
dest.mkdir(parents=True,exist_ok=True)
(dest/'manifest.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(dict(owned=len(owned),dependencies=len(dependencies),manifest=str(dest/'manifest.json'))))
