"""Bind the observed private Android preview to the APK, inputs and native sources."""
import hashlib,json,pathlib,zipfile
ROOT=pathlib.Path(__file__).resolve().parents[3]
LOADER=ROOT/'port/level-loader';REPORTS=LOADER/'reports'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
map_checkpoint=json.loads((REPORTS/'map-assembly-checkpoint.json').read_text())
sources={}
for relative,expected in map_checkpoint['source_sha256'].items():
    path=ROOT/relative
    if path.suffix in ('.cpp','.hpp','.h','.cmake') or path.name=='CMakeLists.txt':
        if sha(path)!=expected:raise RuntimeError('Map source changed after assembly checkpoint: '+relative)
        sources[relative]=expected
preview=json.loads((REPORTS/'swamp-map-preview.json').read_text())
assert preview['validation']=='PASS' and not preview['full_loader_verified']
for relative,expected in preview['source_sha256'].items():
    assert sha(ROOT/relative)==expected,'Preview source changed: '+relative
    sources[relative]=expected
apk=ROOT/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert sha(apk)==preview['apk_sha256'],'APK changed after observed frames'
binary_hashes={}
with zipfile.ZipFile(apk) as z:
    digest=hashlib.sha256()
    with z.open('assets/dh2-original-cache.zip') as cache:
        while chunk:=cache.read(1024*1024):digest.update(chunk)
    assert digest.hexdigest()=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    for abi in ('arm64-v8a','x86_64'):
        for name in ('libdh2_loader_preview.so','libdh2_engine_textures.so'):
            path=f'lib/{abi}/{name}';binary_hashes[path]=hashlib.sha256(z.read(path)).hexdigest()
for folder in ('engine-textures',):
    for p in (ROOT/'port'/folder).iterdir():
        if p.is_file() and p.suffix in ('.cpp','.hpp','.h'):sources[p.relative_to(ROOT).as_posix()]=sha(p)
for name,expected in preview['screenshots'].items():assert sha(REPORTS/name)==expected,'Screenshot changed: '+name
assert sha(REPORTS/'preview-logcat.txt')==preview['logcat_sha256']
coverage_path=REPORTS/'fixed-map-preview-coverage.json'
coverage=json.loads(coverage_path.read_text())
assert coverage['apk_sha256']==preview['apk_sha256']
pixels_path=REPORTS/'fixed-map-preview-pixels.json';pixels=json.loads(pixels_path.read_text())
assert pixels['coverage_sha256']==sha(coverage_path) and pixels['validation']=='PASS'
for row in coverage['levels']:
    if row['preview']=='frame_submitted':assert sha(REPORTS/row['screenshot'])==row['screenshot_sha256']
assert coverage['audit_source_sha256']==sha(LOADER/'tools/audit_fixed_preview_coverage.py')
assert pixels['source_sha256']==sha(LOADER/'tools/verify_preview_pixels.py')
report={'scope':'Private Android map mesh preview checkpoint; not a complete level or gameplay handoff.',
        'source_sha256':sources,'apk_sha256':sha(apk),'packaged_binary_sha256':binary_hashes,
        'canonical_cache_sha256':digest.hexdigest(),'preview_receipt_sha256':sha(REPORTS/'swamp-map-preview.json'),
        'map_assembly_checkpoint_sha256':sha(REPORTS/'map-assembly-checkpoint.json'),
        'fixed_preview_coverage_sha256':sha(coverage_path),'fixed_preview_pixels_sha256':sha(pixels_path),
        'fixed_preview_summary':coverage['summary'],
        'map_mesh_render_verified':True,'complete_authored_level_verified':False,
        'runtime_factory_abi_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
(REPORTS/'map-preview-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'sources':len(sources),'packaged_binaries':len(binary_hashes),'map_mesh_render_verified':True,'full_loader_verified':False}))
