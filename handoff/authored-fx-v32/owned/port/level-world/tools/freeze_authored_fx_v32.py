from pathlib import Path
import hashlib,json,re,shutil
root=Path(__file__).resolve().parents[3];dest=root/'handoff/authored-fx-v32';dest.mkdir(parents=True,exist_ok=True)
owned=list((root/'port/level-world').glob('*v32.*'))+list((root/'port/engine-animation').glob('*v32.*'))+list((root/'port/level-world/tests').glob('*v32*'))+list((root/'port/level-world/tools').glob('*v32*'))
owned+=list((root/'port/level-world/reference').glob('*v32*'))+list((root/'port/level-world/reports').glob('*v32*'))
owned=[p for p in owned if p.is_file()]
deps=set()
for path in ['port/engine-math/math.cpp','port/engine-animation/tests/particle_cloud_models_v1_differential.py','port/engine-animation/tests/particle_factory_differential.py','port/engine-animation/tests/compiled_transforms_differential.py','port/engine-animation/tests/animation_blend_differential.py','port/engine-resources/tests/cpu.py']:deps.add(root/path)
for receipt in (root/'port/level-world/reports').glob('authored-fx-v32-*-v*.json'):
 for path in json.loads(receipt.read_text()).get('sources',{}):deps.add(root/path)
pending=list(deps|set(owned));visited=set()
while pending:
 path=pending.pop()
 if path in visited or not path.is_file():continue
 visited.add(path)
 if path.suffix not in ('.cpp','.hpp','.h'):continue
 for include in re.findall(r'^\s*#include\s+"([^"]+)"',path.read_text(),re.M):
  target=(path.parent/include).resolve()
  if not target.is_file():
   target=next((p/include for p in (root/'port').iterdir() if (p/include).is_file()),target)
  if target.is_file() and target.is_relative_to(root):deps.add(target);pending.append(target)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
manifest={'scope':'45 actual FX resource CPU domains; no GPU/live acceptance','production_tus':['port/level-world/character_authored_resource_v32.cpp','port/level-world/authored_fx_mesh_graph_v32.cpp','port/level-world/authored_fx_nonrender_geometry_v32.cpp','port/engine-animation/particle_resource_init_v32.cpp','port/engine-animation/particle_billboard_v32.cpp'],'owned':[],'dependencies':[],'assets':[],'host_libraries':[]}
for kind,paths in [('owned',owned),('dependencies',sorted(deps-set(owned)))]:
 for p in paths:
  rel=p.relative_to(root).as_posix();target=dest/('owned' if kind=='owned' else 'dependency-snapshot')/rel;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target)
  manifest[kind].append({'path':rel,'frozen':target.relative_to(dest).as_posix(),'sha256':sha(p)})
cache=root/'port/level-world/reference/shared-target-facing-v1/cache/general-v5';census=json.loads((root/'port/level-world/reports/authored-resource-domains-v6.json').read_text())
for row in census['resources']:
 p=cache/row['local'];assert sha(p)==row['sha256'];manifest['assets'].append({'path':p.relative_to(root).as_posix(),'uri':row['uri'],'sha256':sha(p)})
for p in sorted((root/'.local-inputs/character-fx-owner-v1/host-snapshot').glob('*.so')):manifest['host_libraries'].append({'path':p.relative_to(root).as_posix(),'sha256':sha(p)})
manifest['original']={'path':'.local-inputs/libDungeonHunter2.so','sha256':sha(root/'.local-inputs/libDungeonHunter2.so')}
(dest/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps({'frozen':str(dest),'owned':len(manifest['owned']),'dependencies':len(manifest['dependencies']),'assets':len(manifest['assets'])}))
