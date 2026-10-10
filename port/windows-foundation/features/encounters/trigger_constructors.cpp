#include "trigger_constructors.hpp"
#include <cstring>
#include <stdexcept>
namespace dh::foundation::encounters {
struct TriggerConstructors::Impl {
 TriggerConstructorServices p;std::map<std::uintptr_t,std::shared_ptr<TriggerRecord>> records;
 explicit Impl(TriggerConstructorServices x):p(std::move(x)){if(!p.world_owner||!p.properties||!p.source_services||(p.previous.context&&!p.previous_owner))throw std::invalid_argument("Encounter Trigger ctor requires actual world/property/source providers");}
 std::shared_ptr<TriggerRecord> find(const dh2::world::CanonicalObjectBorrowV1&o){auto i=records.find(o.identity);return i==records.end()?nullptr:i->second;}
};
TriggerConstructors::TriggerConstructors(TriggerConstructorServices p):impl_(std::make_shared<Impl>(std::move(p))){}
dh2::world::CanonicalClassServicesV1 TriggerConstructors::services()const{
 dh2::world::CanonicalClassServicesV1 out;out.context=impl_.get();
 out.construct=[](void*c,const auto&factory,const auto&source,auto&object,std::string&e){auto&s=*static_cast<Impl*>(c);if(!factory.name||std::strcmp(factory.name,"TriggerZone")){if(s.p.previous.construct)return s.p.previous.construct(s.p.previous.context,factory,source,object,e);e="Required actual other MGP/MVP class constructor";return false;}if(!source.source_lease||!source.attribute||!source.source_context){e="Trigger ctor requires actual retained source declaration";return false;}auto record=std::make_shared<TriggerRecord>();record->declaration=source;dh2::world::TriggerZoneServicesV22 services;if(!s.p.source_services(record,services,e))return false;record->receiver=std::make_shared<dh2::world::CanonicalTriggerZoneV22>(s.p.world_owner,record->runtime,std::move(services));record->transport=dh2::world::CanonicalTriggerZoneV22::factory_receiver(record->receiver,source.source_lease);const std::weak_ptr<TriggerRecord> weak=record;
record->transport.source_loading_fields_v95=[weak](auto&out,std::string&e){auto r=weak.lock();if(!r||!r->receiver){e="Actual child loading receiver expired";return false;}auto&b=r->receiver->base();out={};out.receiver=std::shared_ptr<void>(r,r->receiver.get());out.gameobject_base=&b;out.archetype48=b.string(0x48);out.enabled8a=b.byte(0x8a);out.minimum_ec=b.integer(0xec);out.disabled_f1=b.byte(0xf1);out.condition_a8=b.pointer(0xa8);out.tested_ac=b.byte(0xac);out.condition_cc=b.pointer(0xcc);out.tested_d0=b.byte(0xd0);e.clear();return true;};
record->transport.source_is_updatable_v95=[](bool&value,std::string&e){value=true;e.clear();return true;}; // Whole GameObject.IsUpdatable38aac0 literal1.
record->transport.source_init_final_v95=[weak](std::string&e){auto r=weak.lock();if(!r||!r->receiver){e="Actual child InitFinal receiver expired";return false;}return r->receiver->init_final(e);};object=record->transport.object;object.lease=std::shared_ptr<void>(record,record->receiver.get());if(!s.records.emplace(object.identity,record).second){e="Duplicate source Trigger receiver identity";return false;}return true;};
 out.init_properties=[](void*c,const auto&o,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(!r){if(s.p.previous.init_properties)return s.p.previous.init_properties(s.p.previous.context,o,e);e="Unbound source property receiver";return false;}auto a=r->transport.properties();return s.p.properties->init_properties(a,e);};
 out.set_template=[](void*c,const auto&o,const char*name,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(!r){if(s.p.previous.set_template)return s.p.previous.set_template(s.p.previous.context,o,name,e);e="Unbound source template receiver";return false;}auto a=r->transport.properties();return s.p.properties->set_template(a,name,e);};
 out.load_defaults=[](void*c,const auto&o,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(!r){if(s.p.previous.load_defaults)return s.p.previous.load_defaults(s.p.previous.context,o,e);e="Unbound source defaults receiver";return false;}auto a=r->transport.properties();return s.p.properties->load_defaults(a,e);};
 out.load_overrides=[](void*c,const auto&o,const auto&q,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(!r){if(s.p.previous.load_overrides)return s.p.previous.load_overrides(s.p.previous.context,o,q,e);e="Unbound source overrides receiver";return false;}if(q.source_lease.get()!=r->declaration.source_lease.get()||q.source_lease.owner_before(r->declaration.source_lease)||r->declaration.source_lease.owner_before(q.source_lease)){e="Trigger overrides require SAME original declaration lease";return false;}auto a=r->transport.properties();return s.p.properties->load_overrides(a,q,e);};
 out.init_post=[](void*c,const auto&o,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(r)return r->transport.init_post(e);if(s.p.previous.init_post)return s.p.previous.init_post(s.p.previous.context,o,e);e="Unbound source InitPost receiver";return false;};
 out.is_game_object=[](void*c,const auto&o,bool&b,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(r)return r->transport.is_game_object(b,e);if(s.p.previous.is_game_object)return s.p.previous.is_game_object(s.p.previous.context,o,b,e);e="Unbound source GameObject receiver";return false;};
 out.position=[](void*c,const auto&o,auto&pos,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(r)return r->transport.position(pos,e);if(s.p.previous.position)return s.p.previous.position(s.p.previous.context,o,pos,e);e="Unbound source position receiver";return false;};
 out.set_position=[](void*c,const auto&o,const auto&pos,bool changed,std::string&e){auto&s=*static_cast<Impl*>(c);auto r=s.find(o);if(r)return r->transport.set_position(pos,changed,e);if(s.p.previous.set_position)return s.p.previous.set_position(s.p.previous.context,o,pos,changed,e);e="Unbound source SetPosition receiver";return false;};
 out.unknown_type_debug=[](void*c,const char*type,std::string&e){auto&s=*static_cast<Impl*>(c);if(s.p.previous.unknown_type_debug)return s.p.previous.unknown_type_debug(s.p.previous.context,type,e);e="Unsupported original child class type";return false;};return out;
}
std::shared_ptr<void> TriggerConstructors::lease()const{return impl_;}
bool TriggerConstructors::record(std::uintptr_t id,std::shared_ptr<TriggerRecord>&out,std::string&e)const{auto i=impl_->records.find(id);if(i==impl_->records.end()){e="Actual canonical child Trigger record missing";return false;}out=i->second;e.clear();return true;}
}




