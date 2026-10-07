#pragma once
#include <condition_data_init_v3.hpp>
namespace dh2::loader {
struct ObjectBaseDestroyLeavesV92 {
 std::shared_ptr<void> owner;world::ConditionDataInitServicesV3 conditions;
 std::function<bool(world::CanonicalGameObjectBaseOwnerV1&,std::string&)> registered29_assertion;
 std::function<bool(world::CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,std::string&)> free_owned2c;
};
// Actual ObjectBaseD2 33e998: no second base, class D0 or free of host storage.
inline bool object_base_source_destroy_v92(world::CanonicalGameObjectBaseOwnerV1& base,
 const ObjectBaseDestroyLeavesV92& leaves,std::string& e){
 if(!leaves.owner){e="Required independent ObjectBase destruction authority";return false;}
 auto registered=base.byte(0x29);auto allocation=base.pointer(0x2c);
 if(!registered||!allocation){e="Required actual ObjectBase29/2c constructor fields";return false;}
 if(*registered&&(!leaves.registered29_assertion||!leaves.registered29_assertion(base,e))){if(e.empty())e="Required actual ObjectBase29 assertion branch";return false;}
 if(*allocation){auto actual=*allocation;if(!leaves.free_owned2c||!leaves.free_owned2c(base,actual,e)){if(e.empty())e="Required actual owned ObjectBase2c allocation release";return false;}if(*allocation&&*allocation!=actual){e="ObjectBase2c replaced during source free";return false;}*allocation=0;}
 auto string=[&](unsigned offset){auto value=base.string(offset);if(!value){e="Required SAME source CString field";return false;}std::string{}.swap(*value);return true;};
 if(!string(0xd4))return false;
 // Source ConditionDataD1 33e7f8 = Clear33e7c8 THEN CString4D1.
 // ObjectBaseD2 reaches b0 before8c (33ea18..33ea24).
 for(auto offset:{0xb0u,0x8cu})if(!world::condition_data_clear_v3(base,offset,leaves.conditions,e)||!string(offset+4))return false;
 for(auto offset:{0x68u,0x48u,0x30u,0x8u})if(!string(offset))return false;
 e.clear();return true;
}
}
