#include "../character_target_search_v5.hpp"
static int replay_search_v5(dh2::target_search::List40*,const dh2::target_search::Registry8*,float,float,const dh2::target_search::Services16*);
#define dh2_target_search replay_search_v5
#include "character_target_search.cpp"
#undef dh2_target_search
static int replay_search_v5(List40* list,const Registry8* registry,float radius,float cone,const Services16* services){
 // Reuse the exact original SearchEff corpus domain. The new Lua wrapper
 // supplies its own explicit position snapshot; this replay uses the original
 // caller-selected center to isolate equivalence of the remaining core.
 if(!list||!list->owner)return dh2_target_search_snapshot_v5(list,registry,radius,cone,nullptr,services,nullptr);
 const auto* point=list->owner->has_target_position?list->owner->target_position:list->owner->position;
 const float captured[3]={point[0],point[1],point[2]};
 return dh2_target_search_snapshot_v5(list,registry,radius,cone,captured,services,nullptr);
}
