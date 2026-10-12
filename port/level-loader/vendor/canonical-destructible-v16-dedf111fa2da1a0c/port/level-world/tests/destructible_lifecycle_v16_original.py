from pathlib import Path
import json,struct
r=Path(__file__).resolve().parents[3];s=(r/'port/level-world/tests/destructible_container_v16_original.py').read_text();exec(compile(s,'destructible-ctor','exec'))
calls=[];count=5;visual=c.data+0xb00000;timeline=c.data+0xb01000;vt=c.data+0xb02000;level=c.data+0xb03000
def store(p,v):c.uc.mem_write(p,struct.pack('<I',v&0xffffffff))
store(visual+0x38,timeline);store(timeline,vt);callback=c.data+0x50000
for slot,offset in [(0x10,0),(0x2c,4),(0x1c,8)]:store(vt+slot,callback+offset)
def lifecycle(uc,a,z,u):
 if a==0x39f910:calls.append({'call':'container_post'});ret()
 elif a==callback:calls.append({'call':'count'});ret(count)
 elif a==callback+4:calls.append({'call':'callbacks'});ret()
 elif a==callback+8:calls.append({'call':'index','index':c.reg(1)});ret(1)
 elif a==0x3a1084:ret(-1) # declared empty source Arrays name domain observer
 elif a==0x36b5d8:calls.append({'call':'sound','id':c.reg(1)});ret()
 elif a==0x31f594:ret(level) # explicit SAME Level receiver observation fixture
 elif a==0x4c4bdc:ret(17) # declared constant lookup fixture
 elif a==0x339090:
  q=c.reg(1);calls.append({'call':'quest','id':word(q+4),'actor':word(q+8),'room':word(q+12),'relation':word(q+20),'data_id':word(q+24)});ret()
 elif a==0x3a0b38:calls.append({'call':'container_interact','actor':c.reg(1)});ret()
c.uc.hook_add(UC_HOOK_CODE,lifecycle);cases=[]
for count in [0,1,3,4,5,0xffffffff]:
 p=c.invoke(0x340d5c,[]);store(p+0x2d8,visual);calls=[];c.invoke(0x3a11dc,[p]);expected=count-3 if count>3 else 0
 assert word(p+0x6f0)==word(p+0x6f4)==expected and [x['call'] for x in calls]==['container_post','callbacks','count'];cases.append({'count':count,'stages':expected,'calls':calls})
count=5;p=c.invoke(0x340d5c,[]);actor=c.invoke(0x3410a4,[]);store(p+0x2d8,visual);store(p+0x64,3);c.invoke(0x3a11dc,[p]);calls=[]
for step in [1,2]:
 c.invoke(0x3a0da0,[p,actor]);assert word(p+0x6f4)==2-step
assert calls==[{'call':'index','index':1},{'call':'sound','id':0xffffffff},{'call':'index','index':2},{'call':'sound','id':0xffffffff}];cases.append({'kind':'staged','calls':calls});calls=[]
c.invoke(0x3a0da0,[p,actor]);assert calls[0]['call']=='quest' and calls[1]=={'call':'container_interact','actor':actor};cases.append({'kind':'final','calls':calls})
out=r/'port/level-world/reference/destructible-container-v16';(out/'lifecycle-original.json').write_text(json.dumps({'status':'PASS','cases':cases,'scope':'whole original derived InitPost and Interact control/stores; inherited container, timeline, Level/event, sound and constant services are explicit observation fixtures. Target AsChar executes original with actual Dummy receiver and returns NULL.'},indent=2));print('Destructible original derived lifecycle PASS',len(cases))
