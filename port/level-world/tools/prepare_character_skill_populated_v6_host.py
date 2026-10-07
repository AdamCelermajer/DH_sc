"""Build a new V6 populated-source proof from read-only existing setup helpers."""
from pathlib import Path
p=Path(__file__).resolve().parents[1]
menu=(p.parent/'engine-ui/tests/character_menu_composite_v1.cpp').read_text()
v6=(p/'tests/character_player_skills_v6_host.cpp').read_text()
platform=menu[menu.index('struct MenuPlatform {'):menu.index('Raw section(')]
debug=v6[v6.index('struct NativeDebug {'):v6.index('struct EmptyWorld {')]
setup=menu[menu.index(' check(argc==3'):menu.index(' inventory_order_gold(root);')]
setup=setup.replace('menu-composite','skill-populated-v6')
setup+=menu[menu.index('Debug debug_files;'):menu.index('unsigned classes=0')]
out='''#include "../character_player_skills_v6.hpp"
#define CharacterPlayerSkillsV3 CharacterPlayerSkillsV6
#define main existing_player_fixture_main
#include "character_player_skills_v3.cpp"
#undef main
#include "../character_skill_native_v6.hpp"
#include "../character_skill_target_queries_v6.hpp"
#include "../character_skill_aggro_v6.hpp"
#include "../character_skill_save_reload_v6.hpp"
#include "../../game-data/player_save_load_owner_v1.hpp"
#include "../player_equipment_render_owner_v1.hpp"
#include "../gameobject_lua_representation.hpp"
#include "../../script-runtime/script_return_observer_v3.h"
#include <filesystem>
#include <cctype>
using namespace dh2::player;using namespace dh2::ui;
namespace {
'''+platform+debug+(p/'tests/character_skill_populated_v6_body.inc').read_text()+'\n}\nint main(int argc,char** argv){try{\n'+setup+(p/'tests/character_skill_populated_v6_main.inc').read_text()+'''\n}catch(const std::exception& e){std::cerr<<e.what()<<'\\n';return 1;}}
#undef CharacterPlayerSkillsV3
'''
(p/'tests/character_skill_populated_v6_host.cpp').write_text(out)
