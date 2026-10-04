"""Serialize captured original input fields and ordered requests for C++ replay."""
import argparse,json,struct
from pathlib import Path
def main():
 p=argparse.ArgumentParser();p.add_argument('gold',type=Path);p.add_argument('output',type=Path);a=p.parse_args();rows=json.loads(a.gold.read_text());out=bytearray(struct.pack('<4sI',b'SCI1',len(rows)))
 for record in rows:
  row=record['input'];raw=bytearray(struct.pack('<12I',*[int(row[k])for k in ['operation','index','flags','hit','labels','stop','action','accept','context','selection','mask','advance']]))
  raw.extend(struct.pack('<3fi',*row['next']));raw.extend(bytes(v for traits in row['traits']for v in traits));raw.extend(struct.pack('<48f',*(v for matrix in row['matrices']for v in matrix)))
  raw.extend(struct.pack('<9I',len(row['buttons']),*(row['buttons']+[0]*(8-len(row['buttons'])))))
  for xy,fields,enabled in row['slots']:raw.extend(struct.pack('<3fi6I',*xy,*fields,enabled))
  state=bytes.fromhex(record['state']);assert len(raw)==484 and len(state)==260
  out.extend(struct.pack('<3I',len(raw),len(state),len(record['trace'])));out.extend(raw);out.extend(state)
  for trace in record['trace']:
   t=bytes.fromhex(trace);out.extend(struct.pack('<I',len(t)));out.extend(t)
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(out);print(json.dumps({'cases':len(rows),'bytes':len(out)}))
if __name__=='__main__':main()
