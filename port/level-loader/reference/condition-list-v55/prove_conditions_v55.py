from pathlib import Path
import sys,struct,json,hashlib,collections
sys.path[:0]=['C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages','C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests']
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/condition-list-v55';elf=root/'.local-inputs/libDungeonHunter2.so'
class Original(Cpu):
 def external(self,uc,a,size,x):
  if self.imports.get(a)=='strcmp':
   def text(p):
    data=bytearray()
    while uc.mem_read(p,1)!=b'\0':data+=uc.mem_read(p,1);p+=1
    return bytes(data)
   left,right=text(self.reg(0)),text(self.reg(1));self.put(0,0 if left==right else -1 if left<right else 1);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,size,x)
c=Original(elf,False,{'functions':[]});u=c.uc
word=lambda a,v:u.mem_write(a,struct.pack('<I',v&0xffffffff));read=lambda a:struct.unpack('<I',u.mem_read(a,4))[0];signed=lambda v:struct.unpack('<i',struct.pack('<I',v))[0]
heap=c.data+0x10000;payload=(r/'v2conditions_pyarray.bin').read_bytes();cursor=0;allocations=[];frees=[];factorycalls=[]
def ret(v=0):c.put(0,v);u.reg_write(c.pc,u.reg_read(c.lr))
def allocate(n):
 global heap
 p=heap;heap+=(n+15)&~15;u.mem_write(p,bytes([0xa5])*n);allocations.append({'bytes':n,'address':p});return p
def nextword():
 global cursor
 assert cursor+4<=len(payload);v=struct.unpack_from('<I',payload,cursor)[0];cursor+=4;return v
def hook(uc,a,s,x):
 if a in [0x31056c,0x310570]:ret(allocate(c.reg(0)))
 elif a==0x310440:frees.append(c.reg(0));ret()
 elif a==0x313a90:ret(nextword())
 elif a in [0x3df1a0,0x459090]:word(c.reg(1),nextword());ret()
 elif a==0x317454:
  global cursor
  n=c.reg(2);assert cursor+n<=len(payload);u.mem_write(c.reg(1),payload[cursor:cursor+n]);cursor+=n;ret()
 elif a in [0x47899c,0x4789e8,0x478a34,0x478a80,0x478acc,0x478b14,0x478b5c]:factorycalls.append(hex(a))
u.hook_add(UC_HOOK_CODE,hook)
c.invoke(0x4b8dec,[c.data+0x200]);assert cursor==len(payload)==2796
assert read(0x9a66d4)==118;table=read(0x9a66d8);decoded=json.loads((r/'decoded-conditions-v55.json').read_text())['rows'];oracle=[]
payload=(r/'v2conditions_pyarraynames.bin').read_bytes();cursor=0;c.invoke(0x4b15f8,[c.data+0x200]);assert cursor==len(payload)==2789
for i,row in enumerate(decoded):
 p=table+16*i;n=read(p+4);stubs=read(p+8);vals=[{'op':signed(read(stubs+16*j+4)),'argument1':signed(read(stubs+16*j+8)),'argument2':signed(read(stubs+16*j+12))} for j in range(n)]
 assert n==row['count'] and signed(read(p+12))==row['list_op'] and vals==row['stubs'];oracle.append({'name':row['name'],'row':p,'stubs':stubs,'count':n})
listaddr=allocate(12);listcases=[]
for poison in [0,0xa5,0x7f]:
 u.mem_write(listaddr,bytes([poison])*12);c.invoke(0x4786f0,[listaddr]);assert [read(listaddr+x) for x in [0,4,8]]==[0,0,0]
 listcases.append({'C1_poison':poison,'fields':[0,0,0]})
for i,row in enumerate(decoded):
 c.invoke(0x4786f0,[listaddr]);factorycalls.clear();before=len(frees);ptr=table+16*i;stubs=read(ptr+8);c.invoke(0x478914,[listaddr,stubs,read(ptr+4)])
 assert read(listaddr)==row['count'] and read(listaddr+8)==stubs
 arr=read(listaddr+4);instances=[]
 for j in range(row['count']):
  actual=read(arr+4*j);assert read(actual+4)==stubs+16*j
  instances.append({'op':row['stubs'][j]['op'],'source_slot4_stub_pointer':read(actual+4),'receiver_vtable':hex(read(actual)),'source_cache8':hex(read(actual+8))})
 c.invoke(0x478eac,[listaddr]);assert read(listaddr+4)==0;assert len(frees)-before==row['count']+(1 if arr else 0)
 row['original_constructed_instances']=instances;row['original_factory_bodies']=list(factorycalls)
