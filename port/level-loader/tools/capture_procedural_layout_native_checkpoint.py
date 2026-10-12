"""Bind verified native procedural layout generation; full loader is incomplete."""
import ast,hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';build=root.parent/'build'
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
original_path=loader/'reports/procedural-layout-original.json';original=json.loads(original_path.read_text())
original_checkpoint=loader/'reports/procedural-layout-checkpoint.json'
reference=json.loads(original_checkpoint.read_text())
assert reference['validation']=='PASS' and reference['original_generate_executed']
assert original['original_execution_complete'] and len(original['cases'])==39
sources=dict(reference['sources_sha256'])
for name,want in sources.items():assert sha(root/name)==want,name
receipts={original_path.name:sha(original_path),original_checkpoint.name:sha(original_checkpoint)}
for name in reference['receipts_sha256']:
    path=loader/'reports'/name;assert sha(path)==reference['receipts_sha256'][name];receipts[name]=sha(path)
counts={'procedural-layout-native.json':78,'procedural-layout-native-facade.json':70,
        'procedural-layout-native-sanitizers.json':78}
script=loader/'tests/procedural_layout_differential.py'
for name,runs in counts.items():
    path=loader/'reports'/name;report=json.loads(path.read_text())
    assert report['validation']=='PASS' and report['native_layout_generation_verified'] and report['runs']==runs
    assert report['original_receipt_sha256']==sha(original_path) and report['cache_sha256']==original['cache_sha256']
    assert report['script_sha256']==sha(script)
    assert not report['android_procedural_rendering_verified'] and not report['full_loader_verified']
    variant='host-sanitizers' if 'sanitizers' in name else 'host-xml'
    assert report['probe_sha256']==sha(build/variant/'dh2_loader_procedural_layout_probe')
    assert len(report['cases'])==runs and all(row['matches'] and not row['differences'] for row in report['cases'])
    receipts[name]=sha(path)
# Record current dependencies; earlier map/constructor checkpoints are historical,
# not evidence that their old binaries were rerun with this added source.
for historical,key in (('map-assembly-checkpoint.json','source_sha256'),
                       ('procedural-instances-checkpoint.json','sources_sha256')):
    for name in json.loads((loader/'reports'/historical).read_text())[key]:
        path=root/name
        if path.is_file():sources[name]=sha(path)
for path in loader.iterdir():
    if path.suffix in ('.cpp','.hpp','.inc') or path.name=='CMakeLists.txt':sources[str(path.relative_to(root))]=sha(path)
for name in ('README.md','PROCEDURAL-LAYOUT-MILESTONE-HANDOFF.md','tests/procedural_layout_probe.cpp',
             'tests/procedural_layout_differential.py','tools/emit_procedural_distribution.py',
             'tools/capture_procedural_layout_native_checkpoint.py'):
    path=loader/name;sources[str(path.relative_to(root))]=sha(path)
    assert all(line==line.rstrip() for line in path.read_text().splitlines()),name
    if path.suffix=='.py':ast.parse(path.read_text())
for name in ('procedural_layout_v1.cpp','procedural_layout_v1.hpp','procedural_distribution_v1.inc'):
    assert all(line==line.rstrip() for line in (loader/name).read_text().splitlines()),name
binaries={};configs={}
for variant in ('host-xml','host-sanitizers','android-arm64','android-x86_64'):
    for name in ('dh2_loader_procedural_layout_probe','libdh2_level_loader.a','libdh2_loader_xml_reference.a',
                 'libdh2_loader_cache.a','libdh2_loader_scene.a','libdh2_loader_floors.a'):
        binaries[variant+'/'+name]=sha(build/variant/name)
    configs[variant]=sha(build/variant/'CMakeCache.txt')
report={'validation':'PASS','scope':__doc__,'sources_sha256':sources,'binaries_sha256':binaries,
        'build_configuration_sha256':configs,'receipts_sha256':receipts,
        'original_totals':reference['totals'],'native_input_runs_compared':78,'native_cache_runs_compared':70,
        'sanitizer_input_runs_compared':78,'generation_event_counters_compared':7,
        'native_layout_generation_verified':True,'repeated_generation_verified':True,
        'source_ownership_after_teardown_verified':True,'failed_candidate_retention_verified':True,
        'pool_allocation_verified':False,'original_serialization_verified':False,
        'generated_module_assembly_verified':False,'android_procedural_execution_verified':False,
        'android_procedural_rendering_verified':False,'repeated_level_load_unload_verified':False,
        'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
(loader/'reports/procedural-layout-native-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','sources':len(sources),'binaries':len(binaries),'receipts':len(receipts),
                  'native_input_runs':78,'native_cache_runs':70,'sanitizer_input_runs':78}))
