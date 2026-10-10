// Executes the current private production singleton bodies against the actual
// shared roster, source registration/unload owners and shipped Droid SWF.
// Asset/image/localization and absent non-inventory providers are host fixtures.
#include "authored_shared_menu_roster_v27.hpp"
#include "authored_character_menu_session_v1.hpp"
#include "menu_manager_prefix_v62.hpp"
#include "menu_manager_unload_v58.hpp"
#include "source_process_arrays_v101.hpp"
#include "swf_text_font_platform_v1.hpp"
#include "swf_font_resolver.hpp"
#include "gameswf/gameswf_sprite.h"
#include <array>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
using namespace dh2::ui;
namespace {
void check(bool value,const std::string& why){if(!value)throw std::runtime_error(why);}
struct Directory {
 std::shared_ptr<MenuStackOwnerV1> stack;
 std::shared_ptr<SwfMovie> movie;
 std::uintptr_t render{};
 bool reject_details{};
 unsigned debug_queries{};
 std::vector<std::string> registrations;
 bool debug_load(std::string& e){e.clear();return true;}
 bool debug_switch(const char*,bool& out,std::string& e){++debug_queries;out=false;e.clear();return true;}
 bool register_base(AuthoredMenuFieldsV1& fields,AuthoredCharacterStateV1& state,MenuStackCharacterV1& projection,std::string& e){
  if(!movie){e.clear();return true;} // Actual startup primary1 NULL.
  if(reject_details&&fields.name=="menu_InventorySheetDetails"){e="Rejected retained details registration";return false;}
  struct Call {
   Directory& directory;AuthoredMenuFieldsV1& fields;AuthoredCharacterStateV1& state;MenuStackCharacterV1& projection;
   static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);
    SwfAsValue root,value;bool found{};AuthoredMenuSearchIndexV1 index;
    if(!graph.root_value(root,e)||!index.initialize(graph,root,e)||!index.find(c.fields.name.c_str(),value,found,e))return false;
    if(!found){e.clear();return true;}
    AuthoredCharacterRegistrationServicesV1 s;s.owner=c.directory.movie;s.render=c.directory.render;s.source_reassignment_v67=true;
    s.append_state=[&c](auto& f,auto& e){return c.directory.stack->register_menu_live_v27(f.identity,f.render,f.name,&c.projection,0,0,e);};
    s.find=[&index](const char* name,auto& out,bool& found,auto& e){return index.find(name,out,found,e);};
    s.bind_weak_context=[&c,&graph](auto&,const auto& value,auto& e){gameswf::as_object* object{};
     return graph.borrow_object(value,object,e)&&authored_menu_stack_character_v4(object,0x84,{},c.projection,e);};
    s.create=[](auto&,auto&){return true;}; // Original inherited MenuBase.Create.
    if(!authored_character_register_state_v1(graph,c.state,nullptr,s,e))return false;
    auto* actual=c.directory.stack->registered_menu_v27(c.fields.identity);
    if(!actual){e="Actual registry publication missing";return false;}actual->valid_menu=c.fields.valid7c;
    c.directory.registrations.push_back(c.fields.name);return true;
   }
  } call{*this,fields,state,projection};return movie->menu_action_script(&call,Call::run,e);
 }
 bool find_base(AuthoredMenuFieldsV1&,AuthoredCharacterStateV1&,const char*,AuthoredMenuWeakCharacterV59&,std::string& e){e="Unexpected non-inventory Init leaf";return false;}
};
struct NativeMenuPostMovieV62 {std::shared_ptr<Directory> directory;};
struct UnusedOriginalUi {
 bool process_arrays_borrow_v101(std::shared_ptr<dh2::android_ui::SourceProcessArraysV101>&,std::string& e){e="Unexpected class-select leaf";return false;}
} original_ui;
const auto source_menu_process_v62=std::make_shared<MenuManagerProcessV62>();
struct NativeMenuSingletonV67;
bool source_worldmap_initialize_v68(NativeMenuPostMovieV62&,NativeMenuSingletonV67&,std::string&);
// Runner extracts the exact struct, Init and GetInstance bodies from the
// current shipping include. There is no test copy of inventory orchestration.
#include "native_inventory_singletons_generated_v1.inc"
bool source_worldmap_initialize_v68(NativeMenuPostMovieV62&,NativeMenuSingletonV67&,std::string& e){e="Unexpected world-map leaf";return false;}
struct Fixture {
 std::string assets;
 MenuStackGlobalsV1 globals{};AuthoredMenuApplicationFieldsV3 application_fields;
 std::shared_ptr<Directory> directory=std::make_shared<Directory>();
 NativeMenuPostMovieV62 provider{directory};
 std::unique_ptr<AuthoredSharedMenuRosterV27> roster;
 std::unique_ptr<SwfTextFontPlatformV1> fonts;
 unsigned images{},native_reload{},deleted{};
 explicit Fixture(std::string path):assets(std::move(path)){
  directory->stack=std::make_shared<MenuStackOwnerV1>(256);std::string e;
  check(directory->stack->publish_source_c1_v104(globals,e),e);
  AuthoredSharedMenuServicesV27 s;s.owner=directory;s.fields=&application_fields;
  s.debug=[](const char*,std::int32_t& out,std::string& e){out=0;e.clear();return true;};
  roster=std::make_unique<AuthoredSharedMenuRosterV27>(directory->stack,std::move(s));
 }
 ~Fixture(){directory->movie.reset();fonts.reset();}
 static int resolve_font(void* raw,FontResolveRequest40* q){auto& f=*static_cast<Fixture*>(raw);
  switch(q->kind){
  case FontResolveService::debug_load:case FontResolveService::debug_get_switch:return 0;
  case FontResolveService::language:q->value=0;return 0;
  case FontResolveService::rewrite_path:{std::string path=q->text;if(path.rfind("cache/data/",0)!=0)return -1;
   path=f.assets+"/../"+path.substr(11);if(path.size()>=q->capacity)return -1;std::memcpy(q->buffer,path.c_str(),path.size()+1);return 0;}
  case FontResolveService::open_read:q->value=reinterpret_cast<std::uintptr_t>(std::fopen(q->text,"rb"));return 0;
  case FontResolveService::close_read:return std::fclose(reinterpret_cast<FILE*>(q->value));
  }return -1;
 }
 static bool read_font(void* raw,const char* name,bool bold,bool italic,std::vector<std::uint8_t>& bytes,std::string& e){
  char path[4096];FontResolveOutput32 out{path,sizeof path,0,0,0};FontResolveInput24 in{name,"cache/",std::uint32_t(bold),std::uint32_t(italic)};
  FontResolveServices16 s{raw,resolve_font};if(dh2_swf_font_resolve(&out,&in,&s)||!out.found){e="Shipped font resolver fixture failed";return false;}
  std::ifstream file(path,std::ios::binary);if(!file){e="Shipped font bytes absent";return false;}bytes.assign(std::istreambuf_iterator<char>(file),{});return true;
 }
 SwfServices services(){
  SwfServices s;s.context=this;
  s.read=[](void* raw,const char* uri,auto& out,auto& e){auto& f=*static_cast<Fixture*>(raw);std::ifstream in(f.assets+"/"+uri,std::ios::binary);
   if(!in){e="Missing authored SWF: "+std::string(uri);return false;}out.assign(std::istreambuf_iterator<char>(in),{});return true;};
  s.image=[](void* raw,int w,int h,unsigned,const std::uint8_t*,int,auto& out,auto&){auto& f=*static_cast<Fixture*>(raw);out={1000+ ++f.images,w,h};return true;};
  s.texture=[](void*,const char*,int w,int h,auto& out,auto&){out={1,w?w:1024,h?h:1024};return true;};
  s.draw=[](void*,const auto&,auto&){return true;};
  s.native_call=[](void*,const char* name,const auto& args,auto& out,auto& e){
   if(std::strcmp(name,"NativeGetStringFromSymbol")){e="Unexpected localization leaf";return false;}
   out.kind=SwfValue::text;out.string=args.empty()?"":args.front().string;return true;};
  s.native_actions={"NativeReloadSkills"};s.native_owner=directory;
  s.native_action=[](void* raw,const char* name,const gameswf::fn_call&,auto& e){
   if(std::strcmp(name,"NativeReloadSkills")){e="Unexpected player callback";return false;}++static_cast<Fixture*>(raw)->native_reload;return true;};
  return s;
 }
 void load(std::uintptr_t render){
  std::string e;directory->render=render;directory->movie=std::make_shared<SwfMovie>();
  auto io=services();TextFontBackendsV2 backends;
  backends.bitmap_face=[](const auto&,auto& face,auto&){face={};return true;};
  fonts=std::make_unique<SwfTextFontPlatformV1>(SwfFontServices{this,read_font,nullptr},io,directory,backends,1);
  fonts->policy().renderer_feature=[](const auto& command,auto& e){if(command.kind==edit_text_display_v1::Command::grid_fit)return true;e="Fixture render cache unavailable";return false;};
  check(directory->movie->load({"dqshared_droid.swf"},"dqcharmenu_droid.swf",fonts->services(),e),e);
  check(directory->stack->register_render_live_v27(render,0x84,nullptr,nullptr,e),e);directory->registrations.clear();
 }
 AuthoredMenuFieldsV1* fields(std::uintptr_t id){
  for(auto& q:source_singleton_v67)if(q&&q->fields.identity==id)return &q->fields;
  return roster->receiver_fields_v59(id);
 }
 void unload(){
  std::string e;auto stack=directory->stack;MenuManagerUnloadServicesV58 s;s.actual_manager=stack;
  s.clear_map_icons=[](auto&){return true;};
  s.movie_slot=[this](auto slot,auto& out,auto&){out={};if(slot==1)out={directory->movie,directory->render};return true;};
  s.registry_count=[stack](auto& out,auto&){out=stack->registry_count_v58();return true;};
  s.registry_at=[this,stack](auto index,auto& out,auto& e){auto* menu=stack->registry_at_v58(index);auto* f=menu?fields(menu->identity):nullptr;
   if(!f){e="Missing actual unload receiver";return false;}out={f->identity,f->render,f->owned7d};return true;};
  s.clear_render=[this,stack](auto id,auto& e){auto* f=fields(id);auto* menu=stack->registered_menu_v27(id);
   if(!f||!menu){e="Missing same unload publication";return false;}f->render=0;menu->render=nullptr;return true;};
  s.read_owned7d=[this](auto id,auto& out,auto& e){auto* f=fields(id);if(!f){e="Missing actual ownership byte";return false;}out=f->owned7d;return true;};
  s.deleting_virtual4=[this](auto id,auto& e){++deleted;return roster->delete_receiver_v59(id,e);};
  s.erase_registry=[stack](auto index,auto id,auto& e){return stack->erase_registry_v58(index,id,e);};
  s.reset_scan_for_anims=[](const auto&,auto&){return true;}; // No Flash owner in this fixture.
  s.unload_swf_file=[this,stack](auto id,auto& e){if(id!=1){e="Wrong primary slot";return false;}
   if(!stack->clear_catalog_104_v91(directory->render,e)||!stack->clear_active_states_114_v91(directory->render,e)||
      !stack->clear_render_fields_v91(directory->render,e))return false;
   if(!stack->retire_render_resource_v93(directory->render,e))return false;directory->movie.reset();fonts.reset();directory->render=0;return true;};
  check(MenuManagerUnloadV58(std::move(s)).unload(1,e),e);
  check(stack->registry_count_v58()==0,"Unloaded registry retained entries");
 }
 void verify(NativeMenuSingletonV67& main,NativeMenuSingletonV67& details){
  std::string e;NativeMenuSingletonV67* retained{};
  check(source_singleton_get_v67(provider,MenuSingletonKindV67::inventory,retained,e)&&retained==&main,"Warm GetInstance replaced the retained inventory receiver");
  check(source_singleton_init_v67(provider,*retained,e),e);
  check(directory->registrations==std::vector<std::string>{"menu_InventorySheetMain","menu_InventorySheetDetails"},"Every inventory Init must register its retained details receiver after main");
  check(main.fields.render==directory->render&&details.fields.render==directory->render,"Details singleton failed to bind the current primary1");
  check(roster->post_load(*directory->movie,directory->render,directory->movie,0x84,e),e);
  auto* actual=directory->stack->menu("menu_InventorySheetDetails");
  check(actual&&actual->identity==details.fields.identity,"PostLoad substituted a generic details receiver");
  check(!roster->receiver_fields_v59(actual->identity)&&!details.fields.owned7d,"Retained details singleton became an owned PostLoad receiver");
  struct Weak {NativeMenuSingletonV67& details;static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Weak*>(raw);SwfAsValue value;bool live{};
   if(!q.details.state.weak_character_v59.borrow(graph,value,live,e))return false;return live;}} weak{details};
  check(directory->movie->menu_action_script(&weak,Weak::run,e),"Details weak character is not from the new movie generation");
 }
};
}
int main(int argc,char** argv){try{
 check(argc==2,"Pass the shipped Droid menu directory");Fixture fixture(argv[1]);std::string e;
 NativeMenuSingletonV67* main{};check(source_singleton_get_v67(fixture.provider,MenuSingletonKindV67::inventory,main,e),e);
 auto details=source_singleton_v67[static_cast<unsigned>(MenuSingletonKindV67::inventory_details)];
 check(main&&details&&main->constructed&&details->constructed&&!main->fields.render&&!details->fields.render,"Startup must retain both C1 receivers before primary1 exists");
 const auto main_identity=main->fields.identity,details_identity=details->fields.identity;
 fixture.load(0x101);fixture.verify(*main,*details);std::weak_ptr<SwfMovie> first=fixture.directory->movie;fixture.unload();
 check(first.expired()&&!main->fields.render&&!details->fields.render,"Actual first-campaign unload did not retire the movie and detach both singletons");
 fixture.load(0x202);fixture.verify(*main,*details);
 check(main->fields.identity==main_identity&&details->fields.identity==details_identity,"Campaign reload reconstructed a process singleton");
 fixture.unload();fixture.load(0x303);fixture.directory->reject_details=true;
 check(!source_singleton_init_v67(fixture.provider,*main,e)&&e=="Rejected retained details registration","Details Init failure was swallowed");
 check(main->fields.render==0x303&&!details->fields.render&&details->fields.identity==details_identity,"Failure did not preserve the same main prefix and retained details owner");
 fixture.unload();source_singleton_v67={};
 std::cout<<"PASS: exact inventory Init/Get bodies, shipped SWF, actual PostLoad/unload roster; startup + first campaign + second campaign preserve details identity; partial failure retained\n";
 return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
