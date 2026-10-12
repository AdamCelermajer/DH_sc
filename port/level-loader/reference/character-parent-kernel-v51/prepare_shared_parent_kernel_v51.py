from pathlib import Path
import hashlib,json,difflib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');v=b/'vendor/character-rng-integration-v5-loading/port/level-world';r=b/'reference/character-parent-kernel-v51';r.mkdir(exist_ok=True)
for n in ['game_object_initialization_owner_v1.hpp','game_object_initialization_owner_v1.cpp','condition_data_init_v3.hpp','condition_data_init_v3.cpp']:(r/('original-'+n)).write_bytes((v/n).read_bytes())
h=(v/'game_object_initialization_owner_v1.hpp').read_text();start=h.index('struct GameObjectInitializationServicesV1 {');end=h.index('\n};',start)+3
services='#pragma once\n#include <functional>\n#include <memory>\n#include <string>\n#include <vector>\nnamespace dh2::world {\n'+h[start:end]+'\n}\n'
(r/'game_object_initialization_services_v51.hpp').write_text(services)
new_h=h[:start]+h[end:];new_h=new_h.replace('#include "canonical_gameobject_base_owner_v1.hpp"','#include "canonical_gameobject_base_owner_v1.hpp"\n#include "game_object_initialization_services_v51.hpp"')
(r/'game_object_initialization_owner_v1.hpp').write_text(new_h)
cpp=(v/'game_object_initialization_owner_v1.cpp').read_text();start=cpp.index('bool GameObjectInitializationOwnerV1::missing');methods=cpp[start:];assert methods.endswith('}\n');methods=methods[:-2]
methods=methods.replace('bool GameObjectInitializationOwnerV1::','template<class Fields>\nbool GameObjectInitializationKernelV51<Fields>::')
methods=methods.replace('base_.integer(0x274)','base_.probability274()').replace('base_.integer(0xec)','base_.difficulty_ec()')
methods=methods.replace('auto* checkpoint=base_.byte(0x28);if(!checkpoint)return missing("checkpoint+28 field",e);*checkpoint=0;','if(!base_.clear_checkpoint(e))return false;')
old='''  auto* target=base_.integer(0x370);if(!target)return missing("idle sound id+370 field",e);
  // Original stores a halfword, read subsequently as signed short.
  *target=static_cast<std::int16_t>(static_cast<std::uint16_t>(id));'''
new='''  // Adapter writes only the actual owner's original signed-halfword value.
  if(!base_.store_idle_sound16(id,e))return false;'''
assert old in methods;methods=methods.replace(old,new)
methods=methods.replace('auto* archetype=base_.string(0x48);auto* pfname=base_.string(0x254);','auto* archetype=base_.string(0x48);auto* pfname=base_.pf_name254();')
helpers=cpp[cpp.index('namespace {'):start].replace('namespace {','namespace parent_kernel_detail_v51 {').replace('float source_float','inline float source_float').replace('bool find_name','inline bool find_name')
methods=methods.replace('source_float(','parent_kernel_detail_v51::source_float(').replace('find_name(','parent_kernel_detail_v51::find_name(')
kernel='''#pragma once
#include "game_object_initialization_services_v51.hpp"
#include <cmath>
#include <cstring>
namespace dh2::world {
// One source control kernel. Fields lends actual storage; it constructs no
// CanonicalBase, pose, visual, condition, save or eligibility/stage cache.
template<class Fields> class GameObjectInitializationKernelV51 {
 Fields& base_;const GameObjectInitializationServicesV1& services_;
 bool missing(const char*,std::string&)const;
 bool spawn(std::int32_t&,std::string&);
public:
 GameObjectInitializationKernelV51(Fields& f,const GameObjectInitializationServicesV1& s):base_(f),services_(s){}
 bool object_base_init_post(std::string&);
 bool init_post(bool&,std::string&);
 bool init_final(bool&,std::string&);
};
'''+helpers+methods+'}\n'
(r/'game_object_initialization_kernel_v51.hpp').write_text(kernel)
wrapper='''#include "game_object_initialization_owner_v1.hpp"
#include "game_object_initialization_fields_v51.hpp"
#include "game_object_initialization_kernel_v51.hpp"
namespace dh2::world {
bool GameObjectInitializationOwnerV1::object_base_init_post(std::string& e){auto fields=canonical_gameobject_fields_v51(base_,services_.owner);return GameObjectInitializationKernelV51<GameObjectInitializationFieldsV51>(fields,services_).object_base_init_post(e);}
bool GameObjectInitializationOwnerV1::init_post(bool& eligible,std::string& e){auto fields=canonical_gameobject_fields_v51(base_,services_.owner);return GameObjectInitializationKernelV51<GameObjectInitializationFieldsV51>(fields,services_).init_post(eligible,e);}
bool GameObjectInitializationOwnerV1::init_final(bool& eligible,std::string& e){auto fields=canonical_gameobject_fields_v51(base_,services_.owner);return GameObjectInitializationKernelV51<GameObjectInitializationFieldsV51>(fields,services_).init_final(eligible,e);}
}
'''
(r/'game_object_initialization_owner_v1.cpp').write_text(wrapper)
patch=''
for n in ['game_object_initialization_owner_v1.hpp','game_object_initialization_owner_v1.cpp']:patch+=''.join(difflib.unified_diff((v/n).read_text().splitlines(True),(r/n).read_text().splitlines(True),fromfile='a/port/level-world/'+n,tofile='b/port/level-world/'+n))
(r/'shared-parent-kernel-v51-adoption.proposed.patch').write_text(patch)
(r/'prepare_shared_parent_kernel_v51.py').write_bytes(Path(__file__).read_bytes())
print('Prepared one shared original control kernel +unchanged generic API wrappers; typed field adapters/tests pending; selected untouched')
