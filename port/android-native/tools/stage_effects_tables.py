"""Stage the five original-reader-proved effects streams and owned renderer."""
import hashlib
import json
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
STUDIO = Path('C:/Users/adamc/AndroidStudioProjects/dh2')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    reference = ROOT/'port/game-data/reference/effects-tables'
    proof_path = reference/'original-reader-projection.json'
    proof = json.loads(proof_path.read_text())
    assert proof['validation'] == 'PASS'
    assert proof['original_sha256'] == sha(ROOT/'.local-inputs/libDungeonHunter2.so')
    prior_path = ROOT/'port/android-native/reports/monster-backings-stage.json'
    prior = json.loads(prior_path.read_text())
    assert prior['validation'] == 'PASS' and prior['assets_per_project'] == 342
    names = ('effects_pyarray.bin','effects_pyarraynames.bin','effects_pystructnames.bin',
             'effects_dictionary_pyarraynames.bin','effects_dictionary_pyarray.bin')
    renderer = ROOT/'port/android-native/app/src/main/cpp/model_renderer.cpp'
    studio_renderer = STUDIO/'app/src/main/cpp/model_renderer.cpp'
    assert sha(studio_renderer) in (prior['renderer_sha256'],sha(renderer)), 'Studio renderer changed independently'
    files = {name: reference/name for name in names}
    for name,path in files.items():
        assert sha(path) == proof['inputs'][name]['sha256']
    projects = (ROOT/'port/android-native',STUDIO)
    roots = [project/'app/src/main/assets' for project in projects]
    for assets in roots:
        for name,digest in prior['asset_sha256'].items():
            assert sha(assets/name) == digest
        for name,source in files.items():
            dest = assets/'data'/name
            if dest.exists():
                assert dest.read_bytes() == source.read_bytes(), ('Existing effects asset differs',dest)
    for assets in roots:
        for name,source in files.items():
            dest = assets/'data'/name
            if not dest.exists():
                dest.parent.mkdir(parents=True,exist_ok=True)
                shutil.copyfile(source,dest)
    if renderer.read_bytes() != studio_renderer.read_bytes():
        shutil.copyfile(renderer,studio_renderer)
    inventories = [{p.relative_to(assets).as_posix():sha(p) for p in assets.rglob('*') if p.is_file()} for assets in roots]
    assert inventories[0] == inventories[1] and len(inventories[0]) == 347
    result = dict(validation='PASS', prior_monster_stage_sha256=sha(prior_path),
        original_reader_proof_sha256=sha(proof_path),
        asset_sha256={'data/'+name:sha(path) for name,path in files.items()},
        assets_per_project=347,renderer_sha256=sha(renderer),
        live_validation='NOT_RUN',FX_factory_ready=False,full_game_assets=False)
    output = ROOT/'port/android-native/reports/effects-tables-stage.json'
    if output.exists():
        assert json.loads(output.read_text()) == result, 'Preserve earlier effects stage report'
    else:
        output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result))


if __name__ == '__main__':
    main()
