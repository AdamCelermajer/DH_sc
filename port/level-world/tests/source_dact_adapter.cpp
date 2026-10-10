#include "../world.hpp"
#include "../source_handle_ledger_v1.hpp"
#include "../../game-data/data.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: source_dact_adapter_audit original-cache-root android-assets-root\n";
        return 2;
    }
    try {
        const std::string cache_root = argv[1];
        const std::string assets_root = argv[2];
        const auto mlx = read(cache_root + "/data/scene/001_swamp.mlx");
        const auto expected = read(assets_root + "/worlds/001_swamp.dact");

        const auto character_records = read(assets_root + "/data/character_properties_pyarray.bin");
        const auto character_names = read(assets_root + "/data/character_properties_pyarraynames.bin");
        const auto character_fields = read(assets_root + "/data/character_properties_pystructnames.bin");
        const auto model_names = read(assets_root + "/data/character_models_dictionary_pyarraynames.bin");
        const auto model_values = read(assets_root + "/data/character_models_dictionary_pyarray.bin");
        dh2::data::CharacterTable characters;
        dh2::data::Dictionary models;
        std::string error;
        require(dh2::data::load_characters(
                    {character_records.data(), character_records.size()},
                    {character_names.data(), character_names.size()},
                    {character_fields.data(), character_fields.size()}, characters, error),
                error.c_str());
        require(dh2::data::load_dictionary(
                    {model_names.data(), model_names.size()},
                    {model_values.data(), model_values.size()}, models, error), error.c_str());

        constexpr std::array<const char*, 9> names = {
            "obj_4of4_brdwalk_sw_00.mgp",
            "obj_3of4_brdwalk_sw_00.mgp",
            "obj_1of4_brdwalk_nse_00.mgp",
            "corner_ruin_ws_00.mgp",
            "merchantcamp_ruins_swe_00.mgp",
            "corner_brdwalk_se_00.mgp",
            "deadend_brdwalk_w_00.mgp",
            "bossroom_ruins_ns_.mgp",
            "obj_2of4_brdwalk_sw_00.mgp"
        };
        std::array<std::vector<std::uint8_t>, names.size()> mgp_bytes;
        std::array<std::string, names.size()> paths;
        std::array<dh2::world::SourceMgpView, names.size()> mgps{};
        constexpr std::array<const char*, 9> mvp_names = {
            "obj_4of4_brdwalk_sw_00.mvp", "obj_3of4_brdwalk_sw_00.mvp",
            "obj_1of4_brdwalk_nse_00.mvp", "corner_ruin_ws_00.mvp",
            "merchantcamp_ruins_swe_00.mvp", "corner_brdwalk_se_00.mvp",
            "deadend_brdwalk_w_00.mvp", "bossroom_ruins_ns_.mvp",
            "obj_2of4_brdwalk_sw_00.mvp"};
        std::array<std::vector<std::uint8_t>, names.size()> mvp_bytes;
        std::array<std::string, names.size()> mvp_paths;
        std::array<dh2::world::source_handle_ledger_v1::OrderedModuleFile,
                   names.size()*2> ordered_files{};
        for (std::size_t i = 0; i < names.size(); ++i) {
            paths[i] = std::string("data/3d/modules/swamp/mgp/") + names[i];
            mgp_bytes[i] = read(cache_root + "/" + paths[i]);
            mgps[i] = {paths[i].c_str(), mgp_bytes[i].data(), mgp_bytes[i].size()};
            mvp_paths[i] = std::string("data/3d/modules/swamp/mvp/") + mvp_names[i];
            mvp_bytes[i] = read(cache_root + "/" + mvp_paths[i]);
            ordered_files[i*2] = {static_cast<std::uint32_t>(i),
                dh2::world::source_handle_ledger_v1::ModuleFileKind::mgp,
                paths[i].c_str(),mgp_bytes[i].data(),mgp_bytes[i].size()};
            ordered_files[i*2+1] = {static_cast<std::uint32_t>(i),
                dh2::world::source_handle_ledger_v1::ModuleFileKind::mvp,
                mvp_paths[i].c_str(),mvp_bytes[i].data(),mvp_bytes[i].size()};
        }

        std::vector<std::uint8_t> compiled;
        require(dh2::world::compile_source_dact(mlx.data(), mlx.size(), mgps.data(),
                    mgps.size(), characters, models, compiled, error),
                error.c_str());
        require(compiled.size() == 1296, "source DACT byte count differs from five v1 rows");
        require(compiled == expected, "source MGP actors differ from checked-in DACT v1 bytes");

        dh2::world::source_handle_ledger_v1::Ledger handles;
        require(dh2::world::source_handle_ledger_v1::build(mlx.data(),mlx.size(),
            "SWAMP","data/scene/001_swamp.mlx",ordered_files.data(),
            ordered_files.size(),4,10,handles,error)==
            dh2::world::source_handle_ledger_v1::Status::ok,error.c_str());
        std::vector<dh2::world::StaticDactActorHandleV1> actor_sources;
        require(dh2::world::compile_source_dact_with_handles(mlx.data(),mlx.size(),
            mgps.data(),mgps.size(),characters,models,handles,actor_sources,
            compiled,error),error.c_str());
        constexpr std::array<std::uint32_t,5> source_modules{2,3,3,5,5};
        constexpr std::array<std::uint32_t,5> source_records{1,1,3,0,2};
        require(actor_sources.size()==5&&compiled==expected,
            "source DACT handle adapter changed byte output or row count");
        for(std::size_t i=0;i<actor_sources.size();++i){
            const auto& actor=actor_sources[i];
            require(actor.dact_record==i&&actor.module_index==source_modules[i]&&
                    actor.source_record==source_records[i],
                "DACT row did not retain the exact MGP module/source-record identity");
            const auto found=std::find_if(handles.occurrences.begin(),
                handles.occurrences.end(),[&](const auto& occurrence){
                    return occurrence.source.kind==static_cast<dh2::world::RecordKind>(1)&&
                        occurrence.source.module_index==source_modules[i]&&
                        occurrence.source.record_index==source_records[i];
                });
            require(found!=handles.occurrences.end()&&!found->duplicate_name&&
                    actor.source_handle==found->handle&&actor.name==found->source.name,
                "DACT actor handle was not joined by exact MGP provenance");
        }
        auto duplicate_handles=handles;
        auto duplicate=std::find_if(duplicate_handles.occurrences.begin(),
            duplicate_handles.occurrences.end(),[](const auto& occurrence){
                return occurrence.source.kind==static_cast<dh2::world::RecordKind>(1)&&
                    occurrence.source.module_index==2&&occurrence.source.record_index==1;
            });
        require(duplicate!=duplicate_handles.occurrences.end(),
            "selected source MGP occurrence is absent from ObjectManager ledger");
        duplicate->duplicate_name=true;
        actor_sources.assign(1,{});compiled.assign(1,0xff);error.clear();
        require(!dh2::world::compile_source_dact_with_handles(mlx.data(),mlx.size(),
            mgps.data(),mgps.size(),characters,models,duplicate_handles,
            actor_sources,compiled,error)&&actor_sources.empty()&&compiled.empty(),
            "duplicate ObjectManager name was accepted as a live DACT Character");

        auto changed_bytes = mgp_bytes;
        auto changed_mgps = mgps;
        std::string changed_mgp(changed_bytes[2].begin(), changed_bytes[2].end());
        constexpr char target[] = "name=\"_prim_Monster_53_03_001\"";
        const auto actor = changed_mgp.find(target);
        require(actor != std::string::npos &&
                    changed_mgp.find(target, actor + sizeof(target) - 1) == std::string::npos,
                "expected exactly one supported source actor for mutation");
        const auto probability_offset = changed_mgp.find("spawn_prob", actor);
        const auto close = changed_mgp.find("/>", actor);
        require(probability_offset != std::string::npos && close != std::string::npos &&
                    probability_offset < close,
                "expected the selected actor's explicit default spawn probability");
        const auto value_begin = changed_mgp.find('"', probability_offset);
        require(value_begin != std::string::npos && value_begin < close,
                "selected actor spawn probability has no value");
        const auto value_end = changed_mgp.find('"', value_begin + 1);
        require(value_end == value_begin + 4 && value_end < close &&
                    changed_mgp.compare(value_begin + 1, 3, "100") == 0,
                "expected canonical spawn probability 100");
        changed_mgp.replace(value_begin + 1, 3, "50");
        changed_bytes[2].assign(changed_mgp.begin(), changed_mgp.end());
        changed_mgps[2].data = changed_bytes[2].data();
        changed_mgps[2].size = changed_bytes[2].size();
        compiled.assign(1, 0xff);
        error.clear();
        require(!dh2::world::compile_source_dact(mlx.data(), mlx.size(), changed_mgps.data(),
                    changed_mgps.size(), characters, models, compiled, error),
                "eligible source actor with unsupported spawn_prob was accepted");
        require(compiled.empty(), "failed source DACT compilation exposed partial bytes");
        require(error.find("spawn_prob") != std::string::npos,
                "spawn probability mutation did not fail closed");

        std::cout << "source DACT adapter: byte-exact five-row match; exact MGP handle provenance; duplicate-name and spawn_prob mutations rejected\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
