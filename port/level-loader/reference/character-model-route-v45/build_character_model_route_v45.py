from pathlib import Path
import sys,json,subprocess,hashlib,argparse
p=argparse.ArgumentParser();p.add_argument('--native',action='store_true');p.add_argument('--coherent-ready',action='store_true');a=p.parse_args();assert not a.native or a.coherent_ready,'Parent coherent READY required before archive link'
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');ref=base/'reference/character-model-route-v45';temp=Path('C:/Users/adamc/AppData/Local/Temp');vendor=base/'vendor/character-rng-integration-v5-loading/port';build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host');sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
setup=(ref/'character_model_route_v45_setup.cpp').read_text()
prefix='''#include "character_model_route_v45.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "game_object_spawn_probability_v1.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include "character_scene.hpp"
#include "player_savegame_v1.hpp"
#include "faery_tables.hpp"
#include <zip_asset_pack_v1.hpp>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;using namespace dh2::world;using namespace dh2::loader;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
'''
test=ref/'character_model_route_v45_test.cpp';test.write_text(prefix+setup+(ref/'character_model_route_v45_body.cpp').read_text())
def w(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
dirs=[ref,vendor/'level-world',vendor/'level-loader',base]+[p for p in vendor.iterdir() if p.is_dir() and p.name not in ['level-world','level-loader']];flags=['-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG']+['-I'+w(p) for p in dirs]
if not a.native:
 cmd=['wsl.exe','-d','Ubuntu','--','c++']+flags+['-fsyntax-only',w(ref/'character_model_route_v45.cpp'),w(ref/'canonical_character_family_v4.cpp'),w(test)];r=subprocess.run(cmd,capture_output=True,text=True);(base/'reports/character-model-route-v45-all-syntax.log').write_text(r.stdout+r.stderr);print((r.stdout+r.stderr)[-10000:],flush=True);assert r.returncode==0;print('PASS V45 full candidate/test syntax; NO archive linkage/native/game launch');sys.exit(0)
snapshot=ref/'genuine-save-snapshot'
assert sha(snapshot/'player_savegame_v1.hpp')==sha(vendor/'game-data/player_savegame_v1.hpp')=='014acd3b821b4ab99b7fbd2072a8df4132465d783917e3b9592e58e8aeda0f6f','Frozen genuine Save header/V5 mismatch'
assert sha(snapshot/'player_savegame_v1.cpp')=='ca713e12bd25307bca7b24c95714cec45bda1e198fdbe70e952315659ef91d43','Frozen genuine Save source mismatch'
assert sha(root/'port/game-data/faery_tables.hpp')==sha(vendor/'game-data/faery_tables.hpp'),'Root/V5 Faery header mismatch'
archives=list(build.rglob('*.a'));assert archives
sources=[ref/'retained_character_actor_v1.cpp',ref/'canonical_character_family_v4.cpp',ref/'character_is_player_v41.cpp',ref/'character_model_route_v45.cpp',vendor/'level-world/character_model_name_v38.cpp',root/'port/level-world/game_object_spawn_probability_v1.cpp',snapshot/'player_savegame_v1.cpp',root/'port/game-data/faery_tables.cpp',test]
out=build.parent/'character-model-route-v45-probe';linkmap=base/'reports/character-model-route-v45.map';selected={str(vendor/'level-world'/n):sha(vendor/'level-world'/n) for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','canonical_character_family_v4.hpp','canonical_character_family_v4.cpp']}
cmd=['wsl.exe','-d','Ubuntu','--','c++']+flags+['-O1','-g','-ffunction-sections','-fdata-sections']+[w(p) for p in sources]+['-Wl,--gc-sections','-Wl,-Map='+w(linkmap),'-lstdc++','-Wl,--start-group']+[w(p) for p in archives]+['-Wl,--end-group',w(build/'libdh2_script_runtime.so'),'-Wl,-rpath,'+w(build),'-lz','-o',w(out)]
r=subprocess.run(cmd,capture_output=True,text=True);(base/'reports/character-model-route-v45-build.log').write_text(r.stdout+r.stderr);print((r.stdout+r.stderr)[-12000:],flush=True);assert r.returncode==0
for bad in ['(retained_character_actor_v1.cpp.o)','(canonical_character_family_v4.cpp.o)']:assert bad not in linkmap.read_text(),bad
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');design=base/'tests/native-ctor-v4/design.bin';r=subprocess.run(['wsl.exe','-d','Ubuntu','--',w(out),w(cache),w(design)],capture_output=True,text=True);print(r.stdout+r.stderr,flush=True);(base/'reports/character-model-route-v45-native.log').write_text(r.stdout+r.stderr);assert r.returncode==0
assert all(sha(Path(p))==expected for p,expected in selected.items()),'Selected graph changed; proof input audit required'
receipt={'schema':'dh2-character-model-route-v45-verified','validation':'PASS bounded ordered actual model-stage route','native_output':r.stdout,'sources':{str(p):sha(p) for p in sources},'genuine_save_header_sha256':sha(snapshot/'player_savegame_v1.hpp'),'genuine_save_source_provenance':str(root/'.local-inputs/publication/checkpoint/port/game-data/player_savegame_v1.cpp'),'selected_sources_unchanged':selected,'coherent_parent_ready':True,'binary_sha256':sha(out),'link_map_sha256':sha(linkmap),'no_archived_actor_factory':True,'actual_LoadBase_Recalc_helpers':True,'remaining_fixtures':'Debug/online/network, saved difficulty1/DEFAULT Faery list/master Save association. Actual retained save getter and original Faery tables used. Whole gameplay save/player/AI/visual not claimed.','whole_InitPost_success':False,'constructor_model_template_preselection':False,'emulator_launched':False};(base/'reports/character-model-route-v45-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
