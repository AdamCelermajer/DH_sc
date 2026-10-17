#define main legacy_monster_session_main
#include "monster_external_script_session.cpp"
#undef main
#include <array>
#include <memory>

namespace {
using Hold = std::shared_ptr<void>;
struct Retirement {
    Session* session;
    std::string* error;
    Services next;
    Hold next_lifetime;
    std::uintptr_t vm;
    unsigned* releases;
    bool* committed;
    bool expect_empty;
    std::array<Status,5>* nested;
};
void retire(Retirement* p) {
    ++*p->releases;
    *p->committed = p->expect_empty ?
        p->session->vm_identity()==0 && p->session->stage()==Stage::empty :
        p->session->uses_services(p->next) && p->session->vm_identity()==p->vm &&
        p->session->stage()==Stage::created && p->next_lifetime.use_count()>=2;
    // Use the caller's error string deliberately: retirement must run while
    // busy, and a successful outer operation clears these nested errors.
    *p->nested = {p->session->reset(*p->error),
        p->session->create(p->next,*p->error),
        p->session->install_created_services(p->next,*p->error,p->next_lifetime),
        p->session->bind_functions(*p->error),
        p->session->dispatch(Event::update,0,*p->error)};
    delete p;
}
bool all_busy(const std::array<Status,5>& nested) {
    for(auto status:nested) if(status!=Status::busy) return false;
    return true;
}
void stage_load(Session& vm,Source commons,Source monster,std::string& error) {
    require(vm.bind_ais_functions(error)==Status::complete &&
        vm.bind_character_functions(error)==Status::complete &&
        vm.load_common(commons,error)==Status::complete &&
        vm.load_external(monster,error)==Status::complete,"bridge staged load failed");
}
}

