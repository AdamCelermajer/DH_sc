exec(open('.local-inputs/capture_level_across_v4.py').read().split('dest=Path')[0])
with Path('.local-inputs/libDungeonHunter2.so').open('rb') as f:
 e=ELFFile(f);segs=list(e.iter_segments())
 def raw(a,n):
  p=next(p for p in segs if p['p_vaddr']<=a and a+n<=p['p_vaddr']+p['p_filesz']);f.seek(p['p_offset']+a-p['p_vaddr']);return f.read(n)
 for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw(0x30e6f4,12),0x30e6f4):print(hex(i.address),i.mnemonic,i.op_str)
 rel=e.get_section_by_name('.rel.plt');sym=e.get_section(rel['sh_link'])
 for r in rel.iter_relocations():
  name=sym.get_symbol(r['r_info_sym']).name
  if name in ('malloc','calloc','memset'):print(hex(r['r_offset']),name)
 print('\n'.join(x for x in Path('port/level-world/reference/level-config-across-rooms-v4/original.asm').read_text().splitlines() if '#0x87]' in x or '#0x64]' in x))
