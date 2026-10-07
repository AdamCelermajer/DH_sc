from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['port/level-world/tests/weapon_socket_alignment_v39.cpp','port/level-world/authored_fx_alignment_v39.cpp','port/level-world/source_fx_node_matrix_v4.cpp','port/engine-skinning/visual_skin_owner_v6.cpp','port/engine-skinning/visual_skin_selection_v6.cpp','port/engine-skinning/skin_pose_cache_v32.cpp','port/scene-materials/scene.cpp','port/engine-animation/animation.cpp','port/engine-animation/angle_interpreter.cpp']
snapshot='.local-inputs/player-item-effects-v5/host-snapshot';libs=['-ldh2_engine_animation','-ldh2_engine_skinning','-ldh2_scene_materials','-ldh2_level_world','-ldh2_game_data','-ldh2_script_runtime','-ldh2_engine_ui']
exe='.local-inputs/weapon-socket-alignment-v39'
commands=[['g++','-std=c++17','-O1','-g','-ffp-contract=off','-fno-fast-math','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-L'+snapshot,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+snapshot,'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'port/android-native/app/src/main/assets/models/prince_modular.bdae','.local-inputs/visual-skin-owner-v6/weapons','port/android-native/app/src/main/assets/animations/prince_walk_1hand.bdae']]
receipts=[]
for args in commands:
 r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);print(r.returncode,r.stdout,r.stderr);receipts.append({'command':args,'exit':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'commands':receipts,'sources':{s:sha(root/s) for s in sources},'dependency_binaries':{p.name:sha(p) for p in (root/snapshot).glob('*.so')},'actual_inputs':{p:sha(root/p) for p in commands[-1][-3:] if (root/p).is_file()},'live_visual_acceptance':False}
(root/'port/level-world/reports/weapon-socket-alignment-v39.json').write_text(json.dumps(report,indent=2)+'\n')
