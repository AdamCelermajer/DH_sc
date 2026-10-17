"""Original ItemPowerList/Table/Ref readers on all genuine cache bytes.
Storage allocation and stream virtual reads are explicit byte services; every
decoded scalar and ordered property including full signed32 Flags is compared.
"""
import sys,json,struct,hashlib,argparse,zipfile
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'));sys.path.insert(0,str(R/'port/engine-resources/tests'))
from items_differential import Original
from aggro_differential import Cpu
sys.path.insert(0,str(R/'port/level-world/tests'))
from run_character_ai_set_skills_and_spells_host import source_symbols
W=lambda *x:struct.pack('<'+'I'*len(x),*[v&0xffffffff for v in x])
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--gold',type=Path,required=True)
 local=R/'.local-inputs/libDungeonHunter2.so'
 p.add_argument('--original-elf',type=Path,default=local if local.is_file() else R.parent/'test_strategy/libDungeonHunter2.so')
 p.add_argument('--cache',type=Path,default=R.parent/'cache/files',help='Original cache files directory containing data/pydata')
 p.add_argument('--apk',type=Path,help='Verify --library is the actual lib/arm64-v8a/libdh2_game_data.so archive member')
 a=p.parse_args()
 attribution=R/'port/game-data/reference/adam-c3ae797-item-v5/attribution.json';public=R/'port/game-data/reference/adam-c3ae797-item-v5/power-fixtures.bin'
 provenance=json.loads(attribution.read_text(encoding='utf-8'));manifest={'functions':provenance['source_ranges']['item_power_tables_v5']}
 assert sha(a.original_elf)==provenance['original_library_sha256']
 source_symbols(a.original_elf,manifest)
 assert a.gold.resolve()!=public.resolve(),'Gold output must not overwrite the public fixture'
 apk=None
 if a.apk:
  member='lib/arm64-v8a/libdh2_game_data.so'
  with zipfile.ZipFile(a.apk) as archive:payload=archive.read(member)
  assert payload==a.library.read_bytes(),'Library differs from captured APK member'
  apk={'path':str(a.apk.resolve()),'sha256':sha(a.apk),'member':member,'member_sha256':hashlib.sha256(payload).hexdigest(),'extracted_library_matches':True}
 cache=a.cache/'data/pydata';input_path=cache/'item_powers_pyarray.bin'
 old=Original(a.original_elf,manifest);new=Cpu(a.library,True,{'functions':[]})
 old.blob=input_path.read_bytes();old.invoke(0x4bacc8,[old.stream],budget=30000000);begin=old.cursor;old.invoke(0x4bab7c,[old.stream],budget=30000000);assert old.cursor==len(old.blob)
 got=(0x4baba8+old.word(0x4bacb8))&0xffffffff;count=old.word(old.word(got+old.word(0x4bacbc)));table=old.word(old.word(got+old.word(0x4bacc4)))
 at=begin+4;cases=[];entry_count=0;input=new.data+0x4000;out=new.data+0x2000
 for i in range(count):
  w=list(struct.unpack('<10I',old.uc.mem_read(table+40*i,40)));scalars=W(w[1]&255,w[2],*w[5:10]);expected=b''.join(bytes(old.uc.mem_read(w[4]+j*16+4,12)) for j in range(w[3]))
  source=old.blob[at:];new.uc.mem_write(input,source);assert new.invoke('dh2_item_power_decode_v5',[out,input,len(source)])==0
  fields=bytes(new.uc.mem_read(out,48));n,ptr,used,res=struct.unpack_from('<IQII',fields,28);assert fields[:28]==scalars and n==w[3] and not res,(i,struct.unpack("<7i",fields[:28]),struct.unpack("<7i",scalars),n,w[3],res,at)
  projected=b''.join(bytes(new.uc.mem_read(ptr+12*j,12)) for j in range(n));assert projected==expected,(i,projected,expected)
  assert used==29+12*n;raw=old.blob[at:at+used];cases.append(W(used)+raw+scalars+W(n)+expected);at+=used;entry_count+=n
 assert at==old.cursor
 gold=b'IPV5'+W(count)+b''.join(cases);gold_sha=hashlib.sha256(gold).hexdigest()
 public_sha=sha(public);assert public_sha==provenance['imported_files']['port/game-data/reference/adam-c3ae797-item-v5/power-fixtures.bin']['sha256']
 assert gold_sha==public_sha,'Original-derived gold differs from public fixture'
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(gold)
 files=['port/game-data/item_power_tables_v5.hpp','port/game-data/item_power_tables_v5.cpp','port/game-data/tests/item_power_tables_v5_original.py']
 tools=sorted({str(Path(module.__file__).resolve().relative_to(R)).replace('\\','/') for module in tuple(sys.modules.values()) if getattr(module,'__file__',None) and Path(module.__file__).resolve().is_relative_to(R) and Path(module.__file__).suffix=='.py'})
 report={'validation':'PASS','comparisons':count,'original_properties_compared':entry_count,'mismatches':0,'original_path':str(a.original_elf.resolve()),'original_sha256':sha(a.original_elf),'library_path':str(a.library.resolve()),'library_sha256':sha(a.library),'apk':apk,'attribution_sha256':sha(attribution),'verified_original_ranges':manifest['functions'],'gold_sha256':sha(a.gold),'public_fixture_sha256':public_sha,'public_fixture_matches':True,'source_sha256':{x:sha(R/x) for x in files},'tool_sha256':{x:sha(R/x) for x in tools},'cache_path':str(a.cache.resolve()),'executed_input':str(input_path.relative_to(a.cache)).replace('\\','/'),'input_sha256':{str(x.relative_to(a.cache)).replace('\\','/'):sha(x) for x in sorted(cache.glob('item_powers_*.bin'))},'original_byte_reader_services':old.reads,'original_allocations':old.allocations,'native_import_calls':new.import_calls,'source_scope':__doc__,'original_reader_begin':begin,'original_reader_end':at,'native_comparison_scope':'ARM64 dh2_item_power_decode_v5 record decoder only; no powered-owner, loot gameplay, or full ItemPowerTablesV5 load/borrow parity. Stream reads and allocation remain explicit byte/storage services.','native_tls_scope':'Existing aggro_differential.Cpu fixture supplies a stable Android TLS canary; actual packaged stack-protector instructions execute. No device thread-runtime claim.','AddPower_instance_and_presentation_parity':False,'loot_gameplay_parity':False,'invocation':sys.argv}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
