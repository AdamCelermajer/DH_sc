from pathlib import Path
import hashlib, json, re, shutil, subprocess

ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/'port/level-loader/reports/root-loading-v50'
LAYOUT=OUT/'compile-layout/port'
NDK=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865')
CLANG=NDK/'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'

def main():
    # Header snapshot resolves quoted relative includes coherently. Only the
    # migration manifest contributes changes to the adoption patch.
    for module in ['level-loader','level-world','game-data','script-runtime','engine-ui',
                   'engine-math','engine-objects','engine-animation','engine-skinning',
                   'engine-effects','engine-resources','engine-textures','scene-materials','asset-payloads']:
        for p in (ROOT/'port'/module).glob('*'):
            if p.suffix not in ('.hpp','.h','.inc','.cpp'):continue
            dest=LAYOUT/module/p.name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,dest)
    for p in (OUT/'stage/port').rglob('*'):
        if p.is_file():
            dest=LAYOUT/p.relative_to(OUT/'stage/port');dest.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,dest)
    units=['level_source_loading_v43.cpp','lifecycle_v36.cpp','native_gslevel_frame_v45.cpp',
           'retained_level_module_graph_v1.cpp','native_gslevel_runtime_v27.cpp',
           'native_level_application_v25.cpp']
    probe=OUT/'facade-compile.cpp'
    probe.write_text('#include "native_root_loading_connection_v50.hpp"\n'
                     '#include "native_driver_unused_v50.hpp"\n'
                     'static_assert(sizeof(dh2::loader::LevelConstructorFieldsV3::field134)==4);\n'
                     'static_assert(sizeof(dh2::loader::LevelConstructorFieldsV3::field138)==4);\n'
                     'static_assert(sizeof(dh2::loader::LevelConstructorFieldsV3::field13c)==4);\n')
    inputs=[LAYOUT/'level-loader'/n for n in units]+[probe]
    inputs.append(LAYOUT/'level-world/canonical_object_manager_v1.cpp')
    dependency=(ROOT/'port/level-loader/native_loading_sources_v50.cmake').read_text()
    for group,module in [('loader','level-loader'),('world','level-world')]:
        part=re.search(r'set\(_dh2_v50_'+group+r'_sources\s+(.*?)\)',dependency,re.S).group(1)
        for name in re.findall(r'\w+\.cpp',part):
            path=LAYOUT/module/name
            if path not in inputs:inputs.append(path)
    results=[]
    box=ROOT/'port/physics-backend/box2d-2.0.1/Include'
    for abi,target in [('x86_64','x86_64-linux-android24'),('arm64-v8a','aarch64-linux-android24')]:
        for source in inputs:
            command=[str(CLANG),'--target='+target,'-std=c++17','-O1','-Wall','-Wextra','-Werror',
                     '-Wno-misleading-indentation','-fno-fast-math','-ffp-contract=off','-fPIC']
            command+=['-I'+str(p) for p in LAYOUT.iterdir() if p.is_dir()]
            command+=['-isystem',str(box),'-c',str(source),'-o',str(OUT/(source.stem+'-'+abi+'.o'))]
            run=subprocess.run(command,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=60)
            (OUT/(source.stem+'-'+abi+'.log')).write_text(run.stdout)
            results.append({'abi':abi,'source':str(source),'sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
                            'exit_code':run.returncode,'command':command,'output':run.stdout})
            print(abi,source.name,run.returncode,run.stdout[:2500],flush=True)
            if run.returncode:
                (OUT/'compile-proof.json').write_text(json.dumps(results,indent=2));return 1
    (OUT/'compile-proof.json').write_text(json.dumps(results,indent=2))
    return 0

if __name__=='__main__':raise SystemExit(main())
