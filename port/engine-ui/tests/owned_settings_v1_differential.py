"""Original cache/manager/parser execution and O2 ARM64 pure-kernel proof.
Full owner gold uses real original private STL nodes; file-copy/empty scene/
TextManager/platform enum services are explicit. Pure ARM64 checks source row
and option parsing, not complete cross-ELF C++ STL ownership.
"""
import argparse,pathlib,sys,json,struct,random,hashlib,importlib.util
R=pathlib.Path(__file__).resolve().parents[3];D=R/'.local-inputs/hud-owned-settings-v1'
sys.path.insert(0,str(R/'port/game-data/tests'));sys.path.insert(0,str(R/'port/engine-resources/tests'))
from items_differential import Original
from cpu import Cpu
from unicorn import UC_HOOK_CODE
W=lambda *x:struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
def sbytes(c,p):
 out=bytearray()
 while c.uc.mem_read(p+len(out),1)!=b'\0':out.extend(c.uc.mem_read(p+len(out),1))
 return bytes(out)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=pathlib.Path,required=True);ap.add_argument('--report',type=pathlib.Path,required=True);a=ap.parse_args()
 class Native(Cpu):
  def external(self,uc,addr,n,u):
   if addr==self.callback+48:
    name=sbytes(self,self.reg(1));self.calls.append(name);self.put(0,self.values+4*keys.index(name) if name in keys else 0);uc.reg_write(self.pc,uc.reg_read(self.lr))
   else:super().external(uc,addr,n,u)
 old=Original(R/'.local-inputs/libDungeonHunter2.so',{'functions':[]});new=Native(a.library,True,{'functions':[]});rng=random.Random(20261004)
 old.manager=old.data+0x4000;old.nodes=[old.data+0x8000+128*i for i in range(3)];new.values=new.data+0x4000
 keys=[b'Language',b'VolumeFX',b'known'];old.calls=[];new.calls=[]
 def ohook(uc,addr,n,u):
  if addr==0x46d784:
   name=sbytes(old,old.reg(1));old.calls.append(name)
   old.returned(old.nodes[keys.index(name)] if name in keys else old.manager+0x10)
 def nhook(uc,addr,n,u):
  if addr==new.callback+48:
   name=sbytes(new,new.reg(1));new.calls.append(name);new.put(0,new.values+4*keys.index(name) if name in keys else 0);uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,ohook)
 parser=[];rows=[]
 for i in range(512):
  entries=[]
  for j in range(rng.randrange(12)):
   key=rng.choice(keys+[b'',b'unknown',b'Language\0ignored',b'x'*127,b'x'*128,b'x'*200]);entries.append((key,rng.getrandbits(32)))
  blob=W(len(entries))+b''.join(W(len(k))+k+W(v) for k,v in entries)+bytes(256)
  initial=[rng.getrandbits(32) for _ in keys];old.blob=blob;old.cursor=0;old.calls=[];new.calls=[]
  for p,v in zip(old.nodes,initial):old.uc.mem_write(p+0x2c,W(v))
  old.invoke(0x46d8f4,[old.stream,old.manager]);expected=[old.word(p+0x2c) for p in old.nodes]
  data=new.data+0x10000;span=new.data+0x1000;lookup=span+64
  new.uc.mem_write(data,blob);new.uc.mem_write(new.values,W(*initial));new.uc.mem_write(span,struct.pack('<QIIII',data,len(blob),0,0,0));new.uc.mem_write(lookup,struct.pack('<QQ',0xf123456789abcdef,new.callback+48))
  assert new.invoke('dh2_settings_v1_read_options',[span,lookup])==0
  size,cursor,recognized,res=struct.unpack('<4I',new.uc.mem_read(span+8,16));got=list(struct.unpack('<3I',new.uc.mem_read(new.values,12)))
  assert expected==got and old.cursor==cursor and old.calls==new.calls and recognized==sum(k in keys for k in old.calls),(i,old.cursor,cursor,old.calls,new.calls)
  parser.append(W(len(blob))+blob+W(*initial,cursor,recognized,*expected,len(old.calls))+b''.join(W(len(k))+k for k in old.calls))
 for i in range(272):
  blob=(R/'.local-inputs/design-settings/design_pyarray.bin').read_bytes()[244+28*i:272+28*i] if i<16 else W(*(rng.getrandbits(32) for _ in range(7)))
  old.blob=blob;old.cursor=0;out=old.data+0x6000;old.uc.mem_write(out,bytes(32));old.invoke(0x4f213c,[out,old.stream]);expected=bytes(4)+bytes(old.uc.mem_read(out+4,28))
  inp=new.data+0x11000;nout=new.data+0x12000;used=nout+64;new.uc.mem_write(inp,blob)
  assert new.invoke('dh2_game_option_v1_decode_record',[nout,used,inp,28])==0 and bytes(new.uc.mem_read(nout,32))==expected and bytes(new.uc.mem_read(used,4))==W(28)
  rows.append(blob+expected)
 owner=json.loads((D/'original-owner.json').read_text());ogold=[]
 for c in owner['cases']:
  o=c['output'];blob=bytes.fromhex(c['blob']);ogold.append(W(len(blob))+blob+W(*map(int,c['args']))+W(*o['options'])+bytes.fromhex(o['tutorials'])+W(o['loaded'],o['new'],o['hint'],o['orientation'],o['cursor'],len(o['calls']))+b''.join(W({'file_copy':1,'switch_pack':2,'platform':3}[k],v) for k,v in o['calls']))
 reference=R/'port/engine-ui/reference/owned-hud-settings-v1';reference.mkdir(parents=True,exist_ok=True)
 gold=b'HSV1'+W(len(ogold),len(parser),len(rows))+b''.join(ogold)+b''.join(parser)+b''.join(rows);(reference/'fixtures.bin').write_bytes(gold)
 sources=['game_option_table_v1','owned_hud_settings_v1','settings_native_files_v1']
 report={'validation':'PASS','original_sha256':owner['original_sha256'],'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(gold).hexdigest(),'source_sha256':{f'port/engine-ui/{n}.{e}':hashlib.sha256((R/f'port/engine-ui/{n}.{e}').read_bytes()).hexdigest() for n in sources for e in ('hpp','cpp')},'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'comparisons':len(parser)+len(rows),'original_full_owner_gold_cases':len(ogold),'parser_cases':len(parser),'record_cases':len(rows),'mismatches':0,'scope':__doc__,'services':'Original stream/storage and keyed map lookup; native callback context full 64-bit. All source key/value reads and stop logic execute original instructions.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
