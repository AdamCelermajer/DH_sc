"""Real original gear arithmetic and vitals cap versus optimized native ARM64.
Original _IsPropertySet/GetDefault/Add/Set/Resolve execute. Item records come
from the real cache via its original loader. Power-entry vectors are explicit
caller projections, not a claimed ItemPower table decoder or powered owner.
"""
import hashlib,json,struct,sys,random,argparse
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'))
from item_inventory_v1_original import Inventory
sys.path.insert(0,str(R/'port/engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE
W=lambda *x:struct.pack('<'+'I'*len(x),*[v&0xffffffff for v in x])
P=lambda x:struct.pack('<224i',*x)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);a=p.parse_args()
 # The loader executes original table readers. No item-effect fixture runs.
 loader=Inventory();records=[]
 for i in range(len(loader.itemnames)):
  w=list(struct.unpack('<41i',loader.uc.mem_read(loader.table+i*164,164)));w[0]=w[2]=w[20]=0;records.append(struct.pack('<41i',*w))
 manifests=[json.loads((R/f'.local-inputs/player-item-effects-v5/{n}/original-functions.json').read_text()) for n in ('gear-producers','property-add','effect-dispatch','character-effects')]
 provenance={'functions':sum((x['functions'] for x in manifests),[])}
 old=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,provenance);new=Cpu(a.library,True,{'functions':[]})
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 owner=old.data+0x1000;character=owner-0x560;table=old.data+0x100000;power=old.data+0x160000;entries=old.data+0x180000
 got=(0x3e3168+word(0x3e32ac))&0xffffffff;old.pointer(word(got+word(0x3e32b0)),table)
 for i,b in enumerate(records):old.uc.mem_write(table+i*164,b)
 got=(0x3e32c8+word(0x3e3d10))&0xffffffff;old.pointer(word(got+word(0x3e3d14)),power)
 raw=(R/'.local-inputs/actors/character_properties_pyarray.bin').read_bytes();d=list(struct.unpack_from('<224i',raw,4));t=list(struct.unpack_from('<224i',raw,900))
 old.pointer(0x9a645c,old.data+0x4000);old.uc.mem_write(old.data+0x4000,bytes(4)+P(d)+bytes(4)+P(t))
 no=[new.data+x for x in (0x8000,0x9000,0xa000,0xb000)];oo=[owner+x for x in (8,0x38c,0x710,0xa94)]
 nd,nt,nitem,nentry,nview=new.data+0x4000,new.data+0x5000,new.data+0x6000,new.data+0x100000,new.data+0x7000
 new.uc.mem_write(nd,P(d));new.uc.mem_write(nt,P(t));new.uc.mem_write(nview,struct.pack('<7QII',nd,nt,*no,0,0,0))
 sentinel=owner+0xe18;old.uc.mem_write(sentinel,W(0,0,sentinel,sentinel));old.pointer(owner+0xe28,0)
 calls=[]
 def trace(uc,at,size,user):
  if at in (0x3df140,0x3deca0):calls.append(W(at,old.reg(2),old.reg(3)))
 old.uc.hook_add(UC_HOOK_CODE,trace)
 def fixture(sheets):
  for x,y,s in zip(oo,no,sheets):old.uc.mem_write(x,bytes(4)+P(s));new.uc.mem_write(y,P(s))
 def compare(label):
  expected=b''.join(bytes(old.uc.mem_read(x+4,896)) for x in oo);actual=b''.join(bytes(new.uc.mem_read(x,896)) for x in no)
  assert expected==actual,(label,[(i,x,y) for i,(x,y) in enumerate(zip(struct.unpack('<896i',expected),struct.unpack('<896i',actual))) if x!=y][:8]);return expected
 rng=random.Random(0x47505635);edge=[-1,0,1,256,-256,2147483647,-2147483648]
 def sheet():return [x if rng.randrange(3)==0 else rng.choice(edge) if rng.randrange(2) else rng.randrange(-2147483648,2147483648) for x in d]
 stats=[];powers=[];caps=[];source_calls=0
 # Actual item types/params, both hands, entire gear sheet including unchanged fields.
 for i,b in enumerate(records):
  for left in (0,1):
   before=sheet();fixture([d,d,before,d]);calls.clear();old.invoke(0x3e3154,[owner,i,left]);new.uc.mem_write(nitem,b);assert new.invoke('dh2_gear_stats_v5',[no[2],nd,nitem,left])==0
   after=compare(('item',i,left))[1792:2688];stats.append(W(left)+b+P(before)+after);source_calls+=len(calls)
 # Reset all source defaults including nonzero/OID sentinels.
 fixture([d,d,sheet(),d]);old.invoke(0x3defac,[owner]);assert new.invoke('dh2_gear_reset_v5',[no[2],nd])==0;compare('reset')
 for kind in list(range(52))+[-1,2147483647,-2147483648]:
  for left in (0,1):
   for repeat in range(8):
    count=1 if repeat<7 else 17;rows=[(kind,rng.choice(edge),rng.randrange(-2147483648,2147483648)) for _ in range(count)];before=sheet();fixture([d,d,before,d]);old.uc.mem_write(power,W(0,0,0,count,entries)+bytes(20));old.uc.mem_write(entries,b''.join(W(0,*x) for x in rows));new.uc.mem_write(nentry,b''.join(struct.pack('<3i',*x) for x in rows));new.uc.mem_write(nview+0x100,struct.pack('<QII',nentry,count,0));calls.clear()
    old.invoke(0x3e32b4,[owner,0,left]);assert new.invoke('dh2_gear_power_v5',[no[2],nd,nview+0x100,left])==0;after=compare(('power',kind,left,repeat))[1792:2688];powers.append(W(left,count)+b''.join(struct.pack('<3i',*x) for x in rows)+P(before)+after);source_calls+=len(calls)
 for i in range(512):
  before=[sheet() for _ in range(4)];fixture(before);old.invoke(0x3bd140,[character]);assert new.invoke('dh2_gear_validate_vitals_v5',[nview])==0;after=compare(('cap',i));caps.append(b''.join(P(x) for x in before)+after)
 gold=b'GPV5'+W(len(stats),len(powers),len(caps))+P(d)+P(t)+b''.join(stats)+b''.join(powers)+b''.join(caps)
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(gold)
 sources=['port/game-data/item_gear_properties_v5.hpp','port/game-data/item_gear_properties_v5.cpp','port/game-data/properties.cpp','port/game-data/class_tables.cpp','port/game-data/data.cpp',str(Path(__file__).relative_to(R)).replace('\\','/')]
 out={'validation':'PASS','original_sha256':sha(R/'.local-inputs/libDungeonHunter2.so'),'arm64_library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'actual_item_records':len(records),'item_both_hands_comparisons':len(stats),'power_comparisons':len(powers),'power_dispatch_types':55,'source_raw_sheet_calls':source_calls,'vitals_entire_owner_comparisons':len(caps),'reset_defaults_comparisons':1,'comparisons':len(stats)+len(powers)+len(caps)+1,'mismatches':0,'source_sha256':{x:sha(R/x) for x in sources},'scope':__doc__,'ItemPower_decoder_parity':False,'visual_Skin_factory_parity':False,'item_presentation_parity':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out))
if __name__=='__main__':main()
