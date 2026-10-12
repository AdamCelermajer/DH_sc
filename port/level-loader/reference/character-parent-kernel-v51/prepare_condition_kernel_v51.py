from pathlib import Path
import difflib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');v=b/'vendor/character-rng-integration-v5-loading/port/level-world';r=b/'reference/character-parent-kernel-v51'
h=(v/'condition_data_init_v3.hpp').read_text().replace('// Native immutable Arrays::Conditions row. argument8/argument4 are actual\n// decoded native receivers passed to CCondition::Init, never ARM addresses.','// Native immutable Arrays::Conditions row. argument8 identifies actual decoded\n// v2ConditionStub storage; argument4 preserves the source signed32 count bits.\n// ConditionList::AssignPyData478914 consumes them; neither is a fake receiver.').replace('CCondition allocation/ctor/lifetime','ConditionList allocation/ctor/lifetime').replace('Whole CCondition destructor then free','Whole ConditionList D1 then CustomFree')
start=h.index('struct ConditionDataRowV3');end=h.index('// Whole ConditionData::Init',start)
(r/'condition_data_services_v51.hpp').write_text('#pragma once\n#include <functional>\n#include <memory>\n#include <string>\n#include <vector>\n#include <cstdint>\nnamespace dh2::world {\n'+h[start:end]+'}\n')
h=h[:start]+h[end:];h=h.replace('#include "canonical_gameobject_base_owner_v1.hpp"','#include "canonical_gameobject_base_owner_v1.hpp"\n#include "condition_data_services_v51.hpp"');(r/'condition_data_init_v3.hpp').write_text(h)
cpp=(v/'condition_data_init_v3.cpp').read_text();start=cpp.index('namespace {');body=cpp[start:];assert body.endswith('}\n');body=body[:-2]
body=body.replace('namespace {','namespace condition_kernel_detail_v51 {').replace('bool borrow(CanonicalGameObjectBaseOwnerV1& base','template<class Fields>\nbool borrow(Fields& base').replace('std::string*& name','const std::string*& name')
body=body.replace('bool condition_data_init_v3(CanonicalGameObjectBaseOwnerV1& base','template<class Fields>\nbool condition_data_init_borrow_v51(Fields& base').replace('bool condition_data_clear_v3(CanonicalGameObjectBaseOwnerV1& base','template<class Fields>\nbool condition_data_clear_borrow_v51(Fields& base').replace('std::string* name{}','const std::string* name{}').replace('if(!borrow(base','if(!condition_kernel_detail_v51::borrow(base')
body=body.replace('actual CCondition allocation/constructor','actual ConditionList allocation/constructor').replace('Actual CCondition constructor receiver','Actual ConditionList constructor receiver').replace('actual CCondition::Init478914','actual ConditionList::AssignPyData478914').replace('actual CCondition destructor/deallocation','actual ConditionList destructor/CustomFree')
(r/'condition_data_kernel_v51.hpp').write_text('#pragma once\n#include "condition_data_services_v51.hpp"\nnamespace dh2::world {\n'+body+'}\n')
(r/'condition_data_init_v3.cpp').write_text('''#include "condition_data_init_v3.hpp"
#include "game_object_initialization_fields_v51.hpp"
#include "condition_data_kernel_v51.hpp"
namespace dh2::world {
bool condition_data_init_v3(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){auto fields=canonical_gameobject_fields_v51(base,services.owner);return condition_data_init_borrow_v51(fields,offset,services,error);}
bool condition_data_clear_v3(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){auto fields=canonical_gameobject_fields_v51(base,services.owner);return condition_data_clear_borrow_v51(fields,offset,services,error);}
}
''')
patch=''
for n in ['game_object_initialization_owner_v1.hpp','game_object_initialization_owner_v1.cpp','condition_data_init_v3.hpp','condition_data_init_v3.cpp']:patch+=''.join(difflib.unified_diff((v/n).read_text().splitlines(True),(r/n).read_text().splitlines(True),fromfile='a/port/level-world/'+n,tofile='b/port/level-world/'+n))
(r/'shared-parent-kernel-v51-adoption.proposed.patch').write_text(patch);(r/'prepare_condition_kernel_v51.py').write_bytes(Path(__file__).read_bytes());print('One borrowed ConditionData kernel +generic wrappers; exact ConditionList/count labels; no constructor/backend supplied')
