from pathlib import Path
import sys,struct,random,json,hashlib
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(REPO/'port/game-data/tests'))
from aggro_differential import Cpu
old=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(REPO/'.local-inputs/blood_render_pass_v3_arm64.so',True,{'functions':[]});rng=random.Random(0x5d7a10);records=[]
for i in range(2002):
 if i<2:
  b=(REPO/'.local-inputs/combat-hit-fx-v1'/('bloodsplat.bdae' if i==0 else 'bloodsplat_hero.bdae')).read_bytes();w=lambda a:struct.unpack_from('<I',b,a)[0];effect=w(w(32)+0x58);tech=w(effect+36);p=w(tech+8);raw=b[p+28:p+104]
 else:raw=bytes(rng.randrange(256) for _ in range(76))
 old.uc.mem_write(old.data+0x1000,raw);old.uc.mem_write(old.data+0x2000,b'\xa5'*52);old.invoke(0x5d7a10,[old.data+0x2000,old.data+0x1000]);expected=bytes(old.uc.mem_read(old.data+0x2000,32));new.uc.mem_write(new.data+0x1000,raw);new.uc.mem_write(new.data+0x2000,b'\xa5'*32);rc=new.invoke('dh2_render_pass_convert_v3',[new.data+0x2000,new.data+0x1000]);actual=bytes(new.uc.mem_read(new.data+0x2000,32));assert rc==0 and actual==expected,(i,actual.hex(),expected.hex());records.append(raw+expected)
folder=ROOT/'reference/blood-render-pass-v3';folder.mkdir(exist_ok=True);(folder/'original-fixtures.bin').write_bytes(struct.pack('<I',len(records))+b''.join(records));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'validation':'PASS','comparisons':len(records),'scope':'Whole original rich76-to-renderpass32 conversion5d7a10 versus compiled native ARM64 kernel; all32 written bytes exact, both actual bloodpassrecords included. GL API calls/driver shadow caching/GPU and global scissor/color-mask are separate owners.','original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'native_sha256':sha(REPO/'.local-inputs/blood_render_pass_v3_arm64.so'),'source_sha256':sha(ROOT/'blood_render_pass_v3.cpp'),'reference_sha256':sha(folder/'original-fixtures.bin')};(ROOT/'reports/blood-render-pass-v3-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
