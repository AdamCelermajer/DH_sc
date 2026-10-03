"""Execute original room-list traversal, filters and TargetInfo heap; backend facts are fixtures."""
import hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from navigation_differential import Cpu
from aggro_differential import float_bits
REF=ROOT/'reference/character-target-search';ELF=REPO/'.local-inputs/libDungeonHunter2.so'
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def floating(x):return struct.unpack('<f',words(x))[0]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class SearchCpu(Cpu):
 def __init__(self,*a,**kw):super().__init__(*a,**kw);self.math_records={}
 def word(self,address):return struct.unpack('<I',self.uc.mem_read(address,4))[0]
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='sincosf':
   x=floating(uc.reg_read(UC_ARM64_REG_S0));self.uc.mem_write(self.reg(0),words(float_bits(math.sin(x) if math.isfinite(x) else math.nan)));self.uc.mem_write(self.reg(1),words(float_bits(math.cos(x) if math.isfinite(x) else math.nan)));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name in ('acosf','sinf','cosf'):
   raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);x=floating(raw);v=float_bits((math.acos(x) if -1<=x<=1 else math.nan) if name=='acosf' else (math.sin(x) if name=='sinf' else math.cos(x)) if math.isfinite(x) else math.nan);self.math_records[(('sinf','cosf','acosf').index(name)+1,raw)]=v
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,v)
   else:self.put(0,v)
   self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
