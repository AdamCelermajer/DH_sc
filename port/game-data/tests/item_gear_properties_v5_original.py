"""Real original gear arithmetic and vitals cap versus optimized native ARM64.
Original _IsPropertySet/GetDefault/Add/Set/Resolve execute. Item records come
from the real cache via its original loader. Power-entry vectors are explicit
caller projections, not a claimed ItemPower table decoder or powered owner.
The Android ARM64 harness uses the existing stable TLS-canary and checked-
memcpy fixture; TLS behavior itself is not reconstructed here.
"""
import hashlib,json,struct,sys,random,argparse,os,zipfile
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'))
from item_inventory_v1_original import Inventory
import item_inventory_v1_original as inventory_module
import player_savegame_v1_original as savegame_module
from run_character_ai_set_skills_and_spells_host import source_symbols
sys.path.insert(0,str(R/'port/engine-resources/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
W=lambda *x:struct.pack('<'+'I'*len(x),*[v&0xffffffff for v in x])
P=lambda x:struct.pack('<224i',*x)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--apk',type=Path,help='optional packaged APK; verifies its arm64 game-data member equals --library');p.add_argument('--original-elf',type=Path,default=R/'.local-inputs/libDungeonHunter2.so');p.add_argument('--cache',type=Path,default=R/'.local-inputs',help='directory containing actors/, items-discovery/, and skill-tables/');p.add_argument('--report',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);a=p.parse_args()
 original_elf=a.original_elf.resolve();cache=a.cache.resolve()
 if not original_elf.is_file():raise FileNotFoundError(f'original ELF not found: {original_elf}')
 if not cache.is_dir():raise NotADirectoryError(f'cache directory not found: {cache}')
 attribution_path=R/'port/game-data/reference/adam-c3ae797-item-v5/attribution.json'
 attribution=json.loads(attribution_path.read_text(encoding='utf-8'))
 source_manifest={'original_sha256':attribution['original_library_sha256'],'functions':attribution['source_ranges']['item_gear_properties_v5']}
 if attribution['upstream']['commit']!='c3ae797332a82a30a586b9156cddc25445e36a4c':raise AssertionError('unexpected upstream attribution commit')
 if source_manifest['original_sha256']!=sha(original_elf):raise AssertionError('original ELF SHA-256 differs from public attribution pin')
 source_symbols(original_elf,source_manifest)
 apk_identity=None
 if a.apk:
  apk_path=a.apk.resolve()
  with zipfile.ZipFile(apk_path) as apk:
   member='lib/arm64-v8a/libdh2_game_data.so'
   apk_library=apk.read(member)
  library_bytes=a.library.read_bytes()
  if apk_library!=library_bytes:raise AssertionError('ARM64 --library bytes differ from APK member '+member)
  apk_identity={'path':str(apk_path),'sha256':sha(apk_path),'member':member,'member_size':len(apk_library),'member_sha256':hashlib.sha256(apk_library).hexdigest(),'matches_library':True}
 # Reuse the existing real ItemTable/Item reader while routing its inputs to
 # the caller-selected cache and original ELF, without editing its dependencies.
 class CacheRoot:
  def __truediv__(self,relative):
   path=Path(relative)
   if path.parts and path.parts[0]=='.local-inputs':return cache.joinpath(*path.parts[1:])
   return R/path
 input_root=CacheRoot()
 inventory_module.ROOT=input_root
 savegame_module.ROOT=input_root
 source_original=savegame_module.Original
 class ExplicitOriginal(source_original):
  def __init__(self,_path,_manifest):super().__init__(original_elf,{'functions':[]})
 savegame_module.Original=ExplicitOriginal
 # The loader executes original table readers. No item-effect fixture runs.
 loader=Inventory();records=[]
 for i in range(len(loader.itemnames)):
  w=list(struct.unpack('<41i',loader.uc.mem_read(loader.table+i*164,164)));w[0]=w[2]=w[20]=0;records.append(struct.pack('<41i',*w))
 provenance=source_manifest
 old=Cpu(original_elf,False,provenance);new=Cpu(a.library,True,{'functions':[]})
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 owner=old.data+0x1000;character=owner-0x560;table=old.data+0x100000;power=old.data+0x160000;entries=old.data+0x180000
 got=(0x3e3168+word(0x3e32ac))&0xffffffff;old.pointer(word(got+word(0x3e32b0)),table)
 for i,b in enumerate(records):old.uc.mem_write(table+i*164,b)
 got=(0x3e32c8+word(0x3e3d10))&0xffffffff;old.pointer(word(got+word(0x3e3d14)),power)
 raw=(cache/'actors/character_properties_pyarray.bin').read_bytes();d=list(struct.unpack_from('<224i',raw,4));t=list(struct.unpack_from('<224i',raw,900))
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
 checked_fixture=R/'port/game-data/reference/adam-c3ae797-item-v5/gear-fixtures.bin'
 gold_path=a.gold.resolve()
 if gold_path==checked_fixture.resolve() or (gold_path.exists() and os.path.samefile(gold_path,checked_fixture)):
  raise ValueError('--gold must not name or alias the checked-in gear fixture')
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(gold)
 checked_fixture_sha=sha(checked_fixture);generated_sha=sha(a.gold)
 fixture_match=a.gold.read_bytes()==checked_fixture.read_bytes()
 if not fixture_match:raise AssertionError(f'generated original gold differs from checked-in fixture: {generated_sha} != {checked_fixture_sha}')
 sources=['port/game-data/item_gear_properties_v5.hpp','port/game-data/item_gear_properties_v5.cpp','port/game-data/properties.cpp','port/game-data/class_tables.cpp','port/game-data/data.cpp',Path(__file__).resolve().relative_to(R).as_posix()]
 cache_inputs=[cache/'items-discovery/loot_table_pyarray.bin',cache/'items-discovery/loot_table_pyarraynames.bin',cache/'skill-tables/skills_pyarraynames.bin',cache/'actors/character_properties_pyarray.bin']
 out={'validation':'PASS','original_elf_path':str(original_elf),'original_sha256':sha(original_elf),'public_attribution_sha256':sha(attribution_path),'source_range_hashes':{row['original_symbol']:row['sha256'] for row in source_manifest['functions']},'source_ranges_verified':len(source_manifest['functions']),'arm64_library_sha256':sha(a.library),'arm64_library_path':str(a.library.resolve()),'apk_identity':apk_identity,'arm64_runtime_fixture_scope':'Existing aggro_differential.Cpu supplies a stable TPIDR_EL0 TLS canary and handles __memcpy_chk; neither TLS runtime behavior nor Android loader behavior is reconstructed by this comparison.','gold_sha256':generated_sha,'checked_in_fixture_sha256':checked_fixture_sha,'matches_checked_in_fixture':fixture_match,'actual_item_records':len(records),'item_both_hands_comparisons':len(stats),'power_comparisons':len(powers),'power_dispatch_types':55,'source_raw_sheet_calls':source_calls,'vitals_entire_owner_comparisons':len(caps),'reset_defaults_comparisons':1,'comparisons':len(stats)+len(powers)+len(caps)+1,'mismatches':0,'cache_inputs_sha256':{str(path.relative_to(cache)).replace('\\','/'):sha(path) for path in cache_inputs},'source_sha256':{x:sha(R/x) for x in sources},'scope':__doc__,'ItemPower_decoder_parity':False,'visual_Skin_factory_parity':False,'item_presentation_parity':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out))
if __name__=='__main__':main()
