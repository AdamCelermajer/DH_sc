#include "npc_interact.hpp"
#include <utility>
namespace dh::foundation::interactions {
namespace {
template<class F,class... A> bool call(const F& f,const char* what,std::string& e,A&&... a){
    if(!f){e=std::string("Character.Interact requires ")+what;return false;}
    return f(std::forward<A>(a)...,e);
}
bool ui_root(const decltype(NpcInteractServices::hud_root)& f,const char* what,std::uintptr_t& root,
 std::shared_ptr<void>& lease,std::string& e){
    if(!call(f,what,e,root,lease))return false;
    if(!root||!lease){e=std::string("Character.Interact requires SAME retained ")+what;return false;}return true;
}
NpcAsValue number(double v){NpcAsValue out;out.number=v;return out;}
}
bool npc_interact(const NpcInteractBorrow& b,std::uintptr_t actor,const NpcInteractServices& s,std::string& e){
    e.clear();if(!b.receiver||!b.identity||!s.world){e="Character.Interact requires canonical receiver/world leases";return false;}
    bool interacting=false;if(!call(s.is_interacting,"SM_IsInteracting",e,b.identity,interacting))return false;
    if(!interacting){
        std::uintptr_t level=0;if(!call(s.current_level,"Application.GetCurrentLevel",e,level))return false;
        if(!level&&!call(s.assert_missing_level,"original missing-level assertion",e))return false;
        if(!b.room64||!b.data13c8){e="Character.Interact requires SAME room64/data13c8";return false;}
        // Source snapshots both fields before getConstant can mutate them.
        NpcTalkEvent event;event.actor=actor;event.data_id=*b.data13c8;event.room=*b.room64;
        if(!call(s.constant,"PyDataConstants TalkToNPC",e,"v2QuestObjectiveType","TalkToNPC",event.objective_type))return false;
        if(!call(s.raise_async,"SAME Level EventManager.RaiseAsync",e,level,event))return false;
    }
    if(!call(s.set_interact_state,"SM_SetInteractState(3,true,actor,false)",e,b.identity,3,true,actor,false))return false;
    if(!b.talk_flag2fa){e="Character.Interact requires SAME byte2fa";return false;}
    if(*b.talk_flag2fa)return true;
    if(!call(s.raise_character_event,"Character.RaiseEvent(5,actor)",e,b.identity,5,actor))return false;
    bool merchant=false;if(!call(s.is_merchant,"Character.IsMerchant",e,b.identity,merchant))return false;
    if(merchant){
        bool loot=false;if(!call(s.has_loot,"Character.HasLoot",e,b.identity,loot))return false;
        if(!loot||!actor)return true;
        std::uintptr_t character=0;if(!call(s.handle_as_character,"ObjectHandle.AsChar",e,actor,character))return false;
        if(!character)return true;
        std::int32_t loot_id=0;if(!call(s.get_loot,"Character.GetLoot",e,b.identity,loot_id))return false;
        // Borrow actual MerchantTable before GetNumItems, matching capture order.
        MerchantRowBorrow row;if(!call(s.merchant_table,"Arrays.MerchantTable",e,loot_id,row))return false;
        if(!row.table||!row.loot_ids){e="Character.Interact requires retained merchant declaration row";return false;}
        std::int32_t count=0;if(!call(s.num_items,"SAME NPC inventory.GetNumItems",e,b.identity,count))return false;
        if(!count){
            for(std::size_t i=0;i<row.loot_ids->size();++i){
                const auto id=(*row.loot_ids)[i];
                if(!call(s.add_merchant_loot,"SAME NPC inventory.AddLoot(id,0,0,-1,false)",e,b.identity,id))return false;
            }
        }
        std::uintptr_t hud=0;std::shared_ptr<void> hud_lease;
        if(!ui_root(s.hud_root,"MenuManager HUDRoot",hud,hud_lease,e))return false;
        std::int32_t player=0;if(!call(s.player_info_id,"PlayerManager.GetPlayerByCharacter playerInfo678",e,character,player))return false;
        if(!call(s.invoke_as,"RenderFX.InvokeASCallback openMerchantMenu",e,hud,"_root","openMerchantMenu",std::vector<NpcAsValue>{number(player)}))return false;
        if(!b.display_name44||!b.room64){e="Character.Interact requires SAME name44/room64 for merchant infos";return false;}
        NpcAsValue name;name.kind=NpcAsValue::Kind::string;name.string=*b.display_name44;
        // These values are captured before the fresh MenuManager lookup.
        std::vector<NpcAsValue> values{name,number(*b.room64)};
        std::uintptr_t menu=0;std::shared_ptr<void> menu_lease;
        if(!ui_root(s.merchant_root,"MenuManager source f4->138 receiver",menu,menu_lease,e))return false;
        return call(s.invoke_as,"RenderFX.InvokeASCallback AdditionnalInfosForMerchant",e,menu,"_root","AdditionnalInfosForMerchant",values);
    }
    bool cleaner=false;if(!call(s.is_cleaner,"Character.IsCleaner",e,b.identity,cleaner))return false;
    if(!cleaner||!actor)return true;
    std::uintptr_t character=0;if(!call(s.handle_as_character,"ObjectHandle.AsChar",e,actor,character))return false;
    if(!character)return true;
    std::uintptr_t hud=0;std::shared_ptr<void> hud_lease;
    if(!ui_root(s.hud_root,"MenuManager HUDRoot",hud,hud_lease,e))return false;
    std::int32_t player=0;if(!call(s.player_info_id,"PlayerManager.GetPlayerByCharacter playerInfo678",e,character,player))return false;
    return call(s.invoke_as,"RenderFX.InvokeASCallback openCleanerMenu",e,hud,"_root","openCleanerMenu",std::vector<NpcAsValue>{number(player)});
}
}
