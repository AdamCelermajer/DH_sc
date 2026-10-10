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
        {
            // P16 LEVELUP4 PLACEHOLDER window logic (level_up_placeholder_column_v1.hpp, NOT original art):
            // the gold sheet part is recoloured white only inside [begin, cut) and dropped outside it;
            // other parts and other FX uris are untouched.
            const auto& column = kLevelUpSet135PlaceholderV1;
            check(column.white_cut_ms - column.white_begin_ms == 330, "placeholder white window length");
            check(column.white_begin_ms == 100 && column.white_cut_ms == 430, "placeholder window bounds");
            std::vector<dh2::scene::Material> table(2);
            table[0].id = column.sheet_material;
            table[0].color[0] = 0.5f; table[0].color[1] = 0.4f; table[0].color[2] = 0.1f; table[0].color[3] = 1.f;
            table[1].id = "gloow";
            const std::vector<std::uint32_t> sheet_binding{0}, other_binding{1};
            auto make = [&](const std::vector<std::uint32_t>* binding) {
                dh2::skinning::VisualDrawPartV6 part;
                part.material_table = &table;
                part.materials = binding;
                return part;
            };
            const std::string uri = "DATA\\3d\\interface\\level_up.bdae";
            {
                std::vector<dh2::skinning::VisualDrawPartV6> parts{make(&sheet_binding), make(&other_binding)};
                dh2::fx::apply_level_up_placeholder_v1(uri, 0, parts);
                check(parts.size() == 1 && parts[0].materials == &other_binding, "sheet dropped before the white window");
            }
            {
                std::vector<dh2::skinning::VisualDrawPartV6> parts{make(&sheet_binding), make(&other_binding)};
                dh2::fx::apply_level_up_placeholder_v1(uri, 200, parts);
                check(parts.size() == 2, "sheet kept inside the white window");
                check(parts[0].material_table != &table, "sheet uses a white snapshot table");
                const auto& white = (*parts[0].material_table)[0];
                check(white.color[0] == 1.f && white.color[1] == 1.f && white.color[2] == 1.f && white.color[3] == 1.f,
                      "sheet colour is white in the window");
                check(table[0].color[0] == 0.5f, "authored table is not modified");
                check(parts[1].material_table == &table && parts[1].materials == &other_binding, "other part untouched");
            }
            {
                std::vector<dh2::skinning::VisualDrawPartV6> parts{make(&sheet_binding)};
                dh2::fx::apply_level_up_placeholder_v1(uri, 430, parts);
                check(parts.empty(), "sheet dropped at the hard cut");
            }
            {
                std::vector<dh2::skinning::VisualDrawPartV6> parts{make(&sheet_binding)};
                dh2::fx::apply_level_up_placeholder_v1("data/3d/characters/other.bdae", 200, parts);
                check(parts.size() == 1 && parts[0].material_table == &table, "other FX uri is a no-op");
            }
        }
        std::cout << "runtime_level_up_presentation_v1_tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "runtime_level_up_presentation_v1_tests failed: " << e.what() << '\n';
        return 1;
    }
}