class OriginalSearch:
 def __init__(self):
  self.manifest=json.loads((REF/'original-functions.json').read_text());self.c=SearchCpu(ELF,False,self.manifest);c=self.c
  self.owner=c.data+0x10000;self.objects=[self.owner]+[c.data+0x14000+i*0x2000 for i in range(32)]
  self.list=c.data+0x60000;self.manager=c.data+0x70000;self.context=c.data+0x71000;self.vt=c.data+0x80000;self.callbacks=[c.data+0x81000+i*32 for i in range(8)]
  for v in self.callbacks:c.uc.mem_write(v,bytes.fromhex('1eff2fe1'))
  for slot,v in zip((0x24,0x28,0x34,0x88,0x90,0x94,0xc4),self.callbacks):c.pointer(self.vt+slot,v)
  got=0x3d0034+c.word(0x3d0074);c.pointer(got+c.word(0x3d0078),self.context);c.pointer(self.context+0x38,self.manager)
  got=0x4a2748+c.word(0x4a2848);self.sorters=[c.word(got+c.word(k)) for k in (0x4a284c,0x4a2850,0x4a2854)]
  self.trace=[];self.heap=c.data+0x100000;self.case=None;self.depth=0;self.triggered=False
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def idx(self,p):return self.objects.index(p) if p in self.objects else -1
 def hook(self,uc,address,size,unused):
  c=self.c
  if address in (0x708ec0,0x310454):
   n=c.word(c.reg(0)) if address==0x708ec0 else c.reg(0);p=self.heap;self.heap+=(n+31)&~15;c.uc.mem_write(p,bytes(n));self.ret(p)
  elif address in (0x708f00,0x310440,0x337888,0x3140ec,0x337a88):self.ret()
  elif address==0x4a1cfc:
   obj=c.word(c.word(c.reg(0)+0x10)+8);i=self.idx(obj);self.trace.append(['character',i]);self.ret(obj if i>=0 and self.case['objects'][i]['character'] else 0)
  elif address==0x3d4c34:self.trace.append(['melee_radius',self.idx(c.reg(0)-0x3c8)]);self.ret(self.case['melee'])
  elif address==0x3d574c:
   assert c.reg(0)==self.owner+0x3c8;i=self.idx(c.reg(1));self.trace.append(['enemy',i]);self.ret(self.case['objects'][i]['enemy'])
  elif address in self.callbacks:
   op=('is_character','player','dead','interactive','interaction','radius','zonable','unused')[self.callbacks.index(address)];i=self.idx(c.reg(0));self.trace.append([op,i]);obj=self.case['objects'][i]
   if op in ('interactive','interaction'):assert c.reg(1)==self.owner
   if op=='interactive' and self.case.get('mutate')==i and not self.triggered:
    self.triggered=True;j=self.case['mutation_target'];self.case['objects'][j]['visible']=0;c.uc.mem_write(self.objects[j]+0x8a,b'\0');self.trace.append(['hide',j])
   if op=='interactive' and self.case.get('reentry')==i and not self.triggered:
    self.triggered=True;self.trace.append(['nested_begin',i]);saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
    try:c.invoke(0x3d0020,[self.list,self.case['radius'],self.case['cone']],budget=2000000)
    finally:c.stack=stack;c.uc.context_restore(saved)
    self.trace.append(['nested_end',i])
   self.ret(obj[{'is_character':'character','player':'player','dead':'dead','interactive':'interactive','interaction':'interaction','radius':'radius','zonable':'zonable'}.get(op,'character')])
 def execute(self,case):
  self.case=json.loads(json.dumps(case));case=self.case;c=self.c;self.heap=c.data+0x100000;self.triggered=False
  for i,o in enumerate(case['objects']):
   p=self.objects[i];c.uc.mem_write(p,bytes(0x1800));c.pointer(p,self.vt);c.uc.mem_write(p+0x8a,bytes([o['visible']]));c.uc.mem_write(p+0x2ee,bytes([o['zoned']]));c.uc.mem_write(p+0x2f0,bytes([o['in_zone']]));c.uc.mem_write(p+0x160,words(*o['position']));c.uc.mem_write(p+0x174,words(case['heading'] if i==0 else 0));c.uc.mem_write(p+0x1310,words(o['rank']));c.uc.mem_write(p+0x1314,words(o['capacity']))
   c.pointer(p+0x180,p+0x1700 if o['has_target_position'] else 0);c.uc.mem_write(p+0x80,bytes([o['has_target_position']]));c.uc.mem_write(p+0x184,words(*o['target_position']))
  # Ordered actual intrusive room/object lists, including empty rooms and null entries.
  head=self.manager+0x80;roomnodes=[c.data+0x72000+i*0x20 for i in range(len(case['rooms']))];c.pointer(head,roomnodes[0] if roomnodes else head)
  for ri,entries in enumerate(case['rooms']):
   rn=roomnodes[ri];oh=rn+16;c.pointer(rn,roomnodes[ri+1] if ri+1<len(roomnodes) else head);c.pointer(rn+8,oh)
   nodes=[c.data+0x74000+ri*0x400+j*16 for j in range(len(entries))];c.pointer(oh,nodes[0] if nodes else oh)
   for j,(node,idx) in enumerate(zip(nodes,entries)):c.pointer(node,nodes[j+1] if j+1<len(nodes) else oh);c.pointer(node+8,self.objects[idx] if idx>=0 else 0)
  c.invoke(0x4a2730,[self.list,self.owner,1,0,case['sort']]);self.trace=[]
  c.invoke(0x3d0020,[self.list,case['radius'],case['cone']],budget=2000000)
  result=[]
  while c.word(self.list)!=c.word(self.list+0x10):
   p=c.word(self.list);raw=struct.unpack('<5I',c.uc.mem_read(p,20));result.append([self.idx(raw[0]),*raw[1:]]);c.invoke(0x38fb18,[self.list])
  return {'output':result,'trace':self.trace.copy(),'after_visible':[o['visible'] for o in self.case['objects']]}
def object_(**kw):
 d={'character':1,'player':0,'dead':0,'interactive':1,'interaction':8,'radius':float_bits(0),'zonable':0,'zoned':0,'in_zone':0,'visible':1,'enemy':1,'rank':0,'capacity':0,'position':[0,float_bits(-10),0],'target_position':[0,0,0],'has_target_position':0};d.update(kw);return d
