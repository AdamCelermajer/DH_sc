"""Inspect actual native libraries for complete-cache script/query selection.

This checks compiler selection/ELF exports. It does not create an APK or claim
that the new player startup or menu screens execute on Android.
"""
import hashlib,io,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3]
APP=ROOT/'port/android-native/app'
OUTPUT=ROOT/'port/android-native/reports/native-script-resources-spell-library-build-v1.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    assert not OUTPUT.exists(),'Preserve prior inspections'
    inputs=[ROOT/p for p in (
      'port/level-world/character_script_assets_v1.hpp','port/level-world/character_script_assets_v1.cpp',
      'port/level-world/character_current_spell_v1.hpp','port/level-world/character_current_spell_v1.cpp',
      'port/level-world/CMakeLists.txt','port/android-native/app/src/main/cpp/CMakeLists.txt',
      'port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp',
      'port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp',
      'port/android-native/app/src/main/cpp/model_renderer.cpp',
      'port/android-native/tools/check_script_resources_spell_v1_android.py',
      'port/level-world/reference/character-current-spell-v1/freeze-manifest.json')]
    source_hashes={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
    abis={}
    for abi,machine in [('arm64-v8a','EM_AARCH64'),('x86_64','EM_X86_64')]:
        build=APP/'.cxx/Debug/5a1n3w3m'/abi
        compiler_path=build/'compile_commands.json';compiler=json.loads(compiler_path.read_text())
        records=[r for r in compiler if r['file'].replace('\\','/').endswith(('/character_script_assets_v1.cpp','/character_current_spell_v1.cpp','/original_cache_assets_v1.cpp','/model_renderer.cpp'))]
        assert len(records)==4
        assert all('-fno-fast-math' in r['command'] and '-ffp-contract=off' in r['command'] for r in records)
        libs={}
        for name in ('dh2_level_world','dh2_native'):
            path=APP/'build/intermediates/cxx/Debug/5a1n3w3m/obj'/abi/('lib'+name+'.so')
            elf=ELFFile(io.BytesIO(path.read_bytes()))
            assert elf.elfclass==64 and elf['e_machine']==machine
            align=[s['p_align'] for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
            assert align and min(align)>=16384
            symbols=[s.name for s in elf.get_section_by_name('.dynsym').iter_symbols() if s['st_shndx']!='SHN_UNDEF']
            selected=[s for s in symbols if 'CharacterScriptAssetsV1' in s or 'current_spell' in s or ('OriginalCacheAssetsV1' in s and 'directory' in s)]
            if name=='dh2_level_world':
                assert 'dh2_character_current_spell_level_v1' in selected
                assert any('current_spell_info_v1' in s for s in selected)
                assert any('CharacterScriptAssetsV1' in s and 'load' in s for s in selected)
            else:assert any('OriginalCacheAssetsV1' in s and 'directory' in s for s in selected)
            libs[name]=dict(path=str(path),sha256=sha(path),elf_class=64,machine=machine,load_alignments=align,selected_defined_symbols=selected)
        abis[abi]=dict(libraries=libs,compiler_records=records,compiler_database_sha256=sha(compiler_path))
    assert source_hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
    OUTPUT.write_text(json.dumps(dict(validation='PASS',source_sha256=source_hashes,abis=abis,
      APK_packaged=False,live_Android_player_connected=False,new_menus_connected=False),indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',ABIs=list(abis),selected_native_sources=4,APK_packaged=False)))
if __name__=='__main__':main()