listcases.append({'real_cache_list_count':118,'instances_constructed_and_deleted':sum(x['count'] for x in decoded),'all_actual_factories_executed':sorted(set(s for x in decoded for s in x['original_factory_bodies']))})
# Repeated Assign overwrites array without freeing old array/conditions.
ptr=next(x for x in oracle if x['count']>0);c.invoke(0x4786f0,[listaddr]);before=len(frees);c.invoke(0x478914,[listaddr,ptr['stubs'],ptr['count']]);old=read(listaddr+4);c.invoke(0x478914,[listaddr,ptr['stubs'],ptr['count']]);assert read(listaddr+4)!=old and len(frees)==before
listcases.append({'repeated_Assign_leaks_prior_array_and_instances':True})
old=read(listaddr+4);c.invoke(0x478914,[listaddr,0,0]);assert read(listaddr)==0 and read(listaddr+4)==old and read(listaddr+8)==0
listcases.append({'zero_Assign_preserves_old_array4':True})
c.invoke(0x478914,[listaddr,123,0xffffffff]);assert read(listaddr)==0xffffffff and read(listaddr+4)==old and read(listaddr+8)==123
c.invoke(0x478eac,[listaddr]);assert read(listaddr+4)==0
listcases.append({'signed_negative_Assign_no_iteration_D1_only_frees_array':True})
inventory=json.loads((b/'reports/chapter-coverage-v36.json').read_text());swamp=next(x for x in inventory['levels'] if x['identity']=='SWAMP');rowsbyname={x['name']:x for x in decoded};refs=[]
for obj in swamp['objects']:
 if obj['authored_gametype']!='Character':continue
 for key,value in obj['attributes'].items():
  if 'activate' in key.lower() and value not in ['','Invalid']:
   refs.append({'object':obj['authored_name'],'property':key,'name':value,'source_uri':obj['source_uri'],'resolved':value in rowsbyname,'row':rowsbyname.get(value)})
conditiondata=allocate(0x24);data_cases=[]
def namefield(name):
 buf=allocate(len(name)+1);u.mem_write(buf,name.encode()+b'\0');word(conditiondata+0x14,buf+len(name));word(conditiondata+0x18,buf);word(conditiondata+0x1c,0x1234);u.mem_write(conditiondata+0x20,b'\x7f')
for name in [x['name'] for x in refs]+['','Invalid','rene_follow','absent-name']:
 namefield(name);before=len(frees);c.invoke(0x33eb28,[conditiondata]);pointer=read(conditiondata+0x1c);assert u.mem_read(conditiondata+0x20,1)==b'\x7f'
 if name in rowsbyname:
  row=rowsbyname[name];assert pointer!=0x1234 and read(pointer)==row['count'];c.invoke(0x33e7c8,[conditiondata]);assert read(conditiondata+0x1c)==0 and u.mem_read(conditiondata+0x20,1)==b'\x7f';assert len(frees)-before==row['count']+2
  data_cases.append({'name':name,'Init_Assign_C1_D1_D0_Clear_completed':True,'same_compiled_pointer_cleared_after_D1_then_CustomFree':True,'tested_byte_preserved':127})
 else:assert pointer==0x1234 and len(frees)==before;data_cases.append({'name':name,'original_early_return_or_case_sensitive_miss':True,'compiled_preserved':hex(pointer),'tested_preserved':127})
namefield('RENE_FOLLOW');c.invoke(0x33eb28,[conditiondata]);old=read(conditiondata+0x1c);before=len(frees);c.invoke(0x33eb28,[conditiondata]);assert read(conditiondata+0x1c)!=old and len(frees)==before;c.invoke(0x33e7c8,[conditiondata]);data_cases.append({'repeated_Init_no_prior_List_D1_or_Free':True,'source_old_list_leak_preserved':True})
out={'schema':'original-condition-list-v55','status':'PASS','elf_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'whole_original_array_reader':'Arrays::v2Conditions4b8dec +v2CondAnd505970 +ConditionStub50600c/base505b28; real bytes2796 all118rows; whole Arrays readNames4b15f8 actual2789bytes','whole_ConditionData_cases':data_cases,'list_cases':listcases,'swamp_character_refs':refs,'factory_types':dict(collections.Counter(s['op'] for row in decoded for s in row['stubs'])),'original_table_rows':decoded,'scope':'Allocation/stream/libc strcmp leaves only. Original table/names/ConditionData Init/Clear/ConditionList/actual factory/deleting-destructor bodies execute. No Eval/quest services invoked; no gameplay/Android activation proof.'}
(r/'original-conditions-v55.json').write_text(json.dumps(out,indent=2)+'\n');(r/'prove_conditions_v55.py').write_bytes(Path(__file__).read_bytes());print('PASS original whole table118/stubs154/constructors3/listAssign118/D1120; SWAMP',len(refs),'refs',collections.Counter(x['property'] for x in refs));print(json.dumps([{'object':x['object'],'property':x['property'],'name':x['name'],'types':[s['op'] for s in x['row']['stubs']] if x['row'] else None} for x in refs],indent=2))
