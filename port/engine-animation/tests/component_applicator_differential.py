"""Original float component contribution/apply/real node setters vs O2 ARM64.

Node virtual slots point to the actual original Position/Scale setters. Any
getter or unexpected virtual service fails. Imported soft-float contracts are
modeled; copied bytes compare exactly and arithmetic NaNs by classification.
"""
import argparse,hashlib,json,math,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from compiled_transforms_differential import Cpu,words,word,sha
from animation_blend_differential import bits,floating,same_word

class FactoryCpu(Cpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__cxa_guard_acquire':self.put(0,1)
  elif name=='__cxa_guard_release':uc.mem_write(self.reg(0),words([1]))
  elif name=='__aeabi_atexit':self.put(0,0)
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True)
 p.add_argument('--report',type=Path,default=ROOT/'reports/component-applicator-arm64-differential.json');p.add_argument('--reference-output',type=Path,default=ROOT/'reference/component-applicator/original-corpus.bin');a=p.parse_args();started=time.monotonic()
 manifest_path=ROOT/'reference/component-applicator/original-functions.json';manifest=json.loads(manifest_path.read_text());assert sha(a.engine)==manifest['original_sha256']
 old=FactoryCpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});node=old.data+0x1000;vtable=node+0x400;trap=node+0x800;values=node+0x1000;weights=values+0x1000;output=weights+0x1000
 native_node=new.data+0x1000;native_values=native_node+0x1000;native_weights=native_values+0x1000;native_output=native_weights+0x1000
 for offset in range(0,0x200,4):old.pointer(vtable+offset,trap)
 old.pointer(vtable+0xa4,0x59712c);old.pointer(vtable+0x94,0x5970c4);old.pointer(node,vtable)
 setter_calls=[]
 def observe(uc,address,size,user):
  if address==trap:raise AssertionError('Unexpected node virtual getter/service')
  if address in (0x59712c,0x5970c4):setter_calls.append({'slot':0xa4 if address==0x59712c else 0x94,'value':bytes(uc.mem_read(old.reg(1),12))})
 old.uc.hook_add(UC_HOOK_CODE,observe)
 handlers={}
 for kind,base in [('Position',2),('Scale',11)]:
  for axis,label in enumerate('XYZ'):
   tag='CSceneNode'+kind+label+'ExIfE';found=[r for r in manifest['functions'] if tag in r['original_symbol']];methods={}
   for method,fragment in [('size','12getValueSize'),('get','15getBlendedValue'),('apply','10applyValue'),('blend','17applyBlendedValue')]:methods[method]=int(next(r['elf_address'] for r in found if fragment in r['original_symbol']),16)
   assert old.invoke(methods['size'],[0])==12;handlers[base+axis]=methods
 # Actual factory and lazy singleton initialization determine numeric type IDs;
 # the C++ guard/atexit imports alone are modeled, never track selection.
 track=old.data+0x10000;channel=track+0x100;old.pointer(track+0x10,channel);old.pointer(track+0x1c,0)
 factories={}
 for type_ in (2,3,4,6,7,8,9,11,12,13):
  old.pointer(channel+8,type_);instance=old.invoke(0x611ae0,[track]);assert instance
  table=word(bytes(old.uc.mem_read(instance,4)),0)
  methods=[word(bytes(old.uc.mem_read(table+offset,4)),0) for offset in range(0,144,4)]
  factories[type_]=[methods[2],methods[4]]
  if type_ in handlers:
   assert factories[type_]==[handlers[type_]['size'],handlers[type_]['get']],(type_,factories[type_],handlers[type_])
   assert handlers[type_]['apply'] in methods and handlers[type_]['blend'] in methods
 assert factories[6]==factories[7]==factories[8]==factories[9]
 assert factories[7]!=factories[11]
 factory_imports=dict(old.import_calls)
 rng=random.Random(0xc012ca11);special=[0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0xffc05678,1,0x7f7fffff]
 cases=[];initial=words([bits(31),bits(32),bits(33),bits(4),bits(5),bits(6),0x400])
 for type_ in handlers:
  for k in range(104):
   n=[0,1,2,3,7,31][k%6] if k<12 else rng.randrange(0,13)
   for operation in range(3):
    count=1 if operation==1 else n;raw_values=words([rng.choice(special) if k<12 else bits(rng.uniform(-1000,1000)) for _ in range(count*3)])
    wc=count if count>1 or (count==1 and k%2) else 0
    raw_weights=words([rng.choice(special) if k<12 else bits(rng.uniform(-2,2)) for _ in range(wc)])
    cases.append((type_,operation,count,wc,raw_values,raw_weights))
 gold=words([0x31504143,len(cases)]);counts={'contributions':0,'direct_applies':0,'blended_applies':0,'position_setter_calls':0,'scale_setter_calls':0};nan_class_comparisons=0
 def pack_node():return bytes(old.uc.mem_read(node+0xac,12))+bytes(old.uc.mem_read(node+0xc8,12))+bytes(old.uc.mem_read(node+0x11c,4))
 for ci,(type_,operation,count,wc,raw_values,raw_weights) in enumerate(cases):
  old.uc.mem_write(node+0xac,initial[:12]);old.uc.mem_write(node+0xc8,initial[12:24]);old.uc.mem_write(node+0x11c,initial[24:]);new.uc.mem_write(native_node,initial)
  if raw_values:old.uc.mem_write(values,raw_values);new.uc.mem_write(native_values,raw_values)
  if raw_weights:old.uc.mem_write(weights,raw_weights);new.uc.mem_write(native_weights,raw_weights)
  vp=values if count else 0;wp=weights if wc else 0;nvp=native_values if count else 0;nwp=native_weights if wc else 0
  setter_calls.clear();old.uc.mem_write(output,words([bits(1),bits(2),bits(3)]));new.uc.mem_write(native_output,words([bits(1),bits(2),bits(3)]))
  if operation==0:
   old.invoke(handlers[type_]['get'],[0,vp,wp,count,output]);assert new.invoke('dh2_animation_component_blend',[native_output,type_,nvp,nwp,count])==0;expected_output=bytes(old.uc.mem_read(output,12));actual_output=bytes(new.uc.mem_read(native_output,12));counts['contributions']+=1
  elif operation==1:
   old.invoke(handlers[type_]['apply'],[0,vp,node,0]);assert new.invoke('dh2_animation_component_apply',[native_node,type_,nvp])==0;expected_output=pack_node()[:12] if type_<=4 else pack_node()[12:24];actual_output=bytes(new.uc.mem_read(native_node+(0 if type_<=4 else 12),12));counts['direct_applies']+=1
  else:
   old.invoke(handlers[type_]['blend'],[0,vp,wp,count,node,0]);assert new.invoke('dh2_animation_component_apply_blended',[native_node,type_,nvp,nwp,count])==0;expected_output=pack_node()[:12] if type_<=4 else pack_node()[12:24];actual_output=bytes(new.uc.mem_read(native_node+(0 if type_<=4 else 12),12));counts['blended_applies']+=1
  expected_state=pack_node();actual_state=bytes(new.uc.mem_read(native_node,28));arithmetic=operation!=1 and count>1
  for i in range(3):
   left,right=word(expected_output,i*4),word(actual_output,i*4);assert same_word(left,right,not arithmetic),(ci,type_,operation,count,'out',expected_output.hex(),actual_output.hex());nan_class_comparisons+=arithmetic and left!=right
  for i in range(7):
   arithmetic_word=arithmetic and operation==2 and ((type_<=4 and i<3) or (type_>=11 and 3<=i<6))
   assert same_word(word(expected_state,i*4),word(actual_state,i*4),not arithmetic_word),(ci,type_,operation,count,'state',expected_state.hex(),actual_state.hex())
  if operation:
   assert len(setter_calls)==1 and setter_calls[0]['slot']==(0xa4 if type_<=4 else 0x94) and setter_calls[0]['value']==expected_output
   counts['position_setter_calls' if type_<=4 else 'scale_setter_calls']+=1
  else:assert setter_calls==[]
  gold+=words([type_,operation,count,wc])+initial+raw_values+raw_weights+expected_state+expected_output
  initial=expected_state # explicit ordered target-write sequence, not rest reset
 a.reference_output.write_bytes(gold)
 sources=[ROOT/'component_applicator.hpp',ROOT/'component_applicator.cpp',ROOT/'tests/component_applicator.cpp',Path(__file__),ROOT/'animation_blend.hpp',ROOT/'animation_blend.cpp',REPO/'port/engine-math/math.hpp',REPO/'port/engine-math/math.cpp',REPO/'port/scene-materials/scene.hpp']
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'manifest_sha256':sha(manifest_path),'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.reference_output),'source_sha256':{str(path.relative_to(REPO)).replace('\\','/'):sha(path) for path in sources},'handlers':{str(k):{name:hex(v) for name,v in row.items()} for k,row in handlers.items()},'factory_type_checks':10,'factory_methods':{str(k):[hex(v) for v in row] for k,row in factories.items()},'factory_imports':factory_imports,'value_size_checks':6,'cases':len(cases),**counts,'virtual_getter_calls':0,'arithmetic_nan_payload_differences':nan_class_comparisons,'mismatches':0,'elapsed_seconds':round(time.monotonic()-started,2),'scope':'Concrete float Position2/3/4 and Scale11/12/13 ComponentMixin tracks: actual factory and lazy singleton instructions establish numeric tags; C++ guard/atexit services modeled. Full12byte typed contribution, direct/applyBlended wrappers and actual original fullvector setters with dirty8/2. Node virtual table targets actual setters; any other virtual call fails. Imported soft-float dependencies modeled. No liveaxis getter/merge, quaternion/Euler6..9, raw defaults/compatibility/accessor/firstunion type dispatch or whole scene/runtime/GPU parity claim.'}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','handlers','scope')}))
if __name__=='__main__':main()
