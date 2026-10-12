"""Exercise retained source players using the complete canonical script cache."""
import hashlib,json,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
ZIP=Path(r'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
PREFIX='com.gameloft.android.GAND.GloftD2SS/files/'
BUILD='/home/adampalace/dh2-world-build'
OUTPUT=ROOT/'port/level-world/reports/character-player-cache-bootstrap-v1-host-audit.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
    assert not OUTPUT.exists(),'Preserve accepted receipts'
    assert sha(ZIP)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    scratch=ROOT/'.local-inputs/character-script-assets-v1';scratch.mkdir(exist_ok=True)
    cache=scratch/'bootstrap-cache';cache.mkdir(exist_ok=True)
    directory=[];scripts=[];extracted={}
    with zipfile.ZipFile(ZIP) as z:
        for item in z.infolist():
            if item.is_dir():continue
            assert item.filename.startswith(PREFIX)
            uri=item.filename[len(PREFIX):];directory.append(uri)
            wanted=uri.startswith('data/scripts/') and uri.endswith('.luac')
            wanted|=uri in {f'data/pydata/{t}{s}.bin' for t in ('skills','faeries') for s in ('_pyarray','_pyarraynames','_pystructnames')}
            if not wanted:continue
            assert '..' not in uri.split('/') and not uri.startswith('/')
            raw=z.read(item);p=cache/uri;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
            h=hashlib.sha256(raw).hexdigest();assert sha(p)==h;extracted[uri]=h
            if uri.endswith('.luac'):scripts.append((uri,len(raw),h))
    assert len(directory)==6833 and len(scripts)==219 and len(extracted)==225
    listing=scratch/'exact-directory.txt';listing.write_text(''.join(p+'\n' for p in sorted(directory)),newline='\n')
    expected=scratch/'expected-script-sha.tsv';expected.write_text(''.join(f'{u}\t{n}\t{h}\n' for u,n,h in sorted(scripts)),newline='\n')
    design=ROOT/'port/level-world/reference/character-game-design/real-cache-inputs.bin'
    paths=[ROOT/p for p in (
      'port/level-world/character_script_assets_v1.hpp','port/level-world/character_script_assets_v1.cpp',
      'port/level-world/tests/character_script_assets_v1.cpp','port/level-world/tests/character_player_cache_bootstrap_v1.cpp',
      'port/level-world/tests/character_player_cache_bootstrap_v1_host.py','port/level-world/CMakeLists.txt',
      'port/level-world/character_player_skills_v2.cpp','port/level-world/character_script_session_v2.cpp',
      'port/level-world/character_skills_session_v2.cpp','port/game-data/player_savegame_v1.cpp',
      'port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp',
      'port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp',
      'port/android-native/app/src/main/cpp/model_renderer.cpp',
      'port/level-world/reference/character-skill-session-v2/freeze-manifest.json')]
    before={p.relative_to(ROOT).as_posix():sha(p) for p in paths};commands=[]
    def run(argv,allow_stderr=False):
        p=subprocess.run(['wsl.exe','--exec',*argv],capture_output=True,text=True,timeout=120)
        commands.append(dict(arguments=argv,returncode=p.returncode,stdout=p.stdout,stderr=p.stderr))
        assert p.returncode==0 and (allow_stderr or not p.stderr.strip()),commands[-1]
        return p.stdout.strip()
    run(['cmake','--build',BUILD,'--target','character_player_cache_bootstrap_v1_audit','character_script_assets_v1_audit','-j','4'],True)
    env=['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
    resources=json.loads(run(env+[BUILD+'/character_script_assets_v1_audit',linux(ZIP),linux(expected)]))
    players=json.loads(run(env+[BUILD+'/character_player_cache_bootstrap_v1_audit',linux(design),linux(cache),linux(listing)]))
    assert resources['validation']=='PASS' and resources['script_files']==219 and resources['guards']==19
    assert players['validation']=='PASS' and players['actual_player_sessions']==3 and players['actual_skill_instances']==19
    assert not players['starting_grants_applied'] and not players['live_Android_connected']
    links=run(['ldd',BUILD+'/character_player_cache_bootstrap_v1_audit']);assert 'libdh2_level_world.so' in links
    compiler=json.loads(run(['cat',BUILD+'/compile_commands.json']))
    records=[r for r in compiler if r['file'].endswith(('/character_script_assets_v1.cpp','/character_player_cache_bootstrap_v1.cpp'))]
    assert len(records)==3
    result=dict(validation='PASS',resource_owner=resources,source_players=players,source_sha256=before,
      original_cache_sha256=sha(ZIP),design_input_sha256=sha(design),directory_sha256=sha(listing),
      expected_script_sha256=sha(expected),actual_extracted_input_sha256=extracted,compiler_records=records,
      world_library_sha256=run(['sha256sum',BUILD+'/libdh2_level_world.so']).split()[0],
      binary_sha256=run(['sha256sum',BUILD+'/character_player_cache_bootstrap_v1_audit']).split()[0],
      commands=commands,sanitizer_findings=0,
      limits=['Idle FSM is an explicit host fixture','Saved rows retain original initial zero levels; no starting grants',
              'No live Android player-skill activation, full gameplay or new menus'])
    assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in paths}
    assert extracted=={u:sha(cache/u) for u in extracted}
    OUTPUT.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',resources=resources,players=players,sanitizer_findings=0)))
if __name__=='__main__':main()
