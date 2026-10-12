from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-name-predicate-v48';tmp=Path('C:/Users/adamc/AppData/Local/Temp')
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
(r/'character_name_predicate_v48_body.cpp').write_bytes((tmp/'character_name_predicate_v48_body.cpp').read_bytes())
(r/'character_name_predicate_v48_test.cpp').write_text(prefix+(r/'character_name_predicate_v48_setup.cpp').read_text()+(r/'character_name_predicate_v48_body.cpp').read_text())
s=(b/'reference/character-spawn-fields-v42/character_spawn_fields_v42_test.cpp').read_text().replace('live_archetype','live_name').replace('&first_alias->source_archetype()','&first_alias->source_name()').replace('fields.write_string(p.fields.context,0x48,archetype','fields.write_string(p.fields.context,0x30,archetype').replace('actual_live_type_archetype','actual_live_type_name')
(r/'character_spawn_name_v48_regression.cpp').write_text(s)
(r/'character_model_name_v48_regression.cpp').write_bytes((b/'reference/character-model-route-v45/character_model_route_v45_test.cpp').read_bytes())
(r/'install_character_name_tests_v48.py').write_bytes(Path(__file__).read_bytes())
print('New actual-factory20 Name/Gametype test; updated spawn8 regression uses actual Name source stores; V45 positive model responses unchanged')
