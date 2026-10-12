from pathlib import Path
import subprocess,json,hashlib,re,shutil
main=Path('C:/Users/adamc/Desktop/workspace/DH_sc');r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/production-filesystem-adoption-v50')
paths=['port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp','port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp','port/android-native/app/src/main/cpp/model_renderer.hpp','port/android-native/app/src/main/cpp/model_renderer.cpp','port/android-native/app/src/main/cpp/renderer_native_level_c1_v25.inc','port/android-native/app/src/main/cpp/native_app.cpp','port/asset-payloads/zip_asset_pack_v1.hpp','port/asset-payloads/zip_asset_pack_v1.cpp']
hashes={};hits=[]
for name in paths:
 p=main/name;dst=r/'main-source-snapshot'/name;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,dst);hashes[name]=hashlib.sha256(p.read_bytes()).hexdigest()
 for i,line in enumerate(p.read_text().splitlines(),1):
  if any(k in line for k in ['files_directory','OriginalCacheAssetsV1','state_->','mounted()','manager(','mount(','read_admitted','static bool key','FileSystem','getWorkingDirectory']):hits.append({'file':name,'line':i,'text':line.strip()})
exe=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-nm.exe');elf=main/'.local-inputs/libDungeonHunter2.so'
nm=subprocess.check_output([str(exe),'--demangle','--print-size','--defined-only',str(elf)],text=True)
names=[x for x in nm.splitlines() if any(k in x for k in ['RES_PATH','WorkingDirectory','ROOT_PATH','SAVE_PATH']) or ('Java_' in x and any(k.lower() in x.lower() for k in ['path','dir','init','native'] ))]
(r/'current-main-source-audit.json').write_text(json.dumps({'hashes':hashes,'hits':hits,'working_directory_producer_present_in_audited_files':False,'original_symbols':names},indent=2)+'\n')
print('\n'.join(names));print('snapshot_files',len(hashes),'hits',len(hits))
