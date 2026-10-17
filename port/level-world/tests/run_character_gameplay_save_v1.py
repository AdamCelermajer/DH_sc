"""Five complete Character save/InitAll wrappers against pinned original ARM.
Allocation, blank Save construction, InitPost/InitFinal virtuals and SG_Load
are named providers; their bodies are not credited by this caller comparison.
The native SG_Load wrapper invokes its actual selected PlayerSaveLoadOwner.
"""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/level-world'
PIN=MODULE/'reference/character-gameplay-save-v1/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
NATIVE_OWNER_SCOPE='Actual embedded QuestSavegame Character words; source quest174->Save10->quest114 order. Matching loader and Save-only setter checked. Swapped/foreign/duplicate/detached fields reject before stores; invalid constructed bindings reject before publication. No production Android startup or gameplay verification.'
NAMES=['_ZN9Character24InitializePlayerSavegameEv','_ZN9Character12SG_SetPlayerEPS_','_ZN9Character10SG_SetSlotEj','_ZN9Character7SG_LoadEi','_ZN9Character7InitAllEv']
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def evidence(path):
 assert sha(path)==SHA
 with path.open('rb') as stream:
  elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};rows=[]
  for name in NAMES:
   s=symbols[name];a,n=s['st_value'],s['st_size'];segment=next(q for q in elf.iter_segments() if q['p_type']=='PT_LOAD' and q['p_vaddr']<=a<a+n<=q['p_vaddr']+q['p_filesz'])
   stream.seek(segment['p_offset']+a-segment['p_vaddr']);raw=stream.read(n)
   rows.append(dict(original_symbol=name,elf_address=hex(a),size=n,sha256=hashlib.sha256(raw).hexdigest()))
  point=symbols['_ZTV9Character']['st_value']+8
  targets=[]
  for offset,name in [(0x1c,'_ZN9Character8InitPostEv'),(0x58,'_ZN9Character9InitFinalEv')]:
   at=point+offset;segment=next(q for q in elf.iter_segments() if q['p_type']=='PT_LOAD' and q['p_vaddr']<=at<at+4<=q['p_vaddr']+q['p_filesz'])
   stream.seek(segment['p_offset']+at-segment['p_vaddr']);raw=stream.read(4);target=int.from_bytes(raw,'little');assert target==symbols[name]['st_value']
   targets.append(dict(slot=hex(offset),original_symbol=name,elf_address=hex(target),size=symbols[name]['st_size'],vtable_word=raw.hex(),scope='static dispatch target identity; body is an external provider'))
 return dict(original_sha256=SHA,functions=rows,character_vtable_address_point=hex(point),virtual_targets=targets,scope=__doc__)
