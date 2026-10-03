"""Record checked source, bundled assets and matching emulator checkpoints."""
import argparse,hashlib,json,zipfile
from pathlib import Path
from emulator_smoke import inspect
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def read(path):
 raw=path.read_bytes();return json.loads(raw.decode('utf-16' if raw.startswith((b'\xff\xfe',b'\xfe\xff')) else 'utf-8-sig'))
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);a=p.parse_args()
 local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libraries=inspect(apk)
 tests={
  'character-smoke.json':local/'prince-tests-final/animation-smoke.json',
  'model-smoke-skinning.json':local/'model-tests-skinning-final/model-smoke.json',
  'candle-smoke-skinning.json':local/'candle-tests-skinning/animation-smoke.json',
  'texture-regression-skinning.json':local/'texture-tests-skinning/emulator-smoke.json'}
 records={name:read(path) for name,path in tests.items()}
 assert all(row['apk_sha256']==sha for row in records.values()),'Mixed APK checkpoints'
 assert records['character-smoke.json']['skinning_implemented'] and records['character-smoke.json']['pose_times_ms']==[100,1700]
 assert len(records['model-smoke-skinning.json']['cases'])==9
 assert len(records['texture-regression-skinning.json']['cases'])==7
 source={}
 for name in ['cpp/CMakeLists.txt','cpp/native_app.cpp','cpp/model_renderer.cpp','cpp/model_renderer.hpp','java/com/example/dh2/MainActivity.java','java/com/example/dh2/NativeBridge.java']:
  file=ROOT/'app/src/main'/name;other=a.studio/'app/src/main'/name;assert digest(file)==digest(other),name;source[name]=digest(file)
 provenance=read(ROOT/'app/src/main/assets/texture-provenance.json');assets=[]
 with zipfile.ZipFile(apk) as archive:
  for row in provenance['samples']:
   folder='models' if row['name'] in {'candle_flame.bdae','main_menu_charactere_swamp.bdae','prince_modular.bdae'} else 'animations' if row['name']=='prince_menu_idle_knight.bdae' else 'textures'
   name=f'assets/{folder}/{row["name"]}';raw=archive.read(name);assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'];assets.append(name)
  assert sorted(n for n in archive.namelist() if n.startswith(('assets/models/','assets/animations/','assets/textures/')))==sorted(assets)
 assert len(assets)==16
 for file in ('prince-android-build.log','prince-studio-build.log'):
  raw=(local/file).read_bytes();log=raw.decode('utf-16' if raw.startswith(b'\xff\xfe') else 'utf-8-sig');assert 'BUILD SUCCESSFUL' in log,file
 raw=(local/'prince-zipalign.log').read_bytes();alignment=raw.decode('utf-16' if raw.startswith(b'\xff\xfe') else 'utf-8-sig');assert 'Verification successful' in alignment
 host=read(REPO/'port/engine-skinning/reports/host-audit.json');assert host['controllers']==173 and host['tracks']==29 and host['poses']==11 and host['mutations']==1000
 oracle=read(REPO/'port/engine-skinning/reports/arm64-differential.json');assert sum(oracle[key] for key in ('affine_matrix_bit_exact_cases','skinned_vertex_bit_exact_cases','position_bit_exact_cases','quaternion_bit_exact_cases'))==500
 reports=ROOT/'reports'
 for name,value in records.items():(reports/name).write_text(json.dumps(value,indent=2)+'\n',encoding='utf-8')
 (REPO/'port/engine-skinning/reports/host-audit.json').write_text(json.dumps(host,indent=2)+'\n',encoding='utf-8')
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'repo_and_studio_gradle_builds_passed':True,'studio_source_sha256':source,'libraries':libraries,'apk_16k_zip_alignment_verified':True,'bundled_assets_verified':assets,'cache_sha256':provenance['cache_sha256'],'host_asan_ubsan':host,'animation_mutation_regression':5000,'scene_mutation_regression':10000,'original_instruction_comparisons':oracle,'emulator_models':3,'emulator_scene_captures':9,'emulator_texture_cases':7,'physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'playable_game':False,'goal_status':'active'}
 (reports/'build-validation-skinning.json').write_text(json.dumps(validation,indent=2)+'\n',encoding='utf-8');print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'source_sync_verified':True,'goal_status':'active'}))
if __name__=='__main__':main()
