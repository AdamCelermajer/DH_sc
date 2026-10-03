"""Preserve actual Android target outputs; this does not build or package an APK."""
import argparse
import hashlib
import io
import json
from pathlib import Path
from elftools.elf.elffile import ELFFile

ROOT=Path(__file__).resolve().parents[1]
REPO=ROOT.parents[1]


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Refusing to overwrite library capture')
    native=REPO/'port/android-native/app'
    names=('engine_animation','engine_skinning','scene_materials','game_data','level_world')
    copies=[]
    libraries={}
    for abi,machine in (('arm64-v8a','EM_AARCH64'),('x86_64','EM_X86_64')):
        for name in names:
            file=native/'build/intermediates/cxx/Debug/5a1n3w3m/obj'/abi/('libdh2_'+name+'.so')
            raw=file.read_bytes()
            elf=ELFFile(io.BytesIO(raw))
            assert elf.elfclass==64 and elf['e_machine']==machine
            loads=[dict(offset=s['p_offset'],vaddr=s['p_vaddr'],align=s['p_align'])
                   for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
            assert loads and all(s['align']>=16384 and s['offset']%16384==s['vaddr']%16384 for s in loads)
            symbols=elf.get_section_by_name('.dynsym')
            exports=[s.name for s in symbols.iter_symbols() if s['st_shndx']!='SHN_UNDEF' and
                     any(key in s.name for key in ('dh2_animation_angle','dh2_animation_component','dh2_character_script','dh2_item_'))]
            libraries[abi+'/'+file.name]=dict(bytes=len(raw),sha256=sha(raw),machine=machine,loads=loads,new_exports=exports)
            copies.append((Path(abi)/file.name,raw))
        commands=native/'.cxx/Debug/5a1n3w3m'/abi/'compile_commands.json'
        copies.append((Path(abi)/'compile_commands.json',commands.read_bytes()))
    sources={}
    modules=('engine-animation','engine-skinning','scene-materials','engine-resources','asset-payloads',
             'engine-math','game-data','level-world','physics-backend')
    for module in modules:
        for path in (REPO/'port'/module).rglob('*'):
            if (path.is_file() and 'reference' not in path.parts and 'tests' not in path.parts and
                    (path.suffix in ('.hpp','.cpp','.h','.cc') or path.name=='CMakeLists.txt')):
                sources[str(path.relative_to(REPO)).replace('\\','/')]=sha(path.read_bytes())
    apk=REPO/'port/android-native/build/checkpoints/dh2-native-character-timing-6d9782be.apk'
    apk_hash=sha(apk.read_bytes())
    assert apk_hash=='6d9782be431a8a70dabacf5932da4f40ccff2f59ec18a00d6f1f9194b6839cef'
    report=dict(validation='BUILD_OUTPUTS_CAPTURED',libraries=libraries,source_tree_sha256=sources,
                preserved_apk_sha256=apk_hash,capture_script_sha256=sha(Path(__file__).read_bytes()),
                scope='Actual successful CMake dh2_level_world Android build outputs, ELF64/16KiB checks and current production source-tree snapshot. '
                      'Compiler commands preserved separately; snapshot is not a pre/post compiler-input provenance claim. '
                      'No rebuilt APK, live renderer, emulator or physical-device validation.')
    for relative,raw in copies:
        destination=args.output/relative
        destination.parent.mkdir(parents=True,exist_ok=True)
        destination.write_bytes(raw)
    (args.output/'libraries.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(validation=report['validation'],libraries=len(libraries),source_files=len(sources),apk_sha256=apk_hash)))


if __name__=='__main__':
    main()
