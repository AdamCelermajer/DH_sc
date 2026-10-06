from pathlib import Path
import zipfile,hashlib,json
root=Path(__file__).resolve().parents[3]
archive=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
with zipfile.ZipFile(archive) as z:
 name='com.gameloft.android.GAND.GloftD2SS/files/data/3d/textures/atlas_fx_particles_001.tga'
 raw=z.read(name)
assert hashlib.sha256(raw).hexdigest()=='2256f485a94b00e8a8e5cbf93f0ff2038eff8d289ad73386683459ce01767b1b'
(root/'.local-inputs/blood_atlas_native_fixture_v1.inc').write_text('static const unsigned char actual_blood_atlas[]={\n'+','.join(str(b) for b in raw)+'\n};\n')
(root/'port/engine-textures/reference/texture-owner-v1/actual-atlas.json').write_text(json.dumps(dict(member=name,size=len(raw),sha256=hashlib.sha256(raw).hexdigest(),purpose='actual-cache parser/decode and source-owned texture lifecycle fixture; not GPU execution'),indent=2)+'\n')
