#include "activation_bindings.hpp"
namespace dh::foundation::encounters {
bool bind_source_activation(ActivationReceiver receiver,
 std::shared_ptr<Admission> admission,dh2::world::CanonicalSpawnApplicationServicesV4 app,
 dh2::world::GameObjectInitializationServicesV1& out,std::string& error){
 if(!receiver.scope||!receiver.same_base||!admission||!out.owner||!app.application_lease||!app.random||!app.handle_as_player){
  error="Source activation requires SAME receiver/admission/application RNG and initialization owner";return false;
 }
 if(out.condition_init||out.check_spawn_probability){
  error="Source activation cannot replace existing initialization providers";return false;
 }
 auto resolve=[receiver](dh2::world::CanonicalGameObjectBaseOwnerV1*& base,std::string& e){
  base=nullptr;if(!receiver.same_base(base,e))return false;
  if(!base){e="Actual source activation receiver absent";return false;}return true;
 };
 out.condition_init=[resolve,admission](unsigned offset,std::string& e){dh2::world::CanonicalGameObjectBaseOwnerV1* base;
  return resolve(base,e)&&admission->initialize(*base,offset,e);};
 out.check_spawn_probability=[resolve,app](int& roll,std::string& e){
  dh2::world::CanonicalGameObjectBaseOwnerV1* base;if(!resolve(base,e))return false;
  int probability;return dh2::world::canonical_check_spawn_probability_v4(*base,app,roll,probability,e);
 };
 error.clear();return true;
}
}
