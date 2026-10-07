#include "object_identity.hpp"
#include <cstddef>
#include <limits>
namespace dh2::object_identity {
namespace {
template<class T> bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an&&bn&&(x<=y?y-x<an:x-y<bn);
}
}
extern "C" int dh2_object_handle_from_pointer(Handle16* out,Handle16* shared,std::uint32_t frame){
 if(!aligned(out)||(shared&&(!aligned(shared)||overlap(out,sizeof(*out),shared,sizeof(*shared)))))return 1;
 *out={0,UINT32_MAX,0};
 if(shared){shared->frame=frame;*out=*shared;}
 return 0;
}
extern "C" int dh2_object_mark_deleted(Flags8* p){
 if(!p)return 1;
 p->delete_delay=2;p->disabled=1;return 0;
}
extern "C" int dh2_object_set_updating(Flags8* p,std::uint32_t value){
 if(!p)return 1;
 p->updating=static_cast<std::uint8_t>(value);return 0;
}
extern "C" int dh2_object_queue_deletion(DeletionQueue16* q,std::uintptr_t identity){
 if(!aligned(q)||q->count>q->capacity||q->capacity>65536||
    (q->capacity&&(!aligned(q->values)||overlap(q,sizeof(*q),q->values,std::size_t(q->capacity)*sizeof(*q->values)))))return 1;
 for(std::uint32_t i=0;i<q->count;++i)if(q->values[i]==identity)return 0;
 if(q->count==q->capacity)return 1;
 q->values[q->count++]=identity;return 0;
}
extern "C" int dh2_object_target_event(const TargetScript16* state,std::uint32_t event,std::uintptr_t enemy,const TargetServices16* services){
 if(!aligned(state)||state->reserved||event<enemy_spotted||event>target_in_melee_range)return 1;
 static const char* const names[]={"OnEnemySpotted","OnTargetDied","OnTargetOutOfSight","OnTargetInSight","OnTargetOutOfRange","OnTargetInRangedRange","OnTargetInCloseRange","OnTargetInMeleeRange"};
 const std::uint32_t bit=event>=target_out_of_range?4u<<(event-target_out_of_range):0;
 if(bit&&!(state->available_callbacks&bit))return 0;
 if(!aligned(services)||!services->invoke)return 1;
 const bool argument=event==enemy_spotted;
 const TargetCall32 call{state->identity,names[event-1],argument?enemy:0,argument?1u:0u,argument?7u:0u};
 return services->invoke(services->context,&call)?2:0;
}
}
