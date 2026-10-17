"""Pinned original cleanup callers, with exact resource/Call boundaries."""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
ORIGINAL='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
RANGES=[('_ZN6CharAI13_SkillCleanUpEv',0x3d8ae0,72),('_ZN6CharAI13_SpellCleanUpEv',0x3d8a98,72),('_ZN17CharAISkillScript14OnSkillCleanUpEv',0x3daafc,212)]
DEPENDENCIES=['_ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsERNS4_12ReturnValuesE','_ZN9LuaScript4CallEPKcRN3sfc6script3lua12ReturnValuesE','_ZN3sfc6script3lua12ReturnValuesC1Ev','_ZN3sfc6script3lua12ReturnValuesD1Ev','_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE8_M_eraseEPS3_S6_RKSt12__false_type']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def evidence(elf):
    from elftools.elf.elffile import ELFFile
    assert sha(elf)==ORIGINAL
    blob=elf.read_bytes();rows=[]
    with elf.open('rb') as f:
        e=ELFFile(f);sy={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()}
        for name in [n for n,_,_ in RANGES]+DEPENDENCIES:
            s=sy[name];a,n=int(s['st_value']),int(s['st_size'])
            if name in [x[0] for x in RANGES]:assert (a,n)==next((a,n) for key,a,n in RANGES if key==name)
            g=next(g for g in e.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=a and a+n<=g['p_vaddr']+g['p_filesz']);offset=int(g['p_offset'])+a-int(g['p_vaddr'])
            rows.append({'original_symbol':name,'elf_address':hex(a),'size':n,'sha256':hashlib.sha256(blob[offset:offset+n]).hexdigest(),'scope':'original caller executed' if name in [x[0] for x in RANGES] else 'pinned dependency; resource/Call boundary fixture, real host Session supplies Call'})
    return rows
def compare(elf,exe,cache,temp,env,run):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'engine-resources/tests'));from cpu import Cpu
    rows=evidence(elf);cpu=Cpu(elf,False,{'original_sha256':ORIGINAL,'functions':rows})
    ai=cpu.data+0x1000;owner=cpu.data+0x2000;script=cpu.data+0x3000;vector=cpu.data+0x4000;data=cpu.data+0x5000;skills=cpu.data+0x6000;faeries=cpu.data+0x7000
    ids=[cpu.data+0x8000+i*0x100 for i in range(4)];instructions=set();records=[]
    def store(at,value):cpu.pointer(at,value)
    def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def text(at):return bytes(cpu.uc.mem_read(at,32)).split(b'\0')[0].decode()
    scenarios=[(0,0,se,sc,ce,cc) for se in (0,7) for sc in (0,2) for ce,cc in ((0,0),(0,3),(9,0))]
    scenarios+=[(mode,ns,0,2,0,3) for mode in range(4) for ns in (0,1)]
    scenarios+=[(mode,0,0,0,0,0) for mode in (4,5,6)]
    for mode,null_script,set_error,set_count,callback_error,callback_count in scenarios:
        store(ai+0xb4,skills);store(ai+0xb8,skills+12);store(ai+0xc0,faeries);store(ai+0xc4,faeries+8)
        if mode>=4:store(ai+0xb8,skills);store(ai+0xc4,faeries)
        for at,id in zip((skills,skills+4,skills+8,faeries,faeries+4),(ids[0],0,ids[1],ids[2],ids[3])):store(at,id)
        for id in ids:store(id+4,owner)
        store(owner+0x3e4,0 if null_script else script)
        state={'constructed':0,'set_calls':0,'cleanup_calls':0,'erased':0,'destroyed':0,'completed':0,'retained':0,'temp':0,'current':0};order=[]
        def fill(count):
            store(vector,data);store(vector+4,data+count*0x70)
        def hook(_,at,__,___):
            if any(a<=at<a+n for _,a,n in RANGES):instructions.add(at)
            if at==0x3daafc:state['current']=cpu.reg(0);order.append(ids.index(cpu.reg(0)))
            elif at==0x31b434:
                state['constructed']+=1;state['temp']=cpu.reg(0);store(state['temp']+8,0);store(state['temp']+0x24,vector);fill(0);ret()
            elif at==0x37c390:
                assert cpu.reg(0)==script and cpu.reg(2)==state['current']+0xc and cpu.reg(3)==state['temp'] and text(cpu.reg(1))=='SetSkill'
                state['set_calls']+=1;store(state['temp']+8,set_error);fill(set_count);ret()
            elif at==0x31c3cc:
                assert cpu.reg(0)==vector and cpu.reg(1)==data and cpu.reg(2)==data+set_count*0x70
                state['erased']+=1;fill(0);ret()
            elif at==0x37c494:
                assert cpu.reg(0)==script and cpu.reg(2)==state['temp'] and text(cpu.reg(1))=='OnSkillCleanUp'
                state['cleanup_calls']+=1;store(state['temp']+8,callback_error);fill(callback_count);ret()
            elif at==0x31b398:
                assert cpu.reg(0)==state['temp'];state['destroyed']+=1;state['completed']+=1;fill(0);ret()
        h=cpu.uc.hook_add(UC_HOOK_CODE,hook)
        try:
            if mode==0:cpu.invoke(0x3daafc,[ids[0]])
            if mode in (1,3,4,6):cpu.invoke(0x3d8ae0,[ai])
            if mode in (2,3,5,6):cpu.invoke(0x3d8a98,[ai])
        finally:cpu.uc.hook_del(h)
        expected={k:v for k,v in state.items() if k not in ('temp','current')}
        actual=json.loads(run([exe,'--oracle',cache,temp,mode,null_script,int(bool(set_error)),set_count,int(bool(callback_error)),callback_count],env))
        assert actual==expected,(mode,null_script,set_error,set_count,callback_error,callback_count,expected,actual)
        assert order==({0:[0],1:[0,1],2:[2,3],3:[0,1,2,3],4:[],5:[],6:[]}[mode])
        records.append({'input':[mode,null_script,set_error,set_count,callback_error,callback_count],'original':expected,'compiled':actual,'original_instance_order':order})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'functions':rows,'instructions_executed':[hex(x) for x in sorted(instructions)],'executed_words':len(instructions),'imports_executed':cpu.import_calls,'results':records,'scope':'Full three cleanup caller bodies execute their normal/null/error/erase/vector branches. Original Lua Call/resource/erase/destructor leaves are boundary fixtures; host uses the real retained Session and ReturnValues observer. ARM void return ignored. Source stack-canary failure is not claimed.'}
