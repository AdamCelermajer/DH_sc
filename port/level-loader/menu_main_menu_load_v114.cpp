#include "menu_main_menu_load_v114.hpp"
#include <exception>
#include <utility>
namespace dh2::ui {
bool MenuMainLoadV114::load(const MenuMainLoadServicesV114& s,std::string& e){
 if(busy_){if(failure_.empty())failure_="Recursive source LoadMainMenu";e=failure_;return false;}
 if(!failure_.empty()){e=failure_;return false;}
 if(complete_){phase_=singleton_index_=suffix_index_=0;uri_.clear();movie_={};receiver_={};complete_=false;}
 busy_=true;
 struct Scope{
  bool& busy;MenuMovieBorrowV58& movie;MainMenuReceiverV114& receiver;
  std::weak_ptr<void>& movie_storage;std::weak_ptr<void>& receiver_storage;
  ~Scope(){if(movie.actual_owner)movie_storage=movie.actual_owner;if(receiver.owner)receiver_storage=receiver.owner;
   movie.actual_owner.reset();receiver.owner.reset();busy=false;}
 } scope{busy_,movie_,receiver_,movie_storage_,receiver_storage_};
 // Pins cover this immediate source call only. Actual existing slot/registry/
 // singleton owners retain failed native resource prefixes; metadata remains
 // weak so Front->services->kernel cannot cycle back through movie receivers.
 if(phase_>=2&&phase_<=3&&movie_.identity&&!movie_.actual_owner){
  movie_.actual_owner=movie_storage_.lock();if(!movie_.actual_owner){failure_="Retired same RenderFX source prefix";e=failure_;return false;}
 }
 if(((phase_>=5&&phase_<=6)||phase_==8||phase_==9||phase_==11||phase_==12||phase_==17||phase_==19)&&receiver_.identity&&!receiver_.owner){
  receiver_.owner=receiver_storage_.lock();if(!receiver_.owner){failure_="Retired same MenuBase source prefix";e=failure_;return false;}
 }
 const auto fail=[&](std::string why){if(failure_.empty())failure_=why.empty()?"Required actual LoadMainMenu leaf":std::move(why);e=failure_;return false;};
 const auto current=[&](){if(!failure_.empty()){e=failure_;return false;}std::string local;bool ok=false;
  try{ok=s.owner&&s.current&&s.current(local);}catch(const std::exception& x){local=x.what();}catch(...){local="LoadMainMenu current borrower threw";}
  if(!failure_.empty()){e=failure_;return false;}return ok?true:fail(std::move(local));};
 const auto call=[&](auto&& fn,auto&& done){if(!current())return false;std::string local;bool ok=false;
  try{ok=fn(local);}catch(const std::exception& x){local=x.what();}catch(...){local="LoadMainMenu source leaf threw";}
  if(!failure_.empty()){e=failure_;return false;}if(!ok)return fail(std::move(local));
  done(); // successful native effect is complete even if following scope guard fails
  return current();
 };
 const auto none=[](){};
 constexpr MainMenuSingletonV114 suffix[]={MainMenuSingletonV114::main_menu,MainMenuSingletonV114::character_select,
  MainMenuSingletonV114::enter_name,MainMenuSingletonV114::lobby,MainMenuSingletonV114::leaderboard};
 for(;;){
  if(!current())return false;
  switch(phase_){
  case 0:{MainMenuLoadFactsV114 facts;
   if(!call([&](auto& x){if(!s.facts){x="Required actual Width_Screen facts";return false;}return s.facts(facts,x);},none))return false;
   if(!facts.owner||!facts.width_screen)return fail("Missing same live source Width_Screen cell");
   const auto width=*facts.width_screen;
   if(width==854)uri_="data/menus/dqmenus_droid.swf";
   else if(width==960)uri_="data/menus/dqmenus.swf";
   else if(width==800){
    if(!facts.htc_devices)return fail("Required reached HTC_Devices byte");
    if(*facts.htc_devices)uri_="data/menus/dqmenus_i9000.swf";
    else {if(!facts.no_igp)return fail("Required reached No_IGP byte");
     uri_=*facts.no_igp?"data/menus/dqmenus_i9000_lg.swf":"data/menus/dqmenus_i9000.swf";}
   }else{
    std::int32_t language{};
    if(!call([&](auto& x){if(!s.save_language){x="Required SAME SavegameManager.getLanguage";return false;}return s.save_language(language,x);},none))return false;
    if(language==5)uri_="data/menus/dqmenus_Kor.swf";
    else {
     // Original reloads SAME Save owner and calls getLanguage again.
     if(!call([&](auto& x){return s.save_language(language,x);},none))return false;
     uri_=language==4?"data/menus/dqmenus_jp.swf":"data/menus/dqmenus.swf";
    }
   }
   phase_=1;break;}
  case 1:
   if(!call([&](auto& x){if(!s.load_slot2){x="Required actual MultiMenu.LoadSWFFile(slot2)";return false;}return s.load_slot2(uri_.c_str(),movie_,x);},[&](){phase_=2;}))return false;
   break;
  case 2:
   if(!movie_.identity||!movie_.actual_owner)return fail("Native LoadMainMenu positive RenderFX2 lacks actual lease");
   if(!call([&](auto& x){if(!s.set_behavior){x="Required actual RenderFX.SetInputBehavior84";return false;}return s.set_behavior(movie_,0x84,x);},[&](){phase_=3;}))return false;
   break;
  case 3:
   if(!call([&](auto& x){if(!s.update_render){x="Required actual RenderFX.virtual10(1,false)";return false;}return s.update_render(movie_,1,false,x);},[&](){phase_=4;}))return false;
   break;
  case 4:
   if(!call([&](auto& x){if(!s.singleton){x="Required actual MenuDebug.GetInstance";return false;}return s.singleton(MainMenuSingletonV114::debug,receiver_,x);},none))return false;
   phase_=5;break;
  case 5:
   if(!receiver_.owner||!receiver_.identity)return fail("Native MenuDebug singleton returned NULL");
   if(!call([&](auto& x){if(!s.weak_live){x="Required actual MenuDebug weak root";return false;}return s.weak_live(receiver_,weak_live_,x);},[&](){phase_=6;}))return false;
   break;
  case 6:
   if(!weak_live_&&!call([&](auto& x){if(!s.initialize){x="Required genuine MenuDebug.Init";return false;}return s.initialize(MainMenuSingletonV114::debug,receiver_,x);},[&](){phase_=7;}))return false;
   phase_=7;break;
  case 7:
   if(!call([&](auto& x){return s.singleton(MainMenuSingletonV114::debug,receiver_,x);},none))return false;
   phase_=8;break;
  case 8:
   if(!call([&](auto& x){return s.weak_live(receiver_,weak_live_,x);},[&](){phase_=9;}))return false;
   break;
  case 9:
   if(!weak_live_)return fail("Unsupported original NULL MenuDebug character at visible9b store");
   if(!call([&](auto& x){if(!s.hide_debug_character){x="Required SAME gameswf Debug character.visible9b";return false;}return s.hide_debug_character(receiver_,x);},[&](){phase_=10;}))return false;
   break;
  case 10:
   if(singleton_index_==5){phase_=14;break;}
   if(!call([&](auto& x){return s.singleton(suffix[singleton_index_],receiver_,x);},none))return false;
   phase_=11;break;
  case 11:
   if(!receiver_.owner||!receiver_.identity)return fail("Native LoadMainMenu singleton returned NULL");
   if(!call([&](auto& x){return s.weak_live(receiver_,weak_live_,x);},[&](){phase_=12;}))return false;
   break;
  case 12:
   if(weak_live_){++singleton_index_;phase_=10;break;}
   // CharacterSelect reaches its genuine Init; remaining classes RegisterMenu.
   if(!call([&](auto& x){
    if(suffix[singleton_index_]==MainMenuSingletonV114::character_select){
     if(!s.initialize){x="Required actual MenuCharacterSelect.Init";return false;}
     return s.initialize(suffix[singleton_index_],receiver_,x);
    }
    if(!s.register_menu){x="Required actual MenuManager.RegisterMenu";return false;}
    return s.register_menu(receiver_,x);
   },[&](){++singleton_index_;phase_=10;}))return false;
   break;
  case 14:
   if(!call([&](auto& x){if(!s.registry_count){x="Required actual manager64/68 before PostLoad";return false;}return s.registry_count(suffix_index_,x);},none))return false;
   phase_=15;break;
  case 15:
   if(!call([&](auto& x){if(!s.post_load){x="Required genuine MenuManager.PostLoad";return false;}return s.post_load(x);},[&](){phase_=16;}))return false;
   break;
  case 16:{std::uint32_t count{};
   if(!call([&](auto& x){return s.registry_count(count,x);},none))return false;
   if(suffix_index_>=count){complete_=true;e.clear();return true;}
   if(!call([&](auto& x){if(!s.registry_at){x="Required live appended manager64 entry";return false;}return s.registry_at(suffix_index_,receiver_,x);},none))return false;
   phase_=17;break;}
  case 17:
   if(!receiver_.owner||!receiver_.identity)return fail("Native PostLoad appended NULL menu");
   if(!call([&](auto& x){if(!s.register_drag_and_drops){x="Required actual RegisterDragAndDrops(NULL)";return false;}return s.register_drag_and_drops(receiver_,x);},[&](){phase_=18;}))return false;
   break;
  case 18:
   // Original reloads manager64[index] AFTER actual drag registration.
   if(!call([&](auto& x){return s.registry_at(suffix_index_,receiver_,x);},none))return false;
   phase_=19;break;
  case 19:
   if(!receiver_.owner||!receiver_.identity)return fail("Native drag callback left NULL registry entry");
   if(!call([&](auto& x){if(!s.hide_virtual10){x="Required selected appended menu.virtual10 Hide";return false;}return s.hide_virtual10(receiver_,x);},[&](){++suffix_index_;phase_=16;}))return false;
   break;
  default:return fail("Invalid retained LoadMainMenu source continuation");
  }
 }
}
bool MenuMainLoadV114::arm_resume(std::string& e){
 if(busy_){e="Cannot arm active LoadMainMenu recovery";return false;}
 failure_.clear();e.clear();return true;
}
bool MenuMainLoadV114::resume(const MenuMainLoadServicesV114& s,std::string& e){
 if(busy_){e="Cannot resume active actual LoadMainMenu";return false;}
 if(failure_.empty()){e="No failed retained LoadMainMenu body";return false;}
 failure_.clear();return load(s,e);
}
}
