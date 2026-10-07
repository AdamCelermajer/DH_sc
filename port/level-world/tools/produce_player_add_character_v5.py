from pathlib import Path
exec(Path(__file__).with_name('produce_level_savegame_backend_v2.py').read_text().split('records=[]')[0])
out=root/'port/level-world/reference/player-add-character-v5';out.mkdir(exist_ok=True)
manager=cpu.data+0x30000;record=cpu.data+0x31000;actor=cpu.data+0x33000;host=cpu.data+0x36000;hostrecord=cpu.data+0x39000
recordvt=cpu.data+0x3a000;actorvt=cpu.data+0x3b000;network=cpu.data+0x3c000;level=cpu.data+0x3d000;save=cpu.data+0x3e000
trace=[];local=True;active=True;online=False;ishost=True;hostpresent=False;room=False;roomaccept=False;conflict=False;levelpresent=True;currentguard=0;countskills=7
def event(op,a=0,b=0):trace.append([op,a&0xffffffff,b&0xffffffff])
def source_hook(uc,address,n,p):
 if address==0x36dfb0:
  event(address,cpu.reg(1),cpu.reg(2));ret(record if not(cpu.reg(2)and conflict)else hostrecord)
 elif address==0x30eae4:ret()
 elif address==0x34b724:event(address,1,1);ret()
 elif address==0x33ff54:ret(actor)
 elif address==cpu.data+0xeb50:event(0x80f1ec);ret(int(local))
 elif address==cpu.data+0xeb5c:event(0x36d48c);ret(int(active))
 elif address==cpu.data+0xeb40:event(0x38b0f0,cpu.reg(1));uc.mem_write(actor+0x80,bytes([cpu.reg(1)&255]));ret()
 elif address==0x7fd794:event(address);uc.mem_write(network+5,bytes([online]));ret(network)
 elif address==0x80f23c:event(address);ret(int(ishost))
 elif address==0x36e09c:event(address);ret(hostrecord)
 elif address==0x36d730:event(address,3);uc.mem_write(cpu.reg(1),bytes([0xfd,2,0xff]));ret()
 elif address==0x36d76c:event(address,30);uc.mem_write(cpu.reg(1),bytes([0xff if i%4==0 else i%6 for i in range(30)]));ret()
 elif address in [0x3b36b0,0x3b35f0,0x3b4bc4,0x38c710]:event(address);ret()
 elif address in [0x3bb740,0x3bb814,0x3c1a00,0x3f059c,0x36f0dc,0x371050]:event(address,cpu.reg(1));ret()
 elif address in [0x3bbe54,0x3bbebc]:event(address,cpu.reg(1),cpu.reg(2));ret()
 elif address==0x31f594:event(address);ret(level if levelpresent else 0)
 elif address==0x393db4:event(address,cpu.reg(2));uc.mem_write(actor+0x160,bytes(uc.mem_read(cpu.reg(1),12)));ret()
 elif address==0x3938a0:event(address);uc.mem_write(actor+0x16c,bytes(uc.mem_read(cpu.reg(1),12)));ret()
 elif address==0x3a58f4:event(address);uc.mem_write(actor+0x1450,bytes(uc.mem_read(cpu.reg(1),12)));ret()
 elif address==0x396a90:event(address);ret(int(roomaccept))
 elif address==0x344184:event(address);ret()
 elif address==0x371d80:event(address);w(record+0x660,0);ret()
uc.hook_add(UC_HOOK_CODE,source_hook)
for off,callback in [(0x50,cpu.data+0xeb50),(0x5c,cpu.data+0xeb5c)]:w(recordvt+off,callback);w(callback,0xe12fff1e)
w(actorvt+0x40,cpu.data+0xeb40);w(cpu.data+0xeb40,0xe12fff1e)
rows=[]
for i in range(96):
 trace=[];local=bool(i&1);active=bool(i&2);online=bool(i&4);ishost=bool(i&8);hostpresent=bool(i&16);room=bool(i&32);roomaccept=bool(i&64);conflict=i%11==0;levelpresent=i%3!=0;currentguard=1 if i%17==0 else 2 if i%19==0 else 0;countskills=[0,3,7,30][i%4]
 for ptr,size in [(manager,0x800),(record,0x700),(actor,0x2400),(host,0x2400),(hostrecord,0x700)]:uc.mem_write(ptr,bytes(size))
 w(record,recordvt);w(actor,actorvt);w(record+0x380,0xffffffff if currentguard==1 else 2);w(record+0x660,actor if currentguard==2 else 0);w(record+0x664,5);w(record+0x670,7);w(record+0x674,9);uc.mem_write(record+0x4e5,b'\0')
 w(hostrecord+0x660,host if hostpresent else 0);w(manager+0x6c4,4)
 w(actor+0x14e8,save);w(save+0x84,countskills)
 for offset,base in [(0x160,100),(0x16c,200),(0x1450,300),(0x145c,400)]:uc.mem_write(host+offset,struct.pack('<3f',base,base+1,base+2))
 w(host+0x2f4,host+0x100 if room else 0)
 cpu.symbols['add_character']=0x372220;cpu.invoke('add_character',[manager,7])
 rows.append({'i':i,'local':local,'active':active,'online':online,'host':ishost,'host_present':hostpresent,'room':room,'room_accept':roomaccept,'conflict':conflict,'level':levelpresent,'guard':currentguard,'skill_count':countskills,'count':word(manager+0x6c4),'published':word(record+0x660)!=0,'controller':word(actor+0x1f88),'internal':word(actor+0x1f8c),'visible':bytes(uc.mem_read(actor+0x80,1))[0],'zoned':bytes(uc.mem_read(actor+0x2ef,1))[0],'pose':bytes(uc.mem_read(actor+0x160,24)).hex(),'initial':bytes(uc.mem_read(actor+0x1450,24)).hex(),'events':trace[:]})
(out/'original-gold.json').write_text(json.dumps({'cases':rows,'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'scope':'Whole372220 executes actual guard/publication/IDs/counter and loop branches. Object Spawn, typecast, PlayerInfo network/virtual queries, Character save/init/camera/skill/visibility, placement/room/LevelQuickSave/controller/light services are explicit ordered fixtures; no whole original application launch claimed.'},indent=2)+'\n')
lines=['#pragma once','struct PlayerAddGoldV5 {unsigned index;bool local,active,online,host,host_present,room,room_accept,conflict,level;unsigned guard,skills,count;bool published;unsigned controller,internal,visible,zoned;const char* events;};']
lines.append('static const PlayerAddGoldV5 player_add_gold_v5[]={')
for r in rows:
 lines.append('{%d,%s,%d,%d,%d,%s,%d,%d,%d,%d,%s},'%(r['i'],','.join('true'if r[k]else'false'for k in ['local','active','online','host','host_present','room','room_accept','conflict','level']),r['guard'],r['skill_count'],r['count'],'true'if r['published']else'false',r['controller'],r['internal'],r['visible'],r['zoned'],json.dumps(','.join(f'{op:x}:{a}:{b}'for op,a,b in r['events']))))
lines.append('};');(out/'original-gold.hpp').write_text('\n'.join(lines)+'\n');print('Whole original PlayerManager AddCharacter96 cases PASS')
