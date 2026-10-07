from pathlib import Path
import sys,json,struct,zipfile,hashlib,xml.etree.ElementTree as ET
root=Path(__file__).resolve().parents[3];out=root/'port/level-world/reference/audio-source-v34';out.mkdir(parents=True,exist_ok=True)
cache=root/'.local-inputs/audio-v34/cache';cache.mkdir(parents=True,exist_ok=True)
class Reader:
 def __init__(self,b):self.b=b;self.p=0
 def word(self):v=struct.unpack_from('<I',self.b,self.p)[0];self.p+=4;return v
 def text(self):n=self.word();v=self.b[self.p:self.p+n].decode().rstrip('\0');self.p+=n;return v
archive=Path(r'C:/Users/adamc/Downloads/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');prefix='com.gameloft.android.GAND.GloftD2SS/files/'
with zipfile.ZipFile(archive)as z:
 assets={Path(n).name:n for n in z.namelist()if n.startswith(prefix+'data/sounds/')and not n.endswith('/')}
 for name,n in assets.items():
  data=z.read(n);(cache/name).write_bytes(data)
  if name.endswith('.xml'):(out/name).write_bytes(data)
 names=Reader(z.read(prefix+'data/pydata/sounds_pyarraynames.bin'));groups=[[names.text()for _ in range(names.word())]for _ in range(5)]
 r=Reader(z.read(prefix+'data/pydata/sounds_pyarray.bin'));assert r.word()==len(groups[0])
 characters=[]
 for name in groups[0]:
  lists=[[r.word()for _ in range(r.word())]for _ in range(4)];flags=list(r.b[r.p:r.p+2]);r.p+=2;characters.append(dict(name=name,lists=lists,flags=flags))
 assert r.word()==len(groups[1]);listeners=[dict(name=name,words=[r.word()for _ in range(6)])for name in groups[1]]
 assert r.word()==len(groups[2]);buses=[dict(name=name,words=[r.word()for _ in range(2)])for name in groups[2]]
 assert r.word()==len(groups[3]);types=[dict(name=name,bus=r.text(),event=r.word())for name in groups[3]]
 assert r.word()==len(groups[4]);logical=[dict(id=i,name=name,selector=r.word(),type=r.word(),filename=r.text(),fields=[r.word()for _ in range(6)])for i,name in enumerate(groups[4])];assert r.p==len(r.b)
 xml=ET.fromstring((cache/'sounds.xml').read_bytes());sounds=[dict(n.attrib)for n in xml.findall('sounds/sound')];events=[dict(n.attrib)for n in xml.findall('events/event')]
 bylabel={s['label']:s for s in sounds};byevent={s['label']:s for s in events}
 for row in logical:
  row['type_metadata']=types[row['type']];row['xml_label_sound']=bylabel.get(row['name']);row['xml_label_event']=byevent.get(row['name']);row['logical_filename_exists']=row['filename']in assets
  if row['xml_label_sound']:row['xml_filename_exists']=row['xml_label_sound']['filename']in assets
 report=dict(archive=str(archive),characters=characters,listeners=listeners,buses=buses,types=types,logical=logical,sounds=sounds,events=events,banks=[dict(n.attrib)for n in xml.findall('banks/bank')],groups=[dict(n.attrib)for n in xml.findall('groups/group')],assets=[dict(name=name,bytes=(cache/name).stat().st_size,sha256=hashlib.sha256((cache/name).read_bytes()).hexdigest())for name in assets])
 (out/'routing-census.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(dict(logical=len(logical),label_sounds=sum(bool(r['xml_label_sound'])for r in logical),label_events=sum(bool(r['xml_label_event'])for r in logical),logical_files=sum(r['logical_filename_exists']for r in logical),xml_files=sum(s['filename']in assets for s in sounds))))
 for row in logical[:8]+[r for r in logical if any(n in r['name']for n in ['Drop','Pickup','Footstep','Slime','Zombie'])]:print(row)

