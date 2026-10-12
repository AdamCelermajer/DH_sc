from pathlib import Path
import subprocess,json,hashlib
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=base/'reference/device-provider-v54';ref.mkdir(exist_ok=True);elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');sdk=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin')
nm=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,text=True,check=True).stdout.splitlines();selected=[]
for line in nm:
 row=line.split(maxsplit=3)
 if len(row)!=4:continue
 if any(key.lower() in row[3].lower() for key in ['getDriverType','SHARP_DEVICES','HTC_DEVICES','Is_In_Multiplayer_Mode','vtable for glitch::video::COpenGL','vtable for glitch::video::COGLES','vtable for glitch::video::CNullDriver','vtable for glitch::video::CSoftware']):
  selected.append({'address':hex(int(row[0],16)),'size':int(row[1],16),'kind':row[2],'name':row[3]})
for s in selected:
 if s['kind'] in ['T','W','t']:
  out=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+s['address'],'--stop-address='+hex(int(s['address'],16)+s['size']),str(elf)],capture_output=True,text=True,check=True).stdout;(ref/(s['address']+'.asm')).write_text(out)
(ref/'driver-symbols-v54.json').write_text(json.dumps(selected,indent=2)+'\n');(ref/'recover_device_provider_v54.py').write_bytes(Path(__file__).read_bytes());print(json.dumps(selected,indent=2))
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp')
for file,ranges in [('native_app.cpp',[(35,90),(360,385)]),('front_ui_session_v87.cpp',[(1245,1264)])]:
 lines=(root/file).read_text().splitlines();print(file)
 for start,end in ranges:print('\n'.join(f'{i+1}: {lines[i]}' for i in range(start-1,end)))
