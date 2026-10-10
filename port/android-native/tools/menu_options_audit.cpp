// Android executable exercising real AS objects/stack and original option data.
// String/Sharp observation callbacks below are explicit test fixtures, not app
// providers. A deliberately missing scene rejects settings load; the source
// retains its initialized option map on that failure. No loaded-state claim.
#include "swf_menu_options.hpp"
#include "swf_menu_navigation.hpp"
#include "settings_native_files_v1.hpp"
#include "swf_input_history.hpp"
#include "swf_frame_connection.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_object.h"
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <map>
#include <stdexcept>
using namespace dh2::ui;
void check(bool value,const char* text){if(!value)throw std::runtime_error(text);}
std::vector<std::uint8_t> bytes(const std::string& name){std::ifstream f(name,std::ios::binary);check(bool(f),"actual data unavailable");return {std::istreambuf_iterator<char>(f),{}};}
struct Object:gameswf::as_object {
    std::vector<std::string> writes;
    bool reject=false;
    explicit Object(gameswf::player* p):as_object(p){}
    bool set_member(const tu_stringi& name,const gameswf::as_value& value) override {
        writes.emplace_back(name.c_str());
        return reject?false:as_object::set_member(name,value);
    }
    double number(const char* name){gameswf::as_value v;check(get_member(name,&v),"numeric member absent");return v.to_number();}
};
struct Fixture {
    int string_calls=0,observations=0,last_id=0,last_language=0;
    static bool text(void* p,int id,std::string& out,std::string&){auto& f=*static_cast<Fixture*>(p);++f.string_calls;f.last_id=id;out="TEST-STRING";return true;}
    static bool observe(void* p,int value,std::string&){auto& f=*static_cast<Fixture*>(p);++f.observations;f.last_language=value;return true;}
};
struct SettingsActionFixture {
    OwnedHudSettingsV1* settings{};bool gameplay{};int applied{},loaded{},saved{},entered{},refreshed{};
    int input_calls{},input_slot=-1;bool input_rollover{};
    SettingsFileServicesV1 files{};SettingsLanguageServicesV1 language{};std::string save_path;
    std::vector<std::uint8_t> persisted;
    static bool string(void*,std::int32_t id,std::string& text,std::string&){text="ACTION-"+std::to_string(id);return true;}
    static bool apply(void* raw,const char* name,std::int32_t value,std::string&){auto& f=*static_cast<SettingsActionFixture*>(raw);++f.applied;f.settings->set_option(name,value);(void)f.settings->saved_option("AutoOrientation");return true;}
    static bool load(void* raw,std::string& error){auto& f=*static_cast<SettingsActionFixture*>(raw);++f.loaded;SettingsLoadReceiptV1 receipt;
        if(!f.files.open_read||!f.files.close_read||!f.language.refresh_scene||!f.language.switch_text_pack_v4){error="Settings load fixture providers missing";return false;}
        if(!f.settings->load(false,f.files,f.language,{},receipt,error)){if(error.empty())error="Owned settings load returned false without a diagnostic";return false;}return true;}
    // Isolated host persistence adapter; settings_save_gate_route_audit.py
    // separately checks production's Application/FileManager writer route.
    static bool save(void* raw,std::string& error){auto& f=*static_cast<SettingsActionFixture*>(raw);++f.saved;
        if(!f.settings->source_save_settings_gate_v102()){error="Settings source save gate rejected test route";return false;}
        f.persisted=f.settings->serialized();std::ofstream out(f.save_path,std::ios::binary|std::ios::trunc);
        if(!out){error="Could not open private settings round-trip file";return false;}
        out.write(reinterpret_cast<const char*>(f.persisted.data()),static_cast<std::streamsize>(f.persisted.size()));out.close();
        if(!out){error="Could not write private settings round-trip file";return false;}
        return f.settings->set_language(f.settings->language(),f.language,error);}
    static bool enter(void* raw,std::string&){++static_cast<SettingsActionFixture*>(raw)->entered;return true;}
    static bool refresh(void* raw,std::string&){++static_cast<SettingsActionFixture*>(raw)->refreshed;return true;}
    static bool input_behavior(void* raw,std::int32_t slot,bool rollover,std::string&){auto& f=*static_cast<SettingsActionFixture*>(raw);++f.input_calls;f.input_slot=slot;f.input_rollover=rollover;return true;}
};
struct SettingsLanguageFixture {
    static bool refresh(void*,OwnedHudSettingsV1&,std::int32_t,std::string&){return true;}
    static bool switch_pack(void*,std::int32_t,bool,std::string&){return true;}
};
struct ParsedOptionFixture {
    std::map<std::string,std::int32_t> values;
    static std::int32_t* lookup(void* raw,const char* name){auto& v=static_cast<ParsedOptionFixture*>(raw)->values;auto i=v.find(name?name:"");return i==v.end()?nullptr:&i->second;}
};
unsigned property_reads=0;
struct NavigationFixture {
    std::vector<std::string> calls;
    bool reject=false;
    static bool push(void* p,const char* name,std::string& error){auto& f=*static_cast<NavigationFixture*>(p);f.calls.emplace_back(std::string("push:")+name);if(f.reject){error="TEST-stack-rejected";return false;}return true;}
    static bool pop(void* p,const char* name,std::string&){static_cast<NavigationFixture*>(p)->calls.emplace_back(std::string("pop:")+name);return true;}
    static bool top(void* p,std::string&){static_cast<NavigationFixture*>(p)->calls.emplace_back("top");return true;}
};
void property_getter(const gameswf::fn_call& fn){++property_reads;fn.result->set_double(123);}
int main(int argc,char** argv){try{
    check(argc==3,"usage design-data-directory settings-private-test-directory");const std::string dir=argv[1],settings_dir=argv[2];
    std::filesystem::create_directories(settings_dir);const auto settings_path=(std::filesystem::path(settings_dir)/"dh2_settings.savegame").string();
    check(!std::filesystem::exists(settings_path),"refusing to overwrite existing private settings test file");
    auto r=bytes(dir+"/design_pyarray.bin"),n=bytes(dir+"/design_pyarraynames.bin"),s=bytes(dir+"/design_pystructnames.bin");
    GameOptionTableV1 table;std::string error;
    check(table.load_design_cache({r.data(),r.size()},{n.data(),n.size()},{s.data(),s.size()},error),"actual design rejected");
    OwnedHudSettingsV1 settings(table.borrow());
    SettingsNativeFilesV1 files(settings_dir);SettingsLoadReceiptV1 receipt;
    check(!settings.load(false,files.services(),{}, {},receipt,error),"missing scene wrongly accepted");
    check(!settings.loaded()&&settings.option_count()==16,"partial source map unavailable");
    check(settings.option("VolumeMusic")==100&&settings.option("VolumeFX")==100,"original volume defaults changed");
    check(settings.option("missing")==-1&&settings.option_max("missing")==-1&&settings.option_string("missing")==-1,"source misses changed");
    auto* language=settings.descriptor("Language");check(language&&language->type==2,"original language descriptor changed");
    check(settings.option_max("Language")==language->maximum-1,"source selector maximum changed");
    check(settings.set_option("Language",6),"actual language option absent");
    gameswf::gc_ptr<gameswf::player> player=new gameswf::player;
    auto history=std::make_shared<SwfInputHistory>();SwfFrameConnection frames;
    check(history->bind(player.get_ptr(),error)&&frames.bind(player.get_ptr(),history,error),"real graph construction owners unavailable");
    // Explicit empty AVM1 test graph. Environment requires a real root; it
    // cannot be constructed from a player that has no movie definition.
    gameswf::gc_ptr<gameswf::movie_def_impl> definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);
    definition->set_frame_count(1);definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);
    gameswf::gc_ptr<gameswf::root> root=definition->create_root();
    gameswf::gc_ptr<Object> object=new Object(player.get_ptr());
    gameswf::as_environment env(player.get_ptr());Fixture fixture;
    SwfMenuOptionServicesV1 services{&settings,&fixture,false,Fixture::text,Fixture::observe};
    gameswf::as_value result(55);
    auto invoke=[&](const gameswf::as_value& key,const gameswf::as_value& receiver){
        env.push(receiver);env.push(key);
        const gameswf::as_value call_this;
        gameswf::fn_call fn(&result,call_this,&env,2,env.get_top_index());
        bool ok=swf_menu_option_parameters(fn,services,error);env.drop(2);return ok;
    };
    check(invoke(gameswf::as_value("Language"),gameswf::as_value(object.get_ptr())),"language dispatch failed");
    check(result.to_object()==object.get_ptr(),"AS result object identity changed");
    check(object->writes==std::vector<std::string>({"NumOptions","CurrentOption","OptionString"}),"source setter order changed");
    check(object->number("CurrentOption")==6&&fixture.last_id==language->value_string+6,"source current/string ID changed");
    gameswf::as_value text;check(object->get_member("OptionString",&text)&&std::string(text.to_string())=="TEST-STRING","actual AS string missing");
    const int before=fixture.string_calls;result.set_double(55);
    check(invoke(gameswf::as_value("Language"),gameswf::as_value()),"null receiver rejected");
    check(result.to_number()==55&&fixture.string_calls==before+1,"null receiver source reads/result changed");
    gameswf::as_value property(gameswf::as_value(property_getter),gameswf::as_value{});
    // A bound PROPERTY is evaluated by this core's copy constructor, so
    // bind it after pushing to test the actual native argument tag gate.
    env.push(property);env.push("Language");
    env.bottom(env.get_top_index()-1).set_property_target(object.get_ptr());
    const gameswf::as_value property_this;
    gameswf::fn_call property_call(&result,property_this,&env,2,env.get_top_index());
    check(swf_menu_option_parameters(property_call,services,error)&&property_reads==0&&result.to_number()==55,"non-object property getter wrongly evaluated");env.drop(2);
    services.sharp_device=true;
    check(invoke(gameswf::as_value("Language"),gameswf::as_value(object.get_ptr())),"Sharp branch failed");
    check(object->number("NumOptions")==5&&fixture.last_language==6&&fixture.observations==1,"Sharp source override/observation changed");
    services.sharp_device=false;object->reject=true;object->writes.clear();
    check(invoke(gameswf::as_value("Language"),gameswf::as_value(object.get_ptr())),"ignored source setter return changed");
    check(object->writes.size()==3&&result.to_object()==object.get_ptr(),"rejected setters altered result/order");
    object->reject=false;services.string_by_id=nullptr;
    check(!invoke(gameswf::as_value("Language"),gameswf::as_value(object.get_ptr()))&&error.find("StringManager")!=std::string::npos,"missing reached backend accepted");
    check(invoke(gameswf::as_value("VolumeMusic"),gameswf::as_value(object.get_ptr())),"source no-string volume branch failed");
    check(object->number("CurrentOption")==100,"real volume value changed");
    services.string_by_id=Fixture::text;
    check(invoke(gameswf::as_value(77),gameswf::as_value(object.get_ptr()))&&fixture.last_id==-1,"numeric option conversion/miss changed");
    const gameswf::as_value call_this;
    env.push(123);env.push(object.get_ptr());env.push("Language");
    gameswf::fn_call extra(&result,call_this,&env,3,env.get_top_index());
    check(swf_menu_option_parameters(extra,services,error)&&object->number("CurrentOption")==6,"source extra argument behavior changed");env.drop(3);
    env.push("Language");gameswf::fn_call short_call(&result,call_this,&env,1,env.get_top_index());
    check(!swf_menu_option_parameters(short_call,services,error)&&error.find("Malformed")!=std::string::npos,"short host call accepted");env.drop(1);
    NavigationFixture navigation;
    SwfMenuNavigationServicesV1 nav{&navigation,NavigationFixture::push,NavigationFixture::pop,NavigationFixture::top};
    result.set_double(55);
    env.push(999);env.push("menu_Options");
    gameswf::fn_call push_call(&result,call_this,&env,2,env.get_top_index());
    check(swf_menu_push(push_call,nav,error)&&result.to_number()==55&&navigation.calls.back()=="push:menu_Options","push arg/result or extra-argument semantics changed");env.drop(2);
    env.push(77);gameswf::fn_call pop_call(&result,call_this,&env,1,env.get_top_index());
    check(swf_menu_pop(pop_call,nav,error)&&result.to_number()==55&&navigation.calls.back()=="pop:77","numeric named-pop conversion changed");env.drop(1);
    // Object names use actual to_xstring pointer conversion; source never
    // calls the object's stringification method in this branch.
    const std::string object_name=gameswf::as_value(object.get_ptr()).to_xstring();
    env.push(object.get_ptr());gameswf::fn_call object_push(&result,call_this,&env,1,env.get_top_index());
    check(swf_menu_push(object_push,nav,error)&&navigation.calls.back()=="push:"+object_name,"object menu name conversion changed");env.drop(1);
    gameswf::fn_call top_pop(&result,call_this,nullptr,0,0);
    check(swf_menu_pop(top_pop,nav,error)&&navigation.calls.back()=="top"&&result.to_number()==55,"zero-arg pop incorrectly reads environment or result");
    check(!swf_menu_push(top_pop,nav,error)&&error.find("Malformed")!=std::string::npos,"zero-arg push fabricated a menu name");
    nav.pop_top=nullptr;check(!swf_menu_pop(top_pop,nav,error)&&error.find("top-pop")!=std::string::npos,"missing top-pop provider accepted");
    env.push("menu_info");gameswf::fn_call rejected(&result,call_this,&env,1,env.get_top_index());
    navigation.reject=true;check(!swf_menu_push(rejected,nav,error)&&error=="TEST-stack-rejected"&&navigation.calls.back()=="push:menu_info"&&result.to_number()==55,"synchronous stack rejection lost");env.drop(1);
    // Exercise the same owned process settings map from both menu contexts.
    // The context marker stands for whether a gameplay player is attached;
    // neither callback consults or creates player-local settings.
    SettingsActionFixture actions{&settings,false};actions.files=files.services();actions.language={nullptr,SettingsLanguageFixture::refresh,nullptr,nullptr,nullptr,SettingsLanguageFixture::switch_pack};actions.save_path=settings_path;
    SwfMenuOptionServicesV1 action_services;action_services.settings=&settings;action_services.context=&actions;
    action_services.apply_option=SettingsActionFixture::apply;action_services.load=SettingsActionFixture::load;
    action_services.save=SettingsActionFixture::save;action_services.enter=SettingsActionFixture::enter;
    action_services.refresh_hud=SettingsActionFixture::refresh;action_services.string_by_id=SettingsActionFixture::string;
    action_services.input_behavior=SettingsActionFixture::input_behavior;
    action_services.japanese_build=true;
    auto call_action=[&](const char* action,int nargs){gameswf::fn_call fn(&result,call_this,&env,nargs,env.get_top_index());return swf_menu_settings_action(action,fn,action_services,error);};
    env.push(false);env.push(0);
    check(call_action("NativeChangeRolloverInputBehavior",2)&&actions.input_calls==1&&actions.input_slot==0&&!actions.input_rollover,
          "authored Options onShow rollover route lost slot or disabled state");env.drop(2);
    env.push(true);env.push(0);
    check(call_action("NativeChangeRolloverInputBehavior",2)&&actions.input_calls==2&&actions.input_slot==0&&actions.input_rollover,
          "authored Options back rollover route lost slot or enabled state");env.drop(2);
    check(call_action("NativeIsJapaneseVersion",0)&&result.to_bool(),"authored Options Japanese edition check lost its owner value");
    check(settings.set_language(5,actions.language,error)&&call_action("NativeIsKorean",0)&&result.to_bool(),
          "authored Options Korean language check lost the settings owner value");
    check(settings.set_language(0,actions.language,error),"restore test language before settings lifecycle");
    check(call_action("NativeEnterOptionMenu",0),"settings menu enter route failed");
    bool action_ok=call_action("NativeLoadSettings",0);check(action_ok,(std::string("settings menu load route failed: ")+error).c_str());
    action_ok=call_action("NativeRefreshHudManager",0);check(action_ok,(std::string("settings menu refresh route failed: ")+error).c_str());
    check(settings.option("VolumeMusic")==settings.descriptor("VolumeMusic")->default_value&&
          settings.option("VolumeFX")==settings.descriptor("VolumeFX")->default_value&&
          settings.option("Language")==settings.descriptor("Language")->default_value,
          "settings entry did not restore actual missing-file defaults");
    env.push(61);env.push(object.get_ptr());env.push("VolumeMusic");
    check(call_action("NativeSetOptions",3)&&settings.option("VolumeMusic")==61&&object->number("CurrentOption")==61,"pre-game option write did not use the shared settings map");env.drop(3);
    actions.gameplay=true;env.push(37);env.push(object.get_ptr());env.push("VolumeFX");
    check(call_action("NativeSetOptions",3)&&settings.option("VolumeFX")==37&&object->number("CurrentOption")==37,"gameplay option write did not use the shared settings map");env.drop(3);
    // These are the five selector controls wired by the authored Options
    // movie in addition to the two volume sliders. Exercise each exact SWF
    // key through NativeSetOptions and verify the receiver is refreshed.
    const std::pair<const char*,std::int32_t> controls[]={
        {"DPad",1},{"HUDStyle",1},{"AutoTransmute",1},{"Language",2},{"AutoOrientation",1}
    };
    for(const auto& control:controls){
        env.push(control.second);env.push(object.get_ptr());env.push(control.first);
        check(call_action("NativeSetOptions",3)&&settings.option(control.first)==control.second&&
              object->number("CurrentOption")==control.second,
              (std::string("authored selector did not update shared settings: ")+control.first).c_str());
        env.drop(3);
    }
    action_ok=call_action("NativeSaveSettings",0);check(action_ok,(std::string("NativeSaveSettings file route failed: ")+error).c_str());
    check(actions.persisted==settings.serialized(),"settings save route did not serialize the shared settings owner");
    const auto option_table=table.borrow();
    ParsedOptionFixture replay;for(std::size_t i=0;i<option_table.rows().size();++i)replay.values[option_table.names()[i]]=option_table.rows()[i].default_value;
    SettingsParserSpan24V1 persisted{actions.persisted.data(),static_cast<std::uint32_t>(actions.persisted.size()),0,0,0};
    SettingsLookup16V1 lookup{&replay,ParsedOptionFixture::lookup};
    check(dh2_settings_v1_read_options(&persisted,&lookup)==0&&actions.persisted.size()-persisted.cursor==14,"serialized process settings parser round trip failed");
    bool all_saved=replay.values["VolumeMusic"]==61&&replay.values["VolumeFX"]==37;
    for(const auto& control:controls)all_saved=all_saved&&replay.values[control.first]==control.second;
    check(all_saved,"serialized process settings failed round trip for an authored control");
    check(call_action("NativeEnterOptionMenu",0),"settings menu re-entry enter route failed");
    action_ok=call_action("NativeLoadSettings",0);check(action_ok,(std::string("settings menu re-entry load route failed: ")+error).c_str());
    action_ok=call_action("NativeRefreshHudManager",0);check(action_ok,(std::string("settings menu re-entry refresh route failed: ")+error).c_str());
    bool all_reloaded=settings.option("VolumeMusic")==61&&settings.option("VolumeFX")==37;
    for(const auto& control:controls)all_reloaded=all_reloaded&&settings.option(control.first)==control.second;
    check(all_reloaded&&actions.loaded==2&&actions.entered==2&&actions.refreshed==2&&actions.saved==1,
          "actual settings file reload did not restore all seven authored values on menu re-entry");
    std::filesystem::remove(settings_path);
    std::printf("PASS | 16 real option records | Language maximum %d | typed AS identity, setter order, null receiver, Sharp, numeric name, missing provider and no-string branch | all 7 authored controls mutate the shared settings owner, save to a real private file, and reload on menu re-entry | navigation wrapper conversions, top-pop, result preservation and synchronous rejection (fixture stack only)\n",settings.option_max("Language"));
    return 0;
}catch(const std::exception& e){std::fprintf(stderr,"FAIL | %s\n",e.what());return 1;}}
