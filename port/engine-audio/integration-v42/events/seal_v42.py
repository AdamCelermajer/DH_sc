from pathlib import Path
import datetime,hashlib,json
O=Path(__file__).resolve().parent;R=O.parents[3]
def entry(p):
 raw=p.read_bytes();return dict(path=p.relative_to(R).as_posix(),bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest())
files=[p for p in sorted(O.rglob('*'))if p.is_file()and p.name!='manifest.json'
 and not {'.git','build','__pycache__'}.intersection(p.relative_to(O).parts)
 and p.suffix.lower()not in {'.o','.obj','.so','.a','.dll','.exe','.class','.pyc'}]
dependencies=['audio_application_manager_v42.hpp','audio_native_session_v42.hpp','../audio_gameplay_runtime_v42.hpp']
manifest=dict(scope='NEW source-only V42 active producer packet; no shared source edits or runtime/device execution',generated_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),owned=[entry(p)for p in files],external_dependencies=[entry((O.parent/p).resolve())for p in dependencies])
(O/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(len(files),'owned source files',entry(O/'manifest.json')['sha256'])
