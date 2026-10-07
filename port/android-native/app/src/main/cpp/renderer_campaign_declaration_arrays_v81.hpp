#pragma once
#include <game_object_arrays_owner_v81.hpp>
#include <production_noncharacter_composition_v67.hpp>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Same process RendererImmutableTables slot and actual World CFS read authority.
bool borrow_source_campaign_game_object_arrays_v81(const SourceCampaignCandidateBorrowV55&,dh2::loader::GameObjectArraysOwnerV81::Borrow&,std::string&);
template<class Self>
bool bind_campaign_declaration_arrays_v81(const std::weak_ptr<Self>& weak,dh2::loader::ProductionNonCharacterServicesV67& services,const dh2::loader::GameObjectArraysOwnerV81::Borrow& arrays,std::string& e){
 auto self=weak.lock();if(!self||!self->current(e)||!arrays.receiver||!arrays.dictionary||!arrays.doors.names||!arrays.doors.rows||!arrays.triggers.names||!arrays.triggers.rows||!arrays.openable||!arrays.destructible){if(e.empty())e="Required SAME initialized grouped game_objects owner";return false;}
 const auto same=[](const auto& a,const auto& b){return a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if((services.containers.openable_table&&!same(services.containers.openable_table,arrays.openable))||(services.containers.destructible_table&&!same(services.containers.destructible_table,arrays.destructible))){e="Competing game_objects container table owner";return false;}
 services.containers.openable_table=arrays.openable;services.containers.destructible_table=arrays.destructible;
 auto door=services.auxiliary.door;
 if(door)services.auxiliary.door=[weak,door](const auto& delivery,auto& receiver,auto& out,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!door(delivery,receiver,out,e))return false;
  const auto source=out.startup.borrow_tables;
  out.startup.borrow_tables=[weak,source](dh2::world::DoorTableBorrowV77& out,std::string& e){auto self=weak.lock();if(!self||!self->current(e))return false;const auto& actual=self->native.declaration_arrays.doors;
   if(source){dh2::world::DoorTableBorrowV77 prior;if(!source(prior,e))return false;if(prior.names!=actual.names||prior.rows!=actual.rows||prior.objects.get()!=actual.objects.get()||prior.receiver.owner_before(actual.receiver)||actual.receiver.owner_before(prior.receiver)){e="Door provider replaced SAME original Arrays snapshot";return false;}}
   out=actual;e.clear();return true;};return true;};
 auto trigger=services.auxiliary.trigger_object;
 if(trigger)services.auxiliary.trigger_object=[weak,trigger](const auto& delivery,auto& receiver,auto& out,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!trigger(delivery,receiver,out,e))return false;
  const auto source=out.startup.borrow_tables;
  out.startup.borrow_tables=[weak,source](dh2::world::TriggerObjectTableBorrowV78& out,std::string& e){auto self=weak.lock();if(!self||!self->current(e))return false;const auto& actual=self->native.declaration_arrays.triggers;
   if(source){dh2::world::TriggerObjectTableBorrowV78 prior;if(!source(prior,e))return false;if(prior.names!=actual.names||prior.rows!=actual.rows||prior.objects.get()!=actual.objects.get()||prior.receiver.owner_before(actual.receiver)||actual.receiver.owner_before(prior.receiver)){e="Trigger provider replaced SAME original Arrays snapshot";return false;}}
   out=actual;e.clear();return true;};return true;};
 e.clear();return true;
}
}
