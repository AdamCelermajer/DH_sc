from pathlib import Path
import sys,hashlib,json
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/character-loot-world-v8';ref.mkdir(parents=True,exist_ok=True)
addresses=[0x3a2fcc,0x3a5ae4,0x3ecba0,0x3ecae4,0x3ec8a0,0x3ec474,0x3ec668,0x3eac00,0x3eb6e4,0x3eacd0,0x3eac34,0x3eb6bc,0x3ec324,0x3ece80,0x3ec0f0,0x3ed144,0x3ec048,0x3ec014,0x3ebffc,0x3f9e34,0x3fc26c,0x3ffa68,0x3ff858,0x36ea50,0x36eab8,0x36eac0,0x36eac8,0x36e478,0x36e2cc,0x36dfb0,0x36e744]
md=Cs(CS_ARCH_ARM,CS_MODE_ARM);captured=[];text=[]
with original.open('rb') as f:
 elf=ELFFile(f);symbols=list(elf.get_section_by_name('.symtab').iter_symbols());segments=list(elf.iter_segments())
 for address in addresses:
  candidates=[s for s in symbols if s['st_value']==address and s['st_size']];symbol=max(candidates,key=lambda s:s['st_size']);size=symbol['st_size']
  segment=next(s for s in segments if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr']);code=f.read(size)
  captured.append(dict(address=hex(address),symbol=symbol.name,size=size,sha256=hashlib.sha256(code).hexdigest()));text.append('# '+hex(address)+' '+symbol.name+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(code,address)))
(ref/'original-source.asm').write_text('\n\n'.join(text)+'\n');(ref/'original-functions.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),functions=captured),indent=2)+'\n');print(json.dumps(dict(functions=len(captured),original_sha256=hashlib.sha256(original.read_bytes()).hexdigest())))
