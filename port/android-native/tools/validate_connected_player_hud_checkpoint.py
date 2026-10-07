"""Freeze one consequential connected player/status HUD checkpoint.

The completed subsystem is world/status composition and retained source status
timelines. Skills, potion actions, portrait, original cursor policy, full AI,
campaign and physical ARM64 verification remain separate milestones.
"""
import argparse
from datetime import datetime,timezone
import hashlib
import json
from pathlib import Path
import shutil
import zipfile

ROOT=Path(__file__).resolve().parents[3]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def record(path):return dict(path=str(path.resolve()),sha256=sha(path))
def read(path):return json.loads(path.read_text())

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('apk','capture','host','inspection','connected-stage','hud-proof','combat-proof','checkpoint','output'):
        p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();assert not a.output.exists() and not a.checkpoint.exists()
    digest=sha(a.apk);host=read(a.host);inspection=read(a.inspection);stage=read(a.connected_stage)
    assert host['validation']==inspection['validation']==stage['validation']=='PASS'
    assert host['sanitizer_findings']==0 and host['actual_main_CMake_targets'] and len(host['host_audits'])==102
    assert stage['host_sha256']==sha(a.host)==inspection['host']['sha256']
    assert inspection['capture']['sha256']==sha(a.capture)
    assert inspection['connected_hud_stage_sha256']==sha(a.connected_stage)
    assert inspection['projects']['packaged']['apk']['sha256']==digest
    hud,combat=read(a.hud_proof),read(a.combat_proof)
    for proof in (hud,combat):assert proof['validation']=='PASS' and proof['apk_sha256']==proof['installed_apk_sha256']==digest
    assert set(hud['cases'])=={'default_scene','touch_movement','authored_damage','context_resume','developer_drawer'}
    assert set(combat['cases'])=={'stationary','moving'}
    initial=hud['cases']['default_scene'];damage=hud['cases']['authored_damage'];resume=hud['cases']['context_resume']
    assert initial['developer_drawer_closed'] and initial['hud']['frames'][:3]==[99,99,0]
    assert damage['hud']['hp'][0]<initial['hud']['hp'][0] and resume['world_and_hud_owner_retained']
    assert damage['hud']['hp']==resume['hud']['hp']
    for key in ('character','movie'):assert initial['hud'][key]==damage['hud'][key]==resume['hud'][key]
    screenshots=[]
    for name,case in hud['cases'].items():
        if 'screenshot' not in case:continue
        image=case['screenshot'];path=Path(image['path'])
        assert sha(path)==image['sha256'] and image['red_bar_pixels']>500 and image['blue_bar_pixels']>500
        screenshots.append(dict(case=name,**record(path)))
    log=(a.hud_proof.parent/'logcat.txt').read_text()
    assert 'Authored UI shader GPU contract PASS | programs 2 | cases 8 | max byte error 1' in log
    assert 'Connected player HUD failed' not in log and 'Fatal signal' not in log
    with zipfile.ZipFile(a.capture) as z:
        manifest=json.loads(z.read('build-capture.json'))
        assert manifest['apks']['packaged']['sha256']==digest
        assert hashlib.sha256(z.read('packaged-app-debug.apk')).hexdigest()==digest
        assert manifest['connected_hud_stage']['sha256']==sha(a.connected_stage)
        for name,value in manifest['source_sha256'].items():
            assert hashlib.sha256(z.read('source/'+name)).hexdigest()==value
        sources=manifest['source_sha256']
    assets={}
    with zipfile.ZipFile(a.apk) as z:
        for name in z.namelist():
            if name.startswith('assets/'):
                raw=z.read(name);assets[name[7:]]=dict(sha256=hashlib.sha256(raw).hexdigest(),bytes=len(raw))
    assert len(assets)==770 and {"assets/"+name:value['sha256'] for name,value in assets.items()}==inspection['asset_sha256']
    a.checkpoint.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(a.apk,a.checkpoint)
    assert sha(a.checkpoint)==digest
    result=dict(validation='PASS',scope=__doc__,recorded_at_utc=datetime.now(timezone.utc).isoformat(),
        checkpoint=record(a.checkpoint),apk_bytes=a.checkpoint.stat().st_size,
        build_capture=record(a.capture),build_inspection=record(a.inspection),main_linked_host_audit=record(a.host),
        connected_source_stage=record(a.connected_stage),source_sha256=sources,assets_verified={'packaged':assets},
        emulator=dict(serial='emulator-5554',api=37,abi='x86_64',visible=True),
        emulator_proofs=[record(a.hud_proof),record(a.combat_proof)],visual_inspection=screenshots,
        source_suites=102,live_scenarios=7,connected_world_and_status_HUD=True,
        live_HP_damage_source_timeline=True,real_touch_movement=True,authored_stationary_and_moving_combat=True,
        source_viewport_publication=True,retained_world_and_HUD_after_context_resume=True,developer_drawer_verified=True,
        physical_arm64_verified=False,original_HUD_input_verified=False,skills_potions_portrait_connected=False,
        full_enemy_ai_verified=False,full_game_verified=False,all_game_assets_bundled=False)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',apk_sha256=digest,source_suites=102,live_scenarios=7,checkpoint=str(a.checkpoint))))
if __name__=='__main__':main()
