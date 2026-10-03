"""Identify original default/type rows through the original getter GOT slots."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tools'))
from prepare_actors import strings

def main():
 p=argparse.ArgumentParser();p.add_argument('engine',type=Path);p.add_argument('characters',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 with a.engine.open('rb') as f:
  elf=ELFFile(f)
  def word(address):
   s=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address and address+4<=s['p_vaddr']+s['p_filesz']);f.seek(s['p_offset']+address-s['p_vaddr']);return struct.unpack('<I',f.read(4))[0]
  default_got=(0x3def1c+8+word(0x3def2c))&0xffffffff;type_got=(0x3deee8+8+word(0x3def08))&0xffffffff
  default_global=word(default_got+word(0x3def30));type_global=word(type_got+word(0x3def0c))
  assert default_global==type_global
  symbols=[s.name for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_value']==default_global]
 names,_=strings((a.characters/'character_properties_pyarraynames.bin').read_bytes());fields,_=strings((a.characters/'character_properties_pystructnames.bin').read_bytes());raw=(a.characters/'character_properties_pyarray.bin').read_bytes()
 defaults=struct.unpack_from('<224i',raw,4);types=struct.unpack_from('<224i',raw,4+896)
 rows=[{'id':i,'name':name,'default_raw':defaults[i],'type_raw':types[i],'effective_type':16 if types[i]==-1 else types[i]} for i,name in enumerate(fields)]
 result={'original_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'character_array_global':hex(default_global),'global_symbols':symbols,'default_row':names[0],'type_row':names[1],'default_row_index':0,'type_row_index':1,'rows':rows,'input_sha256':{n:hashlib.sha256((a.characters/n).read_bytes()).hexdigest() for n in ('character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin')}}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='rows'}));print(json.dumps({t:sum(r['effective_type']==t for r in rows) for t in sorted(set(r['effective_type'] for r in rows))}));print(json.dumps(rows[33:47]))
if __name__=='__main__':main()
