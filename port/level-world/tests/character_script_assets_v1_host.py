"""Actual world DSO cache ownership against independent canonical ZIP hashes."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
ZIP=Path(r'C:\Users\adamc\Downloads\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
PREFIX='com.gameloft.android.GAND.GloftD2SS/files/'
BUILD='/home/adampalace/dh2-world-build'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    assert not args.output.exists(),'Preserve existing receipts'
    assert sha(ZIP)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    scratch=ROOT/'.local-inputs/character-script-assets-v1';scratch.mkdir(exist_ok=True)
    rows=[]
    with zipfile.ZipFile(ZIP) as z:
        for info in z.infolist():
            if not info.filename.startswith(PREFIX):continue
            uri=info.filename[len(PREFIX):]
            if not uri.startswith('data/scripts/') or not uri.endswith('.luac'):continue
            raw=z.read(info);rows.append((uri,len(raw),hashlib.sha256(raw).hexdigest()))
    assert len(rows)==219
    expected=scratch/'expected-script-sha.tsv'
    expected.write_text(''.join(f'{u}\t{n}\t{s}\n' for u,n,s in sorted(rows)),newline='\n')
    paths=[ROOT/p for p in (
      'port/level-world/character_script_assets_v1.hpp','port/level-world/character_script_assets_v1.cpp',
      'port/level-world/tests/character_script_assets_v1.cpp','port/level-world/tests/character_script_assets_v1_host.py',
      'port/level-world/CMakeLists.txt','port/asset-payloads/zip_asset_pack_v1.hpp',
      'port/asset-payloads/zip_asset_pack_v1.cpp','port/asset-payloads/sha256.hpp','port/asset-payloads/sha256.cpp',
      'port/game-data/faery_tables.hpp','port/game-data/faery_tables.cpp',
      'port/game-data/skill_tables.hpp','port/game-data/skill_tables.cpp',
      'port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp',
      'port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp',
      'port/android-native/app/src/main/cpp/model_renderer.cpp')]
    before={p.relative_to(ROOT).as_posix():sha(p) for p in paths};commands=[]
    def run(argv,allow_stderr=False):
        p=subprocess.run(['wsl.exe','--exec',*argv],capture_output=True,text=True,timeout=120)
        commands.append(dict(arguments=argv,returncode=p.returncode,stdout=p.stdout,stderr=p.stderr))
        assert p.returncode==0 and (allow_stderr or not p.stderr.strip()),commands[-1]
        return p.stdout.strip()
    run(['cmake','--build',BUILD,'--target','character_script_assets_v1_audit','-j','4'],True)
    result=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',
      BUILD+'/character_script_assets_v1_audit',linux(ZIP),linux(expected)]))
    assert result['validation']=='PASS' and result['script_files']==219 and result['guards']>=18
    compiler=json.loads(run(['cat',BUILD+'/compile_commands.json']))
    records=[r for r in compiler if r['file'].endswith(('/character_script_assets_v1.cpp','/zip_asset_pack_v1.cpp','/sha256.cpp'))]
    assert any('dh2_level_world.dir/character_script_assets_v1.cpp' in r['command'] for r in records)
    links=run(['ldd',BUILD+'/character_script_assets_v1_audit'])
    assert 'libdh2_level_world.so' in links and 'libdh2_game_data.so' in links
    result.update(source_sha256=before,original_cache_sha256=sha(ZIP),expected_sha256=sha(expected),
      compiler_records=records,commands=commands,sanitizer_findings=0,
      binary_sha256=run(['sha256sum',BUILD+'/character_script_assets_v1_audit']).split()[0],
      world_library_sha256=run(['sha256sum',BUILD+'/libdh2_level_world.so']).split()[0],
      scope={'all_original_script_bytes':True,'same_caller_skill_owner':True,'owned_faery_tables':True,
             'Android_callsite_written':True,'Android_build_verified':False,'live_Android_verified':False,
             'player_gameplay_connected':False,'new_menus_connected':False})
    assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in paths},'Sources changed during proof'
    args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k] for k in ('validation','script_files','script_bytes','authored_script_bindings','guards','checks','sanitizer_findings')}))
if __name__=='__main__':main()
