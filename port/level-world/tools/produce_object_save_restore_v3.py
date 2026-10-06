from pathlib import Path
exec(Path(__file__).with_name('produce_level_savegame_backend_v2.py').read_text().split('records=[]')[0])
out=root/'port/level-world/reference/object-save-restore-v3';out.mkdir(exist_ok=True)
actor=cpu.data+0x30000;actorvt=cpu.data+0x35000;info=cpu.data+0x36000;group=cpu.data+0x33000;visual=cpu.data+0x34000;anchor=cpu.data+0x37000;anchorvt=cpu.data+0x38000
events=[];is_player=False;is_monster=False;debug_f2=debug_f3=False;can_respawn=False;get_position=cpu.data+0x39000;group_present=False;phase='serialize';blocked_state=3
def event(entry,a=0,b=0):events.append([entry,a,b])
def helper(uc,a,n,p):
 global cursor,readpos
 if a in [0x3a49f0,cpu.data+0xea28]:event(0x3a49f0);ret(int(is_player))
 elif a==0x3a3064:event(a);ret(int(is_monster))
 elif a==0x31f594:event(a);d=cpu.data+0x3a000;uc.mem_write(d+0xf2,bytes([debug_f2,debug_f3]));ret(d)
 elif a==0x3e0af8:event(a);ret()
 elif a in [0x4713d0,0x38ba74,0x3cfd7c,0x3cfde4,0x3d6cdc,0x3b4088]:event(a);ret()
 elif a==0x38b0f0:event(a,cpu.reg(1));uc.mem_write(actor+0x80,bytes([cpu.reg(1)&255]));ret()
 elif a==0x3e07a0:event(a,cpu.reg(1),cpu.reg(2));w(actor+0xff8+36*4,cpu.reg(2));ret()
 elif a==0x3a59ac:event(a,cpu.reg(1),cpu.reg(2));uc.mem_write(actor+0x1449,b'\0');ret()
 elif a in [0x3c1a00,0x3c1a64,0x3c1a74]:event(a,cpu.reg(1)if a!=0x3c1a64 else 0);w(info,{0x3c1a00:3,0x3c1a64:17,0x3c1a74:0}[a]);w(actor+0x51c,info);ret()
 elif a==0x3a5248:event(a);ret(int(can_respawn))
 elif a==0x3d6890:event(a,cpu.reg(1),cpu.reg(2));w(actor+0x408,0);ret()
 elif a==0x3d49c4:event(a);w(actor+0x40c,word(actor+0x408));ret()
 elif a==0x3935dc:event(a);ret(get_position)
 elif a==0x393db4:event(a,cpu.reg(2));uc.mem_write(actor+0x160,bytes(uc.mem_read(cpu.reg(1),12)));ret()
 elif a==0x3938a0:event(a);uc.mem_write(actor+0x16c,bytes(uc.mem_read(cpu.reg(1),12)));ret()
 elif a==cpu.data+0xea08:event(0x2e0);ret()
cpu.uc.hook_add(UC_HOOK_CODE,helper)
w(actorvt+0x28,cpu.data+0xea28);w(actorvt+0x40,0x38b0f0);w(cpu.data+0xea28,0xe12fff1e);w(anchorvt+8,cpu.data+0xea08);w(cpu.data+0xea08,0xe12fff1e);w(anchor,anchorvt)
def setup(i,player):
 global buffer,cursor,readpos,is_player,is_monster,events,heap,group_present
 heap=cpu.data+0x10000;events=[];buffer=bytearray();cursor=readpos=0;is_player=player;is_monster=not player;group_present=bool(i&1);uc.mem_write(actor,bytes(0x1800));w(actor,actorvt);w(actor+0x51c,0 if i==0 else info);w(info,[3,0,17,12][i%4]);uc.mem_write(actor+0x80,bytes([i+1]));uc.mem_write(actor+0x8a,b'\1');w(actor+0x270,42)
 for start in [0x8ec,0xff4]:w(actor+start,0x96bbc0 if i<4 else 0x12345678);uc.mem_write(actor+start+4,b''.join(struct.pack('<I',(j*17+i)&0xffffffff)for j in range(224)))
 for offset,base in [(0x160,1),(0x16c,4),(0x1450,7),(0x145c,10),(0x1468,13),(0x1474,16)]:uc.mem_write(actor+offset,struct.pack('<3f',base+i,base+i+1,base+i+2))
 uc.mem_write(actor+0x1449,bytes([i%2]));w(actor+0x3fc,group if group_present else 0);w(group+0x24,77);uc.mem_write(group+0x28,b'\7\10');w(stream,vt)
