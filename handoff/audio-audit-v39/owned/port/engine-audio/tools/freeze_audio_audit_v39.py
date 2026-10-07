"""Freeze only read-only audit artifacts; preserve root source snapshots as captured."""
from pathlib import Path
import json,hashlib,shutil
root=Path(__file__).resolve().parents[3];dest=root/'handoff/audio-audit-v39';dest.mkdir(parents=True,exist_ok=True)
owned=[]
for directory in ['port/engine-audio/reference/active-callers-v39','port/engine-audio/reference/archive-audit-v39']:
 owned +=[p for p in (root/directory).rglob('*')if p.is_file()and '__pycache__'not in p.parts]
owned +=[root/'port/engine-audio/reports/audio-active-source-review-v39.md',root/'port/engine-audio/reports/audio-active-source-review-v39.json',Path(__file__).resolve()]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
manifest=dict(scope='Read-only active-source and exact local-archive audit. Historical source snapshots only; no new build or runtime/audio acceptance.',owned=[],implementation_handoff='../audio-v38/manifest.json')
for p in sorted(set(owned)):
 rel=p.relative_to(root).as_posix();target=dest/'owned'/rel;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target);manifest['owned'].append(dict(path=rel,frozen=target.relative_to(dest).as_posix(),sha256=sha(p),bytes=p.stat().st_size))
(dest/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(dict(frozen=str(dest),owned=len(manifest['owned']))))
