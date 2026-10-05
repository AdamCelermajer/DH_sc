"""Execute original ARM MenuManager.Push and compare the native ARM64 kernel.

Menu/HUD/touch/debug/MultiMenuManager services are explicit call fixtures.
This proves outer control order and live reads, not a functioning menu stack.
"""
import argparse
import hashlib
import itertools
import json
import struct
import subprocess
import sys
from pathlib import Path


def main():
    p=argparse.ArgumentParser();p.add_argument('--snapshot',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
    sys.path.insert(0,str(a.snapshot/'port/game-data/tests'))
    sys.path.insert(0,str(a.snapshot/'port/engine-resources/tests'))
    from items_differential import Original
    from cpu import Cpu
    from unicorn import UC_HOOK_CODE
    source=a.snapshot/'port/engine-ui/menu_manager_push_v1.cpp'
    lib=a.output/'libmenu_manager_push_audit.so'
    ndk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64')
    subprocess.run([str(ndk/'bin/aarch64-linux-android26-clang++.cmd'),'-std=c++17','-O2','-shared','-fPIC','-fno-exceptions','-fno-rtti','-nostdlib++',str(source),'-o',str(lib)],check=True)
    W=lambda *x:struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
    Q=lambda *x:struct.pack('<'+'Q'*len(x),*x)
    def text(cpu,address):
        out=bytearray()
        while cpu.uc.mem_read(address+len(out),1)!=b'\0':out.extend(cpu.uc.mem_read(address+len(out),1))
        return out.decode()
    class Old(Original):
        def external(self,uc,address,size,user):
            if address in (self.callback+48,self.callback+64,self.callback+80):
                op={self.callback+48:1,self.callback+64:2,self.callback+80:5}[address]
                self.calls.append([op,None])
                if op==5 and self.push_renderer is not None:self.pointer(self.menu+4,self.push_renderer)
                self.returned(self.valid if op==1 else self.present if op==2 else 0)
            else:super().external(uc,address,size,user)
        def operations(self,uc,address,size,user):
            if address==0x3140ec:self.debug_key=text(self,self.reg(1));self.returned()
            elif address==0x318254:self.returned()
            elif address in (0x328f40,0x33c568,0x337888,0x337a88,0x42cb8c,0x431750):
                op={0x328f40:3,0x33c568:4,0x337888:6,0x337a88:7,0x42cb8c:8,0x431750:9}[address]
                self.calls.append([op,self.debug_key if op==7 else None])
                if op==8 and self.hud_renderer is not None:self.pointer(self.menu+4,self.hud_renderer)
                self.returned(self.hud if op==8 else self.debug_value if op==7 else 0)
    class New(Cpu):
        def external(self,uc,address,size,user):
            if address==self.callback+48:
                request,response=self.reg(2),self.reg(3)
                op,res,entry,key,receiver=struct.unpack('<IIQQQ',uc.mem_read(request,32));assert res==0
                self.calls.append([op,text(self,key) if key else None])
                if op==5 and self.push_renderer is not None:self.pointer(entry+8,self.push_renderer)
                if op==8 and self.hud_renderer is not None:self.pointer(entry+8,self.hud_renderer)
                value=self.valid if op==1 else self.present if op==2 else self.debug_value if op==7 else 0
                uc.mem_write(response,struct.pack('<IIQ',value,0,self.hud if op==8 else 0))
                self.put(0,0 if op==self.reject else 1);uc.reg_write(self.pc,uc.reg_read(self.lr))
            elif self.imports.get(address)=='strcmp':
                left,right=text(self,self.reg(0)),text(self,self.reg(1));self.put(0,(left>right)-(left<right));uc.reg_write(self.pc,uc.reg_read(self.lr))
            else:super().external(uc,address,size,user)
    elf=a.snapshot/'.local-inputs/libDungeonHunter2.so'
    old=Old(elf,{'functions':[]});new=New(lib,True,{'functions':[]});new.reject=0
    old.manager=old.data+0x4000;old.menu=old.data+0x5000
    multi=old.data+0x6000;app=old.data+0x7000;debug=old.data+0x8000;canary=old.data+0x9000
    menu_vt=old.data+0xa000;multi_vt=old.data+0xb000
    old.pointer(old.menu,menu_vt);old.pointer(menu_vt+0x3c,old.callback+48)
    old.pointer(multi,multi_vt);old.pointer(multi_vt+0x40,old.callback+64);old.pointer(multi_vt+0x34,old.callback+80)
    old.pointer(old.manager+0xf4,multi);old.pointer(app+0x20,app+0x100)
    got=0x4317f8+8+old.word(0x431910)
    old.pointer(got+old.word(0x431914),canary);old.uc.mem_write(canary,W(0x1234))
    old.pointer(got+old.word(0x431918),app);old.pointer(got+old.word(0x43191c),debug)
    old.uc.hook_add(UC_HOOK_CODE,old.operations)
    state=new.data+0x4000;entry=new.data+0x5000;services=new.data+0x6000
    new.uc.mem_write(state,struct.pack('<QIIQQ',0,0,0,0x101,0x202))
    new.uc.mem_write(entry,Q(0x303,0,0));new.uc.mem_write(services,Q(0,new.callback+48))
    cases=[]
    for valid,present,renderer,hud,push_renderer,hud_renderer,debug_value in itertools.product(
        (0,1),(0,1),(0,0x1111,0x2222),(0,0x1111),(None,0x1111,0x2222),(None,0x1111,0x2222),(0,1)):
        for c in (old,new):
            c.valid=valid;c.present=present;c.hud=hud;c.push_renderer=push_renderer;c.hud_renderer=hud_renderer;c.debug_value=debug_value;c.calls=[]
        old.debug_key='';old.pointer(old.menu+4,renderer);new.pointer(entry+8,renderer)
        old.invoke(0x4317e8,[old.manager,old.menu])
        assert new.invoke('dh2_menu_manager_push_v1',[state,entry,services])==0
        assert old.calls==new.calls,(valid,present,renderer,hud,push_renderer,hud_renderer,old.calls,new.calls)
        assert old.word(old.menu+4)==struct.unpack('<Q',new.uc.mem_read(entry+8,8))[0]
        cases.append(dict(valid=valid,present=present,renderer=renderer,hud=hud,push_renderer=push_renderer,hud_renderer=hud_renderer,debug=debug_value,calls=old.calls))
    # Actual original null-entry early return reaches no service.
    old.calls=[];new.calls=[];old.invoke(0x4317e8,[old.manager,0]);assert new.invoke('dh2_menu_manager_push_v1',[state,0,0])==0 and old.calls==new.calls==[]
    # Lookup executes original strcmp/vector traversal, including duplicates.
    old_entries=[];new_entries=[];names=('menu_Options','menu_info','menu_Options')
    for i,name in enumerate(names):
        o=old.data+0xc000+128*i;n=new.data+0xc000+128*i
        old.uc.mem_write(o+8,name.encode()+b'\0');old_entries.append(o)
        key=n+40;new.uc.mem_write(key,name.encode()+b'\0');new.uc.mem_write(n,Q(i+1,0,key));new_entries.append(n)
    ob=old.data+0xd000;nb=new.data+0xd000;old.uc.mem_write(ob,W(*old_entries));new.uc.mem_write(nb,Q(*new_entries))
    old.pointer(old.manager+0x64,ob);old.pointer(old.manager+0x68,ob+12);new.uc.mem_write(state,struct.pack('<QIIQQ',nb,3,0,0x101,0x202))
    lookup=[]
    for name in ('menu_Options','menu_info','menu_OPTIONS','menu_Info','missing',''):
        os=old.data+0xe000;ns=new.data+0xe000;old.uc.mem_write(os,name.encode()+b'\0');new.uc.mem_write(ns,name.encode()+b'\0')
        op=old.invoke(0x42d1f0,[old.manager,os]);np=new.invoke('dh2_menu_manager_find_v1',[state,ns])
        oi=old_entries.index(op) if op else -1;ni=new_entries.index(np) if np else -1;assert oi==ni
        lookup.append(dict(name=name,index=oi))
    # Modern required-backend failure is an adapter contract, not an original
    # exception path: stop at rejection and preserve any prior push mutation.
    rejected=[];new.valid=1;new.present=0;new.hud=0x1111;new.push_renderer=0x2222;new.hud_renderer=None
    for op in range(1,10):
        new.reject=op;new.calls=[];new.pointer(entry+8,0x1111)
        status=new.invoke('dh2_menu_manager_push_v1',[state,entry,services])
        # RegisterListener is skipped here after the push changes renderer.
        expected=-2 if op!=9 else 0
        assert status&0xffffffff==expected&0xffffffff,(op,status,new.calls)
        if expected==-2:assert new.calls[-1][0]==op
        rejected.append(dict(rejected_operation=op,status=expected,calls=new.calls))
    report=dict(validation='PASS',scope=__doc__,original_sha256=hashlib.sha256(elf.read_bytes()).hexdigest(),
        native_sha256=hashlib.sha256(lib.read_bytes()).hexdigest(),success_cases=len(cases),cases=cases,
        lookup_cases=lookup,modern_rejection_cases=rejected,production_menu_stack_connected=False)
    (a.output/'push-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print('PASS | original ARM versus native ARM64 |',len(cases),'push call-order/live-read cases | exact lookup/first duplicate | rejection prefix')


if __name__=='__main__':main()
