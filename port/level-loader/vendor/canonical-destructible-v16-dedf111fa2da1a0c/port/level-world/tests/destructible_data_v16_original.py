from pathlib import Path
import json,struct
r=Path(__file__).resolve().parents[3];s=(r/'port/level-world/tests/canonical_dummy_v14_original.py').read_text();s=s[:s.index('snapshots=[]')];exec(compile(s,'stream-primitives','exec'))
ref=r/'port/level-world/reference/destructible-container-v16';b=(ref/'destructible-group0-records.bin').read_bytes();at=4;actor=c.data+0xa00000;stream=c.data+0xa10000;script_heap=c.data+0xa20000
def stream_hook(uc,a,z,u):
 global at,script_heap
 if a in (0x459090,0x3df1a0):c.uc.mem_write(c.reg(1),b[at:at+4]);at+=4;ret()
 elif a==0x4db89c:c.uc.mem_write(c.reg(1),b[at:at+1]);at+=1;ret()
 elif a==0x31056c:p=script_heap;script_heap+=0x1000;ret(p)
 elif a==0x317454:n=c.reg(2);c.uc.mem_write(c.reg(1),b[at:at+n]);at+=n;ret(n)
c.uc.hook_add(UC_HOOK_CODE,stream_hook);rows=[];gold=bytearray(struct.pack('<II',0x36314444,37))
for i in range(37):
 c.uc.mem_write(actor,b'\0'*0x44);start=at;c.invoke(0x4fe220,[actor,stream]);raw=bytes(c.uc.mem_read(actor,0x44));size=word(actor+0x2c);script=bytes(c.uc.mem_read(word(actor+0x30),size));gold+=raw[4:0x20]+raw[0x20:0x21]+raw[0x24:0x30]+script+raw[0x34:0x44];rows.append({'index':i,'start':start,'end':at,'script_bytes':size})
assert at==len(b);(ref/'rows-original-v16.bin').write_bytes(gold);(ref/'rows-original-v16.json').write_text(json.dumps({'status':'PASS','rows':rows,'scope':'whole Structs::DestructibleContainer::read4fe220 over actual37 cached rows; source stream primitive reads and allocation explicit fixture'},indent=2));print('Original Destructible37 cache rows PASS',at,'bytes')
