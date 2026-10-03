"""Actual dynamic compiler and typed accessor instructions versus O2 ARM64.

Library registration order is supplied. Factory allocation/type identity and
selected resource segment are explicit fixtures; dynamic union, pruning,
binding defaults, clip bounds and typed sampling execute original code.
"""
import argparse,hashlib,json,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
sys.path.insert(0,str(Path(__file__).resolve().parent))
from compiled_transforms_differential import Cpu,words,word,block,sha,nodes,relocate,database,u32,i32
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True)
 p.add_argument('--inputs',type=Path,default=ROOT/'reference/compiled-transforms/original-corpus.bin');a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/dynamic-compiled-transforms/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 raw=a.inputs.read_bytes();offset=4;assert word(raw,0)==0x31535443
 def readblock():
  nonlocal offset
  n=word(raw,offset);offset+=4;b=raw[offset:offset+n];offset+=n;return b
 model=readblock();nc=word(raw,offset);offset+=4;raws=[];ids=[]
 for i in range(nc):ids.append(i32(word(raw,offset)));offset+=4;raws.append(readblock())
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});authored=nodes(model)
 modelroot=relocate(old,model,old.data+0x10000);modeldb=database(old,modelroot,old.data+0x1000);images=[];dbs=[]
 for ci,rawclip in enumerate(raws):
  address=old.data+0x400000+ci*0x10000;images.append(address);dbs.append(database(old,relocate(old,rawclip,address),old.data+0x2000+ci*0x400))
 old.uc.mem_write(0x9f7110,words([1]));table=old.invoke(0x670a60,[]);old.pointer(0x9f7570,table)
 dyn=old.data+0xc000;animator=dyn+0x200;scratch=old.data+0xe000;native_scratch=new.data+0xe000;factory=old.data+0x700000;factory_count=0;selected_data=0
 def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def services(uc,address,size,user):
  nonlocal factory_count
  if address==0x611ae0:
   channel=word(bytes(uc.mem_read(old.reg(0)+16,4)),0);t=word(bytes(uc.mem_read(channel+8,4)),0);assert t in (1,5,10)
   pointer=factory+factory_count*32;factory_count+=1;uc.mem_write(pointer,bytes(32));old.pointer(pointer,{1:0x978ef8,5:0x978ae8,10:0x979618}[t]);ret(pointer)
  elif address==0x65f364:ret(selected_data)
 old.uc.hook_add(UC_HOOK_CODE,services)
 ptrs=new.data+0x2000;sizes=ptrs+0x100;native_ids=sizes+0x100;selection=native_ids+0x100;native_model=new.data+0x10000;new.uc.mem_write(native_model,model)
 for ci,b in enumerate(raws):
  address=new.data+0x400000+ci*0x10000;new.uc.mem_write(address,b);new.pointer(ptrs+ci*8,address);new.uc.mem_write(sizes+ci*4,words([len(b)]));new.uc.mem_write(native_ids+ci*4,words([ids[ci]]))
 cases=[(1,-1,list(range(nc))),(1,-2,list(range(nc))),(0,-2,list(range(nc))),(0,7,[7]),(0,-1,[0,1,2,3,4]),(1,7,[8,7,1,0]),(1,-2,[1,0,4,3,2])]
 gold=words([0x31544344])+block(model)+words([nc])+b''.join(words([cid])+block(b) for cid,b in zip(ids,raws))+words([len(cases)])
 counts={'targets':0,'bindings':0,'samples':0,'retained':0,'libm':0};summaries=[]
 for case,(mismatch,default_index,order) in enumerate(cases):
  old.uc.mem_write(dyn,bytes(0x100));old.invoke(0x3648c4,[dyn]);old.invoke(0x62db98,[dyn,mismatch]);base=old.data+0x600000
  # Preallocated source vectors avoid allocator/ownership services while all
  # append, clear, prune, resize and CompileInternal instructions still run.
  for field,stride,capacity in ((0x18,4,256),(0x24,8,32),(0x30,12,4096),(0x40,4,32),(0x4c,4,32),(0x58,4,32),(0x74,16,256)):
   old.uc.mem_write(base,bytes(stride*capacity));old.pointer(dyn+field,base);old.pointer(dyn+field+4,base);old.pointer(dyn+field+8,base+stride*capacity);base+=0x20000
  for ci in order:old.invoke(0x62e8a8,[dyn,dbs[ci]])
  if default_index!=-1:old.invoke(0x62fc90,[dyn,modeldb if default_index==-2 else dbs[default_index]])
  old.invoke(0x62f61c,[dyn],budget=3000000)
  n=word(bytes(old.uc.mem_read(dyn+0x3c,4)),0);channels=word(bytes(old.uc.mem_read(dyn+0x74,4)),0);bindingbase=word(bytes(old.uc.mem_read(dyn+0x30,4)),0);extended=word(bytes(old.uc.mem_read(dyn+0x18,4)),0)
  # Each native compile case gets a fresh process image. The shared CPU's
  # allocator fixture does not recycle free() storage; resetting an entire
  # image avoids falsely attributing its artificial heap limit to production.
  new=Cpu(a.library,True,{'functions':[]});new.uc.mem_write(native_model,model)
  for index,b in enumerate(raws):
   address=new.data+0x400000+index*0x10000;new.uc.mem_write(address,b);new.pointer(ptrs+index*8,address);new.uc.mem_write(sizes+index*4,words([len(b)]));new.uc.mem_write(native_ids+index*4,words([ids[index]]))
  new.uc.mem_write(selection,words(order));fixture=new.invoke('dh2_dynamic_transform_create',[native_model,len(model),nc,ptrs,sizes,native_ids,selection,len(order),u32(default_index),mismatch],budget=30000000);assert fixture
  assert new.invoke('dh2_dynamic_transform_count',[fixture])==n,(case,n,new.invoke('dh2_dynamic_transform_count',[fixture]))
  gold+=words([mismatch,u32(default_index),len(order)])+words(order)+words([n]);targets=[]
  for ti in range(n):
   ch=channels+ti*16;uri=old.string(word(bytes(old.uc.mem_read(ch+4,4)),0)).decode();t=word(bytes(old.uc.mem_read(ch+8,4)),0);width=4 if t==5 else 3;node=next((i for i,(name,off) in enumerate(authored) if name==uri),0xffffffff)
   assert new.invoke('dh2_dynamic_transform_target',[fixture,ti,native_scratch,native_scratch+0x100,4096])==1
   assert bytes(new.uc.mem_read(native_scratch,12))==words([t,node,width]) and new.string(native_scratch+0x100).decode()==uri
   targets.append((uri,t,width));gold+=block(uri.encode())+words([t,node,width])
  allbindings=[]
  for clip,ci in enumerate(order):
   start=old.invoke(0x65f07c,[dyn,clip]);end=old.invoke(0x65f098,[dyn,clip]);assert new.invoke('dh2_dynamic_transform_clip',[fixture,clip,native_scratch])==1
   info=words([ids[ci],start,end]);assert bytes(new.uc.mem_read(native_scratch,12))==info,(case,ci,info.hex(),bytes(new.uc.mem_read(native_scratch,12)).hex());gold+=info;clipbindings=[]
   for ti,(_,t,width) in enumerate(targets):
    record=bytes(old.uc.mem_read(bindingbase+(clip*n+ti)*12,12));mode,dp,track=struct.unpack('<III',record);value=bytes(old.uc.mem_read(dp,width*4)) if dp else bytes(width*4);expected=words([mode,int(bool(dp))])+value+bytes(16-len(value))
    assert new.invoke('dh2_dynamic_transform_binding',[fixture,clip,ti,native_scratch])==1
    assert bytes(new.uc.mem_read(native_scratch,24))==expected,(case,ci,ti,expected.hex(),bytes(new.uc.mem_read(native_scratch,24)).hex())
    gold+=expected;clipbindings.append((mode,dp,track));counts['bindings']+=1
   allbindings.append(clipbindings)
  cursors=dyn+0x400;old.uc.mem_write(animator,bytes(0x100));old.pointer(animator+0x24,dyn);old.pointer(animator+0x40,cursors);records=[]
  for clip,ci in enumerate(order):
   old.uc.mem_write(animator+0x4c,words([clip*n,clip]));r=word(raws[ci],32);lib=word(raws[ci],r+48);segments=word(raws[ci],lib) if lib else 0;segmentbase=word(raws[ci],lib+4) if lib else 0
   ranges=[(i32(word(raws[ci],segmentbase+i*24)),i32(word(raws[ci],segmentbase+i*24+4))) for i in range(segments)]
   queries=sorted(set([0,1,33,199,401,799,1201]+[x+d for pair in ranges for x in pair for d in (-1,0,1)]))
   if ci==7:queries+= [75,100,175]
   for ti,(uri,t,width) in enumerate(targets):
    mode,dp,track=allbindings[clip][ti]
    if track:old.pointer(track+20,word(bytes(old.uc.mem_read(extended+ti*4,4)),0))
    for interpolate in (0,1):
     for ms in queries:
      segment=0
      while segment+1<len(ranges) and ms>=ranges[segment][1]:segment+=1
      if ranges:
       seg=segmentbase+segment*24;data_offset=word(raws[ci],seg+12 if word(raws[ci],seg+8)==0 else seg+20);selected_data=images[ci]+data_offset
       for index in range(word(raws[ci],data_offset)):
        slot=data_offset+8+index*8;old.pointer(images[ci]+slot,images[ci]+slot+i32(word(raws[ci],slot)))
      else:selected_data=0
      for prior in (0,1,7,-1,0x7fffffff):
       initial=words([0x80000000,0x7fc05678,0x7f800000,0x3f800000]);old.uc.mem_write(scratch,initial);old.uc.mem_write(cursors+ti*4,words([u32(prior)]));old.uc.mem_write(animator+12,words([0 if interpolate else 1]));old.trig=[]
       old.invoke(0x65f7b4,[animator,ti,u32(ms),scratch]);expected=bytes(old.uc.mem_read(scratch,16));key=word(bytes(old.uc.mem_read(cursors+ti*4,4)),0);trace=old.trig[:]
       new.uc.mem_write(native_scratch,initial);new.uc.mem_write(native_scratch+0x20,words([u32(prior)]));new.trig=[]
       assert new.invoke('dh2_dynamic_transform_sample',[fixture,clip,ti,u32(ms),native_scratch,4,native_scratch+0x20,interpolate])==1
       actual=bytes(new.uc.mem_read(native_scratch,16));actual_key=word(bytes(new.uc.mem_read(native_scratch+0x20,4)),0)
       assert expected==actual and key==actual_key,(case,ci,ti,uri,ms,prior,interpolate,expected.hex(),actual.hex(),key,actual_key)
       assert [x for x in trace if x[0]!=2]==[x for x in new.trig if x[0]!=2]
       records.append(words([clip,ti,u32(ms),interpolate,u32(prior)])+initial+words([key])+expected+words([len(trace)])+b''.join(words(x) for x in trace));counts['samples']+=1;counts['retained']+=expected==initial;counts['libm']+=len(trace)
  gold+=words([len(records)])+b''.join(records);new.invoke('dh2_dynamic_transform_destroy',[fixture]);counts['targets']+=n;summaries.append({'mismatch':mismatch,'default_library':default_index,'clip_order':order,'targets':n,'samples':len(records)});print(json.dumps({'case':case,**summaries[-1]}),flush=True)
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
 sources=[ROOT/'animation.hpp',ROOT/'animation.cpp',Path(__file__),ROOT/'tests/dynamic_compiled_transforms.cpp']
 sources += [REPO/path for path in ('port/engine-animation/events.cpp','port/engine-animation/event_track.cpp','port/engine-animation/events.hpp','port/scene-materials/scene.cpp','port/scene-materials/scene.hpp','port/asset-payloads/payloads.cpp','port/asset-payloads/payloads.hpp','port/engine-resources/resources.cpp','port/engine-resources/resources.hpp','port/engine-math/math.cpp','port/engine-math/math.hpp')]
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'manifest_sha256':sha(ROOT/'reference/dynamic-compiled-transforms/original-functions.json'),'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.reference_output),'input_corpus_sha256':sha(a.inputs),'cases':summaries,**counts,'mismatches':0,'source_sha256':{str(path.relative_to(REPO)).replace('\\','/'):sha(path) for path in sources},'factory_identity_calls':factory_count,'imports':old.import_calls,'elapsed_seconds':round(time.monotonic()-started,2),'scope':'Full node1/5/10 dynamic compiler, ordered caller registration, strict0/retain1 pruning, clip/default DB bindings and actual typed accessors. Factory type/identity, preallocated vector capacities and selected segment are explicit fixtures. No whole Prince registration schedule, resource cache mutation, generic unsupported channels or historical Bionic libm claim.'}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','imports','scope','cases')}))
if __name__=='__main__':main()
