#include "renderer_character_campaign_v62.hpp"
#include "source_process_arrays_v101.hpp"
#include "gameswf/gameswf_sprite.h"
namespace {
enum class MenuSingletonKindV67:unsigned {debug,main,merchant,select_class,enter_name,lobby,inventory,inventory_details,world_map,confirm,hud_confirm,leaderboard};
struct NativeMenuSingletonV67 {
 MenuSingletonKindV67 kind;dh2::ui::AuthoredMenuFieldsV1 fields;dh2::ui::AuthoredCharacterStateV1 state;dh2::ui::MenuStackCharacterV1 projection{};
 std::array<dh2::ui::AuthoredMenuWeakCharacterV59,4> cached;
 std::uint8_t select_f4{};std::int32_t select160{-1};std::optional<std::int32_t> select164;
 std::optional<std::uint8_t> merchant_c4; //433548 is this byte's real Update producer
 std::uintptr_t confirm_callback_c4{}; //MenuConfirmBase C2 42ab8c writes NULL.
 struct MapConstructor {
  std::uint8_t cc{},byte12c{};std::uint32_t d0{},dc{},f0{},f4{},word128{};
  const void* d4{&cc};const void* d8{&cc};const void* e4{};
  struct Row{float scale_f8{};std::int32_t fc{},word100{};};std::array<Row,4> rows;
  dh2::ui::AuthoredMenuWeakCharacterV59 limit_e8,buttons_ec;
  std::array<std::optional<std::int32_t>,6> locker108_124;
  float scalar130{1.f},scalar134{1.f},scalar138{1.5f},scalar13c{0.0078125f};
 } map;
 //MenuWorldMap C1 installs its distinct embedded IEventReceiver at+c8.
 //Native identity is its actual member allocation, not ARM32 offset arithmetic.
 struct WorldMapEventReceiverV119 {} worldmap_event_c8_v119;
 bool constructor_attempted{},constructed{};std::string failure;
 explicit NativeMenuSingletonV67(MenuSingletonKindV67 k):kind(k){
  constexpr const char* names[]{"menu_Debug","menu_MainMenu","menu_Merchant","menu_SelectClass","menu_EnterName","menu_MultiplayerLobbyMulti","menu_InventorySheetMain","menu_InventorySheetDetails","menu_FastTravel","menu_confirm","menu_hud_confirm","Multiplayer_Leaderboard"};
  fields.identity=reinterpret_cast<std::uintptr_t>(this);fields.name=names[static_cast<unsigned>(k)];state.fields=&fields;map.e4=this;
 }
};
std::array<std::shared_ptr<NativeMenuSingletonV67>,12> source_singleton_v67;
bool source_worldmap_initialize_v68(NativeMenuPostMovieV62& provider,NativeMenuSingletonV67& owner,std::string& e){
 for(std::uint32_t slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!provider.directory->movie_slot(slot,q,e))return false;
  if(q.identity!=owner.fields.render)continue;dh2::ui::SwfMovie* movie{};std::shared_ptr<void> lease;
  if(!provider.directory->movie(slot,q,movie,lease,e))return false;
  struct Init {NativeMenuPostMovieV62& provider;NativeMenuSingletonV67& owner;
   static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Init*>(raw);using namespace dh2::ui;
    SwfAsValue root;bool live{};if(!q.owner.state.weak_character_v59.borrow(graph,root,live,e)||!live){if(e.empty())e="WorldMap.Init needs SAME live registered weak root";return false;}
    AuthoredMenuSearchIndexV1 index;if(!index.initialize(graph,root,e))return false;
    AuthoredMenuCharacterProjectionV4 projection;AuthoredMenuCharacterBorrowV3* tree{};
    if(!projection.root(graph,q.owner.fields.name,tree,e))return false;
    std::vector<AuthoredMenuCharacterBorrowV3*> actual;if(!authored_menu_collect_characters_v3(tree,nullptr,0,actual,e))return false;
    auto find=[&](const std::string& name,SwfAsValue& out,bool& found){return index.find(name.c_str(),out,found,e);};
    auto bounds=[&](const SwfAsValue& value,AuthoredMenuDeadZoneV3& out){for(auto* entry:actual)if(entry->identity==value.identity())return projection.absolute_bounds(*entry,out,e);
     e="WorldMap bounds receiver not in SAME source display tree";return false;};
    SwfAsValue limit;bool found{};if(!find("WorldMapLimit",limit,found))return false;
    if(found){if(!q.owner.map.limit_e8.source_assign_v67(graph,limit,e))return false;}else q.owner.map.limit_e8.reset();
    SwfAsValue buttons;if(!find("MapButtons",buttons,found))return false;
    if(found){if(!q.owner.map.buttons_ec.source_assign_v67(graph,buttons,e))return false;}else q.owner.map.buttons_ec.reset();
    //The original unconditionally calls absolute rectangle on MapButtons.
    AuthoredMenuDeadZoneV3 reference;if(!found||!bounds(buttons,reference)){if(e.empty())e="Required original WorldMap MapButtons rectangle";return false;}
    for(unsigned i=0;i<4;++i){
     std::string name="MapLimit";name.push_back(static_cast<char>(i+1)); //435fa8 really appends char1..4
     SwfAsValue mapped;if(!find(name,mapped,found))return false;
     auto& row=q.owner.map.rows[i];
     if(!found)row.scale_f8=1.f;
     else {AuthoredMenuDeadZoneV3 rectangle;if(!bounds(mapped,rectangle))return false;
      row.scale_f8=1.f-std::fabs((rectangle.ymin-reference.ymin)/(reference.ymax-reference.ymin));}
     row.fc=-1;row.word100=-1;
    }
    //WorldMap.Init435fa8 reads process Arrays.WorldMapLockers, before any
    //Player/Save exists at cold MenuManager.Init11. Borrow that SAME decoded
    //source group rather than requiring a campaign profile to repeat it.
    std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;
    if(!original_ui.process_arrays_borrow_v101(arrays,e)||!arrays||!arrays->ready())return false;
    const auto* lockers=arrays->group("WorldMapLockers");
    if(!lockers||!lockers->records_loaded||lockers->rows.size()<3){e="Required actual Arrays.WorldMapLockers first three members";return false;}
    for(unsigned i=0;i<3;++i){const auto& row=lockers->rows[i];
     if(row.fields.size()!=2||row.fields[0].kind!=dh2::android_ui::ProcessArrayValueV101::Kind::word||row.fields[1].kind!=dh2::android_ui::ProcessArrayValueV101::Kind::word){e="Malformed actual WorldMapLocker row";return false;}
     std::int32_t state{},quest{};std::memcpy(&state,&row.fields[0].bits,4);std::memcpy(&quest,&row.fields[1].bits,4);
     q.owner.map.locker108_124[i*2]=quest;q.owner.map.locker108_124[i*2+1]=state;
    }
    return true;
   }} init{provider,owner};return movie->menu_action_script(&init,Init::run,e);
 }
 e="WorldMap.Init lost actual registered resource";return false;
}
bool source_singleton_init_v67(NativeMenuPostMovieV62& provider,NativeMenuSingletonV67& owner,std::string& e){
 auto& directory=*provider.directory;
 if(!directory.register_base(owner.fields,owner.state,owner.projection,e))return false;
 if(owner.kind==MenuSingletonKindV67::debug){
  if(owner.fields.render&&!directory.find_base(owner.fields,owner.state,"text",owner.cached[0],e))return false;
 }else if(owner.kind==MenuSingletonKindV67::select_class){
  if(owner.fields.render){
   if(!directory.find_base(owner.fields,owner.state,"class_title.text",owner.cached[2],e)||
      !directory.find_base(owner.fields,owner.state,"s_description.text",owner.cached[3],e))return false;
   std::int32_t id{};std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;
   if(!original_ui.process_arrays_borrow_v101(arrays,e)||!arrays||!arrays->get_member_id("CharacterTable","KnightPlayerBase",id,e))return false;owner.select164=id;
  }
 }else if(owner.kind==MenuSingletonKindV67::world_map&&owner.fields.render){
  if(!source_worldmap_initialize_v68(provider,owner,e))return false;
 }
 return true;
}
bool source_singleton_get_v67(NativeMenuPostMovieV62& provider,MenuSingletonKindV67 kind,NativeMenuSingletonV67*& out,std::string& e){
 const auto index=static_cast<unsigned>(kind);auto& retained=source_singleton_v67[index];
 if(retained){out=retained.get();if(!out->constructed){e=out->failure;return false;}return true;}
 retained=std::make_shared<NativeMenuSingletonV67>(kind);out=retained.get();out->constructor_attempted=true;
 bool ignored{};
 if(!provider.directory->debug_load(e)||!provider.directory->debug_switch("isTracingMenuBase",ignored,e)){out->failure=e;return false;}
 source_menu_process_v62->source_fs_commands().source_menu_base_c2_registration();
 if(!source_singleton_init_v67(provider,*out,e)){out->failure=e;return false;}
 if(kind==MenuSingletonKindV67::inventory){
  NativeMenuSingletonV67* details{};if(!source_singleton_get_v67(provider,MenuSingletonKindV67::inventory_details,details,e)||
     !source_singleton_init_v67(provider,*details,e)){out->failure=e;return false;}
 }
 out->constructed=true;e.clear();return true;
}
}
dh2::ui::AuthoredMenuFieldsV1* model_renderer::native_derived_menu_fields_v67(std::uintptr_t id)noexcept{
 for(auto& owner:source_singleton_v67)if(owner&&owner->fields.identity==id)return &owner->fields;return nullptr;
}
bool model_renderer::native_worldmap_visible_v68(std::shared_ptr<void> world,bool& out,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};NativeMenuSingletonV67* owner{};
 if(!provider||!provider->directory->resolve(native,e)||native->leases->world!=world||
    !source_singleton_get_v67(*provider,MenuSingletonKindV67::world_map,owner,e))return false;
 out=owner->fields.visible74!=0;return true;
}
bool model_renderer::native_derived_menu_update_v67(std::uintptr_t id,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeMenuSingletonV67* owner{};
 for(auto& q:source_singleton_v67)if(q&&q->fields.identity==id){owner=q.get();break;}
 if(!provider||!owner||!owner->constructed){e="Required actual derived menu Update owner";return false;}
 //Do not turn a derived virtual override into MenuBase.Update. Only the
 //selected captured literal/base implementations can use this transport.
 if(owner->kind==MenuSingletonKindV67::main||owner->kind==MenuSingletonKindV67::select_class){
  e="Required original selected derived menu Update for "+owner->fields.name;return false;
 }
 if(owner->kind==MenuSingletonKindV67::merchant)owner->merchant_c4=0;
 for(std::uint32_t slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!provider->directory->movie_slot(slot,q,e))return false;
  if(q.identity!=owner->fields.render)continue;dh2::ui::SwfMovie* movie{};std::shared_ptr<void> lease;
  if(!provider->directory->movie(slot,q,movie,lease,e))return false;
  return movie->menu_action_script(owner,[](void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& q=*static_cast<NativeMenuSingletonV67*>(raw);
   if(!q.state.weak_character_v59.update(graph,q.fields,e))return false;
   if(q.kind!=MenuSingletonKindV67::lobby)return true;
   dh2::ui::SwfAsValue value;bool live{};if(!q.state.weak_character_v59.borrow(graph,value,live,e))return false;if(!live)return true;
   gameswf::as_object* actual{};if(!graph.borrow_object(value,actual,e)||!actual||!actual->is(gameswf::character::m_class_id)){if(e.empty())e="Lobby.Update requires actual source character";return false;}
   auto* receiver=static_cast<gameswf::character*>(actual);auto* environment=receiver->is(gameswf::sprite_instance::m_class_id)?receiver:receiver->get_parent();
   if(!environment||!environment->is(gameswf::sprite_instance::m_class_id))return true; //InvokeASCallback sourcefalse branch
   dh2::ui::SwfAsValue env,result;if(!graph.retain_object(environment,env,e))return false;bool callable{};
   return graph.invoke(env,value,"updatePlayerList",{},result,callable,e);
  },e);
 }
 e="Derived menu Update lost its actual movie generation";return false;
}
bool model_renderer::source_native_loadmenu3_singletons_v67(std::shared_ptr<void> world,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 if(!provider||!provider->directory->resolve(native,e)||native->leases->world!=world)return false;
 auto singleton=[&](MenuSingletonKindV67 kind){NativeMenuSingletonV67* owner{};if(!source_singleton_get_v67(*provider,kind,owner,e))return false;
  //Source tests its weak context and reruns Init only for NULL/expired. Since
  //UnloadMenu clears render and retires the old movie, that reached branch
  //uses the actual new resource; no constructor fields are replayed.
  if(!owner->fields.render)return source_singleton_init_v67(*provider,*owner,e);
  for(std::uint32_t slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!provider->directory->movie_slot(slot,q,e))return false;
   if(q.identity!=owner->fields.render)continue;dh2::ui::SwfMovie* movie{};std::shared_ptr<void> lease;if(!provider->directory->movie(slot,q,movie,lease,e))return false;
   struct Weak {NativeMenuSingletonV67& owner;bool live{};static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Weak*>(raw);dh2::ui::SwfAsValue value;return q.owner.state.weak_character_v59.borrow(graph,value,q.live,e);}} weak{*owner};
   if(!movie->menu_action_script(&weak,Weak::run,e))return false;
   return weak.live||source_singleton_init_v67(*provider,*owner,e);
  }
  e="LoadMenu singleton weak test lost SAME registered movie";return false;
 };
 if(!singleton(MenuSingletonKindV67::debug))return false;
 //4323c4 calls DebugCachedCharacter.GetChar and hides that actual text only
 //when nonNULL; it does not hide the menu root or create a missing text clip.
 if(auto* owner=source_singleton_v67[static_cast<unsigned>(MenuSingletonKindV67::debug)].get();owner&&owner->fields.render){
  for(std::uint32_t slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!provider->directory->movie_slot(slot,q,e))return false;
   if(q.identity!=owner->fields.render)continue;dh2::ui::SwfMovie* movie{};std::shared_ptr<void> lease;if(!provider->directory->movie(slot,q,movie,lease,e))return false;
   if(!movie->menu_action_script(owner,[](void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& owner=*static_cast<NativeMenuSingletonV67*>(raw);dh2::ui::SwfAsValue value;bool live{};
    if(!owner.cached[0].source_get_char_v68(graph,value,live,e))return false;if(!live)return true;gameswf::as_object* actual{};
    if(!graph.borrow_object(value,actual,e)||!actual||!actual->is(gameswf::character::m_class_id)){if(e.empty())e="Required actual MenuDebug text cache character";return false;}
    static_cast<gameswf::character*>(actual)->set_visible(false);return true;},e))return false;
   break;
  }
 }
 dh2::ui::MenuDebugHudOwnerV62* debug{};auto& process=*source_menu_process_v62;
 dh2::ui::MenuDebugHudServicesV62 services;services.actual_manager=native->shared_stack_v27;
 services.debug_load=[directory=provider->directory](auto& e){return directory->debug_load(e);};
 services.debug_switch=[directory=provider->directory](auto key,bool& out,auto& e){return directory->debug_switch(key,out,e);};
 services.register_menu=[directory=provider->directory](auto& q,auto& e){return directory->register_debug(q,e);};
 services.refresh_cache=[directory=provider->directory](auto& q,auto& cache,auto path,auto& e){return directory->refresh_debug(q,cache,path,e);};
 services.load_flags=[directory=provider->directory](auto& flags,bool& written,auto& e){return directory->load_debug_flags(flags,written,e);};
 if(!process.debug_hud(services,debug,e))return false;
 if(!debug->fields().render&&!debug->source_initialize_v67(services,e))return false;
 for(const auto kind:{MenuSingletonKindV67::main,MenuSingletonKindV67::merchant,MenuSingletonKindV67::select_class,MenuSingletonKindV67::enter_name,MenuSingletonKindV67::lobby,MenuSingletonKindV67::inventory})if(!singleton(kind))return false;
 if(!provider->map_instance(e))return false;
 if(!source_map_menu_v62->fields().render){
  dh2::ui::MenuMapServicesV62 map;map.actual_manager=native->shared_stack_v27;map.debug_load=services.debug_load;map.debug_switch=services.debug_switch;
  map.register_menu=[directory=provider->directory](auto& fields,auto& state,auto& p,auto& e){return directory->register_base(fields,state,p,e);};
  map.find_from=[directory=provider->directory](auto& fields,auto& state,const char* path,auto& out,auto& e){return directory->find_base(fields,state,path,out,e);};
  if(!source_map_menu_v62->source_initialize_v67(map,e))return false;
 }
 return singleton(MenuSingletonKindV67::world_map);
}
bool model_renderer::create_native_process_menu_singleton_v104(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const char* name,std::uintptr_t& out,std::string& e){
 out=0;auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 if(!app||app!=application_services_v5||!name||!provider||!provider->directory->resolve(native,e)||
    !native->process_directory_ready_v104||native->process_application_v104.lock()!=app){
  if(e.empty())e="Required actual process menu singleton authority";return false;
 }
 if(!std::strcmp(name,"menu_MapSheet")){
  if(!provider->map_instance(e))return false;out=source_map_menu_v62->fields().identity;e.clear();return true;
 }
 if(!std::strcmp(name,"menu_DebugHUD")){
  dh2::ui::MenuDebugHudServicesV62 services;services.actual_manager=native->shared_stack_v27;
  services.debug_load=[directory=provider->directory](auto& error){return directory->debug_load(error);};
  services.debug_switch=[directory=provider->directory](auto key,bool& value,auto& error){return directory->debug_switch(key,value,error);};
  services.register_menu=[directory=provider->directory](auto& receiver,auto& error){return directory->register_debug(receiver,error);};
  services.refresh_cache=[directory=provider->directory](auto& receiver,auto& cache,auto path,auto& error){return directory->refresh_debug(receiver,cache,path,error);};
  services.load_flags=[directory=provider->directory](auto& flags,bool& written,auto& error){return directory->load_debug_flags(flags,written,error);};
  dh2::ui::MenuDebugHudOwnerV62* receiver{};
  if(!source_menu_process_v62->debug_hud(services,receiver,e)||!receiver)return false;
  out=receiver->fields().identity;e.clear();return true;
 }
 //API callers may name the C++ class; registered movie names are the exact
 //CString literals supplied by original42abb4/42acb8, including lowercase.
 if(!std::strcmp(name,"menu_Confirm")||!std::strcmp(name,"menu_HUDConfirm")){
  NativeMenuSingletonV67* receiver{};
  const auto kind=!std::strcmp(name,"menu_Confirm")?MenuSingletonKindV67::confirm:MenuSingletonKindV67::hud_confirm;
  if(!source_singleton_get_v67(*provider,kind,receiver,e)||!receiver)return false;
  out=receiver->fields.identity;e.clear();return true;
 }
 constexpr const char* names[]{"menu_Debug","menu_MainMenu","menu_Merchant","menu_SelectClass","menu_EnterName",
  "menu_MultiplayerLobbyMulti","menu_InventorySheetMain","menu_InventorySheetDetails","menu_FastTravel","menu_confirm","menu_hud_confirm"};
 for(unsigned i=0;i<source_singleton_v67.size();++i)if(!std::strcmp(name,names[i])){
  NativeMenuSingletonV67* actual{};
  if(!source_singleton_get_v67(*provider,static_cast<MenuSingletonKindV67>(i),actual,e)||!actual)return false;
  out=actual->fields.identity;e.clear();return true;
 }
 e=std::string("Required source constructor for actual menu singleton ")+name;return false;
}
