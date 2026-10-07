"""Verify physical movement-pad touch reaches real player footstep consumers.

Checks the source player Move/Idle cycle and ev_ relay on emulator-5554.
Accepted Crypt floors have constructor-empty types and footprint FX -1.
Positive FX, complete NPC locomotion, combat damage and devices are untested.
"""
import argparse,hashlib,json,re,subprocess,time
from pathlib import Path
import xml.etree.ElementTree as ET

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for key in ('adb','serial'):p.add_argument('--'+key,required=True)
    p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();assert a.serial=='emulator-5554';a.output.mkdir(parents=True,exist_ok=True)
    def raw(*args):return subprocess.check_output([a.adb,'-s',a.serial,*args],timeout=45)
    def adb(*args):return raw(*args).decode('utf8','replace').strip()
    digest=hashlib.sha256(a.apk.read_bytes()).hexdigest()
    remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:')
    assert adb('shell','sha256sum',remote).split()[0]==digest
    pid=adb('shell','pidof','com.example.dh2');assert pid and ' ' not in pid
    def log():return adb('logcat','-d','--pid='+pid,'-v','brief')
    def healthy(text):
        (a.output/'player-animation-event-runtime.log').write_text(text,encoding='utf8')
        process=subprocess.run([a.adb,'-s',a.serial,'shell','pidof','com.example.dh2'],capture_output=True,text=True,timeout=45)
        if process.stdout.strip()!=pid or re.search(r'Fatal signal|FATAL EXCEPTION|Native frame failed|GL error|Required sole player|Original UI display failed',text):
            (a.output/'player-animation-event-crash.log').write_text(adb('logcat','-b','crash','-d','-v','brief'),encoding='utf8')
            raise AssertionError('Player movement failed; runtime and crash logs retained')
    baseline=log();healthy(baseline)
    adb('shell','uiautomator','dump','/sdcard/dh2-move-event.xml')
    xml=adb('shell','cat','/sdcard/dh2-move-event.xml');(a.output/'animation-event-ui.xml').write_text(xml,encoding='utf8')
    node=next(n for n in ET.fromstring(xml).iter('node') if n.get('content-desc')=='Movement control')
    x0,y0,x1,y1=map(int,re.findall(r'\d+',node.get('bounds')))
    x,y=(x0+x1)//2,(y0+y1)//2
    for direction in (1,-1):
        adb('shell','input','swipe',str(x),str(y),str(round(x+direction*(x1-x0)*.36)),str(round(y-(y1-y0)*.12)),'1800')
        healthy(log());time.sleep(.3)
    end=time.monotonic()+10
    while time.monotonic()<end:
        current=log();healthy(current)
        recent=current[len(baseline):]
        if re.search(r'Player source state \| previous 4 \| current 3',recent):break
        time.sleep(.1)
    else:raise AssertionError('Source Move did not return to Idle after touch release')
    assert re.search(r'Player source state \| previous 3 \| current 4 .*?event 0xc351',recent)
    events=re.findall(r'Original player animation event delivered \| name (step_left|step_right) \| lag (-?\d+) \| Lua status (\d+)',recent)
    assert {'step_left','step_right'}=={row[0] for row in events}
    assert all(row[2]=='0' for row in events)
    (a.output/'player-animation-event-runtime.log').write_text(current,encoding='utf8')
    pixels=raw('exec-out','screencap','-p');assert pixels.startswith(b'\x89PNG')
    (a.output/'player-animation-event-gameplay.png').write_bytes(pixels)
    report={'validation':'PASS','scope':__doc__,'apk_sha256':digest,'installed_apk_sha256':digest,
            'pid':pid,'physical_touch_swipes':2,'source_state_cycle':[3,4,3],
            'source_footstep_deliveries':events,'source_lua_statuses':[int(row[2]) for row in events],
            'positive_visual_fx_tested':False,'physical_arm64_tested':False,'full_game_playable':False}
    (a.output/'player-animation-event-smoke.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf8')
    print(json.dumps({k:v for k,v in report.items() if k!='scope'}))

if __name__=='__main__':main()
