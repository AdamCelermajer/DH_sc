"""Bind the native OnInit APK to actual compiler, assets and API37/16KiB evidence."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import zipfile
from emulator_smoke import inspect

ROOT=Path(__file__).resolve().parents[3]

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--apk',type=Path,required=True)
    p.add_argument('--production-snapshot',type=Path,required=True)
    p.add_argument('--runtime-report',type=Path,required=True)
    p.add_argument('--sdk',type=Path,required=True)
    p.add_argument('--java',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--require-native-character-list',action='store_true')
    p.add_argument('--require-native-ghost-skill-initialization',action='store_true')
    p.add_argument('--require-native-frame-foundations',action='store_true')
    a=p.parse_args();digest=sha(a.apk)
    production=json.loads(a.production_snapshot.read_text())
    runtime=json.loads(a.runtime_report.read_text())
    assert production['validation']==runtime['validation']=='PASS'
    assert production['apk_sha256']==runtime['apk_sha256']==runtime['installed_apk_sha256']==digest
    assert production['all_actual_repository_compiler_inputs_unchanged_before_and_after_build']
    assert runtime['api_level']>=37 and runtime['page_size']==16384
    for key in ('native_monster_unchanged_oninit_live_properties','native_monster_vm_health_and_timers_retained',
                'unfinished_ai_dot_timer_providers_paused','native_level_constructor_fields_and_range_callbacks'):
        assert runtime[key],key
    if a.require_native_character_list:
        assert runtime['native_character_list_load_reload_recreation']
        counts=runtime['native_character_list_counts']
        assert len(counts)>=3 and all(row==[14,14,0,0,0] for row in counts)
    if a.require_native_ghost_skill_initialization:
        for key in ('native_skill_catalogue_load_reload_recreation','native_ghost_ordered_init_script_process',
                    'native_ghost_vector_catalogue_backing_retained'):
            assert runtime[key],key
        assert runtime['native_ghost_nonempty_skill_scripts_supported'] is False
    if a.require_native_frame_foundations:
        facts=runtime['native_frame_foundation_character_facts']
        assert len(facts)==2 and all(row['observations']>=7 and row['type']==4 and row['monster']==1
            and row['player']==row['faerie']==row['NPC']==row['projected_death']==0 and row['zonable']==1 for row in facts.values())
    sources=production['source_sha256']
    for name,value in sources.items():
        assert sha(ROOT/name)==value,('actual compiled input changed',name)
    assets={}
    with zipfile.ZipFile(a.apk) as archive:
        for name in archive.namelist():
            if name.startswith('assets/') and not name.endswith('/'):
                raw=archive.read(name);assets[name.removeprefix('assets/')]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
    assert assets['data/design_pycst.bin']['sha256']=='db014f9832c1d9e60c62272a2ee67d15568d003c191ae32bbce070cb49056931'
    assert assets['data/DebugSwitches.savegame']=={'bytes':665,'sha256':'51a3827f0109e16d1520e76d5b19736df3afe38375b91b955ac519f954234d6b'}
    assert assets['scripts/ai/_commons.luac']['sha256']=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    assert assets['scripts/ai/monster.luac']['sha256']=='84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d'
    if a.require_native_ghost_skill_initialization:
        from prepare_skill_tables import INPUTS
        for name, expected in INPUTS.items():
            assert assets['data/'+name]['sha256']==expected,name
    tools=a.sdk/'build-tools/37.0.0'
    alignment=subprocess.run([str(tools/'zipalign.exe'),'-c','-P','16','4',str(a.apk.resolve())],check=True,capture_output=True,text=True)
    env=os.environ.copy();env['JAVA_HOME']=str(a.java)
    signing=subprocess.run(['cmd.exe','/d','/c',str(tools/'apksigner.bat'),'verify','--verbose',str(a.apk.resolve())],env=env,check=True,capture_output=True,text=True)
    libraries=inspect(a.apk)
    report={'validation':'PASS','apk_sha256':digest,'apk_bytes':a.apk.stat().st_size,
            'native_libraries':libraries,'assets':assets,'source_sha256':sources,
            'runtime_report_sha256':sha(a.runtime_report),'actual_compiler_input_snapshot_sha256':sha(a.production_snapshot),
            'all_actual_repository_compiler_inputs_unchanged_before_and_after_build':True,
            'zip_16k_alignment_verified':alignment.returncode==0,'signing_verification':signing.stdout.strip(),
            'native_monster_oninit_live':True,'native_monster_vm_health_timers_retained':True,
            'native_full_character_bindings':False,'native_full_skill_initialization':False,
            'native_autonomous_ghost_ai':False,'full_game_playable':False,'physical_arm64_phone_tested':False,
            'scope':'Exact source-built monster OnInit/native properties/managed fallback host/Level/design/real Debug file and retained VM, damaged health, paused timers across reload/rotation on API37/16KiB. Full nonempty skills, complete bindings and autonomous AI remain pending; additional verified initialization phases are stated below.'}
    if a.require_native_character_list:
        report['native_character_list_load_reload_recreation']=True
        report['native_character_list_count']=14
        report['native_character_list_owned_nodes']=14
        report['native_full_object_manager_factory']=False
        report['scope']+=' Native ownership/enrollment of the Prince and 13 live monster projections uses the source flat Character-list shape. Full name-map/factory/manager cleanup and autonomous Ghost acquisition are unproved.'
    if a.require_native_ghost_skill_initialization:
        report['native_ghost_ordered_init_script_process']=True
        report['native_ghost_vector_catalogue_backing_retained']=True
        report['native_ghost_nonempty_skill_scripts_supported']=False
        report['scope']=report['scope'].replace('Source skills/post/final/full bindings and autonomous AI remain pending.',
                                               'Full nonempty skill scripting/Character bindings and autonomous AI remain pending.')
        report['scope']+=' Authored Ghost HP/MP/SetSkillsAndSpells/UpdateAllSkills/Post/Final run in source order once. Five zero-script faery slots use actual owned vectors and real table/Debug/path/Arguments/InitVCB providers. Full nonempty skill scripting remains pending.'
    if a.require_native_frame_foundations:
        report['native_frame_foundation_character_facts']=runtime['native_frame_foundation_character_facts']
        report['scope']+=' Source cached-ID/type predicates consume loaded native Ghost property/catalogue owners across recreation; zonability is diagnostic only. Ghost type4 avoids the unproved type0 instance-name mapping; death uses the normalized port input. Original name/dead-byte producers, autonomous frame eligibility, room enrollment and target/pursuit providers remain unproved.'
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':'PASS','apk_sha256':digest,'assets':len(assets),'native_libraries':len(libraries),'actual_compiler_inputs':len(sources)}))

if __name__=='__main__':main()
