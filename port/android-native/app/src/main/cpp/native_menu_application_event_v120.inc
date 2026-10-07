//Included after the existing process movie directory/NativeCharacterMenu.
#include <event_manager_owner_v12.hpp>
#include <visual_anim_controller_owner_v4.hpp>
#include <source_menu_consumption_v121.hpp>
#include <array>
#include <cmath>
namespace {
[[maybe_unused]] void source_menu_consume_event_v120(NativeCharacterMenuV4& native){native.fields.source_consume_event_v120();}
bool source_menu_application_event_v119(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 NativeCharacterMenuV4& native,const dh2::events::EventBorrowV12& event,std::int32_t& result,std::string& e){
 result=0;
 if(!app||app!=application_services_v5||native.process_application_v104.lock()!=app){e="App input addressed another actual process MenuManager";return false;}
 auto debug=[&](const char* name,bool& enabled){std::int32_t word{};if(!native.process_debug_v115||!native.process_debug_v115(name,word,e))return false;enabled=word!=0;return true;};
 bool disabled{};if(!debug("IsDeactivatingFlashMenus",disabled))return false;if(disabled){e.clear();return true;}
 if(!debug("IsDeactivatingFlashMenusUpdate",disabled))return false;if(disabled){e.clear();return true;}
 native.fields.source_event_consumed_ac_v120()=0;native.fields.source_current_event_b4_v120()=event.identity;
 if(!event.identity){e.clear();return true;}
 dh2::ui::SourceMenuConsumptionV121 consumption(&native,[](void* raw,std::string& error){source_menu_consume_event_v120(*static_cast<NativeCharacterMenuV4*>(raw));error.clear();return true;});
 if(!event.get_type){e="Required actual IEvent.GetType";return false;}
 auto type=[&](std::int32_t& value){return event.get_type(event.context,value,e);};
 auto input=[&](dh2::events::InputEventProjectionV120& out){if(!event.input_v120){e="Required genuine typed App input payload producer";return false;}return event.input_v120(event.context,out,e);};
 float x=0,y=0;std::int32_t kind{};
 //Whole GetRawCurrentEventPosition: type is re-read at each source query.
 if(!type(kind))return false;if(kind==4){dh2::events::InputEventProjectionV120 p;if(!input(p))return false;x=p.x8;y=p.y_a;}
 else {if(!type(kind))return false;if(kind==5){dh2::events::InputEventProjectionV120 p;if(!input(p))return false;x=p.x8;y=p.y_a;}}
 consumption.x=static_cast<int>(x);consumption.y=static_cast<int>(y);
 std::int32_t cursor=0,pressed=0;bool key=false;dh2::events::InputEventProjectionV120 payload;
 if(!type(kind))return false;
 if(kind==4){if(!input(payload))return false;cursor=payload.index_c;pressed=payload.pressed10;}
 else {if(!type(kind))return false;if(kind==5){if(!input(payload))return false;cursor=payload.index_c;pressed=1;}
  else {if(!type(kind))return false;key=kind==7;if(key&&!input(payload))return false;}}
 if((kind==4||kind==5)&&(static_cast<std::uint32_t>(cursor)>3u||(cursor&&native.fields.manager_multitouch110_v4()))){native.fields.source_current_event_b4_v120()=0;result=1;e.clear();return true;}
 auto provider=source_postmovie_v62.lock();auto directory=provider?provider->directory:nullptr;
 if(!directory){e="Required SAME four-movie process directory";return false;}
 if(key){
  const char* name="";static constexpr const char* names[]{"BTN_1","BTN_2","BTN_3","BTN_4","BTN_5","BTN_6","BTN_L1","BTN_L2","BTN_R1","BTN_R2","BTN_UP","BTN_LEFT","BTN_DOWN","BTN_RIGHT","BTN_START","BTN_SELECT"};
  if(payload.button8>=0&&payload.button8<16)name=names[payload.button8];else if(payload.button8==19)name="BTN_LEFT_CLICK";else if(payload.button8==22)name="BTN_RIGHT_CLICK";else if(payload.button8>=37&&payload.button8<=44){static constexpr const char* axes[]{"BTN_AL_UP","BTN_AL_LEFT","BTN_AL_DOWN","BTN_AL_RIGHT","BTN_AR_UP","BTN_AR_LEFT","BTN_AR_DOWN","BTN_AR_RIGHT"};name=axes[payload.button8-37];}
  bool ignored{};if(!debug("isTracingMenuManager",ignored))return false;
  struct Call {const char* name;std::int32_t first,second;};Call call{name,payload.value_c,payload.value10};
  for(std::uint32_t slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 receipt;if(!directory->movie_slot(slot,receipt,e))return false;if(!receipt.identity)continue;dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;if(!directory->movie(slot,receipt,movie,pin,e)||!movie||!pin)return false;
   //The directory loans the actual live movie generation; invocation resolves
   //its own root weakly in that same graph, never a retained foreign AS value.
   if(!movie->menu_action_script(&call,[](void* raw,auto& graph,auto& e){auto& p=*static_cast<Call*>(raw);return graph.invoke_renderfx("_root","onInputEvent",{dh2::ui::SwfAsValue::text(p.name),dh2::ui::SwfAsValue::number(p.first),dh2::ui::SwfAsValue::number(p.second)},e);},e))return false;
  }
  cursor=0;
 }
 auto in_game=[&](bool& out){dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!model_renderer::borrow_current_native_level_v27(level,e))return false;if(!level){out=false;return true;}if(!model_renderer::borrow_current_native_level_v27(level,e)||!level){e="Original repeated current-Level lookup became NULL";return false;}out=level.level()->constructor_fields_v3().byte198!=0;return true;};
 for(std::uint32_t slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 receipt;if(!directory->movie_slot(slot,receipt,e))return false;if(!receipt.identity)continue;
  if(slot!=3){bool game{};if(!in_game(game))return false;if(game)continue;if(slot==2&&cursor>0)continue;}
  else {dh2::ui::MenuMovieBorrowV58 main;if(!directory->movie_slot(2,main,e))return false;auto* render=main.identity&&native.shared_stack_v27?native.shared_stack_v27->render(main.identity):nullptr;if(main.identity&&!render){e="Actual main movie lacks SAME source state-array loan";return false;}if(render&&render->count)break;bool game{};if(!in_game(game))return false;if(!game)break;}
  if(slot==2){auto online=app->get_online_loading_v55();if(!online){e="Required actual GetOnline for main input";return false;}if(online->byte5()){e="Required original Matching byte28 main-menu input gate";return false;}if(!dh2::world::source_animation_scaling_enabled_v99())continue;}
  dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;if(!directory->movie(slot,receipt,movie,pin,e)||!movie||!pin)return false;
  if(!movie->input_cursor_slot_v120({x,y,0.f,pressed},static_cast<std::uint32_t>(cursor),e))return false;
  if(native.fields.source_event_consumed_ac_v120()==1)break;
 }
 result=native.fields.source_event_consumed_ac_v120();native.fields.source_current_event_b4_v120()=0;e.clear();return true;
}
// Android frontend pointer identities are mapped to the actual TouchScreen
// slots. This transport does not claim to run the original platform queue.
struct NativeAppPointerTransportV121 {
 std::weak_ptr<NativeCharacterMenuV4> owner;
 std::array<int,8> physical{{-1,-1,-1,-1,-1,-1,-1,-1}};
 std::array<std::int16_t,8> x{},y{};
};
NativeAppPointerTransportV121 native_app_pointer_v121;
bool dispatch_source_app_touch_v121(int phase,int pointer,float x,float y,std::string& e){
 auto menu=authored_character_menu;auto app=application_services_v5;
 if(!menu||!app||menu->process_application_v104.lock()!=app||!app->events14()){
  e="Source App touch requires actual process menu/EventManager14";return false;
 }
 auto& t=native_app_pointer_v121;
 if(t.owner.lock()!=menu){t=NativeAppPointerTransportV121{};t.owner=menu;}
 auto& active=menu->touch.active_source_bytes();
 for(std::size_t i=0;i<8;++i)if(!active[i])t.physical[i]=-1;
 struct Packet {std::int32_t type;dh2::events::InputEventProjectionV120 fields;};
 auto deliver=[&](std::size_t slot,bool down,bool move){
  auto packet=std::make_shared<Packet>();packet->type=move?5:4;
  packet->fields.x8=t.x[slot];packet->fields.y_a=t.y[slot];
  packet->fields.index_c=static_cast<std::int32_t>(slot);packet->fields.pressed10=down?1:0;
  dh2::events::EventBorrowV12 event;
  event.identity=reinterpret_cast<std::uintptr_t>(packet.get());event.context=packet.get();event.lifetime=packet;
  event.get_type=[](void* raw,std::int32_t& out,std::string& error){out=static_cast<Packet*>(raw)->type;error.clear();return true;};
  event.input_v120=[](void* raw,dh2::events::InputEventProjectionV120& out,std::string& error){out=static_cast<Packet*>(raw)->fields;error.clear();return true;};
  return app->events14()->raise(event,e);
 };
 if(phase==3){
  // Actual release ordering is ascending source slot; preserve the reached
  // inactive store even if synchronous event delivery fails.
  for(std::size_t i=0;i<8;++i)if(active[i]){active[i]=0;t.physical[i]=-1;if(!deliver(i,false,false))return false;}
  e.clear();return true;
 }
 if(phase<0||phase>2||pointer<0||!std::isfinite(x)||!std::isfinite(y)||x<-32768.f||x>32767.f||y<-32768.f||y>32767.f){e="Invalid Android source pointer payload";return false;}
 std::size_t slot=8;
 for(std::size_t i=0;i<8;++i)if(t.physical[i]==pointer){slot=i;break;}
 if(phase==0&&slot==8)for(std::size_t i=0;i<8;++i)if(!active[i]){slot=i;t.physical[i]=pointer;break;}
 if(slot==8){e.clear();return true;} // Released/reset pointer cannot resurrect.
 t.x[slot]=static_cast<std::int16_t>(x);t.y[slot]=static_cast<std::int16_t>(y);
 if(phase==0)active[slot]=1;
 if(phase==2){active[slot]=0;t.physical[slot]=-1;}
 return deliver(slot,phase!=2,phase==1);
}
}
