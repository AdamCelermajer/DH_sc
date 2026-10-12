import collections,json,pathlib,zipfile
ROOT=pathlib.Path(__file__).resolve().parents[3]
report=json.loads((ROOT/'port/level-loader/reports/canonical-level-inventory.json').read_text())
print('inventory keys',list(report))
with zipfile.ZipFile(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    templates=[n[len(prefix):] for n in z.namelist() if n.startswith(prefix) and any(v in n.lower() for v in ('template','propertymap','gameobjects'))]
    print(json.dumps({'template_candidates':templates[:80]}))
    for n in ('data/scene/001_swamp.mlx','data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp'):
        if prefix+n in z.namelist():print(n+'\n'+z.read(prefix+n).decode(errors='replace')[:9000])
