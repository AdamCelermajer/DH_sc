#include "game_object_initialization_owner_v1.hpp"
#include "game_object_initialization_fields_v51.hpp"
#include "game_object_initialization_kernel_v51.hpp"
namespace dh2::world {
bool GameObjectInitializationOwnerV1::object_base_init_post(std::string& e){auto fields=canonical_gameobject_fields_v51(base_,services_.owner);return GameObjectInitializationKernelV51<GameObjectInitializationFieldsV51>(fields,services_).object_base_init_post(e);}
bool GameObjectInitializationOwnerV1::init_post(bool& eligible,std::string& e){auto fields=canonical_gameobject_fields_v51(base_,services_.owner);return GameObjectInitializationKernelV51<GameObjectInitializationFieldsV51>(fields,services_).init_post(eligible,e);}
bool GameObjectInitializationOwnerV1::init_final(bool& eligible,std::string& e){auto fields=canonical_gameobject_fields_v51(base_,services_.owner);return GameObjectInitializationKernelV51<GameObjectInitializationFieldsV51>(fields,services_).init_final(eligible,e);}
}
