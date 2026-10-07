exec(open(__file__.replace('capture_property_parse_v1.py','capture_property_receivers_v1.py')).read().split('print(json.dumps(mapping')[0])
with original.open('rb')as f:
 elf=ELFFile(f);segments=list(elf.iter_segments());dyn=elf.get_section_by_name('.dynsym');rel=elf.get_section_by_name('.rel.plt');output=[]
 for a,n in((0x30f020,28),(0x30f03c,28),(0x30f288,128)):
  output.append('# '+hex(a)+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}'for i in md.disasm(raw(a,n),a)))
 (ref/'property-parse-source.asm').write_text('\n\n'.join(output)+'\n');print('\n\n'.join(output))
