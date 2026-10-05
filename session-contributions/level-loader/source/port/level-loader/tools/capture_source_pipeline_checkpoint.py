"""Bind integrated source traversal, map/declaration checks and observed APK frames.

Prepared maps and retained declarations remain distinct from runtime factories,
condition evaluation, mob/chest rendering, transitions and campaign restoration.
"""
import hashlib, json, pathlib, subprocess, zipfile

root = pathlib.Path(__file__).resolve().parents[1]
worktree = root.parents[1]
assert worktree == pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
build = worktree.parent/'build'; reports = root/'reports'
def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream,'sha256').hexdigest()
receipts = {}; artifacts = {}; semantics = {}
for kind, stem in [('sources','fixed-sources'),('maps','fixed-maps'),
                   ('declarations','fixed-declarations'),('procedural','procedural-maps')]:
    pair = []
    for label, folder in [('host','host-xml'),('sanitizers','host-sanitizers')]:
        path = reports/f'source-pipeline-{stem}-{label}.json'
        report = json.loads(path.read_text()); receipts[path.name] = sha(path)
        assert not report['full_loader_verified']
        assert report['cache_sha256'] == '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
        probe_name = {'sources':'fixed_sources','maps':'fixed_map',
                      'declarations':'fixed_declarations','procedural':'procedural_map'}[kind]
        probe = build/folder/f'dh2_loader_{probe_name}_probe'
        assert report.get('probe_sha256',report.get('native_probe_sha256')) == sha(probe)
        artifacts[str(probe)] = sha(probe)
        if kind == 'sources':
            assert report['summary'] == {'rule_generation_unimplemented':29,'prepared':16,'parser_blocked':6}
            for row in report['levels']:
                if row['source_preparation'] == 'prepared':
                    assert row['result']['level_buffer_route_verified']
        if kind == 'maps':
            assert report['summary'] == {'source_blocked':35,'assembled':16}
            assert report['source_coverage_sha256'] == sha(reports/f'source-pipeline-fixed-sources-{label}.json')
        if kind == 'declarations':
            assert report['validation'] == 'PASS' and report['occurrences_compared'] == 2331
            assert report['map_coverage_sha256'] == sha(reports/f'source-pipeline-fixed-maps-{label}.json')
            for relative, digest in report['sources_sha256'].items():
                assert sha(root/relative) == digest, relative
        if kind == 'procedural':
            assert report['summary'] == {'assembled':66,'original_no_layout':3,'assembly_blocked':1}
            assert report['script_sha256'] == sha(root/'tests/procedural_map_coverage.py')
        pair.append(report['levels'])
    assert pair[0] == pair[1], kind+' host/sanitizer semantic disagreement'
    semantics[kind] = hashlib.sha256(json.dumps(pair[0],sort_keys=True).encode()).hexdigest()
assert 'CMAKE_CXX_FLAGS:STRING=-fsanitize=address,undefined' in (build/'host-sanitizers/CMakeCache.txt').read_text()
ar = pathlib.Path.home()/'AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-ar.exe'
members = {}
for folder in ('android-arm64','android-x86_64'):
    library = build/folder/'libdh2_level_loader.a'
    for member in ('fixed_sources_v1.cpp.o','cached_level_file_v1.cpp.o','level_file_walk_v1.cpp.o'):
        raw = subprocess.check_output([str(ar),'p',str(library),member],timeout=15)
        assert raw.startswith(b'\x7fELF')
        members[folder+':'+member] = hashlib.sha256(raw).hexdigest()
    artifacts[str(library)] = sha(library)
preview_path = reports/'source-pipeline-preview.json'
preview = json.loads(preview_path.read_text())
assert preview['validation'] == 'PASS' and preview['serial'] == 'emulator-5590'
assert preview['avd'] == 'DH2_Loader_API37' and preview['app_id'] == 'local.dh2.loader'
assert not preview['runtime_objects_verified'] and not preview['full_loader_verified']
assert preview['audit_source_sha256'] == sha(root/'tools/audit_source_pipeline_preview.py')
assert preview['logcat_sha256'] == sha(reports/'source-pipeline-preview-logcat.txt')
for name, digest in preview['screenshots'].items():
    assert sha(reports/name) == digest, name
assert [row['identity'] for row in preview['steps'][:3]] == ['DARKWOOD','SWAMP_02','SWAMP']
assert preview['steps'][3]['previous_map_pixels_unchanged']
apk = worktree/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert preview['apk_sha256'] == sha(apk) == json.loads((reports/'preview-apk.json').read_text())['sha256']
packaged = {}
with zipfile.ZipFile(apk) as archive:
    digest = hashlib.sha256()
    with archive.open('assets/dh2-original-cache.zip') as stream:
        while chunk := stream.read(1024*1024): digest.update(chunk)
    assert digest.hexdigest() == '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    for abi in ('arm64-v8a','x86_64'):
        for name in ('libdh2_loader_preview.so','libdh2_engine_textures.so'):
            uri = f'lib/{abi}/{name}'
            packaged[uri] = hashlib.sha256(archive.read(uri)).hexdigest()
    catalog = json.loads(archive.read('assets/loader-map-catalog.json'))
    assert len(catalog['maps']) == 51
    assert catalog['coverage_sha256'] == sha(reports/'source-pipeline-fixed-maps-host.json')
    assert catalog['procedural_coverage_sha256'] == sha(reports/'source-pipeline-procedural-maps-host.json')
sources = [p for p in root.iterdir() if p.suffix in ('.cpp','.hpp','.inc') or p.name=='CMakeLists.txt']
sources += list((root/'vendor/tinyxml').glob('*.cpp'))+list((root/'vendor/tinyxml').glob('*.h'))
sources += [root/'android/loader_preview.cpp',root/'tools/build_preview.py',
            root/'tools/audit_source_pipeline_preview.py',pathlib.Path(__file__),
            root/'tools/freeze_source_pipeline_handoff.py',
            root/'SOURCE-PIPELINE-HANDOFF.md',root/'OWNER-CONTEXT-REQUIREMENTS.md',
            root/'INTERFACE-PROPOSAL.md',
            worktree/'port/android-native/app/src/main/java/com/example/dh2/LoaderPreviewActivity.java',
            worktree/'port/android-native/app/src/main/cpp/CMakeLists.txt',
            worktree/'port/android-native/app/build.gradle.kts',
            worktree/'port/android-native/app/src/main/AndroidManifest.xml']
report = {'validation':'PASS','scope':__doc__,'source_sha256':{p.relative_to(worktree).as_posix():sha(p) for p in sources},
          'receipt_sha256':receipts,'host_sanitizer_semantic_sha256':semantics,
          'built_artifact_sha256':artifacts,'android_member_sha256':members,
          'apk_sha256':sha(apk),'packaged_binary_sha256':packaged,
          'canonical_cache_sha256':digest.hexdigest(),'preview_receipt_sha256':sha(preview_path),
          'fixed_levels_assembled':16,'fixed_declarations_compared':2331,
          'generated_runs_assembled':66,'generated_no_layout':3,'generated_dependency_blocked':1,
          'current_apk_frame_identities':['DARKWOOD','SWAMP_02','SWAMP'],
          'retained_borrow_direction_accepted':True,
          'runtime_factory_contract_agreed':False,'runtime_factory_provider_available':False,'runtime_objects_verified':False,
          'mob_and_chest_rendering_verified':False,'campaign_restoration_verified':False,
          'full_loader_verified':False}
(reports/'source-pipeline-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','apk_sha256','fixed_levels_assembled','generated_runs_assembled','full_loader_verified')}))
