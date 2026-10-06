from pathlib import Path
import re,json,hashlib,zipfile
root=Path(__file__).resolve().parents[3]
bdae=root/'.local-inputs/effect-cache-v4/spell_dh2_hurt_fire.bdae';raw=bdae.read_bytes()
uri=b'q:/data/iphone/3d/textures/FX_smoke_03.tga'
assert uri+b'\0' in raw
archive=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
with zipfile.ZipFile(archive) as z:
 names=[n for n in z.namelist() if n.lower().endswith('/data/3d/textures/fx_smoke_03.tga')]
 assert len(names)==1,names
 data=z.read(names[0])
out=root/'.local-inputs/fire_atlas_native_fixture_v5.inc'
out.write_text('static const unsigned char actual_fire_atlas[]{\n'+','.join(str(x) for x in data)+'\n};\n')
(root/'.local-inputs/fire_bdae_native_fixture_v5.inc').write_text('static const unsigned char actual_fire_bdae[]{\n'+','.join(str(x) for x in raw)+'\n};\n')
receipt={'bdae':str(bdae.relative_to(root)),'bdae_sha256':hashlib.sha256(raw).hexdigest(),'actual_uri':uri.decode(),'archive_member':names[0],'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest(),'scope':'actual material URI string + exact packaged bytes; parsed scene material identity must also be checked in native test'}
(root/'port/engine-textures/reference/texture-owner-v1/fire-atlas-v5.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt,indent=2))
