from pathlib import Path
import sys,json,hashlib,struct,argparse
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
class IsPlayerCpu(Cpu):
 def external(self,uc,address,size,user):
  if self.imports.get(address)=='strstr':
   def text(p):
    b=bytearray()
    while self.uc.mem_read(p,1)!=b'\0':b+=self.uc.mem_read(p,1);p+=1
    return bytes(b)
   hay=self.reg(0);needle=self.reg(1);assert text(needle)==b'PlayerCharacter';at=text(hay).find(text(needle));self.put(0,hay+at if at>=0 else 0);self.import_calls['strstr']=self.import_calls.get('strstr',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,user)
p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();elf=root/'.local-inputs/libDungeonHunter2.so';elfsha=hashlib.sha256(elf.read_bytes()).hexdigest();assert elfsha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
c=IsPlayerCpu(elf,False,{'functions':[]});actor=c.data+0x1000;archetype=c.data+0x2000;current_type=0;calls=[]
def hook(uc,address,size,user):
 if address==0x3a3054:calls.append('GetCharType fixture');c.put(0,current_type);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for current_type in [-1,0,1,2,3,4,2147483647]:
 for text in ['', 'PlayerCharacter', 'PlayerCharacterKnight', 'xPlayerCharacter', 'playercharacter','NPC_PlayerCharacter','PlayerCharacter PlayerCharacter','PlayerCharacte','PlayerCharacter\0ignored']:
  c.uc.mem_write(archetype,text.encode()+b'\0');c.pointer(actor+0x44,archetype);calls.clear();before=c.import_calls.get('strstr',0);c.invoke(0x3a49f0,[actor]);expected=int(current_type==1 if current_type!=0 else text.split('\0')[0].startswith('PlayerCharacter'));assert c.reg(0)==expected;reached=c.import_calls.get('strstr',0)-before;assert reached==int(current_type==0)
  cases.append({'type':current_type,'archetype':text,'result':bool(expected),'strstr_calls':reached,'calls':list(calls)})
receipt={'validation':'PASS','original_elf_sha256':elfsha,'entry':'0x3a49f0','literal_address':'0x8c3170','literal':'PlayerCharacter','function_sha256':hashlib.sha256(bytes(c.uc.mem_read(0x3a49f0,76))).hexdigest(),'cases':cases,'scope':'Whole original IsPlayer body; actual GetCharType is fixture result, original imported strstr modeled with exact C-string first-match semantics. No main-world type/provider integration claimed.'};assert receipt['function_sha256']=='efb4b11a24129fac129ee1ee6460e1a8f9eef7b8648d7f9f70d3d2338eee342a';a.output.write_text(json.dumps(receipt,indent=2)+'\n');print('PASS original IsPlayer63cases; type0 alone reaches live archetype prefix; actual GetCharType domain fixtures')
