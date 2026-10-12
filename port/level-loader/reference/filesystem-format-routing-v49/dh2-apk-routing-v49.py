from pathlib import Path
import zipfile,hashlib,json,re,subprocess
out=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/filesystem-format-routing-v49')
apk=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2.apk')
rows=[]
with zipfile.ZipFile(apk) as z:
 for info in z.infolist():
  if info.filename.endswith(('.so','.dex','.xml','.cfg','.config')):
   raw=z.read(info)
   hits=[{'offset':m.start(),'text':m.group().decode()} for m in re.finditer(rb'[\x20-\x7e]{4,}',raw) if any(k in m.group().lower() for k in [b'pvr2',b'pvr_',b'textureformat',b'textureprefix'])]
   if info.filename.endswith('.so') or hits:rows.append({'entry':info.filename,'sha256':hashlib.sha256(raw).hexdigest(),'size':len(raw),'hits':hits})
   if info.filename.endswith('libDungeonHunter2.so'):
    (out/'supplied-original-libDungeonHunter2.so').write_bytes(raw)
(out/'original-apk-routing-census.json').write_text(json.dumps({'apk_sha256':hashlib.sha256(apk.read_bytes()).hexdigest(),'entries':rows},indent=2)+'\n')
print(json.dumps(rows,indent=2)[:18000])
elf=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');exe=Path('C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe')
for name,a,b in [('obfuscated-file',0x56e064,0x56e6dc),('archive-open',0x56c0f8,0x56c214),('add-obfuscation',0x56e6dc,0x56eab0)]:
 (out/(name+'.asm')).write_text(subprocess.check_output([str(exe),'-d','--demangle',f'--start-address={a}',f'--stop-address={b}',str(elf)],text=True))
print((out/'obfuscated-file.asm').read_text()[:22000])
