#include "../source_handle_ledger_v1.hpp"
#include "../object_manager_runtime_owner_v1.hpp"
#include "../crypt_generated_source_dact_v1.hpp"
#include "../crypt_generated_mvp_v1.hpp"
#include "../../world-data/world.hpp"

#include "../../game-data/data.hpp"
#include "../../random-level/crypt_level_generator_v1.hpp"
#include "../../random-level/crypt_module_catalog_v1.hpp"
#include "../../random-level/native_rule_plan_v1.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <stdexcept>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace ledger = dh2::world::source_handle_ledger_v1;
namespace random_level = dh2::random_level;

namespace {

using Bytes = std::vector<std::uint8_t>;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

Bytes read_bytes(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open: " + path.string());
    return {std::istreambuf_iterator<char>(file), {}};
}

std::string read_text(const std::filesystem::path& path) {
    const auto bytes = read_bytes(path);
    return {bytes.begin(), bytes.end()};
}

constexpr char source_level[] =
    "<Level>"
    "<GameObject name=\"config\" gametype=\"LevelConfig\"/>"
    "<GameObject name=\"room_a\" gametype=\"Module\" "
      "dae=\"data/3d/modules/crypt/crypt.bdae\" "
      "mgp=\"data/3d/modules/crypt/mgp/a.mgp\" "
      "mvp=\"data/3d/modules/crypt/mvp/a.mvp\" "
      "xrefobject=\"_room_a\" position=\"0,0,0\" "
      "rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "<GameObject name=\"room_b\" gametype=\"Module\" "
      "dae=\"data/3d/modules/crypt/crypt.bdae\" "
      "mgp=\"data/3d/modules/crypt/mgp/b.mgp\" "
      "mvp=\"data/3d/modules/crypt/mvp/b.mvp\" "
      "xrefobject=\"_room_b\" position=\"100,0,0\" "
      "rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "</Level>";

constexpr char mgp_a[] =
    "<Module>"
    "<GameObject name=\"_shared\" gametype=\"Character\" "
      "position=\"1,2,3\" rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "<GameObject name=\"room_a\" gametype=\"Dummy\" "
      "position=\"2,3,4\" rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "</Module>";
constexpr char mvp_a[] =
    "<Module>"
    "<GameObject name=\"_shared\" gametype=\"Decor\" "
      "position=\"4,5,6\" rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "</Module>";
constexpr char mgp_b[] =
    "<Module>"
    "<GameObject name=\"_shared\" gametype=\"Monster\" "
      "position=\"7,8,9\" rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "<GameObject name=\"_other\" gametype=\"Monster\" "
      "position=\"10,11,12\" rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "</Module>";
constexpr char mvp_b[] =
    "<Module>"
    "<GameObject name=\"_prop\" gametype=\"Decor\" "
      "position=\"13,14,15\" rotation=\"0,0,0\" scale=\"1,1,1\"/>"
    "</Module>";

ledger::Ledger build_duplicate_fixture(std::uint32_t first_handle,
                                       std::uint32_t offset) {
    const std::array<ledger::OrderedModuleFile, 4> files{{
        {0, ledger::ModuleFileKind::mgp,
         "data/3d/modules/crypt/mgp/a.mgp",
         reinterpret_cast<const std::uint8_t*>(mgp_a), sizeof(mgp_a) - 1},
        {0, ledger::ModuleFileKind::mvp,
         "data/3d/modules/crypt/mvp/a.mvp",
         reinterpret_cast<const std::uint8_t*>(mvp_a), sizeof(mvp_a) - 1},
        {1, ledger::ModuleFileKind::mgp,
         "data/3d/modules/crypt/mgp/b.mgp",
         reinterpret_cast<const std::uint8_t*>(mgp_b), sizeof(mgp_b) - 1},
        {1, ledger::ModuleFileKind::mvp,
         "data/3d/modules/crypt/mvp/b.mvp",
         reinterpret_cast<const std::uint8_t*>(mvp_b), sizeof(mvp_b) - 1},
    }};
    ledger::Ledger output;
    std::string error;
    const auto status = ledger::build(
        reinterpret_cast<const std::uint8_t*>(source_level),
        sizeof(source_level) - 1, "CRYPT_TEST", "data/scene/generated.mlx",
        files.data(), files.size(), first_handle, offset, output, error);
    require(status == ledger::Status::ok, error.c_str());
    return output;
}

void check_duplicate_and_offset_behavior() {
    const auto result = build_duplicate_fixture(0, 3);
    require(result.initial_top_level_handle_offset == 3,
            "explicit top-level offset was not retained");
    require(result.occurrences.size() == 9 && result.entries.size() == 6,
            "duplicate fixture occurrence/map counts differ");
    require(result.next_handle_after == 6,
            "duplicate names incorrectly advanced the source counter");

    require(result.occurrences[0].source.gametype == "LevelConfig" &&
                result.occurrences[0].handle == 0 &&
                result.occurrences[1].source.gametype == "Module" &&
                result.occurrences[1].handle == 1 &&
                result.occurrences[2].source.gametype == "Module" &&
                result.occurrences[2].handle == 2,
            "top-level LevelConfig/Module registrations were not ordered first");
    const auto& first = result.occurrences[3];
    const auto& duplicate_in_visual = result.occurrences[5];
    const auto& duplicate_in_next_gameplay = result.occurrences[6];
    require(first.handle == 3 && !first.duplicate_name &&
                first.source.module_index == 0 && first.source.record_index == 0 &&
                first.source.kind == dh2::world::RecordKind::mgp &&
                first.source.name == "_shared" &&
                first.source.gametype == "Character",
            "first MGP object provenance/key differs");
    require(duplicate_in_visual.handle == 3 &&
                duplicate_in_visual.duplicate_name &&
                duplicate_in_visual.source.kind == dh2::world::RecordKind::mvp &&
                duplicate_in_visual.source.gametype == "Decor",
            "MVP duplicate did not reuse its source handle");
    require(duplicate_in_next_gameplay.handle == 3 &&
                duplicate_in_next_gameplay.duplicate_name &&
                duplicate_in_next_gameplay.source.module_index == 1 &&
                duplicate_in_next_gameplay.source.module_name == "room_b" &&
                duplicate_in_next_gameplay.source.gametype == "Monster",
            "later MGP duplicate did not resolve to the existing map handle");

    const auto& shared = result.entries[3];
    require(shared.handle == 3 && shared.lookup_count == 3 &&
                shared.retained_source.module_name == "room_a" &&
                shared.retained_source.kind == dh2::world::RecordKind::mgp &&
                shared.retained_source.gametype == "Character",
            "duplicate candidates replaced the original ObjectManager value");
    require(result.entries[1].handle == 1 &&
                result.entries[1].lookup_count == 2 &&
                result.entries[1].retained_source.name == "room_a" &&
                result.entries[1].retained_source.kind ==
                    dh2::world::RecordKind::level &&
                result.entries[1].retained_source.gametype == "Module" &&
                result.occurrences[4].duplicate_name &&
                result.occurrences[4].source.kind == dh2::world::RecordKind::mgp &&
                result.entries[4].handle == 4 &&
                result.entries[4].retained_source.name == "_other" &&
                result.entries[5].handle == 5 &&
                result.entries[5].retained_source.name == "_prop",
            "new names did not receive consecutive map keys");

    const std::array<ledger::OrderedModuleFile, 4> invalid_files{{
        {0, ledger::ModuleFileKind::mgp,
         "data/3d/modules/crypt/mgp/a.mgp",
         reinterpret_cast<const std::uint8_t*>(mgp_a), sizeof(mgp_a) - 1},
        {0, ledger::ModuleFileKind::mgp,
         "data/3d/modules/crypt/mvp/a.mvp",
         reinterpret_cast<const std::uint8_t*>(mvp_a), sizeof(mvp_a) - 1},
        {1, ledger::ModuleFileKind::mgp,
         "data/3d/modules/crypt/mgp/b.mgp",
         reinterpret_cast<const std::uint8_t*>(mgp_b), sizeof(mgp_b) - 1},
        {1, ledger::ModuleFileKind::mvp,
         "data/3d/modules/crypt/mvp/b.mvp",
         reinterpret_cast<const std::uint8_t*>(mvp_b), sizeof(mvp_b) - 1},
    }};
    ledger::Ledger rejected;
    rejected.entries.push_back({});
    std::string error;
    const auto rejected_status = ledger::build(
        reinterpret_cast<const std::uint8_t*>(source_level),
        sizeof(source_level) - 1, "CRYPT_TEST", "data/scene/generated.mlx",
        invalid_files.data(), invalid_files.size(), 0, 3, rejected, error);
    require(rejected_status == ledger::Status::invalid_file_order &&
                rejected.entries.empty() && rejected.occurrences.empty() &&
                !error.empty(),
            "same-kind Module files were accepted or partial output escaped");

    const std::array<ledger::OrderedModuleFile, 4> valid_files{{
        {0, ledger::ModuleFileKind::mgp,
         "data/3d/modules/crypt/mgp/a.mgp",
         reinterpret_cast<const std::uint8_t*>(mgp_a), sizeof(mgp_a) - 1},
        {0, ledger::ModuleFileKind::mvp,
         "data/3d/modules/crypt/mvp/a.mvp",
         reinterpret_cast<const std::uint8_t*>(mvp_a), sizeof(mvp_a) - 1},
        {1, ledger::ModuleFileKind::mgp,
         "data/3d/modules/crypt/mgp/b.mgp",
         reinterpret_cast<const std::uint8_t*>(mgp_b), sizeof(mgp_b) - 1},
        {1, ledger::ModuleFileKind::mvp,
         "data/3d/modules/crypt/mvp/b.mvp",
         reinterpret_cast<const std::uint8_t*>(mvp_b), sizeof(mvp_b) - 1},
    }};
    auto reversed_files = valid_files;
    std::swap(reversed_files[0], reversed_files[1]);
    ledger::Ledger reversed_rejected;
    error.clear();
    const auto reversed_status = ledger::build(
        reinterpret_cast<const std::uint8_t*>(source_level),
        sizeof(source_level) - 1, "CRYPT_TEST", "data/scene/generated.mlx",
        reversed_files.data(), reversed_files.size(), 0, 3,
        reversed_rejected, error);
    require(reversed_status == ledger::Status::invalid_file_order &&
                reversed_rejected.entries.empty() &&
                reversed_rejected.occurrences.empty() && !error.empty(),
            "MVP-before-MGP load order was accepted");

    ledger::Ledger offset_rejected;
    error.clear();
    const auto offset_status = ledger::build(
        reinterpret_cast<const std::uint8_t*>(source_level),
        sizeof(source_level) - 1, "CRYPT_TEST", "data/scene/generated.mlx",
        valid_files.data(), valid_files.size(),
        0,
        static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()),
        offset_rejected, error);
    require(offset_status == ledger::Status::invalid_top_level_offset &&
                offset_rejected.entries.empty() &&
                offset_rejected.occurrences.empty() &&
                !error.empty(),
            "invalid top-level offset was not rejected atomically");