int main(int argc,char** argv) {
    try {
        require(argc==3,"pass unchanged original commons and monster paths");
        const auto common_bytes=read(argv[1]),monster_bytes=read(argv[2]);
        const auto commons=source(common_bytes),monster=source(monster_bytes);
        unsigned cases=0,guards=0,retirement_cases=0;
        std::string error;
        {
            Session vm;Fixture constructor;
            auto callbacks=std::make_shared<Fixture>();auto services=bind(*callbacks);
            std::weak_ptr<Fixture> retained=callbacks;
            require(vm.create(bind(constructor),error)==Status::complete && vm.vm_identity()!=0 &&
                vm.stage()==Stage::created && !vm.ready() && constructor.trace.empty() &&
                vm.statistics().source_libraries_opened==0 && vm.statistics().source_functions_bound==0,
                "constructor VM performed premature binding or Lua loading");++cases;
            const auto identity=vm.vm_identity();
            require(vm.install_created_services(services,error,callbacks)==Status::complete && error.empty() &&
                vm.vm_identity()==identity && vm.stage()==Stage::created && vm.uses_services(services) &&
                callbacks->trace.empty(),"installation replaced VM or ran callbacks");++cases;
            callbacks.reset();require(!retained.expired(),"VM did not retain prepared callbacks");++cases;
            stage_load(vm,commons,monster,error);
            auto live=retained.lock();require(live && live->trace==std::vector<std::string>{"Struct","Prop"} &&
                constructor.trace.empty() && vm.vm_identity()==identity &&
                vm.statistics().source_libraries_opened==4 && vm.statistics().source_functions_bound==35,
                "binding/load did not use same VM and new service context");++cases;
            live->trace.clear();
            require(vm.dispatch(Event::enemy_spotted,enemy,error)==Status::complete &&
                live->target==enemy && live->last_face==enemy && vm.vm_identity()==identity,
                "original Lua dispatch lost 64-bit identities or controller context");
            trace(*live,{"HasTarget","SetTarget","HeadTo"});++cases;
            const auto before=vm.statistics();const char* alias=vm.source_alias(Event::enemy_spotted);
            auto rejected=std::make_shared<Fixture>();
            require(vm.install_created_services(bind(*rejected),error,rejected)==Status::not_ready &&
                vm.uses_services(services) && vm.vm_identity()==identity &&
                vm.source_alias(Event::enemy_spotted)==alias &&
                vm.statistics().completed_callbacks==before.completed_callbacks &&
                vm.statistics().lua_memory_used==before.lua_memory_used && rejected->trace.empty(),
                "late installation disturbed initialized Lua state");++guards;
            // Same resolved key still hits after rejection; duplicate execution
            // would enter HasTarget and trip the Lua assertion.
            const std::string sentinel="assert(BridgeLoaded==nil); BridgeLoaded=true; HasTarget()";
            LoadResult loaded{};require(vm.load_resolved("bridge-sentinel",source(sentinel),&loaded,error)==Status::complete &&
                !loaded.cache_hit,"bridge sentinel did not load");
            require(vm.install_created_services(bind(*rejected),error,rejected)==Status::not_ready &&
                vm.load_resolved("bridge-sentinel",source(sentinel),&loaded,error)==Status::complete &&
                loaded.cache_hit && vm.vm_identity()==identity,"rejection reset resolved-path cache");++guards;
            live.reset();require(vm.reset(error)==Status::complete && vm.vm_identity()==0 && retained.expired(),
                "closing VM leaked prepared callback context");++cases;
        }
        {
            Session vm;Fixture original,new_fixture;
            const auto services=bind(new_fixture);auto hold=std::make_shared<int>(7);
            std::array<Status,5> nested{};unsigned releases=0;bool committed=false;
            auto retired=new Retirement{&vm,&error,services,hold,0,&releases,&committed,false,&nested};
            Hold old(retired,retire);
            require(vm.create(bind(original),error,2*1024*1024,old)==Status::complete,"retirement VM creation failed");
            retired->vm=vm.vm_identity();old.reset();
            // The input service table lives inside the context released by the
            // installation; it must be captured before that context is retired.
            require(vm.install_created_services(retired->next,error,hold)==Status::complete &&
                releases==1 && committed && all_busy(nested) && error.empty() && vm.uses_services(services),
                "installation retirement was premature or accepted mutation reentry");++retirement_cases;
            require(vm.reset(error)==Status::complete,"retirement VM reset failed");
        }
        {
            Session vm;Fixture original,new_fixture;const auto services=bind(new_fixture);
            auto hold=std::make_shared<int>(8);std::array<Status,5> nested{};
            unsigned releases=0;bool committed=false;
            auto retired=new Retirement{&vm,&error,services,hold,0,&releases,&committed,true,&nested};
            Hold context(retired,retire);
            require(vm.create(bind(original),error)==Status::complete &&
                vm.install_created_services(services,error,context)==Status::complete,"close fixture installation failed");
            context.reset();
            require(vm.reset(error)==Status::complete && releases==1 && committed && all_busy(nested) &&
                error.empty() && vm.vm_identity()==0,"VM close allowed callback resurrection");++retirement_cases;
        }
        {
            Fixture original,new_fixture;const auto services=bind(new_fixture);
            auto hold=std::make_shared<int>(9);std::array<Status,5> nested{};
            unsigned releases=0;bool committed=false;
            {
                Session vm;
                auto retired=new Retirement{&vm,&error,services,hold,0,&releases,&committed,true,&nested};
                Hold context(retired,retire);
                require(vm.create(bind(original),error)==Status::complete &&
                    vm.install_created_services(services,error,context)==Status::complete,"destructor fixture failed");
                context.reset();
            }
            require(releases==1 && committed && all_busy(nested),"VM destructor allowed callback resurrection");++retirement_cases;
        }
        // Every actual later stage, including faulted state, rejects callback
        // replacement without disturbing VM, original services or accounting.
        for(unsigned stage=0;stage<7;++stage) {
            Session vm;Fixture original,new_fixture;auto hold=std::make_shared<int>(1);
            const auto old=bind(original),replacement=bind(new_fixture);
            if(stage!=0) require(vm.create(old,error)==Status::complete,"guard creation failed");
            if(stage==2) require(vm.bind_ais_functions(error)==Status::complete,"AIS bind guard failed");
            if(stage>=3) require(vm.bind_functions(error)==Status::complete,"binding guard failed");
            if(stage>=4) require(vm.load_common(commons,error)==Status::complete,"common guard failed");
            if(stage==5) require(vm.load_external(monster,error)==Status::complete,"external guard failed");
            if(stage==6) { const std::string bad="function broken(";
                require(vm.load_external(source(bad),error)==Status::script_error && vm.stage()==Stage::faulted,
                    "faulted guard fixture did not fault"); }
            const auto identity=vm.vm_identity(),memory=vm.statistics().lua_memory_used;const auto current=vm.stage();
            if(stage==1) {
                auto invalid=replacement;invalid.owner=0;
                require(vm.install_created_services(invalid,error,hold)==Status::invalid_argument,"zero owner accepted");++guards;
                invalid.owner=other;
                require(vm.install_created_services(invalid,error,hold)==Status::invalid_argument,"different owner accepted");++guards;
                require(vm.install_created_services(replacement,error,{})==Status::invalid_argument,"unretained callbacks accepted");++guards;
            } else {require(vm.install_created_services(replacement,error,hold)==Status::not_ready,"out-of-order installation accepted");++guards;}
            require(vm.vm_identity()==identity && vm.stage()==current && vm.statistics().lua_memory_used==memory &&
                (stage==0 || vm.uses_services(old)) && new_fixture.trace.empty(),"rejected installation changed state");
        }
        {
            Session vm;Fixture original;auto callbacks=std::make_shared<Fixture>();auto s=bind(*callbacks);
            s.set_target=nullptr;
            require(vm.create(bind(original),error)==Status::complete &&
                vm.install_created_services(s,error,callbacks)==Status::complete,"missing provider fixture failed");
            stage_load(vm,commons,monster,error);
            require(vm.dispatch(Event::enemy_spotted,enemy,error)==Status::script_error && vm.statistics().faulted && !vm.ready() &&
                callbacks->trace==std::vector<std::string>{"Struct","Prop","HasTarget"} && callbacks->target==0 &&
                !error.empty(),"missing provider fabricated target or lost Lua effects");++cases;
        }
        std::printf("{\"validation\":\"PASS\",\"bridge_cases\":%u,\"guard_cases\":%u,\"retirement_cases\":%u,"
            "\"same_actual_vm\":true,\"original_scripts_used\":true,\"native_wired\":false,\"new_original_bodies\":0,\"mismatches\":0}\n",
            cases,guards,retirement_cases);
        return 0;
    } catch(const std::exception& failure) {
        std::fprintf(stderr,"%s\n",failure.what());return 1;
    }
}
