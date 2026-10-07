#include "canonical_character_pf_v62.hpp"
namespace dh2::world {
bool canonical_character_update_pf_v62(CanonicalCharacterCandidateRecordV60& r,const navigation::CollisionWorld& geometry,navigation::ObstacleRegistry& registry,std::string& e){
 if(!r.actor||!r.actor->object){e="Required same canonical Character PF receiver";return false;}
 auto& pf=r.actor->runtime.object;if(pf.motion.floor==UINT32_MAX)return true; //393ea0 source absent-floor guard.
 auto* native=r.physical_owner_v62?&r.physical_owner_v62->native():nullptr;
 const auto slot=r.actor->position_fields_v7().physical2dc;
 if(slot&&(!native||!native->body||slot!=reinterpret_cast<std::uintptr_t>(r.physical_owner_v62.get()))){e="Required same actual Character physical2dc/PF backing";return false;}
 // Character's actual b4/b8/bc leaf implementations: true,50,20. Generic
 // GameObject values are different and deliberately do not pass this route.
 const navigation::ObstacleInitRequest request{&geometry,&registry,&pf,r.actor->object->identity,50.f,20.f,unsigned(slot!=0),0};
 if(dh2_nav_init_obstacle(&request)){e="Actual Character PF obstacle insertion failed or exhausted caller storage";return false;}
 if(slot){pf.radius=native->radius*100.f;return true;}
 const auto* box=r.actor->runtime.subobjects.absolute_bounds;const float x=box[3]-box[0],y=box[4]-box[1];pf.radius=(x<y?y:x)*.5f;return true;
}
bool canonical_character_init_pf_v62(CanonicalCharacterCandidateRecordV60& r,const navigation::CollisionWorld& geometry,const float* p,float radius,std::string& e){
 if(!r.actor||!r.actor->object||!p){e="Required actual Character PF InitObject receiver/position";return false;}
 const auto* stat=r.actor->source_bool_field(0x84);if(!stat){e="Required produced same Character static84";return false;}
 navigation::ObjectInitRequest request{&geometry,&r.actor->runtime.object,r.actor->object->identity,{p[0],p[1],p[2]},radius,unsigned(*stat!=0),0};
 if(dh2_nav_init_object(&request)){e="Actual Character PF InitObject failed";return false;}return true;
}
}
