"""Inspect an isolated source replay and rebuilt APK against a frozen checkpoint.

No build, installation or gameplay is performed. A separate parent-observed
build record is bound as evidence, not inferred from the presence of an APK.
"""
import argparse
from io import BytesIO
import json
from pathlib import Path
import zipfile
from validate_character_combat_source import binding, digest, read, require, sha
from validate_prince_bank_checkpoint import native_library


def artifact(path):
    return {'path': str(path.resolve()), 'sha256': digest(path), 'bytes': path.stat().st_size}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('archive', 'root', 'validation', 'rebuilt-apk', 'build-observation', 'output'):
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    require(not a.output.exists(), 'Refusing to replace rebuild validation')
    validation = read(a.validation)
    require(validation['validation'] == 'PASS', 'Checkpoint validation failed')
    checkpoint = Path(validation['checkpoint']['path'])
    require(digest(checkpoint) == validation['checkpoint']['sha256'], 'Original checkpoint changed')
    observation = read(a.build_observation)
    require(observation['exit_code'] == 0 and observation['build_successful'] is True, 'Observed build did not succeed')
    root = a.root.resolve()
    with zipfile.ZipFile(a.archive) as archive:
        require(archive.testzip() is None, 'Corrupt source archive')
        manifest = json.loads(archive.read('checkpoint-source-manifest.json'))
        require(manifest['checkpoint_sha256'] == validation['checkpoint']['sha256'], 'Source archive checkpoint differs')
        require(manifest['validation_sha256'] == digest(a.validation), 'Source archive validation differs')
        for name, entry in manifest['entries'].items():
            require(not Path(name).is_absolute() and '..' not in Path(name).parts, 'Unsafe source path')
            path = (root/name).resolve()
            require(path.is_relative_to(root), 'Source path escapes replay root')
            raw = archive.read(name)
            require(sha(raw) == entry['sha256'] and len(raw) == entry['bytes'] and path.read_bytes() == raw, 'Replayed source differs: '+name)
    raw = a.rebuilt_apk.read_bytes()
    libraries = []
    with zipfile.ZipFile(BytesIO(raw)) as rebuilt, zipfile.ZipFile(checkpoint) as original:
        require(rebuilt.testzip() is None, 'Corrupt rebuilt APK')
        assets = lambda z: {x: z.read(x) for x in z.namelist() if x.startswith('assets/') and not x.endswith('/')}
        require(assets(rebuilt) == assets(original), 'Rebuilt asset bytes differ')
        names = {x.filename for x in rebuilt.infolist() if x.filename.startswith('lib/') and not x.is_dir()}
        expected = set(validation['libraries']['packaged'])
        require(names == expected, 'Rebuilt native inventory differs')
        for name in sorted(names):
            entry = native_library(rebuilt, rebuilt.getinfo(name), raw)
            libraries.append({'path': name, **entry, 'byte_identical_to_checkpoint': rebuilt.read(name) == original.read(name)})
        count = len(assets(rebuilt))
    report = {'validation': 'PASS', 'scope': __doc__, 'source_archive': artifact(a.archive),
              'checkpoint_sha256': digest(checkpoint), 'checkpoint_validation': binding(a.validation),
              'observed_build': observation, 'build_observation': binding(a.build_observation),
              'rebuilt_apk': artifact(a.rebuilt_apk), 'source_manifest_inputs_verified': len(manifest['entries']),
              'asset_count': count, 'all_asset_bytes_match_checkpoint': True, 'libraries': libraries,
              'whole_apk_byte_identical': a.rebuilt_apk.read_bytes() == checkpoint.read_bytes(),
              'rebuilt_apk_gameplay_independently_verified': False, 'physical_arm64_verified': False,
              'validator_sha256': digest(Path(__file__))}
    a.output.parent.mkdir(parents=True, exist_ok=True)
    with a.output.open('x', encoding='utf-8', newline='\n') as f:
        f.write(json.dumps(report, indent=2)+'\n')
    print(json.dumps({k: report[k] for k in ('validation', 'source_manifest_inputs_verified', 'asset_count', 'all_asset_bytes_match_checkpoint', 'whole_apk_byte_identical')}))


if __name__ == '__main__':
    main()
