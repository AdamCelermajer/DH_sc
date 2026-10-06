from pathlib import Path
import sys,struct,json,hashlib,zipfile,argparse
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
elf=root/'.local-inputs/libDungeonHunter2.so';ELFSHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80';assert hashlib.sha256(elf.read_bytes()).hexdigest()==ELFSHA
c=Cpu(elf,False,{'functions':[]});obj=c.data+0x1000;heap0=c.data+0x5000;heap=heap0;calls=[];writes=[];mode='ctor';fixture={};extra_allocations=0
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def text(p):
 b=bytearray()
 while c.uc.mem_read(p,1)!=b'\0':b+=c.uc.mem_read(p,1);p+=1
 return b.decode()
def ret(value=None):
 if value is not None:c.put(0,value)
 c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
fixtures={0x3ff330:'ItemInventoryC1',0x3dbb0c:'CharTimersC1',0x3c8ff4:'CharAnimatorC1',0x3c1b58:'CharStateMachineC1',0x3df084:'CharPropertiesC1',0x3a6a24:'NetStructCharacterC1',0x4a191c:'TargetListSetRefObject',0x3db480:'CharTimersSetCharacter',0x3cb7c0:'CharAISetCharacter',0x3c9890:'CharAnimatorSetCharacter',0x3c1600:'CharStateMachineSetCharacter',0x3dec0c:'CharPropertiesSetCharacter',0x3c7318:'RegisterState',0x3ce810:'global_CharAI_deque_storage'}
def hook(uc,address,size,user):
 global heap,extra_allocations
 if mode=='ctor' and address==0x310570:
  n=c.reg(0);assert c.reg(1)==0
  if n==0x1f90:out=obj
  else:assert n==16;out=heap;heap+=0x100;extra_allocations+=1
  calls.append({'allocator_fixture':n});ret(out)
 elif mode=='ctor' and address==0x38c398:assert c.reg(1)==0;calls.append({'GameObjectC1_fixture_GO_ID':0});ret()
 elif mode=='ctor' and address==0x31167c:
  target=c.reg(0);assert c.reg(1)==16;c.pointer(target+0x10,target);c.pointer(target+0x14,target);ret()
 elif mode=='ctor' and address in fixtures:calls.append({'subobject_fixture':fixtures[address],'this_offset':c.reg(0)-obj});ret()
 elif mode=='model' and address==0x3a31e8:calls.append('GetCharModelId_actual')
 elif mode=='model' and address==0x3a3094:calls.append('IsFaerie_actual')
 elif mode=='model' and address==0x3a3054:calls.append('GetCharType_fixture');ret(3 if fixture['faery'] else (1 if fixture['player'] else 0))
 elif mode=='model' and address==isplayer_address:calls.append('IsPlayer_fixture');ret(fixture['player'])
 elif mode=='model' and address==0x38174c:calls.append('IsHighPerformance_fixture');ret(fixture['high'])
 elif mode=='model' and address==0x36effc:calls.append('IsLocalPlayer_fixture');ret(fixture['local'])
 elif mode=='model' and address==0x3aeac0:
  assert c.reg(0)==master and c.reg(1)==fixture['saved_current'];calls.append('GetCharFaery_fixture');c.pointer(faery+12,fixture['override']&0xffffffff);ret(faery)
 elif mode=='producer_prefix' and address==0x3d4d9c:
  calls.append('stop_after_actual_master_store_before_AI_behavior');c.uc.reg_write(c.pc,0x3d4e94) # explicit oracle-only skip to original stack epilogue
def writehook(uc,access,address,size,value,user):
 if mode=='ctor' and (address<=obj+0x418<address+size or address<=obj+0x13c8<address+size or address<=obj+0x13ca<address+size):writes.append({'pc':hex(c.uc.reg_read(c.pc)),'offset':hex(address-obj),'bytes':size,'value':hex(value)})