records=[]
for i in range(8):
 setup(i,bool(i&2));cpu.symbols['saveChar']=0x3a65c4;cpu.invoke('saveChar',[actor,stream]);records.append({'i':i,'player':is_player,'group':group_present,'bytes':bytes(buffer).hex(),'cursor':cursor,'events':events[:]})
(out/'character-save-gold.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'cases':records,'scope':'Whole Character Serialize plus GameObject/ObjectBase and raw original byte/int/Point3D/900-byte property writers and SM_GetState execute. Actual stream byte virtuals and IsPlayer query are declared fixtures; same original source fields including opaque vtable tags are observed.'},indent=2)+'\n')
lines=['#pragma once','struct CharacterSaveGoldV3 {unsigned index;bool player,group;const unsigned char* bytes;unsigned size;};']
for i,r in enumerate(records):lines.append('static const unsigned char character_save_%d[]={%s};'%(i,','.join(str(x)for x in bytes.fromhex(r['bytes']))))
lines.append('static const CharacterSaveGoldV3 character_save_gold_v3[]={');lines.extend('{%d,%s,%s,character_save_%d,%d},'%(r['i'],'true'if r['player']else'false','true'if r['group']else'false',i,len(bytes.fromhex(r['bytes'])))for i,r in enumerate(records));lines.append('};');(out/'character-save-gold.hpp').write_text('\n'.join(lines)+'\n');print('Original Character Serialize8 PASS')
loads=[]
for i in range(32):
 record=records[i%8];setup(i%8,record['player']);buffer=bytearray.fromhex(record['bytes']);cursor=readpos=0;debug_f2=bool(i&8);debug_f3=bool(i&16);can_respawn=bool(i&4);initial_dead=bool(i&1);uc.mem_write(actor+0x1449,bytes([initial_dead]));w(actor+0x2dc,0 if i%3==0 else visual);w(actor+0x2d8,visual if i%2 else 0);w(actor+0x2e0,anchor if i%3 else 0);w(actor+0x378,cpu.data+0x3b000);uc.mem_write(cpu.data+0x3b008,b'\11');uc.mem_write(actor+0xac,b'\1');uc.mem_write(actor+0xd0,b'\1');uc.mem_write(get_position,struct.pack('<3f',100,200,300));w(actor+0x408,123);w(actor+0x40c,456)
 cpu.symbols['loadChar']=0x3a6210
 try:cpu.invoke('loadChar',[actor,stream])
 except:print('load failed',i,hex(uc.reg_read(cpu.pc_reg)),events,readpos);raise
 slices=[(0x80,1),(0x8a,1),(0xac,1),(0xd0,1),(0x160,12),(0x16c,12),(0x1449,1),(0x1468,24),(0x8ec,900),(0xff4,900),(0x408,8)]
 result=b''.join(bytes(uc.mem_read(actor+off,n))for off,n in slices)+struct.pack('<I',word(info))+bytes(uc.mem_read(cpu.data+0x3b008,1));loads.append({'i':i,'record':i%8,'initial_dead':initial_dead,'debug_f2':debug_f2,'debug_f3':debug_f3,'respawn':can_respawn,'physical':i%3!=0,'visual':i%2!=0,'anchor':i%3!=0,'events':events[:],'output':result.hex(),'read_cursor':readpos})
(out/'character-load-gold.json').write_text(json.dumps({'cases':loads,'scope':'Whole Character Deserialize plus Base/GameObject byte/int/property/pose readers run. Buff clear, source debug/type/lifecycle/FSM/target/position/rotation/visual/anchor helpers are declared ordered fixtures; no full VM/world/physics restore acceptance.'},indent=2)+'\n')
lines=['#pragma once','struct CharacterLoadGoldV3 {unsigned index,record;bool initial_dead,f2,f3,respawn,physical,visual,anchor;const unsigned char* output;unsigned size,cursor;const char* events;};']
for i,r in enumerate(loads):lines.append('static const unsigned char character_load_%d[]={%s};'%(i,','.join(str(x)for x in bytes.fromhex(r['output']))))
lines.append('static const CharacterLoadGoldV3 character_load_gold_v3[]={')
for i,r in enumerate(loads):
 events_text=','.join(f'{a:x}:{b}:{c}'for a,b,c in r['events']);lines.append('{%d,%d,%s,character_load_%d,%d,%d,%s},'%(r['i'],r['record'],','.join('true'if r[k]else'false'for k in ['initial_dead','debug_f2','debug_f3','respawn','physical','visual','anchor']),i,len(bytes.fromhex(r['output'])),r['read_cursor'],json.dumps(events_text)))
lines.append('};');(out/'character-load-gold.hpp').write_text('\n'.join(lines)+'\n');print('Original Character Deserialize32 PASS')
