#include "player_manage_characters_owner_v70.hpp"
namespace dh2::player {
bool PlayerManageCharactersOwnerV70::offline(std::string& e){bool enabled{};if(!services_.online||!services_.online(enabled,e)){if(e.empty())e="Required GetOnline.byte5 for ManageCharacters";return false;}if(enabled){e="Required whole online ManageCharacters continuation";return false;}return true;}
bool PlayerManageCharactersOwnerV70::assign(PlayerInfoFieldsV1& p,PlayerManageAssignmentV70 kind,std::int32_t value,const std::string* name,std::string& e){if(!services_.assign){e="Required actual PlayerInfo NetStruct assignment";return false;}return services_.assign(p,kind,value,name,e);}
bool PlayerManageCharactersOwnerV70::update(std::string& e){
 if(delivering_||!services_.provider||!manager_.source_initialized_v59()){e="Required nonreentrant constructed PM ManageCharacters receiver";return false;}
 delivering_=true;struct Exit{bool& value;~Exit(){value=false;}} exit{delivering_};
 std::shared_ptr<void> level;const std::uint32_t* phase{};const std::uint8_t* active144{};
 if(!services_.current_level||!services_.current_level(level,phase,active144,e)||(level&&(!phase||!active144))){if(e.empty())e="Required actual current Level phase130 borrow";return false;}
 std::int32_t count{};if(!manager_.num_players(count,e))return false;
 auto* frame=manager_.source_frame_fields_v68();if(!frame){e="Required source PM frame fields";return false;}
 for(std::int32_t index=0;index<count;++index){
  PlayerInfoFieldsV1* p{};if(!manager_.get_player(index,false,p,e))return false;
  bool selected{};if(!p||!services_.selected5c||!services_.selected5c(*p,selected,e)){if(e.empty())e="Required selected PlayerInfo virtual5c";return false;}if(!selected)continue;
  // Source captures Character660 before profile callbacks/AddCharacter. The
  // captured NULL remains NULL even after the original Add stores660.
  const auto captured_character=p->character660;
  PlayerManageFieldsV70 f;if(!services_.fields||!services_.fields(*p,f,e)||!f.receiver||!f.class380||!f.visible4e5||!f.loading525){if(e.empty())e="Required SAME PlayerInfo managed source cells";return false;}
  if(p->local66c){
   if(p->save_slot664!=-1){
    if(!f.profile680||!f.name2d0||!f.level330){e="Required SAME selected local profile680/name/class/level fields";return false;}
    if(!*f.profile680){
     if(!services_.construct_profile680||!services_.construct_profile680(*p,p->save_slot664,*f.profile680,e)){if(e.empty())e="Required original selected profile PlayerSavegame C1(slot,1,false)";return false;}
     if(!*f.profile680){e="Profile680 constructor did not publish actual receiver";return false;}
    }
    bool gc{};if(!services_.game_center_enabled11||!services_.game_center_enabled11(gc,e)){if(e.empty())e="Required GameCenter source byte11";return false;}
    std::string name;
    if(gc){if(!services_.game_center_alias||!services_.game_center_alias(name,e)){if(e.empty())e="Required GameCenter actual alias";return false;}}
    else {bool network_alias{};if(!services_.network_alias_enabled28||!services_.network_alias_enabled28(network_alias,e)){if(e.empty())e="Required NetworkManager source byte28";return false;}
     if(network_alias){if(!services_.network_alias||!services_.network_alias(name,e)){if(e.empty())e="Required actual network profile alias";return false;}}
     else name=(*f.profile680)->name();}
    if(*f.name2d0!=name&&!assign(*p,PlayerManageAssignmentV70::name,0,&name,e))return false;
    if(!captured_character){
     const auto klass=(*f.profile680)->class_id();if(*f.class380!=klass&&!assign(*p,PlayerManageAssignmentV70::character_class,klass,nullptr,e))return false;
     const auto level_value=(*f.profile680)->level();if(*f.level330!=level_value&&!assign(*p,PlayerManageAssignmentV70::level,level_value,nullptr,e))return false;
    }
    std::shared_ptr<void> current;const std::uint32_t* current_phase{};const std::uint8_t* current_active{};
    if(!services_.current_level(current,current_phase,current_active,e)||(current&&(!current_phase||!current_active))){if(e.empty())e="Required refreshed current Level130/144";return false;}
    const bool visible=current&&*current_phase==38&&*current_active;
    if(bool(*f.visible4e5)!=visible&&!assign(*p,PlayerManageAssignmentV70::visible,visible?1:0,nullptr,e))return false;    const bool loading=current&&*current_phase>35;
    if(bool(*f.loading525)!=loading&&!assign(*p,PlayerManageAssignmentV70::loading,loading?1:0,nullptr,e))return false;
   }
  }else{
   if(!services_.remote_changed||!services_.remote_changed(*p,e)){if(e.empty())e="Required actual remote NetStruct HasChanged continuation";return false;}
  }
  if(frame->byte6c9){
   if(!captured_character){if(level&&*f.class380!=-1){if(!offline(e)||!manager_.add_character(p->internal670,e))return false;}}
   else if(!offline(e))return false; // offline existing Character skips network pack/unpack.
  }
 }
 // Original re-queries online at each tail decision; no copied snapshot.
 if(!offline(e)||!offline(e)||!offline(e))return false;
 if(!frame->byte6c9&&*manager_.character_count_field()>0){
  if(!services_.clear_loading_info||!services_.clear_loading_info(e)){if(e.empty())e="Required original ClearLoadingInfo370e00";return false;}
  if(!services_.remove_all_characters||!services_.remove_all_characters(e)){if(e.empty())e="Required original RemoveAllCharacters3721b8";return false;}
 }
 return true;
}
}

