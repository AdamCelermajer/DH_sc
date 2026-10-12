#include "condition_data_init_v3.hpp"
#include "game_object_initialization_fields_v51.hpp"
#include "condition_data_kernel_v51.hpp"
namespace dh2::world {
bool condition_data_init_v3(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){auto fields=canonical_gameobject_fields_v51(base,services.owner);return condition_data_init_borrow_v51(fields,offset,services,error);}
bool condition_data_clear_v3(CanonicalGameObjectBaseOwnerV1& base,std::uint32_t offset,const ConditionDataInitServicesV3& services,std::string& error){auto fields=canonical_gameobject_fields_v51(base,services.owner);return condition_data_clear_borrow_v51(fields,offset,services,error);}
}