c.uc.hook_add(UC_HOOK_CODE,hook);c.uc.hook_add(UC_HOOK_MEM_WRITE,writehook)
got=0x994a98;deque=word(got+0x49ac);assert deque
cases=[]
for poison in (0,0xa5,0x7f):
 c.uc.mem_write(obj,bytes([poison])*0x1f90);calls.clear();writes.clear();heap=heap0;extra_allocations=0
 c.pointer(deque+0x10,heap0+0x1000);c.pointer(deque+0x18,heap0+0x1004)
 c.invoke(0x340800,[]);assert c.reg(0)==obj
 fields={'charprops13c8':struct.unpack('<h',c.uc.mem_read(obj+0x13c8,2))[0],'template13ca':struct.unpack('<h',c.uc.mem_read(obj+0x13ca,2))[0],'master418':word(obj+0x418)}
 assert fields=={'charprops13c8':-1,'template13ca':-1,'master418':0},fields
 assert any(w['pc']=='0x3cec70' and w['offset']=='0x418' for w in writes)
 cases.append({'poison':poison,'fields':fields,'writes':list(writes),'intercepted_services':list(calls)})
isplayer_address=word(word(obj)+0x28)
mode='model';master=c.data+0x18000;save=c.data+0x1b000;faery=c.data+0x1d000
prefix='com.gameloft.android.GAND.GloftD2SS/files/';cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
with cache.open('rb') as f:cache_sha=hashlib.file_digest(f,'sha256').hexdigest()
assert cache_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
z=zipfile.ZipFile(cache)
def names(raw):
 at=0
 def integer():
  nonlocal at;v=struct.unpack_from('<i',raw,at)[0];at+=4;return v
 n=integer();out=[]
 for _ in range(n):count=integer();out.append(raw[at:at+count].decode());at+=count
 assert at==len(raw);return out
files=names(z.read(prefix+'data/pydata/character_models_dictionary_pyarray.bin'));model_names=names(z.read(prefix+'data/pydata/character_models_dictionary_pyarraynames.bin'));assert len(files)==len(model_names)==116
table=c.data+0x20000;strings=c.data+0x23000
for i,value in enumerate(files):raw=value.encode()+b'\0';c.uc.mem_write(strings,raw);c.pointer(table+i*12+8,strings);strings+=len(raw)
c.pointer(word(got+0x3c0c),len(files));c.pointer(word(got+0x4344),table)
current_difficulty=word(got+0x1a9c);assert current_difficulty;c.pointer(current_difficulty,1);c.pointer(master+0x14e8,save)
base={'model':32,'faery':0,'player':0,'high':1,'local':1,'owner':0,'override':34,'class':0,'saved_current':2}
inputs=[{'name':'missing_base_model','model':-1,'faery':1,'owner':master}, {'name':'negative_base','model':-2}, {'name':'out_of_bounds','model':116}, {'name':'NPC_authored_base'}, {'name':'faery_C1_NULL_master','faery':1}, {'name':'faery_live_master_override','faery':1,'owner':master}, {'name':'faery_override_missing','faery':1,'owner':master,'override':-1}, {'name':'high_performance_player','player':1}, {'name':'low_performance_local_player','player':1,'high':0}, {'name':'low_performance_remote_mage','player':1,'high':0,'local':0,'class':290}, {'name':'low_performance_remote_rogue','player':1,'high':0,'local':0,'class':325}, {'name':'low_performance_remote_warrior','player':1,'high':0,'local':0,'class':263}]
model_cases=[]
def name_prefix(raw):
 n=struct.unpack_from('<i',raw,0)[0];at=4;out=[]
 for _ in range(n):size=struct.unpack_from('<i',raw,at)[0];at+=4;out.append(raw[at:at+size].decode());at+=size
 return out,at
