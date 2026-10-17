#include "../faery_selection_transaction_v1.hpp"

#include <iostream>
#include <string>

namespace {
struct Fixture {
    unsigned prepares{}, commits{}, discards{};
    bool placement_ready=true, commit_ok=true;
};
bool prepare(void* raw, std::uintptr_t character, std::uint32_t id,
             std::uintptr_t& plan, std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    ++f.prepares;
    if (!character || id!=2 || !f.placement_ready) {
        error="placement unavailable";
        return false;
    }
    plan=0xfeed;
    error.clear();
    return true;
}
bool commit(void* raw, std::uintptr_t plan, std::string& error) {
    auto& f=*static_cast<Fixture*>(raw);
    ++f.commits;
    if (plan!=0xfeed || !f.commit_ok) {
        error="transaction rejected";
        return false;
    }
    return true;
}
void discard(void* raw, std::uintptr_t plan) {
    if (plan==0xfeed) ++static_cast<Fixture*>(raw)->discards;
}
}

int main() {
    Fixture f; std::string error;
    dh2::ui::FaerySelectionTransactionServicesV1 services{&f,prepare,commit,discard};
    if (dh2::ui::transact_faery_selection_v1(0x1234,2,{},error) || f.prepares || f.commits ||
        error.find("active Level placement") == std::string::npos) {
        std::cerr << "unbound placement must fail before any canonical owner mutation\n";
        return 1;
    }
    f.placement_ready=false;
    if (dh2::ui::transact_faery_selection_v1(0x1234,2,services,error) ||
        f.prepares!=1 || f.commits || error!="placement unavailable") {
        std::cerr << "failed placement preparation must not commit Save or skills\n";
        return 2;
    }
    f.placement_ready=true; f.commit_ok=false;
    if (dh2::ui::transact_faery_selection_v1(0x1234,2,services,error) ||
        f.prepares!=2 || f.commits!=1 || f.discards!=1) {
        std::cerr << "failed coordinated commit must discard its uncommitted plan\n";
        return 3;
    }
    f.commit_ok=true;
    if (!dh2::ui::transact_faery_selection_v1(0x1234,2,services,error) ||
        f.prepares!=3 || f.commits!=2 || f.discards!=1 || !error.empty()) {
        std::cerr << "ready placement and canonical owners must commit once\n";
        return 4;
    }
    std::cout << "{\"validation\":\"PASS\",\"checks\":4}\n";
    return 0;
}
