#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <canonical_colbox_v71.hpp>
namespace dh2::loader {
inline bool bind_gameobject_embedded_release_v93(world::CanonicalGameObjectBaseOwnerV1& base,
 world::PfObjectDestructionServicesV93 pf,world::ColBoxServicesV71& out,std::string& e){
 auto resources=base.source_embedded_resources_v93();if(!resources||!pf.owner){e="Required actual SAME embedded GameObject resources/PF authority";return false;}
 // Caller retains the SAME receiver pin for complete synchronous D2. This
 // captures no new owning actor/World; base lifetime is its existing journal.
 if(!out.destroy_target_list)out.destroy_target_list=[](auto& base,std::string& e){auto actual=base.source_embedded_resources_v93();if(!actual){e="Missing actual TargetList304 C1 resource";return false;}return actual->targets304.destroy(e);};
 if(!out.destroy_pf_object)out.destroy_pf_object=[actual=&base,resources,pf=std::move(pf)](actor::RuntimeState& runtime,std::string& e){if(&actual->runtime()!=&runtime||actual->source_embedded_resources_v93()!=resources){e="PFObjectD1 borrower replaced SAME embedded owner/runtime";return false;}return resources->pf.destroy(pf,e);};
 e.clear();return true;
}
}
