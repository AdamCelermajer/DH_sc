"""Install/test the native viewer on a named development emulator with adb.

Requires Pillow for screenshot aspect checks. Leaves the viewer open and
restores the emulator's previous rotation mode. No global Logcat clearing.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import time
import xml.etree.ElementTree as ET
import zipfile
from PIL import Image,ImageChops

NAMES=('fx_smoke_03.tga','godparticle_face.tga','menugraphics03.tga','pvr2_env_darktemple_alpha.tga','skybox_wind.tga')

def launch_fresh(adb,*extras):
    """Wait out API 37's package-update relaunch before testing intent extras.

    Retry only if Android reports another activity or an already-running
    instance. Crashes, missing processes and GPU errors are not retried here.
    """
    deadline=time.monotonic()+10
    while True:
        adb('shell','am','force-stop','com.example.dh2')
        launch=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity',*extras)
        assert 'Status: ok' in launch,launch
        if 'Activity: com.example.dh2/.MainActivity' in launch and 'Activity not started' not in launch:
            return launch
        if time.monotonic()>deadline:raise AssertionError('Fresh app launch was intercepted: '+launch)
        time.sleep(.25)

def inspect(apk):
    libraries=[]
    with zipfile.ZipFile(apk) as z:
        assert {Path(n).name for n in z.namelist() if n.startswith('assets/textures/')}>=set(NAMES)
        for name in z.namelist():
            if not name.startswith('lib/') or not name.endswith('.so'):continue
            abi=name.split('/')[1];raw=z.read(name)
            assert abi in ('arm64-v8a','x86_64') and raw[:5]==b'\x7fELF\x02',name
            assert Path(name).name in ('libdh2_native.so','libdh2_engine_textures.so','libdh2_scene_materials.so','libdh2_engine_animation.so','libdh2_engine_skinning.so','libdh2_level_world.so','libdh2_game_data.so','libdh2_script_runtime.so','libc++_shared.so'),name
            phoff=struct.unpack_from('<Q',raw,32)[0];phsize,phcount=struct.unpack_from('<HH',raw,54)
            aligns=[struct.unpack_from('<Q',raw,phoff+i*phsize+48)[0] for i in range(phcount) if struct.unpack_from('<I',raw,phoff+i*phsize)[0]==1]
            assert aligns and min(aligns)>=16384,(name,aligns)
            libraries.append({'path':name,'sha256':hashlib.sha256(raw).hexdigest(),'minimum_load_alignment':min(aligns)})
    assert {r['path'].split('/')[1] for r in libraries}=={'arm64-v8a','x86_64'}
    return libraries

def main():
    p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True)
    p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    assert a.serial.startswith('emulator-'),'This harness operates on an emulator only'
    a.output.mkdir(parents=True,exist_ok=True)
    def adb(*args,allow_missing=False):
        run=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
        if run.returncode:
            if allow_missing and run.returncode==1 and not run.stdout.strip() and not run.stderr.strip():return ''
            raise RuntimeError('adb '+repr(args)+': '+run.stdout+run.stderr)
        return run.stdout.strip()
    libraries=inspect(a.apk)
    assert 'Success' in adb('install','-r',str(a.apk))
    prior=adb('shell','cmd','window','user-rotation').split()
    rows=[]
    try:
        for rotation,names in ((0,NAMES),(1,('menugraphics03.tga','skybox_wind.tga'))):
            adb('shell','cmd','window','user-rotation','lock',str(rotation))
            for name in names:
                launch=launch_fresh(adb,'--es','texture',name)
                deadline=time.monotonic()+30
                while True:
                    app_pid=adb('shell','pidof','com.example.dh2',allow_missing=True)
                    if app_pid:break
                    if time.monotonic()>deadline:raise AssertionError('App did not acquire a process: '+launch)
                    time.sleep(0.1)
                while True:
                    logs=adb('logcat','-d','--pid='+app_pid,'-v','brief')
                    if name+': ' in logs and 'RGBA upload OK' in logs:break
                    if time.monotonic()>deadline:raise AssertionError(logs)
                    time.sleep(0.1)
                assert not re.search(r'FATAL EXCEPTION|GL error|Shader failed|Link failed|Texture load failed',logs),logs
                adb('shell','uiautomator','dump','/sdcard/dh2-native-window.xml')
                root=ET.fromstring(adb('shell','cat','/sdcard/dh2-native-window.xml'))
                viewport=next(n for n in root.iter('node') if n.get('content-desc')=='DH2 native texture viewport')
                x0,y0,x1,y1=map(int,re.findall(r'\d+',viewport.get('bounds')))
                stem=f'{rotation}-{Path(name).stem}'
                screenshot=a.output/(stem+'.png')
                adb('shell','screencap','-p','/sdcard/dh2-native-test.png');adb('pull','/sdcard/dh2-native-test.png',str(screenshot))
                picture=Image.open(screenshot).convert('RGB');crop=picture.crop((x0,y0,x1,y1))
                # Find the rendered quad against the known cleared surface.
                delta=ImageChops.difference(crop,Image.new('RGB',crop.size,(20,23,28)))
                delta=delta.convert('L').point(lambda v:255 if v>3 else 0)
                bounds=delta.getbbox();assert bounds,(name,'Blank native texture surface')
                width,height=bounds[2]-bounds[0],bounds[3]-bounds[1]
                assert abs(width-height)<=2,(name,'Square texture stretched',bounds,crop.size)
                rows.append({'texture':name,'rotation':rotation,'viewport':crop.size,'image_bounds':bounds,'screenshot':screenshot.name,'upload_ok':True})
                (a.output/(stem+'.log')).write_text('\n'.join(line for line in logs.splitlines() if 'DH2Native' in line or 'AndroidRuntime' in line)+'\n')
    finally:
        if prior and prior[0] in ('free','lock'):adb('shell','cmd','window','user-rotation',*prior)
    report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'serial':a.serial,
            'android_api':adb('shell','getprop','ro.build.version.sdk'),'abi':adb('shell','getprop','ro.product.cpu.abi'),
            'device_page_size':adb('shell','getconf','PAGESIZE'),'libraries':libraries,'cases':rows,'physical_phone_tested':False}
    (a.output/'emulator-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('libraries','cases')}));print(f'Passed {len(rows)} native uploads and screenshot aspect checks')
if __name__=='__main__':main()
