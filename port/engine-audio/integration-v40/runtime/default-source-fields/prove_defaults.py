from pathlib import Path
import sys,struct,json,hashlib,random,ctypes
ROOT=Path(__file__).resolve().parents[5];OUT=Path(__file__).resolve().parent;OUT.mkdir(parents=True,exist_ok=True)
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,str(ROOT/'port/engine-animation/tests'))
from particle_factory_differential import FactoryCpu,words
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from unicorn import UC_HOOK_CODE
class DefaultsCpu(FactoryCpu):
    def external(self,uc,address,size,user):
        if self.imports.get(address) in ('__aeabi_idiv','__aeabi_idivmod'):
            x,y=ctypes.c_int32(self.reg(0)).value,ctypes.c_int32(self.reg(1)).value;assert y
            q=int(x/y);self.put(0,q&0xffffffff);self.put(1,(x-q*y)&0xffffffff);uc.reg_write(self.pc,uc.reg_read(self.lr));return
        return super().external(uc,address,size,user)
LIB=ROOT/'.local-inputs/libDungeonHunter2.so';cpu=DefaultsCpu(LIB,False,{'functions':[]});md=Cs(CS_ARCH_ARM,CS_MODE_ARM);md.skipdata=True
word=lambda a:struct.unpack('<I',cpu.uc.mem_read(a,4))[0]
emitter=cpu.data+0x1000;driver=emitter+0x1000;data=driver+0x1000;engine=data+0x1000;manager=engine+0x2000
phase='';call_arguments=[]
def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
manager_calls={i.address:i for i in md.disasm(bytes(cpu.uc.mem_read(0x36c2e4,0xF8)),0x36c2e4)if i.mnemonic=='bl'}
def hook(uc,at,size,u):
    if at in (0x8935d0,0x89347c,0x893478):ret()
    elif phase=='emitter' and at==0x8652b8:uc.reg_write(cpu.pc,0x8665b8)
    elif phase=='driver' and at==0x8931f8:uc.reg_write(cpu.pc,0x8932c0)
    elif phase=='manager' and at==0x36c3dc:uc.reg_write(cpu.pc,0x36c4bc)
    elif phase=='manager' and at in manager_calls:
        ins=manager_calls[at]
        if ins.op_str=='#0x861b38':call_arguments.append(dict(call=hex(at),integer_parameter=cpu.reg(1),value=cpu.reg(2)));return
        cpu.put(0,0);uc.reg_write(cpu.pc,at+4)
cpu.uc.hook_add(UC_HOOK_CODE,hook)
captures=[];rng=random.Random(0xDEF4026)
for trial in range(24):
    cpu.uc.mem_write(emitter,bytes(rng.randrange(256)for _ in range(0x160)));cpu.uc.mem_write(data,bytes(128));phase='emitter'
    cpu.invoke(0x866380,[emitter,0,0x123,0x456,6,3,driver,data],budget=100000)
    fields=bytes(cpu.uc.mem_read(emitter+0x9c,36))+bytes(cpu.uc.mem_read(emitter+0xc0,16))+bytes(cpu.uc.mem_read(emitter+0x3c,4))+bytes(cpu.uc.mem_read(emitter+0x74,4))
    expected=words([0]*9+[0,0x7f7fffff,0x42c80000,0x3f800000,0x3f800000,0x3f800000])
    assert fields==expected,(trial,fields.hex())
    phase='';cpu.invoke(0x8652b8,[emitter],budget=100000)
    assert word(emitter+0xc0)==0
    assert bytes(cpu.uc.mem_read(emitter+0x9c,36))==bytes(36)
    assert cpu.invoke(0x8656d0,[emitter])==0x3f800000
    assert cpu.invoke(0x8656a8,[emitter])==0x3f800000
    captures.append(fields)
(OUT/'original-emitter-prefix.bin').write_bytes(words([len(captures),len(captures[0])])+b''.join(captures))

