"""Build and replay the same-inventory equipment owner against private DSOs."""
import argparse,hashlib,json,os,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(R).as_posix()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--no-build',action='store_true');a=ap.parse_args()
 stage=R/'.local-inputs/player-equipment-render-owner-v1';snap=stage/'host-snapshot';exe=stage/'owner-audit'
 base=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec']
 src=['port/level-world/player_equipment_render_owner_v1.cpp','port/level-world/player_equipment_queries_v1.cpp','port/engine-skinning/visual_skin_selection_v6.cpp','port/engine-skinning/visual_skin_owner_v6.cpp','port/level-world/tests/player_equipment_render_owner_v1.cpp']
 libs=['level_world','game_data','engine_ui','engine_skinning','engine_animation','scene_materials','script_runtime']
 compile=base+['g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off',*src,'-L.local-inputs/player-equipment-render-owner-v1/host-snapshot','-Wl,-rpath,/mnt/c/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/player-equipment-render-owner-v1/host-snapshot',*['-ldh2_'+x for x in libs],'-o','.local-inputs/player-equipment-render-owner-v1/owner-audit']
 sources=src+[x.replace('.cpp','.hpp')for x in src[:4]]+['port/game-data/tests/player_gear_cache_v5.cpp',rel(Path(__file__)), 'port/level-world/player_initial_grants_v2.hpp','port/level-world/player_initial_grants_v2.cpp']
 bindings={p:sha(R/p)for p in sources};dependencies={rel(p):sha(p)for p in snap.glob('*.so')}
 if not a.no_build:subprocess.run(compile,cwd=R,check=True)
 inputs=['port/level-world/reference/character-game-design/real-cache-inputs.bin','.local-inputs/items-discovery','.local-inputs/player-item-effects-v5/power-cache','port/android-native/app/src/main/assets','.local-inputs/player-item-effects-v5/private-save','.local-inputs/visual-skin-owner-v6/weapons','port/android-native/app/src/main/assets/models/prince_modular.bdae','port/game-data/reference/player-item-effects-v5/starter-effects-fixtures.bin']
 run=base+['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1','.local-inputs/player-equipment-render-owner-v1/owner-audit',*inputs]
 result=subprocess.run(run,cwd=R,text=True,capture_output=True);(stage/'host-stdout.txt').write_text(result.stdout);(stage/'host-stderr.txt').write_text(result.stderr)
 assert result.returncode==0 and not result.stderr,(result.returncode,result.stdout,result.stderr)
 audit=json.loads(result.stdout);assert audit['validation']=='PASS' and audit['mismatches']==0
 assert bindings=={p:sha(R/p)for p in sources};assert dependencies=={rel(p):sha(p)for p in snap.glob('*.so')}
 files=[R/inputs[0],R/inputs[6],R/inputs[7],R/'port/level-world/reference/player-equipment-render-owner-v1/query-fixtures.bin']
 for root in (R/inputs[1],R/inputs[2],R/inputs[5]):files += [p for p in root.rglob('*')if p.is_file() and p.suffix.lower() in ('.bin','.bdae')]
 for pattern in ('common_text*','text*_py*.bin'):
  files += list((R/inputs[3]/'original-cache/data/pydata').glob(pattern))
 # Exact localization byte inputs; no cache/filesystem/selected-world parity claim.
 files+=[p for p in (R/inputs[3]/'original-cache/data/text').rglob('*')if p.is_file()]
 report={'validation':'PASS','host_audit':audit,'sanitizers':{'address':True,'undefined':True,'leak':True,'findings':0},'source_sha256':bindings,'input_sha256':{rel(p):sha(p)for p in sorted(set(files))if p.is_file()},'dependency_sha256':dependencies,'executable_sha256':sha(exe),'commands':{'compile':compile,'replay':run},'original_sha256':sha(R/'.local-inputs/libDungeonHunter2.so'),'scope':{'same_authoritative_inventory':True,'same_property_backing':True,'original_query_gold_replayed':2000,'world_projections':'explicit offline selected-player/difficulty/player-count fixtures','rng':'explicit caller seed 1; original clock producer unclaimed','localization':'genuine retained cache bytes and filesystem leases','visual':'frozen V6 native retained scene/module/weapon owner','packaged_execution':False,'full_campaign':False},'proof_sha256':{'port/level-world/reports/player-equipment-queries-v1-arm64-differential.json':sha(R/'port/level-world/reports/player-equipment-queries-v1-arm64-differential.json'),'port/engine-skinning/reference/visual-skin-owner-v6/freeze-manifest.json':sha(R/'port/engine-skinning/reference/visual-skin-owner-v6/freeze-manifest.json')}}
 p=R/'port/level-world/reports/player-equipment-render-owner-v1-host-audit-v1.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
