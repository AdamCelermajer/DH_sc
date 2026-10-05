"""Bind original module projection, native map assembly and visible inspection.

This checkpoint does not establish full PropertyMap serialization, runtime
factory integration, campaign transitions, persistence, or whole-level gameplay.
"""
import argparse,ast,hashlib,json,pathlib,zipfile
parser=argparse.ArgumentParser();parser.add_argument('--check-native-only',action='store_true')
parser.add_argument('--retain-audit-source',action='store_true');options=parser.parse_args()
ROOT=pathlib.Path(__file__).resolve().parents[3];LOADER=ROOT/'port/level-loader';REPORTS=LOADER/'reports';BUILD=ROOT.parent/'build'
assert ROOT==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
if options.retain_audit_source:
    report=json.loads((REPORTS/'generated-preview-coverage.json').read_text())
    source=LOADER/'tools/audit_generated_preview.py'
    assert sha(source)==report['script_sha256'],'Current source differs from executed audit'
    retained=REPORTS/'generated-preview-audit-source.py';retained.write_bytes(source.read_bytes())
    assert sha(retained)==report['script_sha256']
    print(json.dumps({'validation':'PASS','executed_audit_source_sha256':sha(retained)}))
    raise SystemExit(0)
receipts={}
def read(name):
    path=REPORTS/name;receipts[name]=sha(path);return json.loads(path.read_text())
original=read('procedural-modules-original.json');layout=read('procedural-layout-original.json')
assert original['original_execution_complete'] and original['selected_execution_complete'] and len(original['cases'])==35
assert original['layout_original_receipt_sha256']==receipts['procedural-layout-original.json']
for name,digest in original['dependency_sources_sha256'].items():assert sha(ROOT/name)==digest,name
assert not original['full_property_serialization_verified']
gold={(case['name'],run['seed']):run for case in original['cases'] for run in case['runs']}
assert len(gold)==70
for variant,name in (('host-xml','procedural-modules-host.json'),('host-sanitizers','procedural-modules-sanitizers.json')):
    report=read(name)
    assert report['validation']=='PASS' and report['native_module_projection_verified']
    assert report['runs']==70 and report['module_occurrences']==533
    assert report['original_receipt_sha256']==receipts['procedural-modules-original.json']
    assert report['probe_sha256']==sha(BUILD/variant/'dh2_loader_procedural_modules_probe')
    assert report['script_sha256']==sha(LOADER/'tests/procedural_modules_differential.py')
    assert all(row['matches'] and not row['differences'] for row in report['cases'])
map_runs={};module_comparisons=0
for variant,name in (('host-xml','procedural-map-host-coverage.json'),('host-sanitizers','procedural-map-sanitizers.json')):
    report=read(name);assert report['all_rows_attempted'] and len(report['levels'])==35
    assert report['summary']=={'assembled':66,'original_no_layout':3,'assembly_blocked':1}
    assert report['probe_sha256']==sha(BUILD/variant/'dh2_loader_procedural_map_probe')
    assert report['script_sha256']==sha(LOADER/'tests/procedural_map_coverage.py')
    assert report['cache_sha256']==original['cache_sha256']
    for row in report['levels']:
        for run in row['runs']:
            key=(row['identity'],run['seed']);expected=gold[('data/scene/'+row['definition'].lower(),run['seed'])]
            if run['status']=='assembled':
                actual=run['result'];assert actual['ownership_checks'] and actual['repeated_preparation_checked']
                assert len(actual['modules'])==len(expected['modules'])==actual['module_count']
                for native,reference in zip(actual['modules'],expected['modules']):
                    assert native=={key:reference['properties'][key] for key in ('name','position','xrefobject','dae')}
                    module_comparisons+=1
            elif run['status']=='original_no_layout':assert not expected['modules']
            else:
                assert key==('VOID_MAZE_03',1)
                assert run['reason']=='Missing authored XML: data/iphone/3d/modules/void_maze/mgp/vm011_corner_voidmaze_ne_00_01.mgp'
            if variant=='host-xml':map_runs[key]=run
for variant,name in (('host-xml','fixed-map-derived-root-regression.json'),('host-sanitizers','fixed-map-derived-root-sanitizers.json')):
    report=read(name);assert len(report['levels'])==51 and report['summary']=={'source_blocked':35,'assembled':16}
    assert report['probe_sha256']==sha(BUILD/variant/'dh2_loader_fixed_map_probe')
if options.check_native_only:
    print(json.dumps({'validation':'PASS','module_projection_runs':70,'module_occurrences':533,
                      'map_module_properties_compared':module_comparisons,'native_map_cases_assembled':66}))
    raise SystemExit(0)
