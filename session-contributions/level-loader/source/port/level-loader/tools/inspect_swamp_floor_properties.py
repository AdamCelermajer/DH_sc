"""Read raw source user-property strings; does not implement floor semantics."""
import argparse,hashlib,json,re,zipfile
ap=argparse.ArgumentParser();ap.add_argument('--cache',required=True);a=ap.parse_args()
with zipfile.ZipFile(a.cache) as z:
    raw=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/3d/modules/swamp/swamp.bdae')
print(json.dumps({'asset_sha256':hashlib.sha256(raw).hexdigest(),
    'strings':[m.group().decode('ascii') for m in re.finditer(rb'[\x20-\x7e\r\n\t]+',raw)
               if b'floortypes' in m.group()]},indent=2))
