"""Read original static property IDs/offsets and the lazy name resource path."""
import argparse,hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();result={}
 with a.engine.open('rb') as f:
  elf=ELFFile(f)
  def raw(address,size):
   s=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz']);f.seek(s['p_offset']+address-s['p_vaddr']);return f.read(size)
  def word(address):return struct.unpack('<I',raw(address,4))[0]
  got=(0x3dedcc+word(0x3deeb0))&0xffffffff;slot=got+word(0x3deeb8);address=word(slot)
  print('offset address',hex(address),'slot',hex(slot))
  offsets=list(struct.unpack('<224I',raw(address,896)));assert sorted(offsets)==list(range(0,896,4))
  result={'original_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'property_offsets_address':hex(address),'property_offsets':offsets,'name_resource_address':hex((0x3e1724+word(0x3e1754))&0xffffffff)}
  result['name_resource_path']=raw(int(result['name_resource_address'],16),120).split(b'\0')[0].decode('ascii')
  a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='property_offsets'}))
if __name__=='__main__':main()
