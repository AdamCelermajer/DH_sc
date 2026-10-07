import argparse,hashlib,json,struct,itertools,sys,random
from pathlib import Path
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
old=Cpu(a.engine,False,{'functions':[]});new=Cpu(a.library,True,{'functions':[]});o=old.data+0x1000;n=new.data+0x1000;out=new.data+0x2000;target=old.data+0x3000;rows=[];rng=random.Random(0x520528)
for fn,current,interaction in itertools.product((0x3ad22c,0x3ad23c,0x3ad244,0x3ad280,0x3ad290,0x3ad29c,0x3ad2c8),(-1,0,3,4,5,6,10,12,17),(-1,0,3,4,5,6,0x7fffffff,-0x80000000)):
 for flags,mask,interrupt,stopped in ((0,0,0,0),(0xffffffff,0xffffffff,255,255),(0x8000,0x20,1,2),(rng.getrandbits(32),rng.getrandbits(32),rng.randrange(256),rng.randrange(256))):
  old.uc.mem_write(o,bytes(0x600));old.pointer(o+0x520,flags);old.pointer(o+0x528,mask);old.pointer(o+0x544,interaction&0xffffffff);old.uc.mem_write(o+0x441,bytes((interrupt,stopped)));old.pointer(target,0x12345678);old.pointer(old.stack+0xd000,target)
  facts=struct.pack('<IIiBBBB',flags,mask,interaction,interrupt,stopped,0,0);new.uc.mem_write(n,facts);new.uc.mem_write(out,struct.pack('<II',0x12345678,0xabcdef01))
  value=old.invoke(fn,[o,0xc351,0x13572468,current,target]);got=new.invoke('dh2_character_state_owner_predicate',[out,fn,current,n]);result=bytes(new.uc.mem_read(out,8));expect=bytes(old.uc.mem_read(target,4))+struct.pack('<I',value)
  assert got==1 and result==expect,(hex(fn),current,interaction,result.hex(),expect.hex());rows.append(struct.pack('<Ii',fn,current)+facts+expect)
binary=b'SBP1'+struct.pack('<I',len(rows))+b''.join(rows);(a.output/'predicate-fixtures.bin').write_bytes(binary)
r=dict(validation='PASS',original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),source_sha256={str((R/x).relative_to(R.parents[1])).replace('\\','/'):sha(R/x) for x in ('character_state_owner.hpp','character_state_owner.cpp','tests/character_state_owner_predicates_differential.py')},reference_sha256=hashlib.sha256(binary).hexdigest(),comparisons=len(rows),mismatches=0,source_predicates=7,spawn_backend=False,original_instructions_executed=True,optimized_arm64_instructions_executed=True)
(a.output/'predicate-differential.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
