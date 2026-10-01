#!/usr/bin/env python3
"""Build the private DH2 test wrapper from reviewed sources and pinned inputs."""
from pathlib import Path
import os,subprocess,shutil,zipfile,json,hashlib,xml.etree.ElementTree as ET

ROOT=Path(__file__).resolve().parent
WORK=ROOT.parent
ZB=WORK/'research/ZettaBridge'
SDK=WORK/'android-sdk'
BT=SDK/'build-tools/35.0.0'
ANDROID=SDK/'platforms/android-35/android.jar'
OUT=ROOT/'out';OUT.mkdir(exist_ok=True)
JDK=next((WORK/'toolchains').glob('jdk-17*'))
ENV=dict(os.environ,JAVA_HOME=str(JDK));ENV['PATH']=str(JDK/'bin')+os.pathsep+ENV['PATH']

def run(args):
    print('Running',str(args[0]),flush=True)
    subprocess.run([str(a) for a in args],check=True,env=ENV)

# One extra guest dex contains only the new path resolver; original game classes
# remain in the apktool-rebuilt classes.dex.
guest_classes=OUT/'guest-classes';guest_classes.mkdir(exist_ok=True)
run([JDK/'bin/javac','-source','8','-target','8','-cp',ANDROID,'-d',guest_classes,*sorted((ROOT/'guest-java').rglob('*.java'))])
guest_dex=OUT/'guest-dex';guest_dex.mkdir(exist_ok=True)
run([BT/'d8','--min-api','21','--lib',ANDROID,'--output',guest_dex,*sorted(guest_classes.rglob('*.class'))])
assets=OUT/'assets'
if assets.exists():shutil.rmtree(assets)
shutil.copytree(ZB/'build/launcher/assets',assets,ignore=shutil.ignore_patterns('.rsync-tmp'))
(assets/'dh2').mkdir()
game=assets/'dh2/game.apk'
with zipfile.ZipFile(ROOT/'game-unsigned.apk') as source_game:
    assert source_game.testzip() is None
    assert {'AndroidManifest.xml','classes.dex','lib/armeabi-v7a/libDungeonHunter2.so'}.issubset(source_game.namelist()), 'Incomplete nested game input'
shutil.copyfile(ROOT/'game-unsigned.apk',game)
with zipfile.ZipFile(game,'a',zipfile.ZIP_DEFLATED) as z:z.write(guest_dex/'classes.dex','classes2.dex')

classes=OUT/'classes';classes.mkdir(exist_ok=True)
sources=sorted((ZB/'android/launcher/app/src/main/java').rglob('*.java'))+sorted((ROOT/'java').rglob('*.java'))
classpath=str(ANDROID)+os.pathsep+str(WORK/'downloads/hiddenapibypass.jar')
run([JDK/'bin/javac','-source','17','-target','17','-cp',classpath,'-d',classes,*sources])
jar=OUT/'launcher.jar'
run([JDK/'bin/jar','cf',jar,'-C',classes,'.'])
dex=OUT/'dex';dex.mkdir(exist_ok=True)
run([BT/'d8','--release','--min-api','29','--lib',ANDROID,'--output',dex,jar,WORK/'downloads/hiddenapibypass.jar'])

# Generate fully qualified component names before changing the application ID.
NS='http://schemas.android.com/apk/res/android';TOOLS='http://schemas.android.com/tools'
ET.register_namespace('android',NS)
a=lambda n:'{'+NS+'}'+n
tree=ET.parse(ZB/'android/launcher/app/src/main/AndroidManifest.xml');manifest=tree.getroot()
manifest.set('package','local.dh2.fold7');manifest.set(a('versionCode'),'5');manifest.set(a('versionName'),'1.0-test5')
ET.SubElement(manifest,'uses-sdk',{a('minSdkVersion'):'29',a('targetSdkVersion'):'35'})
for perm in ['android.permission.ACCESS_WIFI_STATE','android.permission.CHANGE_WIFI_STATE','android.permission.BLUETOOTH','android.permission.BLUETOOTH_ADMIN']:
    ET.SubElement(manifest,'uses-permission',{a('name'):perm})
