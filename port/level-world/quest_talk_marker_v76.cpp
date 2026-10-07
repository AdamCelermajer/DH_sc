#include "quest_talk_marker_v76.hpp"
#include <cstring>
namespace dh2::world {namespace {
bool match(data::QuestObjectivePersistenceV51& q,const QuestTalkMarkerServicesV76& s,
 QuestMarkerCharacterBorrowV76& out,std::string& e){
 if(!s.provider||!s.first_character||!q.words24_2c[0]){e="Required actual TalkToNPC compiled24/ordered Character list";return false;}
 std::int32_t id;const auto word=*q.words24_2c[0];std::memcpy(&id,&word,4);
 if(!s.first_character(id,out,e))return false;
 if(out.identity&&(!out.receiver||!out.store_flags)){e="Talk marker selected NPC without SAME retained field receiver";return false;}
 return true;
}
bool fx_owner(const QuestTalkMarkerServicesV76& s,std::shared_ptr<void>& lease,
 fx::CharacterMeshFxOwnerV4*& fx,std::string& e){
 if(!s.actual_fx||!s.actual_fx(lease,fx,e)||!lease||!fx){if(e.empty())e="Required SAME fresh campaign animated-FX instance";return false;}return true;
}
}
bool quest_remove_talk_marker_v76(data::QuestObjectivePersistenceV51& q,const QuestTalkMarkerServicesV76& s,std::string& e){
 if(!q.compiled8||!q.definition||q.definition->type!=5)return true;
 QuestMarkerCharacterBorrowV76 npc;if(!match(q,s,npc,e))return false;
 if(!npc.identity)return true; //47dad8 first-match miss returns before flags/FX
 npc.store_flags(0,0); //47dae8/47daec, before source marker28 query
 if(!q.marker28)return true;
 std::shared_ptr<void> lease;fx::CharacterMeshFxOwnerV4* fx{};if(!fx_owner(s,lease,fx,e))return false;
 if(!fx->marker_anchor_v28(q.marker28,0,true,e))return false;
 if(!s.visual_owner204||!s.visual_owner204(*fx,q.marker28,0,e)){if(e.empty())e="Required actual marker visual204 clear";return false;}
 //Source keeps the pointer28 and animated-FX allocation, sets visibilityfalse.
 return fx->marker_visible_v28(q.marker28,false,e);
}
bool quest_install_talk_marker_v76(data::QuestObjectivePersistenceV51& q,std::int32_t priority,
 std::int32_t state,const QuestTalkMarkerServicesV76& s,std::string& e){
 if(!q.compiled8||!q.definition||q.definition->type!=5)return true;
 QuestMarkerCharacterBorrowV76 npc;if(!match(q,s,npc,e))return false;
 if(!npc.identity)return true; //47d95c source genuine NULL/miss branch
 if(q.marker28&&!quest_remove_talk_marker_v76(q,s,e))return false;
 npc.store_flags(1,priority==3?1:0); //47d998/47d99c, before settings/Grab
 const std::int32_t offset=state>7?(priority==1?0x70:0x68):(priority==1?0x6c:0x64);
 std::int32_t effect{};
 if(!s.settings_effect||!s.settings_effect(offset,effect,e)){if(e.empty())e="Required actual DesignSettings quest marker effect field";return false;}
 std::shared_ptr<void> lease;fx::CharacterMeshFxOwnerV4* fx{};if(!fx_owner(s,lease,fx,e))return false;
 std::uintptr_t marker{};if(!fx->grab_marker_v28(effect,0,marker,e))return false;
 q.marker28=marker; //47d9cc/47da54 immediately publish genuine nullable return
 if(!marker)return true;
 if(!fx->marker_anchor_v28(marker,npc.identity,true,e))return false;
 if(!s.visual_owner204||!s.visual_owner204(*fx,marker,npc.identity,e)){if(e.empty())e="Required actual marker visual204 publication";return false;}
 if(!fx->marker_visible_v28(marker,true,e))return false;
 if(!s.animator_loop||!s.animator_loop(*fx,marker,true,e)){if(e.empty())e="Required actual marker GetAnimator/SetLooping";return false;}
 return true;
}
}
