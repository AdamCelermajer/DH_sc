from pathlib import Path
import subprocess,hashlib,sys
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');elf=root/'.local-inputs/libDungeonHunter2.so';sdk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin');kind=sys.argv[1]
assert hashlib.sha256(elf.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
out=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,check=True).stdout.decode()
lines=[line for line in out.splitlines() if kind in line];print('\n'.join(lines));(Path(__file__).parent/(kind+'-symbols.txt')).write_text('\n'.join(lines)+'\n')
parts=[]
for line in lines:
 columns=line.split(maxsplit=3)
 if columns[2] in ('T','W','t') and ('GetNewInstance<' in line or 'DeclareProperties' in line or ('::'+kind+'(') in line):
  start=int(columns[0],16);length=int(columns[1],16)
  data=subprocess.run([str(sdk/'llvm-objdump.exe'),'--disassemble','--demangle','--start-address='+hex(start),'--stop-address='+hex(start+length),str(elf)],capture_output=True,check=True).stdout.decode();parts.append(data)
text='\n'.join(parts);(Path(__file__).parent/(kind+'-constructor-properties.asm')).write_text(text);print(text)