app=manifest.find('application');app.set(a('label'),'Dungeon Hunter 2 - Fold7 Test');app.set(a('icon'),'@drawable/dh2_icon');app.set(a('extractNativeLibs'),'true');app.set(a('usesCleartextTraffic'),'true')
ET.SubElement(app,'uses-library',{a('name'):'org.apache.http.legacy',a('required'):'false'})
for node in [app,*list(app)]:
    name=node.get(a('name'),'')
    if name.startswith('.'):node.set(a('name'),'com.zettabridge.launcher'+name)
    if node.get(a('taskAffinity')):node.set(a('taskAffinity'),node.get(a('taskAffinity')).replace('com.zettabridge.launcher','local.dh2.fold7'))
    if node.get('{'+TOOLS+'}node')=='remove':app.remove(node)
for node in app.findall('activity'):
    if node.get(a('name'))=='com.zettabridge.launcher.LibraryActivity':
        node.set(a('name'),'com.zettabridge.launcher.Dh2Activity')
        node.set(a('configChanges'),'orientation|screenSize|screenLayout|smallestScreenSize|keyboardHidden')
    else:node.set(a('exported'),'false')
manifest_path=OUT/'AndroidManifest.xml';tree.write(manifest_path,encoding='utf-8',xml_declaration=True)

# Ship upstream licensing notices with the personal compatibility build.
notices=assets/'licenses';notices.mkdir(exist_ok=True)
shutil.copyfile(ZB/'LICENSE',notices/'ZettaBridge.txt')
shutil.copyfile(ZB/'third_party/README.md',notices/'Third-party.txt')
for i,p in enumerate((ZB/'third_party/dynarmic').glob('LICENSE*')):
    if p.is_file():shutil.copyfile(p,notices/('Dynarmic-'+str(i)+'.txt'))
for p in (ZB/'third_party/dynarmic/externals').rglob('*'):
    if p.is_file() and p.name.upper().startswith(('LICENSE','COPYING')):
        relative=str(p.relative_to(ZB/'third_party/dynarmic/externals')).replace('/','_')
        shutil.copyfile(p,notices/('Dependency-'+relative))
for p in (ROOT/'notices').glob('*'):
    if p.is_file():shutil.copyfile(p,notices/p.name)
with zipfile.ZipFile(WORK/'downloads/hiddenapibypass-6.1.aar') as z:
    for name in z.namelist():
        if 'LICENSE' in name.upper() or 'NOTICE' in name.upper():
            (notices/Path(name).name).write_bytes(z.read(name))

unsigned=OUT/'dh2-unsigned.apk'
res=OUT/'res/drawable';res.mkdir(parents=True,exist_ok=True)
shutil.copyfile(WORK/'patched/res/drawable/icon.png',res/'dh2_icon.png')
compiled_res=OUT/'compiled-res.zip'
run([BT/'aapt2','compile','--dir',OUT/'res','-o',compiled_res])
run([BT/'aapt2','link','-o',unsigned,'--manifest',manifest_path,'-I',ANDROID,'-A',assets,compiled_res])
with zipfile.ZipFile(unsigned,'a',zipfile.ZIP_DEFLATED) as z:
    for p in dex.glob('*.dex'):z.write(p,p.name)
    for p in (ZB/'build/launcher/jniLibs/arm64-v8a').glob('*.so'):z.write(p,'lib/arm64-v8a/'+p.name)
aligned=OUT/'dh2-aligned.apk'
run([BT/'zipalign','-f','-P','16','4',unsigned,aligned])
deliverable=WORK.parent/'deliverables/Dungeon-Hunter-2-Fold7-test5.apk'
deliverable.parent.mkdir(parents=True,exist_ok=True)
run([BT/'apksigner','sign','--ks',WORK/'dh2-local-test.p12','--ks-key-alias','dh2-local-test','--ks-pass','pass:dh2-local-test-only','--key-pass','pass:dh2-local-test-only','--out',deliverable,aligned])
run([BT/'apksigner','verify','--verbose',deliverable])
run([BT/'zipalign','-c','-P','16','4',deliverable])
(ROOT/'build-result.json').write_text(json.dumps({'apk':str(deliverable),'sha256':hashlib.sha256(deliverable.read_bytes()).hexdigest(),'bytes':deliverable.stat().st_size,'upstream_commit':subprocess.check_output(['git','-C',str(ZB),'rev-parse','HEAD'],text=True).strip(),'host_abi':'arm64-v8a','guest_abi':'armeabi-v7a','device_tested':False,'gameplay_tested':False},indent=2)+'\n')
print('Built signed ARM64 test package:',deliverable,flush=True)
