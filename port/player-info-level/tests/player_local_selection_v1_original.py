"""Original local ordinal/slot callers with declared external query providers."""
from __future__ import annotations
import hashlib,json,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
from player_locality_v1_original import image,put,get,signed,ELF_SHA

MODULE=Path(__file__).resolve().parents[1]
MANIFEST=MODULE/'reference/player-local-selection-v1/original-functions.json'
ID=0x36e2cc;LOCAL=0x36e478;INTERNAL=0x36dfb0;ASSIGN=0x43aac4
MANAGER=0x10001000;FALLBACK=MANAGER+8;PLAYERS=[FALLBACK,0x10003000,0x10004000,0x10005000]
MATCH=0x10008000;NET=0x10018000;VECTOR=0x10019000;VECTOR2=0x1001a000
APP=0x1001d000;APP2=0x1001e000;SAVE=0x1001f000;STOP=0x30000000

def cases():
    base=[0,0,0,0,1,1,1,1,0,1,0,22,44,2,7,19,5,0,3,0,0]
    rows=[]
    def add(**changes):
        row=base.copy()
        for k,v in changes.items():row[int(k[1:])]=v
        rows.append(row)
    for mode in [0,1,2]:
        for ordinal in [-1,0,1,2,3,2147483647]:
            for flag in [0,1,7]:add(v0=mode,v1=ordinal,v2=flag)
        for local in [(0,0,0),(1,1,1),(7,0,3),(0,1,0)]:
            for ordinal in [0,1,2]:add(v0=mode,v1=ordinal,v7=local[0],v8=local[1],v9=local[2])
        for count in [0,1,2]:add(v0=mode,v18=count)
        add(v0=mode,v13=88);add(v0=mode,v13=-1)
        for online,game,room,initialized in [(1,0,1,1),(1,1,0,1),(1,1,1,0),(1,1,1,1),(7,1,1,1)]:
            for ordinal in [-1,0,1,2,3]:
                for flag in [0,1]:add(v0=mode,v1=ordinal,v2=flag,v3=online,v4=game,v5=room,v6=initialized)
        for mutation in [1,2,3]:
            for ordinal in [0,1]:add(v0=mode,v1=ordinal,v2=1,v3=1,v10=22 if mutation!=3 else 0,v17=mutation)
        for fresh in [1,2]:add(v0=mode,v3=1 if fresh==1 else 0,v10=22,v19=fresh)
    for slot in [-2,-1,0,17,2147483647]:add(v0=2,v16=slot,v20=1)
    # Standalone public internal-ID wrapper forwards the actual lookup flag.
    for internal in [-1,0,2,7,19,88]:
        for online in [0,1]:
            for flag in [0,1,7]:add(v0=3,v1=internal,v2=flag,v3=online)
    return rows

