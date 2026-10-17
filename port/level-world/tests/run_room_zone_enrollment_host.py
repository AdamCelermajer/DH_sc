"""Compare the bounded RoomZone enrollment kernel with original ARM callers."""
import argparse, hashlib, json, math, struct, subprocess, sys
from pathlib import Path
from unicorn import UC_HOOK_CODE

ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/level-world'
ORIGINAL_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ADD='_ZN8RoomZone16AddInitialObjectEP10GameObject'
ENTER='_ZN10GameObject11ZoneEnteredEv'
EXIT='_ZN10GameObject10ZoneExitedEv'
IS_ZONABLE=0
REMOVE=1
APPEND=2
SYNC_VISIBILITY=3
STATE=4
OBJ=0x02110000
ZONE=0x02220000
OLD_ZONE=0x02440000
ALT_ZONE=0x02450000
VTABLE=0x02500000
NODE=0x02600000
OLD_NODE=0x02610000
IS_ZONABLE_STUB=0x02700000
STATE_STUB=0x02700010

class RoomCpu:
    """Install per-test callbacks while executing the real ARM body."""
    def __init__(self, path):
        from cpu import Cpu
        class CpuWithFcmple(Cpu):
            def external(self, uc, address, size, unused):
                name=self.imports.get(address)
                if name=='__aeabi_fcmple':
                    a=struct.unpack('<f',struct.pack('<I',self.reg(0)))[0]
                    b=struct.unpack('<f',struct.pack('<I',self.reg(1)))[0]
                    self.put(0,1 if a<=b else 0)
                    uc.reg_write(self.pc,uc.reg_read(self.lr))
                    return
                return super().external(uc,address,size,unused)
        self.cpu=CpuWithFcmple(path,False,{'functions':[{'original_symbol':ADD,'elf_address':'0x396a90','size':436}]})
        self.records=[]; self.compare_calls=0; self.zonable_calls=0; self.fixture=None
        self.node_next=NODE
        c=self.cpu
        self.debug_load=c.symbols['_ZN13DebugSwitches4loadEv']
        self.debug_get=c.symbols['_ZN13DebugSwitches9GetSwitchERKSs']
        self.string_ctor=c.symbols['_ZNSsC1EPKcRKSaIcE']
        self.string_dtor=c.symbols['_ZNSsD1Ev']
        self.remove=c.symbols['_ZN8RoomZone12RemoveObjectEP10GameObject']
        self.add_object=c.symbols['_ZN8RoomZone9AddObjectEP10GameObject']
        self.sync_visibility=0x4713d0
        self.alloc_node=0x39670c
        self.free_node=0x708f00
        c.uc.hook_add(UC_HOOK_CODE, self.hook)
        assert any(n=='__aeabi_fcmple' for n in c.imports.values()), 'binary helper import must be identity-gated'

    def read32(self, address): return struct.unpack('<I',self.cpu.uc.mem_read(address,4))[0]
    def write32(self,address,value): self.cpu.pointer(address,value)
    def read8(self,address): return self.cpu.uc.mem_read(address,1)[0]
    def f32(self,address,value): self.cpu.uc.mem_write(address,struct.pack('<f',float(value)))

    def hook(self,uc,address,size,user):
        c=self.cpu
        if address==IS_ZONABLE_STUB:
            obj=c.reg(0); self.zonable_calls+=1; room_before=self.read32(obj+0x2f4)
            raw=self.fixture['zonable_first'] if self.zonable_calls==1 else self.fixture['zonable_second']
            self.records.append([IS_ZONABLE,obj,room_before,0,raw])
            if self.fixture['mutation']==1 and self.zonable_calls==1: self.f32(obj+0x160,2.0)
            if self.fixture['mutation']==2 and self.zonable_calls==1: self.f32(ZONE+0x12c,2.0)
            if self.fixture['mutation']==5 and self.zonable_calls==1: self.write32(obj+0x2f4,ALT_ZONE)
            if self.fixture['mutation']==3 and self.zonable_calls==2:
                uc.mem_write(obj+0x2ee,b'\0'); uc.mem_write(obj+0x2f0,b'\0')
            if self.fixture['mutation']==4 and self.zonable_calls==2: uc.mem_write(obj+0x2f0,b'\0')
            c.put(0,raw); uc.reg_write(c.pc,uc.reg_read(c.lr)); return
        if address==self.remove:
            self.records.append([REMOVE,c.reg(1),c.reg(0),0,0]); return
        if address==self.add_object:
            self.records.append([APPEND,c.reg(1),c.reg(0),0,0]); return
        if address==self.sync_visibility:
            self.records.append([SYNC_VISIBILITY,OBJ,self.read32(OBJ+0x2f4),c.reg(0),0])
            uc.reg_write(c.pc,uc.reg_read(c.lr)); return
        if address==STATE_STUB:
            obj=c.reg(0); self.records.append([STATE,obj,self.read32(obj+0x2f4),0,c.reg(1)])
            uc.reg_write(c.pc,uc.reg_read(c.lr)); return
        if address==self.alloc_node:
            self.records.append([APPEND,OBJ,ZONE,0,0])
            c.put(0,self.node_next); self.node_next+=0x20
            uc.reg_write(c.pc,uc.reg_read(c.lr)); return
        if address==self.free_node or address==self.debug_load or address==self.string_ctor or address==self.string_dtor:
            uc.reg_write(c.pc,uc.reg_read(c.lr)); return
        if address==self.debug_get:
            c.put(0,0); uc.reg_write(c.pc,uc.reg_read(c.lr)); return
        if address==0x30e9ac:
            self.compare_calls+=1; return

    def run(self, fixture):
        self.fixture=fixture; self.records=[]; self.compare_calls=0; self.zonable_calls=0; self.node_next=NODE
        c=self.cpu
        # source RoomZone +0x394/+0x398 use a circular list sentinel
        for zone in (ZONE,OLD_ZONE,ALT_ZONE):
            s=zone+0x394; self.write32(s,s); self.write32(s+4,s); self.write32(zone+0x398,s)
        # candidate object and source virtual table
        self.write32(OBJ,VTABLE); self.write32(VTABLE+0x3c,STATE_STUB); self.write32(VTABLE+0xc4,IS_ZONABLE_STUB)
        self.f32(OBJ+0x160,fixture['x']); self.f32(OBJ+0x164,fixture['y'])
        self.f32(ZONE+0x12c,fixture['min_x']); self.f32(ZONE+0x130,fixture['min_y'])
        self.f32(ZONE+0x138,fixture['max_x']); self.f32(ZONE+0x13c,fixture['max_y'])
        room=fixture['old_room']
        self.write32(OBJ+0x2f4,room)
        uc=c.uc; uc.mem_write(OBJ+0x2ef,bytes([fixture['in_room']&255])); uc.mem_write(OBJ+0x2f0,bytes([fixture['in_zone']&255]))
        uc.mem_write(OBJ+0x2ee,bytes([fixture['zoning']&255])); uc.mem_write(OBJ+0x80,bytes([fixture['visible']&255]))
        self.write32(OBJ+0x2d8,fixture['visual'])
        # Old/target room membership node, when the source requests removal.
        if room:
            node=OLD_NODE; sentinel=room+0x394
            self.write32(sentinel,node); self.write32(room+0x398,node)
            self.write32(node,sentinel); self.write32(node+4,sentinel); self.write32(node+8,OBJ)
        if fixture['mode']=='add':
            result=c.invoke(ADD,[ZONE,OBJ])
        elif fixture['mode']=='enter':
            result=c.invoke(ENTER,[OBJ])
        else:
            result=c.invoke(EXIT,[OBJ])
        return {'result':result,'calls':self.records,'room':self.read32(OBJ+0x2f4),
                'in_room':self.read8(OBJ+0x2ef),'in_zone':self.read8(OBJ+0x2f0),
                'float_compares':self.compare_calls}

