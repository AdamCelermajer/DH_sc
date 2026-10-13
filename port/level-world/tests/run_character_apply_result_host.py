"""Bounded offline F_ApplyResult traversal with nonplayer defenders and Player attacker tail."""
from __future__ import annotations
import argparse,hashlib,json,math,os,random,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-apply-result/original-functions.json'
A,B,D,D2,APP,PLAYER_MANAGER,ONLINE,VFX,POINT,TABLE,OUT=range(0x10000000,0x100b0000,0x10000)
LEVEL,CAMERA,PLAYER_INFO,STAT_MANAGER,TROPHY_MANAGER=0x100b0000,0x100c0000,0x100d0000,0x100e0000,0x100f0000
NAMES=['online_mode','is_player','debug_load','string_construct','debug_query','string_destroy','saved_option','coop_player_count','effective_threat','add_aggro','hit_for','is_dead','blood_death_fx','blood_fx','target_position','play_anim_fx_set','regen_hp','regen_mp','apply_dot','dodge','block','hurt','push','read_property','stun','fear','slow','cancel_sneaking','combat_text','combat_sound','ai_combat_result','critical_camera','player_by_character','player_stat_increment','player_stat_increase','state_machine_is_idle','player_property_add_int','player_property_get_int','player_is_local','trophy_lookup','trophy_unlock','inventory_add_gold','is_monster']
OP={name:i for i,name in enumerate(NAMES)}
KEYS={'NoDamages':1,'GOD':2,'isTracingThreatChange':3,'isTracingChar_Attack':4,'epic_invisible':5,'epic_invincible':6,'knocked_down_50_times':7}
TROPHIES={'epic_invisible':35,'epic_invincible':34,'knocked_down_50_times':55}
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def signed(x):return x if x<0x80000000 else x-0x100000000
def floating(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def bits(x):
    try:return struct.unpack('<I',struct.pack('<f',x))[0]
    except OverflowError:return 0xff800000 if x<0 else 0x7f800000
def fixtures():
    base=[256,0,0x20080000,0xffffffff,0xffffffff,0,0,0xffffffff,0xffffffff,0,0,0,0,0,0,1,bits(2),0,0,0,768,0,10]
    result=[('normal_dot_domain',base.copy())]
    for amount in [0,1,256,0x7fffffff,0xffffffff]:
        for death in [0,1]:
            row=base.copy();row[0]=amount;row[9]=death;row[21]=1;result.append((f'self_dot_{amount}_{death}',row))
    for mutation in [2,3,4,5,7,13,14]:
        row=base.copy();row[21]=1;row[19]=mutation;result.append((f'self_freshness_{mutation}',row))
    for name,index,values in [('amount',0,[0,1,255,0xffffffff,0x80000000,0x7fffffff]),('outcomes',1,[1,2,4,8,16,32,64,128,256,511,0xffff0000]),('remote_update_word',8,[0,3,0x80000000]),('dead',9,[1,7,0x80000000]),('no_damage',10,[1,7,0x80000000]),('god',11,[1,7]),('saved_god',12,[1,7]),('invulnerable',13,[1,255]),('threat',16,[0,0x80000000,bits(-1),bits(0.00390625),0x00000001,0x7f7fffff]),('aggro_return',17,[0,0x80000000,bits(-1),bits(1),0x7f800000,0xff800000,0x7fc00123,0x7f800001]),('status_property',20,[0,1,255,256,0xffffffff,0x80000000,0x7fffffff])]:
        for value in values:
            row=base.copy();row[index]=value
            if index==20:row[1]=511
            result.append((f'{name}_{value}',row))
    for duration in [0,1,255,256,513,0xffffffff]:
        for amount in [0,1,0xffffffff]:
            row=base.copy();row[3:5]=duration,amount;result.append((f'dot_{duration}_{amount}',row))
    for dead in [0,1]:
        for element in [0xffffffff,0,4,0xffffff84,0x7fffffff]:
            row=base.copy();row[2]|=0x200000;row[7]=element;row[9]=dead;result.append((f'impact_{dead}_{element}',row))
    for mutation in range(1,15):
        row=base.copy();row[1]=511;row[2]|=0x200000;row[3:5]=513,1024;row[17]=bits(1);row[19]=mutation;result.append((f'freshness_{mutation}',row))
    for name,index,value in [('network_boundary',14,1),('coop_boundary',15,2),('invalid_threat_boundary',16,0x7fc00123),('defender_player_final',18,202),('attacker_player_final',18,101)]:
        row=base.copy();row[index]=value
        if name=='attacker_player_final':row[9]=1
        result.append((name,row))
    row=base.copy();row[15]=3;row[18]=101;row[0]=512;result.append(('coop_player_scale',row))
    row=base.copy();row[15]=3;row[19]=17;row[0]=256;result.append(('coop_monster_scale',row))
    row=base.copy();row[15]=3;row[0]=256;result.append(('coop_unknown_scale',row))
    row=base.copy();row[11]=1;row[18]=202;result.append(('god_player_boundary',row))
    row=base.copy();row[1]=8;row[18]=101;result.append(('critical_player_boundary',row))
    row=base.copy();row[18]=202;result.append(('living_player_boundary',row))
    row=base.copy();row[0]=0;row[1]=0;row[18]=202;row[19]=15;row[20]=499;result.append(('player_idle_dodge_trophy',row))
    row=base.copy();row[0]=0;row[1]=128;row[18]=202;row[19]=16;row[20]=49;result.append(('player_idle_push_local_false',row))
    for name,amount,gold in [('gold_debit_capped_by_balance',1024,3),('gold_debit_limited_by_attack',512,10),('gold_zero_balance',512,0),('gold_negative_attack',0xffffff00,10)]:
        row=base.copy();row[0]=amount;row[2]|=0x400000;row[22]=gold;result.append((name,row))
    rng=random.Random(0x3b10b4)
    for i in range(64):
        row=base.copy();row[0]=rng.choice([0,1,256,0xffffffff,rng.getrandbits(31)]);row[1]=rng.randrange(512);row[2]=rng.getrandbits(32)&~0x400000;row[3:7]=[rng.choice([0,1,255,256,0xffffffff]) for _ in range(4)];row[7]=rng.choice([0xffffffff,4]);row[8]=rng.choice([0xffffffff,23]);row[9:13]=[rng.randrange(2) for _ in range(4)];row[16]=bits(rng.randrange(-2000,2001)*.00390625);row[17]=rng.choice([bits(1),0x7fc00123,0x80000000]);row[20]=rng.getrandbits(32);result.append((f'random_{i}',row))
    return result

def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    from elf_import_identity import verify_imports
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            seg=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(seg['p_offset'])+at-int(seg['p_vaddr']);return raw[off:off+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert (s['st_value'],s['st_size'])==(at,n);assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['literals']:
            value=row['text'].encode()+b'\0';assert data(int(row['elf_address'],0),len(value))==value;assert hashlib.sha256(value).hexdigest()==row['sha256']
        for row in manifest['vtables']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert (s['st_value'],s['st_size'])==(at,n);assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
            for slot in row['slots']:assert struct.unpack('<I',data(at+row['address_point_offset']+int(slot['offset'],0),4))[0]==int(slot['target'],0)
        for row in manifest['mapping_ranges']:
            b=data(int(row['elf_address'],0),row['size']);assert b.hex()==row['bytes'] and hashlib.sha256(b).hexdigest()==row['sha256']
    imports=verify_imports(original,{0x30e964:'__aeabi_i2f',0x30ed6c:'__aeabi_fmul',0x30e2f8:'__aeabi_fcmpgt',0x30eba4:'__aeabi_fadd',0x30e4cc:'__aeabi_f2iz',0x30e2a4:'__aeabi_idiv'})
    class ArithmeticCpu(Cpu):
        def external(self,uc,address,size,unused):
            if address==self.callback:return # named AI callback observer below
            name=self.imports.get(address)
            if name in ('__aeabi_i2f','__aeabi_fmul','__aeabi_fcmpgt','__aeabi_fadd','__aeabi_f2iz','__aeabi_idiv'):
                self.import_calls[name]=self.import_calls.get(name,0)+1
                if name=='__aeabi_i2f':self.put(0,bits(signed(self.reg(0))))
                elif name=='__aeabi_fmul':self.put(0,bits(floating(self.reg(0))*floating(self.reg(1))))
                elif name=='__aeabi_fadd':self.put(0,bits(floating(self.reg(0))+floating(self.reg(1))))
                elif name=='__aeabi_f2iz':self.put(0,signed(int(floating(self.reg(0)))))
                elif name=='__aeabi_idiv':
                    lhs,rhs=signed(self.reg(0)),signed(self.reg(1));self.put(0,(abs(lhs)//abs(rhs))*(-1 if (lhs<0)!=(rhs<0) else 1))
                else:self.put(0,int(floating(self.reg(0))>floating(self.reg(1))))
                uc.reg_write(self.pc,uc.reg_read(self.lr))
            else:super().external(uc,address,size,unused)
    old=ArithmeticCpu(original,False,manifest);old.uc.mem_map(A,0x100000)
    def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def byte(at):return old.uc.mem_read(at,1)[0]
    def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def string(at):return bytes(old.uc.mem_read(at,128)).split(b'\0')[0].decode('ascii')
    got=(0x3b10c4+8+word(0x3b1d74))&0xffffffff
    debug_slot=got+word(0x3b1d7c);app_slot=got+word(0x3b1d90);fx_slot=got+word(0x3b1da4)
    assert got==0x994a98
    providers={0x7fd794:OP['online_mode'],0x3a49f0:OP['is_player'],0x337888:OP['debug_load'],0x3140ec:OP['string_construct'],0x337a88:OP['debug_query'],0x3139ac:OP['string_destroy'],0x320e14:OP['saved_option'],0x3bd394:OP['effective_threat'],0x3d7c68:OP['add_aggro'],0x3a8bc4:OP['hit_for'],0x3a2ed4:OP['is_dead'],0x3a3368:OP['blood_death_fx'],0x3a33d0:OP['blood_fx'],0x3935dc:OP['target_position'],0x495888:OP['play_anim_fx_set'],0x3bdca4:OP['regen_hp'],0x3bdbb8:OP['regen_mp'],0x3e2720:OP['apply_dot'],0x3c5b3c:OP['dodge'],0x3c5c60:OP['block'],0x3c5d84:OP['hurt'],0x3c5ea0:OP['push'],0x3dedb4:OP['read_property'],0x3c5ffc:OP['stun'],0x3c6144:OP['fear'],0x3e2a5c:OP['slow'],0x3bc6b8:OP['cancel_sneaking'],0x3af77c:OP['combat_text'],0x3afee0:OP['combat_sound'],0x36eea8:OP['player_by_character'],0x3790ec:OP['player_stat_increment'],0x3790e0:OP['player_stat_increase'],0x31f594:OP['critical_camera'],0x40f980:-1,0x3c0260:OP['state_machine_is_idle'],0x3e0798:OP['player_property_add_int'],0x3df6e0:OP['player_property_get_int'],0x36effc:OP['player_is_local'],0x3a3f70:OP['trophy_lookup'],0x3813b8:OP['trophy_unlock'],0x3fe164:OP['inventory_add_gold'],0x3a3064:OP['is_monster'],old.callback:OP['ai_combat_result']}
    instructions=set();records=[];boundary_counts={}
    for name,inputs in fixtures():
        ATT=B if inputs[21] else A
        old.uc.mem_write(A,bytes(0x100000));old.uc.mem_write(old.stack,bytes(0x10000));old.pointer(debug_slot,D);old.pointer(app_slot,APP);old.pointer(fx_slot,VFX);old.pointer(APP+0x40,PLAYER_MANAGER);old.pointer(0x99f76c,PLAYER_MANAGER);old.pointer(PLAYER_MANAGER+0x6c4,inputs[15]);old.pointer(0x99fa88,STAT_MANAGER);old.pointer(0x9a2418,TROPHY_MANAGER);old.pointer(0x9a6498,0x10080000);old.pointer(0x10080000+52,bits(.5));old.pointer(0x10080000+56,bits(.5));old.pointer(LEVEL+296,CAMERA);old.pointer(PLAYER_INFO+1648,73);old.uc.mem_write(ONLINE+5,bytes([inputs[14]]));
        old.pointer(TABLE+0x28,0x3a49f0);old.pointer(TABLE+0x34,0x3a2ed4);old.pointer(TABLE+0xb4,old.callback)
        for actor in (A,B):old.pointer(actor,TABLE);old.pointer(actor+0x3c8,TABLE);old.uc.mem_write(actor+0x14d0,struct.pack('<H',65535 if actor==A else 100));old.pointer(actor+0x3c8+56,4 if actor==ATT and inputs[19]==17 else 0)
        old.pointer(A+0x110,0xffffffff);old.pointer(B+0x110,inputs[8]);old.uc.mem_write(B+0x14f0,bytes([inputs[13]]));old.uc.mem_write(B+0x53b,b'\x01');old.pointer(ATT+0x380,ATT);old.pointer(ATT+0x39c,inputs[22]);old.pointer(ATT+0x3a4,0x7fffffff)
        result=[inputs[0],3,inputs[3],inputs[4],inputs[5],inputs[6],inputs[1],inputs[2],0xffffffff,inputs[7]];old.uc.mem_write(OUT,struct.pack('<10I',*result))
        trace=[];strings={};phase=[0];dead=[inputs[9]];captured=[0];status=[0];unsupported=[0];threat=[0];last_player=[0];player_idle=[int(inputs[19]>=15)];player_local=[int(inputs[19]!=16)];player_props={215:inputs[20],214:inputs[20],222:inputs[20]}
        def normalized(identity):
            return {0:0,A:101,B:202,D:11,D2:22,APP:77,VFX:88,POINT:777,VFX+0x100:99,PLAYER_MANAGER:77,STAT_MANAGER:77,PLAYER_INFO:73,LEVEL:123,CAMERA:124,TROPHY_MANAGER:125}.get(identity,strings.get(identity,(identity,0))[0])
        def record(op,subject=0,peer=0,identity=0,w0=0,w1=0,w2=0,w3=0,key=0):trace.append([op,normalized(subject),normalized(peer),normalized(identity),w0,w1,w2,w3,key])
        class Boundary(Exception):pass
        def stop(s,pc):status[0]=s;unsupported[0]=pc;raise Boundary()
        def observe(_,at,__,___):
            if at==0x3b1440:stop(4,at)
            if at==0x3b15cc and (old.reg(0)&0x7f800000)==0x7f800000:stop(8,at)
            if at==0x3b155c:captured[0]=old.reg(8)
            if at==0x3a8bc4:captured[0]=old.reg(1)
            if at==0x3b1578:record(OP['coop_player_count'],w0=0)
            if at not in providers:
                if 0x3b10b4<=at<0x3b1d74:instructions.add(at)
                return
            op=providers[at];r=[old.reg(i) for i in range(4)];mutation=inputs[19]
            if op==OP['online_mode']:record(op);returned(ONLINE)
            elif op==OP['is_player']:record(op,r[0]);last_player[0]=7 if normalized(r[0])==inputs[18] else 0;returned(last_player[0])
            elif op==OP['is_monster']:record(op,r[0]);returned(int(word(r[0]+0x3c8+56)==4))
            elif op==OP['critical_camera']:record(op,A);returned(LEVEL)
            elif op==OP['player_by_character']:assert r[0]==PLAYER_MANAGER and r[2]==0;record(op,APP,r[1],w0=r[2]);returned(PLAYER_INFO)
            elif op==OP['player_stat_increment']:record(op,APP,w0=r[1],w1=r[2]);returned()
            elif op==OP['player_stat_increase']:record(op,APP,w0=r[1],w1=r[2],w2=r[3]);returned()
            elif at==0x40f980:returned(0)
            elif op==OP['state_machine_is_idle']:assert r[0]==B+0x4fc and r[1]==0;record(op,B,w0=r[1]);returned(player_idle[0])
            elif op==OP['player_property_add_int']:
                assert r[0]==B+0x560;record(op,B,w0=r[1],w1=r[2]);player_props[r[1]]=player_props.get(r[1],0)+signed(r[2]);returned()
            elif op==OP['player_property_get_int']:assert r[0]==B+0x560 and r[2]==0;record(op,B,w0=r[1],w1=r[2]);returned(player_props.get(r[1],0))
            elif op==OP['player_is_local']:assert r[0]==PLAYER_MANAGER;record(op,APP,r[1]);returned(player_local[0])
            elif op==OP['trophy_lookup']:
                trophy_name=string(r[0]);record(op,key=KEYS[trophy_name]);returned(TROPHIES[trophy_name])
            elif op==OP['trophy_unlock']:assert r[0]==TROPHY_MANAGER;record(op,APP,w0=r[1]);returned()
            elif op==OP['inventory_add_gold']:
                assert r[0]==ATT+0x37c;record(op,ATT,w0=r[1]);delta=signed(r[1]);gold_now=signed(word(ATT+0x39c));limit=signed(word(ATT+0x3a4))
                if delta<0:
                    if gold_now< -delta:delta=-gold_now
                elif delta>0:
                    room=limit-gold_now
                    if delta>room:delta=room if room>=0 else 0
                new_gold=gold_now+delta;new_gold=min(new_gold,limit);old.pointer(ATT+0x39c,new_gold);returned()
            elif op==OP['debug_load']:
                record(op,r[0]);phase[0]+=1
                if mutation==1 and phase[0]==1:old.pointer(debug_slot,D2)
                returned()
            elif op==OP['string_construct']:
                key=KEYS[string(r[1])];record(op,key=key);strings[r[0]]=(len([t for t in trace if t[0]==op]),key);returned(r[0])
            elif op==OP['debug_query']:
                key=strings[r[1]][1];record(op,r[0],identity=r[1],key=key);returned(inputs[10] if key==1 else inputs[11] if key==2 else 0)
            elif op==OP['string_destroy']:record(op,r[0]);returned()
            elif op==OP['saved_option']:assert string(r[1])=='GOD';record(op,r[0],key=2);returned(inputs[12])
            elif op==OP['effective_threat']:
                record(op,r[0]);
                if mutation==2:old.pointer(OUT,512)
                returned(inputs[16])
            elif op==OP['add_aggro']:
                record(op,r[0]-0x3c8,r[1],w0=r[2]);threat[0]=r[2]
                if mutation in (3,4):old.pointer(OUT+24,128);old.pointer(OUT+28,word(OUT+28)|0x100000)
                if mutation==3:old.pointer(B+0x110,99)
                returned(inputs[17])
            elif op==OP['hit_for']:
                record(op,r[0],r[2],w0=r[1]);
                if mutation==5:dead[0]=7
                returned()
            elif op==OP['is_dead']:record(op,r[0]);returned(dead[0])
            elif op in (OP['blood_death_fx'],OP['blood_fx']):record(op,r[0]);returned(51 if op==OP['blood_death_fx'] else 52)
            elif op==OP['target_position']:
                record(op,r[0]);
                if mutation==6:old.pointer(fx_slot,VFX+0x100)
                returned(POINT)
            elif op==OP['play_anim_fx_set']:
                assert r[3]==B+0x16c and word(old.uc.reg_read(old.sp))==word(old.uc.reg_read(old.sp)+4)==0
                record(op,r[0],B,r[2],r[1]);returned()
            elif op in (OP['regen_hp'],OP['regen_mp']):
                record(op,r[0],w0=r[1])
                if mutation==7 and op==OP['regen_hp']:old.pointer(OUT+20,513);dead[0]=0
                if mutation==8 and op==OP['regen_mp']:old.pointer(OUT+8,1025);old.pointer(OUT+12,2048);old.pointer(OUT+4,4)
                returned()
            elif op==OP['apply_dot']:
                record(op,r[0]-0x560,w0=r[1],w1=r[2],w2=r[3])
                if mutation==9:old.pointer(OUT+28,word(OUT+28)|0x18000000)
                returned()
            elif op in (OP['dodge'],OP['block'],OP['hurt']):
                record(op,r[0]-0x4fc,r[1],w0=r[2])
                if mutation==10 and op==OP['dodge']:old.pointer(OUT+24,16)
                if mutation==11 and op==OP['hurt']:old.pointer(OUT+24,128|64|32|256)
                returned()
            elif op==OP['push']:record(op,r[0]-0x4fc,r[2],w0=r[1],w2=r[3]);returned()
            elif op==OP['read_property']:
                assert r[1]==ATT+0xff4;record(op,r[0]-0x560,w0=r[2]);value=inputs[20] if r[2] in (140,143,146) else inputs[20]^0x80000100
                if mutation==12 and r[2]==140:old.pointer(OUT+28,word(OUT+28)|0x14000)
                returned(value)
            elif op in (OP['stun'],OP['fear']):record(op,r[0]-0x4fc,r[3],w0=r[1],w1=r[2],w2=word(old.uc.reg_read(old.sp)));returned()
            elif op==OP['slow']:record(op,r[0]-0x560,w0=r[1]);returned()
            elif op==OP['cancel_sneaking']:
                record(op,r[0]);
                if mutation==13:old.pointer(OUT+28,word(OUT+28)&~0x20000000)
                returned()
            elif op==OP['combat_text']:
                assert r[:3]==[OUT,ATT,B];record(op)
                if mutation==14:old.pointer(OUT+28,word(OUT+28)|0x20000000)
                returned()
            elif op==OP['combat_sound']:assert r[:3]==[OUT,ATT,B];record(op);returned()
            elif op==OP['ai_combat_result']:assert r[1:]==[ATT,B,OUT];record(op,r[0]-0x3c8);returned()
            else:raise AssertionError(hex(at))
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        try:old.invoke(0x3b10b4,[OUT,ATT,B,0])
        except Boundary:pass
        finally:old.uc.hook_del(hook)
        def count(op):return sum(t[0]==OP[op] for t in trace)
        def actor(at):return [normalized(at),struct.unpack('<H',old.uc.mem_read(at+0x14d0,2))[0],byte(at+0x14f0),byte(at+0x53b),word(at+0x110)]
        statuses=sum(count(op) for op in ['apply_dot','dodge','block','hurt','push','stun','fear','slow'])
        expected={'status':status[0],'report':[len(trace),trace[-1][0],count('debug_load'),count('hit_for'),count('regen_hp')+count('regen_mp'),statuses,count('cancel_sneaking')+count('combat_text')+count('combat_sound'),count('ai_combat_result'),unsupported[0],captured[0],threat[0]],'attacker':actor(A),'defender':actor(B),'dead':dead[0],'debug':normalized(word(debug_slot)),'fx':normalized(word(fx_slot)),'result':list(struct.unpack('<10I',old.uc.mem_read(OUT,40))),'gold':signed(word(ATT+0x39c)),'trace':trace}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,inputs)],text=True))
        assert expected==actual,(name,inputs,expected,actual)
        if status[0]:boundary_counts[str(status[0])]=boundary_counts.get(str(status[0]),0)+1
        records.append({'case':name,'input':inputs,'matched':True,'source_result':expected,'compiled_result':actual})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'boundary_cases':boundary_counts,'actual_caller_instructions_observed':len(instructions),'instruction_addresses':[hex(a) for a in sorted(instructions)],'imports_verified':imports,'imports_modeled':old.import_calls,'results':records,'scope':'Original F_ApplyResult offline traversal executes for NPC and Player defenders, including Player idle-state outcome mutation, source property counters/TrophyTable unlocks, Player defender stats, attacker inventory gold debit, and Player attacker critical-camera/stat tail; no caller blocks are skipped to simulate successful effects. Typed named service providers observe every reached callee, with controlled mutations and raw returns. Network, co-op, and nonfinite external getter paths stop explicitly at their supported boundary and compare preserved prefixes, not normal whole-body returns. ItemInventory::AddGold numerical mutation is modeled at its reached entry; no complete AddGold/SetGold body is credited. No health/death/status/FX/audio/Debug callee body is credited. i2f, fmul and fcmpgt numerical imports modeled only after actual relocated PLT identity verification.'}

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/character-apply-result/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-apply-result/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++');assert compiler
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'character_apply_result.cpp',MODULE/'tests/character_apply_result.cpp'];command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stdout+build.stderr
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':47,'guard_cases':29,'failure_cases':158,'mismatches':0}
    arm=oracle(a.original_elf.resolve(),exe)
    owned=[MODULE/'character_apply_result.hpp',MODULE/'character_apply_result.cpp',MODULE/'tests/character_apply_result.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-apply-result/NOTES.md'];reused=[ROOT/'port/game-data/combat_result.hpp',ROOT/'port/game-data/combat.hpp',ROOT/'port/engine-resources/tests/cpu.py',MODULE/'tests/elf_import_identity.py']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':arm,'original_sha256':digest(a.original_elf),'compiler_command':command,'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in owned},'reused_source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in reused},'new_complete_caller_bodies':0,'new_complete_dependency_bodies':0,'new_bounded_source_callers':1,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['original_arm_comparison','compiler_command']},indent=2))
if __name__=='__main__':main()
