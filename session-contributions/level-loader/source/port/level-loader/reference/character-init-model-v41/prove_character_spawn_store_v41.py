from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'));from aggro_differential import Cpu
from unicorn import UC_HOOK_MEM_WRITE
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');ref=base/'reference/character-init-model-v41';elf=root/'.local-inputs/libDungeonHunter2.so';elfsha=hashlib.sha256(elf.read_bytes()).hexdigest();assert elfsha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80';c=Cpu(elf,False,{'functions':[]});actor=c.data+0x1000;writes=[]
def hook(uc,access,address,size,value,user):
 if address in [actor+0x270,actor+0x274]:writes.append({'pc':hex(c.uc.reg_read(c.pc)),'offset':hex(address-actor),'value':hex(value),'bytes':size})
c.uc.hook_add(UC_HOOK_MEM_WRITE,hook);cases=[]
for poison in [0,0xa5,0x7f]:
 c.uc.mem_write(actor,bytes([poison])*0x374);c.put(4,actor);c.put(5,0);writes.clear();c.uc.emu_start(0x38c488,0x38c4ac,count=30);assert c.uc.reg_read(c.pc)==0x38c4ac
 cache=struct.unpack('<i',c.uc.mem_read(actor+0x270,4))[0];prob=struct.unpack('<i',c.uc.mem_read(actor+0x274,4))[0];assert(cache,prob)==(-1,100);assert [w['pc'] for w in writes]==['0x38c494','0x38c4a8'];cases.append({'poison':poison,'cached_roll270':cache,'probability274':prob,'writes':list(writes)})
receipt={'validation':'PASS original unconditional C2 producer prefix','original_elf_sha256':elfsha,'GameObject_C2':'0x38c398','producer_prefix':['0x38c488','0x38c4ac'],'prefix_sha256':hashlib.sha256(bytes(c.uc.mem_read(0x38c488,0x24))).hexdigest(),'cases':cases,'source_register_inputs':'r4=this established38c3a4; r5=0 established38c3bc; prefix writes r7=-1 itself; does not depend on GO_ID','scope':'Exact original unconditional GameObject C2 cache270=-1 and probability274=100 stores across3poisons. Source register authority explicit. This is a producer prefix, not whole C1/Character InitPost. Existing whole GameObject C2 audit covers GO7/20; standalone GO0 whole constructor additionally reaches uninitialized external array/helper domain and is not claimed here.'}
(ref/'character-spawn-store-original-v41.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'validation':receipt['validation'],'cases':len(cases),'fields':[-1,100]}))
