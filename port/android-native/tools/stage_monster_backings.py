"""Stage exact proved Crypt monster banks, skills and CAI1 into both projects.

All existing assets must match; this never replaces differing user files.
Renderer synchronization requires the known preceding source hash.
"""
import hashlib,json,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
STUDIO=Path('C:/Users/adamc/AndroidStudioProjects/dh2')
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    staging=ROOT/'.local-inputs/character-monster-bank-assets'
    proof_path=staging/'producer-report.json';proof=json.loads(proof_path.read_text())
    assert proof['validation']=='PASS' and proof['bank_count']==4 and proof['unique_animation_files']==64
    for path,digest in proof['source_sha256'].items():assert sha(ROOT/path)==digest
    files={}
    for name,record in proof['output_sha256'].items():
        path=staging/name
        assert path.stat().st_size==record['bytes'] and sha(path)==record['sha256']
        assert name.startswith('assets/')
        files[name[7:]]=path
    skills=json.loads((ROOT/'port/game-data/reports/skill-tables-arm64-differential.json').read_text())
    for name,digest in skills['input_sha256'].items():
        path=ROOT/'.local-inputs/skill-tables'/name;assert sha(path)==digest;files['data/'+name]=path
    init=ROOT/'port/level-world/reference/actor-initialization/crypt01-actor-initialization.bin'
    manifest=json.loads(init.with_suffix('.json').read_text());assert sha(init)==manifest['binary_sha256']
    files['worlds/'+init.name]=init
    projects=[ROOT/'port/android-native',STUDIO]
    copies=[]
    for project in projects:
        assets=project/'app/src/main/assets'
        assert sha(assets/'worlds/crypt01.dact')==manifest['descriptor_sha256']
        for name,source in files.items():
            dest=assets/name
            if dest.exists():assert dest.read_bytes()==source.read_bytes(),('Existing asset differs',dest)
            else:copies.append((source,dest))
    renderer=ROOT/'port/android-native/app/src/main/cpp/model_renderer.cpp'
    studio_renderer=STUDIO/'app/src/main/cpp/model_renderer.cpp'
    assert sha(studio_renderer) in ('74c0a8b454d4cb8b26e8421ce475a320746197abfc3abd6b44c3e0caa47f9694',sha(renderer)),'Studio renderer changed independently'
    for source,dest in copies:dest.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,dest)
    if renderer.read_bytes()!=studio_renderer.read_bytes():shutil.copyfile(renderer,studio_renderer)
    for project in projects:
        for name,source in files.items():assert sha(project/'app/src/main/assets'/name)==sha(source)
    report=dict(validation='PASS',producer_sha256=sha(proof_path),asset_sha256={name:sha(path) for name,path in files.items()},
                assets_per_project=sum(p.is_file() for p in (projects[0]/'app/src/main/assets').rglob('*')),
                renderer_sha256=sha(renderer),live_validation='NOT_RUN',full_game_assets=False)
    output=ROOT/'port/android-native/reports/monster-backings-stage.json'
    if output.exists():assert json.loads(output.read_text())==report,'Preserve earlier stage report'
    else:output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='asset_sha256'}))
if __name__=='__main__':main()
