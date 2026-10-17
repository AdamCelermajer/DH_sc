#define main legacy_monster_session_main
#include "monster_external_script_session.cpp"
#undef main
#include <array>
#include <cassert>
#include <cstring>

namespace {
struct UpdateFixture {
    Session* session=nullptr;
    CurrentState* projection=nullptr;
    const StateCallbacks* replacement=nullptr;
    StateCallbacks* rename_state=nullptr;
    std::vector<std::string> trace;
    unsigned loads=0;
    bool change_state=false,fail_query=false,throw_query=false,reenter=false;
    std::array<Status,6> nested{};
};
std::int32_t update_structure(void*,const char* category,const char* member,std::int32_t* value) {
    require(std::string(category)=="CharacterProperties"&&std::string(member)=="SkillTree","load field differs");*value=28;return 0;
}
std::int32_t update_property(void*,std::uintptr_t identity,std::int32_t key,float* value) {
    require(identity==owner&&key==28,"load owner/property differs");*value=-256;return 0;
}
std::int32_t update_target(void* raw,std::uintptr_t identity,std::uint32_t* value) {
    require(identity==owner,"update owner differs");auto& f=*static_cast<UpdateFixture*>(raw);f.trace.emplace_back("HasTarget");*value=0;
    if(f.change_state){require(f.projection!=nullptr,"mutation projection missing");f.projection->active=f.replacement;}
    if(f.rename_state)f.rename_state->conditions="ArmStateReplacement";
    if(f.reenter){
        std::string error;LoadResult result{};const std::string code="assert(false)";
        f.nested={f.session->dispatch(Event::update,0,error),f.session->call_state_update(f.projection,error),
            f.session->call_state_conditions(f.projection,error),f.session->reset(error),
            f.session->load_resolved("data/scripts/ai/nested.luac",source(code),&result,error),
            f.session->bind_functions(error)};
    }
    if(f.throw_query)throw std::runtime_error("query error after effect");
    return f.fail_query?1:0;
}
std::int32_t update_state(void* raw,std::uintptr_t identity,std::int32_t* value) {
    require(identity==owner,"conditions owner differs");static_cast<UpdateFixture*>(raw)->trace.emplace_back("GetState");*value=3;return 0;
}
Services update_services(UpdateFixture& f) {
    Services s{};s.context=&f;s.owner=owner;s.get_py_struct=update_structure;s.get_prop=update_property;
    s.has_target=update_target;s.get_state=update_state;return s;
}
void update_initialize(Session& session,UpdateFixture& f,Source commons,const std::string& monster) {
    f.session=&session;std::string error;require(session.initialize(commons,source(monster),update_services(f),error)==Status::complete,error.c_str());f.trace.clear();
}
const char update_fixture[]=R"lua(
function ArmStateUpdate(...)
    assert(select('#',...)==0)
    HasTarget()
end
function ArmStateConditions(...)
    assert(select('#',...)==0)
    GetState()
end
function ArmStateReplacement(...)
    assert(select('#',...)==0)
    HasTarget()
end
AddToVFTable('UpdateKey','ArmStateUpdate')
AddToVFTable('ConditionKey','ArmStateConditions')
)lua";
void print_oracle(const UpdateFixture& f,const Session& s,Status status) {
    std::printf("{\"status\":%d,\"calls\":%u,\"trace\":[",int(status),s.statistics().completed_callbacks);
    for(unsigned i=0;i<f.trace.size();++i)std::printf("%s\"%s\"",i?",":"",f.trace[i].c_str());
    std::printf("]}\n");
}
}

