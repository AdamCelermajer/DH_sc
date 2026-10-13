// v21.8 es-runtime: unit coverage for RuntimeSkillExternalOwnerRegistryV1 (N1: several external owners behind the single
// coordinator slot). No assets: fake owners plus the real Mage owner (handles() only). Covered:
//   - add(): null and duplicate owners are refused;
//   - dispatch: pre()/use() go to the first registered owner whose handles() is true; no owner is an explicit error;
//   - advance() reaches every owner in order and stops at the first failure;
//   - cleanup() and reset() reach every owner;
//   - the real Mage owner keeps its skills when a non-Mage owner is registered beside it.
#include "runtime_mage_skill_owner_v1.hpp"
#include "runtime_skill_external_owner_registry_v1.hpp"
#include "runtime_skill_use_profile_registry_v1.hpp"
#include "../../combat_session.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

class FakeOwner final : public RuntimeSkillExternalOwnerV1 {
public:
    FakeOwner(std::string name, std::vector<std::string> skills) : name_(std::move(name)), skills_(std::move(skills)) {}
    bool handles(const RuntimeSkillUseProfileV1& profile) const override {
        for (const auto& skill : skills_)
            if (skill == profile.skill_name) return true;
        return false;
    }
    bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&) override {
        ++pre_calls;
        last_pre = name_;
        return true;
    }
    bool use(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&) override {
        ++use_calls;
        last_use = name_;
        return true;
    }
    bool advance(CombatSession&, double, std::string& error) override {
        ++advance_calls;
        if (fail_advance) {
            error = name_ + " failed";
            return false;
        }
        return true;
    }
    void cleanup(CombatSession&, ActorId actor) override { cleaned.push_back(actor); }
    void reset() override { ++reset_calls; }

    std::string name_;
    std::vector<std::string> skills_;
    unsigned pre_calls = 0, use_calls = 0, advance_calls = 0, reset_calls = 0;
    bool fail_advance = false;
    std::string last_pre, last_use;
    std::vector<ActorId> cleaned;
};

RuntimeSkillExternalContextV1 context_for(const RuntimeSkillUseProfileV1* profile) {
    RuntimeSkillExternalContextV1 context;
    context.caster = 1;
    context.generation = 1;
    context.profile = profile;
    context.skill_name = profile ? profile->skill_name : "";
    return context;
}
} // namespace

int main() {
    try {
        const auto* chain = find_skill_use_profile_registered_v1("ChainLightning", "prince_mage_chain_lightning");
        const auto* cyclone = find_skill_use_profile_registered_v1("Cyclone", "prince_mage_cyclone");
        const auto* coldray = find_skill_use_profile_registered_v1("ColdRay", "prince_mage_coldray");
        check(chain && cyclone && coldray, "Profile rows are missing");

        CombatSession session;
        std::string error;

        // add(): null and duplicates are refused.
        RuntimeSkillExternalOwnerRegistryV1 registry;
        FakeOwner first("first", {"ChainLightning"});
        FakeOwner second("second", {"ChainLightning", "Cyclone"});
        check(!registry.add(nullptr), "A null owner was registered");
        check(registry.add(&first) && registry.add(&second), "Owners were not registered");
        check(!registry.add(&first) && registry.size() == 2, "A duplicate owner was registered");
        std::cout << "add: null and duplicate refused, size 2\n";

        // Dispatch: the first registered owner that handles the profile runs it.
        check(registry.handles(*chain) && registry.handles(*cyclone), "Registry does not handle its owners' skills");
        check(!registry.handles(*coldray), "Registry claims a skill no owner handles");
        check(registry.owner_for(*chain) == &first, "ChainLightning did not go to the first registered owner");
        check(registry.owner_for(*cyclone) == &second, "Cyclone did not go to the owner that handles it");
        check(registry.use(session, context_for(chain), error) && first.use_calls == 1 && second.use_calls == 0,
              "use() did not dispatch ChainLightning to the first owner: " + error);
        check(registry.use(session, context_for(cyclone), error) && second.use_calls == 1 && first.use_calls == 1,
              "use() did not dispatch Cyclone to the second owner: " + error);
        check(registry.pre(session, context_for(cyclone), error) && second.pre_calls == 1 && first.pre_calls == 0,
              "pre() did not dispatch Cyclone to the second owner: " + error);
        std::cout << "dispatch: ChainLightning -> first, Cyclone -> second (use and pre)\n";

        // No owner for the skill is an explicit error and calls nothing.
        check(!registry.use(session, context_for(coldray), error) && error.find("ColdRay") != std::string::npos,
              "A skill without an owner did not fail explicitly");
        check(!registry.pre(session, context_for(nullptr), error) && !error.empty(), "A null profile was accepted");
        std::cout << "no owner: explicit error \"" << error << "\"\n";

        // advance(): every owner in registration order; the first failure stops the walk.
        FakeOwner third("third", {});
        check(registry.add(&third), "Third owner was not registered");
        check(registry.advance(session, 16.0, error) && first.advance_calls == 1 && second.advance_calls == 1 &&
              third.advance_calls == 1, "advance() did not reach every owner: " + error);
        second.fail_advance = true;
        check(!registry.advance(session, 16.0, error) && error == "second failed", "advance() did not report the owner failure");
        check(first.advance_calls == 2 && second.advance_calls == 2 && third.advance_calls == 1,
              "advance() walked past the failing owner or skipped an earlier one");
        std::cout << "advance: reaches every owner; failure stops the walk, reported as \"" << error << "\"\n";
        second.fail_advance = false;

        // cleanup() and reset() reach every owner.
        registry.cleanup(session, 42);
        check(first.cleaned.size() == 1 && first.cleaned[0] == 42 && second.cleaned.size() == 1 && second.cleaned[0] == 42,
              "cleanup() did not reach every owner with the caster");
        registry.reset();
        check(first.reset_calls == 1 && second.reset_calls == 1, "reset() did not reach every owner");
        std::cout << "cleanup(42) and reset() reach both owners\n";

        // The real Mage owner stays dispatchable next to a non-Mage owner.
        RuntimeMageSkillOwnerV1 mage(RuntimeMageSkillServicesV1{});
        RuntimeSkillExternalOwnerRegistryV1 mixed;
        FakeOwner other("other", {"FireSkullRain"});
        check(mixed.add(&other) && mixed.add(&mage), "Mixed registry was not built");
        check(mixed.owner_for(*chain) == &mage && mixed.owner_for(*cyclone) == &mage, "Mage skills did not stay on the Mage owner");
        check(mixed.owner_for(*coldray) == nullptr, "ColdRay has an owner in the mixed registry");
        std::cout << "mixed registry: Mage skills still resolve to the Mage owner beside a non-Mage owner\n";
        std::cout << "runtime_skill_external_owner_registry_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "runtime_skill_external_owner_registry_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
