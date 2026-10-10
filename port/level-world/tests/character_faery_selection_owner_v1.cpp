#include "../character_faery_selection_owner_v1.hpp"
#include "../objects.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace owner = dh2::character_faery_selection_owner_v1;
namespace visual = dh2::character_faery_visual_facade_v1;
namespace factory = dh2::character_runtime_factory_v1;

namespace {
std::vector<std::uint8_t> read_bytes(const std::string& path);

struct BdaeCharacterPose final : visual::CharacterPose {
    BdaeCharacterPose(const std::vector<std::uint8_t>& model,
                      const std::vector<std::uint8_t>& clip) {
        std::string error;
        if (!dh2::objects::load_resource(model.data(), model.size(),
                clip.data(), clip.size(), resource, error))
            throw std::runtime_error("BDAE faery pose rejected: " + error);
    }
    dh2::objects::Resource& bdae_resource() noexcept override { return resource; }
    bool sample(std::int32_t sequence, std::uint32_t time_ms,
                std::string& error) override {
        if (sequence != 196) { error = "bad Faery idle binding"; return false; }
        return dh2::objects::sample(resource, static_cast<std::int32_t>(time_ms), error);
    }
    dh2::objects::Resource resource;
};

struct BdaeAsset final : visual::AssetData {
    BdaeAsset(const std::string& model_path, const std::string& clip_path)
        : model(read_bytes(model_path)), clip(read_bytes(clip_path)) {}
    std::shared_ptr<visual::CharacterPose> create_character_pose() const override {
        // The current objects::Resource owns mutable scene, skinned vertices,
        // and a non-copyable animation Player. Share immutable source BDAE
        // bytes at the asset level; parse one mutable Resource per Character.
        return std::make_shared<BdaeCharacterPose>(model, clip);
    }
    const std::vector<std::uint8_t> model;
    const std::vector<std::uint8_t> clip;
};

struct Fixture {
    std::shared_ptr<const visual::Asset> assets[5];
    factory::Record* player_record{};
    unsigned skills{}, prepared{}, committed{}, rolled_back{}, discarded{};
    bool skill_fails_once{}, prepare_fails{}, placement_fails{};
    std::uint32_t prepared_id{};
};

bool update_skills(void* raw, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.skills;
    if (f.skill_fails_once) {
        f.skill_fails_once = false;
        error = "skill refresh failed once";
        return false;
    }
    error.clear();
    return true;
}

int resolve_asset(void* raw, std::uint32_t id,
                  std::shared_ptr<const visual::Asset>* output,
                  std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    if (!output || id >= 5 || !f.assets[id]) {
        error = "asset not cached";
        return 1;
    }
    *output = f.assets[id];
    error.clear();
    return 0;
}

int prepare_placement(void* raw, visual::Identity player,
                      visual::Identity faery, std::uint32_t id,
                      std::uintptr_t* plan, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.prepared;
    if (!plan || player != 0x100 || !f.player_record ||
        faery != f.player_record->faery_character_420 ||
        f.prepare_fails) {
        error = "placement preflight rejected";
        return 1;
    }
    f.prepared_id = id;
    *plan = 0xfade;
    error.clear();
    return 0;
}

int commit_placement(void* raw, std::uintptr_t plan, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    if (plan != 0xfade || f.placement_fails) {
        error = "placement commit rejected";
        return 1;
    }
    ++f.committed;
    error.clear();
    return 0;
}

int rollback_placement(void* raw, std::uintptr_t plan, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    if (plan != 0xfade) {
        error = "bad rollback plan";
        return 1;
    }
    ++f.rolled_back;
    error.clear();
    return 0;
}

void discard_placement(void* raw, std::uintptr_t plan) noexcept {
    if (plan == 0xfade) ++static_cast<Fixture*>(raw)->discarded;
}

std::vector<std::uint8_t> read_bytes(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("missing packaged BDAE: " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::shared_ptr<const visual::Asset> make_asset(unsigned id,
    const std::string& model_path, const std::shared_ptr<const visual::AssetData>& data) {
    auto asset = std::make_shared<visual::Asset>();
    asset->faery_id = id;
    constexpr int model_dictionary_ids[5] = {34,35,33,36,32};
    asset->model_dictionary_id = model_dictionary_ids[id];
    asset->idle_sequence = 196;
    asset->model_file = model_path;
    asset->renderer_data = data;
    return asset;
}

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
}

int main() {
    try {
        Fixture f;
        constexpr const char* model_paths[5] = {
            "port/android-native/app/src/main/assets/actors/faeries_02_celeste.bdae",
            "port/android-native/app/src/main/assets/actors/faeries_03_rocky.bdae",
            "port/android-native/app/src/main/assets/actors/faeries_01_wetty.bdae",
            "port/android-native/app/src/main/assets/actors/faeries_04_windy.bdae",
            "port/android-native/app/src/main/assets/actors/faeries_00_hotty.bdae",
        };
        constexpr const char* idle_clip =
            "port/android-native/app/src/main/assets/actors/faeries_celeste_idle.bdae";
        for (unsigned i = 0; i < 5; ++i) {
            auto data = std::make_shared<BdaeAsset>(model_paths[i], idle_clip);
            f.assets[i] = make_asset(i, model_paths[i], data);
        }
        auto old_asset = f.assets[1];
        factory::Record faery_record{};
        factory::Record player_record{};
        player_record.constructor.identity = 0x100;
        player_record.character.identity = 0x100;
        f.player_record = &player_record;
        faery_record.character.identity = 0x200;
        faery_record.constructor.identity = 0x200;
        player_record.faery_character_420 = faery_record.character.identity;
        auto& character = faery_record.faery_visual;
        character.character = 0x200;
        character.asset = old_asset;
        character.pose = old_asset->renderer_data->create_character_pose();
        character.position = {4.f, 5.f, 6.f};
        character.visible = true;
        character.clip_sequence = old_asset->idle_sequence;
        visual::CharacterVisual second_character{};
        second_character.character = 0x201;
        second_character.asset = old_asset;
        second_character.pose = old_asset->renderer_data->create_character_pose();
        require(second_character.asset == character.asset &&
                second_character.pose != character.pose,
                "shared BDAE asset did not create independent per-Character pose state");
        dh2::data::PlayerSavegameV1 save;
        save.initialize_faeries();
        std::string error;
        require(save.set_current_faery(1, 0, error), "initial Save faery failed");

        const dh2::data::player_faery_change_v1::Services save_services{
            &f, update_skills};
        const owner::Services services{&f, resolve_asset, prepare_placement,
            commit_placement, rollback_placement, discard_placement};
        owner::Result result{};

        require(owner::change(player_record, faery_record, save, 0, 3, save_services,
                              services, &result, error) == owner::Status::complete,
                "successful Faery selection failed");
        require(save.current_faery(0) == 3 && character.asset == f.assets[3] &&
                character.clip_sequence == 196 && character.clip_time_ms == 0 &&
                character.pose && character.pose != second_character.pose &&
                character.position[0] == 4.f && character.visible &&
                character.generation == 1 && f.committed == 1 &&
                f.discarded == 1 && result.placement_committed,
                "selection did not update canonical Save/visual/placement owners");
        require(old_asset.use_count() == 3,
                "old renderer asset cache/borrowed lifetime changed unexpectedly");

        // IDA: Level::PlaceFaeryAndFollowers writes the exact Faery Character
        // identity into each Player Character+0x420; ChangeFaery refreshes
        // that linked Character's visual. Reject a merely similar/unlinked
        // factory Record before Save, skills, visual, or placement mutate.
        player_record.faery_character_420 = 0x201;
        const auto skills_before_unlinked = f.skills;
        const auto prepared_before_unlinked = f.prepared;
        require(owner::change(player_record, faery_record, save, 0, 4, save_services,
                              services, &result, error) == owner::Status::invalid_argument &&
                f.prepared == prepared_before_unlinked &&
                save.current_faery(0) == 3 && f.skills == skills_before_unlinked &&
                character.asset == f.assets[3] && !result.save_changed &&
                !result.visual_swapped,
                "unlinked Faery factory Record was allowed to bypass Character+0x420 preflight");
        player_record.faery_character_420 = faery_record.character.identity;
        player_record.character.identity = 0x101;
        player_record.constructor.identity = 0x101;
        const auto prepared_before_foreign_player = f.prepared;
        const auto skills_before_foreign_player = f.skills;
        require(owner::change(player_record, faery_record, save, 0, 4, save_services,
                              services, &result, error) == owner::Status::placement_unavailable &&
                f.prepared == prepared_before_foreign_player + 1 &&
                f.skills == skills_before_foreign_player && save.current_faery(0) == 3 &&
                character.asset == f.assets[3],
                "foreign Player Character reached canonical Save/skill or visual mutation");
        player_record.character.identity = 0x100;
        player_record.constructor.identity = 0x100;

        f.placement_fails = true;
        require(owner::change(player_record, faery_record, save, 0, 4, save_services,
                              services, &result, error) == owner::Status::placement_failed,
                "placement failure did not fail the selection transaction");
        require(save.current_faery(0) == 3 && character.asset == f.assets[3] &&
                character.generation == 3 && f.rolled_back == 1 &&
                f.discarded == 2 && result.rollback_complete,
                "placement failure did not restore Save/visual and roll back placement");

        f.placement_fails = false;
        f.skill_fails_once = true;
        require(owner::change(player_record, faery_record, save, 0, 2, save_services,
                              services, &result, error) == owner::Status::save_or_skill_failed,
                "skill update failure did not reject selection");
        require(save.current_faery(0) == 3 && character.asset == f.assets[3] &&
                f.committed == 1 && f.discarded == 3 && result.rollback_complete,
                "Save/skill failure did not restore canonical state before visual swap");

        const auto skill_calls = f.skills;
        faery_record.character.identity = 0x202;
        faery_record.constructor.identity = 0x202;
        player_record.faery_character_420 = faery_record.character.identity;
        require(owner::change(player_record, faery_record, save, 0, 0, save_services,
                              services, &result, error) == owner::Status::invalid_argument &&
                save.current_faery(0) == 3 && f.skills == skill_calls,
                "noncanonical Record/visual identity was accepted or mutated Save");
        faery_record.character.identity = 0x200;
        faery_record.constructor.identity = 0x200;
        player_record.faery_character_420 = faery_record.character.identity;

        player_record.faery_character_420 = 0x201;
        const auto prepared_before_bad_link = f.prepared;
        require(owner::change(player_record, faery_record, save, 0, 0, save_services,
                              services, &result, error) == owner::Status::invalid_argument &&
                f.prepared == prepared_before_bad_link && save.current_faery(0) == 3 &&
                f.skills == skill_calls,
                "selection owner accepted a Player +0x420 link to another Faery Record");
        player_record.faery_character_420 = faery_record.character.identity;

        f.prepare_fails = true;
        const auto skills_before_preflight = f.skills;
        require(owner::change(player_record, faery_record, save, 0, 0, save_services,
                              services, &result, error) == owner::Status::placement_unavailable &&
                save.current_faery(0) == 3 && f.skills == skills_before_preflight,
                "placement preflight mutated canonical state");

        std::cout << "{\"validation\":\"PASS\",\"player_faery_420_guard\":true,"
                     "\"save_visual_placement\":true,"
                     "\"placement_rollback\":true,\"skill_failure_restore\":true,"
                     "\"shared_asset_lifetime\":true}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
