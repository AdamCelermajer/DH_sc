"""Isolated source-bound owned SkillTables to CancelSneaking adapter audit."""
import hashlib,json,subprocess
from pathlib import Path
WORLD=Path(__file__).resolve().parents[1];REPO=WORLD.parents[1];DATA=REPO/'port/game-data'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
    scratch=REPO/'.local-inputs/character-sneaking-tables';scratch.mkdir(exist_ok=True)
    sources=[WORLD/'character_sneaking_tables.cpp',WORLD/'character_sneaking_tables.hpp',WORLD/'tests/character_sneaking_tables.cpp',Path(__file__),DATA/'skill_tables.cpp',DATA/'skill_tables.hpp',DATA/'data.hpp',WORLD/'character_cancel_sneaking.cpp',WORLD/'character_cancel_sneaking.hpp',WORLD/'character_property_bindings.hpp']
    inputs=[REPO/'.local-inputs/skill-tables'/n for n in ['skills_pyarray.bin','skills_pyarraynames.bin','skills_pystructnames.bin']]
    proofs=[DATA/'reports/skill-tables-arm64-differential.json',DATA/'reference/skill-tables/skill-tables-fixtures.bin',WORLD/'reports/character-cancel-sneaking-arm64-differential.json',WORLD/'reference/character-cancel-sneaking/cancel-sneaking-fixtures.bin']
    # Existing source proofs are dependencies; this new adapter audit does not
    # invent a second ARM32 object layout or reclassify host evidence as ARM64.
    paths=sources+inputs+proofs;before={p.relative_to(REPO).as_posix():sha(p) for p in paths}
    exe=scratch/'character_sneaking_tables_host'
    compile=['wsl.exe','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation',linux(sources[0]),linux(sources[2]),linux(sources[4]),linux(sources[7]),'-o',linux(exe)]
    r=subprocess.run(compile,text=True,capture_output=True,timeout=60);assert r.returncode==0,(r.stdout,r.stderr)
    run=['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(exe),*map(linux,inputs)]
    r=subprocess.run(run,text=True,capture_output=True,timeout=60);assert r.returncode==0 and not r.stderr,(r.stdout,r.stderr)
    audit=json.loads(r.stdout);assert audit['validation']=='PASS' and audit['actual_skills']==127 and audit['pinned_after_loader_and_inputs_destroyed']
    assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths}
    report=dict(validation='PASS',host_audit=audit,source_and_input_sha256=before,executable_sha256=sha(exe),compiler_command=compile,run_command=run,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],central_DSO_rebuilt=False,adapter_original_instruction_comparison=False,full_DelBuff=False,full_skill_VM=False,scope=__doc__)
    (WORLD/'reports/character-sneaking-tables-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
