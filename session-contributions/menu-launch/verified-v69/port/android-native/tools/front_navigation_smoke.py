"""Owned main -> Info -> Back path on the isolated visible emulator.

Checks actual authored touch delivery, navigation logs, resize and repeat.
Does not certify other destinations, settings, or original stack animations.
"""
import argparse,hashlib,json,re,subprocess,time
from pathlib import Path
from PIL import Image

def main():
    p=argparse.ArgumentParser();p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    cmd=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
    def adb(*args):return subprocess.check_output(cmd+list(args),text=True,timeout=45)
    def logs():
        pid=adb('shell','pidof','com.example.dh2').strip()
        return adb('logcat','-d','--pid='+pid,'-v','brief')
    def wait(marker,count=1):
        end=time.monotonic()+60
        while time.monotonic()<end:
            text=logs()
            assert 'Original menu input failed:' not in text and 'FATAL EXCEPTION' not in text,text[-5000:]
            if text.count(marker)>=count:return text
            time.sleep(.2)
        raise AssertionError('Missing '+marker+'\n'+logs()[-5000:])
    def launch():
        adb('shell','am','force-stop','com.example.dh2')
        adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
        wait('Original front/HUD screen submitted | screen main');time.sleep(2)
    def shot(name):
        file=a.output/(name+'.png');file.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
        with Image.open(file) as im:return im.size
    def tap(w,h,x,y):
        cw=min(w,h*3//2);ch=min(h,w*2//3)
        adb('shell','input','tap',str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320)))
    previous=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',previous)
    report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'scope':'Main/Info/Back only; other destinations and stack animations pending','scenarios':[],'complete_menu':False}
    push='Owned menu navigation | push menu_info | depth 2'
    pop='Owned menu navigation | pop menu_info | current menu_MainMenu | depth 1'
    try:
        assert 'Success' in adb('install','-r',str(a.apk))
        time.sleep(1)
        for shape in ['1080x1920','1080x2400','1968x2184']:
            adb('shell','wm','size',shape);launch()
            w,h=shot('main-'+shape)
            for n in range(1,3):
                tap(w,h,410,290);wait(push,n);time.sleep(1)
                shot('info-'+shape+'-'+str(n))
                tap(w,h,18,18);wait(pop,n);time.sleep(1)
            text=logs();assert text.count(push)==2 and text.count(pop)==2
            (a.output/('navigation-'+shape+'.log')).write_text(text)
            report['scenarios'].append({'size':shape,'surface':[w,h],'cycles':2,'info_and_back':True})
        tap(w,h,410,290);wait(push,3)
        pid=adb('shell','pidof','com.example.dh2').strip()
        adb('shell','wm','size','1080x2400');time.sleep(2)
        w,h=shot('info-live-resize');assert adb('shell','pidof','com.example.dh2').strip()==pid
        tap(w,h,18,18);wait(pop,3)
        report['info_resize_retains_process_and_back']=True
        report['status']='PASS'
    except Exception as e:
        report['status']='FAIL';report['error']=str(e)
        (a.output/'failure.log').write_text(logs());shot('failure');raise
    finally:
        adb('shell','wm','size',override[1] if override else 'reset');launch();shot('restored-main')
        (a.output/'navigation-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':main()
