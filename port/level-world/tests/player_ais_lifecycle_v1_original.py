"""Execute the original PlayerIPhone factory/constructor instructions.

Allocation and LuaScript resource construction are explicit service boundaries.
CharAIScript stores, the zero-count vector constructor and allocator branch,
Player fields/vptr stores and pending publication execute their real ARM words.
"""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE
from elftools.elf.elffile import ELFFile
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
FUNCTIONS=[('_ZN6CharAI9SetScriptI15AISPlayerIPhoneEEvv','factory'),('_ZN12CharAIScriptC2Eb','existing constructor dependency'),
 ('_ZNSt6vectorIP9CharacterSaIS1_EEC1Ej.clone.3','empty vector constructor'),('_ZNSaIP9CharacterE11_M_allocateEjRj','zero-count allocator branch'),
 ('_ZN6CharAI17LoadScriptProcessEv','existing lifecycle dependency'),('_ZN6CharAI22LoadNInitScriptProcessEb','existing lifecycle dependency'),
 ('_ZN6CharAI17InitScriptProcessEb','existing lifecycle dependency'),('_ZN6CharAI6OnInitEv','existing lifecycle dependency'),
 ('_ZN6CharAI14StepInitScriptEv','existing lifecycle dependency'),('_ZN10AISDefault6OnInitEv','proven Player inherited leaf'),
 ('_ZN10AISDefault10OnInitPostEv','proven Player inherited leaf'),('_ZN10AISDefault11OnInitFinalEv','proven Player inherited leaf')]
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def provenance(elf):
    assert digest(elf)==ELF_SHA
    blob=elf.read_bytes()
    with elf.open('rb') as f:
        e=ELFFile(f);sy={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()}
        rows=[]
        for name,scope in FUNCTIONS:
            s=sy[name];address,size=int(s['st_value']),int(s['st_size'])
            seg=next(g for g in e.iter_segments() if g['p_type']=='PT_LOAD' and int(g['p_vaddr'])<=address and address+size<=int(g['p_vaddr'])+int(g['p_filesz']))
            offset=int(seg['p_offset'])+address-int(seg['p_vaddr'])
            rows.append({'original_symbol':name,'elf_address':hex(address),'size':size,'sha256':hashlib.sha256(blob[offset:offset+size]).hexdigest(),'scope':scope})
        vtables=[]
        for name in ('_ZTV9AISPlayer','_ZTV15AISPlayerIPhone'):
            s=sy[name];address,size=int(s['st_value']),int(s['st_size'])
            seg=next(g for g in e.iter_segments() if g['p_type']=='PT_LOAD' and int(g['p_vaddr'])<=address and address+size<=int(g['p_vaddr'])+int(g['p_filesz']))
            offset=int(seg['p_offset'])+address-int(seg['p_vaddr']);raw=blob[offset:offset+size]
            slots={hex(at):hex(struct.unpack_from('<I',raw,at+8)[0]) for at in (8,12,16,20,0xcc)}
            assert [slots[hex(at)] for at in (8,12,16)]==['0x3dbe78','0x3dbe7c','0x3dbe80']
            vtables.append({'symbol':name,'elf_address':hex(address),'size':size,'sha256':hashlib.sha256(raw).hexdigest(),'address_point':hex(address+8),'slots':slots})
    return {'original_sha256':ELF_SHA,'functions':rows,'vtables':vtables,'attribution':{'repository':'AdamCelermajer/DH_sc','commit':'791e961b12233100b303038c961666834f4beb9d','reuse':'CharacterPlayerSkillsV6 macro-includes unchanged V3 constructor/load/init composition. Current maintained lifecycle/constructor/default-leaf/VCB modules are reused; no Adam private VM/timer/property owner is imported.'},'additional_complete_original_body_credit':0}
