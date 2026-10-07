"""Run actual portable Java controller and compile thin Android bridge with real SDK."""
from pathlib import Path
import hashlib,json,runpy,subprocess
root=Path(__file__).resolve().parents[3];runpy.run_path(str(root/'port/android-native/tools/prepare_frame_pacing_v44.py'))
out=root/'port/android-native/reports/frame-pacing-v44'
java=Path('C:/Program Files/Android/Android Studio/jbr/bin/java.exe');javac=java.with_name('javac.exe')
sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/platforms/android-37.0/android.jar')
sources=root/'port/android-native/app/src/main/java';package=sources/'com/example/dh2'
controller=package/'FramePacingControllerV44.java';bridge=package/'VsyncSurfaceViewV44.java';test=root/'port/android-native/tools/tests/FramePacingV44Test.java'
receipts=[]
def run(command):
 p=subprocess.run([str(c) for c in command],capture_output=True,text=True,timeout=45)
 receipts.append({'command':[str(c) for c in command],'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
 (out/'java-receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':__doc__,'receipts':receipts},indent=2)+'\n')
 print(p.returncode,p.stdout,p.stderr[:12000]);assert not p.returncode
host=out/'host-classes';host.mkdir(exist_ok=True)
run([javac,'-J-Xmx256m','-Xlint:all','-Werror','-d',host,controller,test])
run([java,'-Xms16m','-Xmx64m','-cp',host,'com.example.dh2.FramePacingV44Test'])
android=out/'android-classes';android.mkdir(exist_ok=True)
run([javac,'-J-Xmx256m','-Xlint:all','-Werror','-classpath',sdk,'-d',android,controller,bridge])
# Existing app carries legacy Android deprecations; compile it as the project
# does, against the actual Android SDK, without writing/building an APK.
files=[p for p in sources.rglob('*.java') if p.name!='MainActivity.java']
run([javac,'-J-Xmx256m','-classpath',sdk,'-d',android,*files,out/'MainActivity.java'])
bindings={str(p.relative_to(root)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in [controller,bridge,test,out/'MainActivity.java',out/'native-wall-clock-contract.json']}
receipt=json.loads((out/'java-receipt.json').read_text());receipt['source_bindings_sha256']=bindings
(out/'java-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