# Prefix of the real callback driver's Init before buffer provisioning. Format
# producer and sample-rate scalar are explicit required fixtures; defaults are
# the instructions reached before the buffer allocator boundary.
cpu.uc.mem_write(driver,bytes([0xa5])*0x240);cpu.pointer(driver+0x10,2);cpu.pointer(driver+0x14,32000);cpu.pointer(driver+0x18,16);cpu.pointer(0xa33e68,48000)
phase='driver';cpu.invoke(0x893108,[driver],budget=100000)
driver_fields=bytes(cpu.uc.mem_read(driver+0x6c,0x50));assert driver_fields[:36]==bytes(36)
assert word(driver+0x28)==0x4000 and word(driver+0x34)==0x4000
assert [word(driver+a)for a in (0x90,0x94,0x98,0x9c)]==[0,0x7f7fffff,0x3f800000,0x3f800000]
(OUT/'original-driver-init-prefix.bin').write_bytes(driver_fields)
phase='';assert cpu.invoke(0x890bb0,[driver])==0x3f800000;assert cpu.invoke(0x890b78,[driver])==0x3f800000

# Direct ELF .data initializer words, distinct from uninitialized engine object
# storage. Manager source initialization explicitly selects distance model4.
general=[word(0x99e260),word(0x99e264),word(0x99e268)];assert general==[2,0x3f800000,0x43aba666]
cpu.uc.mem_write(engine,bytes(0x600));cpu.pointer(0xa33a08,engine);cpu.uc.mem_write(manager,bytes(0x200));phase='manager'
cpu.invoke(0x36c2e4,[manager],budget=100000)
assert call_arguments==[dict(call='0x36c3d8',integer_parameter=2,value=4)]
assert word(engine+0x430)==4 and cpu.uc.mem_read(engine+0x436,1)[0]==1
for address,length,name in [(0x866380,588,'emitter-constructor.asm'),(0x8652b8,224,'emitter-reset3d.asm'),(0x893108,0xf0,'driver-init-prefix.asm'),(0x36c2e4,0xf8,'manager-initialize-prefix.asm'),(0x865ee0,56,'engine-set-general-integer.asm'),(0x890198,188,'driver-set-general.asm')]:
    (OUT/name).write_text('\n'.join(f'{i.address:08x} {i.mnemonic} {i.op_str}'for i in md.disasm(bytes(cpu.uc.mem_read(address,length)),address))+'\n')
report=dict(validation='PASS',original_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest(),original_instructions_executed=True,emitter_prefix_cases=24,emitter_prefix_boundary='0x8652b8 redirected to real epilogue0x8665b8; then whole Reset3D and GetGain/GetPitch execute separately.',emitter_defaults=dict(relative_offset='0xc0',relative=0,position_offset='0x9c',velocity_offset='0xa8',direction_offset='0xb4',vectors=[0,0,0],maximum_distance_bits='0x7f7fffff',reference_distance_bits='0x42c80000',rolloff_bits='0x3f800000',gain_bits='0x3f800000',pitch_bits='0x3f800000'),driver_init_boundary='0x8931f8 redirected to real epilogue0x8932c0 before buffer allocation. Format/sample-rate supplied fixture.',driver_gain_q14=16384,driver_pitch_q14=16384,driver_general_initializers=dict(distance_model=dict(address='0x99e260',value=general[0]),doppler_factor=dict(address='0x99e264',bits=hex(general[1]),value=1.0),speed_over_doppler=dict(address='0x99e268',bits=hex(general[2]),value=struct.unpack('<f',words([general[2]]))[0])),manager_initialization=call_arguments,manager_engine_state=dict(distance_model=word(engine+0x430),dirty_flag=1),scope='Fresh emitter/driver and reached original manager initialization defaults only. Actual later listener/settings/general setters override them. No unknown current-state getter, source-ready/World-phase assumption, driver buffer/render or Android acceptance. Single-thread mutex services are explicit noops.')
(OUT/'proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