def capture(elf:Path,output:Path):
    manifest=provenance(elf)
    original=Cpu(elf,False,manifest);data=original.data
    ai,owner,ais=data+0x1000,data+0x2000,data+0x5000
    original.uc.mem_write(ai,bytes(0x100));original.pointer(ai+4,owner)
    original.uc.mem_write(ais,bytes([0xcd])*0xd8)
    calls=[]
    def word(at):return struct.unpack('<I',original.uc.mem_read(at,4))[0]
    def ret(value=0):original.put(0,value);original.uc.reg_write(original.pc,original.uc.reg_read(original.lr))
    def hook(uc,address,size,unused):
        if address==0x310570:
            assert original.reg(0)==0xd8 and original.reg(1)==0
            calls.append({'operation':'allocate','bytes':original.reg(0),'hint':original.reg(1)});ret(ais)
        elif address==0x37c674:
            assert original.reg(0)==ais and original.reg(1)==1
            calls.append({'operation':'lua_construct','skip_bind':original.reg(1),'owner_before':word(ais+0x98)});ret(ais)
    original.uc.hook_add(UC_HOOK_CODE,hook)
    original.invoke(0x3ccfe4,[ai])
    scalar_offsets=[0xb8,0xbc,0xc0,0xb4,0xa0,0xac,0xc4,0xc8,0xcc,0xd0,0xd4]
    words=[word(ais+at) for at in scalar_offsets]
    assert words==[0]*len(words) and word(ais+0x98)==0
    assert word(ais+0xa4)==ais+0x9c and word(ais+0xa8)==ais+0x9c
    assert original.uc.mem_read(ais+0x9c,1)[0]==0 and word(ai+0x20)==ais
    assert word(ais)==int(manifest['vtables'][1]['address_point'],0)
    result={'original_sha256':ELF_SHA,'constructor_scalar_offsets':[hex(at) for at in scalar_offsets],'constructor_scalar_words':words,
        'owner_98':0,'tree_empty':True,'pending_published':True,'final_dispatch':'AISPlayerIPhone','service_calls':calls,
        'executed_pinned_function_words':len(original.seen),'import_calls':original.import_calls,
        'scope':'Original factory and CharAIScript stores, zero-count vector construction/allocator branch execute. Allocation and real Lua construction are explicit providers. Invalid-assertion/old-pending replacement branches are not executed.'}
    assert not original.import_calls
    # Execute the original LoadNInit/LoadScriptProcess/OnInit/InitProcess and
    # Player inherited virtual leaves. Selection, bindings, files, statistics
    # and timer/design bodies are declared service fixtures; no phase store or
    # pending-to-active publication is intercepted.
    cpu=Cpu(elf,False,manifest);base=cpu.data
    char_ai,character,pending,vt,dead_query=base+0x1000,base+0x2000,base+0x5000,base+0x6000,base+0x7000
    cpu.uc.mem_write(char_ai,bytes(0x100));cpu.pointer(char_ai,cpu.symbols['_ZTV6CharAI']+8)
    cpu.pointer(char_ai+4,character);cpu.pointer(char_ai+0x10,0xffffffff);cpu.pointer(char_ai+0x14,0xffffffff)
    cpu.pointer(character,vt);cpu.pointer(vt+0x34,dead_query)
    cpu.uc.mem_write(pending,bytes([0xcd])*0xd8)
    events=[];timer_ids=[]
    def w(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def done(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def event(name):events.append({'operation':name,'phase':w(char_ai+0x28),'active':int(w(char_ai+0x1c)==pending),'pending':int(w(char_ai+0x20)==pending)})
    def lifecycle(uc,address,size,unused):
        if address==0x3cf04c:
            event('select_player');cpu.uc.mem_write(char_ai+0x2c,b'\x01')
            # The recovered typed selector selects this exact factory. Tail
            # enter it with the caller's LR; all factory words still execute.
            cpu.uc.reg_write(cpu.pc,0x3ccfe4)
        elif address==0x310570:
            assert cpu.reg(0)==0xd8 and cpu.reg(1)==0;event('allocate');done(pending)
        elif address==0x37c674:
            assert cpu.reg(0)==pending and cpu.reg(1)==1;event('lua_construct');done(pending)
        elif address==0x3cc278:event('bind_ais');done()
        elif address==0x3cc26c:event('set_character');cpu.pointer(pending+0x98,character);done()
        elif address==0x3cc218:event('load_common');done()
        elif address==dead_query:event('is_dead');done(0)
        elif address==0x4c4bdc:event('design_tick');done(100 if not timer_ids else 200)
        elif address==0x3dbe24:
            assert cpu.reg(2)==0xffffffff and cpu.reg(3)==0x33+len(timer_ids)
            event('start_timer');timer_ids.append(len(timer_ids));done(timer_ids[-1])
        elif address==0x3b3a70:event('refresh_vitals');done()
        elif address==0x3ce044:event('configure_skills');done()
        elif address==0x3d8894:event('update_skills');done()
    cpu.uc.hook_add(UC_HOOK_CODE,lifecycle)
    source_return=cpu.invoke(0x3cf3a4,[char_ai,1])
    assert source_return==1 and w(char_ai+0x28)==7 and w(char_ai+0x1c)==w(char_ai+0x20)==pending
    assert (w(char_ai+0x10),w(char_ai+0x14))==(0,1)
    assert [e['operation'] for e in events]==['select_player','allocate','lua_construct','bind_ais','set_character','load_common','is_dead','design_tick','start_timer','design_tick','start_timer','refresh_vitals','configure_skills','update_skills']
    assert all(e['phase']==7 and e['active']==e['pending']==1 for e in events[-3:])
    assert not cpu.import_calls
    result['original_load_and_init']={'source_return':source_return,'load_phase_28':w(char_ai+0x28),'active_equals_pending':True,
        'timer_ids':timer_ids,'events':events,'executed_pinned_function_words':len(cpu.seen),'import_calls':cpu.import_calls,
        'scope':'Original load/init phase stores, actual factory/CharAIScript/vector bodies, OnInit timer caller and Default Player init/post/final leaves execute. Selector facts, binding/file, stats/configure/update and design/timer allocation are explicit providers.'}
    # Execute the two distinct source entry points with a real publication
    # boundary between them. No new phase/active store or replay guard is added.
    cpu.uc.mem_write(char_ai,bytes(0x100));cpu.pointer(char_ai,cpu.symbols['_ZTV6CharAI']+8)
    cpu.pointer(char_ai+4,character);cpu.pointer(char_ai+0x10,0xffffffff);cpu.pointer(char_ai+0x14,0xffffffff)
    cpu.uc.mem_write(pending,bytes([0xcd])*0xd8);events.clear();timer_ids.clear()
    cpu.invoke(0x3cf1f0,[char_ai])
    assert w(char_ai+0x28)==7 and w(char_ai+0x1c)==w(char_ai+0x20)==pending
    load_events=list(events)
    assert [e['operation'] for e in load_events]==['select_player','allocate','lua_construct','bind_ais','set_character','load_common','is_dead','design_tick','start_timer','design_tick','start_timer']
    cpu.invoke(0x3cf1f0,[char_ai])
    assert events==load_events
    assert cpu.invoke(0x3cf3a4,[char_ai,1])==0 and events==load_events
    cpu.invoke(0x3ce7c0,[char_ai,0])
    finish_events=events[len(load_events):]
    assert [e['operation'] for e in finish_events]==['refresh_vitals','configure_skills','update_skills']
    assert all(e['phase']==7 and e['active']==e['pending']==1 for e in finish_events)
    assert cpu.invoke(0x3cf3a4,[char_ai,1])==0 and events==load_events+finish_events
    assert not cpu.import_calls
    result['original_split_load_init']={'load_phase_28':w(char_ai+0x28),'active_equals_pending':True,
        'load_events':load_events,'finish_events':finish_events,'load_performs_init_process':False,
        'repeated_load_no_services':True,'load_and_init_active_guard_no_services':True,
        'explicit_finish_init_final':0,'constructor_calls':sum(e['operation']=='lua_construct' for e in events),
        'vitals_calls':sum(e['operation']=='refresh_vitals' for e in events),'configure_calls':sum(e['operation']=='configure_skills' for e in events),
        'executed_pinned_function_words':len(cpu.seen),'import_calls':cpu.import_calls,
        'scope':'Actual original LoadScriptProcess followed by actual InitScriptProcess. The void source wrappers have no meaningful r0 return; native wrapper completion is 1. Profile/equipment/slot-grant bodies are not supplied by this fixture.'}
    output.parent.mkdir(parents=True,exist_ok=True);output.write_text(json.dumps(result,indent=2)+'\n')
    return result
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser();p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    print(json.dumps(capture(a.original_elf,a.output)))
