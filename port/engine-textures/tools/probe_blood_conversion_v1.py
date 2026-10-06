from pathlib import Path
import sys,json,hashlib,zipfile
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
root=Path(__file__).resolve().parents[3];sys.path.insert(0,str(root/'port/engine-textures/tests'))
from differential import Cpu
original=root/'.local-inputs/libDungeonHunter2.so';manifest=json.loads((root/'port/engine-textures/original-functions.json').read_text());cpu=Cpu(original,False,manifest)
with zipfile.ZipFile(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
 raw=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/3d/textures/atlas_fx_particles_001.tga')
# Labelled 8x8 boundary fixture from actual blood PVRTC payload. Full image
# parser/native decode is independently covered by the native owner fixture.
src=cpu.data+0x10000;converted=cpu.data+0x20000;decompressed=cpu.data+0x30000
payload=raw[60:];offset=next(i for i in range(0,len(payload)-32,8) if int.from_bytes(payload[i+4:i+8],'little')&0x7ffe7ffe)
cpu.uc.mem_write(src,payload[offset:offset+32]);cpu.uc.mem_write(converted,bytes(256));cpu.uc.mem_write(decompressed,bytes(256))
result=cpu.invoke(0x5f95ac,[27,src,0,14,converted,0,8,8,0],budget=100000000)
cpu.invoke('_Z15PVRTCDecompressPKviiiPh',[src,0,8,8,decompressed],budget=100000000)
a=bytes(cpu.uc.mem_read(converted,256));b=bytes(cpu.uc.mem_read(decompressed,256));assert a==b and result and any(a)
report=dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),source_format=27,selected_format=14,source_conversion='5f95ac',source_decoder='PVRTCDecompress69f768',fixture='actual nonzero blood payload 32bytes interpreted as labelled8x8',payload_offset=offset,conversion_return=result,bytes=256,rgba_sha256=hashlib.sha256(a).hexdigest(),difference_bytes=sum(x!=y for x,y in zip(a,b)),scope='original ARM format27→14 byte-order proof; not whole GPU texture create/upload')
report['source_mip_offsets']={}
for fmt in (14,27):
 offsets=[0]
 for mip in range(10):offsets.append(offsets[-1]+cpu.invoke(0x5edbec,[fmt,512,512,1,mip,0],budget=100000))
 report['source_mip_offsets'][str(fmt)]=offsets
(root/'port/engine-textures/reference/texture-owner-v1/blood-conversion-original-oracle.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
