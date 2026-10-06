#include "retained_character_position_owner_v7.hpp"
#include <algorithm>
namespace dh2::character {
bool RetainedCharacterPositionOwnerV7::publish_physical(std::uintptr_t actor,std::uintptr_t identity,const physical::NativeBody* backing,bool assigned,std::string& error){
 if(&fields_!=&actor_.position_fields_v7()||!fields_.constructed||!actor_.object||actor!=actor_.object->identity||!identity||!backing){error="Required SAME Character physical assignment receiver";return false;}
 if(assigned){if(!backing->body){error="Source physical assignment requires actual initialized body";return false;}if(fields_.physical2dc&&fields_.physical2dc!=identity){error="Source previous physical receiver requires whole SetPhysicalObject replacement";return false;}fields_.physical2dc=identity;}
 else {if(backing->body){error="Source physical detachment precedes actual body destruction";return false;}if(fields_.physical2dc&&fields_.physical2dc!=identity){error="Different source physical receiver replaced before detachment";return false;}fields_.physical2dc=0;}
 return true;
}
bool RetainedCharacterPositionOwnerV7::set_position(const float* p,bool destination,std::string& error){
 if(&fields_!=&actor_.position_fields_v7()){error="Required sole retained Character position field owner";return false;}
 if(!fields_.constructed){error="Required actual Character physical2dc/anchor2e0 adoption projection";return false;}
 if(!actor_.object||!actor_.object->identity||actor_.shared_handle().cached!=actor_.object->identity){error="Required SAME constructed Character position authority";return false;}
 auto object=actor_.object;CharacterPositionBorrowV7 borrow;
 borrow.receiver=object;borrow.identity=object->identity;borrow.position160=object->position.data();borrow.relative144=actor_.runtime.subobjects.local_bounds;borrow.absolute12c=actor_.runtime.subobjects.absolute_bounds;borrow.destination1a8=actor_.runtime.controller.destination;borrow.attached2e0=&fields_.attached2e0;borrow.physical2dc=&fields_.physical2dc;borrow.visual2d8=&actor_.source_visual();
 borrow.publish_position=[this,object](){std::copy_n(object->position.data(),3,actor_.runtime.subobjects.position);std::copy_n(object->position.data(),3,actor_.runtime.controller.position);std::copy_n(object->position.data(),3,actor_.runtime.object.motion.position);};
 CharacterPositionServicesV7 services;services.world=backends_.world;services.attached_position=backends_.attached_position;services.visual_sync_position=backends_.visual_sync_position;
 services.physical_position=[this](std::uintptr_t identity,float x,float y,std::string& e){
  if(!backends_.physical_body){e="Required SAME Character physical receiver binding";return false;}physical::NativeBody* body=nullptr;if(!backends_.physical_body(identity,body,e))return false;
  if(!body||!body->body){e="Required retained source physical NativeBody";return false;}
  const float xy[2]{x,y};const int status=dh2_native_body_set_position(body,xy);if(status<0){e="Source physical.setPosition invalid input";return false;}
  // Original void setPosition ignores SetXForm's false/frozen return.
  if(dh2_native_body_refresh_view(&actor_.runtime.body,body)){e="Required same physical position observation";return false;}return true;
 };
 return character_set_position_v7(result_,borrow,p,destination,services,error);
}
}
