#include "source_campaign_script_environment_v120.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_fx_v77.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_noncharacter_owners_v105.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_object_manager_v1.hpp>
#include <script_command_receivers_v59.hpp>
#include <catalog_auxiliary_v67.hpp>
namespace model_renderer {namespace {
bool need(const char* name,std::string& e){if(e.empty())e=std::string("Required actual environment command ")+name;return false;}
bool door(const SourceCampaignCandidateBorrowV55& c,std::uintptr_t id,std::shared_ptr<dh2::world::CanonicalDoorV27>& out,std::string& e){
 std::shared_ptr<dh2::loader::ProductionNonCharacterOwnersV67> owners;if(!borrow_source_campaign_noncharacter_owners_v105(c,owners,e)||!owners||!owners->auxiliary)return false;
 auto* families=owners->auxiliary->source_families_v105();if(!families)return need("SAME Door factory",e);
 for(const auto& record:families->doors())if(record&&record->owner&&record->owner->base().identity()==id){out=std::shared_ptr<dh2::world::CanonicalDoorV27>(record,record->owner.get());return true;}
 return need("retained SAME Door receiver",e);
}
}
bool execute_source_campaign_script_environment_v120(const SourceCampaignCandidateBorrowV55& c,const dh2::loader::CheckedCommandBorrowV59& q,bool skip,std::int32_t module,bool& handled,std::string& e){
 handled=false;if(!q.kind8||!q.actual_data||!q.actual_receiver)return need("actual receiver/Data",e);const auto kind=*q.kind8;if(kind<53||kind>59)return true;handled=true;
 std::shared_ptr<SourceWorldBorrowV61> w;if(!borrow_source_campaign_condition_world_v70(c,w,e)||!w->debug||!w->debug_files)return false;unsigned ignored{};
 if(dh2_character_debug_load(w->debug.get(),w->debug_files)!=1||dh2_character_debug_get(&ignored,w->debug.get(),"isTracingScriptCmd",w->debug_files)!=1)return need("original Debug prefix",e);
 const auto* name=q.actual_data->cstring(kind==53?16:12);if(!name||!c.objects)return need("actual ObjectManager/name",e);
 dh2::target_providers::Handle16 handle{};const dh2::world::CanonicalObjectBorrowV1* object{};
 if(!c.objects->by_name(name,module,false,nullptr,handle,e)||!c.objects->resolve_handle_v4(handle,false,object,{},e))return false;
 if(kind==53){std::uintptr_t id{};if(object){if(!object->as_character||!object->as_character(object->context,id,e))return false;}auto* value=q.actual_data->scalar(8);if(!value||value->width!=1)return need("source SetStatic byte8",e);if(!value->bits){e.clear();return true;}
  if(!id)return need("original SetStatic positive NULL Character unsafe access",e);SourceCampaignCharacterBorrowV62 r;dh2::world::GameObjectInitializationFieldsV62 f;if(!borrow_source_campaign_character_v62(c.actual_world,id,r,e)||!r.character||!r.character->actor->inherited_initialization_fields_v62(r.character,f,e)||!f.byte)return false;auto* cell=f.byte(0x84);if(!cell)return need("SAME Character static84",e);*cell=1;e.clear();return true;
 }
 const std::uint32_t type=kind<=55?2u:kind<=57?17u:18u;
 if(!object){e.clear();return q.actual_receiver->write_operand_pointer(16,0,{},e);}
 if(!object->type_f4)return need("actual selected GO_IDf4",e);
 if(*object->type_f4!=type){e.clear();return q.actual_receiver->write_operand_pointer(16,0,{},e);}
 if(!object->lease)return need("actual target allocation lease",e);
 //The native receiver stores the same source raw pointer with its actual
 //allocation loan; no pointer truncation, alternate object or strong pool.
 auto alias=std::shared_ptr<void>(object->lease,reinterpret_cast<void*>(object->identity));
 if(!q.actual_receiver->write_operand_pointer(16,object->identity,std::move(alias),e))return false;
 if(kind<=55){auto* wait=q.actual_data->scalar(16);if(!wait||wait->width!=1)return need("Door command wait byte",e);if(!q.actual_receiver->write_operand_word(20,1,wait->bits,e))return false;std::shared_ptr<dh2::world::CanonicalDoorV27> actual;if(!door(c,object->identity,actual,e))return false;return kind==54?actual->source_opened_v91(skip,e):actual->source_closed_v91(skip,e);}
 std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow_source_campaign_object_base_v77(c,object->identity,pin,base,e)||!base||base->identity()!=object->identity)return false;
 //Original activation is a byte store, not TriggerTrap15.Activate or a new
 //physical/timer owner. A real derived field loan must exist for17/18.
 auto* active=base->byte(type==17?0x40c:0x779);if(!active)return need(type==17?"ProjectileTrap17 actual active40c backing":"TriggerPlate18 actual active779 backing",e);
 *active=(kind==56||kind==58)?1:0;e.clear();return true;
}
bool blocking_source_campaign_script_environment_v120(const SourceCampaignCandidateBorrowV55& c,const dh2::loader::CheckedCommandBorrowV59& q,bool& handled,bool& blocked,std::string& e){
 handled=false;if(!q.kind8||!q.actual_receiver)return need("actual blocking receiver",e);const auto kind=*q.kind8;if(kind<53||kind>59)return true;handled=true;blocked=false;
 //455810/4558bc/c4/cc/d4 are literal MOV r0,#0/BX LR.
 if(kind==53||kind>=56){e.clear();return true;}
 std::uintptr_t id{};if(!q.actual_receiver->read_operand_pointer(16,id,e))return false;if(!id){e.clear();return true;}std::uint32_t wait{};if(!q.actual_receiver->read_operand_word(20,1,wait,e))return false;if(!wait){e.clear();return true;}
 std::shared_ptr<dh2::world::CanonicalDoorV27> actual;if(!door(c,id,actual,e))return false;const auto state=actual->source_state3a8();const bool opening=state==1||state==3;blocked=kind==54?!opening:opening;e.clear();return true;
}
}
