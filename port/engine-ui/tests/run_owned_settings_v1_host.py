"""Bind isolated sanitizer replay of the actual settings owner batch.
This does not build or run production APKs, and does not label fixture world
inventory/audio/device providers as a genuine full application backend.
"""
import argparse,pathlib,subprocess,json,hashlib,os
R=pathlib.Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--executable',default='.local-inputs/hud-owned-settings-v1/owned-settings-audit');ap.add_argument('--scene-executable',default='.local-inputs/hud-owned-settings-v1/language-scene-audit');ap.add_argument('--output',type=pathlib.Path,required=True);a=ap.parse_args()
 names=['game_option_table_v1','owned_hud_settings_v1','settings_native_files_v1','settings_language_scene_v1','localization','hud_startup_callbacks'];paths=[f'port/engine-ui/{n}.{e}' for n in names for e in ('hpp','cpp')]+['port/engine-ui/tests/owned_settings_v1.cpp','port/engine-ui/tests/settings_language_scene_v1.cpp']
 before={p:sha(R/p) for p in paths};gold='port/engine-ui/reference/owned-hud-settings-v1/fixtures.bin';sgold='port/engine-ui/reference/owned-hud-settings-v1/language-scene-fixtures.bin';data='port/android-native/app/src/main/assets/original-cache/data/pydata'
 def run(exe,args):
  cmd=['wsl','--','bash','-lc','cd /mnt/c/Users/adamc/Desktop/workspace/DH_sc && ASAN_OPTIONS=detect_leaks=1 '+exe+' '+' '.join(args)]
  p=subprocess.run(cmd,text=True,capture_output=True);assert p.returncode==0 and not p.stderr,(p.returncode,p.stdout,p.stderr)
  result=json.loads(p.stdout);assert result['validation']=='PASS' and result['mismatches']==0;return result
 owner=run(a.executable,[gold,'.local-inputs/design-settings',data,'.local-inputs/hud-owned-settings-v1/native-file-test']);scene=run(a.scene_executable,[sgold]);assert before=={p:sha(R/p) for p in paths}
 assert owner['comparisons']==1038 and owner['owner_cases']==254 and owner['parser_cases']==512 and owner['record_cases']==272 and owner['real_stdio_missing_and_present'] and scene['comparisons']==162
 arm=R/'port/engine-ui/reports/owned-hud-settings-v1-arm64-differential.json';graph=R/'port/engine-ui/reports/settings-language-scene-v1-arm64-differential.json'
 for p in (arm,graph):
  report=json.loads(p.read_text());assert report['validation']=='PASS' and report['mismatches']==0
  for key,value in report['source_sha256'].items():assert before[key]==value
 inputs={f'.local-inputs/design-settings/design_{kind}.bin':sha(R/f'.local-inputs/design-settings/design_{kind}.bin') for kind in ('pyarray','pyarraynames','pystructnames')};inputs.update({f'{data}/common_text_{kind}.bin':sha(R/f'{data}/common_text_{kind}.bin') for kind in ('pyarray','pyarraynames','pystructnames')})
 result={'validation':'PASS','host_audit':owner,'language_scene_audit':scene,'sanitizers':{'address':True,'undefined':True,'leak':True,'findings':0},'source_sha256':before,'executable_sha256':{a.executable:sha(R/a.executable),a.scene_executable:sha(R/a.scene_executable)},'corpus_sha256':{gold:sha(R/gold),sgold:sha(R/sgold)},'input_sha256':inputs,'original_sha256':sha(R/'.local-inputs/libDungeonHunter2.so'),'proof_sha256':{str(p.relative_to(R)).replace('\\','/'):sha(p) for p in (arm,graph)},'script_sha256':sha(pathlib.Path(__file__)),'scope':__doc__,'required_backends':['ObjectManager graph projection and lifetime','Character virtual predicates and real inventory localization','ItemObject real inventory/item localization','actual platform-language/device facts','live sound backend when present','AS result/ref cleanup for multiplayer result'],'not_claimed':['campaign profile chunk owner/save writes','full original FileSystem implementation','complete inventory/audio/device backend','packaged ARM64 instruction parity','live Android startup']}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'validation':'PASS','owner_comparisons':owner['comparisons'],'scene_comparisons':scene['comparisons'],'sanitizer_findings':0}))
if __name__=='__main__':main()
