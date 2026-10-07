"""Owned NPC model/property→bounds/config composition on private frozen DSOs.

Underlying arithmetic kernels have independent original instruction proofs.
No whole AssetManager/XML/save/equipment factory or full NPC AI claim.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parents[1];ROOT=HERE.parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.resolve().drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists()
 scratch=ROOT/'.local-inputs/character-npc-body';dep=scratch/'dependencies';libraries=list(dep.rglob('*.so'));assert len(libraries)==6
 directories=[linux(d) for d in sorted(set(p.parent for p in libraries))];environment=':'.join(directories)
 expected={'libdh2_level_world.so':'a519e8cd20de2c0cf41c6791a1d48ab63c6d1afc2a09c7266387110481440870','libdh2_game_data.so':'ac2e2f7c4b568db0d4bfee2ae6101aa38fef376fd6f8105739b2db76b65b056d','libdh2_script_runtime.so':'3f1886b22c2230e6ddda5c75fae248d0a710b75034eac3144489f9992b7f9945'}
 deps={p.relative_to(ROOT).as_posix():sha(p) for p in libraries};assert all(sha(p)==expected[p.name] for p in libraries if p.name in expected)
 sources=[HERE/'character_npc_body.hpp',HERE/'character_npc_body.cpp',HERE/'tests/character_npc_body.cpp',Path(__file__)];source_hashes={p.relative_to(ROOT).as_posix():sha(p) for p in sources};commands=[]
 def run(*argv):
  r=subprocess.run(['wsl','--cd',linux(ROOT),*argv],text=True,capture_output=True,timeout=120);commands.append(dict(arguments=list(argv),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 flags=['-O2','-g','-std=c++17','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-isystem',linux(ROOT/'port/physics-backend/box2d-2.0.1/Include')]
 links=[v for d in directories for v in ['-L'+d,'-Wl,-rpath,'+d]]+['-ldh2_level_world','-ldh2_game_data','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldl']
 exe=scratch/'character_npc_body_audit';fixture=HERE/'reference/character-npc-body/source-fixtures.bin';observations=HERE/'reference/character-npc-body/source-model-projections.json'
 run('g++',*flags,linux(HERE/'character_npc_body.cpp'),linux(HERE/'tests/character_npc_body.cpp'),*links,'-o',linux(exe))
 audit=json.loads(run('env','LD_LIBRARY_PATH='+environment,'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),linux(ROOT/'port/android-native/app/src/main/assets'),linux(fixture),linux(observations)))
 loaded=run('env','LD_LIBRARY_PATH='+environment,'ldd',linux(exe));assert all(linux(p) in loaded for p in libraries)
 assert source_hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in sources} and deps=={p.relative_to(ROOT).as_posix():sha(p) for p in libraries}
 assets=ROOT/'port/android-native/app/src/main/assets';inputs=[assets/'worlds/crypt01.dact']+[assets/'actors'/n for n in ['skeleton.bdae','slime_green_v2.bdae','ghost.bdae']]+list((assets/'data').glob('character_*py*.bin'))+list((assets/'data').glob('ai*py*.bin'))
 report=dict(validation='PASS',scope=__doc__,host_audit=audit,source_sha256=source_hashes,dependency_library_sha256=deps,input_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in inputs},executable_sha256=sha(exe),fixture_sha256=sha(fixture),projection_sha256=sha(observations),commands=commands,loaded_dependencies=loaded,sanitizer_diagnostics=0,whole_original_factory_parity=False,full_NPC_AI=False,APK=False,physical_ARM64=False)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
