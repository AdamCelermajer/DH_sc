"""Serialize original-derived frame/drag records for independent C++ replay."""
import argparse,json,struct
from pathlib import Path
def main():
 p=argparse.ArgumentParser();p.add_argument('--frames',type=Path,required=True);p.add_argument('--drag',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 frames=json.loads(a.frames.read_text());drag=json.loads(a.drag.read_text());out=bytearray(struct.pack('<4sII',b'SFG1',len(frames),len(drag)))
 def u(*v):out.extend(struct.pack('<'+'I'*len(v),*v))
 for record in frames:
  row=record['input'];u(row['mode']=='sprite',row['mutation'])
  if row['mode']=='root':
   r,f,g,loaded,dt,catchup=row['fields'];out.extend(struct.pack('<3fIfI',r,f,g,loaded,dt,catchup))
  else:
   out.extend(struct.pack('<6i3IfI',*row['fields'],row['frames'],row['requeue'],row['children'],row['delta'],len(row['goto'])));u(*row['goto'])
  state=bytes.fromhex(record['state']);u(len(state));out.extend(state);u(len(record['trace']))
  for trace in record['trace']:
   raw=bytes.fromhex(trace);u(len(raw));out.extend(raw)
 for record in drag:out.extend(bytes.fromhex(record['input']));out.extend(bytes.fromhex(record['state']))
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(out)
 print(json.dumps({'frame_cases':len(frames),'drag_cases':len(drag),'bytes':len(out)}))
if __name__=='__main__':main()
