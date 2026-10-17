#!/usr/bin/env python3
"""Exact recovered combat formulas on owned actors; authored reference fixtures."""
import argparse,ctypes as c,hashlib,json,struct,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
f=lambda v:c.c_float(v).value
i32=lambda v:c.c_int32(v&0xffffffff).value
add=lambda a,b:f(f(a)+f(b))
sub=lambda a,b:f(f(a)-f(b))
mul=lambda a,b:f(i32(int(f(a))*int(f(b)))>>8)
def random_draw(seed,lo,hi):
    base=max(0,int(f(lo)));bound=(max(0,int(f(hi)))-base)&0xffffffff
    if bound:seed=((seed*0xe6ab+0x2b3fd)&0xffffffff)%0xdaf26b
    return seed,f(i32(base+(seed%bound if bound else 0)))
def damage(p,q,seed,attack,off,skill,state,hit,blocked,critical,element=-1):
    def dot(seed):
        duration=p[181 if skill else 125];value=0;elem=-1
        if duration>0:
            seed,value=random_draw(seed,p[179 if skill else 123],p[180 if skill else 124]);elem=int(p[178])>>8 if skill else -1
            if elem!=-1:value=max(0,sub(value,q[74+elem]))
        return seed,[duration,value,elem]
    if attack==3:return seed,[1234]
    if attack in (0,1):
        lo,hi=(p[174],p[175])if skill else(p[81],p[82])if off else(p[79],p[80]);element=int(p[100 if off else 97])>>8
        seed,value=random_draw(seed,lo,hi)
        if state==9:value=add(value,p[92])
        if p[198]>q[199]:value=add(value,p[93])
        if hit>0:value=add(value,f(f(hit)*p[94]))
        if blocked:value=mul(value,q[121])
        if critical:value=add(value,hi)
        value=sub(value,mul(q[71],25));value=256 if value<=0 else value
        hp=f(mul(value,p[132])/100);mp=f(mul(value,p[133])/100)
        if element!=-1:
            elo,ehi=(p[98],p[99])if off else(p[95],p[96])
            if elo>=0 and ehi>=0:
                seed,ev=random_draw(seed,elo,ehi);pct=max(0,min(1,sub(1,f((int(q[74+element])>>8)/100))))
                value=add(value,f(ev*pct));value=256 if value<=0 else value
        seed,dots=dot(seed);return seed,[value,element,hp,mp,*dots]
    if attack==2:
        lo,hi=(p[174],p[175])if skill else(p[79],p[80]);element=element if skill else int(p[97])>>8
        value=0
        if lo!=0 or hi!=0:
            seed,value=random_draw(seed,lo,hi)
            if element!=-1:
                pct=max(0,min(1,sub(1,f(f(q[74+element]/256)/100))))
                value=f(add(value,p[166+element])*pct);value=256 if value<=0 else value
            if critical:value=add(value,hi)
            value=sub(value,mul(q[73],51));value=256 if value<=0 else value
        if skill:seed,dots=dot(seed)
        else:dots=[0,0,0]
        return seed,[value,None,None,None,*dots]
    return seed,[]
def literal(v):return 'nil'if v is None else str(v).lower()if isinstance(v,bool)else repr(f(v))
def assertion(call,values,label):
    checks=[f"assert(select('#',{call})=={len(values)},{json.dumps(label+' count')})"]
    # Count and values must be obtained in one call because damage consumes RNG.
    assignments=','.join('v'+str(i)for i in range(max(1,len(values))))
    tests=';'.join(f'assert(v{i}=={literal(v)},{json.dumps(label+" field "+str(i))})'for i,v in enumerate(values))
    return f'do local function checked(...) assert(select("#",...)=={len(values)},{json.dumps(label+" count")});local {assignments}=...;{tests} end;checked({call}) end'
def run(*args):
    result=subprocess.run(list(map(str,args)),capture_output=True,text=True)
    if result.returncode:raise RuntimeError(result.stdout+'\n'+result.stderr)
    return result.stdout.strip()