def execute(data,symbols,row,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
    keys=[2,7,19];trace=[];online_calls=0;internal_calls=0;id_value=-1;selected=0;last_writes=0;player_writes=0
    app_got=0x99828c;put(u,app_got,APP);put(u,APP+0x4c,SAVE);put(u,APP+0x40,MANAGER)
    put(u,APP2+0x4c,SAVE+0x100);put(u,APP2+0x40,MANAGER+0x100);put(u,SAVE+8,11)
    count=row[18];nodes=[p-0x18 for p in PLAYERS[1:]];header=MANAGER+0x690
    for index,p in enumerate(PLAYERS):
        put(u,p+0x664,-1);put(u,p+0x660,0 if index==0 else row[9+index])
        u.mem_write(p+0x66c,bytes([0 if index==0 else row[6+index]&255]))
        put(u,p+0x670,-1 if index==0 else row[12+index])
    # Valid source RB trees for each bounded tree size.
    for n in nodes:
        for off in [4,8,12]:put(u,n+off,0)
    if count==3:
        root=nodes[1];put(u,nodes[0]+4,root);put(u,nodes[2]+4,root)
        put(u,root+8,nodes[0]);put(u,root+12,nodes[2])
    elif count==2:
        root=nodes[0];put(u,root+12,nodes[1]);put(u,nodes[1]+4,root)
    elif count==1:root=nodes[0]
    else:root=0
    if root:put(u,root+4,header)
    put(u,header+4,root);put(u,header+8,nodes[0] if count else header)
    put(u,header+12,nodes[count-1] if count else header);put(u,MANAGER+0x6a0,count)
    for i,n in enumerate(nodes):put(u,n+16,keys[i])
    put(u,MANAGER+0x6b4,VECTOR);put(u,MANAGER+0x6b8,VECTOR+12)
    for i,value in enumerate(keys):put(u,VECTOR+i*4,value)
    for i,value in enumerate([7,19,2]):put(u,VECTOR2+i*4,value)
    matching_vtable=0x10009000;put(u,MATCH,matching_vtable);put(u,matching_vtable+0x64,0x805cc8)
    def ret(v):u.reg_write(UC_ARM_REG_R0,v&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def hook(machine,address,size,context):
        nonlocal online_calls,internal_calls,id_value,selected
        if address==STOP:machine.emu_stop();return
        external={0x7fd794,0x320e98,0x800f8c,0x805cc8,0x8100dc,0x8100e0,0x36dec4}
        if address not in external:
            assert any(a<=address<a+n for a,n in [(ID,428),(LOCAL,32),(INTERNAL,236),(ASSIGN,60)]),hex(address)
            words.add(address)
        if address==0x36e484:id_value=signed(machine.reg_read(UC_ARM_REG_R0))
        if address==INTERNAL:
            ident=signed(machine.reg_read(UC_ARM_REG_R1));flag=machine.reg_read(UC_ARM_REG_R2)
            trace.append([8,ident,flag]);internal_calls+=1
            if row[17] and internal_calls==1:
                if row[17]==1:put(machine,VECTOR,19)
                elif row[17]==2:put(machine,MANAGER+0x6b4,VECTOR2);put(machine,MANAGER+0x6b8,VECTOR2+12)
                else:put(machine,MANAGER+0x6b8,VECTOR)
        if address==0x43aaf0:
            selected=machine.reg_read(UC_ARM_REG_R0);trace.append([15,selected,0])
        if address==0x7fd794:
            online=row[3]
            if online_calls and row[19]:online=0 if row[19]==1 else 1
            online_calls+=1;trace.append([1,0,online]);machine.mem_write(0x1001c005,bytes([online&255]));ret(0x1001c000)
        elif address==0x320e98:trace.append([2,0,row[4]]);machine.mem_write(0x1001c124,bytes([row[4]&255]));ret(0x1001c100)
        elif address==0x800f8c:trace.append([3,0,MATCH]);ret(MATCH)
        elif address==0x805cc8:trace.append([4,MATCH,row[5]]);ret(row[5])
        elif address==0x8100dc:trace.append([5,0,NET]);ret(NET)
        elif address==0x8100e0:trace.append([6,NET,row[6]]);ret(row[6])
        elif address==0x36dec4:
            ident=signed(machine.reg_read(UC_ARM_REG_R1));flag=machine.reg_read(UC_ARM_REG_R2)
            trace.append([16,ident,flag]);ret(PLAYERS[keys.index(ident)+1] if ident in keys else FALLBACK)
    def read_hook(machine,access,address,size,value,context):
        if address==app_got:trace.append([12,0,get(machine,address)])
        elif address==APP+0x4c:trace.append([13,APP,get(machine,address)])
        elif address==APP+0x40:
            assert signed(get(machine,SAVE+8))==row[16],'First source store must precede manager field read'
            trace.append([14,APP,get(machine,address)])
        elif address==MANAGER+0x6b4:
            start=get(machine,address);trace.append([7,0,(get(machine,MANAGER+0x6b8)-start)//4])
        for p in PLAYERS:
            if address==p+0x66c:trace.append([9,p,machine.mem_read(address,1)[0]])
            if address==p+0x660:trace.append([10,p,get(machine,address)])
            if address==p+0x670:trace.append([11,p,signed(get(machine,address))])
    def write_hook(machine,access,address,size,value,context):
        nonlocal last_writes,player_writes
        if address==SAVE+8:
            last_writes+=1
            if row[20]:put(machine,app_got,APP2)
        if any(address==p+0x664 for p in PLAYERS):player_writes+=1
    u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_MEM_READ,read_hook);u.hook_add(UC_HOOK_MEM_WRITE,write_hook)
    mode=row[0];u.reg_write(UC_ARM_REG_R0,row[16]&0xffffffff if mode==2 else MANAGER)
    u.reg_write(UC_ARM_REG_R1,row[1]&0xffffffff);u.reg_write(UC_ARM_REG_R2,row[2]&0xffffffff)
    u.emu_start([ID,LOCAL,ASSIGN,INTERNAL][mode],STOP+4,count=20000)
    assert u.reg_read(UC_ARM_REG_PC)==STOP,hex(u.reg_read(UC_ARM_REG_PC))
    if mode==0:id_value=signed(u.reg_read(UC_ARM_REG_R0))
    elif mode==3:id_value=row[1]
    if mode in [1,3]:selected=u.reg_read(UC_ARM_REG_R0)
    return {'id':id_value,'player':selected,'last_slot':signed(get(u,SAVE+8)),
            'slots':[signed(get(u,p+0x664)) for p in PLAYERS],
            'last_writes':last_writes,'player_writes':player_writes,'trace':trace}

def capture(path):
    data,symbols,_=image(path);pins=json.loads(MANIFEST.read_text())
    for pin in pins['functions']:
        s=symbols[pin['original_symbol']];a=int(pin['elf_address'],0);n=pin['size']
        assert (s['st_value'],s['st_size'])==(a,n)
        assert hashlib.sha256(data[a:a+n]).hexdigest()==pin['sha256']
    rows=cases();words=set();results=[execute(data,symbols,r,words) for r in rows]
    return rows,results,{'validation':'PASS','original_sha256':ELF_SHA,'cases':len(rows),
                         'distinct_executed_words':len(words),'scope':pins['scope']}
