#include "../player_kill_sequence_v1.hpp"
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <vector>

using namespace dh2::player_kill_sequence_v1;
struct Fixture { State* state{}; std::vector<unsigned> trace; bool fail_loot=false; };
int invoke(void* raw, Step step, std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    f.trace.push_back(unsigned(step));
    if (f.state) {
        Result nested{}; std::string nested_error="sentinel";
        if (run(*f.state, raw, invoke, &nested, nested_error)!=Status::busy ||
            nested_error!="sentinel") std::abort();
        f.state=nullptr;
    }
    if (step==Step::drop_loot && f.fail_loot) { error="loot owner unavailable"; return 1; }
    return 0;
}
int throwing(void*, Step, std::string&) { throw std::runtime_error("provider exception"); }
int main() {
    State state{}; Fixture fixture{&state,{},true}; Result result{}; std::string error;
    if (run(state,&fixture,invoke,&result,error)!=Status::complete) return 1;
    const std::vector<unsigned> expected{0,1,2,3};
    if (fixture.trace!=expected || result.calls!=4 || result.attempted_mask!=15 ||
        result.failed_mask!=1 || error!="loot owner unavailable") return 2;
    Result replay{}; error.clear();
    if (run(state,&fixture,invoke,&replay,error)!=Status::complete || replay.calls ||
        replay.attempted_mask || fixture.trace!=expected) return 3;
    State exceptional{};Result untouched{91,92,93};error="sentinel";
    try { (void)run(exceptional,nullptr,throwing,&untouched,error); return 4; }
    catch (const std::runtime_error&) {}
    if (exceptional.busy || untouched.calls!=91 || error!="sentinel") return 5;
    std::puts("PASS: Character::Kill stage order, failure continuation, reentry guard, and one-shot replay");
    return 0;
}