def generate(constants,real=None):
    cases=[];checks=0
    def add_case(p,q,seed,attack,off,skill,state,hit,blocked,critical,element,label,row=None):
        nonlocal checks
        lines=['do local a=DH2CreatePropertyState('+str(row if row is not None else 2)+');local d=DH2CreatePropertyState('+str(row if row is not None else 2)+')']
        if row is None:
            lines += [f'a:SetProp({i},{v})'for i,v in enumerate(p)if v]+[f'd:SetProp({i},{v})'for i,v in enumerate(q)if v]
        lines+= [f"a:SetCombatContext(0,{hit},'attacker');d:SetCombatContext({state},0,'defender')",'CF_ClearCombatants()',f'CF_SetCombatants(a,d,{element},{str(off).lower()},{str(skill).lower()})',f'DH2SeedRandom({seed})']
        # Use negative rolls for controlled block/crit flags; arbitrary attack
        # types without these branches remain false. Clear resets both flags.
        if blocked:lines.append('assert(CF_CalcBlock(-1,0)==true)')
        if critical:lines.append('assert(CF_CalcCrit(-1,0)==true)')
        _,expected=damage(p,q,seed,attack,off,skill,state,hit,blocked,critical,element)
        lines.append(assertion(f'CF_CalcDamage(1234,{attack})',expected,label));lines.append('CF_ClearCombatants();assert(CF_CalcDamage(0,0)==false);end')
        cases.append('\n'.join(lines));checks+=len(expected)+2
    if real is not None:
        defaults,types=real[:2]
        def composed(row):
            values=[]
            for v,d,t in zip(row,defaults,types):
                if t==-1:t=16
                values.append(f(v if t&7 or t&16 and not t&32 else d if not t&32 or v==d else i32(v+d)))
            return values
        for row in range(2,len(real)):
            p=composed(real[row])
            for off in (False,True):
                for attack in (0,1,2,3):add_case(p,p,177+row,attack,off,False,0,0,False,False,-1,f'real {row} {off} {attack}',row)
        return cases,checks
    for index in range(192):
        p=[0]*224;q=[0]*224;off=bool(index&1);skill=bool(index&2);blocked=bool(index&4);critical=bool(index&8)
        attack=(0,1,2,3)[(index//16)%4];element=(-1,0,4)[index//64];state=9 if index%3==0 else 0;hit=index%4
        p[19]=q[19]=256;p[79:83]=[2560,5120,1280,2560];p[174:176]=[3840,6400];p[97]=p[100]=element*256
        p[92:95]=[512,768,256];p[198]=1;q[199]=0;p[132:134]=[2560,1280];q[121]=128
        q[71]=0 if index%5==0 else 4096 if index%5==1 else 1048576;q[73]=q[71]
        p[95:97]=[512,1024];p[98:100]=[256,768]
        p[123:126]=[256,768,768];p[178:182]=[element*256,1024,2048,1280]
        for e in range(5):q[74+e]=(-256,0,8576,25600,38400)[index%5];p[166+e]=512
        seed=(0,1,177,14348899)[index%4]
        add_case(p,q,seed,attack,off,skill,state,hit,blocked,critical,element,f'controlled {index}')
    # Branch boundaries and all status effects, with raw fixed-point levels.
    for attack in (0,1,2,3):
        for roll in (-256,0,12800,19200,25088,25600):
            p=[0]*224;q=[0]*224;p[19]=768;q[19]=256;p[50]=p[158]=2560;q[59]=q[164]=1280;q[60]=5120
            p[63]=p[165]=5120;q[61]=5120
            for target,resist,sns in ((135,134,182),(137,136,183),(139,138,184),(142,141,186),(145,144,188)):
                p[target]=p[sns]=7680;q[resist]=2560
            lines=['do local a=DH2CreatePropertyState(2);local d=DH2CreatePropertyState(2)']
            lines +=[f'a:SetProp({i},{v})'for i,v in enumerate(p)if v]+[f'd:SetProp({i},{v})'for i,v in enumerate(q)if v]
            lines +=["CF_ClearCombatants();CF_SetCombatants(a,d,-1,false,false);DH2SeedRandom(0)"]
            # The independent expected chance calculation uses original cache
            # constants and mathematical threshold comparisons, not Lua calls.
            _,dodge_roll=random_draw(0,0,100);dodge_roll*=256
            if attack in (0,2):
                prefix='COMBAT_MELEE_'if attack==0 else 'COMBAT_MAGIC_'
                auto_miss=constants[prefix+'AUTOMATIC_MISS_LIMIT'];auto_hit=constants[prefix+'AUTOMATIC_HIT_LIMIT']
                target=add(add(constants[prefix+'BASE_TO_HIT_CHANCE'],mul(1280,constants[prefix+'AR_DR_DIFFERENCE_MODIFIER_TO_HIT'])),mul(512,constants[prefix+'LEVEL_DIFFERENCE_MODIFIER_TO_HIT']))
                miss=roll>=auto_miss or roll>=auto_hit and roll>=target
                dodge=not miss and roll>=auto_hit and attack==0 and dodge_roll<add(q[60],mul(-512,256))
            else:miss=dodge=False
            lines.append(assertion(f'CF_CalcMissOrDodge({roll},{attack})',[miss,dodge],f'chance miss {attack} {roll}'));checks+=3
            block=attack==0 and roll<25088 and roll<4608;crit=attack in (0,2)and(attack!=0 or roll<25088)and roll<5632
            lines.append(assertion(f'CF_CalcBlock({roll},{attack})',[block],'block'));lines.append(assertion(f'CF_CalcCrit({roll},{attack})',[crit],'crit'));checks+=4
            for name in ('Hurt','Push','Stun','Fear','Slow'):
                threshold=5184 if name=='Slow'else 7680
                lines.append(assertion(f'CF_Calc{name}({roll},{attack})',[attack in (0,2)and roll<threshold],name));checks+=2
            lines.append("assert(pcall(CF_CalcDodge,0,0)==false);CF_ClearCombatants();assert(CF_CalcDodge(0,0)==false);end")
            cases.append('\n'.join(lines));checks+=2
    return cases,checks
def main():
    p=argparse.ArgumentParser()
    for name in ('cache','runner','report'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--real',action='store_true');p.add_argument('--adb',type=Path);p.add_argument('--serial');a=p.parse_args()
    if bool(a.adb)!=bool(a.serial):p.error('--adb and --serial required together')
    mode='real'if a.real else'controlled';stage=ROOT/('build/combat-'+mode+'-'+(a.serial or 'host'));stage.mkdir(parents=True,exist_ok=True)
    base='/data/local/tmp/dh2-combat-'+mode;files=[];constant_rows=[]
    traces=json.loads((REPO/'reports/pydata-constant-reader-trace.json').read_text(encoding='utf-8'));constants={}
    remote=lambda path:base+'/'+path.name if a.adb else str(path.resolve())
    for row in traces['files']:
        if not row['fully_consumed']:continue
        src=a.cache/row['path'];assert sha(src)==row['sha256'];dst=stage/src.name;dst.write_bytes(src.read_bytes());files.append(dst);constant_rows.append(dst)
        constants.update({r['name']:r['value']for r in row['rows']if r['group']=='CombatConstants'})
    assert len(constant_rows)==26
    props=stage/'properties.bin';src=a.cache/'data/pydata/character_properties_pyarray.bin'
    assert sha(src)=='516ba82f631174d4c0a24708342549b5993c402f68c2c1dabd5ddc9eea138784'
    real=[list(struct.unpack_from('<224i',src.read_bytes(),4+i*896))for i in range(448)]if a.real else None
    props.write_bytes(src.read_bytes()if a.real else struct.pack('<I',3)+bytes(896)+struct.pack('<224i',*([8]*224))+bytes(896)+bytes(8));files.append(props)
    loot=stage/'loot.bin';src=a.cache/'data/pydata/loot_table_pyarray.bin';assert sha(src)=='4dd85c8656d60c38a0c651e999fd18a1ed4d936f7cfdfcec52a1dfb4b568847f';loot.write_bytes(src.read_bytes());files.append(loot)
    source=REPO/'recovered/scripts/original/data/scripts/level/combat_formulas.luac';assert sha(source)=='f83639a12c5c1910f9a23fff494f4013330943ad4ef8f4d91c33fe1eb975565f'
    script=stage/'combat_formulas.luac';script.write_bytes(source.read_bytes());files.append(script)
    cases,checks=generate(constants,real);chunks=[]
    for start in range(0,len(cases),32):
        path=stage/f'assertions-{start//32:03}.lua';path.write_text('\n'.join(cases[start:start+32])+'\n',encoding='ascii',newline='\n');assert path.stat().st_size<1024*1024;files.append(path);chunks.append(path)
    constlist=stage/'constants.txt';scriptlist=stage/'scripts.txt'
    for path,paths in ((constlist,constant_rows),(scriptlist,[script,*chunks])):path.write_text('\n'.join(remote(v)for v in paths)+'\n',encoding='utf-8',newline='\n');files.append(path)
    device=None;hashes={}
    if a.adb:
        adb=lambda *args:run(a.adb,'-s',a.serial,*args)
        assert adb('shell','getprop','ro.kernel.qemu')=='1'
        device={'serial':a.serial,'release':adb('shell','getprop','ro.build.version.release'),'sdk':int(adb('shell','getprop','ro.build.version.sdk')),'page_size':int(adb('shell','getconf','PAGE_SIZE')),'abi':adb('shell','getprop','ro.product.cpu.abi')}
        assert device['release']=='17'and device['sdk']==37 and device['abi']=='x86_64'and device['page_size']in (4096,16384)
        adb('shell','mkdir','-p',base);adb('push',str(stage.resolve())+'/.',base+'/');adb('push',a.runner.resolve(),base+'/runner');adb('shell','chmod','755',base+'/runner')
        for line in adb('shell','sha256sum',*[remote(v)for v in files],base+'/runner').splitlines():digest,path=line.split(None,1);hashes[path.strip()]=digest
        for local in files:assert hashes[remote(local)]==sha(local)
        assert hashes[base+'/runner']==sha(a.runner)
        output=adb('shell',base+'/runner','--combat',remote(props),remote(loot),remote(constlist),remote(scriptlist))
    else:output=run(a.runner.resolve(),'--combat',props.resolve(),loot.resolve(),constlist.resolve(),scriptlist.resolve())
    assert output.splitlines()[-1]==f'COMBAT CORPUS PASS 26 {1+len(chunks)}'
    report={'complete_game':False,'original_interpreter_or_gameplay_equivalence_tested':False,'mode':mode,'combat_cases':len(cases),'asserted_values_and_counts':checks,'constant_files':26,'original_script_sha256':sha(source),'runner_sha256':sha(a.runner),'test_sha256':sha(Path(__file__)),
            'files':[{'name':v.name,'sha256':sha(v),'bytes':v.stat().st_size}for v in files],'stdout':output,'source_sha256':{n:sha(ROOT/n)for n in ('runtime.c','runtime.h','build.py','tests/runner.c','tests/combat.c','../lua-character/bridge.c','../random/random.c','../random/random.h','../random/lua-bridge.c')},
            'scope':'Unchanged recovered combat formula executes with owned property actors and 26 exact constant files. Damage, leech, elements, DoT, block/critical flags, state/sneak/combo bonuses, minimum damage and chance thresholds compare an independent authored Python float32/integer reference. Controlled fixtures use type-8 property writes; real mode uses all 446 actual gameplay rows, both hands, melee/range/magic/DoT. No original Lua VM, original combat dispatcher, Character/buff/state machine, HP application, damage events, AI, level or game loop executes. The unchanged CF_CalcDodge live path calls an absent CF__CalcDodge and is explicitly checked to error; no formula repair is silently applied.'}
    if device:report.update({'device':device,'pushed_hashes_verified':len(hashes)})
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('files','scope','stdout','source_sha256')}))
if __name__=='__main__':main()
