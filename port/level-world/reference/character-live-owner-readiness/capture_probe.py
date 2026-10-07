"""Read-only source capture and bounded original default-reset probes.

STL lookup/empty assignment and assertion logger are explicit fixture services.
This is not a full ObjectManager XML/factory execution or a native differential.
"""
import hashlib,json,struct,sys,zipfile,xml.etree.ElementTree as ET
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent
REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/level-world/tests'))
from character_script_selection_differential import Cpu
ENGINE=REPO/'.local-inputs/libDungeonHunter2.so'
ZIP=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(ENGINE)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert sha(ZIP)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
addresses=(0x34b868,0x513d78,0x513fec,0x51419c,0x5136ec,0x513a00,0x51387c,0x3a9fe4,0x33ef7c,0x33e404,0x33e298,0x3a92b4,0x33de80,0x3a35f0,0x394f00,0x3a5784,0x3aa1b4)
functions=[];assembly=[]
with ENGINE.open('rb') as f:
 e=ELFFile(f);symbols=list(e.get_section_by_name('.symtab').iter_symbols())
 for a in addresses:
  s=next(s for s in symbols if s['st_value']==a and s['st_size'] and s['st_info']['type']=='STT_FUNC')
  seg=next(s for s in e.iter_segments() if s['p_vaddr']<=a<s['p_vaddr']+s['p_filesz'])
  f.seek(seg['p_offset']+a-seg['p_vaddr']);raw=f.read(s['st_size'])
  functions.append(dict(address=hex(a),symbol=s.name,size=len(raw),sha256=hashlib.sha256(raw).hexdigest()))
  assembly.append('\n# '+s.name)
  assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,a))
(HERE/'original-functions.asm').write_text('\n'.join(assembly)+'\n')
(HERE/'original-functions.json').write_text(json.dumps(dict(original_sha256=sha(ENGINE),functions=functions),indent=2)+'\n')
provenance=REPO/'port/android-native/app/src/main/assets/worlds/crypt01-provenance.json'
p=json.loads(provenance.read_text());names={'_prim_tmp_cultist01','_prim_tmp_cultist03','_prim_tmp_cultist05','_prim_tmp_cultist06','_prim_tmp_cultist07','_prim_tmp_cultist08','_prim_Monster','_prim_Monster02','_prim_Monster05','_prim_Monster_Ghost05','_prim_Monster_Ghost06'}
records=[];inputs=[]
with zipfile.ZipFile(ZIP) as z:
 for roomindex,room in enumerate(p['rooms']):
  name=room['gameplay'];entry=next(n for n in z.namelist() if n.endswith('/'+name));raw=z.read(entry)
  inputs.append(dict(room=roomindex,entry=entry,sha256=hashlib.sha256(raw).hexdigest()))
  for node in ET.fromstring(raw).iter('GameObject'):
   if node.get('name') not in names:continue
   fields=('ai_state','ai_state_visible','auto_spawn','spawn_delay','spawn_view_radius','char_group','char_group_role')
   authored={k:node.get(k) for k in fields}
   defaults=dict(ai_state='',ai_state_visible=True,auto_spawn=True,spawn_delay=[0,0],spawn_view_radius=0.0,char_group='',char_group_role='')
   resolved={k:authored[k] if authored[k] is not None else defaults[k] for k in fields}
   assert resolved['ai_state'] in ('','Idle') and node.get('_templateName')=='Monster'
   records.append(dict(room=roomindex,name=node.get('name'),template=node.get('_templateName'),charpropsname=node.get('charpropsname'),authored=authored,descriptor_resolved=resolved,preset_state=3))