def oracle(original,exe,env):
 sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
 pinned=json.loads(PIN.read_text());assert evidence(original)==pinned
 old=Cpu(original,False,pinned);A=old.data+0x1000;B=old.data+0x2000;C=old.data+0x4000;VT=old.data+0x7000
 ALLOC,CTOR,LOAD=0x310570,0x465ae0,0x465430
 old.uc.hook_del(old.coverage_hook);seen=set();records=[]
 def word(at):return int.from_bytes(old.uc.mem_read(at,4),'little')
 def ret(value):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def normalize(value):return {A:11,B:22,C:101}.get(value,value)
 # Cpu installs generic import dispatch for its external area; put observing
 # virtual fixtures in writable data instead to avoid that import range.
 INIT,POST,NEW_POST=old.data+0x8000,old.data+0x8010,old.data+0x8020
 for op in range(5):
  arguments=[0,1,0xffffffff,0x80000000,0x7fffffff] if op in (1,2,3) else [0]
  for arg in arguments:
   for present in (0,1):
    for mutation in range(4) if op in (0,4) else (0,):
     trace=[];loads=0;providers=0;stores=0;captured_save=0
     for save in (A,B):
      old.uc.mem_write(save,bytes(0x198));old.pointer(save+4,0xffffffff);old.pointer(save+0x174,77);old.pointer(save+0x114,88)
     old.pointer(C+0x14e8,B if present else 0);old.pointer(C,VT)
     old.pointer(VT+0x1c,INIT);old.pointer(VT+0x58,POST);old.pointer(VT+0x100+0x58,NEW_POST)
     def hook(uc,address,size,unused):
      nonlocal providers,loads,captured_save
      if address in old.address_owner:seen.add(address)
      if address==ALLOC:
       assert old.reg(0)==0x198 and old.reg(1)==0;providers+=1;trace.append(101);ret(A)
      elif address==CTOR:
       assert old.reg(0)==A;providers+=1;trace.append(1101)
       if mutation&1:old.pointer(C+0x14e8,B)
       ret(B if mutation&2 else A)
      elif address==LOAD:
       assert old.reg(0)==B and old.reg(1)==arg;loads+=1;captured_save=22;ret(0xaabbccdd)
      elif address==INIT:
       assert old.reg(0)==C;providers+=1;trace.append(2101)
       if mutation&1:old.pointer(C,VT+0x100)
       ret(0xffffffff)
      elif address in (POST,NEW_POST):
       assert old.reg(0)==C;providers+=1;trace.extend([3101,4001 if address==NEW_POST else 4000]);ret(0x87654321)
     h=old.uc.hook_add(UC_HOOK_CODE,hook)
     try:old.invoke(int(pinned['functions'][op]['elf_address'],0),[C,arg])
     finally:old.uc.hook_del(h)
     selected=word(C+0x14e8)
     if op==0:captured_save=11;stores=4
     elif op in (1,2,3):captured_save=22 if present else 0;stores=(3 if op==1 else 1 if op==2 else 0) if present else 0
     fields=[]
     for save in (A,B):fields.extend([normalize(word(save+0x174)),normalize(word(save+0x10)),normalize(word(save+0x114)),word(save+4)])
     expected=dict(status=0,character=101,save=captured_save,slot_identity=normalize(selected),fields=fields,providers=providers,stores=stores,loads=loads,trace=trace)
     actual=json.loads(subprocess.check_output([str(exe),str(op),str(arg),str(present),str(mutation)],env=env,text=True))
     assert actual==expected,(op,arg,present,mutation,expected,actual)
     records.append(dict(operation=op,argument=arg,present=present,mutation=mutation,matched=True))
 assert seen==set(old.address_owner),(len(seen),len(old.address_owner),set(old.address_owner)-seen)
 assert old.import_calls=={},old.import_calls
 return dict(validation='PASS',comparisons=len(records),mismatches=0,executed_pinned_words=len(seen),providers=['allocate0x198/tag0','blank Save constructor','InitPost virtual1c','fresh InitFinal virtual58','whole PlayerSavegame SG_Load'],records=records)
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--original-elf',required=True,type=Path);p.add_argument('--compiler',required=True);p.add_argument('--library',required=True,type=Path);p.add_argument('--output',required=True,type=Path);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
 a.library=a.library.resolve();build=next(parent for parent in a.library.parents if(parent/'compile_commands.json').is_file())
 sys.path.insert(0,str(MODULE/'tests'));from run_player_skill_cleanup_session_v1_host import selected_entries,actual_dependencies
 env=os.environ.copy();env['PATH']=str(Path(a.compiler).parent)+os.pathsep+env.get('PATH','');ninja=shutil.which('ninja');assert ninja
 def run(command):
  result=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+'\n'+result.stderr;return result.stdout
 commands=run([ninja,'-C',build,'-t','commands','dh2_level_world']);entries=selected_entries(build,commands)
 selected=[r for r in entries if Path(r['file']).resolve()==(MODULE/'character_gameplay_save_v1.cpp').resolve()];assert len(selected)==1,'new wrapper must be selected exactly once'
 data_lib=next(a.library.parent.rglob('libdh2_game_data.dll.a'));exe=a.output/'gameplay_save_host.exe'
 files=actual_dependencies(build,ninja,entries)|{MODULE/'character_gameplay_save_v1.hpp',MODULE/'character_gameplay_save_v1.cpp',MODULE/'tests/character_gameplay_save_v1.cpp',ROOT/'port/game-data/player_savegame_v1.hpp',Path(__file__).resolve(),PIN}
 before={p.relative_to(ROOT).as_posix():sha(p) for p in sorted(files)}
 dlls=sorted(a.library.parent.parent.rglob('*.dll'));binary_before={str(p):sha(p) for p in [a.library,data_lib,*dlls]}
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',str(MODULE/'tests/character_gameplay_save_v1.cpp'),str(a.library),str(data_lib),'-o',str(exe)]
 run(command);env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),env['PATH']])
 host=json.loads(subprocess.check_output([str(exe)],env=env,text=True));assert host['validation']=='PASS'
 arm=oracle(a.original_elf,exe,env)
 assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in sorted(files)}
 assert commands==run([ninja,'-C',build,'-t','commands','dh2_level_world'])
 assert files==actual_dependencies(build,ninja,entries)|{MODULE/'character_gameplay_save_v1.hpp',MODULE/'character_gameplay_save_v1.cpp',MODULE/'tests/character_gameplay_save_v1.cpp',ROOT/'port/game-data/player_savegame_v1.hpp',Path(__file__).resolve(),PIN}
 assert binary_before=={str(p):sha(p) for p in [a.library,data_lib,*dlls]}
 imports=re.findall(r'DLL Name: (\S+)',run([Path(a.compiler).with_name('objdump.exe'),'-p',exe]));assert 'libdh2_game_data.dll' in imports and 'libdh2_level_world.dll' in imports
 report=dict(validation='PASS',host=host,original_comparison=arm,selected_commands=selected,actual_selected_commands=commands,source_sha256=before,binary_sha256={**binary_before,str(exe):sha(exe)},source_before_after_equal=True,binary_before_after_equal=True,commands_before_after_equal=True,binary_imports=imports,compiler_command=command,scope=__doc__,native_owner_scope=NATIVE_OWNER_SCOPE,native_wired=False,android_compilation=False,live_gameplay=False)
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(dict(validation='PASS',host=host,comparisons=arm['comparisons'],executed_pinned_words=arm['executed_pinned_words'],native_wired=False)))
if __name__=='__main__':main()