declarations=read('procedural-declaration-types.json')
assert declarations['source_receipt_sha256']==receipts['procedural-map-host-coverage.json']
assert declarations['script_sha256']==sha(LOADER/'tools/summarize_generated_declarations.py')
assert declarations['type_count']==17 and declarations['selected_run_declarations']==7543
read('generated-material-binding-inputs.json')
visible=read('generated-preview-coverage.json');read('generated-picker-checks.json');window=read('visible-emulator-window.json')
assert visible['summary']=={'frame_submitted':82,'failed':4} and visible['visible_pixel_cases']==82 and len(visible['levels'])==86
assert any(row['visible'] for row in window['windows'])
apk=ROOT/'port/android-native/app/build/outputs/apk/debug/app-debug.apk';installed=read('preview-apk.json')
assert visible['apk_sha256']==sha(apk)==installed['sha256']
audit_source=REPORTS/'generated-preview-audit-source.py'
if not audit_source.is_file():audit_source=LOADER/'tools/audit_generated_preview.py'
assert visible['script_sha256']==sha(audit_source)
assert visible['logs_sha256']==sha(REPORTS/'generated-preview-logs.json')
catalog_path=ROOT/'port/android-native/app/src/main/assets/loader-map-catalog.json';catalog=json.loads(catalog_path.read_text())
assert visible['catalog_sha256']==sha(catalog_path) and len(catalog['maps'])==51
for row in visible['levels']:
    assert row['screenshot_sha256']==sha(REPORTS/row['screenshot'])
    if row['kind']=='procedural':
        expected=map_runs[(row['identity'],row['seed'])]
        assert (row['preview']=='frame_submitted')==(expected['status']=='assembled')
        if row['preview']=='frame_submitted':assert row['native']['modules']==expected['result']['module_count']
    if row['preview']=='frame_submitted':
        assert row['visible_map_pixels']
        if row['surface_non_background_pixels']<=10000:
            focused=row['focused_module'];assert focused['surface_non_background_pixels']>10000
            assert focused['screenshot_sha256']==sha(REPORTS/focused['screenshot'])
picker=json.loads((REPORTS/'generated-picker-checks.json').read_text())
assert picker['validation']=='PASS' and picker['apk_sha256']==sha(apk)
for row in picker['checks']:assert sha(REPORTS/row['screenshot'])==row['screenshot_sha256']
sources={}
sources[str(audit_source.relative_to(ROOT))]=sha(audit_source)
historical=read('procedural-layout-native-checkpoint.json')
for name in historical['sources_sha256']:
    path=ROOT/name
    if path.is_file():sources[name]=sha(path)
for path in LOADER.iterdir():
    if path.suffix in ('.cpp','.hpp','.inc') or path.name=='CMakeLists.txt':sources[str(path.relative_to(ROOT))]=sha(path)
names=['README.md','PROCEDURAL-MODULES-MILESTONE-HANDOFF.md','android/loader_preview.cpp',
       'tests/procedural_modules_original.py','tests/procedural_modules_differential.py','tests/procedural_modules_probe.cpp',
       'tests/procedural_map_coverage.py','tests/procedural_map_probe.cpp','tools/build_preview.py','tools/audit_generated_preview.py',
       'tools/audit_generated_picker.py','tools/inspect_generated_material_bindings.py','tools/summarize_generated_declarations.py',
       'tools/capture_procedural_modules_checkpoint.py','tools/start_preview_emulator.py','tools/preview_device.py',
       'tools/check_visible_preview.py','tools/inspect_preview_process.py','INTERFACE-PROPOSAL.md']
for name in names:
    path=LOADER/name;sources[str(path.relative_to(ROOT))]=sha(path)
    assert all(line==line.rstrip() for line in path.read_text().splitlines()),name
    if path.suffix=='.py':ast.parse(path.read_text())
for name in ('port/android-native/app/src/main/java/com/example/dh2/LoaderPreviewActivity.java',
             'port/android-native/app/src/main/cpp/CMakeLists.txt','port/android-native/app/build.gradle.kts',
             'port/android-native/app/src/main/assets/loader-map-catalog.json'):sources[name]=sha(ROOT/name)
binaries={};configs={}
for variant in ('host-xml','host-sanitizers','android-arm64','android-x86_64'):
    for name in ('dh2_loader_procedural_modules_probe','dh2_loader_procedural_map_probe','libdh2_level_loader.a',
                 'libdh2_loader_xml_reference.a','libdh2_loader_cache.a','libdh2_loader_scene.a','libdh2_loader_floors.a'):
        binaries[variant+'/'+name]=sha(BUILD/variant/name)
    configs[variant]=sha(BUILD/variant/'CMakeCache.txt')
with zipfile.ZipFile(apk) as archive:
    assert json.loads(archive.read('assets/loader-map-catalog.json'))==catalog
    packaged={name:hashlib.sha256(archive.read(name)).hexdigest() for name in archive.namelist() if name.endswith('/libdh2_loader_preview.so')}
    assert len(packaged)==2
report={'validation':'PASS','scope':__doc__,'sources_sha256':sources,'binaries_sha256':binaries,
        'build_configuration_sha256':configs,'receipts_sha256':receipts,'apk_sha256':sha(apk),'packaged_libraries':packaged,
        'emulator_launch_receipt_sha256':sha(BUILD/'emulator/launch.json'),
        'module_projection_runs':70,'module_occurrences_compared':533,'map_module_properties_compared':module_comparisons,
        'procedural_map_cases_assembled':66,'original_no_layout_cases':3,'dependency_blocked_cases':1,
        'android_visible_frame_cases':82,'catalog_definitions':51,'native_module_projection_verified':True,
        'focused_module_captures':sum('focused_module' in row for row in visible['levels']),
        'generated_map_assembly_verified':True,'android_procedural_rendering_verified':True,
        'full_property_serialization_verified':False,'runtime_factory_contract_agreed':False,
        'original_source_root_transform_policy_verified':False,'original_lighting_effects_verified':False,
        'complete_authored_scene_rendered':False,
        'runtime_objects_verified':False,'campaign_transitions_verified':False,'save_restoration_verified':False,
        'full_loader_verified':False}
(REPORTS/'procedural-modules-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({key:report[key] for key in ('validation','module_projection_runs','module_occurrences_compared','map_module_properties_compared','android_visible_frame_cases','catalog_definitions','focused_module_captures')}))