    const auto one_based = build_duplicate_fixture(1, 3);
    require(one_based.first_source_handle == 1 &&
                one_based.occurrences[0].handle == 1 &&
                one_based.occurrences[1].handle == 2 &&
                one_based.occurrences[2].handle == 3 &&
                one_based.occurrences[3].handle == 4 &&
                one_based.next_handle_after == 7,
            "nonzero initial source handle was not applied to the full load order");
}

void check_crypt_generated(const std::filesystem::path& rule_path,
                           const std::filesystem::path& cache_root,
                           const std::filesystem::path& assets_root) {
    const auto rule_xml = read_text(rule_path);
    const auto parsed = random_level::parse_crypt_rule_v1(rule_xml);
    require(static_cast<bool>(parsed), "actual Crypt rule XML failed to parse");

    const auto base = cache_root / "data" / "3d" / "modules" / "crypt";
    const random_level::CryptModuleAssetPathsV1 paths{
        base / "mgx", base / "mgp", base / "mvp", base / "mvx",
        base / "mgx" / "mgxlist.txt"};
    const auto catalogue = random_level::build_crypt_module_catalogue_v1(
        *parsed.document, paths);
    require(catalogue.catalogue_issues.empty() &&
                catalogue.unresolved_candidate_count() == 0,
            "actual Crypt catalogue did not resolve all active candidates");

    constexpr std::uint32_t seed = 0x00C0FFEEU;
    const auto generated = random_level::generate_crypt_level_v1(
        *parsed.document, catalogue, seed);
    require(generated.status == random_level::CryptLevelGeneratorStatusV1::success,
            "actual Crypt generator failed to produce its Level XML");
    require(!generated.modules.empty() &&
                generated.layout.source_level_xml.size() > 0,
            "actual Crypt generator returned no modules or Level XML");

    std::vector<std::string> file_paths;
    std::vector<Bytes> file_bytes;
    std::vector<ledger::OrderedModuleFile> files;
    const auto module_file_count = generated.modules.size() * 2;
    file_paths.reserve(module_file_count);
    file_bytes.reserve(module_file_count);
    files.reserve(module_file_count);
    for (std::size_t index = 0; index < generated.modules.size(); ++index) {
        const auto& module = generated.modules[index];
        const std::array<std::pair<ledger::ModuleFileKind, std::string>, 2> refs{{
            {ledger::ModuleFileKind::mgp,
             "data/3d/modules/crypt/mgp/" + module.gameplay},
            {ledger::ModuleFileKind::mvp,
             "data/3d/modules/crypt/mvp/" + module.visual},
        }};
        for (const auto& ref : refs) {
            file_paths.push_back(ref.second);
            file_bytes.push_back(read_bytes(cache_root / ref.second));
            files.push_back({static_cast<std::uint32_t>(index), ref.first,
                             file_paths.back().c_str(), file_bytes.back().data(),
                             file_bytes.back().size()});
        }
    }

    ledger::Ledger result;
    std::string error;
    const auto expected_root_offset =
        static_cast<std::uint32_t>(1 + generated.modules.size());
    const auto status = ledger::build(
        reinterpret_cast<const std::uint8_t*>(
            generated.layout.source_level_xml.data()),
        generated.layout.source_level_xml.size(), "CRYPT_GENERATED",
        "data/scene/generated_crypt.mlx", files.data(), files.size(),
        4,
        expected_root_offset,
        result, error);
    require(status == ledger::Status::ok, error.c_str());
    require(result.initial_top_level_handle_offset == expected_root_offset &&
                result.next_handle_after >= 0 &&
                result.occurrences.size() > generated.modules.size() &&
                !result.entries.empty(),
            "generated Crypt ledger did not enumerate source Module objects");
    require(result.first_source_handle == 4 &&
                result.occurrences.front().handle == 4 &&
                result.occurrences.front().source.kind ==
                    dh2::world::RecordKind::level &&
                result.occurrences.front().source.gametype == "LevelConfig" &&
                result.occurrences[expected_root_offset].source.module_index == 0 &&
                result.occurrences[expected_root_offset].source.record_index == 0 &&
                result.occurrences[expected_root_offset].handle ==
                    static_cast<std::int32_t>(4 + expected_root_offset),
            "generated Crypt root/module object ordering differs");

    std::map<std::string, const ledger::Occurrence*, std::less<>> first_by_name;
    std::map<std::string, std::int32_t, std::less<>> first_handle_by_name;
    std::int32_t next_unique = 4;
    for (const auto& occurrence : result.occurrences) {
        const bool root_object =
            occurrence.source.kind == dh2::world::RecordKind::level;
        require((root_object ||
                    (occurrence.source.module_index < generated.modules.size() &&
                     !occurrence.source.module_name.empty())) &&
                    !occurrence.source.name.empty() &&
                    !occurrence.source.gametype.empty(),
                "generated Crypt source provenance is incomplete");
        const auto inserted = first_handle_by_name.emplace(
            occurrence.source.name, next_unique);
        if (inserted.second) ++next_unique;
        require(occurrence.handle == inserted.first->second &&
                    occurrence.duplicate_name == !inserted.second,
                "generated Crypt duplicate-name key semantics differ");
        if (inserted.second) first_by_name[occurrence.source.name] = &occurrence;
    }
    require(result.entries.size() == first_handle_by_name.size() &&
                result.next_handle_after == next_unique &&
                result.next_handle_after ==
                    4 + static_cast<std::int64_t>(result.entries.size()),
                "generated Crypt final map size/counter differs from name allocation");
    for (const auto& entry : result.entries) {
        const auto found = first_by_name.find(entry.retained_source.name);
        require(found != first_by_name.end() &&
                    found->second->handle == entry.handle &&
                    found->second->source.module_index ==
                        entry.retained_source.module_index &&
                    found->second->source.kind == entry.retained_source.kind &&
                    found->second->source.record_index ==
                        entry.retained_source.record_index &&
                    entry.lookup_count > 0,
                "generated Crypt map entry does not retain its first object");
    }

    // Carry the real generated-level registration ledger into the selected
    // ObjectManager owner, then append Module::InitPost RoomZones and the
    // five ItemObject shells created for one PreCache category. The zone and
    // shell identities/names below are fixture tokens only; their integer
    // source keys come exclusively from the live ledger/manager counter.
    using RuntimeOwner = dh2::object_manager_runtime_owner_v1::Owner;
    using RuntimeObject = dh2::object_manager_runtime_owner_v1::GameObject;
    using RuntimeStatus = dh2::object_manager_runtime_owner_v1::Status;
    RuntimeOwner runtime_manager;
    std::uintptr_t fixture_identity = 0x100000u;
    for (const auto& entry : result.entries) {
        RuntimeObject object{};
        object.identity = fixture_identity++;
        RuntimeObject* retained = nullptr;
        require(runtime_manager.add_named_object(entry.handle,
                    entry.retained_source.name, object, &retained) ==
                    RuntimeStatus::ok && retained &&
                    retained->source_handle == entry.handle,
                "runtime ObjectManager did not retain the generated source key");
    }
    require(runtime_manager.next_source_handle() == result.next_handle_after,
            "runtime counter did not continue after generated source registrations");
    require(generated.modules.size() == 7,
            "seeded Crypt module count differs from the RoomZone fixture premise");
    for (std::size_t index = 0; index < generated.modules.size(); ++index) {
        const auto zone_handle = runtime_manager.next_source_handle();
        RuntimeObject zone{};
        zone.identity = fixture_identity++;
        RuntimeObject* retained = nullptr;
        require(runtime_manager.add_object(zone_handle, zone, &retained) ==
                    RuntimeStatus::ok && retained &&
                    retained->source_handle == zone_handle,
                "Module::InitPost RoomZone did not consume the next source key");
    }
    const auto first_shell_handle = runtime_manager.next_source_handle();
    for (std::size_t slot = 0; slot < 5; ++slot) {
        const auto shell_name = "ItemManager::PreCache fixture shell " +
                                std::to_string(slot);
        std::int32_t shell_handle = -1;
        bool created = false;
        require(runtime_manager.get_or_reserve_named_handle(shell_name, true,
                    &shell_handle, &created) == RuntimeStatus::ok && created &&
                    shell_handle == first_shell_handle +
                        static_cast<std::int32_t>(slot),
                "ItemManager PreCache fixture did not consume consecutive source keys");
        RuntimeObject shell{};
        shell.identity = fixture_identity++;
        RuntimeObject* retained = nullptr;
        require(runtime_manager.add_named_object(shell_handle, shell_name,
                    shell, &retained) == RuntimeStatus::ok && retained,
                "ItemManager PreCache fixture failed to publish its reserved shell");
    }
    require(runtime_manager.next_source_handle() == first_shell_handle + 5,
            "ItemManager shell registrations did not advance the canonical counter");

    const auto character_bytes = read_bytes(
        assets_root / "data" / "character_properties_pyarray.bin");
    const auto character_names = read_bytes(
        assets_root / "data" / "character_properties_pyarraynames.bin");
    const auto character_fields = read_bytes(
        assets_root / "data" / "character_properties_pystructnames.bin");
    const auto model_names = read_bytes(
        assets_root / "data" / "character_models_dictionary_pyarraynames.bin");
    const auto model_values = read_bytes(
        assets_root / "data" / "character_models_dictionary_pyarray.bin");
    dh2::data::CharacterTable characters;
    dh2::data::Dictionary models;
    std::string data_error;
    require(dh2::data::load_characters(
        {character_bytes.data(), character_bytes.size()},
        {character_names.data(), character_names.size()},
        {character_fields.data(), character_fields.size()}, characters,
        data_error), data_error.c_str());
    require(dh2::data::load_dictionary(
        {model_names.data(), model_names.size()},
        {model_values.data(), model_values.size()}, models, data_error),
        data_error.c_str());

    std::vector<std::string> mgp_paths;
    std::vector<dh2::world::GeneratedMgpView> mgps;
    mgp_paths.reserve(generated.modules.size());
    mgps.reserve(generated.modules.size());
    for (std::size_t index = 0; index < generated.modules.size(); ++index) {
        const auto file_index = index * 2;
        mgp_paths.push_back(file_paths[file_index]);
        mgps.push_back({mgp_paths.back().c_str(), file_bytes[file_index].data(),
                        file_bytes[file_index].size()});
    }
    std::vector<dh2::world::GeneratedCryptActorHandleV1> actor_sources;
    std::vector<std::uint8_t> dact;
    std::size_t actor_count = 0;
    std::size_t deferred_count = 0;
    require(dh2::world::compile_generated_crypt_dact_v1(
        reinterpret_cast<const std::uint8_t*>(
            generated.layout.source_level_xml.data()),
        generated.layout.source_level_xml.size(), "CRYPT_GENERATED",
        "data/scene/generated_crypt.mlx", mgps.data(), mgps.size(),
        characters, models, result, actor_sources, dact, actor_count,
        deferred_count, error), error.c_str());
    require(actor_count == actor_sources.size() &&
                dact.size() == 16 + actor_count * 256 &&
                std::memcmp(dact.data(), "DACT", 4) == 0,
            "Ledger-backed DACT count or record size differs");
    std::map<std::pair<std::uint32_t, std::uint32_t>, std::int32_t> actor_handles;
    for (std::size_t index = 0; index < actor_sources.size(); ++index) {
        const auto& actor = actor_sources[index];
        require(actor.dact_record == index && actor.source_handle >= 0 &&
                    actor_handles.emplace(
                        std::make_pair(actor.module_index, actor.source_record),
                        actor.source_handle).second,
                "Ledger-backed DACT actor source mapping is duplicated or unordered");
        const auto occurrence = std::find_if(result.occurrences.begin(),
            result.occurrences.end(), [&](const auto& candidate) {
                return candidate.source.kind == dh2::world::RecordKind::mgp &&
                    candidate.source.module_index == actor.module_index &&
                    candidate.source.record_index == actor.source_record &&
                    candidate.source.name == actor.name;
            });
        require(occurrence != result.occurrences.end() &&
                    !occurrence->duplicate_name &&
                    occurrence->handle == actor.source_handle,
                "DACT actor does not carry the retained source ObjectManager handle");
    }
    constexpr std::array<std::pair<std::uint32_t, std::uint32_t>, 3>
        discarded_direct_monsters{{{6, 4}, {6, 7}, {6, 8}}};
    for (const auto& discarded : discarded_direct_monsters) {
        require(actor_handles.find(discarded) == actor_handles.end(),
                "ObjectManager-discarded Monster candidate remained in generated DACT");
    }

    std::vector<std::string> mvp_paths;
    std::vector<dh2::world::GeneratedCryptMvpFileV1> mvps;
    mvp_paths.reserve(generated.modules.size());
    mvps.reserve(generated.modules.size());
    for (std::size_t index = 0; index < generated.modules.size(); ++index) {
        const auto file_index = index * 2 + 1;
        mvp_paths.push_back(file_paths[file_index]);
        mvps.push_back({mvp_paths.back().c_str(), file_bytes[file_index].data(),
                        file_bytes[file_index].size()});
    }
    std::vector<dh2::world::GeneratedCryptAnimatedDecorV1> animated_decors;
    std::size_t deferred_visual_count = 0;
    require(dh2::world::compile_generated_crypt_animated_decor_v1(
        reinterpret_cast<const std::uint8_t*>(
            generated.layout.source_level_xml.data()),
        generated.layout.source_level_xml.size(), "CRYPT_GENERATED",
        "data/scene/generated_crypt.mlx", mvps.data(), mvps.size(), result,
        animated_decors, deferred_visual_count, error), error.c_str());

    std::size_t expected_animated_decor_count = 0;
    std::size_t expected_deferred_visual_count = 0;
    for (const auto& occurrence : result.occurrences) {
        if (occurrence.source.kind != dh2::world::RecordKind::mvp ||
            occurrence.source.gametype != "AnimatedDecor") continue;
        if (occurrence.duplicate_name) ++expected_deferred_visual_count;
        else ++expected_animated_decor_count;
    }
    require(animated_decors.size() == expected_animated_decor_count &&
                deferred_visual_count == expected_deferred_visual_count,
            "Crypt AnimatedDecor rows differ from retained ObjectManager MVP registrations");

    std::set<std::int32_t> visual_handles;
    std::pair<std::uint32_t, std::uint32_t> previous_visual{};
    bool has_previous_visual = false;
    for (const auto& decor : animated_decors) {
        const auto location = std::make_pair(decor.module_index,
                                             decor.source_record);
        require(!has_previous_visual || previous_visual < location,
                "Crypt AnimatedDecor projection is not in source file order");
        previous_visual = location;
        has_previous_visual = true;
        const auto occurrence = std::find_if(result.occurrences.begin(),
            result.occurrences.end(), [&](const auto& candidate) {
                return candidate.source.kind == dh2::world::RecordKind::mvp &&
                    candidate.source.module_index == decor.module_index &&
                    candidate.source.record_index == decor.source_record &&
                    candidate.source.name == decor.name &&
                    candidate.source.gametype == "AnimatedDecor";
            });
        require(occurrence != result.occurrences.end() &&
                    !occurrence->duplicate_name &&
                    occurrence->handle == decor.source_handle &&
                    visual_handles.insert(decor.source_handle).second,
                "Crypt AnimatedDecor lost its unique retained source handle");
        require(!decor.name.empty() && !decor.xrefobject.empty() &&
                    !decor.source_path.empty() &&
                    decor.start_animation == "idle" &&
                    decor.dae_path.rfind("data/3d/animateddecors/", 0) == 0,
                "Crypt AnimatedDecor source identity/root/DAE is incomplete");
        const auto model_name = decor.dae_path.substr(
            decor.dae_path.find_last_of('/') + 1);
        require(std::filesystem::exists(assets_root / "actors" / model_name),
                "Crypt AnimatedDecor BDAE is not packaged in the app assets");
        for (unsigned axis = 0; axis < 3; ++axis) {
            require(std::isfinite(decor.local_transform[axis]) &&
                        std::isfinite(decor.world_transform[axis]) &&
                        std::isfinite(decor.world_transform[3 + axis]) &&
                        std::isfinite(decor.world_transform[6 + axis]) &&
                        decor.world_transform[6 + axis] > 0.f,
                    "Crypt AnimatedDecor has an invalid local/world transform");
        }
    }

    std::cout << "Crypt generated handle ledger: modules="
              << generated.modules.size() << " object loads="
              << result.occurrences.size() << " final handles="
              << result.entries.size() << " duplicate candidates discarded="
              << (result.occurrences.size() - result.entries.size())
              << " first source handle=" << result.first_source_handle
              << " explicit root-count offset="
              << result.initial_top_level_handle_offset
              << " retained DACT Monsters=" << actor_count
              << " source records deferred/skipped by actor projection="
              << deferred_count << " retained MVP AnimatedDecor="
              << animated_decors.size() << " duplicate visual candidates omitted="
              << deferred_visual_count << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "usage: source_handle_ledger_v1_audit "
                     "<007_crypt_01.rule.xml> <original-cache-root> "
                     "<android-assets-root>\n";
        return 2;
    }
    try {
        check_duplicate_and_offset_behavior();
        check_crypt_generated(argv[1], argv[2], argv[3]);
        std::cout << "source handle ledger regression passed\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
