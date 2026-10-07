"""Original ItemPowerList/Table/Ref readers on all genuine cache bytes.
Storage allocation and stream virtual reads are explicit byte services; every
decoded scalar and ordered property including full signed32 Flags is compared.
"""
import sys,json,struct,hashlib,argparse
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'));sys.path.insert(0,str(R/'port/engine-resources/tests'))
from items_differential import Original
from cpu import Cpu
W=lambda *x:struct.pack('<'+'I'*len(x),*[v&0xffffffff for v in x])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);a=p.parse_args()
 manifests=[json.loads((R/f'.local-inputs/player-item-effects-v5/{s}/original-functions.json').read_text()) for s in ('power-reader','power-prefix','power-value-reader')]
 old=Original(R/'.local-inputs/libDungeonHunter2.so',{'functions':sum((x['functions'] for x in manifests),[])});new=Cpu(a.library,True,{'functions':[]})
 old.blob=(R/'.local-inputs/player-item-effects-v5/power-cache/item_powers_pyarray.bin').read_bytes();old.invoke(0x4bacc8,[old.stream],budget=30000000);begin=old.cursor;old.invoke(0x4bab7c,[old.stream],budget=30000000);assert old.cursor==len(old.blob)
 got=(0x4baba8+old.word(0x4bacb8))&0xffffffff;count=old.word(old.word(got+old.word(0x4bacbc)));table=old.word(old.word(got+old.word(0x4bacc4)))
 at=begin+4;cases=[];entry_count=0;input=new.data+0x4000;out=new.data+0x2000
 for i in range(count):
  w=list(struct.unpack('<10I',old.uc.mem_read(table+40*i,40)));scalars=W(w[1]&255,w[2],*w[5:10]);expected=b''.join(bytes(old.uc.mem_read(w[4]+j*16+4,12)) for j in range(w[3]))
  source=old.blob[at:];new.uc.mem_write(input,source);assert new.invoke('dh2_item_power_decode_v5',[out,input,len(source)])==0
  fields=bytes(new.uc.mem_read(out,48));n,ptr,used,res=struct.unpack_from('<IQII',fields,28);assert fields[:28]==scalars and n==w[3] and not res,(i,struct.unpack("<7i",fields[:28]),struct.unpack("<7i",scalars),n,w[3],res,at)
  projected=b''.join(bytes(new.uc.mem_read(ptr+12*j,12)) for j in range(n));assert projected==expected,(i,projected,expected)
  assert used==29+12*n;raw=old.blob[at:at+used];cases.append(W(used)+raw+scalars+W(n)+expected);at+=used;entry_count+=n
 assert at==old.cursor
 gold=b'IPV5'+W(count)+b''.join(cases);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(gold)
 files=['port/game-data/item_power_tables_v5.hpp','port/game-data/item_power_tables_v5.cpp','port/game-data/tests/item_power_tables_v5_original.py']
 report={'validation':'PASS','comparisons':count,'original_properties_compared':entry_count,'mismatches':0,'original_sha256':sha(R/'.local-inputs/libDungeonHunter2.so'),'library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'source_sha256':{x:sha(R/x) for x in files},'input_sha256':{str(x.relative_to(R)).replace('\\','/'):sha(x) for x in (R/'.local-inputs/player-item-effects-v5/power-cache').glob('item_powers_*.bin')},'original_byte_reader_services':old.reads,'original_allocations':old.allocations,'source_scope':__doc__,'original_reader_begin':begin,'original_reader_end':at,'AddPower_instance_and_presentation_parity':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()


