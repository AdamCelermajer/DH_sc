"""Run exact authored GameSWF shaders on a named emulator and retain evidence.

Tests shader pixel readback and textured primitive scaling/context recreation.
Does not claim reconstructed Flash timelines, menus, HUD actions or visual parity.
"""
import argparse
import hashlib
import json
import math
from pathlib import Path
import re
import subprocess
import time
import xml.etree.ElementTree as ET
import zipfile
from PIL import Image, ImageChops
from emulator_smoke import NAMES, launch_fresh
from live_actor_smoke import BAD, inspect_apk

GPU = re.compile(r'Authored UI shader GPU contract PASS \| programs (\d+) \| cases (\d+) \| max byte error (\d+)')
FRAME = re.compile(r'Authored UI texture frame submitted \| viewport (\d+) (\d+) \| texture (\d+) (\d+) \| scale ([-+\d.eE]+) ([-+\d.eE]+) \| GameSWF normal')


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--adb', required=True)
    p.add_argument('--serial', required=True)
    p.add_argument('--apk', type=Path, required=True)
    p.add_argument('--shader-stage', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--install', action='store_true')
    args = p.parse_args()
    assert args.serial.startswith('emulator-')
    assert not args.output.exists(), 'Preserve prior shader smoke artifacts'
    args.output.mkdir(parents=True)
    report = dict(validation='FAIL', apk_sha256=digest(args.apk.read_bytes()), serial=args.serial,
                  shader_stage_sha256=digest(args.shader_stage.read_bytes()), cases=[],
                  original_UI_display_list_verified=False, original_material_selection_verified=False,
                  full_game_playable=False, physical_arm64_phone_tested=False)
    commands = []
    prior = None

    def adb(*command, missing=False):
        run = subprocess.run([args.adb, '-s', args.serial, *command], capture_output=True, text=True, timeout=45)
        commands.append(dict(arguments=list(command), returncode=run.returncode, stdout=run.stdout, stderr=run.stderr))
        if missing and run.returncode == 1 and not (run.stdout+run.stderr).strip():
            return ''
        assert run.returncode == 0, str(command)+'\n'+run.stdout+run.stderr
        return run.stdout.strip()

    try:
        libraries, _ = inspect_apk(args.apk)
        stage = json.loads(args.shader_stage.read_text())
        assert stage['validation'] == 'PASS' and stage['original_bytes_unmodified']
        with zipfile.ZipFile(args.apk) as apk:
            assert all(digest(apk.read('assets/'+name)) == value for name,value in stage['asset_sha256'].items())
        if args.install:
            assert 'Success' in adb('install', '-r', str(args.apk))
        paths = adb('shell','pm','path','com.example.dh2').splitlines()
        assert len(paths) == 1
        report['installed_apk_sha256'] = adb('shell','sha256sum',paths[0].removeprefix('package:')).split()[0]
        assert report['installed_apk_sha256'] == report['apk_sha256']
        report['libraries'] = libraries
        report['api'] = adb('shell','getprop','ro.build.version.sdk')
        report['abi'] = adb('shell','getprop','ro.product.cpu.abi')
        prior = adb('shell','cmd','window','user-rotation').split()
        for rotation,names in ((0,NAMES),(1,('menugraphics03.tga','skybox_wind.tga'))):
            adb('shell','cmd','window','user-rotation','lock',str(rotation))
            for name in names:
                launch_fresh(adb,'--es','texture',name)
                deadline = time.monotonic()+30
                text = ''
                while True:
                    pid = adb('shell','pidof','com.example.dh2',missing=True)
                    assert pid, 'Native shader process exited'
                    text = adb('logcat','-d','--pid='+pid,'-v','brief')
                    assert not BAD.search(text) and not re.search(r'Authored UI .*?(?:failed|mismatch)',text,re.I), text
                    matches = FRAME.findall(text)
                    if matches and f'textures/{name}:' in text:
                        break
                    assert time.monotonic()<deadline, text
                    time.sleep(.1)
                layout_observations = []
                # The status label can resize the SurfaceView after upload.
                # Compare the actual hierarchy with the latest submitted frame,
                # rather than a frame captured before that asynchronous layout.
                while True:
                    adb('shell','uiautomator','dump','/sdcard/dh2-authored-ui-window.xml')
                    tree = ET.fromstring(adb('shell','cat','/sdcard/dh2-authored-ui-window.xml'))
                    node = next(n for n in tree.iter('node') if n.get('content-desc')=='DH2 native texture viewport')
                    bounds = tuple(map(int,re.findall(r'\d+',node.get('bounds'))))
                    text = adb('logcat','-d','--pid='+pid,'-v','brief')
                    assert not BAD.search(text) and not re.search(r'Authored UI .*?(?:failed|mismatch)',text,re.I), text
                    matches = FRAME.findall(text)
                    assert matches
                    observed_size = (bounds[2]-bounds[0],bounds[3]-bounds[1])
                    submitted_size = tuple(map(int,matches[-1][:2]))
                    layout_observations.append(dict(hierarchy=observed_size,submitted=submitted_size))
                    if observed_size == submitted_size:
                        break
                    assert time.monotonic()<deadline, layout_observations
                    time.sleep(.1)
                gpu = GPU.findall(text)
                assert gpu and all(int(programs)==2 and int(cases)==8 and int(error)<=2 for programs,cases,error in gpu)
                width,height,tw,th,sx,sy = matches[-1]
                width,height,tw,th = map(int,(width,height,tw,th))
                sx,sy = float(sx),float(sy)
                assert all(map(math.isfinite,(sx,sy))) and min(width,height,tw,th)>0
                # Applied shader matrix preserves authored texture proportions.
                assert abs((width*sx)/(height*sy)-tw/th) < 1e-5
                assert abs(sx-min(1,(tw/th)/(width/height))) < 1e-6
                assert abs(sy-min(1,(width/height)/(tw/th))) < 1e-6
                assert (bounds[2]-bounds[0],bounds[3]-bounds[1]) == (width,height)
                stem = str(rotation)+'-'+Path(name).stem
                screenshot = args.output/(stem+'.png')
                adb('shell','screencap','-p','/sdcard/dh2-authored-ui-test.png')
                adb('pull','/sdcard/dh2-authored-ui-test.png',str(screenshot))
                crop = Image.open(screenshot).convert('RGB').crop(bounds)
                mask = ImageChops.difference(crop,Image.new('RGB',crop.size,(20,23,28))).convert('L').point(lambda v:255 if v>8 else 0)
                drawn = mask.getbbox()
                assert drawn and mask.histogram()[255]>100, 'Blank authored textured primitive'
                # Transparent images need not fill the quad; their visible
                # support must remain inside the aspect-correct projected quad.
                quad = ((width-width*sx)/2,(height-height*sy)/2,(width+width*sx)/2,(height+height*sy)/2)
                assert drawn[0]>=quad[0]-2 and drawn[1]>=quad[1]-2 and drawn[2]<=quad[2]+2 and drawn[3]<=quad[3]+2
                (args.output/(stem+'.log')).write_text(text+'\n')
                report['cases'].append(dict(texture=name,rotation=rotation,viewport=[width,height],texture_size=[tw,th],
                    scale=[sx,sy],gpu_contracts=[list(map(int,row)) for row in gpu],rendered_bounds=drawn,
                    projected_quad=quad,layout_observations=layout_observations,
                    screenshot=screenshot.name,screenshot_sha256=digest(screenshot.read_bytes())))
        report['validation'] = 'PASS'
        report['shader_pixel_contract_cases_per_context'] = 8
    except Exception as error:
        report['error'] = repr(error)
        raise
    finally:
        if prior:
            adb('shell','cmd','window','user-rotation',*prior)
        (args.output/'adb-transcript.json').write_text(json.dumps(commands,indent=2)+'\n')
        (args.output/'authored-ui-shader-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(validation=report['validation'],textured_frames=len(report['cases']),
                         apk_sha256=report['apk_sha256'],physical_ARM64=False)))


if __name__ == '__main__':
    main()
