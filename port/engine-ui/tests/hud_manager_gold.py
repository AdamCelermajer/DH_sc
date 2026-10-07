"""Deterministic original manager/backend JSON to bounded host gold writer."""
import argparse,json,struct
from pathlib import Path
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def text(s):b=s.encode();return words(len(b))+b
def convert_manager(source,destination,reentry=False):
 rows=json.loads(source.read_text())['rows'];b=bytearray(words(0x31525548 if reentry else 0x314d5548,len(rows)))
 for row in rows:
  if reentry:b+=words(row['trigger'])
  b+=words(*row['input'],*row['after'],len(row['calls']))
  for op,i,v,o,p,fx,t,payload in row['calls']:
   e=words(op,i,v,o)+struct.pack('<QQ',p,fx)+text(t)
   if payload is None:e+=words(0)
   elif op==38:e+=words(2,payload[0])+text(payload[1])+words(*payload[2:])
   else:e+=words(1,*payload)
   b+=words(len(e))+e
 destination.parent.mkdir(parents=True,exist_ok=True);destination.write_bytes(b)
def convert_reentry(source,destination):convert_manager(source,destination,True)
def convert_backends(source,destination):
 rows=json.loads(source.read_text())['rows'];b=bytearray(words(0x31425548,len(rows)));kinds={'query':0,'potion':1,'level':2,'slot':3,'cooldown':4}
 for row in rows:
  cfg=row['input'];cfg=[cfg[0],*cfg[1],*cfg[2]] if row['kind']=='slot' else cfg;calls=row.get('calls',[])
  b+=words(kinds[row['kind']],len(cfg),*cfg,row['after'],len(calls))
  for call in calls:b+=words(*call)
 destination.parent.mkdir(parents=True,exist_ok=True);destination.write_bytes(b)
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('kind',choices=['manager','backends','reentry']);p.add_argument('source',type=Path);p.add_argument('destination',type=Path);a=p.parse_args();globals()['convert_'+a.kind](a.source,a.destination)
