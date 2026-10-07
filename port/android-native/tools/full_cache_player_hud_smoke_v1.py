"""Connected world/status HUD: real touch, scripted authored damage and resume.

Enemy attack selection is an explicit debug scenario using the existing native
authored-animation/combat pipeline. It does not establish full enemy AI, potion
actions, skills, portraits, campaign or physical ARM64 behavior.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time
import xml.etree.ElementTree as ET
from PIL import Image
from character_combat_smoke import inspect, launch_fresh

HUD=re.compile(r'Connected player HUD submitted \| viewport (\d+) (\d+) \| character (0x[\da-f]+) \| HP (-?\d+) (-?\d+) \| MP (-?\d+) (-?\d+) \| XP (-?\d+) (-?\d+) \| frames ((?:-?\d+ ){4}-?\d+) \| dirty (\d+) \| retained movie (0x[\da-f]+)')
HIT=re.compile(r'Prince damage received \| attacker (\S+) .*? \| HP (-?\d+) (-?\d+)')
ERROR=re.compile(r'FATAL EXCEPTION|Fatal signal|GL error|Connected player HUD failed|Native frame failed|Original UI font failed|load failed|event dispatch failed',re.I)

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--adb',required=True);p.add_argument('--serial',required=True)
    p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();assert a.serial.startswith('emulator-')
    a.output.mkdir(parents=True,exist_ok=False)
    digest=hashlib.sha256(a.apk.read_bytes()).hexdigest()
    report=dict(validation='FAIL',apk_sha256=digest,libraries=inspect(a.apk),scope=__doc__,physical_arm64_verified=False,cases={})
    last=''
    def adb(*args):
        r=subprocess.run([a.adb,'-s',a.serial,*args],text=True,capture_output=True,timeout=45)
        if r.returncode:raise RuntimeError(repr(args)+': '+r.stdout+r.stderr)
        return r.stdout.strip()
    def logs():
        nonlocal last
        pid=adb('shell','pidof','com.example.dh2');assert pid,'App exited'
        last=adb('logcat','-d','--pid='+pid,'-v','brief');assert not ERROR.search(last),last[-4000:]
        return last
    def wait(test,seconds=35):
        deadline=time.monotonic()+seconds
        while True:
            text=logs()
            if test(text):return text
            assert time.monotonic()<deadline,last[-4000:]
            time.sleep(.15)
    def hud(text):
        m=list(HUD.finditer(text));assert m,'No actual connected HUD submission'
        v=m[-1].groups()
        return dict(viewport=list(map(int,v[:2])),character=v[2],hp=list(map(int,v[3:5])),mp=list(map(int,v[5:7])),xp=list(map(int,v[7:9])),frames=list(map(int,v[9].split())),dirty_nodes=int(v[10]),movie=v[11])
    def views():
        adb('shell','uiautomator','dump','/sdcard/dh2-connected-hud.xml')
        xml=adb('shell','cat','/sdcard/dh2-connected-hud.xml')
        root=ET.fromstring(xml)
        return root,{n.get('content-desc'):list(map(int,re.findall(r'\d+',n.get('bounds')))) for n in root.iter('node') if n.get('content-desc')}
    def capture(name):
        path=a.output/(name+'.png')
        raw=subprocess.run([a.adb,'-s',a.serial,'exec-out','screencap','-p'],capture_output=True,check=True,timeout=30).stdout
        path.write_bytes(raw)
        image=Image.open(path).convert('RGB');w,h=image.size
        # Original bars must be visible over a substantial textured 3D scene.
        region=image.crop((0,0,w//3,h//4));pixels=list(region.getdata())
        red=sum(r>150 and r>g*1.4 and r>b*1.4 for r,g,b in pixels)
        blue=sum(b>100 and g>65 and b>r*1.3 for r,g,b in pixels)
        world=image.crop((w//3,h//4,2*w//3,3*h//4))
        assert red>500 and blue>500 and len(world.getcolors(world.width*world.height))>500,(name,red,blue)
        return dict(path=str(path.resolve()),sha256=hashlib.sha256(raw).hexdigest(),size=[w,h],red_bar_pixels=red,blue_bar_pixels=blue)
    try:
        remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:')
        assert '\n' not in remote and remote.endswith('.apk')
        report['installed_apk_sha256']=adb('shell','sha256sum',remote).split()[0]
        assert report['installed_apk_sha256']==digest,'Installed APK differs'
        launch_fresh(adb,'--ez','enemy_ai','false')
        text=wait(lambda t: HUD.search(t) and 'Native actor ready |' in t)
        assert 'Original cache mounted | files 6833 | archive bytes 433189197 | external storage 0' in text,'Native full cache did not mount'
        probes=re.findall(r'Original cache verified \| uri (.*?) \| bytes (\d+) \| sha256 ([0-9a-f]{64})',text)
        assert len(probes)==6 and len({row[0] for row in probes})==6,'Incomplete native APK cache probes'
        report['full_cache_native_probes']=[dict(uri=row[0],bytes=int(row[1]),sha256=row[2]) for row in probes]
        before=hud(text);assert before['viewport'][0]>before['viewport'][1] and before['frames'][:3]==[99,99,0],before
        root,controls=views();assert not any(n.get('text')=='Development tools' for n in root.iter('node')),'Default scene opens developer drawer'
        assert {'Movement control','Attack nearby enemy','Open development tools'}<=controls.keys(),controls
        report['cases']['default_scene']=dict(hud=before,screenshot=capture('default-scene'),developer_drawer_closed=True)
        x0,y0,x1,y1=controls['Movement control'];x=round((x0+x1)/2+(x1-x0)*.15);y=(y0+y1)//2
        offset=len(logs());adb('shell','input','touchscreen','swipe',str(x),str(y),str(x),str(y),'400')
        moved=wait(lambda t: bool(re.search(r'Player position .*? \| moved [1-9]',t[offset:])))
        assert hud(moved)['character']==before['character']
        report['cases']['touch_movement']=dict(real_touch=True,retained_character=before['character'],screenshot=capture('touch-movement'))
        offset=len(logs())
        adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ez','enemy_ai','false','--ei','object_index','4','--ei','combat_target_index','-2','--es','object_state','Attack','--ei','time_ms','-1')
        damaged=wait(lambda t: any(int(m[2])<int(m[1]) for m in HIT.findall(t[offset:])) and hud(t)['hp'][0]<before['hp'][0])
        damage=hud(damaged);hits=HIT.findall(damaged[offset:]);assert any(int(row[2])==damage['hp'][0] for row in hits),('HUD not driven by observed damage',damage,hits)
        expected=max(0,min(99,100*damage['hp'][0]//damage['hp'][1]-1))
        assert damage['frames'][0]==damage['frames'][3]==damage['frames'][4]==expected,damage
        assert damage['character']==before['character'] and damage['movie']==before['movie'],damage
        # Stop this explicit debug attack after the observed damage scenario.
        adb('shell','am','start','-W','--activity-single-top','-n','com.example.dh2/.MainActivity','--ez','enemy_ai','false','--ei','object_index','4','--ei','combat_target_index','-1','--es','object_state','Idle')
        report['cases']['authored_damage']=dict(hud=damage,authored_hits=hits,expected_source_frame=expected,scenario='Explicit scripted actor selection; full automatic AI not claimed',screenshot=capture('authored-damage'))
        offset=len(logs());adb('shell','input','keyevent','KEYCODE_HOME')
        adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--ez','enemy_ai','false')
        resumed=wait(lambda t: HUD.search(t[offset:]) and 'Native combat resumed' in t[offset:])
        after=hud(resumed);assert after['character']==before['character'] and after['movie']==before['movie'] and after['hp']==damage['hp'],(damage,after)
        report['cases']['context_resume']=dict(hud=after,world_and_hud_owner_retained=True,screenshot=capture('context-resume'))
        root,controls=views();x0,y0,x1,y1=controls['Open development tools'];adb('shell','input','tap',str((x0+x1)//2),str((y0+y1)//2))
        root,_=views();assert any(n.get('text')=='Development tools' for n in root.iter('node')),'Developer drawer did not open'
        adb('shell','input','tap',str((x0+x1)//2),str((y0+y1)//2))
        report['cases']['developer_drawer']=dict(open_close_verified=True)
        report['validation']='PASS'
    except Exception as ex:report['error']=str(ex);raise
    finally:
        (a.output/'logcat.txt').write_text(last,encoding='utf-8')
        (a.output/'connected-player-hud-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',apk_sha256=digest,cases=list(report['cases']))))
if __name__=='__main__':main()
