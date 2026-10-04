"""Read-only packaged ELF/export/asset checks for native game foundations."""
import argparse,hashlib,io,json,zipfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
from validate_prince_bank_checkpoint import native_library
ROOT=Path(__file__).resolve().parents[3]
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--capture',type=Path,required=True);p.add_argument('--host',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 if a.output.exists():raise RuntimeError('Refusing to replace foundation build inspection')
 host=json.loads(a.host.read_text());assert host['validation']=='PASS'
 bundle_path=ROOT/'port/script-runtime/reports/design-constants-bundled-assets.json';bundle=json.loads(bundle_path.read_text());assert bundle['validation']=='PASS' and bundle['constants_files']==26
 expected_assets={'assets/'+p.relative_to(ROOT/'port/android-native/app/src/main/assets').as_posix():sha(p.read_bytes()) for p in (ROOT/'port/android-native/app/src/main/assets').rglob('*') if p.is_file()}
 assert len(expected_assets)==258
 required=['port/script-runtime/script_constants.cpp','port/script-runtime/script_int_bindings.cpp','port/level-world/character_level.cpp','port/level-world/character_native_fsm.cpp']
 projects={};matches={}
 with zipfile.ZipFile(a.capture) as capture:
  manifest=json.loads(capture.read('build-capture.json'))
  for name,binding in manifest['entries'].items():
   raw=capture.read(name);assert len(raw)==binding['bytes'] and sha(raw)==binding['sha256']
  for name,digest in host['source_sha256'].items():
   key=name.replace('\\','/')
   if key in manifest['source_sha256']:assert manifest['source_sha256'][key]==digest;matches[key]=digest
  assert all(name in matches for name in required)
  for tag in ('packaged','studio'):
   for abi in ('arm64-v8a','x86_64'):assert all(name in manifest['compiler_inputs'][tag][abi]['repository_inputs'] for name in required)
   raw=capture.read(tag+'-app-debug.apk');assert sha(raw)==manifest['apks'][tag]['sha256']
   with zipfile.ZipFile(io.BytesIO(raw)) as apk:
    libraries={};exports={}
    for info in apk.infolist():
     if info.filename.startswith('lib/') and info.filename.endswith('.so'):
      libraries[info.filename]=native_library(apk,info,raw);elf=ELFFile(io.BytesIO(apk.read(info)))
      exports[info.filename]={s.name for s in elf.get_section_by_name('.dynsym').iter_symbols() if s['st_shndx']!='SHN_UNDEF'}
    assert len(libraries)==16 and not any('/armeabi' in n or 'DungeonHunter2' in n for n in libraries)
    for abi in ('arm64-v8a','x86_64'):
     runtime=exports['lib/'+abi+'/libdh2_script_runtime.so'];world=exports['lib/'+abi+'/libdh2_level_world.so']
     assert {'dh2_script_constants_create','dh2_script_constants_load','dh2_script_constants_get','dh2_script_constants_lookup','dh2_script_int_bind','dh2_script_value_get_string'}<=runtime
     assert {'dh2_character_set_level','dh2_character_get_level','dh2_character_native_fsm_update','dh2_character_native_fsm_bounded_tail','dh2_character_native_fsm_script_get_state','dh2_character_native_fsm_script_get_time'}<=world
    assets={n:sha(apk.read(n)) for n in apk.namelist() if n.startswith('assets/')};assert assets==expected_assets
    for record in bundle['bundled_assets']:assert assets['assets/data/'+Path(record['path']).name]==record['sha256']
    projects[tag]=dict(apk=manifest['apks'][tag],native_libraries=libraries,assets=len(assets),constants_files=26,recovered_exports_present=True)
 report=dict(validation='PASS',scope=__doc__,build_validation='PASS',live_validation='NOT_RUN',physical_arm64_verified=False,full_game_verified=False,live_enemy_AI=False,capture=dict(path=str(a.capture.resolve()),sha256=sha(a.capture.read_bytes())),host=dict(path=str(a.host.resolve()),sha256=sha(a.host.read_bytes())),source_inputs=len(manifest['source_sha256']),host_compiler_source_matches=matches,projects=projects,asset_sha256=expected_assets,constants_asset_proof_sha256=sha(bundle_path.read_bytes()))
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',live='NOT_RUN',assets=258,source_inputs=report['source_inputs'],projects={k:v['apk']['sha256'] for k,v in projects.items()})))
if __name__=='__main__':main()
