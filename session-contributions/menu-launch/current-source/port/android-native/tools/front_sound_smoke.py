"""Verify real AS -> native menu sound -> Android playback, plus lifecycle.

The shell probe invokes the authored Options handler or actual AS global.
It is not touchscreen/input or navigation verification.
"""
import argparse
import hashlib
import json
import re
import subprocess
import time
from pathlib import Path


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--adb',required=True)
    p.add_argument('--apk',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--installed',action='store_true',help='Use the APK already installed by this invocation/session')
    a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=True)
    command=[a.adb,'-P','5038','-s','emulator-5580']
    def adb(*args):return subprocess.check_output(command+list(args),text=True,timeout=45)
    def logs():
        pid=subprocess.run(command+['shell','pidof','com.example.dh2'],capture_output=True,text=True,timeout=10).stdout.strip()
        return adb('logcat','-d','--pid='+pid,'-v','brief') if pid else ''
    def wait(marker,seconds=20):
        until=time.monotonic()+seconds
        while time.monotonic()<until:
            text=logs()
            if 'FATAL EXCEPTION' in text:raise AssertionError(text[-4000:])
            if marker in text:return text
            time.sleep(.15)
        raise AssertionError('Timed out: '+marker+'\n'+logs()[-4000:])
    def launch():
        adb('shell','am','force-stop','com.example.dh2')
        result=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
        for _ in range(4):
            if 'PackageUpdateActivity' not in result:break
            time.sleep(1)
            adb('shell','am','force-stop','com.example.dh2')
            result=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
        assert 'Activity: com.example.dh2/.MainActivity' in result,result
        wait('Original front/HUD screen submitted | screen main')
    def probe(name):
        adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe',name)
        return wait('Original menu sound probe dispatched | probe '+name)
    def screenshot(name):
        (a.output/name).write_bytes(subprocess.check_output(command+['exec-out','screencap','-p'],timeout=30))
    previous=adb('shell','wm','size')
    override=re.search(r'Override size: (\d+x\d+)',previous)
    report=dict(scope='Source sound callback/playback and pause only; no touch/navigation claim',
                serial='emulator-5580',adb_port=5038,apk_sha256=hashlib.sha256(a.apk.read_bytes()).hexdigest(),
                scenarios=[],complete_menu=False)
    try:
        if not a.installed:assert 'Success' in adb('install','-r',str(a.apk))
        for shape in ('1080x1920','1080x2400','1968x2184'):
            adb('shell','wm','size',shape)
            launch()
            probe('authored-options')
            text=wait('Menu effect completed | file=sfx_menu_confirm.wav')
            assert 'Original menu sound requested | name MenuConfirm | id 94 | file sfx_menu_confirm.wav' in text
            assert 'Menu effect started | file=sfx_menu_confirm.wav' in text
            screenshot('authored-options-'+shape+'.png')
            (a.output/('authored-options-'+shape+'.log')).write_text(text)
            report['scenarios'].append(dict(size=shape,authored_options_sound=True))
        # New process gives distinct markers/counters for wrapper behavior.
        launch()
        for name in ('MenuBack','MenuConfirm','MenuSelect','MenuSpending','MenuTab'):
            probe(name)
            file={'MenuBack':'back','MenuConfirm':'confirm','MenuSelect':'select',
                  'MenuSpending':'spending','MenuTab':'tab'}[name]
            wait('Menu effect completed | file=sfx_menu_'+file+'.wav')
        before=logs().count('Original menu sound requested |')
        for name in ('invalid-number','invalid-arity','does-not-exist'):
            text=probe(name)
            assert text.count('Original menu sound requested |')==before,name
        (a.output/'all-sounds.log').write_text(logs())
        report['all_five_playback_complete']=True
        report['invalid_arguments_unknown_name_noop']=True
        # Pause during a long effect; Android owner releases its player.
        launch()
        probe('MenuBack')
        wait('Menu effect started | file=sfx_menu_back.wav')
        adb('shell','input','keyevent','KEYCODE_HOME')
        text=wait('Menu effects released | count=1')
        time.sleep(1.5)
        assert 'Menu effect completed | file=sfx_menu_back.wav' not in logs()
        (a.output/'pause.log').write_text(logs())
        report['pause_releases_effect']=True
        report['status']='PASS'
    finally:
        adb('shell','wm','size',override.group(1) if override else 'reset')
        launch()
        screenshot('restored-main.png')
        (a.output/'sound-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))


if __name__=='__main__':main()
