"""Finalize the retained equipment milestone from matching build and live receipts."""
import argparse, hashlib, json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
PROJECT=ROOT/'port/android-native'

def sha(path):
    digest=hashlib.sha256()
    with path.open('rb') as source:
        while block:=source.read(1024*1024):digest.update(block)
    return digest.hexdigest()

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture',type=Path,required=True)
    parser.add_argument('--equipment-live',type=Path,required=True)
    parser.add_argument('--hud-live',type=Path,required=True)
    args=parser.parse_args()
    library_path=PROJECT/'reports/native-equipped-player-library-build-v1.json'
    capture=json.loads(args.capture.read_text());equipment=json.loads(args.equipment_live.read_text())
    hud=json.loads(args.hud_live.read_text());library=json.loads(library_path.read_text())
    assert capture['validation']=='BUILD_INPUTS_CAPTURED'
    assert equipment['validation']==hud['validation']==library['validation']=='PASS'
    checkpoint=Path(capture['checkpoint']['path']);digest=sha(checkpoint)
    assert digest==capture['checkpoint']['sha256']
    for live in (equipment,hud):assert digest==live['apk_sha256']==live['installed_apk_sha256']
    host_path=ROOT/capture['main_host_report']['path'];host=json.loads(host_path.read_text())
    assert sha(host_path)==capture['main_host_report']['sha256']==library['main_host_receipt_sha256']
    assert host['validation']=='PASS' and host['sanitizer_findings']==0 and len(host['host_audits'])==155
    assert set(equipment['cases'])=={'starter_equipment','unequip_mainhand','equip_mainhand',
        'alternate_equipment_set','context_resume','restore_original_set','geared_touch_movement'}
    assert set(hud['cases'])=={'default_scene','touch_movement','authored_damage','context_resume','developer_drawer'}
    for receipt in (capture,library):
        for name,value in receipt['source_sha256'].items():assert sha(ROOT/name)==value,('Source drift',name)
    for abi,result in library['abis'].items():
        for metadata in result['libraries'].values():assert sha(Path(metadata['path']))==metadata['sha256']
    stripped=PROJECT/'app/build/intermediates/stripped_native_libs/debug/stripDebugDebugSymbols/out'
    for name,value in capture['libraries'].items():assert sha(stripped/name)==value['sha256']
    for live in (equipment,hud):
        assert len(live['libraries'])==len(capture['libraries'])==18
        for value in live['libraries']:assert capture['libraries'][value['path']]['sha256']==value['sha256']
    snapshot=Path(capture['source_snapshot']['path']);assert sha(snapshot)==capture['source_snapshot']['sha256']
    for live in (equipment,hud):
        for case in live['cases'].values():
            if 'screenshot' in case:
                screen=case['screenshot']
                assert sha(Path(screen['path']))==screen['sha256']
                assert screen['size'][0]>screen['size'][1]
                assert screen['red_bar_pixels']>500 and screen['blue_bar_pixels']>500
    output=PROJECT/f'reports/native-equipped-player-{digest[:8]}-checkpoint-validation.json'
    assert not output.exists(),'Preserve accepted checkpoint validation'
    receipts={path.relative_to(ROOT).as_posix():sha(path) for path in
              (args.capture,args.equipment_live,args.hud_live,library_path,host_path,
               PROJECT/'tools/equipped_player_smoke_v1.py',
               PROJECT/'tools/equipped_player_hud_smoke_v1.py',Path(__file__))}
    report=dict(validation='PASS',scope=__doc__,checkpoint=capture['checkpoint'],source_snapshot=capture['source_snapshot'],
        receipts_sha256=receipts,source_sha256=capture['source_sha256'],compiler_inputs=capture['compiler_inputs'],
        assets_verified=capture['assets_verified'],libraries=capture['libraries'],main_host_suites=155,
        sanitizer_findings=0,equipment_emulator_cases=equipment['cases'],hud_emulator_cases=hud['cases'],
        source_equipment_inventory_and_properties=True,live_equipped_armor_and_weapon_rendering=True,
        live_equipment_actions='Explicit debug actions on the same native player owner',
        retained_inventory_through_GL_recreation=True,live_geared_player_movement=True,
        one_APK=True,bundled_cache_files=6833,external_cache_install=False,ARM32_engine_bundled=False,
        live_character_menu_complete=False,live_active_skills_complete=False,full_enemy_AI=False,
        equipment_dependent_collision_bounds=False,original_packed_GPU_ABI=False,
        audio_and_campaign_saves_complete=False,full_game_verified=False,physical_arm64_verified=False)
    output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(validation='PASS',checkpoint=str(checkpoint),sha256=digest,output=str(output))))

if __name__=='__main__':main()
