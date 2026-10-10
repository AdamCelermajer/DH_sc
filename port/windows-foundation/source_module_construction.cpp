#include "source_module_construction.hpp"
#include <cstring>
#include <stdexcept>
#include <utility>

namespace dh::foundation {
struct SourceModuleConstruction::Impl {
    SourceModuleConstructionProviders providers;
    std::unique_ptr<dh2::world::CanonicalLevelModuleBindingsV2> bindings;
    std::vector<std::shared_ptr<dh2::world::CanonicalModuleRecordV2>> retained;
    static bool spawn(const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>& actual,
        const std::shared_ptr<SourceModuleSpawnApplication>& application,
        std::int32_t& roll,std::int32_t& probability,std::string& error){
        error.clear();
        if(!actual||!actual->receiver){error="Actual Module spawn receiver expired/unconstructed";return false;}
        if(!application){error="Required SAME Module application spawn context";return false;}
        auto& base=actual->receiver->base();
        dh2::world::CanonicalSpawnApplicationServicesV4 source;
        source.application_lease=application->applicationOwner;source.random=application->random;
        source.handle_as_player=[&base](bool& player,std::string& e){
            const auto object=base.canonical({});std::uintptr_t character=0;
            if(!object.as_character||!object.as_character(object.context,character,e))return false;
            if(character){e="Source Module AsChar unexpectedly returned a Character";return false;}
            player=false;e.clear();return true;
        };
        source.online_byte5=[application](bool& online,std::string& e){
            if(!application->onlineByte5){e="Required actual application online byte5 cell";return false;}
            online=*application->onlineByte5!=0;e.clear();return true;
        };
        source.set_visible_false=[application,&base](std::string& e){
            if(!base.store_byte(0x80,0,e))return false;
            const auto visual=*base.pointer(0x2d8);
            if(!visual){e.clear();return true;} // Original SyncVisibility NULL visual branch.
            if(!application->syncVisibility){e="Required SAME Module visual SyncVisibility";return false;}
            return application->syncVisibility(base,visual,e);
        };
        source.mark_for_deletion=[application,&base](std::string& e){
            const auto manager=application->manager.lock();
            if(!manager){e="Required SAME actual Module ObjectManager MarkForDeletion";return false;}
            const auto object=base.canonical({});
            const auto registered=object.shared_handle?manager->object(object.shared_handle->key):nullptr;
            if(!registered||registered->identity!=base.identity()||registered->shared_handle!=object.shared_handle){
                e="Required SAME manager registration for Module MarkForDeletion";return false;
            }
            return manager->source_mark_for_deletion_v89(base.identity(),e);
        };
        return dh2::world::canonical_check_spawn_probability_v4(base,source,roll,probability,error);
    }
    explicit Impl(SourceModuleConstructionProviders p):providers(std::move(p)){
        if(!providers.candidateOwner||!providers.globalsOwner||!providers.globals||!providers.properties)
            throw std::invalid_argument("Actual Module candidate/global/property owners required");
        if(providers.previous.context&&!providers.previousOwner)
            throw std::invalid_argument("Previous source class callbacks require their actual owner lease");
        dh2::world::CanonicalLevelModuleConstructionV2 construction;
        construction.candidate=providers.candidateOwner;construction.module_globals=providers.globals;
        construction.module_services=[this](const auto& source,const auto& record,auto& init,auto& derived,std::string& error){
            if(providers.initialization&&!providers.initialization(source,record,init,derived,error))return false;
            init.owner=providers.candidateOwner;
            const std::weak_ptr<dh2::world::CanonicalModuleRecordV2> weak=record;
            if(providers.spawnApplication){
                const std::weak_ptr<SourceModuleSpawnApplication> application=providers.spawnApplication;
                init.check_spawn_probability=[weak,application](std::int32_t& roll,std::string& e){
                    std::int32_t probability=0;return spawn(weak.lock(),application.lock(),roll,probability,e);
                };
            }
            const auto conditions=providers.conditions?providers.conditions->condition_data_services():dh2::world::ConditionDataInitServicesV3{};
            // CanonicalModuleV1 creates its base after module_services returns.
            // Borrow that SAME record lazily at InitPost; the independent arena
            // owns condition receivers, never the containing Module record.
            init.condition_init=[weak,conditions](std::uint32_t offset,std::string& e){
                const auto actual=weak.lock();if(!actual||!actual->receiver){e="Actual Module ConditionData receiver expired/unconstructed";return false;}
                return dh2::world::condition_data_init_v3(actual->receiver->base(),offset,conditions,e);
            };
            const auto backend=providers.positionBackends;
            init.set_position=[weak,backend](const float* position,bool destination,std::string& e){
                const auto actual=weak.lock();if(!actual||!actual->receiver){e="Actual Module SetPosition receiver expired/unconstructed";return false;}
                auto& base=actual->receiver->base();auto& runtime=base.runtime();
                dh2::character::CharacterPositionBorrowV7 borrow;
                borrow.receiver=actual;borrow.identity=base.identity();borrow.position160=runtime.subobjects.position;
                borrow.relative144=base.relative_aabb144();borrow.absolute12c=base.absolute_aabb12c();
                borrow.destination1a8=runtime.controller.destination;borrow.attached2e0=base.pointer(0x2e0);
                borrow.physical2dc=base.pointer(0x2dc);borrow.visual2d8=base.pointer(0x2d8);
                dh2::character::CharacterPositionResultV7 result;
                return dh2::character::character_set_position_v7(result,borrow,position,destination,backend,e);
            };
            retained.push_back(record);error.clear();return true;
        };
        bindings=std::make_unique<dh2::world::CanonicalLevelModuleBindingsV2>(*providers.properties,providers.previous,std::move(construction));
    }
    static bool construct(void* context,const dh2::world::CanonicalFactoryEntryV1& factory,
        const dh2::world::CanonicalSourceObjectRequestV1& source,dh2::world::CanonicalObjectBorrowV1& object,std::string& error){
        auto& self=*static_cast<Impl*>(context);
        if(factory.name&&(!std::strcmp(factory.name,"Module")||!std::strcmp(factory.name,"Block"))){
            const auto actual=self.bindings->services();return actual.construct(actual.context,factory,source,object,error);
        }
        // Preserve the existing LevelConfig owner rather than allowing the
        // broad legacy binding to construct a second config receiver authority.
        if(self.providers.previous.construct)return self.providers.previous.construct(self.providers.previous.context,factory,source,object,error);
        error=std::string("Required actual other source constructor: ")+(factory.name?factory.name:"<null>");return false;
    }
};
SourceModuleConstruction::SourceModuleConstruction(SourceModuleConstructionProviders p):impl_(std::make_shared<Impl>(std::move(p))){}
dh2::world::CanonicalClassServicesV1 SourceModuleConstruction::services() const noexcept{
    auto result=impl_->bindings->services();
    // Only construction needs class selection; the recovered binding's other
    // callbacks already delegate unknown receivers to SAME predecessor.
    // These callbacks need different contexts, so retain explicit forwarding.
    result.context=impl_.get();result.construct=Impl::construct;
    result.init_properties=[](void* c,const auto& o,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.init_properties(s.context,o,e);};
    result.set_template=[](void* c,const auto& o,const char* n,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.set_template(s.context,o,n,e);};
    result.load_defaults=[](void* c,const auto& o,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.load_defaults(s.context,o,e);};
    result.load_overrides=[](void* c,const auto& o,const auto& q,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.load_overrides(s.context,o,q,e);};
    result.init_post=[](void* c,const auto& o,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.init_post(s.context,o,e);};
    result.is_game_object=[](void* c,const auto& o,bool& v,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.is_game_object(s.context,o,v,e);};
    result.position=[](void* c,const auto& o,std::array<float,3>& v,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.position(s.context,o,v,e);};
    result.set_position=[](void* c,const auto& o,const std::array<float,3>& v,bool update,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.set_position(s.context,o,v,update,e);};
    result.unknown_type_debug=[](void* c,const char* n,auto& e){auto s=static_cast<Impl*>(c)->bindings->services();return s.unknown_type_debug(s.context,n,e);};
    return result;
}
std::shared_ptr<void> SourceModuleConstruction::lease() const noexcept{return impl_;}
bool SourceModuleConstruction::record(std::uintptr_t identity,std::shared_ptr<dh2::world::CanonicalModuleRecordV2>& out,std::string& error) const{
    for(const auto& record:impl_->retained)if(record->receiver&&record->receiver->base().identity()==identity){out=record;error.clear();return true;}
    error="Actual Module constructor record not retained by SAME provider";return false;
}
const std::vector<std::shared_ptr<dh2::world::CanonicalModuleRecordV2>>& SourceModuleConstruction::records() const noexcept{return impl_->retained;}
bool SourceModuleConstruction::clear_conditions(std::uintptr_t identity,std::string& error) const{
    std::shared_ptr<dh2::world::CanonicalModuleRecordV2> actual;
    if(!record(identity,actual,error))return false;
    const auto conditions=impl_->providers.conditions?impl_->providers.conditions->condition_data_services():dh2::world::ConditionDataInitServicesV3{};
    auto& base=actual->receiver->base();
    return dh2::world::condition_data_clear_v3(base,0x8c,conditions,error)&&
           dh2::world::condition_data_clear_v3(base,0xb0,conditions,error);
}
bool SourceModuleConstruction::check_spawn_probability(std::uintptr_t identity,std::int32_t& roll,std::int32_t& probability,std::string& error) const{
    std::shared_ptr<dh2::world::CanonicalModuleRecordV2> actual;
    if(!record(identity,actual,error))return false;
    return Impl::spawn(actual,impl_->providers.spawnApplication,roll,probability,error);
}
} // namespace dh::foundation