def cases():
    def f(name,**kw):
        x={'name':name,'mode':'add','x':0.0,'y':0.0,'min_x':0.0,'min_y':0.0,
           'max_x':0.0,'max_y':0.0,'old_room':0,'in_room':0,'in_zone':0,
           'zoning':0,'visible':0,'visual':0,'zonable_first':1,
           'zonable_second':1,'mutation':0,'fail':-1}
        x.update(kw); return x
    return [
      f('all_edges_equal'), f('exclude_below_min_x',x=-0.01),
      f('exclude_above_max_x',x=0.01), f('exclude_below_min_y',y=-0.01),
      f('exclude_above_max_y',y=0.01), f('nan_world_x',x=float('nan')),
      f('nan_bound',min_x=float('nan')), f('not_zonable',zonable_first=0),
      f('raw_nonzero_zonable',zonable_first=0xffffffff,zonable_second=0x80000000),
      f('remove_prior_room',old_room=OLD_ZONE,in_room=1,in_zone=1),
      f('existing_same_room',old_room=ZONE,in_room=1,in_zone=1),
      f('member_byte_without_previous_room',in_room=1),
      f('pointer_without_room_member_skips_removal',old_room=OLD_ZONE,in_room=0),
      f('fresh_position_after_virtual',x=1.5,min_x=-1,max_x=3,mutation=1),
      f('fresh_bound_after_virtual',x=1,min_x=-1,max_x=3,mutation=2),
      f('fresh_pointer_after_virtual',old_room=OLD_ZONE,mutation=5),
        f('entered_syncs_visible_object',mode='enter',zoning=1,visual=0x02700000,visible=1),
        f('entered_visible_flag_off',mode='enter',zoning=1,visual=0x02700000,visible=0),
        f('exited_syncs_even_when_not_visible',mode='exit',zoning=1,visual=0x02700000,visible=0,in_zone=1),
        f('exited_disabled_uses_fallback_true',mode='exit',zoning=0,visual=0x02700000,in_zone=1),
        f('entry_fresh_flags_fallback',mode='enter',zoning=1,visual=0x02700000,visible=1,mutation=3),
        f('entry_fresh_in_zone_zero',mode='enter',zoning=1,visual=0x02700000,visible=1,mutation=4),
    ]

