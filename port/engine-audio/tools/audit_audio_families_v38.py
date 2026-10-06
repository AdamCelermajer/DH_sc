"""Bounded exact-ELF producer census; logical IDs are not XML UID bindings."""
from pathlib import Path
import sys,struct,json,hashlib,zipfile,bisect
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
O=R/'port/engine-audio/reference/event-families-v38';O.mkdir(parents=True,exist_ok=True)
BUILD=R/'.local-inputs/audio-v38'
raw=(R/'.local-inputs/libDungeonHunter2.so').read_bytes()
elf=ELFFile((R/'.local-inputs/libDungeonHunter2.so').open('rb'))
syms=[s for s in elf.get_section_by_name('.dynsym').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_value'] and s['st_size']]
syms.sort(key=lambda s:s['st_value']);starts=[s['st_value'] for s in syms]
def symbol(a):
 i=bisect.bisect_right(starts,a)-1
 if i>=0 and a<syms[i]['st_value']+syms[i]['st_size']:return syms[i]
def bytes_at(a,n):
 for s in elf.iter_segments():
  if s['p_type']=='PT_LOAD' and s['p_vaddr']<=a and a+n<=s['p_vaddr']+s['p_filesz']:return s.data()[a-s['p_vaddr']:a-s['p_vaddr']+n]
 raise ValueError(hex(a))
targets={0x36b5d8:'Play3D',0x3692f0:'ConvertVisual3DToSound3D',0x3699fc:'LoadSound'}
for s in syms:
 if 'VoxSoundManager' in s.name and any(t in s.name for t in ('PlaySound','Play2D','PlayMusic')):targets[s['st_value']]=s.name
xrefs=[];owners={}
for sec in elf.iter_sections():
 if not (sec['sh_flags']&4):continue
 b=sec.data()
 for i in range(0,len(b)-3,4):
  w=struct.unpack_from('<I',b,i)[0]
  if (w&0x0e000000)!=0x0a000000:continue
  delta=w&0xffffff;delta=delta-(1<<24) if delta&(1<<23) else delta
  a=sec['sh_addr']+i;t=(a+8+delta*4)&0xffffffff
  if t not in targets:continue
  s=symbol(a)
  if not s:continue
  # Literal pools are disassembled too; preserve branch opcode and classify
  # a static candidate until the owning function's control flow is inspected.
  xrefs.append(dict(callsite=hex(a),target=hex(t),operation=targets[t],opcode=hex(w),owner=s.name,owner_address=hex(s['st_value']),owner_bytes=s['st_size']))
  owners[s['st_value']]=s
captures=[]
for a,s in sorted(owners.items()):
 b=bytes_at(a,s['st_size']);name=f'{a:08x}.asm'
 (O/name).write_text('# '+s.name+'\n'+''.join(f'{i.address:08x} {i.mnemonic:8} {i.op_str}\n' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(b,a)))
 captures.append(dict(address=hex(a),name=s.name,bytes=len(b),sha256=hashlib.sha256(b).hexdigest(),file=name))
census=json.loads((R/'port/level-world/reference/audio-source-v34/routing-census.json').read_text())
logical={r['id']:r['name'] for r in census['logical']}
class Reader:
 def __init__(self,b):self.b=b;self.p=0
 def take(self,n):assert n<=len(self.b)-self.p;v=self.b[self.p:self.p+n];self.p+=n;return v
 def w(self):return struct.unpack('<i',self.take(4))[0]
 def text(self):return self.take(self.w()).decode().rstrip('\0')
 def words(self):return [self.w() for _ in range(self.w())]
 def names(self):return [self.text() for _ in range(self.w())]
archive=Path(r'C:/Users/adamc/Downloads/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
prefix='com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'
asset_hashes={}
with zipfile.ZipFile(archive) as z:
 def read(n):
  b=z.read(prefix+n);asset_hashes[n]=hashlib.sha256(b).hexdigest();return Reader(b)
 r=read('animations_pyarray.bin');n=read('animations_pyarraynames.bin');names=n.names();sequence=[]
 assert r.w()==len(names)
 for id,name in enumerate(names):
  loop=r.w();steps=[]
  for ix in range(r.w()):
   anchor=r.take(1)[0];anim=r.w();blend=r.w();cam=r.w();camdir=r.take(1)[0];fx=r.w();move=r.take(1)[0];randomcam=r.words();redir=r.w();sound=r.w();speed=struct.unpack('<f',r.take(4))[0];swoosh=r.take(1)[0]
   steps.append(dict(index=ix,anim=anim,redir=redir,sound=sound,legacy183_label_untrusted=logical.get(sound),swoosh=bool(swoosh),fx=fx,anchor=bool(anchor)))
  sequence.append(dict(id=id,name=name,loop=loop,steps=steps,type=r.w()))
 r=read('loot_table_pyarray.bin');n=read('loot_table_pyarraynames.bin')
 for _ in range(r.w()):r.words()
 r.take(r.w()*2)
 for _ in range(r.w()):r.take(r.w()*7)
 for _ in range(3):n.names()
 item_names=n.names();items=[];assert r.w()==len(item_names)
 for id,name in enumerate(item_names):
  icon=r.text();pickup=r.w();distance=r.w();sound=r.w();fx=r.w();stack=r.take(1)[0];other=[r.w() for _ in range(11)];label=r.text();tail=[r.w() for _ in range(20)]
  items.append(dict(id=id,name=name,label=label,swoosh_sound=sound,legacy183_label_untrusted=logical.get(sound),swoosh_fx=fx,audiovisual=tail[0],type=tail[1]))
 r=read('loot_audiovisual_pyarray.bin');n=read('loot_audiovisual_pyarraynames.bin');avnames=n.names();avs=[];assert r.w()==len(avnames)
 for id,name in enumerate(avnames):
  drop=r.w();pickup=r.w();visual=r.text();avs.append(dict(id=id,name=name,drop_sound=drop,pickup_sound=pickup,legacy183_drop_label_untrusted=logical.get(drop),legacy183_pickup_label_untrusted=logical.get(pickup),visual=visual))
 assert r.p==len(r.b)
r=Reader((R/'port/level-world/reference/openable-container-data-v11/openable-containers-group5-records.bin').read_bytes());n=Reader((R/'port/level-world/reference/openable-container-data-v11/openable-containers-group5-names.bin').read_bytes());cn=n.names();containers=[];assert r.w()==len(cn)
for id,name in enumerate(cn):
 field=r.w();sound=r.w();keep=r.take(1)[0];loot=r.w();script=r.text();f1=r.w();f2=r.w();visual=r.w()
 containers.append(dict(id=id,name=name,sound=sound,loot=loot,script=script,visual=visual))
assert r.p==len(r.b)
ai_blob=(R/'port/level-world/reference/character-target-events/target-events-fixtures.bin').read_bytes();assert ai_blob[:4]==b'CTE1';ai_count=struct.unpack_from('<I',ai_blob,8)[0];ai_sounds=list(struct.unpack_from('<'+'i'*ai_count,ai_blob,12))
sys.path.insert(0,str(R/'port/game-data/tests'))
from items_differential import Original
pc=Original(R/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
with zipfile.ZipFile(archive) as z:
 pb=z.read(prefix+'projectiles_pyarray.bin');pn=Reader(z.read(prefix+'projectiles_pyarraynames.bin')).names()
pc.blob=pb;pc.cursor=4;assert struct.unpack_from('<I',pb)[0]==len(pn)
projectiles=[]
for id,name in enumerate(pn):
 row=pc.data+0x50000+id*72;pc.uc.mem_write(row,bytes(72));start=pc.cursor;pc.invoke(0x4edbb8,[row,pc.stream],budget=30000)
 w=list(struct.unpack('<18i',pc.uc.mem_read(row,72)))
 projectiles.append(dict(id=id,name=name,serialized_start=start,serialized_end=pc.cursor,impact1_sound=w[3],impact_default_sound=w[6],impact2_3_sound=w[12],runtime_words=w))
assert pc.cursor==len(pb)
asset_hashes['projectiles_pyarray.bin']=hashlib.sha256(pb).hexdigest()
reader=next(s for s in syms if s['st_value']==0x4edbb8);b=bytes_at(reader['st_value'],reader['st_size']);(O/'004edbb8.asm').write_text('# '+reader.name+'\n'+''.join(f'{i.address:08x} {i.mnemonic:8} {i.op_str}\n' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(b,reader['st_value'])))
report=dict(status='SOURCE_CENSUS',original_sha256=hashlib.sha256(raw).hexdigest(),scope='Exact original direct branch candidates and cached producer indices; no UID mapping or live audio claim.',targets={hex(a):n for a,n in targets.items()},xrefs=xrefs,captures=captures,asset_sha256=asset_hashes,animation_sequences=sequence,items=items,item_audiovisual=avs,character_sound_rows=census['characters'])
report.update(openable_containers=containers,ai_sound_ids=ai_sounds,projectiles=projectiles,projectile_reader=dict(address='0x4edbb8',sha256=hashlib.sha256(b).hexdigest(),consumed=pc.cursor,rows=len(projectiles)))
(O/'census.json').write_text(json.dumps(report,indent=2)+'\n')
binding_path=R/'port/engine-audio/reference/source-bindings-v38/ledger.json'
if binding_path.exists():
 bindings=json.loads(binding_path.read_text());assert len(bindings)==638 and all(b['source_id']==i for i,b in enumerate(bindings))
 rows=[]
 def emit(family,producer,subject,id,**extra):
  if id<0:return
  assert id<len(bindings),(family,subject,id)
  b=bindings[id];rows.append(dict(family=family,producer=producer,subject=subject,source_id=id,source_name=b['source_name'],uid=b['uid'],event=b['event'],exact_assets=b['assets'],asset_status=b['status'],producer_status='done',runtime_status='partial',**extra))
 for row in sequence:
  for s in row['steps']:
   if not s['redir']:emit('animation-step','0x3ca79c',row['name'],s['sound'],sequence_id=row['id'],step=s['index'],swoosh=s['swoosh'],conditional='only when main/offhand SFX fallback remains' if s['swoosh'] else 'ordinary step')
 for row in items:emit('weapon-swoosh','0x3c94b8',row['name'],row['swoosh_sound'],item_id=row['id'])
 for row in avs:
  emit('item-drop','0x3ec0f0',row['name'],row['drop_sound'],av_id=row['id'],position='cached Item+0x1a8')
  emit('item-pickup','0x3ed144',row['name'],row['pickup_sound'],av_id=row['id'],position='raw Item+0x160')
 for row in containers:emit('chest-container','0x3a0b38',row['name'],row['sound'],container_id=row['id'])
 for row in census['characters']:
  for kind,lst in zip(('death','hit','impact1','impact2'),row['lists']):
   for id in lst:emit('combat-'+kind,'0x3afee0',row['name'],id)
 for row in projectiles:
  for kind,key in ((1,'impact1_sound'),('default','impact_default_sound'),('2/3','impact2_3_sound')):emit('projectile-impact','0x3e4db0',row['name'],row[key],projectile_id=row['id'],impact_type=kind)
 summary={}
 for family in sorted(set(r['family']for r in rows)):
  selected=[r for r in rows if r['family']==family];summary[family]=dict(producer_occurrences=len(selected),distinct_source_ids=sorted(set(r['source_id']for r in selected)),asset_status_counts={s:sum(r['asset_status']==s for r in selected)for s in ('done','partial','missing')},runtime_status='partial: root wiring and guarded audible acceptance pending')
 joined=dict(scope='Every nonnegative decoded producer ID joins genuine source638 rows; done/partial/missing asset status is inherited exact filename availability, not audible acceptance.',binding_sha256=hashlib.sha256(binding_path.read_bytes()).hexdigest(),families=summary,rows=rows,ai_target_in_sight=dict(rows=76,source_sound_ids=[-1],status='done source no-sound IDs; no artificial replacement'),known_contract_defects=[dict(path='port/android-native/app/src/main/cpp/renderer_animation_sound_v4.inc',current='true/0',source='false/1',proof_callsites=['0x3c953c','0x3ca8e0']),dict(path='port/level-world/character_target_events.cpp',current='flag1/integer0',source='flag0/integer1',proof_callsite='0x3d23b8')])
 joined['named_animation_leaf']=dict(producer_status='done',runtime_status='partial: root melee_event_sound_fx wiring and guarded audible acceptance pending',eligible_source_name_rows=638,header='port/engine-audio/audio_named_animation_sound_v38.hpp',trigger='existing melee owner step_index then step_count then sfx_ prefix removal',miss='source no-op before manager/position/Play3D',matched_order=['capture_actual_manager','actual_character_target_position','Play3D false/1/-1/-1 unchanged XYZ'],proof='named-animation-original-proof.json',native_host_receipt='named-animation-host-receipt.json',provider_failure='missing/null manager is explicit failure; null-original-manager prefix parity is not claimed')
 if (R/'port/engine-audio/reference/target-events-v38/manifest.json').exists():
  joined['target_in_sight_successor']=dict(producer_status='done',runtime_status='partial: root generic callee/CMake integration and guarded live acceptance pending',entry='dh2_audio_target_event_v38',header='port/engine-audio/audio_target_events_v38.hpp',source='port/engine-audio/audio_target_events_v38.cpp',reference='port/engine-audio/reference/target-events-v38/manifest.json',original_arm64_comparisons=2247,source_bool=False,source_integer=1,oracle_correction='r3 is bool; stack first word is integer, old test had swapped projection')
 (O/'family-ledger.json').write_text(json.dumps(joined,indent=2)+'\n')
 print(json.dumps(dict(family_summary={name:{k:v for k,v in row.items()if k in ('producer_occurrences','asset_status_counts')}for name,row in summary.items()})))
print(json.dumps(dict(xrefs=len(xrefs),functions=len(captures),animation_sequences=len(sequence),animation_sound_steps=sum(s['sound']>=0 for r in sequence for s in r['steps']),swoosh_steps=sum(s['swoosh'] for r in sequence for s in r['steps']),items=len(items),item_sound_counts={str(k):sum(i['swoosh_sound']==k for i in items)for k in sorted(set(i['swoosh_sound'] for i in items))},containers=len(containers),container_sound_ids=sorted(set(c['sound']for c in containers)),ai_sound_ids=sorted(set(ai_sounds)))))
if '--proof' in sys.argv:
 sys.path.insert(0,str(R/'port/game-data/tests'))
 from items_differential import Original
 from unicorn import UC_HOOK_CODE
 c=Original(R/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
 def word(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
 def put(a,v):c.pointer(a,v&0xffffffff)
 def ret(v=0):c.returned(v)
 animator=c.data+0x20000;actor=c.data+0x22000;item=c.data+0x25000;itemrow=c.data+0x26000;position=c.data+0x27000;manager=c.data+0x28000;seqrow=c.data+0x29000;step=c.data+0x2a000
 c.uc.mem_write(actor,bytes(0x1800));c.uc.mem_write(animator,bytes(128));put(animator+4,actor)
 position_bits=[0x42c80000,0xc3480000,0x80000000];c.uc.mem_write(position,struct.pack('<3I',*position_bits))
 base=0x3c94cc+8+word(0x3c9554);put(word(base+word(0x3c9558)),manager)
 base=0x3ca7b4+8+word(0x3cab24);put(word(base+word(0x3cab28)),seqrow)
 c.uc.mem_write(seqrow,bytes(20));put(seqrow+8,1);put(seqrow+12,step)
 events=[];queries=[]
 offitem=item+0x80;offrow=itemrow+0x80;main_fx=off_fx=0
 def hook(uc,a,size,u):
  if a==0x3f9e08:queries.append('item');ret(itemrow if c.reg(0)==item else offrow)
  elif a==0x3935dc:assert c.reg(0)==actor;queries.append('position');ret(position)
  elif a==0x3ffe3c:queries.append('equipped'+str(c.reg(1)));ret(item if c.reg(1)==1 else offitem)
  elif a==0x3c955c:queries.append('main_fx' if c.reg(1)==item else 'off_fx');ret(main_fx if c.reg(1)==item else off_fx)
  elif a in (0x3a4d5c,0x3c9924,0x495d14):ret()
  elif a==0x36b5d8:
   stack=struct.unpack('<3I',c.uc.mem_read(c.uc.reg_read(c.sp),12));pos=list(struct.unpack('<3I',c.uc.mem_read(c.reg(2),12)))
   assert c.reg(0)==manager and c.reg(3)==0 and stack==(1,0xbf800000,0xbf800000) and pos==position_bits
   events.append(dict(id=struct.unpack('<i',struct.pack('<I',c.reg(1)))[0],position_bits=pos,source_bool=False,source_integer=1,float_bits=list(stack[1:])))
   ret()
 c.uc.hook_add(UC_HOOK_CODE,hook)
 fixtures=[]
 for row in items:
  put(itemrow+0x14,row['swoosh_sound']);events=[];queries=[];rc=c.invoke(0x3c94b8,[animator,item],budget=20000)
  assert rc==int(row['swoosh_sound']!=-1) and [e['id']for e in events]==([] if row['swoosh_sound']==-1 else [row['swoosh_sound']])
  fixtures.append(dict(kind='item_swoosh',item_id=row['id'],queries=list(queries),result=rc,events=list(events)))
 events=[];queries=[];assert c.invoke(0x3c94b8,[animator,0],budget=20000)==0 and not queries and not events
 fixtures.append(dict(kind='null_item',queries=[],events=[]))
 # Ordinary step sound prefix, exact cached sound words; camera/FX/visual
 # receiver inputs are absent caller fixtures to isolate the audio prefix.
 for row in sequence:
  for s in row['steps']:
   if s['redir'] or s['swoosh']:continue
   c.uc.mem_write(step,bytes(56));put(step+8,-1);put(step+0x10,-1);put(step+0x18,-1);put(step+0x2c,s['sound']);put(step+0x30,0x3f800000)
   events=[];queries=[];c.invoke(0x3ca79c,[animator,0],budget=20000)
   assert [e['id']for e in events]==[s['sound']]
   fixtures.append(dict(kind='ordinary_step',sequence_id=row['id'],step=s['index'],queries=list(queries),events=list(events)))
 # Source invalid step-index return precedes every producer callback.
 events=[];queries=[];c.invoke(0x3ca79c,[animator,1],budget=20000);assert not events and not queries
 fixtures.append(dict(kind='invalid_step',events=[],queries=[]))
 import itertools
 for main_sound,off_sound,main_fx,off_fx in itertools.product((-1,478),(-1,479),(0,1),(0,1)):
  c.uc.mem_write(step,bytes(56));put(step+8,-1);put(step+0x10,-1);put(step+0x18,-1);put(step+0x2c,232);put(step+0x30,0x3f800000);c.uc.mem_write(step+0x34,b'\1')
  put(itemrow+0x14,main_sound);put(offrow+0x14,off_sound)
  events=[];queries=[];c.invoke(0x3ca79c,[animator,0],budget=30000)
  sound_missing=main_sound==-1 and off_sound==-1
  if not main_fx:sound_missing=not off_fx # original3cab18 overwrites r8
  want=([main_sound]if main_sound!=-1 else [off_sound]if off_sound!=-1 else [])+([232]if sound_missing else [])
  assert [e['id']for e in events]==want
  fixtures.append(dict(kind='swoosh_branch',main_sound=main_sound,off_sound=off_sound,main_fx_success=main_fx,off_fx_success=off_fx,queries=list(queries),events=list(events)))
 projectile=c.data+0x30000;ptable=c.data+0x32000;c.uc.mem_write(projectile,bytes(0x400))
 base=0x3e4dc0+8+word(0x3e4edc);put(word(base+word(0x3e4ee0)),ptable)
 for row in projectiles:
  c.uc.mem_write(ptable+row['id']*72,struct.pack('<18i',*row['runtime_words']))
  put(projectile+0x374,row['id'])
  for kind in (0,1,2,3,4):
   events=[];queries=[];c.invoke(0x3e4db0,[projectile,kind,position],budget=20000)
   sound=-1 if kind==0 else row['impact1_sound'] if kind==1 else row['impact2_3_sound'] if kind in (2,3) and row['impact2_3_sound']!=-1 else row['impact_default_sound']
   assert [e['id']for e in events]==([] if sound==-1 else [sound])
   fixtures.append(dict(kind='projectile_impact',projectile_id=row['id'],impact_type=kind,events=list(events)))
 from aggro_differential import Cpu
 native=Cpu(BUILD/'producer-oracle-arm64.so',True,{'functions':[]});inp=native.data+0x1000;out=native.data+0x1100
 native_comparisons=0
 for f in fixtures:
  if f['kind']!='swoosh_branch':continue
  native.uc.mem_write(inp,struct.pack('<4i',f['main_sound'],f['off_sound'],f['main_fx_success'],f['off_fx_success']));native.uc.mem_write(out,bytes(32))
  assert native.invoke('audio_swoosh_fixture_v38',[inp,out],budget=20000)==0
  values=struct.unpack('<8i',native.uc.mem_read(out,32));assert list(values[2:2+values[1]])==[e['id']for e in f['events']]
  assert bool(values[0]&2)==(not f['main_fx_success']);native_comparisons+=1
 for pos in ([0,0,0],[0x42c80000,0xc3480000,0x80000000],[0x7fc12345,0x7f801234,0x7f800000],[0xff800000,1,0x80000001]):
  native.uc.mem_write(inp,struct.pack('<3I',*pos));native.uc.mem_write(out,bytes(32));assert native.invoke('audio_request_fixture_v38',[inp,out])==0
  assert list(struct.unpack('<8I',native.uc.mem_read(out,32)))==pos+[0,1,0xbf800000,0xbf800000,0xffffffff];native_comparisons+=1
 proof=dict(status='PASS',cases=len(fixtures),native_arm64_comparisons=native_comparisons,original_sha256=hashlib.sha256(raw).hexdigest(),native_oracle_sha256=hashlib.sha256((BUILD/'producer-oracle-arm64.so').read_bytes()).hexdigest(),production_sha256={p:hashlib.sha256((R/p).read_bytes()).hexdigest()for p in ('port/engine-audio/audio_world_producer_v38.hpp','port/engine-audio/audio_animation_swoosh_v38.hpp')},scope='Whole source ItemSwooshSFX1322 cached fields/null;1158 ordinary SetAnimStep audio prefixes+invalid step;16 Swoosh branches;205 whole Projectile HandleImpactFX cases over41 original-reader rows. Named GetItem/GetTargetPosition/Play3D/equipment/FX and absent visual/camera/no-visual-end are observer inputs. ARM64 successor20 comparisons reproduce16 original Swoosh traces and4 exact-bit request cases. No live audio/UID binding claim.',fixtures=fixtures)
 (O/'producer-original-proof.json').write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps(dict(proof='PASS',cases=len(fixtures))))
if '--named-proof' in sys.argv:
 from unicorn import UC_HOOK_CODE
 class NamedOriginal(Original):
  def text(self,p):
   b=bytearray()
   while self.uc.mem_read(p+len(b),1)!=b'\0':b.extend(self.uc.mem_read(p+len(b),1));assert len(b)<4096
   return bytes(b)
  def external(self,uc,address,size,unused):
   if address==0x30ec7c or self.imports.get(address)=='strncmp':
    n=self.reg(2);a=bytes(uc.mem_read(self.reg(0),n));b=bytes(uc.mem_read(self.reg(1),n));self.returned((a>b)-(a<b))
   else:super().external(uc,address,size,unused)
 c=NamedOriginal(R/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
 def word(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
 def put(a,v):c.pointer(a,v&0xffffffff)
 ai=c.data+0x20000;actor=c.data+0x22000;ptrs=c.data+0x24000;heap=c.data+0x26000;position=c.data+0x32000;event=c.data+0x33000
 c.uc.mem_write(ai,bytes(128));c.uc.mem_write(actor,bytes(0x1800));put(ai+4,actor)
 names=[row['source_name']for row in json.loads(binding_path.read_text())]
 for i,name in enumerate(names):
  b=name.encode()+b'\0';put(ptrs+i*4,heap);c.uc.mem_write(heap,b);heap+=len(b)
 base=0x3d4444+8+word(0x3d4968);count_global=word(base+word(0x3d4980));names_global=word(base+word(0x3d4984));manager_global=word(base+word(0x3d4988))
 put(count_global,len(names));put(names_global,ptrs)
 position_bits=[0x42c80000,0xc3480000,0x80000000];c.uc.mem_write(position,struct.pack('<3I',*position_bits));trace=[];played=[]
 def hook(uc,a,z,u):
  if a==0x3c932c:trace.append('step_index');c.returned(7)
  elif a==0x3c934c:trace.append('step_count');c.returned(9)
  elif a==0x3935dc:
   assert c.reg(0)==actor and c.reg(9)==1;trace.append('position');put(manager_global,9);c.returned(position)
  elif a==0x36b5d8:
   stack=struct.unpack('<3I',uc.mem_read(uc.reg_read(c.sp),12));p=list(struct.unpack('<3I',uc.mem_read(c.reg(2),12)))
   assert c.reg(0)==1 and c.reg(3)==0 and stack==(1,0xbf800000,0xbf800000) and p==position_bits
   played.append(c.reg(1));trace.append('play');c.returned()
 c.uc.hook_add(UC_HOOK_CODE,hook)
 cases=['sfx_'+names[i]for i in (0,637,33,190,478)]+['sfx_not_an_authored_sound_v38','sfx_','sfx_'+names[0].lower()]
 blob=b'NAM8'+struct.pack('<I',len(cases));fixtures=[]
 for name in cases:
  c.uc.mem_write(event,name.encode()+b'\0');put(manager_global,1);trace=[];played=[]
  rc=c.invoke(0x3d4434,[ai,event],budget=100000);id=names.index(name[4:])if name[4:]in names else -1
  assert rc==1 and trace==(['step_index','step_count'] if id<0 else ['step_index','step_count','position','play']) and played==([]if id<0 else[id])
  b=name.encode();blob+=struct.pack('<I',len(b))+b+struct.pack('<iI',id,len(trace));fixtures.append(dict(event=name,source_id=id,source_return=rc,trace=trace,played=played,manager_capture='1 remains captured when GetTargetPosition changes actual global to9'))
 (O/'named-animation-gold.bin').write_bytes(blob)
 (O/'named-animation-original-proof.json').write_text(json.dumps(dict(status='PASS',cases=len(fixtures),original_sha256=hashlib.sha256(raw).hexdigest(),scope='Whole original CharAI._OnAnimEvent3d4434 over genuine638 source names; step/count/position/Play3D/string compare services are observers. Original manager snapshot precedes position; observer mutates actual global to9 and captured1 survives. No runtime playback claim.',fixtures=fixtures),indent=2)+'\n')
 print(json.dumps(dict(named_original='PASS',cases=len(fixtures))))
owned=[R/'port/engine-audio/tools/audit_audio_families_v38.py',R/'port/engine-audio/audio_world_producer_v38.hpp',R/'port/engine-audio/audio_animation_swoosh_v38.hpp',R/'port/engine-audio/audio_named_animation_sound_v38.hpp']+[p for p in O.iterdir()if p.is_file() and p.name!='manifest.json']
dependencies=['port/level-world/character_combat_sound_v1.hpp','port/level-world/character_animation_swoosh_v4.hpp','port/level-world/reference/audio-source-v34/routing-census.json','port/engine-audio/reference/source-bindings-v38/ledger.json','port/engine-audio/audio_source_bindings_v38.hpp','port/engine-audio/audio_source_bindings_v38.cpp','port/engine-audio/reference/source-bindings-v38/sdd_dungeon_hunter_2_iphone_pyarray.bin','port/engine-audio/reference/source-bindings-v38/sdd_dungeon_hunter_2_iphone_pyarraynames.bin','port/level-world/character_melee_animation_event_v1.hpp','port/level-world/character_melee_animation_event_v1.cpp','port/level-world/reference/character-target-events/target-events-fixtures.bin','port/level-world/reference/openable-container-data-v11/openable-containers-group5-records.bin','port/level-world/reference/openable-container-data-v11/openable-containers-group5-names.bin','port/level-world/reports/character-combat-sound-v1-arm64-differential.json']
def hashed(p):return dict(path=p.relative_to(R).as_posix(),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest())
(O/'manifest.json').write_text(json.dumps(dict(version=38,scope='Independent source producer handoff only; shared files unmodified; no APK/runtime claim.',original_sha256=hashlib.sha256(raw).hexdigest(),owned=[hashed(p)for p in sorted(owned)],dependencies=[hashed(R/p)for p in dependencies if(R/p).exists()],asset_sha256=asset_hashes),indent=2)+'\n')
