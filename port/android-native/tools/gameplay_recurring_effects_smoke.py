"""Live same-player MP regeneration, repeated skill availability, and idle soak.

Run after the character panel and no-target skill smoke on emulator-5554.
No save edits, faery unlocks, target injection, or successful damage claims.
"""
import argparse, hashlib, json, re, subprocess, time
from pathlib import Path
import xml.etree.ElementTree as ET

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--adb',required=True);p.add_argument('--serial',required=True)
    p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--soak-seconds',type=int,default=120)
    a=p.parse_args();assert a.serial=='emulator-5554' and a.soak_seconds>=30
    a.output.mkdir(parents=True,exist_ok=True)
    def raw(*args):return subprocess.check_output([a.adb,'-s',a.serial,*args],timeout=45)
    def adb(*args):return raw(*args).decode('utf8','replace').strip()
    digest=hashlib.sha256(a.apk.read_bytes()).hexdigest()
    remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:')
    assert adb('shell','sha256sum',remote).split()[0]==digest
    pid=adb('shell','pidof','com.example.dh2');assert pid and ' ' not in pid
    def log():return adb('logcat','-d','--pid='+pid,'-v','brief')
    def healthy(text):
        assert not re.search(r'Fatal signal|FATAL EXCEPTION|Native frame failed|GL error|Required sole player|Original UI display failed',text)
        assert adb('shell','pidof','com.example.dh2')==pid
    def wait(predicate,seconds):
        end=time.monotonic()+seconds
        while time.monotonic()<end:
            text=log();healthy(text)
            if predicate(text):return text
            time.sleep(.25)
        raise AssertionError('Required live source observation timed out')
    start=log();healthy(start)
    casts=re.findall(r'Original skill activation .*?accepted 1 .*?native targets 0 .*?raw MP (\d+)',start)
    assert casts,'Run the no-target skill smoke first'
    initial_mp=int(casts[-1])
    def mp_rows(text):
        tail=text[text.rfind('Original skill activation'):]
        return [(int(x),int(y)) for x,y in re.findall(r'Connected player HUD submitted .*?MP (\d+) (\d+)',tail)]
    recovered=wait(lambda t:any(x==y and x>initial_mp for x,y in mp_rows(t)),25)
    progression=mp_rows(recovered)
    assert any(initial_mp<x<=y for x,y in progression)
    # Return the development camera to the player after the earlier NPC probe.
    adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ei','object_index','-1','--ei','time_ms','-1')
    wait(lambda t:'Actor command applied | index -1' in t,15)
    adb('shell','uiautomator','dump','/sdcard/dh2-recurring.xml')
    xml=adb('shell','cat','/sdcard/dh2-recurring.xml');(a.output/'recurring-ui.xml').write_text(xml,encoding='utf8')
    hud=next(n for n in ET.fromstring(xml).iter('node') if n.get('content-desc')=='Three equipped skills, faery spell, and health potion')
    x0,y0,x1,y1=map(int,re.findall(r'\d+',hud.get('bounds')))
    count=len(re.findall(r'Original skill activation .*?accepted 1',log()))
    adb('shell','input','tap',str(round(x0+(x1-x0)/10)),str((y0+y1)//2))
    repeated=wait(lambda t:len(re.findall(r'Original skill activation .*?accepted 1',t))>count,10)
    assert re.findall(r'Original skill activation .*?native targets (\d+)',repeated)[-1]=='0'
    wait(lambda t:any(x==y and x>initial_mp for x,y in mp_rows(t)),25)
    soak_start=time.monotonic()
    while time.monotonic()-soak_start<a.soak_seconds:
        healthy(log());time.sleep(1)
    final=log();healthy(final);(a.output/'recurring-runtime.log').write_text(final,encoding='utf8')
    pixels=raw('exec-out','screencap','-p');assert pixels.startswith(b'\x89PNG')
    (a.output/'recurring-gameplay.png').write_bytes(pixels)
    report={'validation':'PASS','scope':__doc__,'apk_sha256':digest,'installed_apk_sha256':digest,
            'pid':pid,'mana_after_skill':initial_mp,'observed_mana_progression':progression,
            'repeat_no_target_skill_accepted':True,'same_process_soak_seconds':a.soak_seconds,
            'unlocked_faery_cast_tested':False,'enemy_target_hud_positive_tested':False,
            'skill_damage_verified':False,'physical_arm64_tested':False,'full_game_playable':False}
    (a.output/'gameplay-recurring-effects-smoke.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf8')
    print(json.dumps({k:v for k,v in report.items() if k!='scope'}))

if __name__=='__main__':main()
