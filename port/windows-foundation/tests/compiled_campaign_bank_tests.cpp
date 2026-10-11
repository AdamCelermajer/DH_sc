#include "../original_campaign_runtime.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

void check_active_bank(const AssetCatalog& assets, const std::string& level, const std::string& expected,
                       const std::string& absent) {
    OriginalCampaignRuntime runtime;
    std::string error;
    check(runtime.load_compiled_level(assets, level, error), "compiled bank load " + level + ": " + error);
    const auto found = runtime.script_id(expected, false);
    check(found >= 0, "expected active-level script is unavailable: " + expected);
    check(runtime.script_id(absent, false) < 0, "script from another level leaked into active scope: " + absent);
    bool active_seen = false;
    for (std::size_t i = 0; i < runtime.scripts().size(); ++i) {
        const auto& script = runtime.scripts()[i];
        check(script.id == static_cast<int>(i), "source ScriptManager order was not retained");
        if (script.scope == "active-level") active_seen = true;
        else check(!active_seen && script.scope == "common", "common scripts must precede the active level bank");
    }
    check(active_seen, "compiled active-level bank was not appended");
    if (expected == "Intro_Witch") {
        const auto& command = runtime.scripts().at(static_cast<std::size_t>(found)).commands.at(63);
        const auto target = command.strings.find(12);
        check(command.kind == 55 && target != command.strings.end() && target->second == "_prim_BlueDoor",
              "active Witch bank command 63 is the source CloseDoor for BlueDoor");
    }
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply the self-contained staged package assets root");
        const AssetCatalog assets(argv[1]);
        check_active_bank(assets, "data/scene/002_swamp_witchcave.rule.xml", "Intro_Witch", "Swamp_Intro");
        check_active_bank(assets, "data/scene/001_swamp.mlx", "Swamp_Intro", "Intro_Witch");
        std::cout << "compiled_campaign_bank tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "compiled_campaign_bank test failed: " << e.what() << '\n';
        return 1;
    }
}
