from pathlib import Path
import shlex,subprocess,json,hashlib,sys
r=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');b=r.parents[2]/'build/receiver-transport-v5-host';own=b.parent/'level-update-loading-v46';own.mkdir(exist_ok=True)
def linux(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
lines=(b/'build.ninja').read_text().splitlines();i=next(i for i,v in enumerate(lines) if v.startswith('build CMakeFiles/dh2_loader_module_graph_source_probe.dir/') and 'canonical_module_graph_source_probe.cpp.o:' in v)
inc=shlex.split(next(v.split(' = ',1)[1] for v in lines[i:i+10] if v.startswith('  INCLUDES = ')))
inc+=['-I'+linux(r/'vendor/character-rng-integration-v5-loading/port/asset-payloads'),'-I'+linux(r/'vendor/character-rng-integration-v5-loading/port/scene-materials')]
i=next(i for i,v in enumerate(lines) if v.startswith('build dh2_loader_module_graph_source_probe:'));libs=shlex.split(next(v.split(' = ',1)[1] for v in lines[i:i+10] if v.startswith('  LINK_LIBRARIES = ')))
libs=[linux(b/x) if x.endswith(('.a','.so')) and not x.startswith('/') else x for x in libs]
libs=[linux(b/'libdh2_loader_source_loading_v43.a'),linux(b/'libdh2_loader_lifecycle_v36.a'),*libs]
commands=[];runs=[]
def run(cmd):
 commands.append(cmd);print(' '.join(cmd),flush=True);p=subprocess.run(cmd,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);print(p.stdout,flush=True);runs.append({'command':cmd,'exit_code':p.returncode,'stdout':p.stdout})
 if p.returncode:save();raise SystemExit(p.returncode)
def save():
 archives={str(Path('/'+x).as_posix()):hashlib.sha256(Path('C:/'+x[7:]).read_bytes()).hexdigest() for x in libs if x.startswith('/mnt/c/') and x.endswith(('.a','.so'))}
 record={'commands':commands,'runs':runs,'archive_hashes':archives,'source_hashes':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [r/'level_update_loading_v46.hpp',r/'level_source_loading_update_v46.hpp',r/'stage_loader_v46_early.hpp',r/'stage_loader_v46_physical.hpp',r/'tests/level_update_loading_v46.cpp',r/'tests/level_update_loading_v46_real_c1.cpp']},'no_common_graph_mutation':True,'coherent_graph_dependency_scope':'Parent confirms C1/layout/GS/physics libraries unchanged since READY744; qualifiedOpenable745 compiled separately. Exact linked hashes recorded; no whole graph READY claim.'}
 (r/'reports/level-update-loading-v46-build.json').write_bytes((json.dumps(record,indent=2)+'\n').encode())
flags=['-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG']
physical=r/'vendor/character-rng-integration-v5-loading/port/level-world/physical_world.cpp';box=b/'openable-box2d/libdh2_box2d_201.a'
if '--real-only' in sys.argv:
 prior=json.loads((r/'reports/level-update-loading-v46-build.json').read_text())
 runs.extend(x for x in prior['runs'] if x['exit_code']==0 and any('unit-' in v for v in x['command']))
 commands.extend(x['command'] for x in runs)
for suffix,sanitizer in ([] if '--real-only' in sys.argv else [('normal',[]),('sanitized',['-fsanitize=address,undefined','-fno-omit-frame-pointer'])]):
 out=own/('unit-'+suffix)
 run(['wsl.exe','-e','/usr/bin/c++',*flags,*sanitizer,*inc,linux(r/'tests/level_update_loading_v46.cpp'),linux(physical),linux(box),'-o',linux(out)])
 run(['wsl.exe','-e',linux(out),linux(r/'reference/level-init-v36/level-update-loading-v46-gold.txt')])
run(['wsl.exe','-e','/usr/bin/c++',*flags,*inc,'-c',linux(r/'tests/level_update_loading_v46_real_c1.cpp'),'-o',linux(own/'real-c1.o')])
run(['wsl.exe','-e','/usr/bin/c++',linux(own/'real-c1.o'),'-o',linux(own/'real-c1'),'-Wl,--gc-sections','-Wl,--no-undefined',*libs])
run(['wsl.exe','-e',linux(own/'real-c1'),'/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip','/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/level-world/reference/character-game-design/real-cache-inputs.bin',linux(own)])
save()
