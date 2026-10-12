from pathlib import Path
import subprocess,hashlib,json,argparse
p=argparse.ArgumentParser();p.add_argument('--native',action='store_true');p.add_argument('--coherent-ready',action='store_true');a=p.parse_args();assert not a.native or a.coherent_ready
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-parent-init-v48/actual-actor-cell-candidate';v=b/'vendor/character-rng-integration-v5-loading/port';build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host');root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');reports=b/'reports';tmp=Path('C:/Users/adamc/AppData/Local/Temp');sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();w=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
for n in ['canonical_character_family_v4.hpp','canonical_character_family_v4.cpp','character_is_player_v41.hpp','character_is_player_v41.cpp']:(r/n).write_bytes((v/'level-world'/n).read_bytes())
prefix='''#include "canonical_character_family_v4.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <zip_asset_pack_v1.hpp>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;using namespace dh2::world;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
'''
test=r/'character_parent_cells_v48_test.cpp'
body=r/'character_parent_cells_v48_body.cpp'
if not body.exists():body.write_bytes((tmp/'character_parent_cells_v48_body.cpp').read_bytes())
setup=r/'character_parent_cells_v48_setup.cpp'
if not setup.exists():setup.write_bytes((b/'reference/character-name-predicate-v48/character_name_predicate_v48_setup.cpp').read_bytes())
test.write_text(prefix+setup.read_text()+body.read_text())
dirs=[r,v/'level-world',v/'level-loader',b]+[p for p in v.iterdir() if p.is_dir() and p.name not in ['level-world','level-loader']];flags=['-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG']+['-I'+w(p) for p in dirs]
sources=[r/'retained_character_actor_v1.cpp',r/'canonical_character_family_v4.cpp',r/'character_is_player_v41.cpp',test]
selected={str(v/'level-world'/n):sha(v/'level-world'/n) for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','canonical_character_family_v4.hpp','canonical_character_family_v4.cpp']}
if not a.native:cmd=['wsl.exe','-d','Ubuntu','--','c++']+flags+['-fsyntax-only']+[w(p) for p in sources]
else:
 archives=sorted(build.rglob('*.a'));assert archives;out=build.parent/'character-parent-cells-v48-probe';linkmap=reports/'character-parent-cells-v48.map';cmd=['wsl.exe','-d','Ubuntu','--','c++']+flags+['-O1','-g','-ffunction-sections','-fdata-sections']+[w(p) for p in sources]+['-Wl,--gc-sections','-Wl,-Map='+w(linkmap),'-lstdc++','-Wl,--start-group']+[w(p) for p in archives]+['-Wl,--end-group',w(build/'libdh2_script_runtime.so'),'-Wl,-rpath,'+w(build),'-lz','-o',w(out)]
q=subprocess.run(cmd,capture_output=True,text=True);(reports/'character-parent-cells-v48-build.log').write_text(q.stdout+q.stderr);print((q.stdout+q.stderr)[-9000:],flush=True);assert q.returncode==0
if not a.native:print('PASS parent8cell candidate and actual factory ownership test syntax; no archive link');raise SystemExit
for bad in ['(retained_character_actor_v1.cpp.o)','(canonical_character_family_v4.cpp.o)','(character_is_player_v41.cpp.o)']:assert bad not in linkmap.read_text(),bad
q=subprocess.run(['wsl.exe','-d','Ubuntu','--',w(out),'/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip',w(b/'tests/native-ctor-v4/design.bin')],capture_output=True,text=True);(reports/'character-parent-cells-v48-native.log').write_text(q.stdout+q.stderr);print(q.stdout+q.stderr,flush=True);assert q.returncode==0
assert all(sha(Path(p))==h for p,h in selected.items())
receipt={'schema':'dh2-character-parent-cells-v48-verified','validation':'PASS isolated actual-owner constructor/alias transport','sources':{str(p):sha(p) for p in sources},'header':sha(r/'character_parent_init_fields_v48.hpp'),'binary_sha256':sha(out),'link_map_sha256':sha(linkmap),'native_output':q.stdout,'selected_sources_unchanged':selected,'no_archived_actor_factory_predicate':True,'newcells':8,'compiled_condition_allocated':False,'whole_GameObject_InitPost_wired':False,'rendered':False,'playable':False,'emulator_launched':False}
(reports/'character-parent-cells-v48-verified.json').write_text(json.dumps(receipt,indent=2)+'\n');(r/'build_parent_cells_v48.py').write_bytes(Path(__file__).read_bytes())
