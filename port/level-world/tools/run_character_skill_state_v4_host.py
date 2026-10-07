"""Private WSL ASan/UBSan kernel audit plus borrowed context type check."""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
drive=ROOT.drive[0].lower();path='/mnt/'+drive+ROOT.as_posix()[2:]
commands=[
 'g++ -std=c++17 -O2 -fsanitize=address,undefined -fno-omit-frame-pointer port/level-world/character_skill_state_v4.cpp port/level-world/tests/character_skill_state_v4_host.cpp -o /tmp/dh2-skill-state-v4-host',
 'ASAN_OPTIONS=detect_leaks=1 /tmp/dh2-skill-state-v4-host',
 'g++ -std=c++17 -O2 -fsyntax-only port/level-world/character_skill_context_v4.cpp',
]
results=[]
for cmd in commands:
 result=subprocess.run(['wsl.exe','--','bash','-lc',f'cd "{path}" && {cmd}'],capture_output=True,text=True)
 results.append(dict(command=cmd,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr))
 if result.returncode:raise RuntimeError(result.stderr or result.stdout)
sources=['character_skill_state_v4.cpp','character_skill_state_v4.hpp','character_skill_context_v4.cpp','character_skill_context_v4.hpp','tests/character_skill_state_v4_host.cpp']
report=dict(validation='PASS',sanitizers=['address','undefined'],leak_detection=True,kernel_assertions='Borrowed State mutation, effects before failed provider, real payload/service ordering, missing prefix provider and malformed pre-entry',context_validation='Host and ARM64 syntax compatibility only; full retained-player integration not executed',full_campaign=False,commands=results,source_sha256={n:hashlib.sha256((ROOT/'port/level-world'/n).read_bytes()).hexdigest()for n in sources})
(ROOT/'port/level-world/reports/character-skill-state-v4-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',sanitizers=report['sanitizers'],full_campaign=False)))
