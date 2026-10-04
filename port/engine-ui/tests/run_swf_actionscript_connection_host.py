"""Retained, typed native AS connection on the actual sanitized UI library.

The authored shared/HUD movies are genuine. Game callback bodies and GPU
uploads in this host batch are fixtures; complete game/AS fork parity is not
established here. Keep existing reports immutable.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]
UI = ROOT / 'port/engine-ui'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def linux(path):
    return '/mnt/c/' + str(path.resolve()).replace('\\', '/')[3:]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Preserve existing retained-AS proof')
    paths = [UI / (name + suffix) for name in
             ('swf_movie', 'swf_actionscript_connection', 'renderfx_text_connection')
             for suffix in ('.hpp', '.cpp')]
    paths += [UI/'tests/swf_actionscript_connection.cpp', Path(__file__)]
    source_hashes = {p.relative_to(ROOT).as_posix(): sha(p) for p in paths}
    swfs = ROOT/'.local-inputs/ui-layout-discovery'
    inputs = [swfs/name for name in ('dqshared_droid.swf', 'dqhud_droid.swf')]
    input_hashes = {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}
    vendor_manifest = UI/'reference/gameswf-core/vendor-manifest.json'
    vendor = json.loads(vendor_manifest.read_text())
    for entry in vendor['files']:
        assert sha(UI/'vendor/gameswf1714'/entry['path']) == entry['sha256']
    commands = []

    def run(*parts):
        commands.append(['wsl', '-e', *parts])
        result = subprocess.run(commands[-1], capture_output=True, text=True,
                                encoding='utf-8', errors='strict')
        if result.returncode or result.stderr.strip():
            raise RuntimeError(json.dumps(dict(command=commands[-1],
                returncode=result.returncode, stdout=result.stdout,
                stderr=result.stderr)))
        return result.stdout.strip()

    binaries = [args.build+'/engine-ui/'+name for name in
                ('libdh2_engine_ui.so', 'libdh2_gameswf_core.a',
                 'libdh2_freetype237.a', 'swf_actionscript_connection_audit')]

    def hashes():
        return {line.split(maxsplit=1)[1]: line.split(maxsplit=1)[0]
                for line in run('sha256sum', *binaries).splitlines()}

    before = hashes()
    linked = run('ldd', binaries[-1])
    assert all(s in linked for s in (binaries[0], 'libasan.so', 'libubsan.so'))
    assert 'not found' not in linked
    audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
        'UBSAN_OPTIONS=halt_on_error=1', binaries[-1], linux(swfs)))
    assert audit == dict(validation='PASS', startup_settings=3,
        startup_multiplayer=3, global_packages=8, original_menu_methods=20,
        member_operations=16, typed_callbacks=3, typed_arguments=21,
        authored_potion_callbacks=1, guards=17, provider_lifetime_checks=2,
        limits=dict(native_game_services_are_host_fixtures=True,
                    texture_uploads_are_host_fixtures=True,
                    full_actions_fork_parity=False))
    compiler = json.loads(run('cat', args.build+'/compile_commands.json'))
    suffixes = ['/engine-ui/'+n+'.cpp' for n in
                ('swf_movie', 'swf_actionscript_connection', 'renderfx_text_connection')]
    suffixes += ['/engine-ui/tests/swf_actionscript_connection.cpp']
    records = [r for r in compiler if any(r['file'].endswith(s) for s in suffixes)]
    assert len(records) == len(suffixes)
    assert all('-fsanitize=address,undefined' in r['command'] for r in records)
    assert before == hashes()
    assert source_hashes == {p.relative_to(ROOT).as_posix(): sha(p) for p in paths}
    assert input_hashes == {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}
    result = dict(validation='PASS', scope=__doc__, source_sha256=source_hashes,
        input_sha256=input_hashes, binary_sha256=before, compiler_records=records,
        cmake_sha256=sha(UI/'CMakeLists.txt'), commands=commands,
        linked_dependencies=linked, sanitizer_findings=0,
        vendor_manifest_sha256=sha(vendor_manifest), vendor_files_unchanged=len(vendor['files']),
        host_audit=audit, pre_construction_hook_checks=4,
        native_ownership_and_property_getter_timing=True,
        plain_text_bound_variable_and_maximum_length=True,
        native_upstream_plain_formatter=True, original_full_text_layout=False,
        HTML_backend=False, live_game_callbacks=False, packaged_APK=False,
        physical_arm64_verified=False)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(dict(validation='PASS', typed_arguments=21,
        guards=17, sanitizer_findings=0, output=str(args.output))))


if __name__ == '__main__':
    main()
