#define main retained_player_session_fixture_main
#include "player_skill_session_v1.cpp"
#undef main
#include "../character_player_scalar_services_v1.hpp"
#include "../character_current_spell_v1.hpp"
#include "../../game-data/player_savegame_v1.hpp"
#include "../../adam-script-runtime/script_function_alias.h"
namespace k=dh2::character_player_scalar_services_v1;
namespace spell=dh2::character_current_spell_v1;
namespace {
dh2_script_value scalar_string(const char* text){dh2_script_value v{};v.type=DH2_SCRIPT_STRING;v.text=text;v.text_bytes=std::strlen(text);return v;}
dh2_script_value scalar_number(float n){dh2_script_value v{};v.type=DH2_SCRIPT_NUMBER;v.number=n;return v;}
struct Conversions {
    k::Arguments* args=nullptr;dh2_script_value alternative[2]{};k::State* state=nullptr;
    const dh2_script_value* replacement=nullptr;
    unsigned strings=0,numbers=0;int failure=0;bool throws=false,shrink=false,nest=false;
    std::string key="coerced";
    static int string(void* raw,const dh2_script_value*,const char** text){
        auto& c=*static_cast<Conversions*>(raw);++c.strings;
        if(c.nest){k::Result nested{};check(k::set(c.state,"nested",19,&nested)==k::Status::complete,"independent conversion nested output failed");}
        if(c.args){c.args->values=c.replacement?c.replacement:c.alternative;if(c.shrink)c.args->count=1;}
        if(c.failure==1){if(c.throws)throw std::runtime_error("string provider failed");return 1;}
        *text=c.key.c_str();return 0;
    }
    static int number(void* raw,const dh2_script_value*,float* number){
        auto& c=*static_cast<Conversions*>(raw);++c.numbers;
        if(c.failure==2){if(c.throws)throw std::runtime_error("number provider failed");return 1;}
        *number=71.5f;return 0;
    }
};
struct SpellProviders {
    Fixture& fixture;d::PlayerSavegameV1 save;const d::PlayerSavegameV1* saved=nullptr;std::int32_t difficulty=0;
    dh2::character_faery_selection::Globals globals{};dh2::character_faery_selection::Services providers{};
    spell::SavedBindings saved_bindings{};spell::Bindings bindings{};
    explicit SpellProviders(Fixture& f):fixture(f){
        save.set_character(CHAR);save.initialize_faeries();saved=&save;
        globals={&f.catalogue.tables->source_faeries(),0};providers={this,constant,nullptr};
        saved_bindings={CHAR,&saved,&difficulty,&f.properties.resolved[29],&globals,&providers};bindings={CHAR,spell::saved_services(&saved_bindings)};
    }
    static std::int32_t constant(void* raw,dh2::character_faery_selection::Character* character,
        const dh2::character_faery_selection::Request* q,dh2::character_faery_selection::Response* out){
        auto& c=*static_cast<SpellProviders*>(raw);if(!character||character->identity!=CHAR||!q||!out)return 1;
        dh2_pycst_view v{};dh2_pycst_result r{};const auto& bytes=c.fixture.catalogue.faery_constants;
        if(dh2_pycst_open(&v,bytes.data(),std::uint32_t(bytes.size()))||dh2_pycst_get(&v,q->category,std::strlen(q->category),q->key,std::strlen(q->key),&r)||!r.found)return 1;
        out->word=r.value;return 0;
    }
};
std::uint32_t uword(const char* p){return std::uint32_t(std::stoull(p));}
float float_word(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
}
int main(int argc,char** argv){try{
    if(argc==8&&std::string(argv[1])=="--oracle"){
        const bool is_set=uword(argv[2])!=0;const auto count=uword(argv[3]);const auto raw=uword(argv[4]);const auto initial=uword(argv[5]);const auto layout=uword(argv[6]);const bool present=(layout&1)!=0;const auto mutate=uword(argv[7]);
        k::IntegerMap map;const char* key="Faery_Cooldown";const auto hash=dh2_script_alias_hash(key);
        std::int32_t prior;std::memcpy(&prior,&initial,4);if(layout&2){map.emplace(hash-1,41);map.emplace(hash+1,43);}if(present)map.emplace(hash,prior);
        k::State state{AIS,&map};dh2_script_value a[4]{scalar_string(key),scalar_number(float_word(raw)),scalar_number(19),scalar_string("ignored")};
        k::Arguments arguments{a,count};Conversions c;c.args=mutate?&arguments:nullptr;c.key=key;c.alternative[1]=scalar_number(float_word(raw^0x80000000u));
        if(mutate){a[0].type=DH2_SCRIPT_TABLE;c.alternative[0]=scalar_string("new vector");}
        const k::Services service{&c,Conversions::string,Conversions::number};k::Result result{};
        const auto status=is_set?k::set_callback(&state,&arguments,&service,&result):k::get_callback(&state,&arguments,&service,&result);
        const auto found=map.find(hash);
        std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<result.value<<",\"returned\":"<<result.returned<<",\"size\":"<<map.size()<<",\"entry\":"<<(found==map.end()?0:found->second)<<",\"hash_reads\":"<<result.hash_reads<<"}\n";return 0;
    }
    check(argc==3,"cache and debug temporary directory required");unsigned cases=0,guards=0,failures=0,lua=0,actual=0;
    k::IntegerMap map;k::State state{AIS,&map};k::Result r{};std::int32_t value=-9;
    check(k::get(&state,"Faery_Cooldown",&value,&r)==k::Status::complete&&value==0&&map.size()==1&&r.inserted==1&&r.hash_reads==2,"missing source integer did not insert zero");++cases;
    check(k::get(&state,"Faery_Cooldown",&value,&r)==k::Status::complete&&r.hash_reads==1&&r.inserted==0,"existing scalar mutated map");++cases;
    const char embedded[]="Faery_Cooldown\0ignored";check(k::set(&state,embedded,7312,&r)==k::Status::complete&&map.size()==1&&k::get(&state,"Faery_Cooldown",&value,&r)==k::Status::complete&&value==7312,"first NUL/hash key changed");++cases;
    check(dh2_script_alias_hash("Alias16038")==dh2_script_alias_hash("Alias16275")&&
          k::set(&state,"Alias16038",-37,&r)==k::Status::complete&&
          k::get(&state,"Alias16275",&value,&r)==k::Status::complete&&value==-37&&r.inserted==0,
          "source hash collision incorrectly split dictionary keys");++cases;
    for(float n:{0.f,-0.f,1.75f,-1.75f,16777216.f,-2147483648.f,2147483520.f}){
        dh2_script_value a[3]{scalar_string("raw"),scalar_number(n),scalar_string("ignored")};k::Arguments args{a,3};
        check(k::set_callback(&state,&args,nullptr,&r)==k::Status::complete&&r.value==std::int32_t(n)&&!r.returned,"finite float scalar differs");++cases;
    }
    for(auto bits:{0x7fc00001u,0x7f800000u,0xff800000u,0x4f000000u,0xcf000001u}){
        dh2_script_value a[2]{scalar_string("unsupported"),scalar_number(float_word(bits))};k::Arguments args{a,2};const auto before=map;
        check(k::set_callback(&state,&args,nullptr,&r)==k::Status::unsupported_domain&&r.phase==k::Phase::number&&map==before,
              "unsupported signed conversion fabricated a map write");++cases;
    }
    for(bool is_set:{false,true})for(unsigned count=0;count<4;++count){
        dh2_script_value a[3]{scalar_string("arity"),scalar_number(23),scalar_string("ignored")};k::Arguments args{a,count};
        check((is_set?k::set_callback(&state,&args,nullptr,&r):k::get_callback(&state,&args,nullptr,&r))==k::Status::complete&&r.returned==(!is_set&&count?1u:0u),"source arity/ignored tail differs");++cases;
    }
    for(int phase:{1,2})for(bool throws:{false,true}){
        dh2_script_value a[2]{};a[0].type=DH2_SCRIPT_TABLE;a[1].type=DH2_SCRIPT_TABLE;k::Arguments args{a,2};Conversions c;c.failure=phase;c.throws=throws;
        k::Services services{&c,Conversions::string,Conversions::number};const auto before=map.size();
        check(k::set_callback(&state,&args,&services,&r)==k::Status::provider_failed&&c.strings==1&&c.numbers==unsigned(phase==2)&&map.size()==before,"coercion failure added dictionary effect");++failures;
    }
    {dh2_script_value a[2]{};a[0].type=DH2_SCRIPT_TABLE;a[1]=scalar_number(12);k::Arguments args{a,2};Conversions c;c.args=&args;c.state=&state;c.nest=true;c.alternative[1]=scalar_number(47);
     k::Services services{&c,Conversions::string,Conversions::number};check(k::set_callback(&state,&args,&services,&r)==k::Status::complete&&r.value==47&&map.at(dh2_script_alias_hash("nested"))==19,"fresh second Argument/nested effect lost");++cases;
     args={a,2};c.shrink=true;const auto before=map;check(k::set_callback(&state,&args,&services,&r)==k::Status::unsupported_domain&&r.phase==k::Phase::string&&map==before,"shrink assertion boundary continued");++failures;}
    {dh2_script_value a[2]{};a[0].type=DH2_SCRIPT_TABLE;k::Arguments args{a,2};Conversions c;c.args=&args;c.replacement=reinterpret_cast<const dh2_script_value*>(&r);
     k::Services services{&c,Conversions::string,Conversions::number};const auto before=map;
     check(k::set_callback(&state,&args,&services,&r)==k::Status::unsupported_domain&&r.phase==k::Phase::string&&map==before&&c.numbers==0,
           "fresh argument vector alias reached numeric conversion");++failures;}
    const auto original=r;auto guard=[&](k::Status status){check(status==k::Status::invalid_argument&&!std::memcmp(&r,&original,sizeof(r)),"invalid control mutated output");++guards;};
    guard(k::get(nullptr,"x",&value,&r));guard(k::get(&state,nullptr,&value,&r));guard(k::get(&state,"x",reinterpret_cast<std::int32_t*>(&r),&r));
    guard(k::set(&state,reinterpret_cast<const char*>(&r),1,&r));
    value=0;guard(k::get(&state,reinterpret_cast<const char*>(&value),&value,&r));
    guard(k::set(reinterpret_cast<const k::State*>(UINTPTR_MAX-7),"x",1,&r));guard(k::set(&state,"x",1,reinterpret_cast<k::Result*>(UINTPTR_MAX-7)));
    guard(k::set(&state,"x",1,reinterpret_cast<k::Result*>(reinterpret_cast<std::uintptr_t>(&r)+1)));k::Arguments bad{nullptr,1};guard(k::get_callback(&state,&bad,nullptr,&r));
    {k::Arguments good{nullptr,0};guard(k::get_callback(&state,&good,reinterpret_cast<const k::Services*>(UINTPTR_MAX-7),&r));}
    {k::Bindings b{&state,nullptr};char error[128];dh2_script_value v=scalar_string("x");std::uint32_t n=9;
     check(k::get_int(&b,&v,1,nullptr,0,&n,error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&n==9,"invalid native output performed prefix");++guards;
     check(k::get_int(&b,&v,1,&v,1,&n,error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&n==9,"native argument/output alias accepted");++guards;
     check(k::get_int(&b,&v,1,&v,1,reinterpret_cast<std::uint32_t*>(&state),error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE,"native returned/state alias accepted");++guards;}
    {k::Bindings b{&state,nullptr};char error[128];dh2_script_value v=scalar_string("x"),output{};std::uint32_t n=9;
     check(k::get_int(&b,&v,65537,&output,1,&n,error,sizeof(error))==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&n==9,"oversized native vector changed output");++guards;}
    Catalogue catalogue(argv[1]);Fixture fixture(argv[1],std::filesystem::path(argv[2])/"scalar",catalogue,"KnightPlayerBase");fixture.common();p::source::Result prepared{};
    check(fixture.owner->prepare(&prepared)==p::source::Status::complete,"unchanged Knight/faery preparation failed");
    k::IntegerMap owned;k::State retained{AIS,&owned};k::Bindings callback{&retained,nullptr};SpellProviders spell(fixture);
    auto* vm=fixture.session->vm();const auto paths=fixture.session->loaded_path_count();
    check(!dh2_script_vm_bind_source_values(vm,"GetInt",k::get_int,&callback)&&!dh2_script_vm_bind_source_values(vm,"SetInt",k::set_int,&callback)&&!dh2_script_vm_bind_source_values(vm,"GetCurrentSpellInfo",spell::current_spell_info_v1,&spell.bindings),"scalar source callbacks bind failed");
    for(const char* script:{"faerie_hotty","faerie_rocky","faerie_wetty","faerie_windy"}){
        std::uint32_t index=0;bool found=false;for(auto identity:fixture.owner->slots(p::source::List::faery)){const auto* instance=fixture.owner->instance(identity);if(instance&&std::string(instance->script_name)==script){found=true;break;}++index;}
        check(found,"actual faery script slot missing");Return selected,updated;std::string name=script;dh2_script_value slot=scalar_number(float(index));
        check(!fixture.call("SetSkill",{fixture.string(name),slot},selected)&&!fixture.call("OnSkillUpdate",{},updated)&&updated.count==0,"unchanged real faery update failed");++actual;
    }
    check(owned.size()==1&&owned.at(dh2_script_alias_hash("Faery_Cooldown"))==0,"authored faery GetInt did not persist miss");++cases;
    fixture.overlay("fixture/scalar","function scalar_roundtrip() SetInt('Faery_Cooldown',7312.75,'ignored'); return GetInt('Faery_Cooldown',{}) end\nfunction scalar_required() return pcall(function() return GetInt({}) end) end");s::LoadResult loaded{};check(!fixture.load("fixture/scalar",loaded)&&loaded.source_success,"scalar Lua fixture load failed");
    Return returned;check(!fixture.call("scalar_roundtrip",{},returned)&&returned.number==7312&&owned.size()==1,"same Lua scalar roundtrip lost backing");++lua;
    const auto epoch=dh2_script_vm_required_failure_epoch(vm);Return rejected;check(fixture.call("scalar_required",{},rejected)==-5&&rejected.observed==1&&!rejected.boolean&&dh2_script_vm_required_failure_epoch(vm)==epoch+1,"pcall hid missing scalar coercion");++lua;
    check(!fixture.call("scalar_roundtrip",{},returned)&&fixture.session->vm()==vm&&fixture.session->loaded_path_count()==paths+1&&owned.at(dh2_script_alias_hash("Faery_Cooldown"))==7312,"retained same VM/map recovery failed");++lua;
    std::string celest="faerie_celest";Return selected,blocked;check(!fixture.call("SetSkill",{fixture.string(celest),scalar_number(0)},selected),"real Celest selection failed");
    check(fixture.call("OnSkillUpdate",{},blocked)==-5&&fixture.session->last_error().find("GetEquippedFaeryElement")!=std::string::npos,"missing actual faery-element/buff provider fabricated success");++failures;++actual;
    fixture.session.reset();fixture.owner.reset(); // Close before dictionary/save/provider retirement.
    std::cout<<"{\"validation\":\"PASS\",\"dictionary_cases\":"<<cases<<",\"guards\":"<<guards<<",\"failure_cases\":"<<failures<<",\"real_lua_cases\":"<<lua<<",\"actual_faery_cases\":"<<actual<<",\"actual_faery_updates_completed\":4,\"actual_faery_updates_blocked\":1,\"same_vm\":true,\"native_wired\":false,\"complete_13_callbacks\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
