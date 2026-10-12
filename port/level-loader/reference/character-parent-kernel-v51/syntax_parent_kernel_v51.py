from pathlib import Path
import subprocess
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-parent-kernel-v51';v=b/'vendor/character-rng-integration-v5-loading/port'
p=r/'parent_kernel_compile_v51.cpp';p.write_text('#include "character_parent_init_post_v51.hpp"\n#include "condition_data_kernel_v51.hpp"\n#include "source_device_pipeline_v51.hpp"\nvoid instantiate(dh2::world::GameObjectInitializationFieldsV51& f,dh2::world::ConditionDataInitServicesV3& s,std::string& e){dh2::world::condition_data_init_borrow_v51(f,0x8c,s,e);dh2::world::condition_data_clear_borrow_v51(f,0x8c,s,e);}\n')
w=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:];dirs=[r,v/'level-world',v/'level-loader',b]+[x for x in v.iterdir() if x.is_dir() and x.name not in ['level-world','level-loader']]
cmd=['wsl.exe','-d','Ubuntu','--','c++','-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsyntax-only']+['-I'+w(x) for x in dirs]+[w(r/'game_object_initialization_owner_v1.cpp'),w(r/'condition_data_init_v3.cpp'),w(p)]
q=subprocess.run(cmd,capture_output=True,text=True);(b/'reports/character-parent-kernel-v51-syntax.log').write_text(q.stdout+q.stderr);print((q.stdout+q.stderr)[-10000:],flush=True);assert q.returncode==0;print('PASS shared parent/ConditionData kernel, generic wrappers and checked Character bridge syntax; no archive link')
(r/'syntax_parent_kernel_v51.py').write_bytes(Path(__file__).read_bytes())
