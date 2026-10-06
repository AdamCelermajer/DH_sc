import pathlib,zipfile,json,hashlib,argparse
p=argparse.ArgumentParser();p.add_argument('--cache',type=pathlib.Path,default=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip'));a=p.parse_args()
sha=lambda data:hashlib.sha256(data).hexdigest();assert sha(a.cache.read_bytes())=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
out=pathlib.Path(__file__).parent/'scene-v3-chest-assets';out.mkdir(exist_ok=True);rows={}
with zipfile.ZipFile(a.cache) as z:
 for name in ('go_chest_swamp.bdae','go_chest_swamp_big.bdae','go_chest_swamp_rotten.bdae'):
  entries=[n for n in z.namelist() if pathlib.PurePosixPath(n).name==name];assert len(entries)==1
  data=z.read(entries[0]);(out/name).write_bytes(data);rows[name]={'cache_entry':entries[0],'sha256':sha(data),'bytes':len(data)}
(out/'manifest.json').write_text(json.dumps({'cache_sha256':sha(a.cache.read_bytes()),'entries':rows},indent=2)+'\n',encoding='utf-8')
print('Extracted and hashed three exact original chest assets.')