assert len(records)==11 and sum(r['authored']['ai_state'] is None for r in records)==6
(HERE/'crypt-authoring.json').write_text(json.dumps(dict(cache_sha256=sha(ZIP),provenance_sha256=sha(provenance),inputs=inputs,records=records,scope='MGP XML read directly from canonical cache; descriptor defaults from captured instructions, not full XML factory replay.'),indent=2)+'\n')
c=Cpu(ENGINE,False,{'functions':[]});events=[]
def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
character=c.data+0x1000;receiver=character+4;descriptor=c.data+0x5000;vt=c.data+0x5100;empty=c.data+0x5200;name=c.data+0x5300
def hook(u,a,n,x):
 if a==0x30e004:events.append('assertion_log');ret()
 elif a==0x513858:
  assert c.reg(1)==receiver and c.reg(2)==name
  u.mem_write(c.reg(0),struct.pack('<II',descriptor,receiver));events.append('property_lookup_fixture');ret()
 elif a==0x3109e0:
  assert c.reg(1)==c.reg(2)==empty
  dst=c.reg(0);u.mem_write(dst+0x10,struct.pack('<II',empty,empty));events.append('empty_STL_assignment_fixture');ret(dst)
c.uc.hook_add(UC_HOOK_CODE,hook)
base=(0x5141a8+8+w(0x51420c))&0xffffffff;slot=base+w(0x514210);policy=c.data+0x9000;c.pointer(slot,policy)
probes=[]
for mode in (0,1,3):
 c.pointer(policy,mode);c.uc.mem_write(receiver,b'X'*512);events.clear();c.invoke(0x51419c,[receiver]);assert bytes(c.uc.mem_read(receiver,512))==b'X'*512
 probes.append(dict(operation='LoadTemplate',assertion_policy=mode,receiver_unchanged=True,services=events.copy()))
c.uc.mem_write(empty,b'\0');c.uc.mem_write(name,b'ai_state\0');c.pointer(descriptor,vt);c.pointer(vt+12,0x33e298);c.pointer(descriptor+4,0x13cc-4);c.pointer(descriptor+0x30,empty);c.pointer(descriptor+0x34,empty)
for row in records:
 if row['authored']['ai_state'] is not None:continue
 c.uc.mem_write(character+0x13dc,struct.pack('<II',empty,empty+99));events.clear();c.invoke(0x51387c,[receiver,name,0]);result=c.invoke(0x3a5784,[character]);assert result==3 and w(character+0x13dc)==w(character+0x13e0)==empty
 probes.append(dict(operation='missing_ai_state_reset_then_GetPreSetAIState',name=row['name'],state=result,services=events.copy()))
for offset in (0x13e4,0x1430):
 c.pointer(descriptor+4,offset-4);c.uc.mem_write(descriptor+0x20,b'\1');c.uc.mem_write(character+offset,b'\0');c.invoke(0x33de80,[descriptor,receiver]);assert c.uc.mem_read(character+offset,1)==b'\1';probes.append(dict(operation='bool_descriptor_reset',offset=hex(offset),result=1))
c.pointer(descriptor+4,0x1434-4);c.uc.mem_write(descriptor+0x20,bytes(8));c.uc.mem_write(character+0x1434,b'X'*8);c.invoke(0x3a35f0,[descriptor,receiver]);assert c.uc.mem_read(character+0x1434,8)==bytes(8);probes.append(dict(operation='delay_descriptor_reset',result=[0,0]))
c.pointer(descriptor+4,0x143c-4);c.uc.mem_write(descriptor+0x20,bytes(4));c.uc.mem_write(character+0x143c,b'X'*4);c.invoke(0x394f00,[descriptor,receiver]);assert c.uc.mem_read(character+0x143c,4)==bytes(4);probes.append(dict(operation='view_radius_descriptor_reset',word=0))
report=dict(validation='PASS',original_sha256=sha(ENGINE),capture_script_sha256=sha(Path(__file__)),original_manifest_sha256=sha(HERE/'original-functions.json'),authored_fixture_sha256=sha(HERE/'crypt-authoring.json'),original_instruction_probes=len(probes),probes=probes,full_ObjectManager_factory_executed=False,full_template_STL_map_executed=False,optimized_native_differential=False,packaged_or_live_proof=False,scope=__doc__)
(HERE/'default-probes.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',original_instruction_probes=len(probes),raw_records=len(records),empty_ai_state=6,explicit_idle=5)))
