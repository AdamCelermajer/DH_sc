"""Bounded original constructor/policy and registration-order evidence.

No production changes or native pose comparison. The sampler call is observed
at its original call boundary; its body is a fixture because this probe asks
only which interpolation argument the original caller produces.
"""
import argparse, hashlib, json, struct, sys
from pathlib import Path
from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_WRITE

HERE=Path(__file__).resolve().parent
REPO=HERE.parents[4]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu, words, word

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--engine',type=Path,default=REPO/'.local-inputs/libDungeonHunter2.so')
    parser.add_argument('--output',type=Path,default=HERE/'probe.json')
    args=parser.parse_args()
    manifest=json.loads((HERE/'original-functions.json').read_text())
    assert hashlib.sha256(args.engine.read_bytes()).hexdigest()==manifest['original_sha256']
    cpu=Cpu(args.engine,False,manifest)
    obj=cpu.data+0x1000; shared=cpu.data+0x3000; game=shared+0x100; engine=shared+0x200
    allocation_calls=[]; interpolation_arguments=[]; writes=[]
    active={'name':'','object':obj}
    def ret(value):
        cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def services(uc,address,size,user):
        if address in (0x310570,0x5341ac):
            count=cpu.reg(0);assert count<0x100000
            pointer=cpu.heap;cpu.heap+=(count+15)&~15
            cpu.uc.mem_write(pointer,bytes(count));allocation_calls.append({'address':hex(address),'bytes':count});ret(pointer)
        elif address==0x66a1a8:
            interpolation_arguments.append(word(bytes(cpu.uc.mem_read(cpu.uc.reg_read(cpu.sp),4)),0));ret(0)
    def observe(uc,access,address,size,value,user):
        if address==active['object']+12:
            writes.append({'probe':active['name'],'instruction':hex(cpu.uc.reg_read(cpu.pc)),'value':value,'size':size})
    cpu.uc.hook_add(UC_HOOK_CODE,services)
    cpu.uc.hook_add(UC_HOOK_MEM_WRITE,observe)
    constructors=[]
    for poison in (0,1,2,0xffffffff,0xa5a5a5a5):
        active['name']='ISceneNodeAnimator C1';cpu.uc.mem_write(obj,words([poison])*64)
        cpu.invoke(0x669828,[obj]);value=word(bytes(cpu.uc.mem_read(obj+12,4)),0);assert value==0
        constructors.append({'constructor':'0x669828','initial':poison,'field':value})
    database=cpu.data+0x5000;resource=database+0x100;loaded=database+0x200;root=database+0x300
    cpu.pointer(database,resource);cpu.pointer(resource+0x24,loaded);cpu.pointer(loaded+0x20,root)
    cpu.uc.mem_write(root,bytes(0x80))
    cpu.pointer(shared,game);cpu.pointer(game+0x20,engine)
    cpu.uc.mem_write(engine,bytes(0x80));cpu.uc.mem_write(engine+4,words([100]));cpu.pointer(engine+12,engine+0x100);cpu.pointer(engine+16,engine+0x100)
    cpu.pointer(engine+0x24,database);cpu.pointer(engine+0x28,database+8)
    active['name']='game AnimatorSet C1';cpu.uc.mem_write(obj,words([0xa5a5a5a5])*64)
    cpu.invoke(0x3676b8,[obj,shared]);value=word(bytes(cpu.uc.mem_read(obj+12,4)),0);assert value==0
    constructors.append({'constructor':'0x3676b8','initial':0xa5a5a5a5,'field':value,'targets':0})
    initialization=[]
    engine_shared=shared+8;cpu.pointer(engine_shared,engine)
    active['name']='engine animator C1';cpu.uc.mem_write(obj,words([0xa5a5a5a5])*64)
    cpu.invoke(0x660c20,[obj,engine_shared]);assert word(bytes(cpu.uc.mem_read(obj+12,4)),0)==0
    constructors.append({'constructor':'0x660c20','initial':0xa5a5a5a5,'field':0,'targets':0})
    for field in (0,1,2,0xffffffff):
        active['name']='engine init and selection';cpu.uc.mem_write(obj+12,words([field]))
        cpu.invoke(0x660af4,[obj,engine_shared]);assert word(bytes(cpu.uc.mem_read(obj+12,4)),0)==field
        cpu.invoke(0x65f8c8,[obj,0]);assert word(bytes(cpu.uc.mem_read(obj+12,4)),0)==field
        initialization.append({'field_preserved':field,'functions':['0x660af4','0x65f8c8']})
    serialization=[]
    for field in (0,1,2,0xffffffff):
        for function in (0x667cb4,0x667cb8):
            active['name']='no-op attributes';cpu.uc.mem_write(obj+12,words([field]));before=bytes(cpu.uc.mem_read(obj,0xa4))
            cpu.invoke(function,[obj,0,0]);assert before==bytes(cpu.uc.mem_read(obj,0xa4))
            serialization.append({'function':hex(function),'field_preserved':field})
    table=cpu.invoke(0x667c38,[0]);labels=[]
    for i in range(3):labels.append(cpu.string(word(bytes(cpu.uc.mem_read(table+i*4,4)),0)).decode())
    assert labels==['Default','Step','Linear']
    # Genuine binding layout, null default, mode2. Selected database and segment
    # are preinstalled immutable caller fixtures, so real getters execute.
    records=cpu.data+0x6000;cursor=records+0x100;output=records+0x300
    cpu.pointer(obj+0x24,engine);cpu.pointer(engine+0x24,database)
    cpu.pointer(engine+0x30,records);cpu.uc.mem_write(records,words([2,0,0]));cpu.pointer(obj+0x40,cursor)
    cpu.uc.mem_write(obj+0x4c,words([0,0]));cpu.uc.mem_write(obj+0x54,words([0]))
    # getAnimationData requires source clip record; this is the only selected
    # segment service fixture in this focused policy caller probe.
    def segment(uc,address,size,user):
        if address==0x65f364:ret(0)
    cpu.uc.hook_add(UC_HOOK_CODE,segment)
    policies=[]
    for field in (0,1,2,3,0xffffffff,0x80000000):
        active['name']='caller policy';cpu.uc.mem_write(obj+12,words([field]));cpu.invoke(0x65f7b4,[obj,0,100,output])
        actual=interpolation_arguments[-1];assert actual==int(field!=1);policies.append({'field':field,'interpolate':actual})
    # Actual database-vector push_back, with preallocated capacity. Caller IDs
    # are deliberately non-sorted and repeated; reference object bytes retained.
    registration=[];vector=cpu.data+0x8000;input_=vector+0x1000;capacity=vector+0x100
    cpu.uc.mem_write(vector,bytes(0x80));cpu.pointer(vector+0x24,capacity);cpu.pointer(vector+0x28,capacity);cpu.pointer(vector+0x2c,capacity+80)
    for identifier in (248,241,243,241,328):
        cpu.uc.mem_write(input_,words([0,identifier]));index=cpu.invoke(0x6601d4,[vector,input_]);registration.append({'caller_id':identifier,'returned_index':index})
    contents=[word(bytes(cpu.uc.mem_read(capacity+i*8,8)),4) for i in range(len(registration))]
    assert contents==[248,241,243,241,328]
    report={'validation':'PASS','original_sha256':manifest['original_sha256'],'manifest_sha256':hashlib.sha256((HERE/'original-functions.json').read_bytes()).hexdigest(),
            'original_instructions_executed':True,'constructors':constructors,'enum_labels':labels,'enum_table':hex(table),'field_writes':writes,
            'initialization_and_selection':initialization,'serialization_noops':serialization,'sampling_argument_cases':policies,'registration':registration,'registration_vector_order':contents,
            'allocation_services':allocation_calls,'imports':cpu.import_calls,'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
            'scope':'Actual inherited and game constructor/init/timeline/applicator instructions with zero-target immutable AnimationSet. Allocator service is supplied; no file loading. Actual getAnimationValue caller with selected-segment and downstream-sampler fixtures proves policy argument only. Actual database-vector insertion with supplied capacity proves append order, not real Prince registration schedule.'}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','constructors':len(constructors),'policy_cases':len(policies),'registration_calls':len(registration)}))

if __name__=='__main__':main()
