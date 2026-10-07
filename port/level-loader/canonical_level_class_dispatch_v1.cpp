#include "canonical_level_class_dispatch_v1.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::loader {
bool CanonicalLevelClassDispatchV1::construct(const world::CanonicalFactoryEntryV1& f,
    const world::CanonicalSourceObjectRequestV1& q,world::CanonicalClassReceiverV1& out,std::string& e){
    if(!input_.providers_owner||!f.name){e="Required retained actual class constructor providers";return false;}
    if(!std::strcmp(f.name,"LevelConfig")||!std::strcmp(f.name,"Module")||!std::strcmp(f.name,"Block")){
        if(!input_.modules){e="Required SAME LevelConfig/Module constructor bindings";return false;}
        return input_.modules->construct_receiver(f,q,out,e);
    }
    if(!std::strcmp(f.name,"Character")){
        if(!input_.characters){e="Required actual Character family constructor";return false;}
        return input_.characters->construct(f,q,out,e);
    }
    if(!std::strcmp(f.name,"OpenableContainer")){
        if(f.original_address!=0x340da4){e="Original OpenableContainer factory address differs";return false;}
        world::CanonicalOpenableGraphServicesV4 s;
        if(!input_.openable_services){e="Required actual OpenableContainer graph providers";return false;}
        if(!input_.openable_services(q,s,e))return false;
        // A weak publication cell binds the actual C1 receiver after allocation
        // without a cycle or a parallel base object. InitPost is invoked later.
        auto link=std::make_shared<std::weak_ptr<world::CanonicalOpenableGraphV4>>();
        auto application=input_.application_owner;auto* random=input_.random;auto providers=input_.spawn_services;
        s.container.spawn_roll_and_probability=[link,application,random,providers](std::int32_t& roll,std::int32_t& probability,std::string& error){
            auto graph=link->lock();
            if(!graph||!application||!random||!providers){error="Required actual application/Container spawn providers";return false;}
            world::CanonicalSpawnApplicationServicesV4 services;
            if(!providers(graph,services,error))return false;
            if(!services.application_lease||services.application_lease.owner_before(application)||application.owner_before(services.application_lease)||services.random!=random){error="Container spawn replaced the application RNG authority";return false;}
            return world::canonical_check_spawn_probability_v4(graph->receiver().base(),services,roll,probability,error);
        };
        auto check=s.container.spawn_roll_and_probability;
        s.initialization.check_spawn_probability=[check](std::int32_t& roll,std::string& error){std::int32_t probability{};return check(roll,probability,error);};
        auto graph=std::make_shared<world::CanonicalOpenableGraphV4>(std::move(s));*link=graph;
        auto receiver=graph->factory_receiver();receiver.source_lease=q.source_lease;
        containers_.push_back(std::move(graph));out=std::move(receiver);return true;
    }
    if(input_.remaining)return input_.remaining(f,q,out,e);
    e=std::string("Required actual catalog constructor: ")+f.name;return false;
}
void CanonicalLevelClassDispatchV1::erased(std::uintptr_t id){
    if(input_.modules)input_.modules->erased(id);
    if(input_.characters)input_.characters->erase_after_unpublication(id);
    containers_.erase(std::remove_if(containers_.begin(),containers_.end(),[id](const auto& graph){return graph->receiver().base().identity()==id;}),containers_.end());
}
}
