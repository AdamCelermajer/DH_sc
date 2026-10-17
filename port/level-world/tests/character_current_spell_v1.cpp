#include "../character_current_spell_v1.hpp"
#include "../player_skill_tables_adapter.hpp"
#include "../../game-data/properties.hpp"
#include "../../game-data/class_tables.hpp"
extern "C" {
#include "../../pydata-constants/constants.h"
}
#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_current_spell_v1;
namespace d=dh2::data;
constexpr std::uintptr_t C=0x100000001ull;
void require(bool b,const char* why){if(!b)throw std::runtime_error(why);}
std::uint32_t word(const char* text){return static_cast<std::uint32_t>(std::stoull(text));}
std::int32_t signed_word(std::uint32_t word){std::int32_t value;std::memcpy(&value,&word,4);return value;}
std::vector<std::uint8_t> read(const std::filesystem::path& path){std::ifstream f(path,std::ios::binary);require(bool(f),"cache input missing");return {std::istreambuf_iterator<char>(f),{}};}
d::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
struct Fixture {
    std::int32_t first=0,second=4,level=65535;unsigned selections=0;
    int failure=-1;bool throws=false,mutate=false,nested=false;unsigned effects=0;
    k::Bindings bindings{C,{this,invoke}};
    std::vector<std::array<std::int64_t,4>> trace;
    static int invoke(void* raw,const k::Request* q,k::Response* out){
        auto& s=*static_cast<Fixture*>(raw);require(q->character==C && q->difficulty==-1,"captured owner/difficulty changed");
        s.trace.push_back({static_cast<std::int64_t>(q->operation),q->id,q->difficulty,q->character==C?1:2});
        const auto ordinal=s.effects++;
        if(q->operation==k::Operation::selected_faery)out->value=s.selections++?s.second:s.first;
        if(q->operation==k::Operation::validate_faery)require(q->id==static_cast<std::uint32_t>(s.first),"first selection not validated");
        if(q->operation==k::Operation::saved_level){require(q->id==static_cast<std::uint32_t>(s.second),"second selection not used for saved level");out->value=s.level;}
        if(s.mutate && !ordinal){s.bindings.character=C+1;s.bindings.services.invoke=nullptr;}
        if(s.nested && !ordinal){s.nested=false;Fixture other;k::Result r{};require(k::query(C,&other.bindings.services,&r)==k::Status::complete && r.level==65535,"independent nested query failed");}
        if(s.failure==static_cast<int>(ordinal)){if(s.throws)throw std::runtime_error("provider failed after effect");return -1;}
        return 0;
    }
    void print(const k::Result& r,k::Status status)const{
        std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"first\":"<<r.first_id<<",\"second\":"<<r.second_id<<",\"level\":"<<r.level<<",\"calls\":"<<r.calls<<",\"complete\":"<<r.complete<<",\"trace\":[";
        for(std::size_t i=0;i<trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned n=0;n<4;++n){if(n)std::cout<<',';std::cout<<trace[i][n];}std::cout<<']';}std::cout<<"]}\n";
    }
};
struct Catalogue {
    std::shared_ptr<const dh2::player_skill_tables_adapter::Tables> tables;
    d::CharacterTable characters;d::PropertyRules rules;d::ClassTables classes;
    std::vector<std::uint8_t> constant_bytes;dh2_pycst_view constants{};std::int32_t count=0;
    explicit Catalogue(const std::filesystem::path& cache){
        auto load=[&](const char* stem,const char* suffix){return read(cache/"data/pydata"/(std::string(stem)+suffix+".bin"));};
        std::string error;d::SkillTables skills;d::FaeryTables faeries;
        auto a=load("skills","_pyarray"),b=load("skills","_pyarraynames"),c=load("skills","_pystructnames");
        require(d::load_skill_tables(bytes(a),bytes(b),bytes(c),skills,error),error.c_str());
        a=load("faeries","_pyarray");b=load("faeries","_pyarraynames");c=load("faeries","_pystructnames");
        require(d::load_faery_tables(bytes(a),bytes(b),bytes(c),faeries,error),error.c_str());
        tables=dh2::player_skill_tables_adapter::Tables::create(std::move(skills),std::move(faeries),error);require(bool(tables),error.c_str());
        a=load("character_properties","_pyarray");b=load("character_properties","_pyarraynames");c=load("character_properties","_pystructnames");
        require(d::load_characters(bytes(a),bytes(b),bytes(c),characters,error) && d::load_property_rules(characters,rules,error),error.c_str());
        a=load("character_classes","_pyarray");b=load("character_classes","_pyarraynames");c=load("character_classes","_pystructnames");
        require(d::load_classes(bytes(a),bytes(b),bytes(c),classes,error),error.c_str());
        constant_bytes=load("faeries","_pycst");require(!dh2_pycst_open(&constants,constant_bytes.data(),static_cast<std::uint32_t>(constant_bytes.size())),"faery constants rejected");
        dh2_pycst_result found{};require(!dh2_pycst_get(&constants,"FaeryTypes",10,"COUNT",5,&found) && found.found,"actual COUNT missing");count=found.value;require(count==5,"actual faery COUNT differs");
    }
    std::int32_t selector(unsigned which){
        const char* name=which==0?"KnightPlayerBase":which==1?"MagePlayerBase":"RoguePlayerBase";
        auto at=std::find(characters.names.begin(),characters.names.end(),name);require(at!=characters.names.end(),"class row missing");
        d::PropertyState properties;d::reset_properties(rules,properties,&characters.rows.at(static_cast<std::size_t>(at-characters.names.begin())));
        std::string error;require(d::recalc_properties_with_class(classes,rules,properties,error),error.c_str());return properties.resolved[29];
    }
    void describe(){
        std::cout<<"{\"count\":"<<count<<",\"selectors\":[";for(unsigned i=0;i<3;++i){if(i)std::cout<<',';std::cout<<selector(i);}std::cout<<"],\"lists\":[";
        const auto& source=tables->source_faeries();
        for(unsigned i=0;i<source.list_count;++i){if(i)std::cout<<',';const auto& row=source.list_rows[i];std::cout<<'[';for(int n=0;n<row.list_size;++n){if(n)std::cout<<',';std::cout<<row.members[n];}std::cout<<']';}
        std::cout<<"],\"rows\":[";for(unsigned i=0;i<source.faery_count;++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned n=0;n<9;++n){if(n)std::cout<<',';std::cout<<source.faery_rows[i].words[n];}std::cout<<']';}std::cout<<"]}\n";
    }
};
struct Borrowed {
    Catalogue& catalogue;d::PlayerSavegameV1 save,alternate;
    const d::PlayerSavegameV1* selected=&save;
    std::int32_t difficulty=0,list=0;unsigned queries=0,mutation=0;bool fail_constant=false;
    dh2::character_faery_selection::Globals globals;
    dh2::character_faery_selection::Services faery;
    k::SavedBindings borrowed;k::Bindings bindings;
    Borrowed(Catalogue& c,unsigned which,bool initialized=true):catalogue(c),list(c.selector(which)),globals{&c.tables->source_faeries(),0},faery{this,constant,nullptr},
        borrowed{C,&selected,&difficulty,&list,&globals,&faery},bindings{C,k::saved_services(&borrowed)}{
        save.set_character(C);alternate.set_character(C);if(initialized){save.initialize_faeries();alternate.initialize_faeries();}
        std::string error;
        if(initialized)for(unsigned d=0;d<3;++d)for(unsigned id=0;id<5;++id){require(save.set_faery_level(id,static_cast<int>(100*d+id),d,error),error.c_str());require(alternate.set_faery_level(id,static_cast<int>(1000+100*d+id),d,error),error.c_str());}
    }
    void selection(unsigned id){std::array<std::uint32_t,3> ids{id,id,id};std::size_t used=0;std::string error;require(save.load_current_faery({reinterpret_cast<const std::uint8_t*>(ids.data()),sizeof(ids)},used,error) && used==12,"profile current selection rejected");}
    static int constant(void* raw,dh2::character_faery_selection::Character* character,const dh2::character_faery_selection::Request* q,dh2::character_faery_selection::Response* out){
        auto& s=*static_cast<Borrowed*>(raw);require(character->identity==C,"wrong Character validation");++s.queries;
        dh2_pycst_result found{};require(!dh2_pycst_get(&s.catalogue.constants,q->category,static_cast<std::uint32_t>(std::strlen(q->category)),q->key,static_cast<std::uint32_t>(std::strlen(q->key)),&found) && found.found,"real faery COUNT delivery failed");out->word=found.value;
        if(s.queries==1){if(s.mutation==1)s.difficulty=1;if(s.mutation==2)s.selected=&s.alternate;if(s.mutation==3)s.selected=nullptr;}
        return s.fail_constant?-1:0;
    }
};
struct Observation {std::uint32_t count=0;float number=0;bool first=false;unsigned calls=0;static int observe(void* raw,const dh2_script_first_return_v1* values,std::uint32_t count,char*,std::size_t)noexcept{
    auto& s=*static_cast<Observation*>(raw);++s.calls;s.count=count;if(count){s.number=values[0].number;s.first=values[0].boolean!=0;}return 0;}};
