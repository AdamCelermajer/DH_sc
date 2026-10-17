#include "../player_skill_session_v1.hpp"
#include "../player_skill_update_session_v1.hpp"
#include "../../android-native/app/src/main/cpp/native_debug_files.hpp"
#include "../../adam-script-runtime/script_game_bindings.h"
extern "C" {
#include "../../pydata-names/names.h"
#include "../../pydata-names/struct-names.h"
#include "../../pydata-constants/constants.h"
}
#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
namespace s=dh2::player_skill_session_v1;
namespace p=dh2::character_player_skills_preparation_v3;
namespace d=dh2::data;
using Tables=dh2::player_skill_tables_adapter::Tables;
constexpr std::uintptr_t CHAR=0x100000001ull,AIS=0x200000001ull;
void check(bool value,const char* why){if(!value)throw std::runtime_error(why);}
std::vector<std::uint8_t> read(const std::filesystem::path& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("missing cache resource: "+path.string());return {std::istreambuf_iterator<char>(f),{}};}
d::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
std::uint32_t word(const std::vector<std::uint8_t>& b,std::size_t at){check(at<=b.size() && b.size()-at>=4,"truncated names");std::uint32_t w;std::memcpy(&w,b.data()+at,4);return w;}
std::vector<std::uint8_t> first_names(const std::vector<std::uint8_t>& b){std::size_t at=4;auto n=word(b,0);while(n--){auto size=word(b,at);at+=4;check(size<=b.size()-at,"name overflow");at+=size;}return {b.begin(),b.begin()+at};}
std::string text(const dh2_script_value& v){check(v.type==4 && v.text,"nonstring name query");return {v.text,v.text_bytes};}
void result(dh2_script_value* out,std::uint32_t cap,std::uint32_t* n,float number){check(out && cap && n,"missing result");out[0]={};out[0].type=3;out[0].number=number;*n=1;}
struct Return {std::uint32_t count=99,type=99;float number=0;bool boolean=false;std::uintptr_t identity=0;std::string text;unsigned observed=0;};
int observe(void* p,const dh2_script_first_return_v1* r,char*,std::size_t){auto& out=*static_cast<Return*>(p);out.count=r->count;out.type=r->type;out.number=r->number;out.boolean=r->boolean!=0;out.identity=r->identity;out.text=r->text?std::string(r->text,r->text_bytes):std::string{};++out.observed;return 0;}
int observe_reentry(void* raw,const dh2_script_first_return_v1*,char*,std::size_t){
    auto& session=*static_cast<s::Session*>(raw);s::LoadResult result{9,8,7,6},before=result;std::string error="sentinel";
    check(session.load_resolved("fixture/vcb",&result,error)==-1 && !std::memcmp(&result,&before,sizeof(before)) && error=="sentinel","observer reentry changed output");
    Return nested;check(session.call("fixture_kill",nullptr,0,0,observe,&nested,error)==-1 && !nested.observed && error=="sentinel","observer reentry called Lua");return 0;
}
struct Catalogue {
    std::shared_ptr<const Tables> tables;d::CharacterTable characters;d::PropertyRules rules;d::ClassTables classes;
    std::map<std::string,std::vector<std::uint8_t>> names;std::vector<std::uint8_t> faery_constants,design,ai_constants;
    explicit Catalogue(const std::filesystem::path& cache){
        auto load=[&](const char* stem,const char* suffix){return read(cache/"data/pydata"/(std::string(stem)+suffix+".bin"));};
        std::string error;d::SkillTables skills;d::FaeryTables faeries;
        auto a=load("skills","_pyarray"),b=load("skills","_pyarraynames"),c=load("skills","_pystructnames");
        check(d::load_skill_tables(bytes(a),bytes(b),bytes(c),skills,error),error.c_str());
        a=load("faeries","_pyarray");b=load("faeries","_pyarraynames");c=load("faeries","_pystructnames");
        check(d::load_faery_tables(bytes(a),bytes(b),bytes(c),faeries,error),error.c_str());
        tables=Tables::create(std::move(skills),std::move(faeries),error);check(bool(tables),error.c_str());
        a=load("character_properties","_pyarray");b=load("character_properties","_pyarraynames");c=load("character_properties","_pystructnames");
        check(d::load_characters(bytes(a),bytes(b),bytes(c),characters,error) && d::load_property_rules(characters,rules,error),error.c_str());
        a=load("character_classes","_pyarray");b=load("character_classes","_pyarraynames");c=load("character_classes","_pystructnames");
        check(d::load_classes(bytes(a),bytes(b),bytes(c),classes,error),error.c_str());names["ClassTable"]=first_names(b);
        names["AnimatedEffectTable"]=first_names(load("effects","_pyarraynames"));
        names["ProjectileTable"]=first_names(load("projectiles","_pyarraynames"));
        faery_constants=load("faeries","_pycst");
        design=load("design","_pycst");
        ai_constants=load("ai","_pycst");
    }
};
struct Fixture {
    std::filesystem::path cache;Catalogue& catalogue;
    d::PropertyState properties;d::PropertyView view;
    dh2::native::debug_files::Backend debug;
    dh2::ais_player_init_vcb::State ais{AIS,0xffffffff};
    std::unique_ptr<p::Owner> owner;std::unique_ptr<s::Session> session;
    std::map<std::string,std::shared_ptr<const std::vector<std::uint8_t>>> overlays;
    std::weak_ptr<const std::vector<std::uint8_t>> last_resource;
    std::set<std::string> loaded_paths;std::vector<std::string> native_names;
    bool resolver_reentry=false,throw_resolver=false,throw_provider=false;
    bool native_missing=false;unsigned close_queries=0;
    explicit Fixture(const std::filesystem::path& c,const std::filesystem::path& temp,Catalogue& cat,const char* name,
                     bool create_owner=true,std::shared_ptr<void> lifetime={})
        :cache(c),catalogue(cat){
        auto it=std::find(cat.characters.names.begin(),cat.characters.names.end(),name);check(it!=cat.characters.names.end(),"missing player row");
        d::reset_properties(cat.rules,properties,&cat.characters.rows.at(it-cat.characters.names.begin()));
        std::string error;check(d::recalc_properties_with_class(cat.classes,cat.rules,properties,error),error.c_str());view=d::property_view(cat.rules,properties);
        std::filesystem::create_directories(temp);auto seed=read(cache/"DebugSwitches.savegame");check(debug.initialize(std::filesystem::absolute(temp),seed.data(),seed.size(),error),error.c_str());
        s::Configuration config;config.character=CHAR;config.ais=&ais;config.tables=cat.tables;
        config.debug=&debug.globals();config.debug_services=debug.services();
        config.providers.context=this;config.providers.resolve=resolve;config.providers.native=native;
        config.providers.lifetime=std::move(lifetime);
        config.providers.faery={this,constant,nullptr};
        auto vm=s::Vm(dh2_script_vm_create_deferred(8*1024*1024));check(bool(vm),"VM creation failed");
        auto identity=vm.get();session=s::Session::adopt(std::move(vm),std::move(config),error);check(bool(session),error.c_str());
        check(session->vm()==identity && session->stage()==s::Stage::created,"adoption created/replaced VM");
        if(!create_owner)return;
        check(!session->bind_ais_functions(error) && !session->bind_character_functions(error),error.c_str());
        check(session->statistics().ais_bindings==35,"source AIS registrations changed");
        owner=p::Owner::create(cat.tables,{CHAR,AIS,&view,0},session->preparation_services(),error);check(bool(owner),error.c_str());
    }
    ~Fixture(){session.reset();owner.reset();} // VM closes before borrowed actor/Debug/instances.
    static int resolve(void* p,const std::string& path,s::Resource& out,std::string& error){
        auto& f=*static_cast<Fixture*>(p);
        if(f.throw_resolver)throw std::runtime_error("resolver throw");
        if(f.resolver_reentry){s::LoadResult nested{99,98,97,96};auto before=nested;std::string unchanged="sentinel";
            check(f.session->load_resolved(path,&nested,unchanged)==-1 && !std::memcmp(&nested,&before,sizeof(before)) && unchanged=="sentinel","resolver reentry changed output");}
        auto found=f.overlays.find(path);std::shared_ptr<const std::vector<std::uint8_t>> b;
        if(found!=f.overlays.end())b=found->second;
        else {if(!std::filesystem::exists(f.cache/path)){error="source file missing";return -1;}b=std::make_shared<const std::vector<std::uint8_t>>(read(f.cache/path));}
        f.last_resource=b;f.loaded_paths.insert(path);out={path,b};return 0;
    }
    static int constant(void* p,dh2::character_faery_selection::Character* ch,const dh2::character_faery_selection::Request* q,dh2::character_faery_selection::Response* out){
        auto& f=*static_cast<Fixture*>(p);check(ch->identity==CHAR,"wrong faery owner");dh2_pycst_view v{};dh2_pycst_result r{};
        check(!dh2_pycst_open(&v,f.catalogue.faery_constants.data(),std::uint32_t(f.catalogue.faery_constants.size())),"invalid faery constants");
        check(!dh2_pycst_get(&v,q->category,std::uint32_t(std::strlen(q->category)),q->key,std::uint32_t(std::strlen(q->key)),&r) && r.found,"missing real faery constant");out->word=r.value;return 0;
    }
    static int native(void* p,const s::NativeRequest& q,const dh2_script_value* a,std::uint32_t n,dh2_script_value* out,std::uint32_t cap,std::uint32_t* returned,char* error,std::size_t size){
        auto& f=*static_cast<Fixture*>(p);check(q.character==CHAR,"lost full-width Character");f.native_names.emplace_back(q.name);*returned=0;
        if(f.throw_provider)throw std::runtime_error("provider throw");
        if(f.native_missing)return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
        if(q.domain==s::Domain::ais){
            using F=dh2::ais_native_bindings::Function;
            check(q.userdata==AIS,"AIS userdata changed");
            if(q.ais_function==F::trace)return dh2_script_game_trace(nullptr,a,n,out,cap,returned,error,size);
            if(q.ais_function==F::get_py_cst){
                check(n==2,"wrong source constant arguments");auto category=text(a[0]),key=text(a[1]);dh2_pycst_view v{};dh2_pycst_result r{};
                const auto& input=category=="AIStates"?f.catalogue.ai_constants:f.catalogue.design;
                check(!dh2_pycst_open(&v,input.data(),std::uint32_t(input.size())),"invalid source constants");
                check(!dh2_pycst_get(&v,category.data(),std::uint32_t(category.size()),key.data(),std::uint32_t(key.size()),&r) && r.found,"missing actual design constant");
                result(out,cap,returned,float(r.value));return 0;
            }
            if(q.ais_function==F::get_py_oid || q.ais_function==F::get_py_struct){
                check(n==2,"wrong source name arguments");auto category=text(a[0]),key=text(a[1]);dh2_pynames_view v{};bool found=false;
                if(q.ais_function==F::get_py_oid){auto it=f.catalogue.names.find(category);if(it!=f.catalogue.names.end()){check(!dh2_pynames_open(&v,it->second.data(),std::uint32_t(it->second.size())),"OID input malformed");found=true;}}
                else for(const auto& row:dh2_struct_name_tables)if(category==row.name){check(!dh2_pynames_open(&v,row.bytes,row.size),"builtin structure malformed");found=true;break;}
                check(found,"unsupported real name namespace");std::int32_t id=-1;check(!dh2_pynames_get(&v,key.data(),std::uint32_t(key.size()),&id),"name lookup failed");result(out,cap,returned,float(id));return 0;
            }
        }else {
            using F=dh2::character_native_bindings::Function;
            if(q.character_function==F::character_get_prop){
                check(q.userdata==CHAR && n>=1 && a[0].type==3 && std::isfinite(a[0].number) && a[0].number>=0 && a[0].number<224,"property input unsupported");
                std::int32_t value=0;check(!dh2_property_resolve(&f.view,std::int32_t(a[0].number),&value),"property resolve failed");result(out,cap,returned,float(value));return 0;
            }
        }
        if(error && size)std::snprintf(error,size,"Unbound actual source provider: %s",q.name);
        return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
    }
    int load(const std::string& path,s::LoadResult& out){std::string error;return session->load_resolved(path,&out,error);}
    void common(){s::LoadResult r{};check(!load("data/scripts/ai/_commons.luac",r) && r.source_success,"actual AI common failed");}
    void overlay(const char* path,const std::string& source){overlays[path]=std::make_shared<const std::vector<std::uint8_t>>(source.begin(),source.end());}
    int call(const char* name,const std::vector<dh2_script_value>& a,Return& out,std::uint32_t index=0){std::string error;return session->call(name,a.data(),std::uint32_t(a.size()),index,observe,&out,error);}
    dh2_script_value string(const std::string& text){dh2_script_value v{};v.type=4;v.text=text.c_str();v.text_bytes=text.size();return v;}
};
int main(int argc,char** argv){try{
    check(argc==3,"cache and temporary directory required");std::filesystem::path cache=argv[1],temp=argv[2];Catalogue cat(cache);
    unsigned class_cases=0,guards=0,errors=0,callbacks=0;std::set<std::string> paths;
    for(const char* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
        Fixture f(cache,temp/name,cat,name);f.common();p::source::Result prepared{};
        auto status=f.owner->prepare(&prepared);
        if(status!=p::source::Status::complete){std::cerr<<name<<" preparation status "<<int(status)<<" op "<<prepared.last_operation<<" error "<<f.session->last_error()<<"\n";for(const auto& call:f.native_names)std::cerr<<call<<' ';std::cerr<<'\n';}
        check(status==p::source::Status::complete,"actual source preparation failed");
        check(prepared.script_allocations==13 && prepared.null_appends==8 && f.session->loaded_path_count()==15,"actual class script preparation differed");
        check(f.session->script_path()=="data/scripts/ai/" && f.debug.counters().read_opens==1 && f.debug.counters().read_closes==1,"path/Debug source effects lost");
        auto slots=f.owner->slots(p::source::List::skill);auto faeries=f.owner->slots(p::source::List::faery);
        auto before=f.session->statistics().load_calls;check(f.owner->prepare(&prepared)==p::source::Status::complete && before==f.session->statistics().load_calls && slots==f.owner->slots(p::source::List::skill) && faeries==f.owner->slots(p::source::List::faery),"repeat preparation replayed source");
        auto* passive=f.owner->instance(slots.back()); // Last eight slots are source nulls.
        check(!passive,"source null slots manufactured instances");
        auto id=slots[7];passive=f.owner->instance(id);check(passive,"actual passive skill missing");
        std::string script=passive->script_name;dh2_script_value slot{};slot.type=3;slot.number=7;
        Return selected;check(!f.call("SetSkill",{f.string(script),slot},selected) && selected.count==0,"real SetSkill failed");
        Return check0,check1;check(!f.call("OnSkillCheck",{},check0,0) && !f.call("OnSkillCheck",{},check1,1) && check0.type==1 && !check0.boolean && check1.type==1 && check1.boolean,"authored passive check differs");callbacks+=3;
        auto epoch=dh2_script_vm_required_failure_epoch(f.session->vm());Return update;
        check(f.call("OnSkillUpdate",{},update)==-5 && !update.observed && dh2_script_vm_required_failure_epoch(f.session->vm())==epoch+1,"missing skill level/buff provider silently succeeded");++errors;
        check(f.native_names.back()=="GetCurrentSkillInfo__","source update failed at wrong dependency");
        const std::int32_t idle=3;dh2::player_skill_update_session_v1::Runtime runtime(*f.session,*f.owner,AIS,CHAR,idle);
        dh2::player_skill_update_session_v1::Result all{};std::string update_error;
        check(runtime.update(all,update_error)==-1 && all.last_lua_status==-5 && runtime.retained_failed_returns()==1 && all.callbacks==0,"source UpdateAllSkills/OnSkillUpdate failed prefix differs");
        check(f.native_names.back()=="GetCurrentSkillInfo__","source first active skill failed at wrong dependency");++errors;
        paths.insert(f.loaded_paths.begin(),f.loaded_paths.end());++class_cases;
    }
    Fixture f(cache,temp/"protocol",cat,"KnightPlayerBase");f.common();
    s::LoadResult loaded{};f.resolver_reentry=true;check(!f.load("data/scripts/ai/_commons.luac",loaded) && loaded.cache_hit,"resolved hit reloaded common");f.resolver_reentry=false;++guards;
    check(!f.last_resource.expired(),"resolved byte ownership not retained");++guards;
    f.overlay("fixture/vcb","AddToVFTable('OnKill','fixture_kill'); function fixture_kill() return true end; function OnUpdate() return true end");
    check(!f.load("fixture/vcb",loaded),"VCB fixture load failed");dh2::ais_player_init_vcb::Result vcb{};std::string error;
    check(!f.session->initialize_vcb(&vcb,error) && (f.ais.flags_b8&0x400) && !(f.ais.flags_b8&1) && vcb.service_calls==3,"Player VCB was replaced by External/global membership");++callbacks;
    Return kill;check(!f.call("OnKill",{},kill) && kill.boolean,"actual alias did not dispatch");++callbacks;
    check(!f.session->call("OnKill",nullptr,0,0,observe_reentry,f.session.get(),error),"source return observer reentry guard failed");++guards;
    Fixture combat(cache,temp/"combat",cat,"KnightPlayerBase");combat.common();
    const std::string combat_script=
        "AddToVFTable('OnTargetHit','fixture_player_hit');"
        "AddToVFTable('OnTargetMissed','fixture_player_missed');"
        "function fixture_player_hit(a,d) hit_attacker=a._this; hit_defender=d._this; hit_arg_types=type(a)..':'..type(d) end;"
        "function fixture_player_missed(a,d) miss_attacker=a._this; miss_defender=d._this; miss_arg_types=type(a)..':'..type(d) end";
    combat.overlay("fixture/combat",combat_script);check(!combat.load("fixture/combat",loaded),"Player combat source fixture failed");
    dh2::ais_player_init_vcb::Result combat_vcb{};
    check(!combat.session->initialize_vcb(&combat_vcb,error) &&
        (combat.ais.flags_b8&0x800) && (combat.ais.flags_b8&0x1000),"Player hit/missed VCB membership was not retained");
    const auto combat_vm=combat.session->vm();
    constexpr std::uintptr_t ENEMY=0x300000007ull;
    check(!combat.session->dispatch_combat_result(AIS,dh2::ais_combat_result_dispatch_v1::Callback::target_hit,
        ENEMY,CHAR,error),"Player hit callback dispatch failed");
    dh2_script_value seen_attacker{},seen_defender{},seen_types{};
    check(!dh2_script_vm_get_global(combat_vm,"hit_attacker",&seen_attacker) && seen_attacker.type==DH2_SCRIPT_IDENTITY && seen_attacker.identity==ENEMY &&
        !dh2_script_vm_get_global(combat_vm,"hit_defender",&seen_defender) && seen_defender.type==DH2_SCRIPT_IDENTITY && seen_defender.identity==CHAR &&
        !dh2_script_vm_get_global(combat_vm,"hit_arg_types",&seen_types) && seen_types.type==DH2_SCRIPT_STRING && std::string(seen_types.text,seen_types.text_bytes)=="table:table",
        "Player hit callback userdata/order differs");
    check(!combat.session->dispatch_combat_result(AIS,dh2::ais_combat_result_dispatch_v1::Callback::target_missed,
        CHAR,ENEMY,error),"Player missed callback dispatch failed");
    check(!dh2_script_vm_get_global(combat_vm,"miss_attacker",&seen_attacker) && seen_attacker.identity==CHAR &&
        !dh2_script_vm_get_global(combat_vm,"miss_defender",&seen_defender) && seen_defender.identity==ENEMY &&
        !dh2_script_vm_get_global(combat_vm,"miss_arg_types",&seen_types) && std::string(seen_types.text,seen_types.text_bytes)=="table:table" &&
        combat.session->vm()==combat_vm && combat.session->statistics().combat_callback_calls==2,
        "Player missed callback changed owner, ABI, order, or retained VM");
    error="sentinel";
    check(combat.session->dispatch_combat_result(AIS+1,dh2::ais_combat_result_dispatch_v1::Callback::target_hit,ENEMY,CHAR,error)==-1 && error=="sentinel",
        "Player combat accepted a different AIS owner");
    ++callbacks;
    s::LoadResult untouched{1,2,3,4},saved=untouched;error="sentinel";
    check(f.session->load_resolved("",&untouched,error)==-1 && !std::memcmp(&untouched,&saved,sizeof(saved)) && error=="sentinel","invalid load changed outputs");++guards;
    check(f.session->load_resolved("fixture/vcb",reinterpret_cast<s::LoadResult*>(&f.ais),error)==-1 && (f.ais.flags_b8&0x400),"load result aliases AIS");++guards;
    check(f.session->initialize_vcb(reinterpret_cast<dh2::ais_player_init_vcb::Result*>(&f.ais),error)==-1,"VCB result aliases AIS");++guards;
    for(auto* control:{static_cast<void*>(f.session.get()),static_cast<void*>(&f.debug.globals()),static_cast<void*>(f.debug.globals().singleton)}){
        const auto vm=f.session->vm();
        check(f.session->load_resolved("fixture/vcb",reinterpret_cast<s::LoadResult*>(control),error)==-1 && f.session->vm()==vm,"load result aliases owning control");
        check(f.session->initialize_vcb(reinterpret_cast<dh2::ais_player_init_vcb::Result*>(control),error)==-1 && f.session->vm()==vm,"VCB result aliases owning control");guards+=2;
    }
    alignas(s::LoadResult) unsigned char misaligned[sizeof(s::LoadResult)+8];std::memset(misaligned,0x5a,sizeof(misaligned));
    check(f.session->load_resolved("fixture/vcb",reinterpret_cast<s::LoadResult*>(misaligned+1),error)==-1 && misaligned[1]==0x5a,"misaligned load output accepted");++guards;
    p::source::State projected{};projected.owner=CHAR;projected.ai=AIS;p::source::Request request{};request.operation=p::source::Operation::capture_script_path;request.receiver=AIS;
    auto services=f.session->preparation_services();
    check(services.invoke(services.context,&projected,&request,nullptr,reinterpret_cast<p::source::Response*>(services.context))==-1 && f.session->vm(),"preparation output aliases Session Impl");++guards;
    check(f.session->bind_ais_functions(error)==-1 && f.session->bind_character_functions(error)==-1,"late stages rebound closures");++guards;
    f.overlay("fixture/syntax","invalid lua ???");check(!f.load("fixture/syntax",loaded) && loaded.vm_status>0 && !loaded.source_success && !f.session->contains_path("fixture/syntax"),"syntax error lost source status");++errors;
    f.overlay("fixture/partial","partial_effect=17; error('normal Lua failure')");check(!f.load("fixture/partial",loaded) && loaded.vm_status>0 && !f.session->contains_path("fixture/partial"),"normal error cached path");dh2_script_value global{};check(!dh2_script_vm_get_global(f.session->vm(),"partial_effect",&global) && global.number==17,"Lua failure rolled back effects");++errors;
    f.overlay("fixture/unsupported","error({})");check(f.load("fixture/unsupported",loaded)==-4 && !f.session->contains_path("fixture/unsupported"),"nonstring error treated as source false");++errors;
    f.overlay("fixture/caught","caught=pcall(function() GetHostPlayer() end); function caught_result() return caught end");check(f.load("fixture/caught",loaded)==-5 && loaded.source_success && f.session->contains_path("fixture/caught"),"caught required failure lost cache/source effects");++errors;
    Return caught;check(!f.call("caught_result",{},caught) && caught.type==1 && !caught.boolean,"caught Lua error state lost");++callbacks;
    f.overlay("fixture/call-errors","function nested_failure() pcall(function() GetHostPlayer() end); return 12 end; function normal_failure() error('source error') end; function bad_error() error({}) end; function many_returns() return 1,2,3,'retained' end");check(!f.load("fixture/call-errors",loaded),"error functions load failed");
    Return r;check(f.call("nested_failure",{},r)==-5 && r.observed==1 && r.number==12,"caught required call lost source observer effects");++errors;
    r={};
    check(f.call("normal_failure",{},r)>0 && !r.observed,"normal call failure status lost");++errors;
    check(f.call("bad_error",{},r)==-4 && !r.observed,"unsupported call error lost");++errors;
    check(!f.call("many_returns",{},r,3) && r.count==4 && r.type==4 && r.text=="retained","indexed returns truncated/replayed");++callbacks;
    Return absent;check(!f.call("many_returns",{},absent,7) && absent.count==4 && absent.type==0,"absent return index is not source nil");++callbacks;
    f.throw_provider=true;check(f.call("nested_failure",{},absent)==-5,"provider throw escaped/caught as success");f.throw_provider=false;++errors;
    f.throw_resolver=true;check(f.load("fixture/unreached",loaded)==-1,"resolver throw accepted");f.throw_resolver=false;++errors;
    auto vm=f.session->vm();auto count=f.session->loaded_path_count();check(f.session->vm()==vm && f.session->loaded_path_count()==count,"protocol changed owning VM");++guards;
    // Closing calls a real Lua __gc closure through the still-live borrowed
    // provider. Required missing effects remain failures, never fake cleanup.
    f.overlay("fixture/close","finalizer=newproxy(true); getmetatable(finalizer).__gc=function() Trace('close provider retained') end");check(!f.load("fixture/close",loaded),"finalizer fixture failed");
    auto native_before=f.native_names.size();f.session.reset();check(f.native_names.size()==native_before+1 && f.native_names.back()=="Trace","provider retired before VM close");++guards;
    bool retired=false;Fixture* retiring=nullptr;s::Session* retiring_session=nullptr;
    auto token=std::shared_ptr<void>(new int(7),[&](void* raw){
        check(retiring && retiring->native_names.back()=="Trace","lifetime token retired before real VM close");
        s::LoadResult out{9,8,7,6},before=out;std::string unchanged="sentinel";
        check(retiring_session->load_resolved("fixture/close",&out,unchanged)==-1 && !std::memcmp(&out,&before,sizeof(before)) && unchanged=="sentinel","retirement reentry changed outputs");
        delete static_cast<int*>(raw);retired=true;
    });
    Fixture lifecycle(cache,temp/"lifecycle",cat,"KnightPlayerBase",false,std::move(token));retiring=&lifecycle;retiring_session=lifecycle.session.get();
    s::LoadResult created{1,2,3,4},created_before=created;error="sentinel";
    check(lifecycle.session->load_resolved("fixture/close",&created,error)==-1 && !std::memcmp(&created,&created_before,sizeof(created)) && error=="sentinel","created Session loaded source");++guards;
    check(lifecycle.session->bind_character_functions(error)==-1 && lifecycle.session->stage()==s::Stage::created,"Character bound before AIS");++guards;
    check(!lifecycle.session->bind_ais_functions(error) && !lifecycle.session->bind_character_functions(error),"lifecycle bindings failed");
    lifecycle.overlay("fixture/close","finalizer=newproxy(true); getmetatable(finalizer).__gc=function() Trace('token close') end");
    check(!lifecycle.load("fixture/close",loaded) && !retired,"lifetime token retired during active VM");
    lifecycle.session.reset();check(retired,"Session retained provider token after closing VM");++guards;
    check(paths.size()==31,"three-class source file footprint changed");
    std::cout<<"{\"validation\":\"PASS\",\"class_cases\":"<<class_cases<<",\"registered_class_slots\":24,\"registered_faery_slots\":15,\"unique_skill_files\":29,\"unchanged_script_files\":"<<paths.size()<<",\"guards\":"<<guards<<",\"error_cases\":"<<errors<<",\"callback_cases\":"<<callbacks<<",\"player_combat_same_vm\":true,\"player_combat_userdata_order\":true,\"native_wired\":false,\"OnSkillUpdate_complete\":false}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
