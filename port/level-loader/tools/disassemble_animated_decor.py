from pathlib import Path
import subprocess
elf=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so');exe=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-objdump.exe')
out=[]
for start,size in [(0x342600,0x90),(0x389128,0x214),(0x389dd4,0x28),(0x388cec,0xe0),(0x389098,0x6c)]:
 r=subprocess.run([str(exe),'--disassemble','--demangle','--start-address='+hex(start),'--stop-address='+hex(start+size),str(elf)],check=True,capture_output=True)
 out.append(r.stdout.decode())
text='\n'.join(out);(Path(__file__).parent/'animated-decor-original.asm').write_text(text);print(text)
