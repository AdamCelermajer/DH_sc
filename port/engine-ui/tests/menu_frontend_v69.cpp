#include "swf_menu_launch_v1.hpp"
#include "game_option_table_v1.hpp"
#include "../../level-world/design_settings_xp_provider_v1.hpp"
#include "owned_hud_settings_v1.hpp"
#include "swf_menu_options.hpp"
#include "menu_save_slot_projection_v1.hpp"
#include "../../game-data/campaign_profile_files_v1.hpp"
#include "../../game-data/menu_profile_metadata_v1.hpp"
#include "../../game-data/player_profile_create_v1.hpp"
#include "../../game-data/player_profile_filename_v1.hpp"
#include "../../game-data/savegame_options_v1.hpp"
#include "swf_frame_connection.hpp"
#include "../../android-native/app/src/main/cpp/menu_diagnostic_filter.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <filesystem>
#include <algorithm>
#include <chrono>
using namespace dh2::ui;
namespace gameswf {void ensure_loaders_registered();}
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static void ck(bool yes,const char* text){++checks;if(!yes)throw std::runtime_error(text);}
static Raw file(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);ck(bool(f),"cache file");return {std::istreambuf_iterator<char>(f),{}};}
static void word(Raw& r,unsigned v,unsigned width=4){for(unsigned i=0;i<width;++i)r.push_back(std::uint8_t(v>>(i*8)));}
static void text(Raw& r,const std::string& s,unsigned width=4){word(r,unsigned(s.size()),width);r.insert(r.end(),s.begin(),s.end());}
static void names(Raw& r,std::initializer_list<const char*> a){word(r,unsigned(a.size()));for(auto* s:a)text(r,s);}
struct Context {
 std::vector<std::string> trace;bool fail_create{},publish=true,fail_integer{},fail_text{},fail_start{};unsigned assignment{};std::int32_t assigned_slot=-1;
 static bool create(void* p,const std::string& name,const std::string& klass,std::int32_t& slot,bool& publish,std::string& e){auto& c=*static_cast<Context*>(p);c.trace.push_back("create:"+name+":"+klass);if(c.fail_create){e="persistence failed";return false;}slot=2;publish=c.publish;return true;}
 static bool assign(void* p,std::int32_t slot,std::int32_t ordinal,std::string&){auto& c=*static_cast<Context*>(p);c.trace.push_back("assign:"+std::to_string(slot)+":"+std::to_string(ordinal));++c.assignment;c.assigned_slot=slot;return true;}
 static bool integer(void* p,double value,std::int32_t& output,std::string& e){auto& c=*static_cast<Context*>(p);c.trace.push_back("integer");if(c.fail_integer){e="EABI failed";return false;}output=std::int32_t(value);return true;}
 static bool open(void*,const char* uri,bool& found,Raw& bytes,std::uintptr_t& lease,std::string&){bytes.clear();found=true;lease=1;word(bytes,1,2);text(bytes,std::string(uri)=="text/p8_s0"?"TEST_LABEL":"ACT ^d / ^s",2);return true;}
 static bool close(void*,std::uintptr_t,std::string&){return true;}
 static bool refresh_settings(void*,OwnedHudSettingsV1&,std::int32_t,std::string&){return true;}
 static bool debug(void* p,const char*,std::string&){static_cast<Context*>(p)->trace.push_back("debug");return true;}
 static bool hud(void* p,const HudTextRequestV1& q,HudTextResponseV1& out,std::string& e){auto& c=*static_cast<Context*>(p);c.trace.push_back("text:"+std::to_string(q.operation));if(c.fail_text){e="text provider failed";return false;}
  if(q.operation==hud_text_constant_v1){out.value=std::string(q.key)=="GLOBAL_DECIMAL_SEPERATOR"?0:std::string(q.key)=="GLOBAL_THOUSANDS_SEPERATOR"?1:2;return true;}
  if(q.operation==hud_text_integer_string_v1){out.text=q.value==0?".":q.value==1?",":"1000";return true;}
  if(q.operation==hud_text_pack_v1){out.value=0;return true;}e="unreached text operation";return false;
 }
 static bool preview(void* p,std::int32_t slot,bool force,std::string&){static_cast<Context*>(p)->trace.push_back("preview:"+std::to_string(slot)+":"+std::to_string(force));return true;}
 static bool start(void* p,bool numeric,std::int32_t difficulty,std::string& e){auto& c=*static_cast<Context*>(p);c.trace.push_back("start:"+std::to_string(numeric)+":"+std::to_string(difficulty)+":"+std::to_string(c.assigned_slot));if(c.fail_start){e="development continuation rejected";return false;}return true;}
 SwfMenuLaunchServicesV1 services(){return {this,create,assign,integer,preview,start};}
};
struct SavedSettingsFixture {
 Raw bytes;
 static bool open(void* p,const char* uri,bool& found,Raw& bytes,std::uintptr_t& lease,std::string& error){
  if(!uri||std::string(uri)!="dh2_settings.savegame"){error="unexpected saved settings URI";return false;}
  auto& self=*static_cast<SavedSettingsFixture*>(p);found=true;bytes=self.bytes;lease=1;return true;
 }
 static bool close(void*,std::uintptr_t,std::string&){return true;}
};
struct NameObject:gameswf::as_object {
 Context& context;std::string value;
 NameObject(gameswf::player* p,Context& c,const char* v):as_object(p),context(c),value(v){}
 const char* to_string()override{context.trace.push_back("string:"+value);return value.c_str();}
};
struct TestCall:gameswf::fn_call {
 using gameswf::fn_call::fn_call;
 TestCall& operator=(const TestCall& next){result=next.result;this_ptr=next.this_ptr;env=next.env;nargs=next.nargs;first_arg_bottom_index=next.first_arg_bottom_index;return *this;}
};
static TestCall call(gameswf::as_environment& env,gameswf::as_value& result,const gameswf::as_value& self,std::initializer_list<gameswf::as_value> args){env.set_stack_size(0);for(auto i=args.end();i!=args.begin();){--i;env.push(*i);}return {&result,self,&env,int(args.size()),env.get_top_index()};}
static bool store_menu_difficulty(void* raw,std::int32_t value,std::string&){
 *static_cast<std::int32_t*>(raw)=value;return true;
}
static bool menu_display_constant(void*,const char*,const char*,std::int32_t& value,std::string&){
 value=900;return true;
}
static bool menu_display_string(void*,std::uint32_t id,std::string& value,std::string&){
 value="id:"+std::to_string(id);return true;
}
static bool menu_display_date(void*,std::uint32_t,std::tm& value,std::string&){
 value={};value.tm_year=126;value.tm_mon=9;value.tm_mday=10;value.tm_hour=13;value.tm_min=45;return true;
}
static void created_campaign_start_screen_reload(){
 using namespace dh2::data;
 std::string error;CharacterTable characters;characters.names.resize(326);
 characters.names[290]="KnightPlayerBase";characters.rows.resize(291);
 characters.rows[290][5]=81;
 PlayerSavegameV1 created;
 ck(created.initialize_new_profile_metadata(2,error),"start-screen create Save init");
 created.set_player_name("ReloadHero");created.set_player_level(12);created.set_class(290);
 created.set_unlocked_difficulty(2);created.set_save_date(1700000000u);
 ck(created.set_new_profile_locations(3,error),"start-screen source initial locations");
 std::int32_t difficulty=1;PlayerProfileIndexV1 empty_index;
 const std::uint8_t empty[4]={0,0,0,0};
 ck(empty_index.load({empty,sizeof(empty)},error),"start-screen empty prior index");
 const std::vector<std::string> writers={"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP"};
 Raw bytes;
 ck(serialize_player_metadata_profile_v1(empty_index.borrow(),writers,created,
     characters,&difficulty,bytes,error),"serialize created profile with selected writer library");
 const auto nonce=std::chrono::steady_clock::now().time_since_epoch().count();
 const auto root=std::filesystem::temp_directory_path()/
     ("dh2-start-screen-reload-v1-"+std::to_string(nonce));
 ck(std::filesystem::create_directory(root),"create isolated start-screen fixture directory");
 const auto path=root/player_profile_filename_v1(2,false,false);
 {std::ofstream out(path,std::ios::binary);out.write(reinterpret_cast<const char*>(bytes.data()),std::streamsize(bytes.size()));
  ck(bool(out),"publish fixture campaign file");}
 bool occupied=false;ck(campaign_profile_exists_v1(root.string(),2,occupied,error)&&occupied,
     "new start screen detects newly created campaign");
 CampaignProfileFileV1 file;
 ck(read_campaign_profile_v1(root.string(),2,file,error)&&
     file.origin==CampaignProfileOriginV1::base&&file.bytes==bytes,
     "new start screen reopens published campaign bytes");
 std::int32_t loaded_difficulty=-1;MenuProfileMetadataServicesV1 load_services{
     &loaded_difficulty,store_menu_difficulty,nullptr};
 MenuProfileMetadataV1 metadata;
 ck(load_menu_profile_metadata_v1({file.bytes.data(),file.bytes.size()},characters,2,0,
     load_services,metadata,error),"new start screen reads saved metadata sections");
 ck(metadata.slot==2&&metadata.name=="ReloadHero"&&metadata.level==12&&
     metadata.character_row==290&&metadata.selected_difficulty==1&&
     metadata.unlocked_difficulty==2&&metadata.location.save_date==1700000000u&&
     metadata.location.levels==std::array<std::int32_t,3>{{41,41,41}}&&loaded_difficulty==1,
     "reloaded metadata matches newly created campaign");
 dh2::data::LevelTables levels;levels.levels.resize(42);levels.levels[41].level_name_id=77;
 dh2::ui::MenuSaveSlotPresentationServicesV1 display{
     nullptr,menu_display_constant,menu_display_string,menu_display_date};
 dh2::ui::SwfFrontSaveSlotDetailsV1 details;
 ck(dh2::ui::project_menu_save_slot_v1(metadata,characters,levels,-1,false,0,
     display,details,error)&&details.in_use&&details.slot_id==2&&
     details.player_name=="ReloadHero"&&details.player_level==12&&
     details.player_class=="id:81"&&details.player_location=="id:77"&&
     details.difficulty==1&&details.difficulty_unlocked==2,
     "new start screen projects created campaign details");
 std::filesystem::remove_all(root);
}
int main(int argc,char** argv){try{
 ck(argc==2,"cache arg");std::filesystem::path cache=argv[1];std::string error;
 created_campaign_start_screen_reload();
 unsigned label_index=0;
 for(auto* label:{"Stats","Equipment","Skills","Faeries","Quests"}){
  const auto base=std::string("Note To Self--> Hard-coded text --> \"")+label+"\" in textfield undefined";
  ck(dh2::android_ui::hardcoded_menu_label(base.c_str())==int(label_index++),"exact log label");
  ck(dh2::android_ui::hardcoded_menu_label((base+"\n").c_str())==int(label_index-1)&&dh2::android_ui::hardcoded_menu_label((base+"\r\n").c_str())==int(label_index-1),"single source trace line ending");
  ck(dh2::android_ui::hardcoded_menu_label((base+" ").c_str())==-1&&dh2::android_ui::hardcoded_menu_label((base+"\n\n").c_str())==-1&&dh2::android_ui::hardcoded_menu_label(("prefix"+base).c_str())==-1,"unrecognized text remains logged");
 }
 ck(dh2::android_ui::hardcoded_menu_label(nullptr)==-1,"null diagnostic remains outside filter");
 // Exercise genuine last-player retirement, then the next renderer owner.
 // HUD tag classes must remain usable after a main/shared menu lifetime.
 gameswf::gc_ptr<gameswf::player> first=new gameswf::player;
 gameswf::ensure_loaders_registered();
 auto loaders=[](){gameswf::loader_function loader=nullptr;for(int tag:{0,2,39,74})if(!gameswf::get_tag_loader(tag,&loader)||!loader)return false;return true;};
 ck(loaders(),"initial standard/end/shape/sprite/CSM loaders");
 gameswf::gc_ptr<gameswf::player> second=new gameswf::player;
 first=nullptr;ck(loaders(),"shared loaders retained with another renderer player");
 second=nullptr;ck(!gameswf::get_tag_loader(0,nullptr)&&!gameswf::get_tag_loader(39,nullptr),"actual last-player teardown clears one loader table");
 for(unsigned cycle=0;cycle<3;++cycle){
  gameswf::gc_ptr<gameswf::player> next=new gameswf::player;gameswf::ensure_loaders_registered();
  ck(loaders(),"new renderer re-registers actual cleared loader table");
  gameswf::ensure_loaders_registered();ck(loaders(),"registration is idempotent while owner remains");
  next=nullptr;ck(!gameswf::get_tag_loader(0,nullptr),"next renderer complete table retirement");
 }
 GameOptionTableV1 options;auto records=file(cache/"design_pyarray.bin"),ns=file(cache/"design_pyarraynames.bin"),schema=file(cache/"design_pystructnames.bin");
 ck(options.load_design_cache({records.data(),records.size()},{ns.data(),ns.size()},{schema.data(),schema.size()},error),error.c_str());auto borrow=options.borrow();ck(borrow.difficulty_count()==3&&borrow.rows().size()==16,"actual difficulty count/option rows");
 const auto retained_design=borrow.design_settings_table();
 ck(retained_design.data&&retained_design.size==176&&
    std::equal(retained_design.data,retained_design.data+retained_design.size,records.data()),
    "canonical GameOption snapshot retains exact DesignSettings row zero");
 dh2::design_settings_xp_provider_v1::View xp_design{retained_design.data,retained_design.size};
 for(const auto& field:std::array<std::pair<std::uint32_t,float>,7>{{
       {160,900.f},{156,5.f},{140,10.f},{144,20.f},{152,0.f},{148,200.f},{164,.025f}}}){
  float value=-1.f;
  ck(dh2::design_settings_xp_provider_v1::read(&xp_design,field.first,&value,error)==0&&value==field.second,
     "XP DesignSettings callback reads retained canonical row");
 }
 auto damaged=ns; // Names group two count: skip the first group's name(s).
 std::size_t at=4;auto u32=[&](std::size_t p){unsigned v=0;for(unsigned j=0;j<4;++j)v|=unsigned(damaged[p+j])<<(8*j);return v;};for(unsigned i=0,n=u32(0);i<n;++i){auto nbytes=u32(at);at+=4+nbytes;}damaged[at]=2;
 GameOptionTableV1 invalid;ck(!invalid.load_design_cache({records.data(),records.size()},{damaged.data(),damaged.size()},{schema.data(),schema.size()},error)&&!invalid.borrow(),"difficulty names mismatch rejected");
 ck(!options.load_design_cache({records.data(),records.size()},{ns.data(),ns.size()},{schema.data(),schema.size()},error)&&borrow.difficulty_count()==3,"borrow prevents replacement");
 ck(std::equal(retained_design.data,retained_design.data+retained_design.size,records.data()),
    "retained XP row survives rejected replacement");
 Context c;gameswf::gc_ptr<gameswf::player> player=new gameswf::player;
 auto history=std::make_shared<SwfInputHistory>();SwfFrameConnection frames;
 ck(history->bind(player.get_ptr(),error)&&frames.bind(player.get_ptr(),history,error),"actual source constructor/frame receiver binding");
 // A genuine empty GameSWF root is required before constructing its AS
 // environment. This declared host graph fixture carries no game timeline.
 gameswf::gc_ptr<gameswf::movie_def_impl> definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::create_bitmaps_flag(0),gameswf::create_font_shapes_flag(0));
 definition->set_frame_count(1);definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);
 gameswf::gc_ptr<gameswf::root> root=definition->create_root();
 gameswf::as_environment env(player.get_ptr());gameswf::as_value self,result("keep");auto services=c.services();
 auto fn=call(env,result,self,{});ck(swf_menu_create_save_slot_v1(fn,services,error)&&c.trace.empty()&&result.to_tu_string()=="keep","create argc no-op");
 gameswf::gc_ptr<NameObject> name=new NameObject(player.get_ptr(),c,"Hero"),klass=new NameObject(player.get_ptr(),c,"KnightPlayerBase");
 fn=call(env,result,self,{gameswf::as_value(name.get_ptr()),gameswf::as_value(klass.get_ptr())});ck(swf_menu_create_save_slot_v1(fn,services,error)&&result.to_number()==2,"create actual result");ck(c.trace==std::vector<std::string>{"string:Hero","string:KnightPlayerBase","create:Hero:KnightPlayerBase"},"create conversion/effect order");
 c.trace.clear();c.publish=false;result.set_string("keep");fn=call(env,result,self,{"Hero","unknown"});ck(swf_menu_create_save_slot_v1(fn,services,error)&&result.to_tu_string()=="keep"&&c.trace.size()==1,"invalid class no result publication");
 c.fail_create=true;c.trace.clear();ck(!swf_menu_create_save_slot_v1(fn,services,error)&&result.to_tu_string()=="keep"&&c.trace.size()==1,"persistence failure keeps result/prefix");c.fail_create=false;
 c.trace.clear();fn=call(env,result,self,{-1,0});ck(swf_menu_assign_save_slot_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer"}&&c.assignment==0,"first negative skips second EABI");
 c.trace.clear();fn=call(env,result,self,{2,-1,999});ck(swf_menu_assign_save_slot_v1(fn,services,error)&&c.trace.size()==2&&c.assignment==0,"second negative/no argc gate");
 c.trace.clear();fn=call(env,result,self,{2,0});ck(swf_menu_assign_save_slot_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer","integer","assign:2:0"}&&result.to_tu_string()=="keep","assignment ordinal and untouched result");
 c.trace.clear();c.fail_integer=true;ck(!swf_menu_assign_save_slot_v1(fn,services,error)&&c.trace.size()==1&&c.assignment==1,"integer failure prefix");c.fail_integer=false;
 c.trace.clear();fn=call(env,result,self,{-1});ck(swf_menu_preview_save_slot_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer","preview:-1:0"}&&result.to_tu_string()=="keep","preview no negative gate and default force");
 c.trace.clear();fn=call(env,result,self,{2,"force",999});ck(swf_menu_preview_save_slot_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer","preview:2:1"},"preview actual optional bool and ignored extras");
 c.trace.clear();auto missing=services;missing.change_preview_slot=nullptr;ck(!swf_menu_preview_save_slot_v1(fn,missing,error)&&c.trace==std::vector<std::string>{"integer"},"missing preview retains integer prefix");
 c.trace.clear();fn=call(env,result,self,{});ck(!swf_menu_preview_save_slot_v1(fn,services,error)&&c.trace.empty(),"preview explicit unsafe argc guard");
 // This AS adapter preserves the authored arguments and resumes against the
 // same assigned PlayerInfo after the current SWF dispatch has unwound.
 c.trace.clear();fn=call(env,result,self,{0});ck(swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer","start:1:0:2"}&&result.to_tu_string()=="keep","NativeStartGame preserves assigned owner and AS result");
 c.trace.clear();fn=call(env,result,self,{"0"});ck(swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"start:0:0:2"},"Start numeric string does not satisfy actual is_number");
 c.trace.clear();fn=call(env,result,self,{std::numeric_limits<double>::quiet_NaN()});ck(swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"start:0:0:2"},"Start NaN source classification avoids EABI");
 c.trace.clear();fn=call(env,result,self,{2,999});ck(swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"start:0:0:2"},"Start extra args skip numeric conversion");
 c.trace.clear();fn=call(env,result,self,{-1});ck(swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer","start:1:-1:2"},"Start signed difficulty is preserved for source policy");
 c.trace.clear();c.fail_start=true;ck(!swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer","start:1:-1:2"}&&result.to_tu_string()=="keep","rejected continuation preserves reached conversion and AS result");c.fail_start=false;
 c.trace.clear();c.fail_integer=true;ck(!swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer"},"Start integer failure never invokes continuation");c.fail_integer=false;
 c.trace.clear();fn=call(env,result,self,{std::numeric_limits<double>::infinity()});ck(!swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace.empty(),"explicit nonfinite source guard");
 c.trace.clear();missing=services;missing.request_start_game=nullptr;fn=call(env,result,self,{0});ck(!swf_menu_start_game_continuation_v1(fn,missing,error)&&c.trace==std::vector<std::string>{"integer"},"missing Start continuation preserves reached conversion");
 c.trace.clear();fn=call(env,result,self,{3,0});ck(swf_menu_assign_save_slot_v1(fn,services,error),"authored order assignment fixture");fn=call(env,result,self,{0});ck(swf_menu_start_game_continuation_v1(fn,services,error)&&c.trace==std::vector<std::string>{"integer","integer","assign:3:0","integer","start:1:0:3"},"Assign before Start reads actual assigned fixture owner");
 Localization strings;Raw lr,ln,ls;word(lr,9);names(ln,{"ENGLISH","FRENCH","GERMAN","ITALIAN","JAPANESE","KOREAN","SC","SPANISH","SYMBOLS"});names(ls,{"filename","name"});names(ls,{"list"});
 for(unsigned p=0;p<9;++p){word(lr,37);for(unsigned s=0;s<37;++s){text(lr,"p"+std::to_string(p)+"_s"+std::to_string(s));text(lr,s==0?"TEST":"UNUSED");}}
 ck(strings.load({lr.data(),lr.size()},{ln.data(),ln.size()},{ls.data(),ls.size()},error)&&strings.switch_pack(0,false,error),"localization cache setup");LocalizationServices localized{&c,Context::open,Context::close,Context::debug};HudTextServicesV1 hud{&c,Context::hud};
 // The active gameplay Application view borrows the sole loaded UI settings
 // owner. The override is a real source-format dh2_settings.savegame record;
 // table defaults remain sourced from the bundled design cache.
 auto auto_transmute=std::find(borrow.names().begin(),borrow.names().end(),"AutoTransmute");
 ck(auto_transmute!=borrow.names().end(),"actual GameOptionTable contains AutoTransmute");
 const auto auto_index=std::size_t(auto_transmute-borrow.names().begin());const auto& auto_row=borrow.rows()[auto_index];
 ck(auto_row.default_value==0&&auto_row.maximum==5&&auto_row.minimum==0&&auto_row.step==1&&auto_row.type==2,"exact AutoTransmute source descriptor/default");
 SavedSettingsFixture settings_file;word(settings_file.bytes,1);text(settings_file.bytes,"AutoTransmute");word(settings_file.bytes,3);settings_file.bytes.insert(settings_file.bytes.end(),14,1);
 SettingsFileServicesV1 settings_files{&settings_file,SavedSettingsFixture::open,SavedSettingsFixture::close};
 SettingsLanguageServicesV1 settings_language{&c,Context::refresh_settings,nullptr,&strings};SettingsDeviceFactsV1 settings_device{};
 OwnedHudSettingsV1 settings(borrow);SettingsLoadReceiptV1 settings_receipt;
 ck(settings.load(false,settings_files,settings_language,settings_device,settings_receipt,error)&&settings_receipt.found&&settings_receipt.recognized_records==1,"load saved AutoTransmute override through original settings owner");
 dh2::data::savegame_options_v1::Owner settings_view;dh2::data::savegame_options_v1::Application settings_app{1,&settings_view};
 ck(settings.option("AutoTransmute")==3&&dh2::data::savegame_options_v1::bind_read_only_settings(&settings),"bind one borrowed settings owner");
 std::int32_t saved_option=-1;bool toggled=true;
 ck(dh2::data::savegame_options_v1::get_saved_option(&settings_app,"AutoTransmute",&saved_option)==dh2::data::savegame_options_v1::Status::complete&&saved_option==3,"Application reads loaded saved-option override");
 ck(dh2::data::savegame_options_v1::is_saved_option_on(&settings_app,"AutoTransmute",&toggled)==dh2::data::savegame_options_v1::Status::complete&&!toggled&&settings_view.size()==16,"numeric AutoTransmute is not a boolean toggle; size follows source owner");
 ck(settings.set_option("AutoTransmute",2)&&dh2::data::savegame_options_v1::get_saved_option(&settings_app,"AutoTransmute",&saved_option)==dh2::data::savegame_options_v1::Status::complete&&saved_option==2,"borrowed Application view observes live settings updates without copying");
 ck(dh2::data::savegame_options_v1::unbind_read_only_settings(&settings),"unbind exact settings owner");
 gameswf::gc_ptr<gameswf::as_array> a=new gameswf::as_array(player.get_ptr());a->push(3);a->push("Hero");fn=call(env,result,self,{"TEST_LABEL",gameswf::as_value(a.get_ptr())});c.trace.clear();ck(swf_menu_parsed_string_v1(fn,strings,localized,hud,services,error)&&result.to_tu_string()=="ACT 3 / Hero","real AS array numeric/string composition");
 ck(std::count(c.trace.begin(),c.trace.end(),"integer")==1,"exact numeric conversion count");
 result.set_string("keep");c.fail_text=true;c.trace.clear();ck(!swf_menu_parsed_string_v1(fn,strings,localized,hud,services,error)&&result.to_tu_string()=="keep"&&std::count(c.trace.begin(),c.trace.end(),"integer")==1,"parse failure retains AS result and numeric effects");c.fail_text=false;
 a->resize(0);a->push(std::numeric_limits<double>::quiet_NaN());a->push("Hero");c.trace.clear();ck(swf_menu_parsed_string_v1(fn,strings,localized,hud,services,error)&&result.to_tu_string()=="ACT 0 / Hero"&&std::count(c.trace.begin(),c.trace.end(),"integer")==0,"source NaN variant avoids EABI");
 dh2::data::MenuProfileMetadataV1 profile;profile.slot=2;profile.character_row=0;profile.name="Hero";profile.level=7;profile.location.levels[0]=0;dh2::data::CharacterTable characters;characters.rows.resize(1);characters.rows[0][5]=11;dh2::data::LevelTables levels;levels.levels.resize(1);levels.levels[0].level_name_id=77;
 std::vector<std::uint32_t> ids;MenuSaveSlotPresentationServicesV1 ps{&ids,[](void*,const char*,const char*,std::int32_t& out,std::string&){out=22;return true;},[](void* p,std::uint32_t id,std::string& out,std::string&){static_cast<std::vector<std::uint32_t>*>(p)->push_back(id);out=std::to_string(id);return true;},[](void*,std::uint32_t,std::tm& out,std::string&){out={};out.tm_year=126;out.tm_mon=9;out.tm_mday=6;return true;}};
 SwfFrontSaveSlotDetailsV1 details;ck(project_menu_save_slot_v1(profile,characters,levels,-1,false,0,ps,details,error)&&details.player_location=="77"&&ids==std::vector<std::uint32_t>{11,22,77},"named LevelName and actual ordered row reads");
 details.player_location="kept";ids.clear();ps.string_id=nullptr;ck(!project_menu_save_slot_v1(profile,characters,levels,-1,false,0,ps,details,error)&&details.player_location=="kept"&&ids.empty(),"missing presentation provider preserves output");
 constexpr std::array<std::string_view,7> expected_gameplay_settings{{
  "NativeGetOptionParameters","NativeSetOptions","NativeSaveSettings",
  "NativeEnterOptionMenu","NativeRefreshHudManager",
  "NativeIsJapaneseVersion","NativeIsKorean"}};
 ck(gameplay_settings_actions_v1==expected_gameplay_settings,
    "gameplay registers only settings actions with attached-owner providers");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"difficulty_count\":3,\"AS_creation_assignment_parsed_cases\":11,\"AS_preview_cases\":4,\"development_start_checks\":11,\"diagnostic_filter_checks\":16,\"renderer_player_lifetime_checks\":12,\"projection_cases\":2,\"mismatches\":0}\n";
 return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
