"""Live retained equipment/GPU integration, with explicit debug equipment actions.

This does not establish a finished character menu, active skills, campaign or
physical ARM64 behavior. Real movement uses the visible Android touch control.
"""
import argparse, hashlib, io, json, re, subprocess, time
from pathlib import Path
import xml.etree.ElementTree as ET
from character_combat_smoke import inspect, launch_fresh
from PIL import Image

CONNECTED = re.compile(r'Native player equipment connected \| identity ([\da-f]+) \| items (\d+) \| potions (-?\d+) \| selected (-?\d+) \| retained (\d+) \| property backing ([\da-f]+) \| RNG calls (\d+)')
ITEM = re.compile(r'Native inventory item \| index (\d+) \| ID (-?\d+) \| quantity (-?\d+) \| name (.*?) \| slots (-?\d+) (-?\d+)')
ACTION = re.compile(r'Native equipment action \| operation (\d+) \| index (-?\d+) \| slot (-?\d+) \| result (-?\d+) \| selected (-?\d+) \| parts (\d+) \| property checksum ([\da-f]+)')
ERROR = re.compile(r'FATAL EXCEPTION|Fatal signal|GL error|Native frame failed|equipment.*failed|Equipment.*rejected|load failed', re.I)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb',required=True);parser.add_argument('--serial',required=True)
    parser.add_argument('--apk',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();assert args.serial=='emulator-5554'
    args.output.mkdir(parents=True,exist_ok=False)
    sha=lambda raw:hashlib.sha256(raw).hexdigest()
    report=dict(validation='FAIL',scope=__doc__,apk_sha256=sha(args.apk.read_bytes()),
                libraries=inspect(args.apk),cases={},physical_arm64_verified=False)
    last=''
    def adb(*command):
        result=subprocess.run([args.adb,'-s',args.serial,*command],capture_output=True,text=True,timeout=45)
        assert result.returncode==0,(command,result.stdout,result.stderr)
        return result.stdout.strip()
    def logs():
        nonlocal last
        pid=adb('shell','pidof','com.example.dh2');assert pid,'Player process exited'
        last=adb('logcat','-d','--pid='+pid,'-v','brief')
        assert not ERROR.search(last),last[-5000:]
        return last
    def wait(predicate,timeout=120):
        deadline=time.monotonic()+timeout
        while True:
            current=logs()
            if predicate(current):return current
            assert time.monotonic()<deadline,last[-5000:]
            time.sleep(.2)
    def capture(name):
        # GL logs can precede the first displayed surface or a landscape
        # recreation. Accept actual HUD and textured world pixels, not PNG
        # syntax alone, and keep the last capture on failure.
        path=args.output/(name+'.png');deadline=time.monotonic()+90
        while True:
            raw=subprocess.run([args.adb,'-s',args.serial,'exec-out','screencap','-p'],capture_output=True,check=True,timeout=30).stdout
            assert raw.startswith(b'\x89PNG\r\n\x1a\n');path.write_bytes(raw)
            image=Image.open(io.BytesIO(raw)).convert('RGB');w,h=image.size
            pixels=list(image.crop((0,0,w//3,h//4)).getdata())
            red=sum(r>150 and r>g*1.4 and r>b*1.4 for r,g,b in pixels)
            blue=sum(b>100 and g>65 and b>r*1.3 for r,g,b in pixels)
            world=image.crop((w//3,h//4,2*w//3,3*h//4))
            colors=len(world.getcolors(world.width*world.height))
            if w>h and red>500 and blue>500 and colors>500:
                return dict(path=str(path.resolve()),sha256=sha(raw),size=[w,h],red_bar_pixels=red,blue_bar_pixels=blue,world_colors=colors)
            assert time.monotonic()<deadline,(name,w,h,red,blue,colors)
            time.sleep(.3)
    def equipment(operation,index=-1,slot=-1):
        offset=len(logs())
        adb('shell','am','broadcast','--receiver-foreground','-a','com.example.dh2.DEBUG_EQUIPMENT','-p','com.example.dh2',
            '--ei','operation',str(operation),'--ei','index',str(index),'--ei','slot',str(slot))
        text=wait(lambda value:'Equipment command applied | Equipment updated' in value[offset:] and ACTION.search(value[offset:]))
        rows=list(ACTION.finditer(text[offset:]));assert len(rows)==1
        values=rows[0].groups();assert tuple(map(int,values[:3]))==(operation,index,slot)
        return dict(operation=operation,index=index,slot=slot,result=int(values[3]),
                    selected=int(values[4]),parts=int(values[5]),property_checksum=values[6])
    def connected(text):
        values=list(CONNECTED.finditer(text))[-1].groups()
        return dict(character=values[0],items=int(values[1]),potions=int(values[2]),
                    selected=int(values[3]),retained=int(values[4]),properties=values[5],rng_calls=int(values[6]))
    try:
        remote=adb('shell','pm','path','com.example.dh2').removeprefix('package:')
        assert '\n' not in remote and remote.endswith('.apk')
        report['installed_apk_sha256']=adb('shell','sha256sum',remote).split()[0]
        assert report['installed_apk_sha256']==report['apk_sha256']
        adb('logcat','-c');launch_fresh(adb,'--ez','enemy_ai','false','--ez','developer','false','--es','world','crypt01.dwld')
        text=wait(lambda value:CONNECTED.search(value) and 'Connected player HUD submitted' in value and 'Native actor ready |' in value)
        initial=connected(text);items=[m.groups() for m in ITEM.finditer(text)]
        assert initial['items']==5 and initial['selected']==0 and initial['retained']==0
        assert len(items)==initial['items'] and all(row[3] for row in items)
        main=[row for row in items if int(row[4])==1];assert len(main)==1,items
        index=int(main[0][0]);parts=int(re.findall(r'Equipped GPU graph rebuilt \| parts (\d+) \|',text)[-1])
        assert parts>4
        print('Checking visible starter equipment',flush=True)
        report['cases']['starter_equipment']=dict(owner=initial,items=items,screenshot=capture('starter-equipment'))
        # Screen readiness also crosses any initial surface recreation.
        # Commands below must therefore target the current live GL owner.
        wait(lambda value:'worlds/crypt01.dwld: Crypt' in value)
        empty=equipment(2,slot=1);assert empty['parts']==4
        report['cases']['unequip_mainhand']=dict(action=empty,screenshot=capture('unequipped-mainhand'))
        restored=equipment(1,index,1);assert restored['parts']==parts
        report['cases']['equip_mainhand']=dict(action=restored,screenshot=capture('equipped-mainhand'))
        alternate=equipment(3);assert alternate['selected']==1 and alternate['parts']==4
        report['cases']['alternate_equipment_set']=dict(action=alternate,screenshot=capture('alternate-equipment-set'))
        print('Equipment mutations passed; checking context resume',flush=True)
        offset=len(logs());adb('shell','input','keyevent','KEYCODE_HOME')
        adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--ez','enemy_ai','false')
        text=wait(lambda value:CONNECTED.search(value[offset:]) and 'Connected player HUD submitted' in value[offset:])
        resumed=connected(text);assert resumed['retained']==1 and resumed['selected']==1
        for key in ('character','items','potions','properties','rng_calls'):assert resumed[key]==initial[key],(key,initial,resumed)
        assert 'Equipped GPU graph rebuilt | parts 4 |' in text[offset:]
        report['cases']['context_resume']=dict(owner=resumed,screenshot=capture('context-resume'))
        original=equipment(3);assert original['selected']==0 and original['parts']==parts
        report['cases']['restore_original_set']=dict(action=original)
        adb('shell','uiautomator','dump','/sdcard/dh2-equipment-smoke.xml')
        nodes=ET.fromstring(adb('shell','cat','/sdcard/dh2-equipment-smoke.xml'))
        movement=[node for node in nodes.iter('node') if node.get('content-desc')=='Movement control'];assert len(movement)==1
        x0,y0,x1,y1=map(int,re.findall(r'\d+',movement[0].get('bounds')))
        x=round((x0+x1)/2+(x1-x0)*.15);y=(y0+y1)//2;offset=len(logs())
        adb('shell','input','touchscreen','swipe',str(x),str(y),str(x),str(y),'500')
        wait(lambda value:re.search(r'Player position .*? \| moved [1-9]',value[offset:]))
        report['cases']['geared_touch_movement']=dict(real_touch=True,screenshot=capture('geared-touch-movement'))
        report['validation']='PASS'
    except Exception as error:
        report['error']=str(error);raise
    finally:
        (args.output/'logcat.txt').write_text(last,encoding='utf-8')
        (args.output/'equipped-player-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',apk_sha256=report['apk_sha256'],cases=list(report['cases']))))

if __name__=='__main__':main()
