"""Visible shared Help/credits navigation, text, resize and lifecycle checks.

Scope excludes Options settings, online actions and original stack animations.
"""
import argparse,hashlib,json,re,subprocess,time
from pathlib import Path
from PIL import Image,ImageChops

def main():
    p=argparse.ArgumentParser();p.add_argument('--apk',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    cmd=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-P','5038','-s','emulator-5580']
    def adb(*args):return subprocess.check_output(cmd+list(args),text=True,encoding='utf-8',timeout=45)
    def logs():return adb('logcat','-d','--pid='+adb('shell','pidof','com.example.dh2').strip(),'-v','brief')
    def wait(marker,count=1):
        end=time.monotonic()+60
        while time.monotonic()<end:
            text=logs()
            assert 'Original menu input failed:' not in text and 'FATAL EXCEPTION' not in text,text[-6000:]
            if text.count(marker)>=count:return text
            time.sleep(.2)
        raise AssertionError('Missing '+marker+'\n'+logs()[-6000:])
    def launch():
        adb('shell','am','force-stop','com.example.dh2')
        adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','main')
        wait('Original front/HUD screen submitted | screen main');time.sleep(2)
    def shot(name):
        file=a.output/(name+'.png');file.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
        with Image.open(file) as im:return im.copy().convert('RGB')
    surface=[2400,1080]
    def tap(x,y):
        w,h=surface;cw=min(w,h*3//2);ch=min(h,w*2//3)
        adb('shell','input','tap',str(round((w-cw)/2+x*cw/480)),str(round((h-ch)/2+y*ch/320)))
    def action(x,y,marker):
        before=logs().count(marker);tap(x,y);wait(marker,before+1);time.sleep(.35)
    def normalized(im):
        w,h=im.size;cw=min(w,h*3//2);ch=min(h,w*2//3)
        return im.crop(((w-cw)//2,(h-ch)//2,(w+cw)//2,(h+ch)//2)).resize((480,320))
    def text_bands(im):
        crop=normalized(im).crop((72,95,400,250));rows=[]
        for y in range(crop.height):
            count=sum(1 for x in range(crop.width) if all(v>limit for v,limit in zip(crop.getpixel((x,y)),(175,140,120))))
            if count>=5:rows.append(y)
        return sum(1 for i,y in enumerate(rows) if i==0 or y>rows[i-1]+1)
    previous=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',previous)
    report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'scope':'Shared Help topics, credits navigation/motion, real text rows, live resize and resume; settings/online/animations pending','complete_menu':False,'scenarios':[]}
    topics=[(1,150,115),(2,322,115),(3,150,151),(4,322,151),(5,150,190),(6,322,190),(7,240,230)]
    try:
        assert 'Success' in adb('install','-r',str(a.apk));time.sleep(1)
        for size in ['1080x1920','1080x2400','1968x2184']:
            adb('shell','wm','size',size);launch();surface[:]=shot('main-'+size).size
            action(410,290,'Owned menu navigation | push menu_info | depth 2')
            action(240,117,'Owned menu navigation | push menu_HelpButtons | depth 3')
            shot('help-buttons-'+size)
            checked=[]
            for number,x,y in topics:
                action(x,y,'Owned menu navigation | push menu_Help | depth 4')
                text=logs().lower()
                assert 'symbol menu_help_%02d_title | found 1'%number in text
                assert 'symbol menu_help_%02d_text'%number in text
                im=shot('help-%02d-'%number+size)
                bands=text_bands(im)
                if number==1:assert bands>=5,('Controls missing text rows',bands)
                checked.append({'topic':number,'visible_text_bands':bands})
                if number==5:
                    for suffix in ['b','c']:
                        action(439,175,'Original UI string delivered | symbol menu_help_05_text_'+suffix+' | found 1')
                        shot('help-05-'+suffix+'-'+size)
                    for suffix in ['b','a']:
                        action(40,175,'Original UI string delivered | symbol menu_help_05_text_'+suffix+' | found 1')
                        shot('help-05-return-'+suffix+'-'+size)
                    checked[-1]['pagination_a_b_c_b_a']=True
                action(18,18,'Owned menu navigation | pop menu_Help | current menu_HelpButtons | depth 3')
            action(18,18,'Owned menu navigation | pop menu_HelpButtons | current menu_info | depth 2')
            action(240,172,'Owned menu navigation | push menu_About | depth 3')
            first=shot('credits-a-'+size);time.sleep(1);second=shot('credits-b-'+size)
            crop=(75,95,405,240)
            changed=ImageChops.difference(normalized(first).crop(crop),normalized(second).crop(crop)).getbbox()
            assert changed,'Credits region has no movement'
            action(18,18,'Owned menu navigation | pop menu_About | current menu_info | depth 2')
            action(18,18,'Owned menu navigation | pop menu_info | current menu_MainMenu | depth 1')
            (a.output/('shared-'+size+'.log')).write_text(logs(),encoding='utf-8')
            report['scenarios'].append({'size':size,'surface':surface.copy(),'topics':checked,'credits_region_changes':True,'returns_to_main':True})
        action(410,290,'Owned menu navigation | push menu_info | depth 2')
        action(240,117,'Owned menu navigation | push menu_HelpButtons | depth 3')
        action(150,115,'Owned menu navigation | push menu_Help | depth 4')
        pid=adb('shell','pidof','com.example.dh2').strip()
        adb('shell','wm','size','1080x2400');time.sleep(2);surface[:]=shot('help-live-resize').size
        assert adb('shell','pidof','com.example.dh2').strip()==pid
        adb('shell','input','keyevent','KEYCODE_HOME');time.sleep(.5)
        adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','-f','0x10200000');time.sleep(2)
        assert adb('shell','pidof','com.example.dh2').strip()==pid
        shot('help-after-resume')
        action(18,18,'Owned menu navigation | pop menu_Help | current menu_HelpButtons | depth 3')
        action(18,18,'Owned menu navigation | pop menu_HelpButtons | current menu_info | depth 2')
        action(18,18,'Owned menu navigation | pop menu_info | current menu_MainMenu | depth 1')
        report['shared_resize_resume_and_return']=True;report['status']='PASS'
    except Exception as e:
        report['status']='FAIL';report['error']=str(e);(a.output/'failure.log').write_text(logs(),encoding='utf-8');shot('failure');raise
    finally:
        adb('shell','wm','size',override[1] if override else 'reset');launch();shot('restored-main')
        (a.output/'shared-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':main()
