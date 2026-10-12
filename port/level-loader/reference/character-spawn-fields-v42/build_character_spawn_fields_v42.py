from pathlib import Path
import json,subprocess,shutil,hashlib
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');ref=base/'reference/character-spawn-fields-v42';temp=Path('C:/Users/adamc/AppData/Local/Temp');vendor=base/'vendor/character-rng-integration-v5-loading/port';build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host');sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
source=(base/'tests/character_swamp_model_v40_probe.cpp').read_text();start=source.index('static assets::ZipAssetPackV1 pack(');end=source.index('// Source-probe extraction',start);setup=source[start:end]
prefix='''#include "canonical_character_family_v4.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "game_object_spawn_probability_v1.hpp"
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
test=ref/'character_spawn_fields_v42_test.cpp';assert test.is_file()
def w(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
includes=[ref,vendor/'level-world',vendor/'level-loader',base]+[p for p in vendor.iterdir() if p.is_dir() and p.name not in ['level-world','level-loader']];archives=list(build.rglob('*.a'));assert archives
sources=[ref/'retained_character_actor_v1.cpp',ref/'canonical_character_family_v4.cpp',ref/'character_is_player_v41.cpp',root/'port/level-world/game_object_spawn_probability_v1.cpp',test];out=build.parent/'character-spawn-fields-v42-probe';linkmap=base/'reports/character-spawn-fields-v42.map'
cmd=['wsl.exe','-d','Ubuntu','--','c++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG','-ffunction-sections','-fdata-sections']+['-I'+w(p) for p in includes]+['-isystem',w(vendor/'physics-backend/box2d-2.0.1/Include')]+[w(p) for p in sources]+['-Wl,--gc-sections','-Wl,-Map='+w(linkmap),'-lstdc++','-Wl,--start-group']+[w(p) for p in archives]+['-Wl,--end-group',w(build/'libdh2_script_runtime.so'),'-Wl,-rpath,'+w(build),'-lz','-o',w(out)]
r=subprocess.run(cmd,capture_output=True,text=True);(base/'reports/character-spawn-fields-v42-build.log').write_text(r.stdout+r.stderr);print((r.stdout+r.stderr)[-12000:],flush=True);assert r.returncode==0
for bad in ['(retained_character_actor_v1.cpp.o)','(canonical_character_family_v4.cpp.o)']:assert bad not in linkmap.read_text(),bad
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');design=base/'tests/native-ctor-v4/design.bin';r=subprocess.run(['wsl.exe','-d','Ubuntu','--',w(out),w(cache),w(design)],capture_output=True,text=True);print(r.stdout+r.stderr,flush=True);(base/'reports/character-spawn-fields-v42-native.log').write_text(r.stdout+r.stderr);assert r.returncode==0
initial=json.loads((base/'reports/character-spawn-fields-v42-proposal.json').read_text())['initial_hashes'];assert all(sha(Path(p))==expected for p,expected in initial.items()),'Selected graph input changed during proof; review newer authority before adoption'
receipt={'schema':'dh2-character-spawn-fields-v42-verified','validation':'PASS candidate actual actor/factory fields and borrowed spawn kernel','native_output':r.stdout,'sources':{str(p):sha(p) for p in sources},'root_spawn_header_sha256':sha(root/'port/level-world/game_object_spawn_probability_v1.hpp'),'V5_spawn_header_sha256':sha(vendor/'level-world/game_object_spawn_probability_v1.hpp'),'binary_sha256':sha(out),'link_map_sha256':sha(linkmap),'original_cache_sha256':sha(cache),'original_design_fixture_sha256':sha(design),'old_actor_factory_archives_not_linked':True,'selected_graph_unchanged':True,'actual_constructor_property_owner':True,'network_offline_lifecycle_providers':'Explicit pinned fixtures; existing kernel branch/order tested, main behavior not claimed','model_template_constructor_preselection':False,'emulator_launched':False}
assert receipt['root_spawn_header_sha256']==receipt['V5_spawn_header_sha256'];(base/'reports/character-spawn-fields-v42-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
