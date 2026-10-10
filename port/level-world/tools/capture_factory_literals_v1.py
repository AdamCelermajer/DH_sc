exec(open(__file__.replace('capture_factory_literals_v1.py','capture_canonical_object_factory_v1.py')).read().split("(ref/'original-source.asm')")[0])
with original.open('rb') as f:
 elf=ELFFile(f);segments=list(elf.iter_segments())
 pairs=[(0x34acd4,0x34b040),(0x34ad08,0x34b044),(0x34ad24,0x34b048),(0x34ad54,0x34b04c),(0x34ad64,0x34b050),(0x34afa4,0x34b060),(0x34aec0,0x34b05c),(0x34b4bc,0x34b51c),(0x34b8a4,0x34bbb4),(0x34b8b8,0x34bbb8),(0x34b900,0x34bbbc),(0x34b92c,0x34bbc0),(0x34b9b0,0x34bbc4),(0x34ba4c,0x34bbc8),(0x34bb14,0x34bbcc),(0x5246c0,0x52474c)]
 result=[]
 for pc,literal in pairs:
  addr=(pc+8+word(literal))&0xffffffff
  result.append(dict(add_instruction=hex(pc),literal=hex(literal),address=hex(addr),value=string(addr)))
 (ref/'factory-literals.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps(result,indent=2))
