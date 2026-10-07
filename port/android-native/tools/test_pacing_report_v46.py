"""Pure one-shot export evidence and full real-SDK Java compile, no APK/device."""
from pathlib import Path
import hashlib,json,runpy,subprocess
root=Path(__file__).resolve().parents[3];runpy.run_path(str(root/'port/android-native/tools/prepare_pacing_report_v46.py'))
out=root/'port/android-native/reports/pacing-report-v46';package=root/'port/android-native/app/src/main/java/com/example/dh2'
jdk=Path('C:/Program Files/Android/Android Studio/jbr/bin');sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/platforms/android-37.0/android.jar')
host=out/'host-classes';host.mkdir(exist_ok=True);android=out/'android-classes';android.mkdir(exist_ok=True)
files=[package/'FramePacingControllerV44.java',package/'FramePacingReportV46.java',root/'port/android-native/tools/tests/FramePacingReportV46Test.java']
commands=[[jdk/'javac.exe','-J-Xmx256m','-Xlint:all','-Werror','-d',host,*files],
 [jdk/'java.exe','-Xmx64m','-cp',host,'com.example.dh2.FramePacingReportV46Test'],
 [jdk/'javac.exe','-J-Xmx256m','-classpath',sdk,'-d',android,*[p for p in package.glob('*.java') if p.name!='MainActivity.java'],out/'MainActivity.java']]
receipts=[]
for command in commands:
 p=subprocess.run(list(map(str,command)),capture_output=True,text=True,timeout=30)
 receipts.append({'command':list(map(str,command)),'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr});print(p.returncode,p.stdout,p.stderr[:4000])
 (out/'receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'receipts':receipts,'source_sha256':{p.relative_to(root).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in [*files,out/'MainActivity.java']}},indent=2)+'\n');assert not p.returncode
