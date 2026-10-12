from pathlib import Path
import subprocess
elf=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so');sdk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin')
out=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,check=True).stdout.decode()
lines=[s for s in out.splitlines() if ' Zone::' in s or s.endswith('vtable for Zone')];print('\n'.join(lines));parts=[]
for s in lines:
 if any(v in s for v in ['Zone::Zone(','Zone::DeclareProperties']):
  fields=s.split(maxsplit=3);start=int(fields[0],16);size=int(fields[1],16)
  parts.append(subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+hex(start),'--stop-address='+hex(start+size),str(elf)],capture_output=True,check=True).stdout.decode())
text='\n'.join(parts);(Path(__file__).parent/'zone-checkpoint-base-original.asm').write_text(text);print(text)
