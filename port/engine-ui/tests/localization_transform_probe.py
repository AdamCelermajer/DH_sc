"""Execute original color/plain/player transformations with explicit providers."""
import json,re,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/scene-materials/tests'))
from swf_texture_resolver_probe import Cpu as BaseCpu
from unicorn import UC_HOOK_CODE
class Cpu(BaseCpu):
 def string(self,address):
  out=bytearray()
  for i in range(1048576):
   b=self.uc.mem_read(address+i,1)[0]
   if not b:return bytes(out)
   out.append(b)
  raise AssertionError('Unterminated bounded localization string')
 def external(self,uc,address,size,user):
  name=self.imports.get(address)
  if name in ('snprintf','sprintf'):
   bounded=name=='snprintf';fmt=self.string(self.reg(2 if bounded else 1));val=self.reg(3 if bounded else 2)
   if fmt==b'%c':out=bytes([val&255])
   elif fmt==b'<font color="#%06X">':out=b'<font color="#'+('%06X'%(val&0xffffffff)).encode()+b'">'
   elif b'%' not in fmt:out=fmt
   else:raise AssertionError(fmt)
   uc.mem_write(self.reg(0),out+b'\0');self.put(0,len(out));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='strchr':
   p=self.string(self.reg(0)).find(bytes([self.reg(1)&255]));self.put(0,self.reg(0)+p if p>=0 else 0);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='atoi':
   match=re.match(rb'\s*[+-]?\d+',self.string(self.reg(0)));self.put(0,int(match[0])if match else 0);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name in ('strcasecmp','strncasecmp'):
   a=self.string(self.reg(0)).lower();b=self.string(self.reg(1)).lower()
   if name=='strncasecmp':a=a[:self.reg(2)];b=b[:self.reg(2)]
   self.put(0,(a>b)-(a<b));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,user)
def main():
 c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});c.pointer(0x99f698,1);at=c.data+0x1000;out=at+0x3000;manager=out+0x200;trace=[]
 def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(uc,address,size,user):
  if address==0x4c4bdc:
   group=c.string(c.reg(1));key=c.string(c.reg(2));trace.append([group.decode(),key.decode()]);ret(int(key==b'GLOBAL_THOUSANDS_GROUP_AT')+{b'zero':0,b'one':1,b'two':2,b'three':3,b'four':4,b'five':5,b'six':6,b'seven':7,b'eight':8,b'nine':9}.get(key,0)*0x010203)
  elif address==0x508edc:
   trace.append(['string_id',c.reg(1)]);ret(at+0x2800)
 c.uc.hook_add(UC_HOOK_CODE,hook);c.uc.mem_write(at+0x2800,b'3\0')
 rows=[]
 cases=[b'',b'plain',b'a | b',b'a\nb',b'^x',b'a^',b'^n',b'^e',b'^^',b'^$x',b'$player',b'caf\xc3\xa9 !?',b'a ! b : c ; d ? e .']+[b'a^'+bytes([i])+b'z' for i in range(32,127)]
 for mode in [0,1]:
  for value in cases:
   if mode==1 and any(b'^'+bytes([x]) in value for x in b'$bcdfghikmpstuv'):continue
   c.pointer(manager+4,0xffffffff);c.uc.mem_write(at,value+b'\0');c.uc.mem_write(at+0x2000,b'\0');c.invoke(0x3140ec,[out,at+0x2000,0]);trace.clear();r=c.invoke(0x507ea4 if mode==0 else 0x508ef4,[manager,out,at]);start=struct.unpack('<I',c.uc.mem_read(out+20,4))[0];end=struct.unpack('<I',c.uc.mem_read(out+16,4))[0];text=bytes(c.uc.mem_read(start,end-start));rows.append({'mode':mode,'input':value.hex(),'output':text.hex(),'returned':r,'trace':list(trace)});c.invoke(0x3139ac,[out])
 replacements=[b'$player',b'$player $player',b'$play',b'$plaX $player',b'x$playerz',b'$',b'$pX',b'$$player',b'a $players b']
 for value in replacements:
  c.uc.mem_write(at,b'^$\0');c.uc.mem_write(at+0x400,value+b'\0');c.uc.mem_write(at+0x1000,b'$player\0');c.uc.mem_write(at+0x1200,b'Prince\0');c.invoke(0x3140ec,[out,at+0x2000,0]);trace.clear();c.invoke(0x508ef4,[manager,out,at,at+0x400,at+0x1000,at+0x1200]);start=struct.unpack('<I',c.uc.mem_read(out+20,4))[0];end=struct.unpack('<I',c.uc.mem_read(out+16,4))[0];rows.append({'mode':2,'input':value.hex(),'output':bytes(c.uc.mem_read(start,end-start)).hex(),'trace':list(trace)});c.invoke(0x3139ac,[out])
 tables=[]
 for pack in range(-1,9):c.pointer(manager+4,pack&0xffffffff);tables.append(c.invoke(0x50750c,[manager]))
 report={'validation':'PASS','original_instructions_executed':True,'format_add_space_by_pack_minus1_to8':tables,'cases':rows,'services':['original initialized allocator','libc snprintf %c and %06X','explicit FontTextColors constants','explicit three unused number formatter string defaults']};p=REPO/'.local-inputs/localization-discovery/transform-probe.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','cases':len(rows),'add_space':tables}));print([(bytes.fromhex(r['input']),bytes.fromhex(r['output']))for r in rows if r['mode']==2])
if __name__=='__main__':main()
