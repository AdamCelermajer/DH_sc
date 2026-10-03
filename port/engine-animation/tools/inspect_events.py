"""Inspect candidate original event roots and authored clip metadata."""
import argparse,json,struct
from pathlib import Path
def main():
 p=argparse.ArgumentParser();p.add_argument('files',nargs='+',type=Path);a=p.parse_args()
 for file in a.files:
  raw=file.read_bytes()
  def word(o):return struct.unpack_from('<I',raw,o)[0]
  root=word(32);out={'file':str(file),'root':root,'root_prefix_words':[word(root+i*4) for i in range(24)]}
  for offset in (0x14,0x18,0x2c,0x38):
   address=word(root+offset)
   if 0<address<len(raw)-24:out[f'pointer_{offset:x}']={'offset':address,'words':[word(address+i*4) for i in range(min(16,(len(raw)-address)//4))]}
  address=word(root+0x2c)
  out['events']=[]
  if address:
   kind,components,count,times,groups_count,groups=struct.unpack_from('<6I',raw,address)
   assert components==1 and count==groups_count and kind in (1,3,4)
   width={1:1,3:2,4:4}[kind];fmt={1:'B',3:'H',4:'i'}[kind]
   for i in range(count):
    time=struct.unpack_from('<'+fmt,raw,times+width*i)[0]
    n,pointer=struct.unpack_from('<2I',raw,groups+8*i)
    names=[]
    for j in range(n):
     at=word(pointer+4*j);end=raw.index(b'\0',at);names.append(raw[at:end].decode('ascii'))
    out['events'].append({'raw_time':time,'type':kind,'names':names})
  print(json.dumps(out))
if __name__=='__main__':main()
