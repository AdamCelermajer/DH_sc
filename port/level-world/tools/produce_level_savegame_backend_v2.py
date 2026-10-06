from pathlib import Path
import sys,json,struct,importlib.util,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('oracle',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
class Dependencies(dep.Dependencies):
 def call(self,cpu,name):
  if name=='strlen':ret(len(string(cpu.reg(0))));return
  if name=='strcmp':a,b=string(cpu.reg(0)),string(cpu.reg(1));ret(0 if a==b else 1 if a>b else 0xffffffff);return
  super().call(cpu,name)
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,Dependencies(),{'functions':[]});uc=cpu.uc;uc.mem_map(cpu.data+0x10000,0x40000)
def w(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
def word(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
def string(a):return bytes(uc.mem_read(a,2048)).split(b'\0')[0].decode()
def ret(r0=0,r1=0):cpu.write_reg(0,r0);cpu.write_reg(1,r1);uc.reg_write(cpu.pc_reg,uc.reg_read(cpu.lr_reg))
owner=cpu.data+0x1000;stream=cpu.data+0x2000;vt=cpu.data+0x3000;nodes=[cpu.data+0x4000,cpu.data+0x5000];heap=cpu.data+0x10000
buffer=bytearray();cursor=0;readpos=0;jobs=[];recached=b'';body=b'';object_calls=[]
phase='saveAll';network=cpu.data+0xd000;obj=cpu.data+0x7000;character=False;player=False;local=True
load_missing=False;load_consume=0;lookup=[];load_calls=0;player_info=cpu.data+0x1f000
callbacks={0x8:'size',0x18:'read',0x1c:'write',0x20:'seek',0x24:'tell',0x2c:'seekwrite',0x30:'tellwrite'}
for off,name in callbacks.items():a=cpu.data+0xf000+off;w(a,0xe12fff1e);w(vt+off,a)
def write(data):
 global cursor,buffer
 if cursor+len(data)>len(buffer):buffer.extend(bytes(cursor+len(data)-len(buffer)))
 buffer[cursor:cursor+len(data)]=data;cursor+=len(data)
def hook(uc,a,n,p):
 global heap,cursor,readpos,recached,buffer,load_calls
 if a in [0x3136b4,0x3136b8,0x3139ac,0x313c90]:ret()
 elif a==0x31167c:
  if cpu.reg(1)>16:p=heap;heap+=2048;w(cpu.reg(0)+16,p);w(cpu.reg(0)+20,p)
  ret()
 elif a==0x310454:ret(stream)
 elif a==0x316d3c:buffer=bytearray();cursor=readpos=0;w(stream,vt);ret()
 elif a==0x3116e8:ret()
 elif a==0x315110:jobs.append({'backup':bool(bytes(uc.mem_read(cpu.reg(0)+28,1))[0]),'bytes':bytes(buffer)if word(cpu.reg(0))else b''});ret()
 elif a==0x315ad0:recached=bytes(buffer);ret()
 elif a==0x3109e0:
  out,start,end=[cpu.reg(i)for i in range(3)];data=bytes(uc.mem_read(start,end-start))+b'\0';p=heap;heap+=2048;uc.mem_write(p,data);w(out+16,p+end-start);w(out+20,p);ret()
 elif a==0x461ca8:
  out,length=cpu.reg(0),cpu.reg(1);p=heap;heap+=2048;w(out+16,p+length);w(out+20,p);ret()
 elif a==0x34aca0:lookup.append(['name',string(cpu.reg(2)),cpu.reg(3),word(uc.reg_read(cpu.sp_reg)),word(uc.reg_read(cpu.sp_reg)+4)]);ret(cpu.reg(0))
 elif a==0x33fdc0:ret(0 if load_missing else obj)
 elif a==0x36e478:lookup.append(['player',cpu.reg(1),cpu.reg(2)]);w(player_info+0x660,0 if load_missing else obj);ret(player_info)
 elif a==0x7fd794:ret(network)
 elif a==0x36effc:object_calls.append('local');ret(int(local))
 elif a==cpu.data+0xed04:object_calls.append('character');ret(int(character))
 elif a==cpu.data+0xed08:object_calls.append('player');ret(int(player))
 elif a==cpu.data+0xee00:write(body);ret()
 elif a==cpu.data+0xee04:load_calls+=1;readpos+=load_consume;ret()
 elif cpu.data+0xf000<=a<cpu.data+0xf100:
  action=callbacks[a-cpu.data-0xf000]
  if action=='size':ret(len(buffer))
  elif action=='write':size=cpu.reg(2)|(cpu.reg(3)<<32);write(bytes(uc.mem_read(cpu.reg(1),size)));ret(size)
  elif action=='read':size=cpu.reg(2)|(cpu.reg(3)<<32);uc.mem_write(cpu.reg(1),bytes(buffer[readpos:readpos+size]));readpos+=size;ret(size)
  elif action=='tellwrite':ret(cursor)
  elif action=='tell':ret(readpos)
  elif action=='seekwrite':cursor=min(cpu.reg(2)|(cpu.reg(3)<<32),len(buffer));ret()
  elif action=='seek':readpos=min(cpu.reg(2)|(cpu.reg(3)<<32),len(buffer));ret()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
body_callback=cpu.data+0xee00;w(body_callback,0xe12fff1e)
records=[]
for i,size in enumerate([0,1,4,17,2047,2048,2049,4097]):
 uc.mem_write(owner,bytes(64));uc.mem_write(nodes[0],bytes(128));uc.mem_write(nodes[1],bytes(128));jobs=[];recached=b'';body=bytes((j*31+i)%256 for j in range(size))
 header=owner+32;w(owner+20,cpu.data+0xc004);w(owner+24,cpu.data+0xc000);uc.mem_write(cpu.data+0xc000,b'test\0');w(owner+40,nodes[0]);w(owner+48,2)
 for j,name in enumerate(['INFO','OBJS']):
  node=nodes[j];tag=cpu.data+0xa000+j*32;uc.mem_write(tag,name.encode()+b'\0');w(node+32,tag+4);w(node+36,tag);w(node+4,header if j==0 else nodes[0]);w(node+56,body_callback);w(node+60,owner)
 w(nodes[0]+12,nodes[1]);w(nodes[1]+8,0);w(nodes[1]+12,0)
 # INFO and OBJS fixture writers intentionally share exact supplied bytes.
 cpu.symbols['saveAll']=0x315fb8;cpu.invoke('saveAll',[owner]);assert jobs[0]['backup']and not jobs[1]['backup'];assert recached==jobs[1]['bytes']
 records.append({'size':size,'bytes':recached.hex()})
out=root/'port/level-world/reference/level-savegame-v2';out.mkdir(exist_ok=True)
(out/'save-all-gold.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'cases':records,'scope':'Whole original Savegame.saveAll315fb8 executes source directory order/framing/cursor patch and backup-before-write enqueue. StreamBuffer byte virtuals, mutex, allocation, nested cache and job transport plus registered section writer bodies are declared fixtures. No file delivery accepted.'},indent=2)+'\n')
lines=['#pragma once','struct SaveAllGoldenV2{unsigned size;const unsigned char* bytes;unsigned count;};']
for i,r in enumerate(records):lines.append('static const unsigned char save_all_bytes_%d[]={%s};'%(i,','.join(str(x)for x in bytes.fromhex(r['bytes']))))
lines.append('static const SaveAllGoldenV2 save_all_gold_v2[]={');lines.extend('{%d,save_all_bytes_%d,%d},'%(r['size'],i,len(bytes.fromhex(r['bytes'])))for i,r in enumerate(records));lines.append('};');(out/'save-all-gold.hpp').write_text('\n'.join(lines)+'\n')
print('Original saveAll',len(records),'cases PASS')
appbase=(0x462d9c+8+word(0x462d8c+8+0x390))&0xffffffff;app=word(appbase+word(0x462d88+8+0x390));manager=cpu.data+0x8000;node=cpu.data+0x9000;objvt=cpu.data+0xe000
w(app+0x38,manager);w(objvt+0x10,body_callback)
for offset,address in [(0x24,cpu.data+0xed04),(0x28,cpu.data+0xed08)]:w(objvt+offset,address);w(address,0xe12fff1e)
objects_records=[]
for i in range(32):
 phase='objects';heap=cpu.data+0x10000;buffer=bytearray();cursor=readpos=0;object_calls=[];body=bytes((j*13+i)%256 for j in range((i%4)*17+1));character=bool(i&1);player=bool(i&2);local=bool(i&4);online=bool(i&8);disabled=bool(i&16)
 uc.mem_write(manager,bytes(128));uc.mem_write(node,bytes(128));uc.mem_write(obj,bytes(0x200));uc.mem_write(network,bytes(16));uc.mem_write(network+5,bytes([online]));w(manager+0x14,node);w(node+4,manager+0xc);w(node+0x2c,obj);w(obj,objvt);uc.mem_write(obj+0x28,b'\1');uc.mem_write(obj+0x81,bytes([disabled]));w(obj+0x64,7)
 gt=cpu.data+0xa000;name=cpu.data+0xb000;uc.mem_write(gt,b'Monster\0');uc.mem_write(name,b'Mob\0');w(obj+0x5c,gt);w(node+0x24,name+3);w(node+0x28,name)
 w(stream,vt);cpu.symbols['objects']=0x462d84;cpu.invoke('objects',[stream,owner]);objects_records.append({'character':character,'player':player,'local':local,'online':online,'disabled':disabled,'body':body.hex(),'bytes':bytes(buffer).hex(),'cursor':cursor,'calls':object_calls[:]})
(out/'save-objects-gold.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'cases':objects_records,'scope':'Whole __SaveObjects462d84 executes actual source map walk, network/player/disabled filters, CString framing, payload length and final cursor. Actual map node, receiver virtual type/Save, Network/PlayerManager and stream byte methods are declared fixtures.'},indent=2)+'\n')
lines=['#pragma once','struct SaveObjectsGoldenV2{bool character,player,local,online,disabled;const unsigned char* body;unsigned body_size;const unsigned char* bytes;unsigned byte_size,cursor;const char* calls;};']
for i,r in enumerate(objects_records):
 lines.append('static const unsigned char save_obj_body_%d[]={%s};'%(i,','.join(str(x)for x in bytes.fromhex(r['body']))))
 lines.append('static const unsigned char save_obj_bytes_%d[]={%s};'%(i,','.join(str(x)for x in bytes.fromhex(r['bytes']))))
lines.append('static const SaveObjectsGoldenV2 save_objects_gold_v2[]={')
for i,r in enumerate(objects_records):lines.append('{%s,save_obj_body_%d,%d,save_obj_bytes_%d,%d,%d,%s},'%(','.join('true'if r[x]else'false'for x in ['character','player','local','online','disabled']),i,len(bytes.fromhex(r['body'])),i,len(bytes.fromhex(r['bytes'])),r['cursor'],json.dumps(','.join(r['calls']))))
lines.append('};');(out/'save-objects-gold.hpp').write_text('\n'.join(lines)+'\n');print('Original SaveObjects32 cases PASS')
w(objvt+0x14,cpu.data+0xee04);w(cpu.data+0xee04,0xe12fff1e)
load_records=[]
for i in range(6):
 name='PlayerCharacter_0'if i%2 else'Mob';gametype='IgnoredSourceRole';load_missing=i==2;load_consume=[0,4,0,8,4,8][i];lookup=[];load_calls=0;heap=cpu.data+0x10000;readpos=0
 def saved_string(s):b=s.encode()+b'\0';return struct.pack('<I',len(b))+b
 buffer=bytearray(struct.pack('<I',1)+saved_string(gametype)+saved_string(name)+struct.pack('<IQ',7,8)+bytes(range(8)));w(stream,vt);uc.mem_write(owner+0x38,bytes([i==5]));cpu.symbols['loadObjects']=0x461e90;cpu.invoke('loadObjects',[stream,owner]);load_records.append({'name':name,'missing':load_missing,'consume':load_consume,'initializing':i==5,'bytes':bytes(buffer).hex(),'lookup':lookup[:],'loads':load_calls,'cursor':readpos})
(out/'load-objects-gold.json').write_text(json.dumps({'cases':load_records,'scope':'Whole __LoadObjects461e90 and actual source CString decoder run. Canonical/PlayerManager lookup and actual receiver Load body are declared fixtures; original pointer identities and unbounded whole-stream cursor policy retained.'},indent=2)+'\n')
lines=['#pragma once','struct LoadObjectsGoldenV2{const char* name;bool missing,initializing;unsigned consume,loads,cursor;const unsigned char* bytes;unsigned byte_size;};']
for i,r in enumerate(load_records):lines.append('static const unsigned char load_obj_bytes_%d[]={%s};'%(i,','.join(str(x)for x in bytes.fromhex(r['bytes']))))
lines.append('static const LoadObjectsGoldenV2 load_objects_gold_v2[]={')
for i,r in enumerate(load_records):lines.append('{%s,%s,%s,%d,%d,%d,load_obj_bytes_%d,%d},'%(json.dumps(r['name']),'true'if r['missing']else'false','true'if r['initializing']else'false',r['consume'],r['loads'],r['cursor'],i,len(bytes.fromhex(r['bytes']))))
lines.append('};');(out/'load-objects-gold.hpp').write_text('\n'.join(lines)+'\n');print('Original LoadObjects6 cases PASS')
