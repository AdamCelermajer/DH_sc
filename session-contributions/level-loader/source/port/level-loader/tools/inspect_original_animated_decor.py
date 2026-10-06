from pathlib import Path
import subprocess,hashlib
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');elf=root/'.local-inputs/libDungeonHunter2.so';sdk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin')
assert hashlib.sha256(elf.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
r=subprocess.run([str(sdk/'llvm-nm.exe'),'-S','--defined-only','--demangle',str(elf)],capture_output=True,check=True)
lines=[line for line in r.stdout.decode().splitlines() if 'AnimatedDecor' in line]
print('\n'.join(lines))
out=Path(__file__).parent/'animated-decor-original-symbols.txt';out.write_text('\n'.join(lines)+'\n')
