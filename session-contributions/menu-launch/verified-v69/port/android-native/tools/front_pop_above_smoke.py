"""Exercise the actual NativePopAllAbove callback on visible emulator5580."""
from pathlib import Path
import argparse,subprocess,time,json,hashlib
from PIL import Image

def main():
    p=argparse.ArgumentParser();p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    cmd=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
    def adb(*args):return subprocess.check_output(cmd+list(args),encoding='utf-8',timeout=45)
    def logs():return adb('logcat','-d','--pid='+adb('shell','pidof','com.example.dh2').strip(),'-v','brief')
    def shot(name):
        f=a.output/(name+'.png');f.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
        with Image.open(f) as im:
            w,h=im.size
            sample=im.crop((w//5,h//5,4*w//5,4*h//5)).resize((64,64))
            assert len(set(sample.get_flattened_data()))>500,'Blank presentation: '+name
            return w,h
    def tap(x,y):
        w,h=shot('point');cw=min(w,h*3//2);ch=min(h,w*2//3)
        adb('shell','input','tap',str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320)));time.sleep(.3)
    def wait(marker,before):
        end=time.monotonic()+20
        while time.monotonic()<end:
            s=logs()
            if s.count(marker)>before:return s
            time.sleep(.2)
        raise RuntimeError('Missing fresh '+marker)
    def probe(name):
        marker='Original menu sound probe dispatched | probe '+name
        before=logs().count(marker)
        adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe',name)
        wait(marker,before)
    def inspect(expected):
        marker='Front state inspected | selected '
        before=logs().count(marker)
        adb('shell','am','broadcast','-a','com.example.dh2.DEBUG_MENU_SOUND','-p','com.example.dh2','--es','probe','inspect-front')
        s=wait(marker,before);value=s[s.rfind(marker)+len(marker):].split(' |')[0]
        assert value=='_root.'+expected,(value,expected)
    def enter_class():
        tap(410,94);time.sleep(1.6);inspect('menu_EnterName')
        tap(40,219);tap(240,126);time.sleep(1.6);inspect('menu_SelectClass')
    report=dict(status='FAIL',apk_sha256=hashlib.sha256(a.apk.read_bytes()).hexdigest(),scope='NativePopAllAbove source argument guards, missing/current target, one/two-level stack unwinding, real Back after unwind and rapid resize; campaign creation/game launch still incomplete')
    oldwm=adb('shell','wm','size');assert 'Override' not in oldwm,'Test requires physical initial dimensions'
    try:
        adb('install','-r',str(a.apk));time.sleep(1)
        adb('shell','am','force-stop','com.example.dh2');adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main');time.sleep(6)
        inspect('menu_MainMenu');enter_class();shot('class-before')
        before=logs().count('Owned menu navigation | pop menu_')
        for name in ['pop-above-invalid-number','pop-above-invalid-arity','pop-above-absent']:
            probe(name);inspect('menu_SelectClass')
        assert logs().count('Owned menu navigation | pop menu_')==before
        probe('pop-above-name');time.sleep(1.6);inspect('menu_EnterName');shot('unwound-name')
        before=logs().count('Owned menu navigation | pop menu_')
        probe('pop-above-name');inspect('menu_EnterName')
        assert logs().count('Owned menu navigation | pop menu_')==before
        tap(18,18);time.sleep(1.6);inspect('menu_MainMenu')
        enter_class();probe('pop-above-main');time.sleep(.1)
        adb('shell','wm','size','1968x2184');time.sleep(2);inspect('menu_MainMenu');shot('unwound-main-resized')
        s=logs();assert 'pop above menu_MainMenu | current menu_MainMenu | depth 1' in s
        assert 'pop above menu_EnterName | current menu_EnterName | depth 2' in s
        assert 'Original UI display failed:' not in s and 'Original menu input failed:' not in s
        report.update(status='PASS',argument_guards='PASS',missing_target='PASS',current_target='PASS',one_level='PASS',two_levels='PASS',back_after_unwind='PASS',rapid_resize='PASS')
    finally:
        adb('shell','wm','size','reset');time.sleep(2);shot('visible-main')
        (a.output/'pop-above-validation.log').write_text(logs(),encoding='utf-8')
        (a.output/'pop-above-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))
if __name__=='__main__':main()
