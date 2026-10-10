// I026 level-up presentation: outcome branches over the real original
// EffectsTables (set 135 must be the level_up set) and the FX play seam.
#include "runtime_level_up_presentation_v1.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::effects;

namespace {
void check(bool value, const char* what) {
    if (!value) throw std::runtime_error(what);
}

std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot read " + path);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

struct Calls {
    int count = 0;
    std::int32_t set = -1;
    float position[3]{-1, -1, -1};
    const float* rotation = reinterpret_cast<const float*>(1);
    std::uintptr_t anchor = 0;
    std::uintptr_t* created = reinterpret_cast<std::uintptr_t*>(1);
};
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "usage: runtime_level_up_presentation_v1_tests <original data dir>");
        const std::string dir = argv[1];
        const auto records = read(dir + "/effects_pyarray.bin");
        const auto names = read(dir + "/effects_pyarraynames.bin");
        const auto schema = read(dir + "/effects_pystructnames.bin");
        const auto dictionary_names = read(dir + "/effects_dictionary_pyarraynames.bin");
        const auto dictionary_paths = read(dir + "/effects_dictionary_pyarray.bin");
        auto bytes = [](const std::vector<std::uint8_t>& v) {
            return dh2::data::Bytes{v.data(), v.size()};
        };
        dh2::data::EffectsTables tables;
        std::string error;
        check(tables.load(bytes(records), bytes(names), bytes(schema), bytes(dictionary_names),
                          bytes(dictionary_paths), error), error.c_str());
        const auto borrow = tables.borrow();
        const ActorId player = 7;

        // Branch 1: success. One play of set 135 at ZERO, NULL rotation, anchor = player.
        {
            Calls calls;
            RuntimeLevelUpPlaySetV1 play = [&](std::int32_t set, const float position[3],
                const float* rotation, std::uintptr_t anchor, std::uintptr_t* created, std::string&) {
                ++calls.count; calls.set = set;
                for (int i = 0; i < 3; ++i) calls.position[i] = position[i];
                calls.rotation = rotation; calls.anchor = anchor; calls.created = created;
                return true;
            };
            RuntimeLevelUpPresentationResultV1 result;
            check(play_level_up_presentation_v1(borrow, play, player, result, error), error.c_str());
            check(calls.count == 1, "success must play exactly one FX set");
            check(calls.set == 135, "level-up FX must be set 135");
            check(calls.position[0] == 0 && calls.position[1] == 0 && calls.position[2] == 0,
                  "level-up FX position must be the source ZERO point");
            check(calls.rotation == nullptr, "level-up FX rotation must be NULL");
            check(calls.anchor == static_cast<std::uintptr_t>(player), "anchor must be the leveling player");
            check(calls.created == nullptr, "created identity must be NULL as in PlayAnimFXSet");
            check(result.fx_played && result.fx == "fx=135:level_up:played", "success log text");
            check(result.text.find("LEVEL UP!") != std::string::npos, "MENU_LEVEL_UP text in log");
        }
        // Branch 2: FX play failure is reported but does not fail the award.
        {
            RuntimeLevelUpPlaySetV1 play = [](std::int32_t, const float*, const float*, std::uintptr_t,
                std::uintptr_t*, std::string& e) { e = "asset missing"; return false; };
            RuntimeLevelUpPresentationResultV1 result;
            check(play_level_up_presentation_v1(borrow, play, player, result, error),
                  "FX failure must not fail the level-up presentation");
            check(!result.fx_played, "failed play must not be marked played");
            check(result.fx == "fx=135:level_up:failed(asset missing)", "failure log text");
        }
        // Branch 3: invalid requests fail.
        {
            RuntimeLevelUpPresentationResultV1 result;
            RuntimeLevelUpPlaySetV1 play = [](std::int32_t, const float*, const float*, std::uintptr_t,
                std::uintptr_t*, std::string&) { return true; };
            check(!play_level_up_presentation_v1(borrow, play, invalid_actor_id, result, error),
                  "invalid ActorId must fail");
            check(!play_level_up_presentation_v1(borrow, nullptr, player, result, error),
                  "missing FX play seam must fail");
            check(!play_level_up_presentation_v1(dh2::data::EffectsTables::Borrow{}, play, player, result, error),
                  "missing EffectsTables must fail");
        }
        std::cout << "runtime_level_up_presentation_v1_tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "runtime_level_up_presentation_v1_tests failed: " << e.what() << '\n';
        return 1;
    }
}