def json_safe(value):
    if isinstance(value,float) and not math.isfinite(value): return 'NaN' if math.isnan(value) else ('Infinity' if value>0 else '-Infinity')
    if isinstance(value,dict): return {k:json_safe(v) for k,v in value.items()}
    if isinstance(value,(list,tuple)): return [json_safe(v) for v in value]
    return value

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--compiler',required=True); ap.add_argument('--original-elf',type=Path,required=True)
    ap.add_argument('--output',type=Path,default=MODULE/'build/room-zone-enrollment-host')
    args=ap.parse_args(); out=args.output.resolve(); out.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'room_zone_enrollment.cpp',MODULE/'room_zone_enrollment.hpp',MODULE/'tests/room_zone_enrollment.cpp']
    exe=out/'room-zone-enrollment-host.exe'
    subprocess.run([args.compiler,'-std=c++17','-O1','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-pedantic',str(sources[0]),str(sources[2]),'-o',str(exe)],check=True)
    if hashlib.sha256(args.original_elf.read_bytes()).hexdigest()!=ORIGINAL_SHA: raise SystemExit('original ELF hash mismatch')
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
    from elftools.elf.elffile import ELFFile
    old=RoomCpu(args.original_elf); rows=[]
    for case in cases():
        cli=[str(exe),case['mode'],*(str(case[k]) for k in ('x','y','min_x','min_y','max_x','max_y','old_room','in_room','in_zone','zoning','visible','visual','zonable_first','zonable_second','mutation','fail'))]
        host=json.loads(subprocess.run(cli,capture_output=True,text=True,check=True).stdout)
        orig=old.run(case)
        accept=orig['result']!=0 if case['mode']=='add' else False
        rejection=(0 if accept else (1 if orig['float_compares']==0 and case['mode']=='add' else 2 if case['mode']=='add' else 0))
        expected={'accepted':int(accept),'rejection':rejection,'calls':orig['calls'],
                  'room':orig['room'],'in_room':orig['in_room'],'in_zone':orig['in_zone']}
        observed={'accepted':host['accepted'],'rejection':host['rejection'],'calls':host['calls'],
                  'room':host['room'],'in_room':host['in_room'],'in_zone':host['in_zone']}
        if observed!=expected: raise AssertionError((case['name'],observed,expected,orig))
        if case['mode']=='add':
            wanted_compares=(4 if accept else orig['float_compares'])
            assert orig['float_compares']==wanted_compares,(case['name'],orig['float_compares'])
        rows.append({'name':case['name'],'input':json_safe({k:v for k,v in case.items() if k!='name'}),
                     'host':observed,'original':orig})
    # The source executor cannot fail these owner calls: they are explicit
    # providers in this reconstruction. Exercise the adapter's fail-stop
    # behavior separately and record that already-completed source writes are
    # retained (there is no rollback claim).
    failure_cases=[]
    failures=[
      ('fail_first_eligibility',cases()[0],IS_ZONABLE,0,0,0,1),
      ('fail_old_room_removal',next(c for c in cases() if c['name']=='remove_prior_room'),REMOVE,OLD_ZONE,1,1,2),
        ('fail_visibility_sync_after_enter_write',next(c for c in cases() if c['name']=='entered_syncs_visible_object'),SYNC_VISIBILITY,0,0,1,1),
      ('fail_state_callback_after_enter_write',next(c for c in cases() if c['name']=='all_edges_equal'),STATE,ZONE,1,1,3),
      ('fail_append_after_enrollment_writes',next(c for c in cases() if c['name']=='all_edges_equal'),APPEND,ZONE,1,1,4),
    ]
    for name, base, fail_op, wanted_room, wanted_in_room, wanted_in_zone, expected_calls in failures:
        case=dict(base); case['name']=name; case['fail']=fail_op
        cli=[str(exe),case['mode'],*(str(case[k]) for k in ('x','y','min_x','min_y','max_x','max_y','old_room','in_room','in_zone','zoning','visible','visual','zonable_first','zonable_second','mutation','fail'))]
        host=json.loads(subprocess.run(cli,capture_output=True,text=True,check=True).stdout)
        assert host['status']==3,(name,host)
        assert host['room']==wanted_room and host['in_room']==wanted_in_room and host['in_zone']==wanted_in_zone,(name,host)
        assert len(host['calls'])==expected_calls,(name,host['calls'])
        failure_cases.append({'name':name,'failed_operation':fail_op,'host_status':host['status'],
                              'calls':host['calls'],'room':host['room'],'in_room':host['in_room'],
                              'in_zone':host['in_zone'],'assertion':'service_failed; prior writes retained'})
    with args.original_elf.open('rb') as stream:
        elf=ELFFile(stream); symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        function_records=[]
        for name in (ADD,ENTER,EXIT,'_ZN8RoomZone12RemoveObjectEP10GameObject','_ZN8RoomZone9AddObjectEP10GameObject'):
            s=symbols[name]; raw=bytes(old.cpu.uc.mem_read(int(s['st_value']),int(s['st_size'])))
            function_records.append({'original_symbol':name,'elf_address':hex(int(s['st_value'])),'size':int(s['st_size']),'sha256':hashlib.sha256(raw).hexdigest()})
    report={'validation':'PASS','original_arm_cases':len(rows),'mismatches':0,'original_sha256':ORIGINAL_SHA,
      'functions':function_records,'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+[Path(__file__).resolve()]},
      'scope':'RoomZone::AddInitialObject membership decision and ordered mutation/list-owner callbacks, plus GameObject ZoneEntered/ZoneExited raw-byte, VisualObject::SyncVisibility, and setUpdating callback leaves. Spatial values are source-plane XY. Zone AABB production, debug logging, and owner/list storage implementation remain explicit providers.',
      'cases':rows,'host_failure_cases':failure_cases}
    (out/'validation.json').write_text(json.dumps(report,indent=2,allow_nan=False)+'\n',encoding='utf-8')
    print(json.dumps({k:report[k] for k in ('validation','original_arm_cases','mismatches')}))
if __name__=='__main__': main()
