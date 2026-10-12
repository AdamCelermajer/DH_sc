"""Execute original Level.Update loading selector/envelope, and original
_LoadProcess stages0..6 up to common tail. Called engine bodies are explicit
observation fixtures; no gameplay, complete Init, cleanup/audio/menu/FX/PM/event
body is certified. Genuine profiling/memory no-ops and source stores execute ARM.
"""
from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original as Base
from unicorn import UC_HOOK_CODE
class Original(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='strlen':
   p=self.reg(0);n=0
   while uc.mem_read(p+n,1)!=b'\0':n+=1
   self.returned(n)
  elif name in ('malloc','_Znwj'):
   n=self.reg(0);p=self.heap;self.heap+=(n+15)&~15
   if n:uc.mem_write(p,bytes(n))
   self.returned(p)
  elif name in ('free','_ZdlPv','pthread_mutex_lock','pthread_mutex_unlock'):self.returned(0)
  else:super().external(uc,address,size,unused)
root=Path(__file__).resolve().parents[2];elf=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so')
c=Original(elf,{'functions':[]});level=c.data+0x4000;pm=c.data+0x5000;physics=c.data+0x6000;sound=c.data+0x7000;event=c.data+0x8000
got=0x3f69b0+c.word(0x3f7964) # verified below against executed r5 at stage0
mode='';events=[];updated=0;stage=0;stopped=False;debug_keys={};actual_got=0;actual_app=0;words={};current=False;display=False;player=c.data+0xc000;worldmap=c.data+0xd000;menu_debug=c.data+0xe000;stepnames=set()
def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def string(at):
 begin,end=c.word(at+0x14),c.word(at+0x10);return bytes(c.uc.mem_read(begin,end-begin)).decode()
def cstring(at):
 n=0
 while c.uc.mem_read(at+n,1)!=b'\0':n+=1
 return bytes(c.uc.mem_read(at,n)).decode()
def hook(uc,at,size,unused):
 global stopped,actual_got,actual_app
 if mode=='update':
  if at==0x3f6990:
   assert c.reg(0)==level;events.append('load_process');c.pointer(level+0x130,updated);ret()
  elif at==0x3f8350:events.append('main_gameplay_required');stopped=True;uc.reg_write(c.pc,0x3f8328)
 elif mode in ('stage','prefix'):
  if at==0x3f6e9c:stopped=True;uc.reg_write(c.sp,c.stack+0xe000);uc.reg_write(c.pc,c.stop)
  elif at==0x3f6a28 and mode=='prefix':stopped=True;uc.reg_write(c.sp,c.stack+0xe000);uc.reg_write(c.pc,c.stop)
  elif at==0x31f594:events.append('application_current_level');ret(level if current else 0)
  elif at==0x36e478:assert c.reg(0)==pm and c.reg(1)==0 and c.reg(2)==1;events.append('local_player:0:true');ret(player)
  elif at==0x3bba70:assert c.reg(0)==event;events.append('unlock_all_fast_travels');ret()
  elif at==0x4364f8:events.append('world_map_instance');ret(worldmap)
  elif at==0x42a660:events.append('menu_debug_instance');ret(menu_debug)
  elif at==0x42a08c:assert c.reg(0)==menu_debug;value=cstring(c.reg(1));stepnames.add(value);events.append('menu_debug_text:'+value);ret()
  elif at==0x337888:events.append('prefix_debug_load');ret()
  elif at==0x324114:events.append('prefix_debug_out');ret()
  elif at==0x330740:events.append('debug_instance');ret(c.data+0x9000)
  elif at==0x337a88:
   key=string(c.reg(1))
   if c.uc.reg_read(c.lr)==0x3f6a04:debug_keys['prefix']=key;events.append('prefix_debug:'+key);ret(int(display) if mode=='prefix' else 0)
   else:debug_keys[stage]=key;events.append('debug:'+key);ret(updated)
  elif at==0x31f55c:
   actual_got=c.uc.reg_read(71) if False else c.reg(5) # Base.reg indexes r0..rN.
   actual_app=c.reg(0);words['first']=c.word(actual_got+c.word(0x3f799c));words['second']=c.word(actual_got+c.word(0x3f79a0))
   assert c.word(words['first'])==0 and c.word(words['second'])==0
   events.append('clean_glitch');ret()
  elif at==0x369990:assert c.reg(0)==sound and c.reg(1)==500;events.append('stop_sounds:500');ret()
  elif at==0x42ca8c:events.append('menu_instance');ret(c.data+0xa000+len(events)*4)
  elif at==0x42d4bc:events.append('unload_menu:'+str(c.reg(1)));ret()
  elif at==0x496bd8:events.append('build_fx_libraries');ret()
  elif at==0x34c048:
   assert c.reg(0)==physics
   sp=c.uc.reg_read(c.sp);bounds=[c.reg(1),c.reg(2),c.reg(3),c.word(sp)];assert bounds==[0xc4fa0000,0xc4fa0000,0x44fa0000,0x44fa0000],bounds
   events.append('physical_load:'+'/'.join(hex(x) for x in bounds));ret()
  elif at==0x378fb4:assert c.reg(0)==pm and uc.mem_read(pm+0x6c9,1)==b'\1';events.append('player_manager_update');ret()
  elif at==0x310570:assert c.reg(0)==12 and c.reg(1)==0;events.append('allocate_game_event:12');ret(event)
  elif at==0x479d08:assert c.reg(0)==event;events.append('game_event_c1');ret(event)
  elif at==0x479e5c:assert c.reg(0)==event and c.word(level+0x194)==event;events.append('game_event_load');ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
cases=[]
for phase in list(range(39))+[39,0x7fffffff,0x80000000,0xffffffff]:
 for force in (0,1):
  for nextphase in (phase,38):
   c.uc.mem_write(level,bytes(0x240));c.pointer(level+0x130,phase);events.clear();updated=nextphase;mode='update';stopped=False
   c.invoke(0x3f82d8,[level,force,0,0],budget=20000);mode=''
   expected=phase==38 or force!=0;assert stopped==expected
   assert events==(['main_gameplay_required'] if expected else ['load_process'])
   assert c.word(level+0x130)==(phase if expected else nextphase)
   cases.append({'phase':phase,'force':force,'updated':nextphase,'events':list(events),'actual_state':c.word(level+0x130)})
# Read actual GOT directly from original method prologue, no guessed globals.
# ADD r5,pc,r5 at3f69ac => PC3f69b4. Literal is3f7930.
import re
asm=(Path(__file__).parent/'level-3f6990.asm').read_text()
add=re.search(r'\s+([0-9a-f]+): [0-9a-f]+\s+add\s+r5, pc, r5',asm);assert add
ldr=re.search(r'\s+[0-9a-f]+: [0-9a-f]+\s+ldr\s+r5, \[pc, #[^\]]+\].*?@ 0x([0-9a-f]+)',asm);assert ldr
got=int(add.group(1),16)+8+c.word(int(ldr.group(1),16));app=c.word(got+c.word(0x3f7a18));sound_slot=c.word(got+c.word(0x3f79a8));c.pointer(sound_slot,sound);c.pointer(app+0x40,pm);c.pointer(app+0x44,physics)
c.uc.mem_write(c.word(got+c.word(0x3f7924)),b'\0')
words={'first':c.word(got+c.word(0x3f799c)),'second':c.word(got+c.word(0x3f79a0))};stages=[]
for stage in list(range(7))+[16,19]:
 for updated in (0,1):
  c.uc.mem_write(level,bytes(0x240));c.pointer(level+0x130,stage)
  for off,v in ((0x134,9),(0x138,11),(0x13c,13)):c.pointer(level+off,v)
  c.uc.mem_write(level+0x144,b'\x7f');c.uc.mem_write(app+0xb4,b'\x7e');c.uc.mem_write(pm+0x6c9,b'\0')
  c.pointer(words['first'],0x12345678);c.pointer(words['second'],0xabcdef00)
  events.clear();mode='stage';stopped=False;c.invoke(0x3f6990,[level,0,0,0],budget=100000);mode='';assert stopped
  assert c.word(level+0x130)==stage+1
  if stage==0:assert actual_got==got and actual_app==app
  stages.append({'stage':stage,'debug_result':updated,'events':list(events),'actual_cells':[c.word(level+off) for off in (0x130,0x134,0x138,0x13c)]+[c.uc.mem_read(level+0x144,1)[0],c.uc.mem_read(app+0xb4,1)[0]],'global_words':[c.word(words[k]) for k in ('first','second')]})
prefixes=[];fasttravel_global=c.word(got+c.word(0x3f7924))
import itertools
for stage,fast,current,character,display in itertools.product((0,5,37),(0,1),(False,True),(False,True),(False,True)):
 c.uc.mem_write(level,bytes(0x240));c.pointer(level+0x130,stage);c.uc.mem_write(fasttravel_global,bytes((fast,)));c.pointer(player+0x660,event if character else 0);c.uc.mem_write(worldmap+0x12c,b'\0')
 events.clear();mode='prefix';stopped=False;c.invoke(0x3f6990,[level,0,0,0],budget=100000);mode='';assert stopped
 unlocked=bool(fast and current and character);assert ('unlock_all_fast_travels' in events)==unlocked;assert c.uc.mem_read(worldmap+0x12c,1)[0]==int(unlocked);assert ('menu_debug_instance' in events)==display;assert c.word(level+0x130)==stage
 prefixes.append({'stage':stage,'fasttravel_global_byte':fast,'current_level_present':current,'player_character_present':character,'display_loading_step_name':display,'events':list(events),'world_map_byte12c':int(unlocked)})
noops={hex(a):hex(c.word(a)) for a in (0x3136b4,0x3136b8,0x31041c)};assert all(v=='0xe12fff1e' for v in noops.values())
report={'validation':'PASS','engine_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'loading_branch_cases':len(cases),'stage_prefix_cases':len(stages),'per_call_prefix_cases':len(prefixes),'fasttravel_global_byte':hex(fasttravel_global),'actual_step_names':sorted(stepnames),'literal_noops':noops,'actual_got':hex(got),'actual_application':hex(app),'source_global_words':{k:hex(v) for k,v in words.items()},'debug_keys':debug_keys,'called_bodies_are_observation_fixtures':True,'whole_level_init_verified':False,'cases':cases,'stages':stages,'per_call_prefixes':prefixes,'scope':__doc__}
(Path(__file__).parent/'level-update-loading-v46-original.json').write_bytes((json.dumps(report,indent=2)+'\n').encode())
rows=[str(len(cases))]+[' '.join(map(str,[x['phase'],x['force'],x['updated'],x['actual_state'],int(x['events'][0]=='main_gameplay_required')])) for x in cases]
(Path(__file__).parent/'level-update-loading-v46-gold.txt').write_bytes(('\n'.join(rows)+'\n').encode())
print(json.dumps({k:v for k,v in report.items() if k not in ('scope','cases','stages','per_call_prefixes')}))
