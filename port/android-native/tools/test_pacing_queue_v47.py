"""Actual staged queue ownership and prior real-controller regressions; real SDK compile."""
from pathlib import Path
import hashlib,json,runpy,subprocess
root=Path(__file__).resolve().parents[3];runpy.run_path(str(root/'port/android-native/tools/prepare_pacing_queue_v47.py'))
out=root/'port/android-native/reports/pacing-request-lifetime-v47';package=root/'port/android-native/app/src/main/java/com/example/dh2'
jdk=Path('C:/Program Files/Android/Android Studio/jbr/bin');sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/platforms/android-37.0/android.jar')
host=out/'host-classes';host.mkdir(exist_ok=True);android=out/'android-classes';android.mkdir(exist_ok=True)
tests=[root/'port/android-native/tools/tests'/name for name in ['FramePacingQueueV47Test.java','FramePacingV44Test.java','FramePacingReportV46Test.java']]
staged=[out/'FramePacingControllerV44.java',out/'FramePacingReportV46.java']
commands=[[jdk/'javac.exe','-J-Xmx256m','-Xlint:all','-Werror','-d',host,*staged,*tests]]
commands += [[jdk/'java.exe','-Xms16m','-Xmx64m','-cp',host,'com.example.dh2.'+p.stem] for p in tests]
commands += [[jdk/'javac.exe','-J-Xmx256m','-Xlint:all','-Werror','-classpath',sdk,'-d',android,*staged,package/'VsyncSurfaceViewV44.java']]
commands += [[jdk/'javac.exe','-J-Xmx256m','-classpath',sdk,'-d',android,*staged,*[p for p in package.glob('*.java') if p.name not in [p.name for p in staged]]]]
receipts=[]
for command in commands:
 p=subprocess.run(list(map(str,command)),capture_output=True,text=True,timeout=30)
 receipts.append({'command':list(map(str,command)),'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr});print(p.returncode,p.stdout,p.stderr[:6500])
 (out/'receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'receipts':receipts,'source_sha256':{p.relative_to(root).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in [*staged,*tests]}},indent=2)+'\n');assert not p.returncode
