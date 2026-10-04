"""Actual Crypt non-player registration request producer with cache-backed source tables.

The original SetAnimationSet and its recursive table helpers execute. Manager
registration/resource loading, character IsPlayer, constants lookup, FX/audio
and script services are observed fixtures. This is not a cache-loader replay.
"""
import argparse, hashlib, json, struct, sys, zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE

HERE=Path(__file__).resolve().parent
REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
sys.path.insert(0,str(REPO/'port/level-world/tools'))
sys.path.insert(0,str(REPO/'port/android-native/tools'))
from compiled_transforms_differential import Cpu,words,word,nodes
from prepare_actors import strings
from prepare_player_combat import animation_tables
from bundle_locomotion import constants
from inventory import EXPECTED

def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--engine',type=Path,default=REPO/'.local-inputs/libDungeonHunter2.so')
 p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
 p.add_argument('--output',type=Path,default=HERE/'probe.json')
 p.add_argument('--resources-output',type=Path);p.add_argument('--character',default='Crypt_Skeleton');a=p.parse_args()
 manifest=json.loads((HERE/'original-functions.json').read_text())
 assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 with a.cache.open('rb') as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==EXPECTED
 inputs={}
 with zipfile.ZipFile(a.cache) as archive:
  index={}
  for e in archive.infolist():index.setdefault(e.filename.lower(),[]).append(e)
  def read(path):
   found=index['com.gameloft.android.gand.gloftd2ss/files/'+path.lower()];assert len(found)==1
   raw=archive.read(found[0]);inputs[found[0].filename]={'entry':found[0].filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()};return raw
  def table(name):return read('data/pydata/'+name)
  names,_=strings(table('character_properties_pyarraynames.bin'));fields,_=strings(table('character_properties_pystructnames.bin'))
  row=names.index(a.character);props=struct.unpack_from('<224i',table('character_properties_pyarray.bin'),4+row*896)
  table_id=props[fields.index('AnimTable')];skill_id=props[fields.index('SkillTree')]
  seqs,chars=animation_tables(table('animations_pyarray.bin'));raw=table('animations_pystructnames.bin')
  for _ in range(4):state_names,n=strings(raw);raw=raw[n:]
  paths,_=strings(table('animations_dictionary_pyarray.bin'));cst=constants(table('animations_pycst.bin'))
  model_paths,_=strings(table('character_models_dictionary_pyarray.bin'));model_id=props[fields.index('ModelFile')];model_blob=read(model_paths[model_id])
  model={'dictionary_id':model_id,'path':model_paths[model_id],**inputs['com.gameloft.android.GAND.GloftD2SS/files/'+model_paths[model_id].lower()],'scene_nodes':len(nodes(model_blob))}
  skillraw=table('skills_pyarray.bin');off=0
  def take(fmt):
   nonlocal off
   x=struct.unpack_from(fmt,skillraw,off)[0];off+=struct.calcsize(fmt);return x
  def ints():return [take('<i') for _ in range(take('<i'))]
  skill_lists=[ints() for _ in range(take('<i'))];skills=[]
  for _ in range(take('<i')):
   anim=take('<i');take('<B');ints();take('<i');take('<B');take('<i');take('<i')
   n=take('<i');off+=n;take('<B');take('<i');take('<i');n=take('<i');off+=n;take('<i');take('<i');take('<i');skills.append(anim)
  assert off==len(skillraw),(off,len(skillraw))
  cpu=Cpu(a.engine,False,manifest);seqbase=cpu.data+0x10000;stepbase=cpu.data+0x100000;charbase=cpu.data+0x200000
  listbase=cpu.data+0x300000;skillbase=cpu.data+0x310000;itembase=cpu.data+0x320000
  stepcursor=stepbase;itemcursor=itembase
  for i,s in enumerate(seqs):
   cpu.uc.mem_write(seqbase+i*20,words([0,s['loop']&0xffffffff,len(s['steps']),stepcursor,s['type']&0xffffffff]))
   for step in s['steps']:
    b=bytearray(56)
    for offset,key in ((8,'anim'),(12,'blend_out'),(16,'cam'),(24,'fx'),(40,'redir'),(44,'sound')):struct.pack_into('<i',b,offset,step[key])
    struct.pack_into('<f',b,48,step['speed']);cpu.uc.mem_write(stepcursor,bytes(b));stepcursor+=56
  native_fields={};fieldcursor=4
  for j,name in enumerate(state_names):native_fields[hex(fieldcursor)]=name;fieldcursor+=8 if j in (15,31) else 4
  for i,record in enumerate(chars):
   b=bytearray(160);cursor=4
   for j,values in enumerate(record):
    if j in (15,31):
     struct.pack_into('<II',b,cursor,len(values),itemcursor);cpu.uc.mem_write(itemcursor,words([x&0xffffffff for x in values]));itemcursor+=len(values)*4;cursor+=8
    else:struct.pack_into('<i',b,cursor,values[0]);cursor+=4
   assert cursor==160;cpu.uc.mem_write(charbase+i*160,bytes(b))
  for i,items in enumerate(skill_lists):
   cpu.uc.mem_write(listbase+i*12,words([0,len(items),itemcursor]));cpu.uc.mem_write(itemcursor,words(items));itemcursor+=len(items)*4
  for i,anim in enumerate(skills):cpu.uc.mem_write(skillbase+i*76,bytes(76));cpu.uc.mem_write(skillbase+i*76+4,words([anim&0xffffffff]))
  for name,value in [('AnimTable4sizeE',len(seqs)),('AnimTable7membersE',seqbase),('CharAnimTable4sizeE',len(chars)),('CharAnimTable7membersE',charbase),('SkillListTable4sizeE',len(skill_lists)),('SkillListTable7membersE',listbase),('SkillTable7membersE',skillbase)]:
   typename=name.split('4sizeE')[0].split('7membersE')[0];symbol='_ZN6Arrays'+str(len(typename))+name;cpu.uc.mem_write(cpu.symbols[symbol],words([value]))
  actor=cpu.data+0x400000;animator=actor+0x2000;vtable=actor+0x3000;player_service=actor+0x4000
  cpu.uc.mem_write(actor,bytes(0x2000));cpu.pointer(actor,vtable);cpu.pointer(vtable+0x28,player_service)
  cpu.uc.mem_write(actor+0x1000,words([table_id&0xffffffff]));cpu.uc.mem_write(actor+0x1068,words([skill_id&0xffffffff]));cpu.uc.mem_write(actor+0x1004,words([model_id&0xffffffff]));cpu.pointer(animator+4,actor)
  model_got=0x3a31f8+8+word(bytes(cpu.uc.mem_read(0x3a3220,4)),0);model_slot=model_got+word(bytes(cpu.uc.mem_read(0x3a3224,4)),0);model_global=word(bytes(cpu.uc.mem_read(model_slot,4)),0);cpu.uc.mem_write(model_global,words([len(model_paths)]))
  assert cpu.invoke(0x3a31e8,[actor])==model_id
  calls=[];table_calls=[];constant_queries=[];existing=False;preload=[];idle_calls=[]
  def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
  def services(uc,address,size,user):
   if address==player_service:ret(0)
   elif address==0x475404:ret(int(existing))
   elif address==0x4c4bdc:
    group=cpu.string(cpu.reg(1)).decode();label=cpu.string(cpu.reg(2)).decode();value=cst[group][label];constant_queries.append({'group':group,'key':label,'value':value});ret(value)
   elif address in (0x476398,0x47653c):
    calls.append({'kind':'template' if address==0x476398 else 'clip','set_id':cpu.reg(1),'clip_id':cpu.reg(2),'path':paths[cpu.reg(2)]});ret()
   elif address==0x3c9c7c:table_calls.append({'template':True,'sequence':cpu.reg(2),'stance':cpu.reg(3)})
   elif address==0x3c9d80:
    flags,mask=struct.unpack('<II',uc.mem_read(uc.reg_read(cpu.sp),8));table_calls.append({'template':False,'sequence':cpu.reg(2),'stance':cpu.reg(3),'flags':flags,'mask':mask})
   elif address in (0x337888,0x337a88,0x3140ec,0x3139ac,0x318254):ret()
   elif address==0x3cacb0:idle_calls.append({'animator_is_embedded':cpu.reg(0)==actor+0x49c,'sequence':cpu.reg(1),'flags':word(bytes(uc.mem_read(actor+0x520,4)),0)});ret()
   elif address in (0x3699fc,0x4967e8):preload.append({'service':hex(address),'id':cpu.reg(1)});ret()
  cpu.uc.hook_add(UC_HOOK_CODE,services)
  try:cpu.invoke(0x3c9f4c,[animator],budget=5000000)
  except Exception:
   print('PC',hex(cpu.uc.reg_read(cpu.pc)), 'regs',[hex(cpu.reg(i)) for i in range(4)]);raise
  fresh=calls[:];authored_skill_id=skill_id;skill_id=cpu.invoke(0x3bc5c0,[actor]);expected_set=cpu.invoke(0x3a54b4,[actor]);assert all(x['set_id']==expected_set for x in fresh)
  assert fresh and fresh[0]['kind']=='template' and fresh[0]['clip_id']==seqs[chars[table_id][33][0]]['steps'][0]['anim'],(constant_queries,table_calls[:8],len(table_calls),hex(cpu.invoke(0x3a3264,[actor])),expected_set)
  existing=True;calls=[];cpu.invoke(0x3c9f4c,[animator]);assert calls==[]
  unique=list(dict.fromkeys(x['clip_id'] for x in fresh));resources=[]
  for cid in unique:
   blob=read(paths[cid]);root=word(blob,32);types={};unsupported=[]
   for ti in range(word(blob,root+36)):
    record=word(blob,root+40)+ti*32;channel=word(blob,record+16);t=word(blob,channel+8);types[str(t)]=types.get(str(t),0)+1
    if t not in (1,5,10) or word(blob,record+4)!=1 or word(blob,record+12)!=1 or word(blob,record+28):unsupported.append(ti)
   if a.resources_output:
    a.resources_output.mkdir(parents=True,exist_ok=True);(a.resources_output/(str(cid)+'.bdae')).write_bytes(blob)
   resources.append({'clip_id':cid,'path':paths[cid],**inputs['com.gameloft.android.GAND.GloftD2SS/files/'+paths[cid].lower()], 'tracks':word(blob,root+36),'start':word(blob,root+28),'end':word(blob,root+32),'scene_nodes':len(nodes(blob)),'channel_type_counts':types,'unsupported_domain_tracks':unsupported})
  template=next(x for x in resources if x['clip_id']==fresh[0]['clip_id'])
  registration_constants=constant_queries[:];idle_probes=[]
  for suppressed in [0,1,255]:
   constant_queries=[];idle_calls=[];cpu.uc.mem_write(actor+0x538,bytes([suppressed]));cpu.uc.mem_write(actor+0x520,words([0x55]));cpu.invoke(0x3c3020,[cpu.data+0x480000,3,actor,actor+0x4fc],stack=(0,0,0));flags=word(bytes(cpu.uc.mem_read(actor+0x520,4)),0)
   assert flags==(0x55 if suppressed else 0x2380);assert len(idle_calls)==int(not suppressed)
   idle_probes.append({'suppressed':suppressed,'flags':flags,'calls':idle_calls[:],'constant_queries':constant_queries[:]})
  constant_queries=registration_constants
  subset={955,956,957,958,959,960,961,962,963,967,969,971,1023,1040,1041,1114,1126}
  projection=[{'clip_id':cid,'first_library_index':next(i for i,x in enumerate(fresh) if x['clip_id']==cid),'path':paths[cid]} for cid in unique if cid in subset]
  report={'validation':'PASS','original_sha256':manifest['original_sha256'],'cache_sha256':EXPECTED,'manifest_sha256':hashlib.sha256((HERE/'original-functions.json').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'character':a.character,'authored_skill_list':authored_skill_id,'character_row':row,'animation_table':table_id,'skill_list':skill_id,'skill_sequences':[skills[i] for i in skill_lists[skill_id]],'animation_set_id':expected_set,'constant_queries':constant_queries,'model_property':model,'native_character_table_fields':native_fields,'template':template,'registration_calls':fresh,'registration_calls_count':len(fresh),'unique_clip_count':len(unique),'first_unique_order':unique,'table_calls':table_calls,'preload_requests':preload,'existing_set_registration_calls':0,'prototype_projection':projection,'prototype_projection_count':len(projection),'resources':resources,'inputs':list(inputs.values()),'row160_projection':{'words':[struct.unpack_from('<I',bytes(cpu.uc.mem_read(charbase+table_id*160,160)),i*4)[0] if i not in (17,34) else 0 for i in range(40)],'pointer_words_zeroed':[17,34],'interact':chars[table_id][15],'spells':chars[table_id][31]},'idle_focus_probes':idle_probes,'source_idle_sequence':chars[table_id][9][0],'first_dictionary_engine_index':{str(cid):next(i for i,x in enumerate(fresh) if x['clip_id']==cid) for cid in unique},'scope':'Actual SetAnimationSet, recursive table registration and Character table/skill/model-ID getters execute on cache-backed structs with actual non-player branch. Character IsPlayer, constant lookup, manager/resource-loading, script/FX/audio services are fixtures. Clip registrations are call requests; load-probe separately proves duplicate append. GetCharModelName equipment/game-state override branches are not executed here. No live equipment/property inheritance or full cache lifecycle claim.'}
  a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','set_id':expected_set,'template':template,'calls':len(fresh),'unique':len(unique),'projection_count':len(projection)}))

if __name__=='__main__':main()