int main(int argc,char** argv){try{
    if(argc==6 && std::string(argv[1])=="--oracle"){
        Fixture f;f.first=signed_word(word(argv[2]));f.second=signed_word(word(argv[3]));f.level=signed_word(word(argv[4]));f.mutate=word(argv[5])!=0;k::Result r{};
        auto status=k::query(C,&f.bindings.services,&r);f.print(r,status);return 0;
    }
    require(argc>=2,"cache argument missing");Catalogue catalogue(argv[1]);
    if(argc==3 && std::string(argv[2])=="--view"){catalogue.describe();return 0;}
    if(argc==7 && std::string(argv[2])=="--borrowed"){
        Borrowed b(catalogue,word(argv[3]));b.difficulty=signed_word(word(argv[4]));b.selection(word(argv[5]));b.mutation=word(argv[6]);k::Result r{};
        auto status=k::query(C,&b.bindings.services,&r);
        std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"first\":"<<r.first_id<<",\"second\":"<<r.second_id<<",\"level\":"<<r.level<<",\"calls\":"<<r.calls<<",\"queries\":"<<b.queries<<",\"complete\":"<<r.complete<<"}\n";return 0;
    }
    unsigned valid=0,failures=0,guards=0,lua=0;
    for(unsigned class_id=0;class_id<3;++class_id)for(unsigned d=0;d<3;++d)for(unsigned id=0;id<5;++id){
        Borrowed b(catalogue,class_id);b.difficulty=static_cast<int>(d);b.selection(id);k::Result r{};
        require(k::query(C,&b.bindings.services,&r)==k::Status::complete && r.level==static_cast<int>(100*d+id) && r.calls==4 && b.queries==2,"real class/faery/saved query differs");++valid;
    }
    for(int phase=0;phase<4;++phase)for(bool throws:{false,true}){Fixture f;f.failure=phase;f.throws=throws;k::Result r{};
        require(k::query(C,&f.bindings.services,&r)==k::Status::provider_failed && r.calls==static_cast<unsigned>(phase+1) && f.effects==r.calls && !r.complete,"provider failure added actions");++failures;}
    {Fixture f;f.mutate=true;auto services=f.bindings.services;k::Result r{};require(k::query(C,&services,&r)==k::Status::complete && r.character==C && r.level==65535,"provider/Character capture lost");++valid;}
    {Fixture f;f.nested=true;k::Result r{};require(k::query(C,&f.bindings.services,&r)==k::Status::complete,"nested distinct output rejected");++valid;}
    {Fixture f;k::Result r{};r.level=77;auto old=r;
        auto invalid=[&](k::Status s){require(s==k::Status::invalid_argument && !std::memcmp(&r,&old,sizeof(r)) && f.effects==0,"invalid query changed output/effects");++guards;};
        invalid(k::query(0,&f.bindings.services,&r));invalid(k::query(C,nullptr,&r));
        invalid(k::query(C,&f.bindings.services,reinterpret_cast<k::Result*>(UINTPTR_MAX-7)));
        invalid(k::query(C,&f.bindings.services,reinterpret_cast<k::Result*>(reinterpret_cast<std::uintptr_t>(&r)+1)));
        require(k::query(C,&f.bindings.services,reinterpret_cast<k::Result*>(&f.bindings.services))==k::Status::invalid_argument && !f.effects,"service/output alias accepted");++guards;
        f.bindings.services.invoke=nullptr;invalid(k::query(C,&f.bindings.services,&r));
    }
    Borrowed b(catalogue,0);k::Result r{};
    b.borrowed.difficulty=nullptr;b.selected=nullptr;require(k::query(C,&b.bindings.services,&r)==k::Status::complete && r.level==-1 && r.first_id==0 && r.second_id==0,"null save read missing difficulty");++valid;
    b.borrowed.difficulty=&b.difficulty;b.selected=&b.save;
    b.alternate.set_character(C+1);b.selected=&b.alternate;require(k::query(C,&b.bindings.services,&r)==k::Status::provider_failed && r.calls==1,"foreign save accepted");++guards;b.selected=&b.save;
    b.difficulty=3;require(k::query(C,&b.bindings.services,&r)==k::Status::provider_failed && r.calls==1,"unsafe difficulty accepted");++guards;b.difficulty=0;
    b.list=-1;require(k::query(C,&b.bindings.services,&r)==k::Status::complete && r.level==0 && r.calls==4,"genuine faery list0 fallback rejected");++valid;b.list=catalogue.selector(0);
    b.borrowed.difficulty=reinterpret_cast<const std::int32_t*>(reinterpret_cast<std::uintptr_t>(&b.difficulty)+1);
    require(k::query(C,&b.bindings.services,&r)==k::Status::provider_failed && r.calls==1,"misaligned difficulty read");++guards;b.borrowed.difficulty=&b.difficulty;
    b.borrowed.faery_list_106c=reinterpret_cast<const std::int32_t*>(reinterpret_cast<std::uintptr_t>(&b.list)+1);
    require(k::query(C,&b.bindings.services,&r)==k::Status::provider_failed && r.calls==2,"misaligned faery field read");++guards;b.borrowed.faery_list_106c=&b.list;
    b.selected=reinterpret_cast<const d::PlayerSavegameV1*>(UINTPTR_MAX-15);
    require(k::query(C,&b.bindings.services,&r)==k::Status::provider_failed && r.calls==1,"wrapping saved owner read");++guards;b.selected=&b.save;
    {Borrowed fresh(catalogue,0,false);require(k::query(C,&fresh.bindings.services,&r)==k::Status::provider_failed && r.calls==4,"uninitialized saved backing invented");++guards;fresh.save.initialize_faeries();require(k::query(C,&fresh.bindings.services,&r)==k::Status::complete && r.level==0,"real fresh faery initialization did not preserve ctor selection/zero level");++valid;}
    b.fail_constant=true;unsigned prior=b.queries;require(k::query(C,&b.bindings.services,&r)==k::Status::provider_failed && r.calls==2 && b.queries==prior+1,"validation error skipped prefix/continued read");++failures;b.fail_constant=false;
    {Fixture f;dh2_script_value out{};std::uint32_t returned=99;char error[128]{};
        require(k::current_spell_info_v1(&f.bindings,reinterpret_cast<const dh2_script_value*>(1),UINT32_MAX,&out,1,&returned,error,sizeof(error))==0 && returned==1 && out.number==65535,"ignored Arguments read or changed arity");++valid;
        f.effects=0;returned=99;require(k::current_spell_info_v1(&f.bindings,nullptr,0,reinterpret_cast<dh2_script_value*>(&f.bindings),1,&returned,error,sizeof(error))==-1001 && returned==99 && !f.effects,"callback output/control alias accepted");++guards;
        require(k::current_spell_info_v1(&f.bindings,nullptr,0,&out,1,reinterpret_cast<std::uint32_t*>(&f.bindings),error,sizeof(error))==-1001 && !f.effects,"callback count/control alias accepted");++guards;
    }
    // One selected real float32 VM; callback backing and the canonical table /
    // saved owner survive repeated calls representing graphics recreation.
    using Vm=std::unique_ptr<dh2_script_vm,decltype(&dh2_script_vm_destroy)>;
    Vm vm(dh2_script_vm_create(1024*1024),dh2_script_vm_destroy);require(bool(vm),"real Lua VM unavailable");auto* identity=vm.get();
    require(!dh2_script_vm_bind_source_values(vm.get(),"GetCurrentSpellInfo",k::current_spell_info_v1,&b.bindings),"source callback bind failed");
    const char* script="queries=0; function spell() queries=queries+1; return GetCurrentSpellInfo({},'ignored',false) end; function caught_spell() local ok=pcall(GetCurrentSpellInfo); return ok,queries end";
    require(!dh2_script_vm_load_source_file(vm.get(),script,std::strlen(script)),"real Lua source failed");
    for(unsigned i=0;i<3;++i){Observation o;require(!dh2_script_vm_call_all_source_v1(vm.get(),"spell",nullptr,0,Observation::observe,&o) && o.count==1 && o.number==0 && vm.get()==identity,"same-VM spell query changed owners");++lua;}
    b.fail_constant=true;auto epoch=dh2_script_vm_required_failure_epoch(vm.get());Observation o;
    require(dh2_script_vm_call_all_source_v1(vm.get(),"caught_spell",nullptr,0,Observation::observe,&o)==-5 && o.count==2 && !o.first && o.calls==1 && dh2_script_vm_required_failure_epoch(vm.get())==epoch+1,"caught required failure became successful native result");++lua;
    b.fail_constant=false;require(!dh2_script_vm_call_all_source_v1(vm.get(),"spell",nullptr,0,Observation::observe,&o) && o.number==0,"same VM did not recover after provider error");++lua;
    vm.reset();
    std::cout<<"{\"validation\":\"PASS\",\"actual_data_cases\":"<<valid<<",\"failure_cases\":"<<failures<<",\"guards\":"<<guards<<",\"real_lua_cases\":"<<lua<<",\"same_vm\":true,\"new_saved_owner\":false,\"native_wired\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
