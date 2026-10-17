#include "../character_ai_update_all_skills.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace k = dh2::character_ai_update_all_skills;
constexpr std::uintptr_t AI = 0x100143c8;
constexpr std::uintptr_t OWNER_A = 0x10018000;
constexpr std::uintptr_t OWNER_B = 0x10019000;

struct Call { std::uint32_t operation, list; std::uintptr_t subject; std::uint32_t index; };
struct Fixture {
    std::string scenario;
    std::array<std::uintptr_t, 4> skill_storage{{0x1101, 0, 0x1102, 0x1103}};
    std::array<std::uintptr_t, 4> skill_replacement{{0x1190, 0x1191, 0x1192, 0x1193}};
    std::array<std::uintptr_t, 4> faery_storage{{0x2201, 0x2202, 0x2203, 0}};
    std::array<std::uintptr_t, 4> faery_replacement{{0x2290, 0x2291, 0x2292, 0x2293}};
    k::State state{};
    k::Result result{};
    std::uint32_t using_skill=0,casting=0,updates=0,fail_on=0,throw_on=0;
    std::vector<Call> calls;
    k::Services services{this,invoke};

    explicit Fixture(std::string name) : scenario(std::move(name)) {
        state={AI,OWNER_A,{skill_storage.data(),skill_storage.data()+3},
                         {faery_storage.data(),faery_storage.data()+2}};
        if(scenario=="using") using_skill=7;
        if(scenario=="casting") casting=1;
        if(scenario=="owner_mutation") {}
        if(scenario=="skill_rebase") state.skills={skill_storage.data(),skill_storage.data()+3};
        if(scenario=="faery_replace_from_skill") {
            state.skills={skill_storage.data(),skill_storage.data()+1};
            state.faeries={faery_storage.data(),faery_storage.data()+1};
        }
        if(scenario=="faery_rebase") state.faeries={faery_storage.data(),faery_storage.data()+3};
        if(scenario=="null_slots") {
            state.skills={skill_storage.data()+1,skill_storage.data()+2};
            state.faeries={faery_storage.data()+3,faery_storage.data()+4};
        }
        if(scenario=="empty") {
            state.skills={nullptr,nullptr};state.faeries={nullptr,nullptr};
        }
        if(scenario=="fail_callback") fail_on=4;
        if(scenario=="throw_callback") throw_on=4;
    }

    static std::int32_t invoke(void* opaque,k::State* state,const k::Request* request,k::Response* response) {
        auto& f=*static_cast<Fixture*>(opaque);
        f.calls.push_back({static_cast<std::uint32_t>(request->operation),
                           static_cast<std::uint32_t>(request->list),request->subject,request->index});
        const auto call=static_cast<std::uint32_t>(f.calls.size());
        if(f.fail_on==call)return 9;
        if(f.throw_on==call)throw std::runtime_error("source provider failed");
        switch(request->operation) {
        case k::Operation::is_using_skill:
            response->word=f.using_skill;
            if(f.scenario=="owner_mutation")state->owner=OWNER_B;
            break;
        case k::Operation::is_casting:response->word=f.casting;break;
        case k::Operation::on_skill_update:
            ++f.updates;
            if(f.scenario=="skill_rebase"&&f.updates==1)
                state->skills={f.skill_replacement.data(),f.skill_replacement.data()+3};
            if(f.scenario=="faery_replace_from_skill"&&request->list==k::List::skill&&f.updates==1)
                state->faeries={f.faery_replacement.data(),f.faery_replacement.data()+3};
            if(f.scenario=="faery_rebase"&&request->list==k::List::faery&&f.updates==3)
                state->faeries={f.faery_replacement.data(),f.faery_replacement.data()+3};
            response->word=0xffffffffu; // original child has a void return.
            break;
        }
        return 0;
    }
    k::Status run(){return k::update(&state,&services,&result);}
};

void dump(const Fixture& f,k::Status status) {
    std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"decision\":"
      <<static_cast<std::uint32_t>(f.result.decision)<<",\"skills\":"<<f.result.skill_slots
      <<",\"faeries\":"<<f.result.faery_slots<<",\"updates\":"<<f.result.script_updates
      <<",\"using_word\":"<<f.result.using_skill_word<<",\"casting_word\":"<<f.result.casting_word
      <<",\"calls\":[";
    for(std::size_t i=0;i<f.calls.size();++i){if(i)std::cout<<',';const auto& c=f.calls[i];
      std::cout<<'['<<c.operation<<','<<c.list<<','<<c.subject<<','<<c.index<<']';}
    std::cout<<"]}\n";
}

int main(int argc,char** argv) {
    if(argc==2){Fixture f(argv[1]);dump(f,f.run());return 0;}
    unsigned cases=0;
    {
        Fixture f("baseline");assert(f.run()==k::Status::complete);
        assert(f.calls.size()==6&&f.result.skill_slots==3&&f.result.faery_slots==2&&f.result.script_updates==4);
        assert(f.calls[2].subject==0x1101&&f.calls[3].subject==0x1102&&f.calls[4].subject==0x2201&&f.calls[5].subject==0x2202);++cases;
    }
    {
        Fixture f("using");assert(f.run()==k::Status::complete&&f.calls.size()==1);
        assert(f.result.decision==k::Decision::skipped_while_using_skill&&f.result.using_skill_word==7);++cases;
    }
    {
        Fixture f("casting");assert(f.run()==k::Status::complete&&f.calls.size()==2);
        assert(f.result.decision==k::Decision::skipped_while_casting&&f.result.casting_word==1);++cases;
    }
    {
        Fixture f("owner_mutation");assert(f.run()==k::Status::complete&&f.calls[1].subject==OWNER_B);++cases;
    }
    {
        Fixture f("skill_rebase");assert(f.run()==k::Status::complete);
        assert(f.calls[2].subject==0x1101&&f.calls[3].subject==0x1191&&f.calls[4].subject==0x1192);++cases;
    }
    {
        Fixture f("faery_replace_from_skill");assert(f.run()==k::Status::complete);
        assert(f.result.faery_slots==3&&f.calls.back().subject==0x2292);++cases;
    }
    {
        Fixture f("faery_rebase");assert(f.run()==k::Status::complete);
        assert(f.calls[4].subject==0x2201&&f.calls[5].subject==0x2291&&f.calls[6].subject==0x2292);++cases;
    }
    {
        Fixture f("null_slots");assert(f.run()==k::Status::complete&&f.calls.size()==2&&f.result.script_updates==0);++cases;
    }
    {
        Fixture f("empty");assert(f.run()==k::Status::complete&&f.calls.size()==2);++cases;
    }
    {
        Fixture f("fail_callback");assert(f.run()==k::Status::service_failed&&f.calls.size()==4&&f.result.script_updates==1);++cases;
    }
    {
        Fixture f("throw_callback");assert(f.run()==k::Status::service_failed&&f.calls.size()==4&&f.result.script_updates==1);++cases;
    }
    {
        Fixture f("baseline");f.services.invoke=nullptr;assert(f.run()==k::Status::service_unavailable&&f.calls.empty());++cases;
    }
    {
        Fixture f("baseline");auto* alias=reinterpret_cast<k::Result*>(&f.state);
        assert(k::update(&f.state,&f.services,alias)==k::Status::invalid_argument&&f.calls.empty());++cases;
    }
    {
        Fixture f("baseline");f.state.skills={nullptr,f.skill_storage.data()+2};
        assert(f.run()==k::Status::invalid_source_fact&&f.calls.size()==2);++cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases
      <<",\"native_wired\":false,\"missing_script_implementation_fails_closed\":true}\n";
}