int main(int argc,char** argv) {
    require(argc==3||argc==8,"expected original script paths or oracle arguments");
    const auto common_bytes=read(argv[1]),monster_bytes=read(argv[2]);const auto commons=source(common_bytes);
    const std::string with_fixture=monster_bytes+update_fixture;
    if(argc==8){
        require(std::string(argv[3])=="--oracle","oracle flag missing");const unsigned kind=unsigned(std::stoul(argv[4])),present=unsigned(std::stoul(argv[5])),alias=unsigned(std::stoul(argv[6])),mutation=unsigned(std::stoul(argv[7]));
        Session s;UpdateFixture f;update_initialize(s,f,commons,with_fixture);std::string error;
        StateCallbacks a{0x1111,alias?"UpdateKey":"ArmStateUpdate",alias?"ConditionKey":"ArmStateConditions",nullptr,nullptr},b{0x2222,"ArmStateUpdate","ArmStateReplacement",nullptr,nullptr};
        CurrentState state{0x3333,present?&a:nullptr};f.projection=&state;f.change_state=mutation==1||mutation==2;f.replacement=mutation==1?&b:nullptr;f.rename_state=mutation==3?&a:nullptr;
        Status status=kind==1?s.call_state_conditions(&state,error):s.call_state_update(&state,error);
        if(kind==2&&status==Status::complete)status=s.call_state_conditions(&state,error);
        print_oracle(f,s,status);return 0;
    }
    // Replay the unchanged legacy shared fixture against this extended source.
    require(legacy_monster_session_main(argc,argv)==0,"legacy session regression");
    unsigned cases=0,guards=0,failures=0,cache_cases=0;
    std::string error;
    {
        Session s;UpdateFixture f;update_initialize(s,f,commons,monster_bytes);bool present=true;
        require(s.contains_source_alias("OnUpdate",present)&&!present&&std::string(s.source_alias(Event::update))=="OnUpdate","plain monster unexpectedly overrides OnUpdate");
        require(s.dispatch(Event::update,0,error)==Status::complete&&f.trace.empty()&&s.statistics().completed_callbacks==1,"unchanged common OnUpdate failed");++cases;
        CurrentState state{0x3333,nullptr};require(s.call_state_update(&state,error)==Status::complete&&s.call_state_conditions(&state,error)==Status::complete&&s.statistics().completed_callbacks==1,"actual null-state wrapper called Lua");++cases;
        require(s.call_state_update(nullptr,error)==Status::invalid_argument&&s.ready(),"null control accepted");++guards;
        alignas(16)std::array<unsigned char,64> bytes{};auto* bad=bytes.data()+1;
        require(s.call_state_conditions(reinterpret_cast<const CurrentState*>(bad),error)==Status::invalid_argument,"misaligned control accepted");++guards;
        state.active=reinterpret_cast<const StateCallbacks*>(bad);require(s.call_state_update(&state,error)==Status::invalid_argument,"misaligned state accepted");++guards;
        StateCallbacks names{0x1111,nullptr,nullptr,nullptr,nullptr};state.active=&names;
        require(s.call_state_update(&state,error)==Status::invalid_source_fact&&s.call_state_conditions(&state,error)==Status::invalid_source_fact&&s.ready()&&s.statistics().failed_callbacks==0,"null names invented source call");guards+=2;
        state.ais=0;require(s.call_state_update(&state,error)==Status::invalid_argument&&s.ready(),"missing original AIS identity accepted");++guards;
    }
    {
        const std::string extension=monster_bytes+R"lua(
function UpdateOne(timestamp,...)
    assert(timestamp==nil and select('#',...)==0)
    HasTarget()
    return setmetatable({}, {__index=function(_,key)
        assert(key=='_this'); AddToVFTable('OnUpdate','UpdateTwo'); return nil
    end})
end
function UpdateTwo(...) assert(select('#',...)==0); GetState() end
AddToVFTable('OnUpdate','UpdateOne')
)lua";
        Session s;UpdateFixture f;update_initialize(s,f,commons,extension);
        require(s.dispatch(Event::update,0,error)==Status::complete&&std::string(s.source_alias(Event::update))=="UpdateTwo"&&s.dispatch(Event::update,0,error)==Status::complete&&f.trace==std::vector<std::string>{"HasTarget","GetState"},"zero-arg callback/fresh discarded-return alias failed");++cases;
    }
    for(const std::string& extension:{std::string("OnUpdate=nil"),std::string("AddToVFTable('OnUpdate','MissingUpdate')"),std::string("function OnUpdate() HasTarget(); GetPosition() end")}){
        Session s;UpdateFixture f;update_initialize(s,f,commons,monster_bytes+extension);
        require(s.dispatch(Event::update,0,error)==Status::script_error&&!s.ready()&&s.statistics().failed_callbacks==1&&!error.empty(),"missing update/provider fabricated success");
        if(extension.find("GetPosition")!=std::string::npos)require(f.trace==std::vector<std::string>{"HasTarget"},"error lost source prefix");
        ++failures;
    }
    for(unsigned mutation=0;mutation<3;++mutation){
        Session s;UpdateFixture f;update_initialize(s,f,commons,with_fixture);StateCallbacks a{0x1111,"UpdateKey","ConditionKey",nullptr,nullptr},b{0x2222,"ArmStateUpdate","ArmStateReplacement",nullptr,nullptr};CurrentState state{0x3333,&a};
        f.projection=&state;f.change_state=mutation!=0;f.replacement=mutation==1?&b:nullptr;
        require(s.call_state_update(&state,error)==Status::complete&&s.call_state_conditions(&state,error)==Status::complete,"state wrapper dispatch failed");
        const auto expected=mutation==0?std::vector<std::string>{"HasTarget","GetState"}:mutation==1?std::vector<std::string>{"HasTarget","HasTarget"}:std::vector<std::string>{"HasTarget"};
        require(f.trace==expected&&s.statistics().completed_callbacks==(mutation==2?1u:2u),"state pointer was cached across wrappers");++cases;
    }
    {
        Session s;UpdateFixture f;update_initialize(s,f,commons,with_fixture);StateCallbacks names{0x1111,"ArmStateUpdate","ArmStateConditions",nullptr,nullptr};CurrentState state{0x3333,&names};f.rename_state=&names;
        require(s.call_state_update(&state,error)==Status::complete&&s.call_state_conditions(&state,error)==Status::complete&&f.trace==std::vector<std::string>{"HasTarget","HasTarget"},"condition name was cached on unchanged state pointer");++cases;
    }
    {
        Session s;UpdateFixture f;update_initialize(s,f,commons,with_fixture);StateCallbacks state_names{0x1111,"ArmStateUpdate","ArmStateConditions",nullptr,nullptr};CurrentState state{0x3333,nullptr};
        f.projection=&state;f.change_state=true;f.replacement=&state_names;
        const std::string extension="function RealUpdate() HasTarget() end; AddToVFTable('OnUpdate','RealUpdate')";LoadResult load{};
        require(s.load_resolved("data/scripts/ai/audit_update.luac",source(extension),&load,error)==Status::complete&&s.dispatch(Event::update,0,error)==Status::complete&&s.call_state_conditions(&state,error)==Status::complete&&f.trace==std::vector<std::string>{"HasTarget","GetState"},"fresh state after OnUpdate lost");++cases;
    }
    for(unsigned kind=0;kind<2;++kind){
        Session s;UpdateFixture f;update_initialize(s,f,commons,with_fixture);StateCallbacks names{0x1111,kind?"ArmStateUpdate":"MissingState","MissingState",nullptr,nullptr};CurrentState state{0x3333,&names};
        if(kind)require(s.call_state_update(&state,error)==Status::complete,"successful state prefix lost");
        const auto status=kind?s.call_state_conditions(&state,error):s.call_state_update(&state,error);
        require(status==Status::script_error&&!s.ready()&&s.statistics().failed_callbacks==1&&f.trace.size()==kind,"missing state function fell back");++failures;
    }
    for(unsigned throwing=0;throwing<2;++throwing){
        Session s;UpdateFixture f;update_initialize(s,f,commons,with_fixture);StateCallbacks names{0x1111,"ArmStateUpdate","ArmStateConditions",nullptr,nullptr};CurrentState state{0x3333,&names};f.fail_query=throwing==0;f.throw_query=throwing!=0;
        require(s.call_state_update(&state,error)==Status::script_error&&f.trace==std::vector<std::string>{"HasTarget"}&&s.statistics().failed_callbacks==1,"provider failure lost effects or fabricated success");++failures;
    }
    {
        Session s;UpdateFixture f;update_initialize(s,f,commons,with_fixture);StateCallbacks names{0x1111,"ArmStateUpdate","ArmStateConditions",nullptr,nullptr};CurrentState state{0x3333,&names};f.projection=&state;f.reenter=true;
        require(s.call_state_update(&state,error)==Status::complete&&s.ready(),"outer reentry call failed");for(auto status:f.nested)require(status==Status::busy,"same-VM recursive operation accepted");++cases;
    }
    {
        Session s;UpdateFixture f;f.session=&s;LoadResult load{};const std::string text="loads=1";
        require(s.load_resolved("audit.luac",source(text),&load,error)==Status::not_ready&&s.create(update_services(f),error)==Status::complete&&s.load_resolved("audit.luac",source(text),&load,error)==Status::not_ready,"unbound resolved load executed");++guards;
        require(s.bind_functions(error)==Status::complete&&s.load_resolved("audit.luac",source(text),&load,error)==Status::complete&&load.load_called==1&&s.stage()==Stage::functions_bound,"bound staged real load failed");++cache_cases;
        require(s.load_common(commons,error)==Status::complete&&s.load_external(source(monster_bytes),error)==Status::complete,"legacy staged order changed");++cases;
    }
    {
        Session s;UpdateFixture f;update_initialize(s,f,commons,monster_bytes);LoadResult load{};
        const std::string text="loads=(loads or 0)+1; function CachedUpdate(...) assert(select('#',...)==0 and loads==1); HasTarget() end; AddToVFTable('OnUpdate','CachedUpdate')";
        require(s.load_resolved("data/scripts/ai/cached.luac",source(text),&load,error)==Status::complete&&load.load_called==1&&!load.cache_hit,"same VM cache miss not loaded");++cache_cases;
        require(s.load_resolved("data/scripts/ai/cached.luac",{nullptr,0},&load,error)==Status::complete&&load.cache_hit&&load.source_success&&!load.load_called,"exact cache hit touched absent bytes");++cache_cases;
        const std::string wrong="error('duplicate ran')";require(s.load_resolved("data/scripts/ai/cached.luac",source(wrong),&load,error)==Status::complete&&load.cache_hit&&s.dispatch(Event::update,0,error)==Status::complete&&f.trace==std::vector<std::string>{"HasTarget"},"cache hit replayed script/global registry");++cache_cases;
        require(s.initialize(commons,source(monster_bytes),update_services(f),error)==Status::complete&&s.load_resolved("data/scripts/ai/cached.luac",{nullptr,0},&load,error)==Status::invalid_argument&&!load.cache_hit&&!load.load_called,"replacement VM inherited old cache keys");++cache_cases;
        require(s.load_resolved("data/scripts/ai/cached.luac",source(text),&load,error)==Status::complete&&load.load_called&&s.dispatch(Event::update,0,error)==Status::complete,"replacement VM did not rerun actual source");++cache_cases;
    }
    {
        Session s;UpdateFixture f;update_initialize(s,f,commons,monster_bytes);LoadResult load{};
        const std::string count="loads=(loads or 0)+1";for(const char* path:{"Exact.luac","exact.luac","./Exact.luac"}){require(s.load_resolved(path,source(count),&load,error)==Status::complete&&load.load_called&&!load.cache_hit,"resolved path was normalized");++cache_cases;}
        const std::string proof="function OnUpdate() assert(loads==3) end";require(s.load_resolved("proof.luac",source(proof),&load,error)==Status::complete&&s.dispatch(Event::update,0,error)==Status::complete,"distinct exact paths did not execute");++cache_cases;
    }
    for(unsigned mode=0;mode<3;++mode){
        Session s;UpdateFixture f;update_initialize(s,f,commons,monster_bytes);LoadResult load{};
        const std::string broken=mode==0?"function bad(":mode==1?"partial=7; error('runtime error')":"HasTarget()";f.fail_query=mode==2;
        require(s.load_resolved("retry.luac",source(broken),&load,error)==Status::script_error&&load.load_called&&!load.source_success&&s.ready()&&!error.empty(),"failed miss poisoned VM or cache");++cache_cases;
        f.fail_query=false;const std::string retry=mode==1?"assert(partial==7); function OnUpdate() GetState() end":"function OnUpdate() GetState() end";
        require(s.load_resolved("retry.luac",source(retry),&load,error)==Status::complete&&load.load_called&&!load.cache_hit&&s.dispatch(Event::update,0,error)==Status::complete&&f.trace.back()=="GetState","failed path retry lost effects or hit prematurely");++cache_cases;
        require(s.load_resolved("retry.luac",{nullptr,0},&load,error)==Status::complete&&load.cache_hit,"successful retry was not cached");++cache_cases;
    }
    {
        Session s;UpdateFixture f;update_initialize(s,f,commons,monster_bytes);LoadResult load{};const std::string code="assert(false)";
        require(s.load_resolved(nullptr,source(code),&load,error)==Status::invalid_argument&&s.load_resolved("",source(code),&load,error)==Status::invalid_argument&&s.load_resolved("valid",source(code),nullptr,error)==Status::invalid_argument&&s.ready(),"bad load controls were accepted");guards+=3;
    }
    std::printf("{\"validation\":\"PASS\",\"update_cases\":%u,\"failure_cases\":%u,\"guard_cases\":%u,\"resolved_load_cases\":%u,"
        "\"zero_argument_update\":true,\"independent_live_state_reads\":true,\"fresh_alias_and_discarded_return_effects\":true,"
        "\"actual_same_vm_load_cache\":true,\"load_failure_prefix_and_retry\":true,\"native_wired\":false,\"mismatches\":0}\n",cases,failures,guards,cache_cases);
}
