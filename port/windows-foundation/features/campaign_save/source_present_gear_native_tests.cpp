// Reuse the real source PlayerGear cache/Text fixture, then provide the same
// source design, asset and explicit World projections used by the Gear harness.
#define main frozen_v5_cache_fixture_main_present_gear
#include "../../../game-data/tests/player_gear_cache_v5.cpp"
#undef main

#include "../../../level-world/character_profile_bootstrap_v59.hpp"
#include "../../../level-world/player_save_inventory_writer_v45.hpp"
#include "../../../game-data/fresh_player_profile_v1.hpp"
#include "../../../level-world/character_menu_quests_v51.hpp"
#include "../../../game-data/quest_persistence_v51.hpp"
#include "../../../level-world/savegame_jobs_owner_v2.hpp"
#include "../../../level-world/character_menu_mutations_v4.hpp"
#include "../../../game-data/item_presentation_v5.hpp"
#include <map>
#include <memory>
#include <filesystem>

namespace {
using namespace dh2;
using namespace dh2::player;

struct DesignInput {
    std::array<std::array<Raw,3>,5> tables;
    std::vector<Raw> constants;
    std::vector<Bytes> constant_views;
    character::GameDesignInputs256 view;
};
static std::string read_design_string(Reader& reader) {
    const auto size = reader.u();
    std::string value(size, '\0');
    reader.copy(value.data(), size);
    return value;
}
static Raw read_design_blob(Reader& reader) {
    const auto size = reader.u();
    Raw value(size);
    reader.copy(value.data(), size);
    return value;
}
static DesignInput design_input(const char* path) {
    const auto bytes = file(path);
    Reader reader{bytes};
    ck(reader.u() == 0x314f4447);
    DesignInput result;
    for (auto& table : result.tables) for (auto& part : table) part = read_design_blob(reader);
    auto count = reader.u();
    for (unsigned i=0; i<count; ++i) { (void)read_design_string(reader); result.constants.push_back(read_design_blob(reader)); }
    count = reader.u();
    for (unsigned i=0; i<count; ++i) (void)read_design_string(reader);
    ck(reader.at == bytes.size());
    return result;
}
static void views(DesignInput& input) {
    character::GameDesignTableInput48* tables[]{&input.view.characters, &input.view.classes,
        &input.view.ai, &input.view.factions, &input.view.levels};
    for (unsigned i=0; i<5; ++i) *tables[i] = {{input.tables[i][0].data(),input.tables[i][0].size()},
        {input.tables[i][1].data(),input.tables[i][1].size()},
        {input.tables[i][2].data(),input.tables[i][2].size()}};
    input.constant_views.clear();
    for (auto& blob : input.constants) input.constant_views.push_back({blob.data(),blob.size()});
    input.view.constants = input.constant_views.data();
    input.view.constant_count = input.constant_views.size();
}
struct Platform {
    TextEnvironment env;
    std::string items, powers, weapons;
    unsigned assets{}, world_queries{}, notifications{}, saved_power_calls{}, gold_notification_calls{};
    bool asset_failure{}, reentry{};
    std::uintptr_t local_character{};
    data::ItemPresentationOwnerV5* presentation{};
    data::ItemTextServicesV5 item_text{};
    player::PlayerEquipmentRenderOwnerV1* owner{};
};
static skinning::VisualAssetResultV6 asset(void* raw, const char* name, Raw& bytes, std::string& error) {
    auto& platform = *static_cast<Platform*>(raw);
    ++platform.assets;
    if (platform.asset_failure) { error = "Deliberate actual asset provider rejection"; return skinning::VisualAssetResultV6::failed; }
    const std::string uri(name);
    std::string path;
    if (uri.find("data/pydata/loot_table_") == 0) path = platform.items + "/" + uri.substr(12);
    else if (uri.find("data/pydata/item_powers_") == 0) path = platform.powers + "/" + uri.substr(12);
    else if (uri.find("data/3d/characters/prince/weapons/") == 0) {
        auto base = uri.substr(uri.find_last_of('/') + 1);
        for (auto& ch : base) ch = char(std::tolower(static_cast<unsigned char>(ch)));
        path = platform.weapons + "/" + base;
    } else path = platform.env.assets + "/original-cache/" + uri;
    if (!std::filesystem::exists(path)) return skinning::VisualAssetResultV6::missing;
    bytes = file(path);
    return skinning::VisualAssetResultV6::found;
}
static bool world(void* raw, player::EquipmentWorldQueryV1 query, std::uintptr_t subject,
                  std::uintptr_t& identity, std::int32_t& value, std::string&) {
    auto& platform = *static_cast<Platform*>(raw);
    ck(subject == 0x100000001ULL);
    ++platform.world_queries;
    identity = 0; value = 0;
    if (query == player::EquipmentWorldQueryV1::current_player) identity = subject;
    if (query == player::EquipmentWorldQueryV1::player_count) value = 1;
    if (platform.reentry && platform.owner) { std::string nested; ck(!platform.owner->swap(nested)); }
    return true;
}
static bool required(void* raw, data::FreshInventoryOwnedV4& inventory, const data::OwnedInventoryRequestV4& request,
                     data::OwnedInventoryResponseV4&, std::string& error) {
    auto& platform = *static_cast<Platform*>(raw);
    ++platform.notifications;
    if (request.operation == data::OwnedInventoryOperationV4::add_power) {
        ++platform.saved_power_calls;
        if (!platform.presentation || !request.item || !platform.item_text.invoke) {
            error = "Required SAME source ItemPower/Text providers"; return false;
        }
        return platform.presentation->add_power(*request.item, request.argument,
            static_cast<std::int32_t>(request.index), platform.item_text, error);
    }
    if (request.operation == data::OwnedInventoryOperationV4::gold_notifications) {
        ++platform.gold_notification_calls;
        character::CharacterMenuMutationServicesV4 services;
        services.owner = std::make_shared<int>(1);
        services.is_player = [&platform](std::uintptr_t actor, bool& value, std::string& e) {
            value = actor == platform.local_character; e.clear(); return true;
        };
        services.is_local_player = [&platform](std::uintptr_t actor, bool& value, std::string& e) {
            value = actor == platform.local_character; e.clear(); return true;
        };
        services.achievement = [](std::uintptr_t, const char*, std::string& e) {
            e = "Reached source achievement continuation unavailable"; return false;
        };
        return character::character_menu_gold_notifications_v4(
            inventory, services, error);
    }
    error = "Required remaining source continuation unavailable: operation=" +
        std::to_string(unsigned(request.operation)) + " caller=" + std::to_string(request.source_caller);
    return false;
}

struct MemoryFiles : std::enable_shared_from_this<MemoryFiles> {
    struct Output { std::string name; std::vector<std::uint8_t> bytes; std::size_t offset{}; };
    std::map<std::string, std::vector<std::uint8_t>> files;

