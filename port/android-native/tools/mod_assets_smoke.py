"""Exercise real native asset overrides on the API37/16KiB development emulator.

Changes one mod file temporarily and restores any previous bytes. Does not clear
app data or Logcat. The changed spawn is a fan override, never original parity.
"""
import argparse, hashlib, json, pathlib, re, struct, subprocess, tempfile, time

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for key in ('adb','serial','apk','output'):p.add_argument('--'+key,required=True)
    a=p.parse_args();apk=pathlib.Path(a.apk);output=pathlib.Path(a.output);output.mkdir(parents=True,exist_ok=True)
    if not re.fullmatch(r'emulator-\d+',a.serial):raise SystemExit('Use the named development emulator')
    def adb(*args,check=True):
        r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=40)
        if check and r.returncode:raise RuntimeError(r.stdout+r.stderr)
        return r.stdout.strip()
    assert adb('shell','getprop','ro.build.version.sdk')=='37'
    assert adb('shell','getconf','PAGE_SIZE')=='16384'
    package='com.example.dh2';root='/sdcard/Android/data/'+package+'/files/mods'
    remote=root+'/worlds/crypt01.dwld'
    report={'validation':'FAIL','api':37,'page_size':16384,'serial':a.serial,'apk_sha256':hashlib.sha256(apk.read_bytes()).hexdigest(),'scope':'Modified world spawn, malformed override rejection and bundled recovery; not original-game parity'}
    def launch():
        adb('shell','am','force-stop',package)
        adb('shell','am','start','-W','-n',package+'/.MainActivity','--es','world','crypt01.dwld','--ez','enemy_ai','false')
        deadline=time.monotonic()+25
        while time.monotonic()<deadline:
            pid=adb('shell','pidof',package,check=False)
            if pid:
                logs=adb('logcat','-d','--pid='+pid,'-v','brief')
                if 'World ready |' in logs or 'World load failed' in logs:return pid,logs
            time.sleep(.2)
        raise RuntimeError('No world load result within25seconds')
    def position(logs):return list(map(float,re.findall(r'World ready .*?position ([\d.-]+) ([\d.-]+) ([\d.-]+)',logs)[-1]))
    with tempfile.TemporaryDirectory(prefix='dh2-mod-smoke-') as temporary:
        temp=pathlib.Path(temporary);backup=temp/'previous.dwld'
        probe=subprocess.run([a.adb,'-s',a.serial,'shell','test','-e',remote],capture_output=True)
        previous=probe.returncode==0
        if previous:adb('pull',remote,str(backup))
        try:
            assert 'Success' in adb('install','-r',str(apk))
            adb('shell','mkdir','-p',root+'/worlds')
            # This exact single path was explicitly selected and backed up.
            adb('shell','rm','-f',remote)
            pid,logs=launch();baseline=position(logs);report['baseline']=baseline
            import zipfile
            with zipfile.ZipFile(apk) as archive:raw=bytearray(archive.read('assets/worlds/crypt01.dwld'))
            authored=list(struct.unpack_from('<3f',raw,12));authored[0]+=100;struct.pack_into('<3f',raw,12,*authored)
            changed=temp/'changed.dwld';changed.write_bytes(raw);adb('push',str(changed),remote)
            pid,logs=launch();assert 'Mod asset loaded | worlds/crypt01.dwld | bytes 1048' in logs
            actual=position(logs);assert abs(actual[0]-baseline[0]-100)<.01 and abs(actual[1]-baseline[1])<.01
            report['modified_spawn']=actual;report['mod_sha256']=hashlib.sha256(raw).hexdigest()
            adb('shell','screencap','-p','/sdcard/dh2-mod-smoke.png');adb('pull','/sdcard/dh2-mod-smoke.png',str(output/'modified-spawn.png'))
            changed.write_bytes(b'bad-world');adb('push',str(changed),remote)
            pid,logs=launch();assert 'World descriptor rejected' in logs and 'World ready |' not in logs
            assert 'FATAL EXCEPTION' not in logs and 'Fatal signal' not in logs
            report['malformed_override_rejected']=True
            adb('shell','rm','-f',remote)
            pid,logs=launch();assert all(abs(x-y)<.01 for x,y in zip(position(logs),baseline))
            report['baseline_recovered']=True;report['validation']='PASS'
            (output/'mod-smoke.log').write_text(logs+'\n')
        finally:
            if previous:adb('push',str(backup),remote)
            else:adb('shell','rm','-f',remote)
            report['previous_mod_restored']=True
            (output/'mod-smoke.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':main()
