from pathlib import Path
import shlex,subprocess,json,hashlib
r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader')
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host')
own=b.parent/'source-stage-swamp-v40';own.mkdir(exist_ok=True)
def linux(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
lines=(b/'build.ninja').read_text().splitlines()
i=next(i for i,v in enumerate(lines) if v.startswith('build CMakeFiles/dh2_loader_module_graph_source_probe.dir/') and 'canonical_module_graph_source_probe.cpp.o:' in v)
inc=shlex.split(next(v.split(' = ',1)[1] for v in lines[i:i+10] if v.startswith('  INCLUDES = ')))
inc+=['-I'+linux(r/'vendor/character-rng-integration-v5-loading/port/asset-payloads'),'-I'+linux(r/'vendor/character-rng-integration-v5-loading/port/scene-materials')]
i=next(i for i,v in enumerate(lines) if v.startswith('build dh2_loader_module_graph_source_probe:'))
libs=shlex.split(next(v.split(' = ',1)[1] for v in lines[i:i+10] if v.startswith('  LINK_LIBRARIES = ')))
libs=[linux(b/x) if x.endswith(('.a','.so')) and not x.startswith('/') else x for x in libs]
libs.insert(0,linux(b/'libdh2_loader_lifecycle_v36.a'))
commands=[['wsl.exe','-e','/usr/bin/c++','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG',*inc,'-c',linux(r/'tests/source_stage_swamp_v40.cpp'),'-o',linux(own/'probe.o')],['wsl.exe','-e','/usr/bin/c++',linux(own/'probe.o'),'-o',linux(own/'probe'),'-Wl,--gc-sections','-Wl,--no-undefined',*libs]]
for cmd in commands:
 print(' '.join(cmd),flush=True);run=subprocess.run(cmd,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);print(run.stdout,flush=True)
 if run.returncode:raise SystemExit(run.returncode)
cmd=['wsl.exe','-e',linux(own/'probe'),'/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip','/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/level-world/reference/character-game-design/real-cache-inputs.bin',linux(own)]
run=subprocess.run(cmd,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);print(run.stdout,flush=True)
report={'commands':commands+[cmd],'exit_code':run.returncode,'stdout':run.stdout,'archive_hashes':{str(p.relative_to(b)):hashlib.sha256(p.read_bytes()).hexdigest() for p in b.rglob('*.a')},'source_sha256':hashlib.sha256((r/'tests/source_stage_swamp_v40.cpp').read_bytes()).hexdigest()}
(r/'reports/source-stage-swamp-v40-build.json').write_bytes((json.dumps(report,indent=2)+'\n').encode())
raise SystemExit(run.returncode)
