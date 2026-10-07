#include "native_menu_preview_fx_initialization_v122.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "application_services_owner_v5.hpp"
namespace model_renderer {
bool bind_native_menu_preview_fx_initialization_v122(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const MenuPreviewProcessDomainV121& d,dh2::world::CanonicalCharacterCandidateServicesV60& s,std::string& e){
 auto libraries=d.fx_libraries;
 if(!app||!d.owner||!libraries||app->source_fx_libraries_v63()!=libraries||!libraries->belongs_to(app)||s.world!=d.owner){
  e="Required SAME process VisualFXManager constructor/library owner";return false;
 }
 //The borrowed views retain authored table indices for RegisterSetToLoad;
 //only actual BuildLibraries496bd8 produces the distinct pool/set vectors.
 s.effect_table=&libraries->registration_table_v122();s.effect_queue=&libraries->pending_queue_v63();
 s.character_effects=&libraries->source_tables().characters();
 auto leaf=std::make_shared<dh2::fx::PreloadServices16>(libraries->registration_services_v122());
 //Retain the callback view through the same resource provider's closures;
 //the existing owner pin is not replaced with a second queue/manager.
 s.effect_services=leaf.get();
 auto marker=s.initialize_target_marker_v70;
 s.initialize_target_marker_v70=[libraries,leaf,marker,owner=d.owner](auto& record,bool& present,std::string& e){
  present=false;if(record.services.world!=owner||!record.actor){e="Different process Character at source marker gate";return false;}
  //Original3b51e8..5210: (end20-begin1c)/sizeof(AnimFXSetInfo)==0
  //branches past the whole9-circle/local-marker/highlight block. Genuine
  //C1 creates that empty vector even though Arrays are already loaded.
  if(libraries->sets().empty()){e.clear();return true;}
  if(!marker){e="Required actual positive process target-marker instance provider";return false;}
  return marker(record,present,e);
 };
 if(!s.grab_fx)s.grab_fx=[libraries,leaf](auto& record,std::int32_t id,std::uintptr_t& out,std::string& e){
  if(!record.actor){e="Released source preview GrabAnimFX receiver";return false;}
  const auto result=dh2_character_init_fx_negative_grab(&out,id,
   static_cast<std::uint32_t>(libraries->source_tables().sets().size()),leaf.get());
  if(result==1){e.clear();return true;}
  //Grab495430 and _Get494ad4 do not construct a missing library. A positive
  //authored request still requires its source-produced pool and real backend.
  e=result==-3?"Required positive GrabAnimFX instance over source-produced process pools":"Original process GrabAnimFX Debug delivery failed";return false;
 };
 const auto prior_registration=s.register_character_fx;
 s.register_character_fx=[libraries,leaf,prior_registration](auto& record,std::string& e){
  if(prior_registration)return prior_registration(record,e);
  if(!record.services.character_effects){e="Required actual CharacterEffects rows for source registration";return false;}
  const auto& rows=*record.services.character_effects;
  dh2::character::InitFxRows16 input{rows.data(),static_cast<std::uint32_t>(rows.size()),record.properties->resolved[7]};
  const auto result=dh2_character_init_fx_register(&input,&libraries->registration_table_v122(),&libraries->pending_queue_v63(),leaf.get());
  if(result!=1){e="Original process CharacterFX registration failed: "+std::to_string(result);return false;}e.clear();return true;
 };
 e.clear();return true;
}
}
