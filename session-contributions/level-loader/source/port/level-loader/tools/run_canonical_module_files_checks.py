import hashlib, json, pathlib, subprocess, zipfile
root = pathlib.Path(__file__).resolve().parents[3] if 'tools' == pathlib.Path(__file__).parent.name else pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports = root / 'port/level-loader/reports'
build = root.parent / 'build'
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
linux = lambda p: '/mnt/c/' + str(p).replace('\\', '/')[3:]
cache = pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache) == '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
fixture = build / 'module-file-fixtures.zip'
with zipfile.ZipFile(fixture, 'w') as z:
    entry = zipfile.ZipInfo('com.gameloft.android.GAND.GloftD2SS/files/player.mgp', (2026, 10, 5, 0, 0, 0))
    entry.compress_type = zipfile.ZIP_STORED
    z.writestr(entry, b'<Module><GameObject gametype="Player" name="Player"/></Module>')
def parse(output):
    lines = output.strip().splitlines()
    result = json.loads(lines[-1])
    assert result['validation'] == 'PASS' and result['module_file_checks'] == 14
    assert not result['class_construction_verified'] and not result['full_loader_verified']
    assert lines[0] == 'real_first_failure=OpenableContainer uri=data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp'
    return result, lines[0]
rows = []
for kind in ('host', 'sanitizers'):
    probe = build / ('connected-owner-' + kind) / 'loader/dh2_loader_canonical_module_files_probe'
    run = subprocess.run(['wsl.exe', '-d', 'Ubuntu', '--', 'env',
        'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1', 'UBSAN_OPTIONS=halt_on_error=1',
        linux(probe), linux(cache), linux(fixture)], capture_output=True, timeout=60)
    assert run.returncode == 0 and not run.stderr, (run.stdout, run.stderr)
    result, stop = parse(run.stdout.decode())
    rows.append({'build': kind, 'probe_sha256': sha(probe), 'result': result, 'original_source_stop': stop})
    print(json.dumps(rows[-1]), flush=True)
base = [r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe', '-s', 'emulator-5590']
def adb(args):
    avd = subprocess.check_output(base + ['emu', 'avd', 'name'], timeout=15).decode().replace('\r', '').splitlines()[0]
    assert avd == 'DH2_Loader_API37', avd
    return subprocess.check_output(base + args, timeout=60).decode(errors='replace')
cache_remote = '/data/local/tmp/dh2-loader-map-recovery/cache.zip'
assert adb(['shell', 'sha256sum', cache_remote]).split()[0] == sha(cache)
remote = '/data/local/tmp/dh2-loader-module-files-v1'
adb(['shell', 'mkdir', '-p', remote])
probe = build / 'connected-owner-android-x86_64/loader/dh2_loader_canonical_module_files_probe'
adb(['push', str(probe), remote + '/probe'])
adb(['push', str(fixture), remote + '/fixtures.zip'])
adb(['shell', 'chmod', '755', remote + '/probe'])
result, stop = parse(adb(['shell', remote + '/probe', cache_remote, remote + '/fixtures.zip']))
assert result == rows[0]['result']
rows.append({'build': 'android-x86_64', 'probe_sha256': sha(probe), 'result': result, 'original_source_stop': stop})
print(json.dumps(rows[-1]), flush=True)
arm = build / 'connected-owner-android-arm64-v8a/loader/dh2_loader_canonical_module_files_probe'
paths = [root / 'port/level-loader' / p for p in ('canonical_module_files_v1.hpp', 'canonical_module_files_v1.cpp', 'tests/canonical_module_files_probe.cpp')]
receipt = {'validation': 'PASS', 'scope': 'Actual Module LoadFile relay over retained original XML and SAME Level fields; positive Player-exclusion source fixtures only',
    'cases': rows, 'arm64_compile_sha256': sha(arm), 'cache_sha256': sha(cache), 'fixture_sha256': sha(fixture),
    'source_sha256': {p.relative_to(root).as_posix(): sha(p) for p in paths},
    'actual_module_class_verified': False, 'class_construction_verified': False, 'full_loader_verified': False}
(reports / 'canonical-module-files-checks.json').write_text(json.dumps(receipt, indent=2) + '\n', encoding='utf-8')
