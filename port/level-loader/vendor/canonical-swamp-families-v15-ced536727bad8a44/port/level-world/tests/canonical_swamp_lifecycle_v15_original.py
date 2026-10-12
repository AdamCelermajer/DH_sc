from pathlib import Path
import json,struct
r=Path(__file__).resolve().parents[3]
exec(compile((r/'port/level-world/tests/canonical_swamp_family_v15_original.py').read_text(),'family-original-constructors','exec'))
calls=[];script_id=-1;room_present=True
visual=c.data+0x900000;root=c.data+0x901000;room=c.data+0x902000
def store(p,v):c.uc.mem_write(p,struct.pack('<I',v&0xffffffff))
store(visual+8,root)
def lifecycle(uc,a,z,u):
 global calls
 names={0x38be5c:'base_init',0x470a54:'visual_sync',0x388a2c:'podecor_ctor',0x394bf8:'set_physical',0x523c14:'load_room',0x388218:'extend_bounds',0x4591f0:'script_id',0x393db4:'set_position',0x3938a0:'set_rotation',0x4605c0:'start_script'}
 if a not in names:return
 calls.append({'method':names[a],'arguments':[c.reg(i) for i in range(4)]})
 if a==0x523c14:ret(room if room_present else 0)
 elif a==0x4591f0:ret(script_id)
 else:ret()
c.uc.hook_add(UC_HOOK_CODE,lifecycle)
cases=[]
for physical in (0,1):
 for room_present in (False,True):
  p=c.invoke(0x3410fc,[]);store(p+0x2d8,visual);c.uc.mem_write(visual+0x28,bytes([physical]));store(room+0x24,4);calls=[]
  c.invoke(0x388a98,[p]);expected=['base_init','visual_sync']+(['podecor_ctor','set_physical'] if physical else [])+['load_room']+(['extend_bounds'] if room_present else [])
  assert [x['method'] for x in calls]==expected and c.uc.mem_read(p+0x375,1)==b'\0'
  if room_present:assert word(room+0x24)==5
  cases.append({'kind':'Decor','physical':physical,'room_present':room_present,'calls':calls})
for script_id in (-1,17):
 p=c.invoke(0x340e10,[]);calls=[];c.invoke(0x3ea28c,[p]);assert [x['method'] for x in calls]==['base_init','script_id'];assert word(p+0x390)==script_id&0xffffffff
 calls=[];c.invoke(0x3ea22c,[p,c.data+0x903000]);assert [x['method'] for x in calls]==['set_position','set_rotation']+(['start_script'] if script_id!=-1 else [])
 cases.append({'kind':'SpawnPoint','script_id':script_id,'calls':calls})
report={'status':'PASS','cases':cases,'scope':'Whole original derived InitPost/LoadFloorMap/PlaceObject control and stores. Required inherited InitPost, visual, PODecor/PF and ScriptManager calls use declared observation fixtures; not a full World acceptance claim.'}
(r/'port/level-world/reference/swamp-families-v14/family-lifecycle-original-v15.json').write_text(json.dumps(report,indent=2));print('Whole derived original lifecycle PASS',len(cases))
