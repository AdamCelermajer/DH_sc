from pathlib import Path
import sys,struct,json
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
root=Path(__file__).resolve().parents[3]
elf=ELFFile((root/'.local-inputs/libDungeonHunter2.so').open('rb'))
symbols=list(elf.get_section_by_name('.dynsym').iter_symbols())
if len(sys.argv)>1:
 address=int(sys.argv[1],16);length=int(sys.argv[2],16)
 for seg in elf.iter_segments():
  if seg['p_type']=='PT_LOAD' and seg['p_vaddr']<=address<seg['p_vaddr']+seg['p_filesz']:
   for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(seg.data()[address-seg['p_vaddr']:address-seg['p_vaddr']+length],address):print(hex(i.address),i.mnemonic,i.op_str)
else:
 for s in symbols:
  if any(k in s.name for k in ('CMorphingMesh','SGeometry','CColladaFactory10createMesh')) and s['st_value']: print(hex(s['st_value']),s['st_size'],s.name)
 census=json.loads((root/'port/level-world/reports/authored-resource-domains-v6.json').read_text())
 for row in census['resources']:
  if row['status']=='PASS':continue
  raw=(root/'port/level-world/reference/shared-target-facing-v1/cache/general-v5'/row['local']).read_bytes()
  w=lambda p:struct.unpack_from('<I',raw,p)[0]
  r=w(32);print(row['local'],row['uri'],row['required'])
  for i in range(w(r+120)):
   p=w(r+124)+144*i;print('emitter',i,'direction',w(p+72),'spin',w(p+136),'max',w(p+24),'spinvariation',w(p+140),'descriptor', [w(w(p+84)+4*k)for k in range(9)])
  for i in range(w(r+104)):
   p=w(r+108)+16*i;payload=w(p+12);print('geometry',i,'kind',w(p+8),'payload',hex(payload),[w(payload+4*k)for k in range(min(20,(len(raw)-payload)//4))])

