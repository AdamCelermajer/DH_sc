from pathlib import Path
import sys,json,struct,random,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(root/'port/game-data/tests'));sys.path.insert(0,str(root/'port/level-world/tests'))
from items_differential import Original
engine=root/'.local-inputs/libDungeonHunter2.so';old=Original(engine,{'functions':[]});a=old.data+0x10000;b=a+0x100;out=b+0x100
rng=random.Random(39);records=[]
for i in range(160):
 matrices=[]
 for j in range(2):
  identity=i%8==j;values=[float(k in (0,5,10,15)) if identity else rng.uniform(-3,3) for k in range(16)];matrices.append(struct.pack('<16fB3x',*values,int(identity)))
 old.uc.mem_write(a,matrices[0]);old.uc.mem_write(b,matrices[1]);old.uc.mem_write(out,bytes(68));old.invoke(0x35e998,[out,a,b]);result=bytes(old.uc.mem_read(out,65));records.append(matrices[0]+matrices[1]+result)
dest=root/'port/level-world/reference/fx-alignment-v39';dest.mkdir(parents=True,exist_ok=True);blob=struct.pack('<I',len(records))+b''.join(records);(dest/'matrix-original.bin').write_bytes(blob)
report={'original_function':'Matrix4f::operator*35e998','cases':160,'defined_bytes_compared':65,'padding65_67':'not compared; native value-initialized','original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest()};(dest/'matrix-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
