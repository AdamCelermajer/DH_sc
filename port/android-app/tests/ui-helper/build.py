#!/usr/bin/env python3
"""Build the local accessibility snapshot instrumentation using Android SDK tools."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import zipfile

HERE=Path(__file__).resolve().parent
def run(*argv):subprocess.run([str(x) for x in argv],check=True)
def main():
    p=argparse.ArgumentParser();p.add_argument('--sdk',type=Path,required=True);a=p.parse_args()
    sdk=a.sdk.resolve();tools=sdk/'build-tools/35.0.0'
    jar=sdk/'platforms/android-37.2/android.jar'
    if not jar.is_file():jar=sdk/'platforms/android-37.0/android.jar'
    build=HERE/'build';classes=build/'classes';dex=build/'dex'
    classes.mkdir(parents=True,exist_ok=True);dex.mkdir(exist_ok=True)
    java=Path(os.environ.get('JAVA_HOME','C:/Program Files/Java/jdk-23'))/'bin/javac.exe'
    run(java,'-source','8','-target','8','-Xlint:-options','-classpath',jar,'-d',classes,HERE/'Snapshot.java')
    run(tools/'d8.bat','--min-api','26','--output',dex,classes/'local/dh2/uitest/Snapshot.class')
    base=build/'base.apk'
    run(tools/'aapt2.exe','link','--manifest',HERE/'AndroidManifest.xml','-I',jar,'-o',base)
    with zipfile.ZipFile(base,'a')as apk:apk.write(dex/'classes.dex','classes.dex',compress_type=zipfile.ZIP_DEFLATED)
    aligned=build/'aligned.apk';run(tools/'zipalign.exe','-f','4',base,aligned)
    key=HERE.parents[1]/'build/debug.jks'
    if not key.is_file():raise FileNotFoundError('Build the source app first to create its local debug signing key')
    signed=build/'dh2-ui-helper.apk'
    run(tools/'apksigner.bat','sign','--ks',key,'--ks-key-alias','debug','--ks-pass','pass:android','--key-pass','pass:android','--out',signed,aligned)
    run(tools/'apksigner.bat','verify',signed)
    sha=lambda x:hashlib.sha256(x.read_bytes()).hexdigest()
    report={'test_only':True,'target_sdk':37,'apk_sha256':sha(signed),'bytes':signed.stat().st_size,
            'source_sha256':{x.name:sha(x)for x in (HERE/'Snapshot.java',HERE/'AndroidManifest.xml',Path(__file__))}}
    (HERE/'build-validation.json').write_text(json.dumps(report,indent=2)+'\n');print(signed)
if __name__=='__main__':main()
