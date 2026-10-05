"""Real Android tap/swipe -> source hit/event -> authored main button checks.

Does not claim MenuManager/HUDControls forwarding or destination navigation.
"""
import argparse
import hashlib
import json
import re
import subprocess
import time
from pathlib import Path
from PIL import Image


def main():
    p=argparse.ArgumentParser()
    p.add_argument('--apk',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--installed',action='store_true')
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    cmd=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
    def adb(*args):return subprocess.check_output(cmd+list(args),text=True,timeout=45)
    def logs():
        pid=subprocess.run(cmd+['shell','pidof','com.example.dh2'],text=True,capture_output=True,timeout=10).stdout.strip()
        return adb('logcat','-d','--pid='+pid,'-v','brief') if pid else ''
    def wait(marker,seconds=25):
        end=time.monotonic()+seconds
        while time.monotonic()<end:
            text=logs()
            if 'FATAL EXCEPTION' in text or 'Original menu input failed:' in text:raise AssertionError(text[-4000:])
            if marker in text:return text
            time.sleep(.15)
        raise AssertionError('Missing '+marker+'\n'+logs()[-4000:])
    def launch(trace=True):
        for _ in range(5):
            adb('shell','am','force-stop','com.example.dh2')
            result=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main','--ez','trace_menu_input',str(trace).lower())
            if 'PackageUpdateActivity' not in result:break
            time.sleep(1)
        assert 'Activity: com.example.dh2/.MainActivity' in result,result
        wait('Original source input connected | direct main event stage')
        # 3D texture upload can outlast the input binding. Deliver test taps
        # only after a complete menu frame has actually been submitted.
        wait('Original front/HUD screen submitted | screen main',seconds=60)
        text=logs()
        assert 'Original shared renderer frame/history bound before root construction' in text
        assert 'Original shared menu renderer loaded | independent player | states 9 | Options id 121 | frames 23 | inactive | native stack pending' in text
        players=re.search(r'Original menu renderer player identities \| main ([0-9a-f]+) \| shared ([0-9a-f]+) \| distinct 1',text)
        assert players and int(players[1],16) and int(players[2],16) and players[1]!=players[2],text[-4000:]
        time.sleep(2)
    def shot(name):
        path=a.output/name
        path.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
        with Image.open(path) as image:return image.size
    def tap_options(w,h):
        # Test coordinates are a point inside the original authored Options
        # art. The application itself never contains rectangle/button tap zones.
        cw=min(w,h*3//2);ch=min(h,w*2//3)
        return int((w-cw)/2+410*cw/480),int((h-ch)/2+160*ch/320)
    def count():return logs().count('Original menu sound requested | name MenuConfirm')
    previous=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',previous)
    report=dict(scope='Touch hit/events and authored sound; navigation incomplete',
                apk_sha256=hashlib.sha256(a.apk.read_bytes()).hexdigest(),scenarios=[],complete_menu=False)
    try:
        if not a.installed:assert 'Success' in adb('install','-r',str(a.apk))
        for shape in ('1080x1920','1080x2400','1968x2184'):
            adb('shell','wm','size',shape);launch()
            w,h=shot('before-'+shape+'.png');x,y=tap_options(w,h)
            adb('shell','input','tap',str(x),str(y))
            text=wait('Menu effect completed | file=sfx_menu_confirm.wav')
            assert 'kind 4 | name btn_MENU_OPTIONS | delivered 1' in text
            assert 'kind 6 | name btn_MENU_OPTIONS | delivered 1' in text
            assert 'kind 2 | name btn_MENU_OPTIONS | delivered 1' in text
            assert count()==1, 'Expected one tap confirmation, got '+str(count())+'; inspect Menu input trace in failure.log'
            shot('after-'+shape+'.png')
            # Outside the authored canvas must not activate its controls.
            adb('shell','input','tap','10','10');time.sleep(.3);assert count()==1
            # Start on Options, drag into the empty central area and release.
            focus_outs=logs().count('kind 1 | name btn_MENU_OPTIONS | delivered 1')
            adb('shell','input','swipe',str(x),str(y),str(w//2),str(h//2),'300')
            # 0x80 can transfer focus to a non-mouse authored shape, so this
            # path need not emit Options onReleaseOutside (kind 7). It must
            # leave Options focus and must not activate any release handler.
            until=time.monotonic()+10
            while logs().count('kind 1 | name btn_MENU_OPTIONS | delivered 1')==focus_outs and time.monotonic()<until:time.sleep(.15)
            assert logs().count('kind 1 | name btn_MENU_OPTIONS | delivered 1')==focus_outs+1
            time.sleep(.3);assert count()==1
            # LoadMainMenu's original 0x84 behavior updates focus while a
            # held pointer moves between buttons. Release uses the new focus.
            ch=min(h,w*2//3)
            info_y=int((h-ch)/2+290*ch/320)
            time.sleep(2)
            adb('shell','input','motionevent','DOWN',str(x),str(y))
            adb('shell','input','motionevent','MOVE',str(x),str(info_y))
            wait('kind 10 | name btn_MENU_INFO | delivered 1')
            adb('shell','input','motionevent','UP',str(x),str(info_y))
            text=wait('kind 6 | name btn_MENU_INFO | delivered 1')
            assert 'kind 10 | name btn_MENU_INFO | delivered 1' in text
            until=time.monotonic()+10
            while count()<2 and time.monotonic()<until:time.sleep(.15)
            assert count()==2
            (a.output/('input-'+shape+'.log')).write_text(logs())
            report['scenarios'].append(dict(size=shape,actual_surface=[w,h],point=[x,y],
                independent_shared_player_and_inactive_options=True,
                press_release_clicked=True,authored_confirm=True,margin_no_activation=True,release_outside_no_activation=True,
                held_drag_changes_focus_and_releases_info=True))
        # Resize the same retained process; exercise updated hit coordinates.
        start=adb('shell','pidof','com.example.dh2').strip()
        adb('shell','wm','size','1080x2400')
        wait('Original health panel submitted | viewport 2400 1080')
        time.sleep(2)
        end=adb('shell','pidof','com.example.dh2').strip();assert start==end
        w,h=shot('live-resize.png');x,y=tap_options(w,h);before=count()
        adb('shell','input','tap',str(x),str(y))
        until=time.monotonic()+10
        while count()==before and time.monotonic()<until:time.sleep(.2)
        assert count()==before+1
        (a.output/'live-resize.log').write_text(logs())
        report['live_resize_retains_process_and_hit']=True
        # Hold a real DOWN event, then background/resume the same app. A
        # canceled gesture must not turn into an authored onRelease callback.
        time.sleep(2)
        before=count();presses=logs().count('kind 4 | name btn_MENU_OPTIONS')
        adb('shell','input','motionevent','DOWN',str(x),str(y))
        until=time.monotonic()+10
        while logs().count('kind 4 | name btn_MENU_OPTIONS')==presses and time.monotonic()<until:time.sleep(.15)
        assert logs().count('kind 4 | name btn_MENU_OPTIONS')==presses+1
        adb('shell','input','keyevent','KEYCODE_HOME');time.sleep(.5)
        # Close the shell's injected pointer stream after Android canceled
        # delivery to the app window, just as lifting the finger would do.
        adb('shell','input','motionevent','UP',str(x),str(y))
        assert count()==before
        adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','-f','0x10200000')
        time.sleep(2);assert count()==before
        adb('shell','input','tap',str(x),str(y))
        until=time.monotonic()+10
        while count()==before and time.monotonic()<until:time.sleep(.15)
        assert count()==before+1
        (a.output/'cancel-resume.log').write_text(logs())
        report['pause_cancels_held_touch_without_release']=True
        report['post_resume_tap_works']=True
        report['status']='PASS'
    except Exception as e:
        report['status']='FAIL';report['error']=str(e)
        (a.output/'failure.log').write_text(logs());raise
    finally:
        adb('shell','wm','size',override.group(1) if override else 'reset')
        launch(trace=False);shot('restored-main.png')
        (a.output/'input-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))


if __name__=='__main__':main()
