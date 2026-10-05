"""Exercise the separate menu emulator's original intro, aspect fit and audio.

This is a media milestone test. It does not claim menu input/savegame parity.
"""
import argparse
import hashlib
import json
import re
import subprocess
import time
from pathlib import Path
from PIL import Image, ImageChops


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--adb',required=True)
    p.add_argument('--port',default='5038')
    p.add_argument('--serial',default='emulator-5580')
    p.add_argument('--apk',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--installed',action='store_true')
    a=p.parse_args()
    if a.serial!='emulator-5580' or a.port!='5038':
        raise ValueError('This contribution tests only its isolated emulator and ADB server')
    a.output.mkdir(parents=True,exist_ok=True)
    command=[a.adb,'-P',a.port,'-s',a.serial]
    def adb(*args):
        return subprocess.check_output(command+list(args),text=True,timeout=60)
    def pid():
        r=subprocess.run(command+['shell','pidof','com.example.dh2'],capture_output=True,text=True,timeout=10)
        return r.stdout.strip()
    def logs():
        current=pid()
        return adb('logcat','-d','--pid='+current,'-v','brief') if current else ''
    def wait(needle,seconds=30):
        until=time.monotonic()+seconds
        while time.monotonic()<until:
            text=logs()
            if 'FATAL EXCEPTION' in text:raise AssertionError(text[-4000:])
            if needle in text:return text
            time.sleep(.2)
        raise AssertionError('Timed out waiting for '+needle+'\n'+logs()[-3000:])
    def launch(*extras):
        adb('shell','am','force-stop','com.example.dh2')
        text=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity',*extras)
        for _ in range(4):
            if 'PackageUpdateActivity' not in text:break
            time.sleep(1)
            adb('shell','am','force-stop','com.example.dh2')
            text=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity',*extras)
        if 'Status: ok' not in text:raise AssertionError(text)
    def shot(name):
        path=a.output/(name+'.png')
        path.write_bytes(subprocess.check_output(command+['exec-out','screencap','-p'],timeout=30))
        return path
    previous=adb('shell','wm','size')
    override=re.search(r'Override size: (\d+x\d+)',previous)
    result=dict(scope='Original cinematic/video/audio fit and lifecycle only',
                apk_sha256=hashlib.sha256(a.apk.read_bytes()).hexdigest(),
                serial=a.serial,adb_server_port=a.port,full_menu_functionality=False,scenarios=[])
    try:
        # Explicit force-stop/launch after the package installer has settled.
        if not a.installed and 'Success' not in adb('install','-r',str(a.apk)):raise AssertionError('Install failed')
        time.sleep(1)
        for shape in ('1080x1920','1080x2400','1968x2184'):
            adb('shell','wm','size',shape)
            launch('--ez','cinematic','true')
            text=wait('Intro first video frame')
            time.sleep(.8)
            first=shot('intro-'+shape+'-first')
            time.sleep(5)
            second=shot('intro-'+shape+'-playing')
            with Image.open(first) as i,Image.open(second) as j:
                if i.size!=j.size:raise AssertionError('Display changed during frame comparison')
                # Ignore Skip and system edges; demand actual movie-frame changes.
                w,h=i.size;box=(w//4,h//4,w*3//4,h*3//4)
                if ImageChops.difference(i.crop(box).convert('RGB'),j.crop(box).convert('RGB')).getbbox() is None:
                    raise AssertionError('Movie did not advance visibly')
            fit=re.findall(r'Intro fit \| surface=(\d+)x(\d+) \| video=1280x720 \| fitted=([\d.]+)x([\d.]+)',text)
            if not fit:raise AssertionError('Original video dimensions not delivered')
            sw,sh,fw,fh=map(float,fit[-1])
            if abs(fw/fh-16/9)>.00001 or fw>sw+.01 or fh>sh+.01:
                raise AssertionError('Video stretched or cropped')
            audio=adb('shell','dumpsys','media.audio_flinger')
            (a.output/('intro-'+shape+'-audio-flinger.txt')).write_text(audio)
            (a.output/('intro-'+shape+'.log')).write_text(logs())
            adb('shell','input','keyevent','4')
            wait('Original front/HUD screen submitted')
            wait('Title music loop entered',15)
            shot('main-after-intro-'+shape)
            result['scenarios'].append(dict(size=shape,video_fit=fit[-1],frames_change=True,
                skip_to_main=True,title_intro_then_loop=True))
            print('PASS media fit/skip/title loop:',shape,flush=True)
        launch('--ez','cinematic','true')
        wait('Intro first video frame');time.sleep(2)
        adb('shell','input','keyevent','3');paused=wait('Intro paused')
        time.sleep(2)
        adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','-f','0x10200000')
        resumed=wait('Intro resumed')
        pause_pos=int(re.findall(r'Intro paused \| position_ms=(\d+)',paused)[-1])
        resume_pos=int(re.findall(r'Intro resumed \| position_ms=(\d+)',resumed)[-1])
        if abs(resume_pos-pause_pos)>1000:raise AssertionError('Background playback advanced or lost position')
        shot('intro-resumed')
        text=wait('Intro completed',60)
        wait('Original front/HUD screen submitted')
        (a.output/'intro-completion-lifecycle.log').write_text(logs())
        shot('main-after-completion')
        result['scenarios'].append(dict(lifecycle_pause_position=pause_pos,
            lifecycle_resume_position=resume_pos,natural_completion_to_main=True))
        result['validation']='PASS'
    except Exception as ex:
        result['validation']='FAIL';result['error']=str(ex)
        (a.output/'failure.log').write_text(logs())
        shot('failure')
        raise
    finally:
        adb('shell','wm','size',override.group(1) if override else 'reset')
        launch('--es','front_screen','main')
        wait('Original front/HUD screen submitted')
        time.sleep(2) # Authored entry animation must settle before the final view.
        shot('restored-main')
        (a.output/'media-validation.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':main()