    static bool read(void* raw, const std::string& name, bool& found,
                     std::vector<std::uint8_t>& bytes, std::string& error) {
        auto& self = *static_cast<MemoryFiles*>(raw);
        auto it = self.files.find(name);
        found = it != self.files.end();
        bytes = found ? it->second : std::vector<std::uint8_t>{};
        error.clear();
        return true;
    }
    static bool backup(void* raw, const std::string& from, const std::string& to,
                       bool& result, std::string& error) {
        auto& self = *static_cast<MemoryFiles*>(raw);
        auto it = self.files.find(from);
        if (it == self.files.end()) { result = false; error.clear(); return true; }
        self.files.erase(to);
        self.files[to] = std::move(it->second);
        self.files.erase(it);
        result = true;
        error.clear();
        return true;
    }
    static bool open(void* raw, const std::string& name, void*& out, std::string& error) {
        (void)raw;
        out = new Output{name, {}, 0};
        error.clear();
        return true;
    }
    static bool write(void*, void* raw, data::Bytes bytes, std::uint64_t& written,
                      std::string& error) {
        auto* out = static_cast<Output*>(raw);
        if (!out || (!bytes.data && bytes.size)) { error = "Malformed in-memory save write"; return false; }
        if (out->offset + bytes.size > out->bytes.size()) out->bytes.resize(out->offset + bytes.size);
        if (bytes.size) std::memcpy(out->bytes.data() + out->offset, bytes.data, bytes.size);
        out->offset += bytes.size;
        written = bytes.size;
        error.clear();
        return true;
    }
    static bool seek(void*, void* raw, std::uint64_t offset, std::string& error) {
        auto* out = static_cast<Output*>(raw);
        if (!out || offset > std::numeric_limits<std::size_t>::max()) {
            error = "Malformed in-memory save seek"; return false;
        }
        out->offset = static_cast<std::size_t>(offset);
        error.clear();
        return true;
    }
    static bool close(void* raw, void*& handle, std::string& error) {
        auto& self = *static_cast<MemoryFiles*>(raw);
        auto* out = static_cast<Output*>(handle);
        if (!out) { error = "Missing in-memory save stream"; return false; }
        self.files[out->name] = std::move(out->bytes);
        delete out;
        handle = nullptr;
        error.clear();
        return true;
    }
    static bool failure(void*, const char* reason, std::string& error) {
        error = reason ? reason : "In-memory source save failure";
        return false;
    }
    level::SavegameFileServicesV2 services() {
        return {this, read, backup, open, write, seek, close, failure, shared_from_this()};
    }
};

struct ProfileGraph {
    std::shared_ptr<data::PlayerSavegameV1> save;
    std::shared_ptr<data::PlayerSaveLoadOwnerV1> load;
    std::shared_ptr<character::CharacterMenuQuestsV51> quests;
    std::shared_ptr<character::CharacterProfileBootstrapV59> bootstrap;
    std::shared_ptr<void> cells_lease = std::make_shared<int>(1);
    std::uintptr_t save14e8{};
    std::int32_t slot664{};
    std::vector<std::int32_t> level_defaults, map_defaults;
};

void check(bool condition, const std::string& label) {
    ++checks;
    if (!condition) throw std::runtime_error(label);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("Required actual source fixture " + path);
    return {std::istreambuf_iterator<char>(input), {}};
}

std::shared_ptr<ProfileGraph> make_profile_graph(
    std::uintptr_t character, const std::shared_ptr<level::CampaignSaveProfileV45>& profile,
    const std::shared_ptr<const std::vector<std::uint8_t>>& selected_bytes,
    const std::shared_ptr<data::PlayerSavegameV1>& same_save,
    const std::shared_ptr<data::QuestTablesPersistenceV51>& quest_tables,
    const data::CharacterTable& characters, data::SkillTables::Borrow skills,
    const std::shared_ptr<void>& tables_lease, bool fail_after_gear) {
    auto graph = std::make_shared<ProfileGraph>();
    graph->save = same_save ? same_save : std::make_shared<data::PlayerSavegameV1>();
    if (!same_save) graph->save->set_character(character);
    graph->load = std::make_shared<data::PlayerSaveLoadOwnerV1>(graph->save);
    graph->quests = std::make_shared<character::CharacterMenuQuestsV51>(graph->save, quest_tables);
    graph->save14e8 = reinterpret_cast<std::uintptr_t>(graph->save.get());

    character::CharacterProfileBootstrapInputsV59 input;
    input.save = graph->save;
    input.load = graph->load;
    input.profile = profile;
    input.selected_file_lease = selected_bytes;
    input.selected_file_bytes = {selected_bytes->data(), selected_bytes->size()};
    input.selected_slot = 0;
    input.character = character;
    input.actual_source_cells_lease = graph->cells_lease;
    input.source_save14e8 = &graph->save14e8;
    input.source_player_info_slot664 = &graph->slot664;
    std::weak_ptr<ProfileGraph> weak = graph;
    input.source_set_slot = [weak](std::int32_t slot, std::string& error) {
        auto actual = weak.lock();
        if (!actual) { error = "Expired actual profile-slot cells"; return false; }
        actual->save->set_slot(slot);
        error.clear();
        return true;
    };
    input.reads.tables = tables_lease;
    input.reads.characters = &characters;
    input.reads.skills = std::move(skills);
    input.reads.level_defaults28 = &graph->level_defaults;
    input.reads.map_defaults8 = &graph->map_defaults;
    input.reads.quests = graph->quests;
    input.reads.character_skill_list = [weak, &characters, skills = input.reads.skills](
        std::uintptr_t id, const std::vector<std::int32_t>*& rows, std::string& error) {
        auto actual = weak.lock();
        if (!actual || id != actual->save->character() || !skills) {
            error = "Required SAME Character.GetSkillsList source fixture"; return false;
        }
        (void)characters;
        rows = &skills.lists().at(0);
        error.clear();
        return true;
    };
    input.reads.store_current_difficulty = [](std::int32_t, std::string& error) {
        error.clear(); return true;
    };
    unsigned online_calls{};
    input.remaining.owner = tables_lease;
    input.remaining.invoke = [fail_after_gear, online_calls](
        const auto& request, auto& response, std::string& error) mutable {
        if (request.operation == data::PlayerSaveLoadOpV1::online) {
            ++online_calls;
            if (fail_after_gear && online_calls == 2) {
                error = "Deliberate failure after genuine source GEAR Load4 prefix";
                return false;
            }
            response.flag = false;
            error.clear();
            return true;
        }
        error = "Unexpected selected-profile source operation " +
            std::to_string(unsigned(request.operation));
        return false;
    };
    graph->bootstrap = std::make_shared<character::CharacterProfileBootstrapV59>(std::move(input));
    return graph;
}

struct ItemState {
    std::int32_t id{}, quantity{}, value{};
    std::uint8_t identified{};
    std::array<std::int8_t,2> slots{};
    std::vector<std::int32_t> powers;
    std::string name;
    bool operator==(const ItemState& other) const {
        return id == other.id && quantity == other.quantity && value == other.value &&
            identified == other.identified && slots == other.slots &&
            powers == other.powers && name == other.name;
    }
};

std::vector<ItemState> item_states(const data::FreshInventoryOwnedV4& inventory) {
    std::vector<ItemState> result;
    for (const auto& slot : inventory.items()) {
        if (!slot || !slot->item) throw std::runtime_error("Expected complete actual item row");
        const auto& item = *slot->item;
        result.push_back({item.id, item.signed_quantity(), item.value, item.identified,
                          slot->slots, item.powers, item.name});
    }
    return result;
}

int run(int argc, char** argv) {
    check(argc == 9, "eight native source fixture paths required");
    std::string error;

    auto source_design = design_input(argv[1]);
    views(source_design);
    character::CharacterGameDesign design;
    check(design.initialize(source_design.view, error), "actual source design cache");
    skinning::VisualSkinResourcesV6 resources;
    if (!resources.load(file(argv[7]), error))
        throw std::runtime_error("actual retained visual resources: " + error);

    auto gold = read_file(argv[8]);
    Reader original{gold};
    check(original.u() == 0x35564547 && original.u() == 6, "actual six-row starter/equipment fixture");
    PropertyState initial;
    bool got_class{};
    for (unsigned index=0; index<6; ++index) {
        (void)original.u();
        const auto selected = original.u();
        const auto count = original.u();
        PropertyState candidate;
        original.copy(&candidate, sizeof candidate);
        for (unsigned row=0; row<count; ++row) {
            for (unsigned word=0; word<5; ++word) (void)original.u();
            PropertyState ignored;
            original.copy(&ignored, sizeof ignored);
        }
        if (!selected && !got_class) { initial = candidate; got_class = true; }
    }
    check(got_class && original.at == gold.size(), "complete selected source class fixture");

    constexpr std::uintptr_t character = 0x100000001ULL;
    Platform platform;
    platform.local_character = character;
    platform.items = argv[2];
    platform.powers = argv[3];
    platform.env.assets = argv[4];
    platform.env.private_files = argv[5];
    platform.weapons = argv[6];
    const auto constants = read_file(platform.env.assets + "/original-cache/data/pydata/common_text_pycst.bin");
    dh2_script_constants_reload constant_receipt{};
    check(platform.env.constants && platform.env.debug &&
        !dh2_script_constants_load(platform.env.constants, constants.data(), constants.size(), &constant_receipt),
        "actual localized constants and DebugSwitches");

    auto source_properties = std::make_shared<PropertyState>(initial);
    LootRandom8V2 source_random{1,0};
    auto scene = resources.borrow().factory_scene();
    auto make_equipment_input = [&](const std::shared_ptr<PropertyState>& properties,
                                    LootRandom8V2& random) {
        PlayerEquipmentRenderInputsV1 input;
        input.design = design.borrow();
        input.properties = properties;
        input.random = &random;
        input.character = character;
        input.potion_capacity = 12;
        input.language_pack = 0;
        input.resources = resources.borrow();
        input.live_scene = &scene;
        input.assets = {&platform, asset};
        input.debug = platform.env.debug;
        input.debug_files = {&platform.env, debug_open, debug_close};
        input.text_environment.localization = {&platform.env, text_open, text_close,
            text_debug, text_constant, nullptr, nullptr};
        input.world = {&platform, ::world};
        input.required = {&platform, required};
        return input;
    };

    PlayerEquipmentRenderOwnerV1 source_gear(make_equipment_input(source_properties, source_random));
    check(source_gear.initialize(error) && source_gear.ready(), "source Gear and original initial equipment");
    data::LootTablesV2::Borrow source_loot;
    data::ItemPowerTablesV5::Borrow source_powers;
    data::ItemTextServicesV5 source_text;
    LootRandom8V2* source_rng{};
    check(source_gear.loot_sources_v8(source_loot, source_powers, source_text, source_rng, error),
        "same native source Gear table/power/text owners");
    const auto source_items = item_states(*source_gear.inventory());
    check(!source_items.empty(), "genuine source generated nonempty GEAR");
    std::size_t source_power_count{};
    for (const auto& item : source_items) source_power_count += item.powers.size();
    level::SavegameStreamV2 gear_payload;
    check(level::player_save_inventory_writer_v45(*source_gear.inventory(), source_powers.names(),
        gear_payload, error), "original GEAR writer emits selected source inventory");
    check(!gear_payload.bytes().empty(), "native GEAR payload is nonempty");

    auto bytes_path = read_file(platform.env.assets + "/original-cache/data/pydata/character_properties_pyarray.bin");
    auto names_path = read_file(platform.env.assets + "/original-cache/data/pydata/character_properties_pyarraynames.bin");
    auto schema_path = read_file(platform.env.assets + "/original-cache/data/pydata/character_properties_pystructnames.bin");
    data::CharacterTable characters;
    check(data::load_characters({bytes_path.data(),bytes_path.size()}, {names_path.data(),names_path.size()},
        {schema_path.data(),schema_path.size()}, characters, error), "actual CharacterTable");
    bytes_path = read_file(platform.env.assets + "/original-cache/data/pydata/skills_pyarray.bin");
    names_path = read_file(platform.env.assets + "/original-cache/data/pydata/skills_pyarraynames.bin");
    schema_path = read_file(platform.env.assets + "/original-cache/data/pydata/skills_pystructnames.bin");
    data::SkillTables skills;
    check(skills.load({bytes_path.data(),bytes_path.size()}, {names_path.data(),names_path.size()},
        {schema_path.data(),schema_path.size()}, error), "actual SkillTables");
    const std::string quest_root = "port/level-world/reference/character-menu-profile-v51/cache/";
    auto quest_array = read_file(quest_root + "v2quests_pyarray.bin");
    auto quest_names = read_file(quest_root + "v2quests_pyarraynames.bin");
    auto quest_tables = std::make_shared<data::QuestTablesPersistenceV51>();
    check(quest_tables->decode({quest_array.data(),quest_array.size()},
        {quest_names.data(),quest_names.size()}, error), "actual persistence QuestTables");

    auto fs = std::make_shared<MemoryFiles>();
    auto file_services = fs->services();
    auto jobs = std::make_shared<level::SavegameJobsOwnerV2>(file_services);
    auto source_save = std::make_shared<data::PlayerSavegameV1>();
    source_save->set_character(character);
    data::FreshPlayerProfileV1 fresh;
    check(data::fresh_player_profile_v1(characters, "KnightPlayerBase", "Saved Gear Fixture",
        123, 456, fresh, error), "actual FreshPlayerProfile source metadata");
    constexpr const char* filename = "native-present-gear.savegame";
    fs->files[filename] = fresh.bytes;
    auto profile = std::make_shared<level::CampaignSaveProfileV45>(filename, file_services, jobs);
    check(profile->construct(error), "actual profile C1 over fresh source bytes");
    auto writer_lease = std::make_shared<int>(7);
    check(profile->register_writer("GEAR", writer_lease,
        [&](level::SavegameStreamV2& stream, std::string& write_error) {
            return level::player_save_inventory_writer_v45(*source_gear.inventory(),
                source_powers.names(), stream, write_error);
        }, error), "register actual same-owner GEAR section writer");
    auto properties_writer_lease = std::make_shared<int>(9);
    check(profile->register_writer("PROP", properties_writer_lease,
        [&](level::SavegameStreamV2& stream, std::string& write_error) {
            return level::player_save_properties_writer_v45(*source_gear.property_view(),
                source_save->source_property_tail194_v45(), stream, write_error);
        }, error), "register actual same-Character PROP writer");
    check(profile->save_all(error) && jobs->flush(nullptr, error), "write original framed profile and GEAR section");
    auto selected_bytes = std::make_shared<const std::vector<std::uint8_t>>(fs->files.at(filename));
    auto lease = std::make_shared<int>(8);

    auto success_graph = make_profile_graph(character, profile, selected_bytes, source_save, quest_tables,
        characters, skills.borrow(), lease, false);
    check(success_graph->bootstrap->prepare(error), "same Save/profile prepare before Gear");
    auto success_properties = std::make_shared<PropertyState>(initial);
    LootRandom8V2 restore_random{1,0};
    auto restored_input = make_equipment_input(success_properties, restore_random);
    restored_input.immutable_loot_v88 = source_loot;
    restored_input.immutable_powers_v88 = source_powers;
    restored_input.services_lease_v62 = lease;
    check(success_graph->bootstrap->configure_equipment(restored_input, error),
        "configure source Gear through the same CharacterProfileBootstrap");
    unsigned load4_calls{};
    data::ItemPresentationOwnerV5 restored_presentation(source_powers);
    platform.presentation = &restored_presentation;
    auto native_load4 = restored_input.source_profile_load_v59;
    restored_input.source_profile_load_v59 = [&](PlayerEquipmentRenderOwnerV1& gear, std::string& load_error) {
        ++load4_calls;
        platform.item_text = const_cast<ui::ItemTextOwnerV5*>(gear.item_text_owner_v1())->services();
        return native_load4(gear, load_error);
    };
    PlayerEquipmentRenderOwnerV1 restored_gear(std::move(restored_input));
    if (!restored_gear.prepare_restore_v60(error) || !restored_gear.prepared_v60() || restored_gear.ready())
        throw std::runtime_error("original pre-grant Load4 source profile window: " + error);
    check(load4_calls == 1 && success_graph->bootstrap->gear_delivered(), "Load4 delivered GEAR once");
    check(platform.gold_notification_calls == 1 && platform.saved_power_calls == source_power_count,
        "same source SetGold tail and one native ItemPresentation AddPower per saved power");
    check(item_states(*restored_gear.inventory()) == source_items, "item IDs, powers, slots, quantity, value and text roundtrip");
    check(restored_gear.inventory()->current_equipment() == source_gear.inventory()->current_equipment(),
        "same selected source equipment set");
    check(success_properties->saved == source_properties->saved,
        "same source PROP sheet after the ordered Load4 profile sections");
    check(restored_gear.source_reset_gear_properties_v61(error) &&
        restored_gear.source_load_gear_properties_v61(error),
        "original same-owner ResetGearProperties/LoadGearProperties after Load4");
    check(success_properties->gear == source_properties->gear,
        "same original source GEAR property sheet after native gear-load phases");
    for (unsigned set=0; set<2; ++set) for (unsigned slot=0; slot<9; ++slot) {
        auto* expected = source_gear.inventory()->equipment()[set][slot];
        auto* actual = restored_gear.inventory()->equipment()[set][slot];
        check((expected == nullptr) == (actual == nullptr), "same equipped source slot occupancy");
        if (expected && actual) check(expected->item->id == actual->item->id &&
            expected->item->name == actual->item->name, "same equipped item/name/text");
    }
    const auto& active = restored_gear.inventory()->equipment()[restored_gear.inventory()->current_equipment()];
    check((active[1] || active[2]) && restored_gear.item_text_owner_v1() != nullptr,
        "selected source weapon and retained actual text owner");
    const auto before_grants = item_states(*restored_gear.inventory());
    const auto before_gear_sheet = success_properties->gear;
    const auto before_resolved = success_properties->resolved;
    const auto before_random_calls = restore_random.calls;
    check(restored_gear.finish_initial_grants_v60(error) && restored_gear.ready(),
        "original InitialGrant after present GEAR");
    check(item_states(*restored_gear.inventory()) == before_grants &&
        restored_gear.inventory()->items().size() == source_items.size() && restore_random.calls == before_random_calls,
        "saved present GEAR skips duplicate starting grants");
    check(success_properties->gear == before_gear_sheet && success_properties->resolved == before_resolved,
        "starting grant phase retains loaded source Gear sheets");
    check(load4_calls == 1 && success_graph->bootstrap->gear_delivered(), "no duplicate Load4 after grants");

    auto failure_graph = make_profile_graph(character, profile, selected_bytes, {}, quest_tables,
        characters, skills.borrow(), lease, true);
    check(failure_graph->bootstrap->prepare(error), "failure fixture same Save/profile prepare");
    auto failure_properties = std::make_shared<PropertyState>(initial);
    LootRandom8V2 failure_random{1,0};
    auto failure_input = make_equipment_input(failure_properties, failure_random);
    failure_input.immutable_loot_v88 = source_loot;
    failure_input.immutable_powers_v88 = source_powers;
    failure_input.services_lease_v62 = lease;
    check(failure_graph->bootstrap->configure_equipment(failure_input, error),
        "configure source Gear failure fixture");
    unsigned failure_load4_calls{};
    data::ItemPresentationOwnerV5 failure_presentation(source_powers);
    platform.presentation = &failure_presentation;
    auto failure_load4 = failure_input.source_profile_load_v59;
    failure_input.source_profile_load_v59 = [&](PlayerEquipmentRenderOwnerV1& gear, std::string& load_error) {
        ++failure_load4_calls;
        platform.item_text = const_cast<ui::ItemTextOwnerV5*>(gear.item_text_owner_v1())->services();
        return failure_load4(gear, load_error);
    };
    PlayerEquipmentRenderOwnerV1 failed_gear(std::move(failure_input));
    check(!failed_gear.prepare_restore_v60(error) &&
        error == "Deliberate failure after genuine source GEAR Load4 prefix",
        "later reached native service failure retains source failure receipt");
    check(failure_load4_calls == 1 && failure_graph->bootstrap->gear_delivered() &&
        item_states(*failed_gear.inventory()) == source_items,
        "failure keeps the genuine same-owner GEAR prefix");
    check(platform.gold_notification_calls == 2 && platform.saved_power_calls == 2 * source_power_count,
        "failure reaches source GEAR only after real SetGold/AddPower prefixes");
    const auto failed_prefix = item_states(*failed_gear.inventory());
    const auto failed_random_calls = failure_random.calls;
    std::string grant_error;
    check(!failed_gear.finish_initial_grants_v60(grant_error) &&
        item_states(*failed_gear.inventory()) == failed_prefix &&
        failure_random.calls == failed_random_calls,
        "failed source prefix cannot fall through to starting grants");

    check(platform.env.opened == platform.env.closed, "all actual localization file leases close");
    std::cout << "{\"validation\":\"PASS\",\"source_items\":" << source_items.size()
        << ",\"source_powers\":" << source_power_count
        << ",\"native_gear_bytes\":" << gear_payload.bytes().size()
        << ",\"load4_calls\":1,\"failure_prefix_items\":" << failed_prefix.size()
        << ",\"duplicate_starting_grants\":0,\"same_profile_save_owner\":true"
        << ",\"item_power_vector_roundtrip\":true,\"source_slot_and_selection_roundtrip\":true"
        << ",\"item_text_roundtrip\":true,\"gear_sheet_roundtrip\":true"
        << ",\"checks\":" << checks
        << ",\"full_initpost_or_player_readiness\":false}\n";
    return 0;
}
} // namespace

int main(int argc, char** argv) {
    try { return run(argc, argv); }
    catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
