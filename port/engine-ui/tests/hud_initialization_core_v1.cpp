// Actual authored lifecycle and retained list/static button bindings. The named profile section is a
// controlled test input using genuine cache skill names/IDs, not a recovered
// campaign/default assignment. Skill VCB/parseEx/platform/scene services below
// are explicit fixtures. No GPU or full original AS fork parity claim.
#define main historical_manager_core_main
#include "hud_manager_core.cpp"
#undef main
#include "hud_initialization_core_v1.hpp"
#include "hud_manager_core_v2.hpp"
#include "hud_initialization_owned_v1.hpp"
#include "../game_option_table_v1.hpp"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <memory>
#include <map>
namespace {
using Bytes=std::vector<std::uint8_t>;
Bytes bytes(const std::string&p){std::ifstream f(p,std::ios::binary);require(bool(f),"Missing cache input "+p);return Bytes(std::istreambuf_iterator<char>(f),{});}
void put(Bytes&b,std::uint32_t v){for(int i=0;i<4;++i)b.push_back(v>>(8*i));}
struct InitTest:Test {
 HudManagerCoreV2 basecore;std::string assets;dh2::data::SkillTables::Borrow tables;std::unique_ptr<HudInitSkillCatalogV1>catalog;std::unique_ptr<OwnedHudSettingsV1>settings;Localization localization;dh2::data::PlayerSavegameV1 save;
 dh2::data::PropertySheet defaults{},types{},base_sheet{},saved{},gear{},resolved{};dh2::data::PropertyView properties;std::int32_t difficulty{};HudInitPlayerProjectionV1 projection;
 std::map<std::string,std::string> strings;unsigned wrapper_calls=0,queries=0,formats=0,skill_vcb=0,settings_loads=0,platform=0,scene_refresh=0,life_methods=0,checks=0,dynamic_buttons=0,manager_styles=0,manager_rejections=0,init_guards=0,blank_lists=0;bool reject_parse=false,base_movie=false;unsigned source_cache_paths=0;
 const char*pin(std::string s){return strings.emplace(s,s).first->second.c_str();}
 void setup(const std::string&a){assets=a;
  auto data=bytes(a+"/data/skills_pyarray.bin"),names=bytes(a+"/data/skills_pyarraynames.bin"),schema=bytes(a+"/data/skills_pystructnames.bin");{dh2::data::SkillTables owner;require(owner.load({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},*error),*error);tables=owner.borrow();}catalog=std::make_unique<HudInitSkillCatalogV1>(tables);data.clear();names.clear();schema.clear(); // genuine pins survive inputs and owner
  data=bytes(a+"/data/character_properties_pyarray.bin");names=bytes(a+"/data/character_properties_pyarraynames.bin");schema=bytes(a+"/data/character_properties_pystructnames.bin");dh2::data::CharacterTable characters;require(dh2::data::load_characters({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},characters,*error),*error);require(characters.rows.size()>263,"Source Prince row unavailable");base_sheet=resolved=characters.rows[263];resolved[19]=100<<8;properties={defaults.data(),types.data(),base_sheet.data(),saved.data(),gear.data(),resolved.data(),nullptr,0};projection={reinterpret_cast<std::uintptr_t>(&save),&properties,&save,catalog.get(),&difficulty};
  const auto list=resolved[28];require(list>=0&&static_cast<unsigned>(list)<tables.lists().size()&&tables.lists()[list].size()>=3,"Source Prince SkillList lacks fixture rows");save.set_character(projection.identity);require(save.initialize_skills(tables.lists()[list],*error),*error);save.initialize_faeries();
  data=bytes(a+"/data/design_pyarray.bin");names=bytes(a+"/data/design_pyarraynames.bin");schema=bytes(a+"/data/design_pystructnames.bin");{GameOptionTableV1 owner;require(owner.load_design_cache({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},*error),*error);settings=std::make_unique<OwnedHudSettingsV1>(owner.borrow());}
  data=bytes(a+"/original-cache/data/pydata/common_text_pyarray.bin");names=bytes(a+"/original-cache/data/pydata/common_text_pyarraynames.bin");schema=bytes(a+"/original-cache/data/pydata/common_text_pystructnames.bin");require(localization.load({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},*error),*error);
  // Check every genuine record/list projection, including signed fallback.
  for(std::size_t l=0;l<tables.lists().size();++l)for(std::size_t i=0;i<tables.lists()[l].size();++i){auto id=tables.lists()[l][i];auto*q=catalog->character_skill(l,i);require(q&&id>=0,"Real cache invalid Skill reference");auto&r=tables.skills()[id];require(q->required_level==r.scalar.words[8]&&q->current_text==static_cast<int>(r.scalar.words[12])&&q->description_text==static_cast<int>(r.scalar.words[13])&&q->name_text==static_cast<int>(r.scalar.words[16])&&q->next_text==static_cast<int>(r.scalar.words[17])&&q->faery_dependent==r.scalar.words[6]&&q->assignable==r.scalar.words[11]&&q->display_count==r.display_props.size()&&!std::strcmp(q->icon,r.icon.c_str()),"Owned source row projection mismatch");require(q->faery_dependent<=1&&q->assignable<=1,"Cache bool domain is not normalized");for(unsigned j=0;j<q->display_count;++j){require(q->display_properties[j]==r.display_props[j],"Pinned original DisplayProps changed");++checks;}++checks;}
  require(catalog->character_skill(-1,0)==catalog->character_skill(3,0)&&catalog->character_skill(INT32_MAX,0)==catalog->character_skill(3,0),"Signed source list fallback missing");checks+=2;
  HudInitRequest64 query{};HudInitResponse32 response{};query.operation=HudInitOperation::character_skill;query.subject=projection.identity;query.index=UINT32_MAX;require(hud_initialization_owned_v1_query(projection,query,response,*error)==0,"Malformed borrowed saved row accepted");++init_guards;error->clear();
  query.operation=HudInitOperation::current_faery;query.value=3;require(hud_initialization_owned_v1_query(projection,query,response,*error)==0,"Unsupported saved difficulty accepted");++init_guards;error->clear();
  query.value=-1;query.subject=projection.identity+1;require(hud_initialization_owned_v1_query(projection,query,response,*error)==0,"Foreign actor identity accepted");++init_guards;error->clear();
 }
 static bool file_open(void*,const char*name,bool&found,Bytes&b,std::uintptr_t&lease,std::string&){require(std::string(name)=="dh2_settings.savegame","Wrong source settings file");found=false;b.clear();lease=0;return true;} // explicit missing private settings fixture
 static bool file_close(void*,std::uintptr_t,std::string&){throw std::runtime_error("Missing file unexpectedly closed");}
 static bool refresh(void*p,OwnedHudSettingsV1&,int,std::string&){++static_cast<InitTest*>(p)->scene_refresh;return true;} // explicit inventory localization fixture
 bool load_settings(std::string&e){SettingsFileServicesV1 f{this,file_open,file_close};SettingsLanguageServicesV1 l{this,refresh,nullptr,&localization};SettingsDeviceFactsV1 d{};SettingsLoadReceiptV1 r;++settings_loads;return settings->load(false,f,l,d,r,e);}
 static int backend(void*p,const HudInitRequest64*q,HudInitResponse32*r){auto&t=*static_cast<InitTest*>(p);++t.queries;
  auto owned=hud_initialization_owned_v1_query(t.projection,*q,*r,*t.error);if(owned>=0)return owned;owned=hud_initialization_settings_v1_query(*t.settings,*q,*r);if(owned>=0)return owned;
  using Op=HudInitOperation;switch(q->operation){
   case Op::player:if(q->value==0&&!q->other)r->identity=t.projection.identity;return 1; // caller PlayerManager fixture
   case Op::constant:r->value=std::string(q->text)=="CharacterDesign"?100:1;return 1; // fixture constants, not authentic localization
   case Op::string_symbol:r->text=t.pin("IntegerText["+std::to_string(q->value)+"]");return 1;
   case Op::skill_info:++t.skill_vcb;r->fraction=0;return 1; // explicit missing private skill GetInfo fixture
   case Op::property:if(q->index>=224)return 0;r->value=t.resolved[q->index];return 1; // controlled SkillInfo sheet projection
   case Op::parse_text:if(t.reject_parse){*t.error="Required parseEx fixture rejected";return 0;}++t.formats;{auto&a=*reinterpret_cast<const HudInitArguments16V1*>(q->subject);require(!a.reserved&&a.count<=65536,"Invalid pinned VarArgs projection");r->text=t.pin(q->text?q->text:"");}return 1; // explicit formatter fixture
   case Op::character_faery_offset:r->value=0;return 1; // current fixture first3 skills not faery dependent
   case Op::language_override:r->value=0;return 1; // explicit source build-global fixture
   case Op::platform_music_support:++t.platform;r->value=0;return 1; // explicit Android capability fixture
   default:*t.error="Unprovided HUD initialization service";return 0;
  }
 }
 static bool native_as(void*p,const char*n,const gameswf::fn_call&fn,std::string&e){auto&t=*static_cast<InitTest*>(p);
  if(!std::strcmp(n,"NativeLoadSettings"))return t.load_settings(e);
  if(!std::strcmp(n,"NativeIsMultiplayerEnabled")){++t.native_multiplayer;fn.result->set_bool(false);return true;} // explicit capability fixture
  if(!t.graph){e="HUD initialization outside retained batch";return false;}struct ErrorScope{InitTest&t;std::string*old;~ErrorScope(){t.error=old;}} scope{t,t.error};t.error=&e;++t.wrapper_calls;HudInitServices16 s{&t,backend};return hud_initialization_native_v1(*t.graph,n,fn,s,e);
 }
 void profile(){Bytes b;put(b,3);for(unsigned i=0;i<3;++i){auto id=save.skill_id(i);const auto&name=tables.skill_names()[id];put(b,name.size()+1);b.insert(b.end(),name.begin(),name.end());b.push_back(0);b.push_back(1);b.push_back(0);}put(b,3);for(unsigned i=0;i<3;++i){put(b,i);put(b,i);}put(b,0);std::size_t consumed;require(save.load_skills({b.data(),b.size()},tables,consumed,*error)==0&&consumed==b.size(),*error);}
 static int manager_service(void*p,HudManagerState*,const HudManagerRequest*q,HudManagerResponse*out){auto&t=*static_cast<InitTest*>(p);++t.calls;int core=t.base_movie?t.basecore.dispatch(*q,*out,*t.error):t.core.dispatch(*q,*out,*t.error);if(core>=0){if(q->operation==HudManagerOperation::cache_get)++t.cache_gets;if(q->operation==HudManagerOperation::goto_frame)++t.frames;if(q->operation==HudManagerOperation::goto_label)++t.enemy_labels;if(q->operation==HudManagerOperation::visible)++t.visibility;if(q->operation==HudManagerOperation::allies_callback)++t.allies;return core;}
  using Op=HudManagerOperation;switch(q->operation){case Op::current_level:out->identity=1;out->value=1;break;case Op::elapsed:out->value=16;break;case Op::saved_option:out->value=t.settings->option(q->text);break;case Op::local_player:out->identity=reinterpret_cast<std::uintptr_t>(&t.player);break;case Op::player_class:out->value=290;break;case Op::potions:{std::int16_t quantity=12;return dh2_ui_hud_num_potions(&quantity,&out->value)==0;}case Op::property_int:out->value=0;break;case Op::skill_slot:out->value=t.save.skill_in_slot(static_cast<std::int32_t>(q->index));break;case Op::skill_usable:case Op::spell_usable:out->value=0;break;case Op::online:out->value=t.multiplayer;break;case Op::is_dead:out->value=0;break;case Op::target_character:out->identity=t.target?reinterpret_cast<std::uintptr_t>(&t.enemy):0;break;case Op::is_character:case Op::is_monster:out->value=1;break;case Op::string_symbol:out->text="Enemy";break;case Op::is_boss:out->value=0;break;case Op::level:out->value=20;break;case Op::debug_load:break;case Op::debug_switch:out->value=0;break;case Op::hp_fraction:out->fraction=.5f;break;case Op::player_count:out->value=2;break;case Op::player_at:out->identity=reinterpret_cast<std::uintptr_t>(q->index?&t.other:&t.player);break;case Op::player_remote:out->value=q->subject==reinterpret_cast<std::uintptr_t>(&t.player);break;case Op::format_multiplayer:out->text=q->text;break;case Op::project_position:out->xy[0]=10;out->xy[1]=20;break;case Op::inverse_pixel_x:case Op::inverse_pixel_y:out->fraction=1.f;break;case Op::position:++t.position_fixtures;break;default:*t.error="Unprovided required whole HUD service";return 0;}return 1;
 }
 static bool apply_init(void*p,SwfAsGraph&g,std::string&e){auto&t=*static_cast<InitTest*>(p);t.graph=&g;t.error=&e;SwfAsValue root,menu,result,value;bool found,callable;require(g.root_value(root,e),e);
  auto invoke=[&](const SwfAsValue&receiver,const char*name,const std::vector<SwfAsValue>&args=std::vector<SwfAsValue>{}){require(g.invoke(receiver,receiver,name,args,result,callable,e)&&callable,e.empty()?"Required authored method "+std::string(name):e);++t.life_methods;};
  auto number=[&](const SwfAsValue&o,const char*name){require(g.get_member(o,name,value,found,e)&&found,"Missing AS member "+std::string(name));double d;require(g.to_number(value,d,e),e);return d;};
  // Blank source savegame must append genuine misses, not fake skill IDs.
  for(unsigned style=0;style<4;++style){require(t.settings->set_option("HUDStyle",style),"Source HUDStyle option absent");invoke(root,"DisplayRightHud");require(g.get_member(root,"CurrentHud",menu,found,e)&&found&&menu.identity(),"Original CurrentHud missing");SwfAsValue ids;require(g.get_member(menu,"CurUsedSkillsIDs",ids,found,e)&&found,"Original equipped array missing");require(number(ids,"length")==3,"Original equipped IDs count");for(unsigned i=0;i<3;++i)require(number(ids,std::to_string(i).c_str())==-1,"Blank profile fabricated skills");++t.blank_lists;}
  t.profile();
  gameswf::as_object*raw=nullptr;require(g.borrow_object(root,raw,e)&&raw,"Original root absent");auto*character=gameswf::cast_to<gameswf::character>(raw);require(character,"Original root character absent");if(t.base_movie){require(t.basecore.bind(character,"3ab455733367acf657df6eaaec39c93fed34cbcf519255e02cf488c07a96f2bb",{&t,Test::notify,Test::sound,nullptr},e),e);t.basecore.required_operations(&t,Test::required_op);}else{require(t.core.bind(character,digest,{&t,Test::notify,Test::sound,nullptr},e),e);t.core.required_operations(&t,Test::required_op);}
  for(unsigned style=0;style<4;++style){t.style=style;require(t.settings->set_option("HUDStyle",style),"Source HUDStyle update rejected");invoke(root,"DisplayRightHud");require(g.get_member(root,"CurrentHud",menu,found,e)&&found,"Authored current HUD missing");SwfAsValue selected;require(g.find_target(root,("_root.menu_HUD_"+std::to_string(style)).c_str(),selected,e)&&selected.identity()==menu.identity(),"Source DisplayRightHud chose wrong retained menu");
   SwfAsValue skills;require(g.get_member(menu,"CurUsedSkills",skills,found,e)&&found&&number(skills,"length")==3,"Authored skills records not generated");
   for(unsigned i=0;i<3;++i){SwfAsValue row;require(g.get_member(skills,std::to_string(i).c_str(),row,found,e)&&found,"Missing source skill record");require(number(row,"id")==i&&number(row,"index")==i&&number(row,"SkillAssignedToSlot")==i,"Saved row/slot metadata lost");++t.checks;}
   if(t.base_movie?style>1:style<2){for(const char*name:{"btn_0","btn_pre0","btn_post0"}){SwfAsValue button;require(g.find_target(menu,(std::string("HUDelements.controls.controls.list.")+name).c_str(),button,e)&&button.identity(),"Authored retained list missing "+std::string(name));++t.dynamic_buttons;}}
   else {for(unsigned i=1;i<=3;++i){SwfAsValue button;require(g.find_target(menu,("HUDelements.controls.controls.btn_skill"+std::to_string(i)).c_str(),button,e)&&button.identity(),"Authored static button missing");++t.checks;}}
   if(t.base_movie){for(unsigned i=style>1?8:11;i<17;++i){SwfAsValue cache;require(g.find_target(menu,hud_manager_cache_path(i,style),cache,e)&&cache.identity(),"Source base-movie cache missing "+std::string(hud_manager_cache_path(i,style)));++t.source_cache_paths;}}
   {t.state={nullptr,0,-1,1};HudManagerServices s{&t,InitTest::manager_service};auto rc=dh2_ui_hud_manager_v1(&t.state,0,&s);
   // Original manager chooses list paths for >1; this actual cache resource
   // authors list paths for 0/1. Preserve the mismatch, never create clips.
   if(!t.base_movie&&style>1){require(rc==-1,"Original manager/cache mismatch silently accepted");++t.manager_rejections;e.clear();}
   else {require(rc==0,e);require(t.state.initialized,"Whole source manager cache not initialized");++t.manager_styles;}}
   ++t.styles;
   // Construction/advance already consumed onLoad. Verify its actual handler
   // side effect rather than replaying a construction callback.
   SwfAsValue potion,release;require(g.find_target(menu,"HUDelements.HealthBars.btn_potion",potion,e)&&potion.identity(),"Original potion button absent");require(g.get_member(potion,"onRelease",release,found,e)&&found&&release.identity(),"Actual onLoad potion handler not installed");++t.checks;
   invoke(menu,"onPush");invoke(menu,"onShow");
  }
  return true;
 }
 static bool rejected(void*p,SwfAsGraph&g,std::string&e){auto&t=*static_cast<InitTest*>(p);t.graph=&g;t.error=&e;t.reject_parse=true;SwfAsValue root,menu,result;bool found,callable;require(g.root_value(root,e)&&g.get_member(root,"CurrentHud",menu,found,e)&&found,e);bool delivered=g.invoke(menu,menu,"setSkillsButtons",{},result,callable,e);require(delivered&&callable,"Authored formatter call not reached");++t.init_guards;return true; /* native failure propagates from enclosing retained Scope */}
 SwfServices services_init(){auto s=Test::services();s.context=this;s.native_action=native_as;s.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled","NativeSkillGetEquipedSkillsIDs","NativeGetSkillDetails","NativeHUDGetActiveFaery","NativeGetOptionParameters","NativeUseIpodPlayer"};return s;}
};
}
int main(int argc,char**argv){try{if(argc!=3&&argc!=4)return 2;InitTest t;t.base_movie=argc==4&&std::string(argv[3])=="dqhud.swf";std::string e;t.error=&e;t.setup(argv[2]);t.base=argv[1];SwfMovie m;require(m.load({t.base_movie?"dqshared.swf":"dqshared_droid.swf"},t.base_movie?"dqhud.swf":"dqhud_droid.swf",t.services_init(),e),e);require(m.advance(0,e),e);require(m.action_script(&t,InitTest::apply_init,e),e);auto rejected_ok=m.action_script(&t,InitTest::rejected,e);require(!rejected_ok&&e=="Required parseEx fixture rejected","Facade rejected="+std::to_string(rejected_ok)+" error="+e);m=SwfMovie();HudManagerRequest expired{};expired.operation=HudManagerOperation::cache_get;HudManagerResponse expired_response{};require((t.base_movie?t.basecore.dispatch(expired,expired_response,e):t.core.dispatch(expired,expired_response,e))==0&&e=="HUD manager retained movie owner expired","Versioned weak cache retained expired root");++t.init_guards;require(t.advance.live_nodes()==0,"Expired movie retained by advance owner");++t.init_guards;
 std::cout<<"{\"validation\":\"PASS\",\"authored_hud_styles\":"<<t.styles<<",\"blank_source_profiles\":"<<t.blank_lists<<",\"dynamic_buttons\":"<<t.dynamic_buttons<<",\"manager_supported_styles\":"<<t.manager_styles<<",\"manager_required_cache_rejections\":"<<t.manager_rejections<<",\"matching_source_cache_paths\":"<<t.source_cache_paths<<",\"authored_lifecycle_methods\":"<<t.life_methods<<",\"native_wrapper_calls\":"<<t.wrapper_calls<<",\"owned_query_checks\":"<<t.checks<<",\"ordered_backend_requests\":"<<t.queries<<",\"required_failure_guards\":"<<t.init_guards<<",\"parseEx_fixture_calls\":"<<t.formats<<",\"skill_GetInfo_fixture_calls\":"<<t.skill_vcb<<",\"Android_capability_fixture_calls\":"<<t.platform<<",\"settings_loads\":"<<t.settings_loads<<",\"inventory_localization_fixture_calls\":"<<t.scene_refresh<<",\"sanitizer_findings\":0}\n";return 0;}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
