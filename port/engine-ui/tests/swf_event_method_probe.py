"""Original complete InvokeASCallback gates; object virtuals/AS execution explicit services."""
import argparse,hashlib,itertools,json
from pathlib import Path
from unicorn import UC_HOOK_CODE
from swf_event_dispatch_differential import EventCpu,words,string
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/swf-input-connection/callbacks/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 c=EventCpu(a.engine,False,manifest);d=c.data;ch=d+0x1000;parent=d+0x2000;vt=d+0x3000;weak=d+0x4000;environment=d+0x5000;name=d+0x6000
 c.pointer(vt+8,c.callback+32);c.pointer(vt+0x58,c.callback+48);c.uc.mem_write(name,b'onRelease\0');records=[];events=[];facts={}
 def callback(address):
  if address==c.callback+32:
   assert c.reg(1)==2;role='input' if c.reg(0)==ch else 'parent';events.append({'service':'is_sprite','receiver':role});c.put(0,facts['sprite' if role=='input' else 'parent_sprite'])
  else:
   assert address==c.callback+48;role='input' if c.reg(0)==ch else 'parent';events.append({'service':'environment','receiver':role});facts['environment']=role;c.put(0,environment)
 def hook(uc,address,size,unused):
  if address!=0x7bbbfc:return
  assert c.reg(1)==environment and c.reg(2)==ch and string(c,c.reg(3))==b'onRelease'
  assert bytes(uc.mem_read(uc.reg_read(c.sp),8))==bytes(8)
  events.append({'service':'actual_method_boundary','this':'input','environment':facts['environment'],'name':'onRelease','arguments':0})
  uc.mem_write(c.reg(0),bytes(20));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 c.handler=callback;c.uc.hook_add(UC_HOOK_CODE,hook)
 for present,sprite,parent_present,parent_sprite,alive in itertools.product((0,1),repeat=5):
  facts={'sprite':sprite,'parent_sprite':parent_sprite};events=[]
  for target in (ch,parent):c.uc.mem_write(target,bytes(0x80));c.pointer(target,vt);c.uc.mem_write(target+4,words(4))
  c.uc.mem_write(weak,words(2,alive));c.pointer(ch+0x3c,weak if parent_present else 0);c.pointer(ch+0x40,parent if parent_present else 0)
  result=c.invoke(0x7abe0c,[0,ch if present else 0,name,0,0]);expected=bool(present and (sprite or (parent_present and alive and parent_sprite)))
  assert result==expected
  assert bytes(c.uc.mem_read(ch+4,4))==words(4)
  records.append({'present':present,'sprite':sprite,'parent_present':parent_present,'parent_sprite':parent_sprite,'parent_alive':alive,'source_invoked':result,'services':events,'weak_parent_after':bool(int.from_bytes(c.uc.mem_read(ch+0x40,4),'little'))})
 out={'validation':'PASS','original_instructions_executed':True,'comparisons':len(records),'original_sha256':sha(a.engine),'manifest_sha256':sha(ROOT/'reference/swf-input-connection/callbacks/original-functions.json'),'script_sha256':sha(Path(__file__)),'scope':__doc__,'records':records,'whole_method_or_frame_parity':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps({k:v for k,v in out.items() if k!='records'}))
if __name__=='__main__':main()
