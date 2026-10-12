"""Actual inverse/point and source setter-notify policy versus optimized ARM64."""
import argparse,json,random,struct,math
from pathlib import Path
from unicorn import UC_HOOK_CODE
from swf_cursor_input_differential import InputCpu,words,string,sha
from body_transform_differential import equal
UI=Path(__file__).resolve().parents[1]
def main():
 p=argparse.ArgumentParser(description=__doc__)
 for key in('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args();old=InputCpu(a.engine,False,{'functions':[]});new=InputCpu(a.library,True,{'functions':[]});trace=[]
 def hook(uc,address,size,data):
  if address==0x751d10:
   old.put(0,0 if string(old,old.reg(0))==string(old,old.reg(1))else 1)
  elif address==0x797124:pass
  elif address==0x7750e8:trace.append('need_advance')
  else:return
  uc.reg_write(old.pc,uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,hook);rng=random.Random(20261006);records=[]
 edge=[0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc00123,0x7f7fffff,0xff7fffff,1,0x80000001]
 for k in range(2600):
  raw=words(*[rng.getrandbits(32)if k%3 else edge[(k+j)%len(edge)]for j in range(8)])
  for operation in range(2):
   expected=None
   for cpu,native in[(old,False),(new,True)]:
    m=cpu.data+0x1000;out=m+0x100;point=m+0x200;cpu.uc.mem_write(m,raw[:24]);cpu.uc.mem_write(point,raw[24:]);cpu.uc.mem_write(out,b'\xa5'*24)
    if operation==0:cpu.invoke('dh2_ui_swf_inverse'if native else 0x795adc,[out,m])
    else:cpu.invoke('dh2_ui_swf_inverse_point'if native else 0x753d7c,[out,m,point]if native else[m,out,point])
    value=bytes(cpu.uc.mem_read(out,24 if operation==0 else 8))
    if native:assert equal(expected,value),(k,operation,raw.hex(),expected.hex(),value.hex())
    else:expected=value
   records.append(dict(operation=operation,input=raw.hex(),output=expected.hex()))
 names=['onEnterFrame','onKeyPress','onRelease','onDragOver','onDragOut','onPress','onReleaseOutside','onRollout','onRollover','onRollOut','onRollOver','ONPRESS','onpress','on','onEnterFrameX','ordinary','']
 for k in range(1700):
  name=names[k%len(names)];initial=bytes([k%2,(k//2)%2,(k//4)%2,0]);trace.clear()
  c=old;ch=c.data+0x3000;ss=ch+0x400;c.uc.mem_write(ch,b'\0'*0x100);c.uc.mem_write(ss,b'\0'+name.encode()+b'\0');c.uc.mem_write(ch+0x9c,initial[:2]);c.uc.mem_write(ch+0xe9,initial[2:3]);c.invoke(0x77fe60,[ch,ss,0])
  expected=bytes(c.uc.mem_read(ch+0x9c,2))+bytes(c.uc.mem_read(ch+0xe9,1))+b'\0'
  c=new;flags=c.data+0x3000;ss=flags+0x400;c.uc.mem_write(flags,initial);c.uc.mem_write(ss,name.encode()+b'\0');result=c.invoke('dh2_ui_swf_note_assignment',[flags,ss]);value=bytes(c.uc.mem_read(flags,4))
  assert value==expected and trace==(['need_advance']if result==2 else[]),(k,name,initial.hex(),expected.hex(),value.hex(),result,trace)
  records.append(dict(operation=2,input=initial.hex(),name=name,output=expected.hex(),need_advance=trace.copy()))
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_text(json.dumps(records,separators=(',',':'))+'\n')
 paths=[UI/(name+ext)for name in('swf_input_geometry','swf_input_policy')for ext in('.hpp','.cpp')]+[Path(__file__).resolve()]
 report=dict(validation='PASS',comparisons=len(records),geometry_comparisons=5200,setter_policy_comparisons=1700,mismatches=0,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),gold_sha256=sha(a.gold),source_sha256={str(x.relative_to(UI.parents[1])):sha(x)for x in paths},boundaries=['Original tu_string equality helper imported as exact byte equality','NeedAdvance remains the original recursively reached service; observer separately traverses actual weak parents'],whole_input_parity=False)
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