def corpus():
 cases=[];base={'objects':[object_(player=1,position=[0,0,0]),*[object_() for _ in range(12)]],'rooms':[[],list(range(13)),[]],'heading':0,'melee':0,'radius':float_bits(100),'cone':float_bits(math.pi),'sort':1}
 def add(label,**kw):v=json.loads(json.dumps(base));v.update(kw);v['label']=label;cases.append(v);return v
 for sort in (0,1,2):add('13 equal keys '+str(sort),sort=sort)
 for field,vals in [('visible',[0,1]),('zonable',[0,1]),('interactive',[0,1]),('character',[0,1]),('interaction',[7,8,0xffffffff]),('dead',[0,1]),('enemy',[0,1]),('player',[0,1]),('rank',[0,1,0xffffffff])]:
  for v in vals:o=add('filter '+field+' '+str(v));o['objects'][2][field]=v;o['objects'][2]['zoned']=1;o['objects'][2]['in_zone']=0
 for cone in (0.,.25,1.,math.pi/2,math.pi,4.,math.nan):
  for radius in (0.,9.999,10.,10.001,100.,math.nan):
   o=add('cone/radius',cone=float_bits(cone),radius=float_bits(radius));o['objects'][3]['position']=[float_bits(10),0,0];o['objects'][4]['position']=[0,float_bits(10),0];o['objects'][5]['position']=[0,0,float_bits(10)];o['objects'][6]['position']=[0,0,0]
 add('empty',rooms=[[],[]]);add('null/self/duplicates',rooms=[[-1,0,1,1],[],[2,-1,3]]);add('callback hide future',mutate=1,mutation_target=3)
 for sort in range(3):
  for i in (1,4,8):add('nested same-list search',sort=sort,reentry=i)
 for heading in (0.,math.pi/2,math.pi,-math.pi/2):add('heading',heading=float_bits(heading),cone=float_bits(.3))
 for word in (0x80000000,1,0x7f800000,0xff800000,0x7fc01234,0x7f7fffff):
  o=add('nonfinite/key words');o['objects'][2]['position'][0]=word;o['objects'][3]['radius']=word;o['objects'][4]['position']=[word,word,word]
 o=add('non-character reference');o['objects'][0]['character']=0
 for owner_alt in (0,1):
  for target_alt in (0,1):
   o=add('source alternate target position');o['objects'][0]['has_target_position']=owner_alt;o['objects'][0]['target_position']=[0,float_bits(-50),float_bits(3)];o['objects'][1]['has_target_position']=target_alt;o['objects'][1]['target_position']=[float_bits(8),float_bits(-51),float_bits(6)]
 rng=random.Random(20261113)
 for k in range(320):
  n=rng.randrange(2,25);o=add('random'+str(k),objects=[object_(player=1)],rooms=[[],[],[]],sort=rng.randrange(3),radius=float_bits(rng.choice((0,20,100))),cone=float_bits(rng.choice((.3,1,math.pi))),melee=float_bits(rng.choice((0,5,200))))
  for i in range(1,n):o['objects'].append(object_(character=rng.randrange(2),visible=int(rng.randrange(8)!=0),dead=int(rng.randrange(5)==0),enemy=int(rng.randrange(5)!=0),player=int(rng.randrange(8)==0),rank=rng.choice((0,0,1)),position=[float_bits(rng.choice((-20,-10,0,10,20))) for _ in range(3)],radius=float_bits(rng.choice((0,1,10)))))
  for i in range(n):o['rooms'][rng.randrange(3)].append(i)
 return cases
def main():
 o=OriginalSearch();cases=corpus();rows=[]
 for i,case in enumerate(cases):rows.append({'input':case,**o.execute(case)})
 report={'validation':'PASS','original_only_cases':len(rows),'native_comparisons':0,'original_sha256':sha(ELF),'script_sha256':sha(Path(__file__)),'sort_functions':o.sorters,'target_record_stride':20,'ordered_callbacks':sum(len(x['trace']) for x in rows),'accepted_records':sum(len(x['output']) for x in rows),'scope':__doc__,'cases':rows}
 (REF/'discovery-probes.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='cases'}))
if __name__=='__main__':main()
