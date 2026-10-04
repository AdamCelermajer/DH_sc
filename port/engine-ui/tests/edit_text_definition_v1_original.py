"""Original removeHTML instruction loop and retained string assignment service."""
import argparse,hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
import text_layout_v1_original as base
from text_layout_v1_original import Original,TextCpu,words,txt
class StringCpu(TextCpu):
 def external(self,uc,a,size,u):
  if self.imports.get(a)=='strstr':
   x,y=self.reg(0),self.reg(1);at=self.machine.cstr(x).find(self.machine.cstr(y));self.put(0,0 if at<0 else x+len(self.machine.cstr(x)[:at].encode()));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,size,u)
base.TextCpu=StringCpu
class Probe(Original):
 def hook(self,uc,a,size,u):
  if not self.active:return
  if a==0x76c818:self.store(self.c.reg(0),self.cstr(self.c.reg(1)));self.ret(self.c.reg(0))
  else:super().hook(uc,a,size,u)
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists();m=Probe(ROOT/'.local-inputs/libDungeonHunter2.so',json.loads((ROOT/'port/engine-ui/reference/edit-text-connection-v1/definition-string/original-functions.json').read_text()));cases=[]
 texts=['','plain','<p>text</p>','<font>outer<b>inner</b>end</font>','<b></b>','<a>日本語Ω</a>','text>middle</bogus>','<p>trailing</p>outside']
 for size in (0,1,15,32,255,511):
  texts+=['<p>'+'x'*size+'</p>','<b><i>'+'z'*size+'</i></b>']
 for supplied in texts:
  m.next=m.c.data+0x10000;m.store(m.state,supplied);m.active=True;m.c.invoke(0x78b4d8,[m.state],budget=1000000);m.active=False;cases.append(txt(supplied)+txt(m.string(m.state)))
 data=words(0x31454454,len(cases))+b''.join(cases);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(data);print(json.dumps({'validation':'PASS','original_cases':len(cases),'gold_sha256':hashlib.sha256(data).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
