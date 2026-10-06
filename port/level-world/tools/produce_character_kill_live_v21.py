from pathlib import Path
exec(Path(__file__).with_name('produce_level_savegame_backend_v2.py').read_text().split('records=[]')[0])
from unicorn.arm_const import UC_ARM_REG_R4,UC_ARM_REG_R6,UC_ARM_REG_R8
out=root/'port/level-world/reference/character-kill-live-v21';out.mkdir(exist_ok=True)
actor=cpu.data+0x30000;uc.mem_write(actor,bytes([0xa5])*0x1800)
uc.reg_write(UC_ARM_REG_R4,actor);uc.reg_write(UC_ARM_REG_R6,0xffffffff);uc.reg_write(UC_ARM_REG_R8,0)
uc.reg_write(cpu.sp_reg,cpu.stack+0xe000)
for first,last in [(0x3aa350,0x3aa358),(0x3aa434,0x3aa43c),(0x3aa578,0x3aa580),(0x3aa588,0x3aa590)]:uc.emu_start(first,last,count=100)
fields={'template13ca':struct.unpack('<h',bytes(uc.mem_read(actor+0x13ca,2)))[0],'killer144c':word(actor+0x144c),'master14d4':word(actor+0x14d4),'suppress14e4':bytes(uc.mem_read(actor+0x14e4,1))[0]}
assert fields==dict(template13ca=-1,killer144c=0,master14d4=0,suppress14e4=0),fields
(out/'ctor-original.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'fields':fields,'scope':'Actual original CharacterC1 four field-store blocks execute with source-proven r8=0/r6=-1; no whole Character constructor or initialized-adoption reset claim.'},indent=2)+'\n')
ai=cpu.data+0x30000;header=ai+0x7c;result=cpu.data+0x38000;nodes=cpu.data+0x31000;alloc=cpu.data+0x3a000
def source_services(uc,address,n,p):
 global alloc
 if address in [0x310454,0x310570,0x5341ac]:size=cpu.reg(0);ret(alloc);alloc+=(size+15)&~15
 elif address==0x3d6eec:ret(alloc);alloc+=32 # actual temporary RB node allocator24
 elif address in [0x3d5d64,0x30e060]:ret() # destruction only after outputs captured
uc.hook_add(UC_HOOK_CODE,source_services)
rows=[]
for trial in range(64):
 n=trial%13;alloc=cpu.data+0x3a000;uc.mem_write(ai,bytes(0x1000));uc.mem_write(nodes,bytes(0x1000))
 w(header+4,nodes if n else 0);w(header+8,nodes if n else header);w(header+12,nodes+24*(n-1)if n else header);w(header+16,n)
 entries=[]
 for i in range(n):
  node=nodes+24*i;w(node+4,header if i==0 else node-24);w(node+8,0);w(node+12,node+24 if i+1<n else 0);w(node+16,100+i)
  value=float((i*7+trial)%5-2);bits=struct.unpack('<I',struct.pack('<f',value))[0];w(node+20,bits);entries.append([100+i,bits])
 index=(trial//13)-1;w(result,0xa5a5a5a5);w(result+4,0xa5a5a5a5)
 cpu.symbols['get_aggro_entry']=0x3d72d0;cpu.invoke('get_aggro_entry',[ai,index&0xffffffff,result,result+4])
 rows.append({'entries':entries,'index':index,'character':word(result),'threat':word(result+4)})
(out/'aggro-original.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'cases':rows,'scope':'Whole AI_GetAggroEntry3d72d0 including actual converse-map3d7208/unique float tree insertion source, outgoing key-order, duplicate threats and signed missing-index behavior. Arena allocation and post-result temporary map destruction are explicit fixtures.'},indent=2)+'\n')
lines=['#pragma once','struct KillAggroGoldV21 {unsigned count;int index;const unsigned* entries;unsigned character,threat;};']
for i,r in enumerate(rows):lines.append('static const unsigned kill_aggro_%d[]={%s};'%(i,','.join(str(x)for entry in r['entries']for x in entry)or'0'))
lines.append('static const KillAggroGoldV21 kill_aggro_gold_v21[]={');lines.extend('{%d,%d,kill_aggro_%d,%d,%d},'%(len(r['entries']),r['index'],i,r['character'],r['threat'])for i,r in enumerate(rows));lines.append('};');(out/'aggro-original.hpp').write_text('\n'.join(lines)+'\n')
print('Original Kill metadata C1 and whole AggroEntry64 PASS')
