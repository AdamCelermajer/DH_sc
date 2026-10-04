"""Check the real authored loading movie on the dedicated visible emulator."""
import argparse, hashlib, json, re, subprocess, time
from pathlib import Path
from PIL import Image, ImageChops

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--apk',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();args.output.mkdir(parents=True,exist_ok=True)
    cmd=['C:/Users/adamc/AppData/Local/Android/Sdk/platform-tools/adb.exe','-P','5038','-s','emulator-5580']
    def adb(*parts):return subprocess.check_output(cmd+list(parts),text=True,timeout=30)
    def logs():
        r=subprocess.run(cmd+['shell','pidof','com.example.dh2'],text=True,capture_output=True,timeout=30)
        if r.returncode not in (0,1):raise RuntimeError(r.stderr)
        return adb('logcat','-d','--pid='+r.stdout.strip(),'-v','brief') if r.stdout.strip() else ''
    def shot(path):path.write_bytes(subprocess.check_output(cmd+['exec-out','screencap','-p'],timeout=30))
    previous=adb('shell','wm','size');override=re.search(r'Override size: (\d+x\d+)',previous)
    receipt=dict(scope='Original loading animation and source frame-owner delivery only',full_menu_functionality=False,apk_sha256=hashlib.sha256(args.apk.read_bytes()).hexdigest(),scenarios=[])
    try:
        if 'Success' not in adb('install','-r',str(args.apk)):raise AssertionError('Install failed')
        time.sleep(1)
        for size in ('1080x1920','1080x2400','1968x2184'):
            adb('shell','wm','size',size)
            for attempt in range(2):
                adb('shell','am','force-stop','com.example.dh2')
                launch=adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity','--es','front_screen','loading')
                if 'Activity: com.example.dh2/.MainActivity' in launch:break
                if 'PackageUpdateActivity' not in launch:raise AssertionError(launch)
            end=time.monotonic()+35
            while time.monotonic()<end:
                text=logs()
                if 'FATAL EXCEPTION' in text or 'Authored UI draw failed' in text:raise AssertionError(text[-5000:])
                if 'Original front/HUD screen submitted | screen loading' in text:break
                time.sleep(.2)
            else:raise AssertionError('Loading movie did not submit')
            if 'Original source frame/history bound before shared/root construction' not in text:raise AssertionError('Missing real frame owner')
            first=args.output/('loading-'+size+'-a.png');second=args.output/('loading-'+size+'-b.png')
            shot(first);time.sleep(.65);shot(second)
            with Image.open(first) as a,Image.open(second) as b:
                # Android bars are hidden; compare actual app surface pixels.
                diff=ImageChops.difference(a.convert('RGB'),b.convert('RGB'))
                box=diff.getbbox()
                if box is None:raise AssertionError('Original loading movie did not visibly advance')
                dimensions=a.size
            (args.output/('loading-'+size+'.log')).write_text(logs())
            receipt['scenarios'].append(dict(size=size,screenshot=dimensions,changed_bounds=box,original_frame_owner=True,visible_animation=True))
            print('PASS original loading animation:',size,flush=True)
        receipt['validation']='PASS'
    except Exception as error:
        receipt['validation']='FAIL';receipt['error']=str(error);raise
    finally:
        adb('shell','wm','size',override.group(1) if override else 'reset')
        (args.output/'loading-validation.json').write_text(json.dumps(receipt,indent=2)+'\n')

if __name__=='__main__':main()
