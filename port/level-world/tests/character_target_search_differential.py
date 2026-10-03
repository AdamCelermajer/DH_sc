"""Original full room traversal/filters/heap versus O2 ARM64 borrowed registry coordinator."""
import argparse,json,struct
from character_target_search_discovery import *
OPS=['character','player','dead','interactive','interaction','radius','zonable','enemy','melee_radius','is_character','hide','nested_begin','nested_end']
FIELDS=['position','rank','capacity','visible','zoned','in_zone','character','player','dead','interactive','interaction','radius','zonable','enemy']
def object_words(o):return [*o['position'],*[o[k] for k in FIELDS[1:]],*o['target_position'],o['has_target_position']]
def same(a,b):return a==b or math.isnan(floating(a)) and math.isnan(floating(b))
class NativeSearch:
 def __init__(self,path):
  self.c=SearchCpu(path,True,{'functions':[]});c=self.c;self.objects=[c.data+0x10000+i*64 for i in range(33)];self.list=c.data+0x60000;self.heap=c.data+0x62000;self.registry=c.data+0x70000;self.svc=c.data+0x80000;self.callback=c.data+0x81000
  c.uc.mem_write(self.callback,bytes.fromhex('c0035fd6'));c.uc.mem_write(self.svc,struct.pack('<QQ',0,self.callback));c.uc.hook_add(UC_HOOK_CODE,self.hook);self.trace=[];self.triggered=False
 def hook(self,uc,address,size,unused):
  if address!=self.callback:return
  c=self.c;op,_,subject,other=struct.unpack('<IIQQ',c.uc.mem_read(c.reg(1),24));idx=(other if op==8 else subject)-1 if (other if op==8 else subject) else -1;obj=self.case['objects'][idx] if idx>=0 else None;name=OPS[op-1];self.trace.append([name,idx]);result=0;number=0
  if op in (4,5):assert other==1
  if op==8:assert subject==1
  if op==1:result=self.objects[idx] if obj and obj['character'] else 0
  elif op==9:number=self.case['melee']
  else:result=obj[{'is_character':'character','player':'player','dead':'dead','interactive':'interactive','interaction':'interaction','radius':'radius','zonable':'zonable','enemy':'enemy'}[name]]
  if op==6:number=result;result=0
  if op==4 and self.case.get('mutate')==idx and not self.triggered:
   self.triggered=True;j=self.case['mutation_target'];self.case['objects'][j]['visible']=0;c.uc.mem_write(self.objects[j]+44,b'\0');self.trace.append(['hide',j])
  if op==4 and self.case.get('reentry')==idx and not self.triggered:
   self.triggered=True;self.trace.append(['nested_begin',idx]);saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
   try:self.search()
   finally:c.stack=stack;c.uc.context_restore(saved)
   self.trace.append(['nested_end',idx])
  c.uc.mem_write(c.reg(2),struct.pack('<QII',result,number,0));c.put(0,0);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def search(self):
  c=self.c;c.uc.reg_write(UC_ARM64_REG_S0,self.case['radius']);c.uc.reg_write(UC_ARM64_REG_S0+1,self.case['cone']);assert c.invoke('dh2_target_search',[self.list,self.registry,self.svc],budget=2000000)==0
 def execute(self,case):
  self.case=json.loads(json.dumps(case));case=self.case;c=self.c;self.triggered=False
  for i,o in enumerate(case['objects']):c.uc.mem_write(self.objects[i],struct.pack('<Q9I4B',i+1,*o['position'],*o['target_position'],case['heading'] if not i else 0,o['rank'],o['capacity'],o['visible'],o['zoned'],o['in_zone'],o['has_target_position']))
  head=self.registry+16;rooms=[self.registry+0x100+i*32 for i in range(len(case['rooms']))];c.pointer(self.registry,head);c.pointer(head,rooms[0] if rooms else head)
  for ri,entries in enumerate(case['rooms']):
   rn=rooms[ri];oh=rn+16;c.pointer(rn,rooms[ri+1] if ri+1<len(rooms) else head);c.pointer(rn+8,oh);nodes=[self.registry+0x1000+ri*0x800+j*16 for j in range(len(entries))];c.pointer(oh,nodes[0] if nodes else oh)
   for j,(node,idx) in enumerate(zip(nodes,entries)):c.pointer(node,nodes[j+1] if j+1<len(nodes) else oh);c.pointer(node+8,self.objects[idx] if idx>=0 else 0)
  assert c.invoke('dh2_target_list_init',[self.list,self.heap,128,self.objects[0],case['sort'],self.svc])==0;self.trace=[];self.search();result=[];out=self.list+128
  while struct.unpack('<I',c.uc.mem_read(self.list+8,4))[0]:
   assert c.invoke('dh2_target_pop',[self.list,out])==0;raw=struct.unpack('<Q4I',c.uc.mem_read(out,24));result.append([raw[0]-1,*raw[1:]])
  return {'output':result,'trace':self.trace.copy(),'after_visible':[o['visible'] for o in self.case['objects']]}
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,default=ROOT/'reports/character-target-search-arm64-differential.json');a=p.parse_args();old=OriginalSearch();new=NativeSearch(a.library);records=[];cases=corpus();totaltrace=accepted=0
 for i,case in enumerate(cases):
  expected=old.execute(case);actual=new.execute(case);assert expected['trace']==actual['trace'],(i,case['label'],'trace',expected['trace'],actual['trace']);assert expected['after_visible']==actual['after_visible'];assert len(expected['output'])==len(actual['output']),(i,'count',expected['output'],actual['output'])
  for e,n in zip(expected['output'],actual['output']):assert e[0]==n[0] and e[3:]==n[3:] and same(e[1],n[1]) and same(e[2],n[2]),(i,case['label'],e,n)
  h=[len(case['objects']),len(case['rooms']),case['sort'],case['heading'],case['melee'],case['radius'],case['cone'],case.get('mutate',0xffffffff),case.get('mutation_target',0),case.get('reentry',0xffffffff),len(expected['output']),len(expected['trace'])];r=words(*h)
  for o in case['objects']:r+=words(*object_words(o))
  for room in case['rooms']:r+=words(len(room),*room)
  for o in expected['output']:r+=words(*o)
  for op,j in expected['trace']:r+=words(OPS.index(op)+1,j)
  r+=words(*expected['after_visible']);records.append(r);totaltrace+=len(expected['trace']);accepted+=len(expected['output'])
 math_records=old.c.math_records;gold=REF/'search-fixtures.bin';gold.write_bytes(words(0x31525354,len(records))+b''.join(records)+words(len(math_records))+b''.join(words(op,raw,v) for (op,raw),v in sorted(math_records.items())));report={'validation':'PASS','comparisons':len(records),'ordered_callbacks':totaltrace,'accepted_records':accepted,'synchronous_same_list_reentry_cases':sum('reentry' in x for x in cases),'mismatches':0,'original_sha256':sha(ELF),'arm64_library_sha256':sha(a.library),'corpus_sha256':sha(gold),'source_sha256':sha(ROOT/'character_target_search.cpp'),'header_sha256':sha(ROOT/'character_target_search.hpp'),'script_sha256':sha(Path(__file__)),'scope':__doc__,'arithmetic_nan_comparison':'unordered class; finite and non-arithmetic words exact','libm_caller_fixture_records':len(math_records),'original_import_calls':old.c.import_calls,'native_import_calls':new.c.import_calls};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
