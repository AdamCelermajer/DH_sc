"""Source-bound Swamp native geometry/navigation preparation acceptance.

This does not establish renderer, gameplay, runtime factory or complete engine
parity. Placements are compared to XML, not a per-level C++ descriptor.
"""
import argparse,hashlib,json,pathlib,subprocess,xml.etree.ElementTree as ET,zipfile
ap=argparse.ArgumentParser();ap.add_argument('--cache',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True);ap.add_argument('--out',type=pathlib.Path,required=True)
a=ap.parse_args();cache_sha=hashlib.sha256(a.cache.read_bytes()).hexdigest()
assert cache_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
with zipfile.ZipFile(a.cache) as z:
    definition=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/scene/001_swamp.mlx')
    rows=[e.attrib for e in ET.fromstring(definition) if e.attrib.get('gametype')=='Module']
result=subprocess.run([str(a.probe.resolve()),str(a.cache.resolve()),'SWAMP','001_swamp.mlx'],text=True,capture_output=True)
if result.returncode:raise RuntimeError(result.stderr)
native=json.loads(result.stdout);assert native['validation']=='PASS'
assert native['module_count']==len(rows)==9
for actual,authored in zip(native['modules'],rows):
    assert actual['name']==authored['name'] and actual['xrefobject']==authored['xrefobject']
    assert actual['position']==[float(v) for v in authored['position'].split(',')]
assert native['navigation']['floors']==16 and native['navigation']['triangles']==626
assert native['ownership_checks'] and not native['map_render_verified'] and not native['gameplay_verified']
root=pathlib.Path(__file__).resolve().parents[1]
report={'validation':'PASS','scope':__doc__,'canonical_cache_sha256':cache_sha,
    'definition_sha256':hashlib.sha256(definition).hexdigest(),
    'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in
        [root/'fixed_map_v1.cpp',root/'fixed_map_v1.hpp',root/'visual_transform_v1.cpp',root/'user_properties_v1.cpp',pathlib.Path(__file__)]},
    'native':native,'authored_module_placements_compared':len(rows)}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','modules':native['module_count'],'geometry':native['geometry_kinds'],'navigation':native['navigation'],'floor_flags':native['floor_flag_counts']}))
