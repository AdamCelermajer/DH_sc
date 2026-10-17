#include "../character_dead_focus_services_v1.hpp"
#include "../character_coordinator.hpp"
#include "../../game-data/ai.hpp"
#include <iomanip>
#define main retained_session_fixture_main
#include "player_skill_session_v1.cpp"
#undef main

namespace dead=dh2::character_dead_focus_services_v1;
namespace buff=dh2::character_player_buffs_v1;
namespace ch=dh2::character;
struct Borrowed {
    Fixture& f;ch::Coordinator coordinator{CHAR,2};std::vector<d::ClassRow> rows;
    d::AiTables ai;
    std::unique_ptr<buff::Owner> buffs;std::uint8_t byte415=1;
    std::uintptr_t self=0,state=0,highlight=0;unsigned players=0,drops=0;
    bool fail_player=false,fail_drop=false,mutate_drop=false,reenter=false;
    std::vector<buff::Operation> operations;std::vector<ch::Service> focus;
    dead::Adapter* active=nullptr;dead::Bindings bindings;
    explicit Borrowed(Fixture& fixture):f(fixture){
        coordinator.bind({this,[](void*){ch::Facts facts;facts.is_player=1;return facts;},
            {this,state_call},[](void*,ch::Coordinator&,int,ch::Timer32&,std::uint32_t){throw std::runtime_error("unreached expiry fixture");},nullptr});
        coordinator.state.current=3;
        const char* names[]={"ai_pyarray.bin","ai_pyarraynames.bin","ai_pystructnames.bin","ai_factions_pyarray.bin","ai_factions_pyarraynames.bin","ai_factions_pystructnames.bin"};
        std::vector<std::vector<std::uint8_t>> inputs;for(auto name:names)inputs.push_back(read(f.cache/"data/pydata"/name));std::string error;
        check(d::load_ai(bytes(inputs[0]),bytes(inputs[1]),bytes(inputs[2]),bytes(inputs[3]),bytes(inputs[4]),bytes(inputs[5]),ai,error),error.c_str());
        for(const auto& r:f.catalogue.classes.rows)rows.push_back({r.data(),std::uint32_t(r.size())});
        buffs=buff::Owner::create({CHAR,&f.view,{this,invoke},std::uint32_t(rows.size()),12});check(bool(buffs),"buff owner create");
        bindings.character=CHAR;bindings.properties=&f.view;bindings.buffs=buffs.get();bindings.byte415=&byte415;
        bindings.self_fx=&self;bindings.state_fx=&state;bindings.highlight=&highlight;
        bindings.debug_globals=&f.debug.globals();bindings.debug_services=&f.debug.services();
        bindings.skills=&f.catalogue.tables->skills();bindings.preparation=f.owner.get();bindings.services={this,is_player,nullptr};
    }
    ~Borrowed(){coordinator.stop_timers();buff::Result r{};buffs->retire(&r);}
    static int invoke(void* raw,d::PropertyView* view,const buff::Request* q,buff::Response* out){
        auto& b=*static_cast<Borrowed*>(raw);check(view==&b.f.view&&q->character==CHAR,"second property or Character owner");b.operations.push_back(q->operation);
        switch(q->operation){
        case buff::Operation::timer_start:out->word=b.coordinator.start_timer(q->duration,q->repeat,q->event,q->subject);return out->word< -1?-1:0;
        case buff::Operation::timer_stop:return b.coordinator.stop_timer(std::uint32_t(q->id))<0?-1:0;
        case buff::Operation::timer_time_left:return dh2_character_timer_time_left(&out->elapsed,&out->duration,&b.coordinator.timers(),std::uint32_t(q->id))==1?0:-1;
        case buff::Operation::apply_class:return int(dh2_class_apply(b.rows.data(),std::uint32_t(b.rows.size()),q->id,q->sheet,view->resolved));
        case buff::Operation::recalculate:return int(dh2_class_recalc_base(b.rows.data(),std::uint32_t(b.rows.size()),b.f.properties.base.data(),view));
        case buff::Operation::fx_release:return q->subject?-1:0;
        default:return -1; // Positive FX requires its actual owner.
        }
    }
    static int is_player(void* raw,std::uintptr_t character,std::uint32_t* result,std::string&){
        auto& b=*static_cast<Borrowed*>(raw);check(character==CHAR,"lost full Character identity");++b.players;
        if(b.reenter){dead::Result r{};r.calls=99;std::string e="sentinel";check(b.active->deliver(ch::remove_buffs,&r,e)==dead::Status::busy&&r.calls==99&&e=="sentinel","same adapter reentry altered output");}
        if(b.fail_player)return -1;
        const auto* row=d::ai_props(b.ai,b.f.view.resolved[1]);check(row&&row->type==1,"actual retained Player AI type1 branch");
        *result=1;return 0; // The source type1 IsPlayer branch needs no name producer.
    }
    static int drop(void* raw,std::uintptr_t& handle,std::string&){
        auto& b=*static_cast<Borrowed*>(raw);++b.drops;if(b.mutate_drop)handle=0;
        if(b.fail_drop)return -1;
        handle=0;return 0; // Declared test callee, not native FX activation.
    }
    static void state_call(void* raw,ch::State* state,const ch::Request* q){
        auto& b=*static_cast<Borrowed*>(raw);auto service=static_cast<ch::Service>(q->service);b.focus.push_back(service);
        if(dead::Adapter::handles(service)){dead::Result r{};std::string error;check(b.active->deliver(service,&r,error)==dead::Status::complete,error.c_str());return;}
        // These reached state dependencies are declared fixtures; actual native
        // animation/controller/body/Character-event owners are a separate gate.
        if(service==ch::set_animation)state->current_animation=q->argument[0];
        else check(service==ch::look_at||service==ch::stop_loop||service==ch::raise_event,"unexpected dead source callee");
    }
};
int main(int argc,char** argv){try{
    check(argc==3,"cache and real Debug directory required");Catalogue catalogue(argv[1]);unsigned normal=0,failures=0,guards=0,positive=0;std::set<std::string> resources;
    for(const char* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
        Fixture f(argv[1],std::filesystem::path(argv[2])/name,catalogue,name);Borrowed b(f);dead::Adapter adapter(b.bindings);b.active=&adapter;dead::Result r{};std::string error;
        check(adapter.focus_prelude(&r,error)==dead::Status::complete&&r.calls==4,"actual two Debug load/query pairs");
        check(f.debug.counters().read_opens==1&&f.debug.counters().read_closes==1&&f.debug.runtime().switches().count("isTracingCharState")&&f.debug.runtime().switches().count("isTracingCSDead"),"actual Debug owner/map/file effects");
        check(adapter.focus_prelude(&r,error)==dead::Status::complete&&f.debug.counters().read_opens==1,"Debug guard replayed filesystem");++normal;
        for(auto service:{ch::remove_highlight,ch::disable_state_fx,ch::disable_self_fx})
            check(adapter.deliver(service,&r,error)==dead::Status::complete&&!r.calls&&!b.drops,"source null FX branch called provider");
        ++normal;
        buff::Result added{};check(b.buffs->add(146,100,1,1,-1,"source key",&added)==buff::Status::complete&&added.instance,"actual key146 buff producer");b.byte415=0;
        check(adapter.deliver(ch::cancel_sneaking,&r,error)==dead::Status::complete&&r.player==1&&b.byte415==1&&!b.buffs->count()&&!b.coordinator.timers().slots[0].active,"DelBuff146 before byte415");++normal;
        b.operations.clear();check(adapter.deliver(ch::remove_buffs,&r,error)==dead::Status::complete&&b.operations==std::vector<buff::Operation>{buff::Operation::recalculate},"empty RemoveAllBuffs must recalc");++normal;
        b.fail_player=true;b.byte415=0;check(adapter.deliver(ch::cancel_sneaking,&r,error)==dead::Status::provider_failed&&r.phase==dead::Phase::is_player&&!b.byte415,"IsPlayer failure invented prefix");b.fail_player=false;++failures;
        b.highlight=0x900000001ull;check(adapter.deliver(ch::remove_highlight,&r,error)==dead::Status::provider_failed&&r.phase==dead::Phase::drop_fx&&b.highlight==0x900000001ull&&!b.drops,"nonnull missing FX silently succeeded");++failures;
        auto with_fx=b.bindings;with_fx.services.drop_fx=Borrowed::drop;dead::Adapter fx(with_fx);b.fail_drop=true;b.mutate_drop=true;
        check(fx.deliver(ch::remove_highlight,&r,error)==dead::Status::provider_failed&&!b.highlight&&r.calls==1,"FX provider failure rolled back prefix");b.fail_drop=false;b.mutate_drop=false;b.state=0x900000002ull;
        check(fx.deliver(ch::disable_state_fx,&r,error)==dead::Status::complete&&!b.state,"declared nonnull FX callee");++failures;++normal;
        error="sentinel";auto before=f.properties;check(adapter.deliver(ch::remove_buffs,reinterpret_cast<dead::Result*>(f.view.resolved),error)==dead::Status::invalid_argument&&error=="sentinel"&&!std::memcmp(&before,&f.properties,sizeof(before)),"result aliases live property sheets");++guards;
        auto invalid=b.bindings;invalid.character=CHAR+1;dead::Adapter detached(invalid);r.calls=99;check(detached.deliver(ch::remove_buffs,&r,error)==dead::Status::invalid_argument&&r.calls==99&&error=="sentinel","Character mismatch mutated output");++guards;
        b.reenter=true;check(adapter.deliver(ch::cancel_sneaking,&r,error)==dead::Status::complete,"guarded synchronous IsPlayer");b.reenter=false;++guards;
        auto missing=b.bindings;missing.debug_globals=nullptr;dead::Adapter no_debug(missing);check(no_debug.focus_prelude(&r,error)==dead::Status::provider_failed&&r.phase==dead::Phase::debug_load,"missing Debug silently accepted");++failures;
        ch::State state;state.current=3;state.flags=0x2380;state.animation_override=259;ch::Facts facts;facts.is_player=1;ch::Services state_services{&b,Borrowed::state_call};b.focus.clear();
        check(dh2_character_state_transition(&state,&facts,12,0xc358,0,&state_services)==1&&state.flags==0x2241&&state.controller_locked&&state.current_animation==259,"dead state composition");
        const std::vector<ch::Service> order{ch::dead_focus_prelude,ch::look_at,ch::remove_highlight,ch::set_animation,ch::stop_loop,ch::cancel_sneaking,ch::disable_state_fx,ch::disable_self_fx,ch::remove_buffs,ch::raise_event,ch::raise_event,ch::raise_event,ch::raise_event};
        check(b.focus==order,"dead focus exact service order");++normal;
        f.common();p::source::Result prepared{};check(f.owner->prepare(&prepared)==p::source::Status::complete,"source skill preparation");
        dh2::player_skill_use_session_v1::Runtime calls(*f.session,*f.owner,CHAR);auto ready=b.bindings;ready.skill_calls=&calls;dead::Adapter skill(ready);
        // Inject a positive cached read fact solely to reach the missing-provider
        // boundary. This is not a native Sneaking producer or SetProp claim.
        f.view.resolved[198]=256;auto absent=ready;absent.skills=nullptr;dead::Adapter no_tables(absent);b.byte415=0;
        check(no_tables.deliver(ch::cancel_sneaking,&r,error)==dead::Status::provider_failed&&r.phase==dead::Phase::list&&b.byte415==1&&f.view.resolved[198]==256,"positive Sneak boundary lost reached prefix");++failures;
        const auto vm=f.session->vm();const auto selected=catalogue.tables->skill_list(f.view.resolved[28]);check(selected,"real SkillList missing");
        bool flagged=false;for(auto id:selected->members)flagged=flagged||bool(std::uint32_t(catalogue.tables->skills().skills.at(id).flags)&0x02000000u);
        const auto status=skill.deliver(ch::cancel_sneaking,&r,error);check(f.session->vm()==vm,"CancelSneaking replaced VM");
        if(!flagged)check(status==dead::Status::complete&&r.selected_slot==-1,"no flagged skill invented cancellation");
        else check(r.selected_slot>=0&&(status==dead::Status::complete||((r.phase==dead::Phase::active||r.phase==dead::Phase::pre)&&status==dead::Status::provider_failed)),"actual flagged skill callback boundary");
        ++positive;resources.insert(f.loaded_paths.begin(),f.loaded_paths.end());f.session.reset();
    }
    std::cout<<"{\"validation\":\"PASS\",\"actual_class_debug_buff_cases\":"<<normal<<",\"failure_prefixes\":"<<failures<<",\"ownership_guards\":"<<guards<<",\"actual_skill_selection_cases\":"<<positive<<",\"live_gameplay\":false,\"native_fx_activation\":false,\"resolved_cache_resources\":[";
    bool first=true;for(const auto& path:resources){if(!first)std::cout<<',';first=false;std::cout<<std::quoted(path);}std::cout<<"]}\n";return 0;
}catch(const std::exception& e){std::cerr<<"dead focus: "<<e.what()<<'\n';return 1;}}
