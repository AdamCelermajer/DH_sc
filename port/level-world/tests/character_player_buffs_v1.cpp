#include "../character_player_buffs_v1.hpp"
#include "../character_coordinator.hpp"
extern "C" {
#include "../../adam-script-runtime/script_runtime.h"
#include "../../pydata-constants/constants.h"
}
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace b=dh2::character_player_buffs_v1;
namespace d=dh2::data;
namespace c=dh2::character;
constexpr std::uintptr_t CHAR=0x100000001ull;
void ck(bool value,const char* reason){if(!value)throw std::runtime_error(reason);}
std::vector<std::uint8_t> read(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);ck(bool(f),"resource missing");return {std::istreambuf_iterator<char>(f),{}};}
d::Bytes bytes(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
struct Catalogue {
    d::ClassTables classes;d::CharacterTable characters;d::PropertyRules rules;
    std::vector<d::ClassRow> rows;std::vector<std::uint8_t> constants;
    explicit Catalogue(const std::filesystem::path& cache){
        auto load=[&](const char* stem,const char* suffix){return read(cache/"data/pydata"/(std::string(stem)+suffix+".bin"));};
        auto a=load("character_classes","_pyarray"),n=load("character_classes","_pyarraynames"),s=load("character_classes","_pystructnames");std::string error;
        ck(d::load_classes(bytes(a),bytes(n),bytes(s),classes,error),error.c_str());
        a=load("character_properties","_pyarray");n=load("character_properties","_pyarraynames");s=load("character_properties","_pystructnames");
        ck(d::load_characters(bytes(a),bytes(n),bytes(s),characters,error)&&d::load_property_rules(characters,rules,error),error.c_str());
        for(const auto& r:classes.rows)rows.push_back({r.data(),std::uint32_t(r.size())});
        constants=load("design","_pycst");
    }
    int class_id(const std::string& name)const{auto p=std::find(classes.names.begin(),classes.names.end(),name);ck(p!=classes.names.end(),"class missing");return int(p-classes.names.begin());}
    int character_id(const std::string& name)const{auto p=std::find(characters.names.begin(),characters.names.end(),name);ck(p!=characters.names.end(),"character missing");return int(p-characters.names.begin());}
};
struct Fixture {
    Catalogue& cat;d::PropertyState props;d::PropertyView view;c::Coordinator character{CHAR,2};
    std::unique_ptr<b::Owner> owner;b::CallbackBindings callbacks{};
    std::vector<b::Operation> calls;int fail_at=-1;bool throw_failure=false,mutate_failure=false,fx_available=false;
    std::int32_t timer_override=INT32_MAX;unsigned expirations=0,fx_loads=0,fx_releases=0;dh2_script_vm* vm=nullptr;
    int level=0,element=1;unsigned scalar_calls=0;
    explicit Fixture(Catalogue& data,int base=-1):cat(data){
        d::PropertySheet negative=cat.rules.defaults;negative[26]=-1;negative[19]=256;
        d::reset_properties(cat.rules,props,base<0?&negative:&cat.characters.rows.at(base));view=d::property_view(cat.rules,props);
        ck(!dh2_class_recalc_base(cat.rows.data(),std::uint32_t(cat.rows.size()),props.base.data(),&view),"initial recalc");
        character.bind({this,[](void*){c::Facts f{};f.is_player=1;return f;},{this,[](void*,c::State*,const c::Request*){}},
            [](void* p,c::Coordinator& co,int event,c::Timer32& timer,std::uint32_t){auto& f=*static_cast<Fixture*>(p);ck(&co==&f.character,"second coordinator");if(event==0x36){b::Result r{};ck(f.owner->expired(&timer,&r)==b::Status::complete,"expiry removal");++f.expirations;}},nullptr});
        character.state.current=3;owner=b::Owner::create({CHAR,&view,{this,invoke},std::uint32_t(cat.rows.size()),12});ck(bool(owner),"owner create");callbacks.owner=owner.get();
    }
    ~Fixture(){character.stop_timers();if(vm)dh2_script_vm_destroy(vm);b::Result r{};if(owner)owner->retire(&r);}
    static int invoke(void* p,d::PropertyView* v,const b::Request* q,b::Response* out){
        auto& f=*static_cast<Fixture*>(p);ck(v==&f.view&&q->character==CHAR,"borrowed property/character changed");
        const int index=int(f.calls.size());f.calls.push_back(q->operation);
        if(index==f.fail_at){if(f.mutate_failure&&q->sheet)q->sheet[60]=12345;if(f.throw_failure)throw std::runtime_error("injected");return -1;}
        switch(q->operation){
        case b::Operation::timer_start:ck(q->event==0x36&&!q->repeat,"source timer request");out->word=f.timer_override==INT32_MAX?f.character.start_timer(q->duration,q->repeat,q->event,q->subject):f.timer_override;return out->word< -1?-1:0;
        case b::Operation::timer_stop:return f.character.stop_timer(std::uint32_t(q->id))<0?-1:0;
        case b::Operation::timer_time_left:return dh2_character_timer_time_left(&out->elapsed,&out->duration,&f.character.timers(),std::uint32_t(q->id))==1?0:-1;
        case b::Operation::apply_class:{std::int32_t* sheet=nullptr;ck(f.owner->owned_sheet(q->subject,&sheet)&&sheet==q->sheet,"identity sheet replaced");return int(dh2_class_apply(f.cat.rows.data(),std::uint32_t(f.cat.rows.size()),q->id,q->sheet,v->resolved));}
        case b::Operation::recalculate:return int(dh2_class_recalc_base(f.cat.rows.data(),std::uint32_t(f.cat.rows.size()),f.props.base.data(),v));
        case b::Operation::fx_load:if(!f.fx_available)return -1;++f.fx_loads;out->identity=0x200000001ull;return 0;
        case b::Operation::fx_release:if(!q->subject)return 0;if(!f.fx_available)return -1;++f.fx_releases;return 0;
        case b::Operation::fx_object:if(!f.fx_available)return -1;out->identity=0x300000001ull;return 0;
        case b::Operation::fx_enable:return f.fx_available&&q->enabled==1?0:-1;
        }return -1;
    }
    std::uintptr_t add(int id,unsigned duration=0,int capacity=1,unsigned strength=1,int fx=-1){b::Result r{};ck(owner->add(id,duration,capacity,strength,fx,"fixture",&r)==b::Status::complete,"add");return r.instance;}
    void apply(int id,std::uintptr_t identity){b::Result r{};ck(owner->apply(id,identity,&r)==b::Status::complete,"apply");}
    void remove(int id,std::uintptr_t identity){b::Result r{};ck(owner->remove(id,identity,&r)==b::Status::complete,"remove");}
    static void number(dh2_script_value* out,unsigned cap,unsigned* n,float value){ck(cap&&out&&n,"result storage");out[0]={};out[0].type=3;out[0].number=value;*n=1;}
    static int scalar(void* p,const dh2_script_value* a,unsigned n,dh2_script_value* out,unsigned cap,unsigned* returned,char*,std::size_t){
        auto& f=*static_cast<Fixture*>(p);++f.scalar_calls;ck(n==1&&a[0].type==3,"scalar fixture protocol");const int operation=int(a[0].number);
        float value=operation==0?float(f.level):operation==1?float(f.element):0;number(out,cap,returned,value);return 0;
    }
    static int oid(void* p,const dh2_script_value* a,unsigned n,dh2_script_value* out,unsigned cap,unsigned* returned,char*,std::size_t){
        auto& f=*static_cast<Fixture*>(p);ck(n==2&&a[0].type==4&&a[1].type==4,"name fixture protocol");
        const std::string category=a[0].text,key=a[1].text;int value=-1;
        if(category=="ClassTable")value=f.cat.class_id(key);
        else if(category=="CharacterProperties"){auto it=std::find(f.cat.characters.fields.begin(),f.cat.characters.fields.end(),key);ck(it!=f.cat.characters.fields.end(),"property missing");value=int(it-f.cat.characters.fields.begin());}
        else if(category=="Elemental"){dh2_pycst_view view{};dh2_pycst_result r{};ck(!dh2_pycst_open(&view,f.cat.constants.data(),std::uint32_t(f.cat.constants.size()))&&!dh2_pycst_get(&view,category.c_str(),unsigned(category.size()),key.c_str(),unsigned(key.size()),&r)&&r.found,"actual constant missing");value=r.value;}
        else ck(false,"unsupported name fixture");
        number(out,cap,returned,float(value));return 0;
    }
    static int prop(void* p,const dh2_script_value* a,unsigned n,dh2_script_value* out,unsigned cap,unsigned* returned,char*,std::size_t){auto& f=*static_cast<Fixture*>(p);ck(n==1&&a[0].type==3,"prop arguments");int value;ck(!dh2_property_resolve(&f.view,int(a[0].number),&value),"live prop");number(out,cap,returned,float(value));return 0;}
    static int observe(void*,const dh2_script_first_return_v1*,char*,std::size_t){return 0;}
    void lua(const std::filesystem::path& source){
        vm=dh2_script_vm_create(8*1024*1024);ck(vm,"VM create");
        for(auto entry:{std::pair<const char*,dh2_script_function>{"CreateBuff",b::create_buff},{"RemoveBuff",b::remove_buff},{"ApplyBuff",b::apply_buff}})ck(!dh2_script_vm_bind_source_values(vm,entry.first,entry.second,&callbacks),"buff same-VM binding");
        for(const char* name:{"GetPyOID","GetPyStruct","GetPyCst"})ck(!dh2_script_vm_bind_source_values(vm,name,oid,this),"name same-VM binding");
        ck(!dh2_script_vm_bind_source_values(vm,"GetProp",prop,this)&&!dh2_script_vm_bind_source_values(vm,"FixtureScalar",scalar,this),"scalar/property binding");
        const char* prerequisites="function ToFixed(n)return math.floor(n*256)end function MulFixed(a,b)return math.floor(a*b/256)end function GetInt()return 0 end function GetCurrentSpellInfo()return FixtureScalar(0)end function GetCurrentEquippedFaeryLevel()return FixtureScalar(0)end function GetEquippedFaeryElement()return FixtureScalar(1)end function RegisterSkill(update,...) RunUpdate=update end function Protocol(id) local x=CreateBuff(id,0,false,1); ApplyBuff(id,x); return x end function BadFx(id) CreateBuff(id,0,false,1,3)end function CaughtFx(id)local ok=pcall(CreateBuff,id,0,false,1,3);return ok end";
        ck(!dh2_script_vm_load(vm,prerequisites,std::strlen(prerequisites),"fixture prerequisites"),"Lua prerequisites");auto script=read(source);ck(!dh2_script_vm_load_source_file(vm,script.data(),script.size()),dh2_script_vm_error(vm));
    }
};
struct OracleCase {int base,id;d::PropertyState initial,applied,removed;d::PropertySheet buff;};
std::vector<OracleCase> oracle(const std::filesystem::path& p){auto raw=read(p);std::size_t at=0;auto copy=[&](void* out,std::size_t size){ck(size<=raw.size()-at,"fixture truncated");std::memcpy(out,raw.data()+at,size);at+=size;};unsigned count;copy(&count,4);std::vector<OracleCase> cases(count);for(auto& r:cases){copy(&r.base,4);copy(&r.id,4);copy(&r.initial,sizeof(r.initial));copy(&r.buff,sizeof(r.buff));copy(&r.applied,sizeof(r.applied));copy(&r.removed,sizeof(r.removed));}ck(at==raw.size(),"fixture suffix");return cases;}
int main(int argc,char** argv){try{
    ck(argc==3,"cache and original fixture required");std::filesystem::path cache=argv[1];Catalogue cat(cache);auto cases=oracle(argv[2]);unsigned original_cases=0,checks=0,lua_cases=0;
    for(const auto& expected:cases){Fixture f(cat,expected.base);ck(!std::memcmp(&f.props,&expected.initial,sizeof(f.props)),"original initial state differs");auto identity=f.add(expected.id);f.apply(expected.id,identity);std::int32_t* sheet=nullptr;ck(f.owner->owned_sheet(identity,&sheet)&&!std::memcmp(sheet,expected.buff.data(),896),"original buff sheet differs");ck(!std::memcmp(&f.props,&expected.applied,sizeof(f.props)),"original applied state differs");f.remove(expected.id,identity);ck(!std::memcmp(&f.props,&expected.removed,sizeof(f.props))&&!f.owner->owned_sheet(identity,&sheet)&&!f.view.group_count,"original removed state differs");++original_cases;}
    const int fire=cat.class_id("Avalon_FireResistance");
    {Fixture f(cat);auto first=f.add(fire,100,2);f.character.update_timers(20,0);auto second=f.add(fire,100,2);f.character.update_timers(10,0);auto refreshed=f.add(fire,200,2);ck(refreshed==first&&f.owner->count()==2,"elapsed selection did not choose oldest");ck(f.character.timers().slots[0].user_ref==first&&f.character.timers().slots[0].duration_ms==200,"second timer owner");f.apply(fire,first);f.apply(fire,second);f.character.update_timers(90,1);ck(f.owner->count()==2,"blocked timers changed retention");f.character.pause_timer(0,1);f.character.update_timers(90,0);ck(f.owner->count()==1&&f.expirations==1,"expiry deque removal");f.character.pause_timer(0,0);f.character.update_timers(200,0);ck(!f.owner->count()&&!f.view.group_count&&f.expirations==2,"last expiry declaration removal");++checks;}
    {Fixture f(cat);auto neg=f.add(-3),pos=f.add(5),middle=f.add(0);b::Snapshot a{},b{},cc{};ck(f.owner->snapshot(0,&a)&&f.owner->snapshot(1,&b)&&f.owner->snapshot(2,&cc)&&a.instance==neg&&b.instance==middle&&cc.instance==pos,"signed key order");f.view=d::property_view(cat.rules,f.props);ck(f.owner->attach(&f.view)==b::Status::complete&&f.view.group_count==3,"view refresh lost groups");f.remove(-3,999);ck(f.owner->count()==2,"single-instance ignores supplied identity");f.apply(-1,pos);ck(f.calls.back()==b::Operation::recalculate,"ClassID=-1 skipped recalc");++checks;}
    {Fixture f(cat);auto first=f.add(fire,0,2,5);auto second=f.add(fire,0,2,7);ck(!f.add(fire,0,2,3)&&f.owner->count()==2,"weaker capacity appended");ck(f.add(fire,0,2,9)==second,"weaker writes did not choose last");b::Snapshot x{},y{};ck(f.owner->snapshot(0,&x)&&f.owner->snapshot(1,&y)&&x.instance==first&&x.strength==9&&y.strength==9,"completed strength writes lost");f.remove(fire,0);ck(f.owner->count()==2,"null multiple removal");b::Result r{};ck(f.owner->remove_all(&r)==b::Status::complete&&!f.owner->count(),"remove_all");++checks;}
    {Fixture f(cat);f.fail_at=0;b::Result r{};ck(f.owner->add(fire,20,1,1,-1,"x",&r)==b::Status::provider_failed&&f.owner->count()==1&&r.calls==1,"start failure prefix");b::Snapshot s{};ck(f.owner->snapshot(0,&s)&&s.timer==-1&&s.sheet[172]==0,"start failure sheet initialized early");++checks;}
    {Fixture f(cat);f.timer_override=-1;b::Result r{};ck(f.owner->add(fire,20,1,1,-1,"x",&r)==b::Status::complete&&!r.instance&&!f.owner->count()&&r.calls==4,"StartTimer=-1 removal prefix");++checks;}
    {Fixture f(cat);auto id=f.add(fire,20);f.fail_at=int(f.calls.size());b::Result r{};ck(f.owner->remove(fire,id,&r)==b::Status::provider_failed&&f.owner->count()==1&&f.character.timers().slots[0].active,"stop failure prefix");f.fail_at=int(f.calls.size())+1;ck(f.owner->remove(fire,id,&r)==b::Status::provider_failed&&!f.owner->count()&&f.owner->declarations()==1&&!f.view.groups[0].count,"release failure prefix");++checks;}
    {Fixture f(cat);auto id=f.add(fire);f.fail_at=0;f.mutate_failure=true;b::Result r{};ck(f.owner->apply(fire,id,&r)==b::Status::provider_failed&&r.calls==1,"class failure continued");std::int32_t* sheet=nullptr;ck(f.owner->owned_sheet(id,&sheet)&&sheet[60]==12345&&f.owner->count()==1,"class provider prefix rolled back");d::PropertySheet before;std::copy_n(sheet,224,before.begin());f.fail_at=2;f.throw_failure=true;ck(f.owner->apply(fire,id,&r)==b::Status::provider_failed&&r.calls==2&&std::memcmp(sheet,before.data(),896),"recalc failure lost class prefix");++checks;}
    {Fixture f(cat);b::Result r{};ck(f.owner->add(fire,0,1,1,3,"fx",&r)==b::Status::provider_failed&&f.owner->declarations()==1&&!f.owner->count()&&r.last_operation==b::Operation::fx_load,"missing FX silently succeeded");f.fx_available=true;auto id=f.add(fire,0,2,1,3);f.add(fire,0,2,1,3);ck(f.fx_loads==1&&f.calls.back()==b::Operation::fx_enable,"FX ownership or enable");f.remove(fire,id);ck(!f.fx_releases,"nonlast released FX");f.remove(fire,0);ck(f.fx_releases==1,"last FX release");++checks;}
    {Fixture f(cat);dh2_script_value args[5]{},out{};unsigned count=99;char error[128]{};
        auto num=[](float n){dh2_script_value v{};v.type=3;v.number=n;return v;};args[0]=num(float(fire));args[1]=num(-50);args[2].type=1;args[2].boolean=1;args[3]=num(-2);
        ck(!b::create_buff(&f.callbacks,args,4,&out,1,&count,error,sizeof(error))&&count==1,"native unsigned negative conversion");auto first=out.identity;
        ck(!b::create_buff(&f.callbacks,args,4,&out,1,&count,error,sizeof(error))&&f.owner->count()==2&&out.identity!=first,"Boolean true capacity128");
        args[2].boolean=0;args[0]=num(float(fire+1));args[3]=num(4294967296.f);ck(!b::create_buff(&f.callbacks,args,4,&out,1,&count,error,sizeof(error)),"unsigned saturation");b::Snapshot snapshot{};ck(f.owner->snapshot(2,&snapshot)&&snapshot.strength==UINT32_MAX&&snapshot.sheet[172]==-256,"saturated strength word wrap");
        args[4].type=1;args[4].boolean=1;args[0]=num(float(fire+2));f.fx_available=true;ck(!b::create_buff(&f.callbacks,args,5,&out,1,&count,error,sizeof(error))&&f.fx_loads==1,"Boolean FX conversion/catalog guard");
        auto before=f.owner->count();args[0]=num(float(cat.rows.size()));ck(!b::create_buff(&f.callbacks,args,5,&out,1,&count,error,sizeof(error))&&!count&&before==f.owner->count(),"CreateBuff count guard");
        args[0]=num(float(fire+3));args[2].type=4;args[2].text="3";ck(b::create_buff(&f.callbacks,args,4,&out,1,&count,error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&before==f.owner->count(),"unprovided string conversion silently substituted");++checks;}
    {Fixture f(cat);auto id=f.add(fire);std::int32_t* untouched=reinterpret_cast<std::int32_t*>(0x1234);ck(!f.owner->owned_sheet(id+1,&untouched)&&untouched==reinterpret_cast<std::int32_t*>(0x1234),"arbitrary identity resolver writes");b::Snapshot snap{};ck(f.owner->snapshot(0,&snap),"snapshot");b::Result result{4,3,b::Operation::fx_load},saved=result;ck(f.owner->apply(fire,id,reinterpret_cast<b::Result*>(f.props.resolved.data()))==b::Status::invalid_argument&&result.instance==saved.instance,"result aliases live sheets");b::Result retired{};f.character.stop_timers();ck(f.owner->retire(&retired)==b::Status::complete&&!f.owner->owned_sheet(id,&untouched),"retired identity remains registered");++checks;}
    for(int base:{-1,cat.character_id("KnightPlayerBase"),cat.character_id("MagePlayerBase"),cat.character_id("RoguePlayerBase")}){
        Fixture f(cat,base);f.lua(cache/"data/scripts/skills/faerie_celest.luac");
        for(const char* element:{"Fire","Water","Air","Earth","Lightning"})for(int level:{0,1}){
            dh2_pycst_view v{};dh2_pycst_result r{};ck(!dh2_pycst_open(&v,cat.constants.data(),unsigned(cat.constants.size()))&&!dh2_pycst_get(&v,"Elemental",9,element,unsigned(std::strlen(element)),&r)&&r.found,"element constants");f.level=level;f.element=r.value;
            auto* same=f.vm;ck(!dh2_script_vm_call_first_source_v1(f.vm,"RunUpdate",nullptr,0,Fixture::observe,nullptr),dh2_script_vm_error(f.vm));ck(f.vm==same&&f.owner->count()==1,"Celest retained more than one source buff");b::Snapshot snap{};ck(f.owner->snapshot(0,&snap),"Celest snapshot");const char* stem=std::strcmp(element,"Fire")==0?"Avalon_FireResistance":std::strcmp(element,"Water")==0?"Maria_WaterResistance":std::strcmp(element,"Air")==0?"Sylph_WindResistance":std::strcmp(element,"Earth")==0?"Primula_EarthResistance":"Celeste_LightningResistance";ck(snap.id==cat.class_id((level?"AW_":"")+std::string(stem)),"Celest class selected wrong");auto expected=std::find_if(cases.begin(),cases.end(),[&](const auto& x){return x.base==base&&x.id==snap.id;});ck(expected!=cases.end()&&!std::memcmp(snap.sheet,expected->buff.data(),896),"same-VM original buff differs");++lua_cases;
        }
        dh2_script_value a{};a.type=3;a.number=float(fire);auto epoch=dh2_script_vm_required_failure_epoch(f.vm);ck(dh2_script_vm_call_first_source_v1(f.vm,"CaughtFx",&a,1,Fixture::observe,nullptr)==-5&&dh2_script_vm_required_failure_epoch(f.vm)==epoch+1,"same VM swallowed required FX failure");
        f.character.stop_timers();dh2_script_vm_destroy(f.vm);f.vm=nullptr;b::Result retired{};ck(f.owner->retire(&retired)==b::Status::complete&&!f.view.group_count,"VM close then retire");++checks;
    }
    std::cout<<"{\"validation\":\"PASS\",\"original_whole_state_cases\":"<<original_cases<<",\"coordinator_prefix_and_ownership_checks\":"<<checks<<",\"unchanged_celest_same_vm_cases\":"<<lua_cases<<",\"selected_native_build\":false,\"live_gameplay\":false}\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"player buffs: "<<e.what()<<'\n';return 1;}}
