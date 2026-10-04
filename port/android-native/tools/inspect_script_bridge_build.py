"""Inspect captured Android builds for the recovered native script bridge.

Read-only build/ELF/asset evidence. No installation, gameplay, packaged
instruction execution or physical-device verification is performed.
"""
import argparse,hashlib,io,json,zipfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
from validate_prince_bank_checkpoint import native_library

def sha(raw):return hashlib.sha256(raw).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--capture',type=Path,required=True)
    p.add_argument('--host',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    if a.output.exists():raise RuntimeError('Refusing to replace Android build inspection')
    results={};asset_sets={}
    with zipfile.ZipFile(a.capture) as captured:
        manifest=json.loads(captured.read('build-capture.json'))
        for name,binding in manifest['entries'].items():
            raw=captured.read(name)
            assert sha(raw)==binding['sha256'] and len(raw)==binding['bytes']
        host=json.loads(a.host.read_text());assert host['validation']=='PASS'
        matches={}
        for name,expected in host['source_sha256'].items():
            key=name.replace('\\','/')
            if key in manifest['source_sha256']:
                assert manifest['source_sha256'][key]==expected
                matches[key]=expected
        assert 'port/script-runtime/script_runtime.c' in matches
        required=['port/script-runtime/script_design_bindings.c',
                  'port/script-runtime/script_scalar_bindings.c',
                  'port/level-world/character_property_bindings.cpp',
                  'port/level-world/character_script_states.cpp',
                  'port/level-world/character_relationships.cpp']
        for tag in ['packaged','studio']:
            for abi in ['arm64-v8a','x86_64']:
                used=manifest['compiler_inputs'][tag][abi]['repository_inputs']
                assert all(key in used for key in required)
            raw=captured.read(tag+'-app-debug.apk');assert sha(raw)==manifest['apks'][tag]['sha256']
            with zipfile.ZipFile(io.BytesIO(raw)) as apk:
                native={};exports={}
                for info in apk.infolist():
                    if not info.filename.startswith('lib/') or not info.filename.endswith('.so'):continue
                    native[info.filename]=native_library(apk,info,raw)
                    elf=ELFFile(io.BytesIO(apk.read(info)))
                    exports[info.filename]={s.name for s in elf.get_section_by_name('.dynsym').iter_symbols()
                                            if s['st_shndx']!='SHN_UNDEF'}
                assert len(native)==16
                for abi in ['arm64-v8a','x86_64']:
                    names=[name for name in native if name.startswith('lib/'+abi+'/')]
                    assert len(names)==8
                    runtime=exports['lib/'+abi+'/libdh2_script_runtime.so']
                    world=exports['lib/'+abi+'/libdh2_level_world.so']
                    assert {'dh2_script_scalar_bind','dh2_script_design_bind',
                            'dh2_script_vm_bind_source_scoped','dh2_script_callback_call_discard_source',
                            'dh2_script_vm_bind_source_include','dh2_script_vm_load_source_file'}<=runtime
                    assert {'dh2_character_property_bind','dh2_character_get_prop',
                            'dh2_character_script_states_register','dh2_character_script_states_update',
                            'dh2_character_relationship'}<=world
                asset_sets[tag]={name:sha(apk.read(name)) for name in apk.namelist() if name.startswith('assets/')}
                assert len(asset_sets[tag])==233
                results[tag]=dict(apk=manifest['apks'][tag],native_libraries=native,
                                  assets=len(asset_sets[tag]),recovered_exports_present=True)
    assert asset_sets['packaged']==asset_sets['studio']
    report=dict(validation='PASS',scope=__doc__,build_validation='PASS',
        live_validation='NOT_RUN',physical_arm64_verified=False,full_game_verified=False,
        capture=dict(path=str(a.capture.resolve()),sha256=sha(a.capture.read_bytes())),
        host=dict(path=str(a.host.resolve()),sha256=sha(a.host.read_bytes())),
        source_inputs=len(manifest['source_sha256']),host_compiler_source_matches=matches,
        projects=results,asset_sha256=asset_sets['packaged'])
    a.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(validation='PASS',live='NOT_RUN',source_inputs=report['source_inputs'],
                         native_libraries=16,assets=233,projects={k:v['apk']['sha256'] for k,v in results.items()})))

if __name__=='__main__':main()
