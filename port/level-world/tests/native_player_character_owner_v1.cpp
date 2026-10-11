#include "../native_player_character_owner_v1.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool value, const char* what) {
    if (!value) throw std::runtime_error(what);
}
std::vector<std::uint8_t> file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("missing loot cache input: " + path.string());
    return {std::istreambuf_iterator<char>(input), {}};
}
bool inventory_random(void*, std::int32_t bound, std::uint32_t,
                      std::int32_t& value, std::string&) {
    if (bound <= 0) return false;
    value = 0;
    return true;
}
}

int main(int argc, char** argv) {
    using namespace dh2;
    require(argc == 2, "usage: native_player_character_owner_v1_audit <game-data-cache>");
    const std::filesystem::path cache(argv[1]);
    const auto records = file(cache / "loot_table_pyarray.bin");
    const auto names = file(cache / "loot_table_pyarraynames.bin");
    const auto schema = file(cache / "loot_table_pystructnames.bin");
    data::LootTablesV2 loot_tables;
    std::string error;
    require(loot_tables.load({records.data(), records.size()},
                             {names.data(), names.size()},
                             {schema.data(), schema.size()}, error),
            error.c_str());

    character::NativePlayerCharacterOwnerV1 owner;
    data::PlayerSavegameV1 save;
    data::PropertyState properties;
    std::uintptr_t character_660 = 0;
    const auto identity = owner.identity();
    require(identity != 0 &&
            identity == reinterpret_cast<std::uintptr_t>(&owner) &&
            owner.owner() == identity,
            "native object address is not the Coordinator identity");
    require(owner.visibility_80() == 1 && owner.enabled_8a() == 1,
            "native player visibility bytes did not start at source defaults");
    owner.set_visible(0);
    require(owner.visibility_80() == 0 && !owner.render_batch_visible(false) &&
            owner.render_batch_visible(true),
            "SetVisible(false) did not hide player batches while preserving environment");
    owner.set_visible(1);
    require(owner.visibility_80() == 1 && owner.render_batch_visible(false),
            "SetVisible(true) did not restore player batch visibility");
    owner.set_enabled_8a(0);
    require(owner.visibility_80() == 1,
            "changing the enabled byte incorrectly rewrote current visibility");
    owner.set_visible(2);
    require(owner.visibility_80() == 0,
            "nonzero SetVisible did not preserve the exact enabled-byte rule");
    owner.set_enabled_8a(1);
    owner.set_visible(1);
    auto* game_object_moving = owner.game_object_moving_storage();
    require(game_object_moving && !owner.game_object_moving() &&
            *game_object_moving == 0,
            "canonical Player GameObject moving projection did not start clear");
    auto* ooi_intent = owner.ooi_intent_storage();
    require(ooi_intent && !owner.ooi_intent() && *ooi_intent == 0 &&
            ooi_intent != game_object_moving &&
            ooi_intent != reinterpret_cast<std::uint8_t*>(&owner.state.flags),
            "canonical Character OOI intent storage is missing, aliased, or not initially clear");
    *game_object_moving = 1;
    require(owner.game_object_moving() == 1,
            "canonical Player GameObject moving projection is not owned storage");
    *ooi_intent = 1;
    require(owner.ooi_intent() == 1 && owner.game_object_moving() == 1 &&
            owner.state.flags == 0,
            "Character OOI intent was aliased to movement or Coordinator flags");
    owner.stop_game_object_moving();
    require(!owner.game_object_moving() && owner.ooi_intent() == 1,
            "Player Stop did not clear only the moving projection");
    save.set_character(identity);

    require(owner.bind_session(&character_660, save, properties, nullptr, error),
            error.c_str());
    require(!save.source_save_blocked() &&
            !owner.sg_block(identity + 8, 1) && !owner.sg_block(identity, 2) &&
            owner.sg_block(identity, 1) && save.source_save_blocked() &&
            owner.sg_block(identity, 0) && !save.source_save_blocked(),
            "Character::SG_Block did not mutate its one bound PlayerSavegame +0x0c gate");
    const std::uint32_t checkpoint_words[]{0x80000000u, 0x3f800001u, 0x7fc01234u};
    require(!owner.save_checkpoint_position(identity + 8, checkpoint_words) &&
            owner.save_checkpoint_position(identity, checkpoint_words) &&
            owner.checkpoint_position_1468(identity) &&
            owner.checkpoint_position_1468(identity)[0] == checkpoint_words[0] &&
            owner.checkpoint_position_1468(identity)[1] == checkpoint_words[1] &&
            owner.checkpoint_position_1468(identity)[2] == checkpoint_words[2],
            "QuickSave checkpoint point did not preserve the current Character XYZ words");
    const std::uint32_t revive_words[]{0x00000000u, 0xc2480001u, 0x7fc0abcdu};
    require(!owner.save_checkpoint_revive_position(identity + 8, revive_words) &&
            !owner.save_checkpoint_revive_position(identity, nullptr) &&
            owner.save_checkpoint_revive_position(identity, revive_words) &&
            owner.checkpoint_revive_position_1474(identity) &&
            owner.checkpoint_revive_position_1474(identity)[0] == revive_words[0] &&
            owner.checkpoint_revive_position_1474(identity)[1] == revive_words[1] &&
            owner.checkpoint_revive_position_1474(identity)[2] == revive_words[2] &&
            !owner.checkpoint_revive_position_1474(identity + 8),
            "CheckpointSave revive destination did not preserve raw XYZ words on the canonical Player");
    require(!owner.ooi_intent() && character_660 == identity && owner.save_for(identity) == &save &&
            owner.properties_for(identity) == &properties &&
            owner.inventory_for(identity) == nullptr,
            "fresh session did not clear OOI intent or resolve the sole live owners");
    auto* reward_gold = owner.reward_gold_1500_for(identity);
    auto* reward_xp = owner.reward_xp_1504_for(identity);
    require(reward_gold && reward_xp && *reward_gold == -1 && *reward_xp == -1 &&
            !owner.reward_gold_1500_for(identity + 8) &&
            !owner.reward_xp_1504_for(identity + 8),
            "Reward_Gold/XP receipt fields lack canonical Character ownership or source -1 defaults");
    *reward_gold = 375;
    *reward_xp = 8192;
    require(owner.reward_gold_1500_for(identity) == reward_gold &&
            owner.reward_xp_1504_for(identity) == reward_xp &&
            *reward_gold == 375 && *reward_xp == 8192,
            "reward receipts were copied, aliased, or detached from the same Character owner");
    *reward_gold = -1;
    *reward_xp = -1;
    require(owner.matches_session(&character_660, save, properties, nullptr),
            "initial session binding did not validate");
    *ooi_intent = 1;
    require(owner.bind_session(&character_660, save, properties, nullptr, error),
            "same live session was not idempotent");
    require(owner.ooi_intent()==1 && owner.ooi_intent_storage()==ooi_intent,
            "idempotent session binding replaced active OOI intent storage");

    data::FreshInventoryOwnedV4 inventory(identity, loot_tables.borrow(),
                                           {nullptr, inventory_random}, 10,
                                           properties);
    require(owner.bind_inventory(inventory, error), error.c_str());
    require(character_660 == identity && owner.save_for(identity) == &save &&
            owner.properties_for(identity) == &properties &&
            owner.inventory_for(identity) == &inventory &&
            owner.matches_session(&character_660, save, properties, &inventory),
            "late inventory attachment replaced a canonical owner");
    require(owner.bind_inventory(inventory, error),
            "same canonical inventory attachment was not idempotent");

    data::ItemPowerTablesV5 presentation_tables;
    data::ItemPresentationOwnerV5 presentation(presentation_tables.borrow());
    data::ItemTextServicesV5 text{};
    require(!owner.update_inventory_localization(identity + 8, presentation,
                                                  text, error),
            "SetLanguage refresh accepted a different Character identity");
    require(owner.update_inventory_localization(identity, presentation, text,
                                                 error),
            error.c_str());

    data::PropertyState foreign_properties;
    data::FreshInventoryOwnedV4 mismatched_properties(identity, loot_tables.borrow(),
                                                       {nullptr, inventory_random},
                                                       10, foreign_properties);
    require(!owner.bind_inventory(mismatched_properties, error),
            "inventory with a second property store was accepted");
    require(owner.inventory_for(identity) == &inventory,
            "rejected property-store swap changed the live inventory");

    data::FreshInventoryOwnedV4 foreign_inventory(identity + 8, loot_tables.borrow(),
                                                   {nullptr, inventory_random},
                                                   10, properties);
    require(!owner.bind_inventory(foreign_inventory, error),
            "inventory with a foreign Character identity was accepted");
    require(owner.inventory_for(identity) == &inventory,
            "rejected Character swap changed the live inventory");

    require(!owner.bind_session(&character_660, save, properties, nullptr, error),
            "session rebind silently detached an attached inventory");
    require(owner.inventory_for(identity) == &inventory && character_660 == identity,
            "rejected session downgrade changed live owners");

    data::PlayerSavegameV1 foreign_save;
    foreign_save.set_character(identity + 8);
    require(!owner.bind_session(&character_660, foreign_save, properties, &inventory, error),
            "foreign Save identity was accepted");
    require(character_660 == identity && owner.save_for(identity) == &save,
            "rejected rebind changed the published owner");

    std::uintptr_t foreign_character = identity + 8;
    require(!owner.bind_session(&foreign_character, save, properties, &inventory, error),
            "foreign Character660 was overwritten");
    require(foreign_character == identity + 8,
            "rejected publication changed foreign Character660");

    *ooi_intent = 1;
    require(owner.unbind_session(error) && character_660 == 0 &&
            !owner.ooi_intent() &&
            !owner.checkpoint_revive_position_1474(identity),
            "terminal Character retirement did not clear Character660");
    require(owner.ooi_intent()==0 && owner.ooi_intent_storage()==ooi_intent,
            "terminal Character retirement did not clear canonical OOI intent");
    require(!owner.save_for(identity) && !owner.properties_for(identity) &&
            !owner.inventory_for(identity) && !owner.reward_gold_1500_for(identity) &&
            !owner.reward_xp_1504_for(identity),
            "retired Character still resolves borrowed owners");
    require(owner.unbind_session(error) && !owner.ooi_intent(),
            "empty retirement was not idempotent or retained OOI intent");

    *ooi_intent=1;
    require(owner.bind_session(&character_660, save, properties, nullptr, error) &&
            !owner.ooi_intent() && character_660 == identity &&
            owner.checkpoint_revive_position_1474(identity) &&
            owner.checkpoint_revive_position_1474(identity)[0] == 0 &&
            owner.checkpoint_revive_position_1474(identity)[1] == 0 &&
            owner.checkpoint_revive_position_1474(identity)[2] == 0 &&
            *owner.reward_gold_1500_for(identity) == -1 &&
            *owner.reward_xp_1504_for(identity) == -1,
            "fresh session rebind did not clear OOI intent");
    require(owner.unbind_session(error) && !owner.ooi_intent(),
            "rebound session retirement retained OOI intent");

    std::cout << "{\"validation\":\"PASS\",\"checks\":41,"
                 "\"identity_is_native_object_address\":true,"
                 "\"source_visibility_enabled_semantics\":true,"
                 "\"coordinator_save_properties_inventory_single_owner\":true,"
                 "\"language_refresh_requires_same_player_and_v4\":true,"
                 "\"inventory_attached_after_character_publish\":true,"
                 "\"source_add_character_parity\":false}\n";
    return 0;
}
