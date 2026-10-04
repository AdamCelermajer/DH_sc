"""Validate actual authored main-menu HTML labels at three isolated display sizes."""
import argparse
import hashlib
import json
import re
import struct
import subprocess
import time
from pathlib import Path
from PIL import Image


def face_metrics(path):
    data=path.read_bytes();tables={}
    for offset in range(12,12+16*struct.unpack_from('>H',data,4)[0],16):
        name,checksum,start,length=struct.unpack_from('>4sIII',data,offset)
        tables[name]=start
    units=struct.unpack_from('>H',data,tables[b'head']+18)[0]
    ascent,descent=struct.unpack_from('>hh',data,tables[b'hhea']+4)
    return units,ascent-descent


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--apk',type=Path,required=True)
    parser.add_argument('--font',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
    command=['C:/Users/adamc/AppData/Local/Android/Sdk/platform-tools/adb.exe','-P','5038','-s','emulator-5580']
    def adb(*parts):return subprocess.check_output(command+list(parts),text=True,timeout=30)
    def logs():
        probe=subprocess.run(command+['shell','pidof','com.example.dh2'],text=True,capture_output=True,timeout=30)
        if probe.returncode not in (0,1):raise RuntimeError(probe.stderr)
        pid=probe.stdout.strip()
        return adb('logcat','-d','--pid='+pid,'-v','brief') if pid else ''
    def wait():
        end=time.monotonic()+35
        while time.monotonic()<end:
            text=logs()
            if 'FATAL EXCEPTION' in text or 'Authored UI draw failed' in text:raise AssertionError(text[-5000:])
            if 'Original front/HUD screen submitted' in text:return text
            time.sleep(.2)
        raise AssertionError('Original main menu did not submit\n'+logs()[-3000:])
    previous=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',previous)
    expected=face_metrics(args.font)
    result=dict(scope='Authored main-menu HTML text and real face metrics only',full_menu_functionality=False,
                apk_sha256=hashlib.sha256(args.apk.read_bytes()).hexdigest(),font_sha256=hashlib.sha256(args.font.read_bytes()).hexdigest(),expected_face_metrics=expected,scenarios=[])
    try:
        if 'Success' not in adb('install','-r',str(args.apk)):raise AssertionError('Install failed')
        time.sleep(1)
        for shape in ('1080x1920','1080x2400','1968x2184'):
            adb('shell','wm','size',shape)
            for attempt in range(2):
                adb('shell','am','force-stop','com.example.dh2')
                launch=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
                if 'Activity: com.example.dh2/.MainActivity' in launch:break
                if 'PackageUpdateActivity' not in launch:raise AssertionError(launch)
            text=wait();time.sleep(2)
            if 'Original source frame/history bound before shared/root construction' not in text:
                raise AssertionError('Original frame/history owner was not connected before SWF startup')
            for label in ('Start game','Options','Info','MORE GAMES!'):
                if ' | display '+label+' | source ' not in text:raise AssertionError('Original layout did not deliver '+label)
            metrics=re.findall(r'Original FT face metrics \| name Fontin SmallCaps \| bold \d \| italic \d \| units (\d+) \| height (\d+) \| scale ([\d.]+)',text)
            if not metrics or any(tuple(map(int,m[:2]))!=expected or float(m[2])!=1 for m in metrics):
                raise AssertionError('FT face metrics differ from exact original font tables: '+str(metrics))
            png=args.output/('main-'+shape+'.png')
            png.write_bytes(subprocess.check_output(command+['exec-out','screencap','-p'],timeout=30))
            with Image.open(png) as image:
                # Original font color #9CFF9A must appear as green rendered glyphs.
                green=sum(1 for r,g,b in image.convert('RGB').get_flattened_data() if 110<r<190 and g>215 and 110<b<190)
                if green<30:raise AssertionError('Styled green More Games text is not visible')
                screenshot_size=image.size
            (args.output/('main-'+shape+'.log')).write_text(logs())
            result['scenarios'].append(dict(size=shape,screenshot=screenshot_size,green_glyph_pixels=green,original_labels=True,real_face_metrics=True,original_frame_owner=True))
            print('PASS original menu text:',shape,flush=True)
        result['validation']='PASS'
    except Exception as error:
        result['validation']='FAIL';result['error']=str(error);raise
    finally:
        adb('shell','wm','size',override.group(1) if override else 'reset')
        (args.output/'text-validation.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':main()