char_raw=z.read(prefix+'data/pydata/character_properties_pyarray.bin');char_names,char_names_end=name_prefix(z.read(prefix+'data/pydata/character_properties_pyarraynames.bin'));char_fields,char_fields_end=name_prefix(z.read(prefix+'data/pydata/character_properties_pystructnames.bin'));assert len(char_fields)==224 and struct.unpack_from('<i',char_raw,0)[0]==len(char_names)
default_fairy=char_names.index('DefaultFairy');default_values=struct.unpack_from('<224i',char_raw,4+default_fairy*896);assert default_fairy==97 and default_values[3]==34 and char_fields[3]=='ModelFile'
inputs.append({'name':'original_DefaultFairy_C1_master_NULL','model':default_values[3],'faery':1})
for addition in inputs:
 fixture={**base,**addition};calls.clear();c.pointer(obj+0x1004,fixture['model']&0xffffffff);c.pointer(obj+0x418,fixture['owner']);c.uc.mem_write(obj+0x13c8,struct.pack('<h',fixture['class']));c.pointer(save+0xac+4,fixture['saved_current']);c.invoke(0x3a54d4,[obj]);result=c.reg(0);result_text=text(result) if result else None
 if fixture['model']==-1 or fixture['model']<-1 or fixture['model']>=116:expected=None
 elif fixture['faery'] and fixture['owner']:expected=files[fixture['override']] if fixture['override']>=0 else None
 elif fixture['player'] and not fixture['high'] and not fixture['local']:expected=files[{290:75,325:76,263:77}[fixture['class']]]
 else:expected=files[fixture['model']]
 assert result_text==expected,(fixture,result_text,expected)
 if fixture['name']=='missing_base_model':assert calls==['GetCharModelId_actual']
 if fixture['name']=='faery_C1_NULL_master':assert 'GetCharFaery_fixture' not in calls
 model_cases.append({'fixture':fixture,'result':result_text,'calls':list(calls),'original_saved_current_lookup_executed':fixture['model']!=-1 and fixture['faery'] and bool(fixture['owner'])})
producer_cases=[]
mode='producer_prefix';calls.clear();c.invoke(0x3d4d80,[obj+0x3c8,master]);assert word(obj+0x418)==master;assert calls==['stop_after_actual_master_store_before_AI_behavior'];producer_cases.append({'input_master':hex(master),'output_field418':hex(word(obj+0x418)),'scope':'Original nonNULL SetMaster prefix; stops before main-owned AI behavior'})
mode='producer_null';c.invoke(0x3d4d80,[obj+0x3c8,0]);assert word(obj+0x418)==0;producer_cases.append({'input_master':0,'output_field418':0,'scope':'Whole original NULL-master SetMaster branch'})
report={'validation':'PASS','original_elf_sha256':ELFSHA,'cache_sha256':cache_sha,'factory':{'address':'0x340800','allocation':0x1f90,'GO_ID':0,'C1':'0x3aa1b4','CharAI_subobject':'0x3c8','master_offset':'0x3c8+0x50=0x418'},'constructor_cases':cases,'model_cases':model_cases,'master_producer_cases':producer_cases,'DefaultFairy_authored_row':{'row':97,'AI':default_values[1],'AnimTable':default_values[2],'ModelFile':default_values[3],'model_name':model_names[default_values[3]],'asset':files[default_values[3]],'names_prefix_bytes':char_names_end,'records_prefix_bytes':4+len(char_names)*896,'records_total_bytes':len(char_raw),'payload_sha256':hashlib.sha256(char_raw).hexdigest()},'model_dictionary':{'rows':len(files),'payload_sha256':hashlib.sha256(z.read(prefix+'data/pydata/character_models_dictionary_pyarray.bin')).hexdigest(),'names_sha256':hashlib.sha256(z.read(prefix+'data/pydata/character_models_dictionary_pyarraynames.bin')).hexdigest(),'low_performance_remote_rows':{str(k):{'name':model_names[k],'file':files[k]} for k in [75,76,77]}},'scope':'Whole original Character factory/C1 body and CharAI C1 stores execute; unrelated inherited/subobject/storage services explicitly intercepted. Original GetCharModelName, GetCharModelId and IsFaerie bodies execute over real original model dictionary; GetCharType/IsPlayer/device/local-player/Faery row are explicit domain fixtures. Original SG_GetCurrentFaerieId executes against a difficulty1 save-storage fixture. AI_SetMaster nonNULL source store prefix and whole NULL branch execute separately. No whole InitPost, stage lifecycle, actual player equipment/save or production faery selection is claimed.'}
p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','constructor_poison_cases':len(cases),'model_cases':len(model_cases),'C1_fields':cases[0]['fields'],'output':str(a.output)}))
