"""Replay original source-owned font resolver corpus against actual O2 ARM64."""
import argparse,pathlib,sys,struct,json,hashlib
R=pathlib.Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def normalize(c):
 out=[]
 for row in c['services']:
  k=row[0]
  if k=='Debug.load':out.append([1,0,''])
  elif k=='Debug.GetSwitch':out.append([2,0,row[1]])
  elif k=='Savegame.getLanguage':out.append([3,row[1],''])
  elif k=='path_manager.rewrite':out.append([4,0,row[1]])
  elif k=='fopen':out.append([5,0,row[1]])
  elif k=='fclose':out.append([6,row[1],''])
 return out
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--oracle',type=pathlib.Path,required=True);ap.add_argument('--output',type=pathlib.Path,required=True);a=ap.parse_args()
 original=R/'port/level-world/reference/font-text/font-resolver-corrected/original-probe.json';source=json.loads(original.read_text());rows=source['cases']
 c=Cpu(a.oracle,True,{'functions':[]});d=c.data;inp=d+0x100;out=d+0x200;services=d+0x300;name=d+0x1000;base=d+0x3000;output=d+0x5000;callback=d+0x10000;context=0xf123456789abcdef
 def ustr(p):
  b=bytes(c.uc.mem_read(p,4096)).split(b'\0')[0];return b.decode()
 def store(p,s):c.uc.mem_write(p,s.encode()+b'\0')
 calls=[];current=None
 def handler(uc,address,size,unused):
  if address!=callback:return
  assert c.reg(0)==context
  p=c.reg(1);kind,res,text,buffer,capacity,value=struct.unpack('<IIQQQQ',c.uc.mem_read(p,40));assert res==0
  t=ustr(text) if text else ''
  if kind==3:value=current['language']
  elif kind==4:t=current['rewrite_prefix']+t;assert capacity>=len(t)+1;store(buffer,t)
  elif kind==5:value=0x1234 if current['exists'] else 0
  calls.append([kind,(current['language'] if kind==3 else value if kind==6 else 0),t])
  c.pointer(p+32,value);c.put(0,0);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 c.uc.hook_add(UC_HOOK_CODE,handler);store(base,'source-root/');c.uc.mem_write(services,struct.pack('<QQ',context,callback))
 count=0;requests=0;blob=bytearray(struct.pack('<II',0x31535246,len(rows)))
 def addstr(s):b=s.encode();blob.extend(struct.pack('<I',len(b)));blob.extend(b)
 for current in rows:
  calls.clear();store(name,current['name']);c.uc.mem_write(output,b'\xa5'*4096);c.uc.mem_write(inp,struct.pack('<QQII',name,base,current['bold'],current['italic']));c.uc.mem_write(out,struct.pack('<QQQII',output,4096,123,123,0))
  rc=c.invoke('dh2_swf_font_resolve',[out,inp,services]);ptr,capacity,n,found,res=struct.unpack('<QQQII',c.uc.mem_read(out,32))
  expected=normalize(current);assert rc==0 and found==current['return'] and ustr(output)==current['path'] and n==len(current['path'])
  assert calls==expected,(current,calls,expected);count+=1;requests+=len(calls)
  blob.extend(struct.pack('<5I',current['language'],current['bold'],current['italic'],current['exists'],bool(current['rewrite_prefix'])));addstr(current['name']);addstr(current['path']);blob.extend(struct.pack('<II',current['return'],len(expected)))
  for k,v,t in expected:blob.extend(struct.pack('<IQ',k,v));addstr(t)
 corpus=R/'port/engine-ui/reference/swf-font-resolver/source-fixtures.bin';corpus.parent.mkdir(parents=True,exist_ok=True);corpus.write_bytes(blob)
 report={'validation':'PASS','comparisons':count,'ordered_services':requests,'mismatches':0,'original_sha256':source['original_sha256'],'original_probe_sha256':sha(original),'corpus_sha256':sha(corpus),'oracle_sha256':sha(a.oracle),'script_sha256':sha(pathlib.Path(__file__)),'source_sha256':{str(p.relative_to(R)).replace('\\','/'):sha(p) for p in [R/'port/engine-ui/swf_font_resolver.hpp',R/'port/engine-ui/swf_font_resolver.cpp']},'scope':'Actual original ARM resolver golden paths and ordered explicit Debug/language/path-rewrite/file-open/close services vs optimized ARM64 source resolver; callback context uses high64bit identity. Imported libc memory only. No file-manager/backend/default font substitution or GPU parity.'}
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','comparisons':count,'ordered_services':requests}))
if __name__=='__main__':main()
