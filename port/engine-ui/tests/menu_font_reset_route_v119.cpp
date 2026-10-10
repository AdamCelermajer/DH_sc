#include "swf_menu_options.hpp"
#include "swf_input_history.hpp"
#include "swf_frame_connection.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_object.h"
#include <fstream>
#include <stdexcept>
#include <cstdio>
using namespace dh2::ui;
namespace {
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"missing option data");return {std::istreambuf_iterator<char>(f),{}};}
struct Fixture {
 OwnedHudSettingsV1* settings{};bool populated[4]{true,false,true,false};int reset_calls[4]{};int apply_calls{};
 static bool apply(void* raw,const char* name,int value,std::string&){auto& f=*static_cast<Fixture*>(raw);++f.apply_calls;f.settings->set_option(name,value);return true;}
 static bool text(void*,int id,std::string& out,std::string&){out="OPTION-"+std::to_string(id);return true;}
 static bool reset(void* raw,std::string&){auto& f=*static_cast<Fixture*>(raw);for(int slot=0;slot<4;++slot)if(f.populated[slot])++f.reset_calls[slot];return true;}
};
}
int main(int argc,char** argv){try{
 check(argc==2,"usage design-data-directory");std::string error;
 auto raw=read(std::string(argv[1])+"/design_pyarray.bin");
 auto names=read(std::string(argv[1])+"/design_pyarraynames.bin");
 auto structs=read(std::string(argv[1])+"/design_pystructnames.bin");
 GameOptionTableV1 table;check(table.load_design_cache({raw.data(),raw.size()},{names.data(),names.size()},{structs.data(),structs.size()},error),"option table rejected");
 OwnedHudSettingsV1 settings(table.borrow());Fixture fixture;fixture.settings=&settings;
 gameswf::gc_ptr<gameswf::player> player=new gameswf::player;
 auto history=std::make_shared<SwfInputHistory>();SwfFrameConnection frames;
 check(history->bind(player.get_ptr(),error)&&frames.bind(player.get_ptr(),history,error),"actual frame owners unavailable");
 gameswf::gc_ptr<gameswf::movie_def_impl> definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);
 definition->set_frame_count(1);definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);
 gameswf::gc_ptr<gameswf::root> root=definition->create_root();
 gameswf::as_environment env(player.get_ptr());gameswf::as_value result;const gameswf::as_value call_this;
 SwfMenuOptionServicesV1 services;services.settings=&settings;services.context=&fixture;services.apply_option=Fixture::apply;
 services.string_by_id=Fixture::text;services.reset_fonts=Fixture::reset;
 auto set=[&](const char* name,int value){env.push(value);env.push(gameswf::as_value());env.push(name);
  gameswf::fn_call fn(&result,call_this,&env,3,env.get_top_index());bool ok=swf_menu_settings_action("NativeSetOptions",fn,services,error);env.drop(3);return ok;};
 check(set("VolumeMusic",41),"volume action failed");
 check(set("DPad",1),"selector action failed");
 for(int count:fixture.reset_calls)check(count==0,"non-Language option reset fonts");
 check(set("Language",2),"language action failed");
 check(fixture.reset_calls[0]==1&&fixture.reset_calls[1]==0&&fixture.reset_calls[2]==1&&fixture.reset_calls[3]==0,
       "Language reset did not visit only populated slots");
 std::puts("PASS | NativeSetOptions invokes font reset only for Language; reset visits populated renderer slots 0 and 2, skips empty slots 1 and 3");
 return 0;
}catch(const std::exception& e){std::fprintf(stderr,"FAIL | %s\n",e.what());return 1;}}
