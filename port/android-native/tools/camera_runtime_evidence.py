"""Replay captured Android frustum snapshots against the original ARM graph.

Capture is performed by crypt_script_runtime_smoke.py. This offline checker
validates its APK/PID/serial transcript and report; it does not itself query adb.
"""
import argparse, hashlib, json, math, re, struct, sys, zipfile
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-camera/tests'))
from run_frustum_runtime import SourceCpu
import run_frustum_bounds as bounds
import run_frustum_host as planes

PATTERN=re.compile(r'I/DH2Native\(\s*(\d+)\): Source camera frame \| (\d+) \| (\d+)x(\d+) \| matrix((?: [0-9a-f]{8}){16}) \| frustum((?: [0-9a-f]{8}){33})')
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--apk',type=Path,required=True)
    p.add_argument('--installed-apk',type=Path,required=True)
    p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--logcat',type=Path,required=True)
    p.add_argument('--runtime-report',type=Path,required=True)
    p.add_argument('--adb-transcript',type=Path,required=True)
    p.add_argument('--serial',required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    assert sha(a.apk)==sha(a.installed_apk),'installed APK differs from tested build'
    assert sha(a.original_elf)==bounds.ELF_SHA,'wrong original ELF'
    runtime=json.loads(a.runtime_report.read_bytes())
    transcript=json.loads(a.adb_transcript.read_bytes())
    assert runtime['validation']=='PASS' and runtime['serial']==a.serial
    assert runtime['apk_sha256']==runtime['installed_apk_sha256']==sha(a.apk)
    assert runtime['page_size']==16384 and runtime['api_level']==37
    assert runtime['logcat_sha256']==sha(a.logcat),'detached or truncated camera log'
    assert runtime['adb_transcript_sha256']==sha(a.adb_transcript),'transcript differs from capture report'
    boundaries=runtime['launch_log_boundaries']
    pids={row['pid'] for row in boundaries}
    assert pids and all(pid.isdecimal() for pid in pids)
    successful=[row for row in transcript if row['returncode']==0]
    package='com.example.dh2'
    packages=[row['stdout'].strip().removeprefix('package:') for row in successful
              if row['args']==['shell','pm','path',package]]
    assert packages and any(row['args']==['shell','sha256sum',path] and
                            row['stdout'].split()[0]==sha(a.apk)
                            for path in packages for row in successful)
    for boundary in boundaries:
        assert any(row['args']==['shell','pidof',package] and
                   row['stdout'].strip()==boundary['pid'] for row in successful)
        assert any(row['args']==['logcat','-d','-T',boundary['epoch_since'],
                                '--pid='+boundary['pid'],'-v','brief'] for row in successful)
    log_text=a.logcat.read_text(encoding='utf-8',errors='strict')
    normalized_log_sha=hashlib.sha256(log_text.strip().encode('utf-8')).hexdigest()
    assert any(row['args'][0]=='logcat' and row.get('stdout_utf8_sha256')==normalized_log_sha
               for row in successful),'log is not a recorded logcat response'
    module=ROOT/'port/engine-camera'
    bm=json.loads((module/'reference/frustum-bounds/original-functions.json').read_text())
    pm=json.loads((module/'reference/frustum-producer/original-functions.json').read_text())
    bounds.verify_manifest(a.original_elf,bm);planes.verify_manifest(a.original_elf,pm)
    rows=[pm['functions'][0],*bm['functions'],*bm['supporting_functions']]
    cpu=SourceCpu(a.original_elf,False,{'functions':rows})
    matrix_address,frustum_address=cpu.data+0x10000,cpu.data+0x20000
    snapshots=[]
    for match in PATTERN.finditer(log_text):
        pid=match[1]
        assert pid in pids,'camera log PID is outside captured app launches'
        frame,width,height=int(match[2]),int(match[3]),int(match[4])
        matrix=[int(word,16) for word in match[5].split()]
        native=[int(word,16) for word in match[6].split()]
        assert frame>0 and width>0 and height>0
        cpu.uc.mem_write(matrix_address,struct.pack('<16I',*matrix))
        cpu.uc.mem_write(frustum_address,struct.pack('<33I',*native[:3],*([0]*30)))
        cpu.invoke(planes.SYMBOL,[frustum_address,matrix_address])
        original=list(struct.unpack('<33I',cpu.uc.mem_read(frustum_address,132)))
        nan_payload_only=0
        for index,(x,y) in enumerate(zip(original,native)):
            if math.isnan(bounds.from_bits(x)) or math.isnan(bounds.from_bits(y)):
                assert math.isnan(bounds.from_bits(x)) and math.isnan(bounds.from_bits(y)),(frame,index,x,y)
                nan_payload_only+=int(x!=y)
            else:assert x==y,(frame,index,f'{x:08x}',f'{y:08x}')
        snapshots.append({'pid':pid,'frame':frame,'viewport':[width,height],'matrix':matrix,
                          'native_frustum':native,'matched':True,
                          'NaN_payload_only_differences':nan_payload_only})
    assert snapshots,'no actual Android camera snapshots found'
    assert len(snapshots)==runtime['source_camera_snapshot_count'],'captured camera snapshot count differs'
    libraries=[]
    with zipfile.ZipFile(a.apk) as archive:
        for abi in ('arm64-v8a','x86_64'):
            name=f'lib/{abi}/libdh2_engine_camera.so';raw=archive.read(name)
            assert raw[:6]==b'\x7fELF\x02\x01'
            assert struct.unpack_from('<H',raw,18)[0]=={'arm64-v8a':183,'x86_64':62}[abi]
            phoff=struct.unpack_from('<Q',raw,32)[0]
            phsize,phcount=struct.unpack_from('<HH',raw,54)
            alignments=[struct.unpack_from('<Q',raw,phoff+i*phsize+48)[0]
                        for i in range(phcount) if struct.unpack_from('<I',raw,phoff+i*phsize)[0]==1]
            assert alignments and min(alignments)>=16384
            libraries.append({'path':name,'sha256':hashlib.sha256(raw).hexdigest(),
                              'minimum_load_alignment':min(alignments)})
    report={'validation':'PASS','scope':'Actual Android camera snapshots match the unpatched original setFrom/bounds/intersection graph. Renderer follow/view/projection inputs remain development producers; native scene registration, Character culling and enemy AI are not proved here. Scalar imported FP routines are IEEE models; NaN payload identity and historical Bionic implementation are not claimed.',
            'apk_sha256':sha(a.apk),'installed_apk_sha256':sha(a.installed_apk),
            'original_elf_sha256':bounds.ELF_SHA,'logcat_sha256':sha(a.logcat),
            'runtime_report_sha256':sha(a.runtime_report),'adb_transcript_sha256':sha(a.adb_transcript),
            'serial':a.serial,'api_level':runtime['api_level'],'page_size':runtime['page_size'],
            'capture_scope':'Offline verification of the smoke runner report, package hash command and PID/time-bounded logcat transcript. Capture provenance depends on that runner; this checker does not perform live adb capture.',
            'snapshot_count':len(snapshots),'mismatches':0,'libraries':libraries,
            'checker_sha256':sha(Path(__file__).resolve()),'snapshots':snapshots}
    a.output.parent.mkdir(parents=True,exist_ok=True)
    a.output.write_bytes((json.dumps(report,indent=2)+'\n').encode())
    print(json.dumps({'validation':'PASS','snapshot_count':len(snapshots),'mismatches':0}))
if __name__=='__main__':main()
