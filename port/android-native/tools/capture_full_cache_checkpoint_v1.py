"""Capture the native all-in-one APK and its actual compiler/resource inputs.

This records packaging and source provenance. Playability/menus and hardware
acceptance are separate; a bundled level is not a completed playable level.
"""
import hashlib,io,json,shutil,struct,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
PROJECT=ROOT/'port/android-native'
NINJA=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\cmake\3.22.1\bin\ninja.exe')
def sha(raw):return hashlib.sha256(raw).hexdigest()
def file_sha(p):
    with p.open('rb') as f:return stream_sha(f)
def stream_sha(f):
    h=hashlib.sha256()
    while b:=f.read(1024*1024):h.update(b)
    return h.hexdigest()
def main():
    apk=PROJECT/'app/build/outputs/apk/debug/app-debug.apk'
    digest=file_sha(apk)
    checkpoint=PROJECT/f'build/checkpoints/dh2-native-full-cache-{digest[:8]}.apk'
    output=PROJECT/f'reports/native-full-cache-{digest[:8]}-build-capture.json'
    snapshot=PROJECT/f'build/checkpoints/dh2-native-full-cache-{digest[:8]}-source.zip'
    assert not checkpoint.exists() and not output.exists() and not snapshot.exists(),'Preserve accepted artifacts'
    main_path=ROOT/'port/level-world/reports/native-localized-skill-text-main-linked-host-audit-v1.json'
    cache_path=ROOT/'port/asset-payloads/reports/zip-asset-pack-v1-host-audit.json'
    main=json.loads(main_path.read_text());cache=json.loads(cache_path.read_text())
    assert main['validation']==cache['validation']=='PASS' and len(main['host_audits'])==129 and cache['files']==6833
    assert main['sanitizer_findings']==cache['sanitizer_findings']==0
    for report in (main,cache):
        for name,h in report['source_sha256'].items():assert file_sha(ROOT/name)==h,('Source drift',name)
    sources,entries,compiler={},{},{}
    for abi in ('arm64-v8a','x86_64'):
        db=PROJECT/'app/.cxx/Debug/5a1n3w3m'/abi/'compile_commands.json'
        raw=db.read_bytes();rows=json.loads(raw)
        deps=subprocess.run([str(NINJA),'-C',str(db.parent),'-t','deps'],capture_output=True,text=True,check=True).stdout
        used={}
        for name in sorted(set([r['file'] for r in rows]+[s.strip() for s in deps.splitlines() if s.startswith('    ')])):
            p=Path(name).resolve()
            if not p.is_relative_to(ROOT):continue
            assert p.is_file(),p
            key=p.relative_to(ROOT).as_posix();data=p.read_bytes();h=sha(data)
            assert key not in sources or sources[key]==h,('Capture drift',key)
            if key in main['source_sha256']:assert main['source_sha256'][key]==h,('Main mismatch',key)
            sources[key]=used[key]=h;entries['source/'+key]=data
        for suffix in ('zip_asset_pack_v1.cpp','original_cache_assets_v1.cpp','script_runtime_return_v1.c','hud_text_format_v1.cpp','character_skill_info_session_v1.cpp'):
            assert any(k.endswith('/'+suffix) for k in used),('Missing actual compilation',abi,suffix)
        compiler[abi]=dict(database_sha256=sha(raw),ninja_dependencies_sha256=sha(deps.encode()),repository_inputs=used)
        entries['compiler/'+abi+'-commands.json']=raw;entries['compiler/'+abi+'-dependencies.txt']=deps.encode()
    for p in list((PROJECT/'app/src/main').rglob('*.java'))+[PROJECT/'app/build.gradle.kts',PROJECT/'app/src/main/cpp/CMakeLists.txt',Path(__file__),main_path,cache_path]:
        key=p.relative_to(ROOT).as_posix();raw=p.read_bytes();sources[key]=sha(raw);entries['source/'+key]=raw
    assets,libraries={},{}
    with zipfile.ZipFile(apk) as z:
        cache_info=z.getinfo('assets/dh2-original-cache.zip')
        assert cache_info.compress_type==zipfile.ZIP_STORED and cache_info.file_size==433189197
        with z.open(cache_info) as f:assert stream_sha(f)==cache['original_cache_sha256']
        for info in z.infolist():
            if info.filename.startswith('assets/') and not info.is_dir():
                with z.open(info) as f:assets[info.filename[7:]]=dict(bytes=info.file_size,sha256=stream_sha(f))
            if info.filename.startswith('lib/') and info.filename.endswith('.so'):
                raw=z.read(info);abi=info.filename.split('/')[1]
                assert abi in compiler and raw[:5]==b'\x7fELF\x02'
                assert struct.unpack_from('<H',raw,18)[0]==(183 if abi=='arm64-v8a' else 62)
                start=struct.unpack_from('<Q',raw,32)[0];size,count=struct.unpack_from('<HH',raw,54)
                aligns=[struct.unpack_from('<Q',raw,start+i*size+48)[0] for i in range(count) if struct.unpack_from('<I',raw,start+i*size)[0]==1]
                assert aligns and min(aligns)>=16384
                libraries[info.filename]=dict(sha256=sha(raw),bytes=len(raw),minimum_LOAD_alignment=min(aligns))
        assert len(libraries)==18 and len(assets)==771
        for path in (PROJECT/'app/src/main/assets').rglob('*'):
            if path.is_file():
                key=path.relative_to(PROJECT/'app/src/main/assets').as_posix()
                assert assets[key]==dict(bytes=path.stat().st_size,sha256=file_sha(path)),('Bundled asset changed',key)
    assert file_sha(apk)==digest
    checkpoint.parent.mkdir(exist_ok=True,parents=True);shutil.copyfile(apk,checkpoint)
    assert file_sha(checkpoint)==digest
    report=dict(validation='BUILD_INPUTS_CAPTURED',scope=__doc__,checkpoint=dict(path=str(checkpoint),sha256=digest,bytes=apk.stat().st_size),source_sha256=sources,compiler_inputs=compiler,assets_verified=assets,libraries=libraries,
        cache_files=6833,cache_uncompressed_bytes=648357710,original_cache_SHA256=cache['original_cache_sha256'],one_APK=True,external_cache_install=False,ARM32_engine_bundled=False,
        main_host_report=dict(path=main_path.relative_to(ROOT).as_posix(),sha256=file_sha(main_path),suites=129),cache_host_report=dict(path=cache_path.relative_to(ROOT).as_posix(),sha256=file_sha(cache_path)),physical_arm64_verified=False,full_game_verified=False,new_menus_live=False)
    entries['build-capture.json']=(json.dumps(report,indent=2)+'\n').encode()
    with zipfile.ZipFile(snapshot,'w',zipfile.ZIP_DEFLATED) as z:
        for name,raw in sorted(entries.items()):z.writestr(name,raw)
    with zipfile.ZipFile(snapshot) as z:assert z.testzip() is None
    report['source_snapshot']=dict(path=str(snapshot),sha256=file_sha(snapshot),bytes=snapshot.stat().st_size)
    assert sources=={name:file_sha(ROOT/name) for name in sources}
    output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(checkpoint=str(checkpoint),sha256=digest,bytes=apk.stat().st_size,cache_files=6833,assets=len(assets),libraries=len(libraries),source_inputs=len(sources),report=str(output))))
if __name__=='__main__':main()
