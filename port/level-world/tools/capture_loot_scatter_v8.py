from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3];path=root/'.local-inputs/libDungeonHunter2.so'
with path.open('rb') as f:
 elf=ELFFile(f);address=0x3ec5d4;size=0x3ec668-address
 segment=next(s for s in elf.iter_segments()if s['p_vaddr']<=address< s['p_vaddr']+s['p_filesz'])
 f.seek(segment['p_offset']+address-segment['p_vaddr']);code=f.read(size)
 text='\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(code,address))
 def word(a):
  s=next(s for s in elf.iter_segments()if s['p_vaddr']<=a<s['p_vaddr']+s['p_filesz'])
  f.seek(s['p_offset']+a-s['p_vaddr']);return struct.unpack('<I',f.read(4))[0]
 got=0x3ec5e0+8+word(0x3ec65c)
 symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
 globals=[]
 for name,literal in [('seed',0x3ec660),('calls',0x3ec664)]:
  slot=got+word(literal);target=word(slot)
  globals.append(dict(field=name,got_slot=hex(slot),target=hex(target),symbols=[s.name for s in symbols if s['st_value']==target]))
 loot_got=0x401b10+word(0x401b84)
 loot_globals=[word(loot_got+word(a))for a in [0x401b88,0x401b8c]]
 scatter_got=0x3ec67c+8+word(0x3ec898);basis=word(scatter_got+word(0x3ec89c))
 basis_info=dict(address=hex(basis),symbols=[s.name for s in symbols if s['st_value']==basis])
ref=root/'port/level-world/reference/character-loot-world-v8'
(ref/'scatter-random-source.asm').write_text(text+'\n')
(ref/'scatter-random-globals.json').write_text(json.dumps(dict(original_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),globals=globals,loot_random_global_addresses=[hex(a)for a in loot_globals],same_seed_and_counter=[int(g['target'],16)for g in globals]==loot_globals,basis=basis_info),indent=2)+'\n')
print(text);print(json.dumps(globals));print(json.dumps(dict(loot_globals=[hex(a)for a in loot_globals],same=[int(g['target'],16)for g in globals]==loot_globals)))
print('\n'.join((ref/'original-source.asm').read_text().splitlines()[239:389]))
print(json.dumps(basis_info))
