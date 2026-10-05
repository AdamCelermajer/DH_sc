from pathlib import Path
import subprocess,shlex,json,hashlib
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def run(argv):
 r=subprocess.run(['wsl','--','bash','-lc',shlex.join(argv)],capture_output=True,text=True);assert r.returncode==0,(r.returncode,r.stdout,r.stderr);return r
folder=REPO/'.local-inputs/particle-cloud-models-v1-host';folder.mkdir(exist_ok=True)
deps=[ROOT/x for x in ['particle_cloud_models_v1.cpp','particle_cloud_runtime_v1.cpp','particle_billboard_v1.cpp','particle_random_v1.cpp','particle_factory.cpp','particle_emission.cpp','particle_parameter.cpp','particle_force_scene_v1.cpp','particle_scene_color_v1.cpp','tests/particle_cloud_models_v1.cpp','tests/particle_billboard_v1.cpp','tests/particle_cloud_runtime_v1_host.cpp','tests/particle_cloud_graph_v1.cpp']]+[REPO/'port/scene-materials/particle_scene_v1.cpp',REPO/'port/scene-materials/scene.cpp',REPO/'port/engine-resources/resources.cpp',REPO/'port/asset-payloads/payloads.cpp',REPO/'port/engine-math/math.cpp']
flags=['-std=c++17','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-misleading-indentation']
reports=[]
for opt in ['-O1','-O2']:
 exe=folder/('cloud'+opt[1:]);run(['g++',*flags,opt,*[linux(p)for p in deps],'-o',linux(exe)])
 result=run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(ROOT/'reference/particle-cloud-models-v1/original-fixtures.bin'),linux(ROOT/'reference/particle-cloud-models-v1/billboard-fixtures.bin'),linux(ROOT/'reference/particle-cloud-models-v1/billboard-apply-fixtures.bin'),linux(REPO/'.local-inputs/combat-hit-fx-v1/bloodsplat.bdae'),linux(REPO/'.local-inputs/combat-hit-fx-v1/bloodsplat_hero.bdae'),linux(ROOT/'reference/particle-cloud-models-v1/graph-fixtures.bin')]);assert not result.stderr,result.stderr;reports.append(dict(optimization=opt,**json.loads(result.stdout),executable_sha256=hashlib.sha256(exe.read_bytes()).hexdigest()))
report=dict(validation='PASS',runs=reports,scope='Original ARM gold replay plus actual authored blood BirthRate cache, same generation vector/model storage/source seed, real force scene binding and CPU color/bounds/sort/spun billboard vertices. Camera is an explicit host fixture; no live renderer/GPU/manager claim.',host_corner_float_tolerance_ulp=4,source_sha256={str(p.relative_to(REPO)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest()for p in deps})
(ROOT/'reports/particle-cloud-runtime-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items()if k!='source_sha256'}))
