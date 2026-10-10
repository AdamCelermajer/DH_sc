#include "../character_state_page_v1.hpp"
#include "../character_state_faery_v1.hpp"
#include "../../../asset_catalog.hpp"
#include "../../../features/character_menu/menu_text.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace dh::foundation::faery_menu {
bool source_text_symbol_v1(const std::string&, std::int32_t, std::int32_t,
                           std::string&, std::string&);
}

namespace {
using Bytes = std::vector<std::uint8_t>;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
Bytes file(const char* path) {
    std::ifstream in(path, std::ios::binary);
    check(bool(in), "actual source Faery table input missing");
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}
dh2::data::Bytes view(const Bytes& bytes) { return {bytes.data(), bytes.size()}; }
}

int main(int argc, char** argv) {
    try {
        check(argc == 5, "expected actual Faery tables and staged MenuLocalization asset root");
        auto raw = file(argv[1]);
        auto names = file(argv[2]);
        auto schema = file(argv[3]);
        dh2::data::FaeryTables tables;
        std::string error;
        check(tables.load(view(raw), view(names), view(schema), error),
              "source FaeryTables reader rejected original cache inputs");
        auto borrow = tables.borrow();
        check(borrow.list_names() == std::vector<std::string>({"DEFAULT", "Knight", "Mage", "Rogue"}),
              "source FaeryList name order differs");
        check(borrow.lists() == std::vector<std::vector<std::int32_t>>({
              {2, 4, 5, 6, 3}, {1, 13, 14, 15, 7},
              {1, 13, 14, 15, 7}, {1, 13, 14, 15, 7}}),
              "source FaeryList row order differs");

        dh::foundation::CharacterState character;
        character.id = "source-faery";
        character.name = "Faery";
        character.class_id = "mage";
        character.source_faery_state_known = true;
        character.source_faery_list_id = 2;
        character.faery_by_difficulty[0].current_faery = 0;
        character.faery_by_difficulty[1].current_faery = 1;
        character.faery_by_difficulty[2].current_faery = 4;
        const std::array<std::array<std::uint8_t, 5>, 3> states{{
            {{1, 0, 2, 1, 0}}, {{0, 1, 0, 1, 0}}, {{2, 1, 255, 0, 1}}}};
        for (std::size_t difficulty = 0; difficulty < states.size(); ++difficulty) {
            for (std::size_t slot = 0; slot < states[difficulty].size(); ++slot) {
                character.faery_by_difficulty[difficulty].faeries[slot].state = states[difficulty][slot];
                character.faery_by_difficulty[difficulty].faeries[slot].level =
                    static_cast<std::uint16_t>(difficulty * 10 + slot);
            }
        }
        auto owner = std::make_shared<int>(7);
        dh::foundation::faery_menu::CharacterStateFaeryBindingsV1 bindings;
        bindings.owner = owner;
        bindings.character = &character;
        bindings.tables = borrow;
        bindings.difficulty = 0;
        bindings.validate_same_character = [&character](const auto* actual, std::string&) {
            return actual == &character;
        };
        bindings.debug_unlock_all_faeries = [](bool& unlocked, std::string&) {
            unlocked = false;
            return true;
        };
        dh::foundation::AssetCatalog localization_assets(argv[4]);
        dh::foundation::character_menu::MenuLocalization localization;
        check(localization.load(localization_assets, "original-cache/data", 0, error),
              "actual English MenuLocalization source corpus failed to load");
        check(localization.bind_profile(&character, error),
              "actual MenuLocalization did not retain same CharacterState profile");
        bindings.localize = [&localization, &character](const std::string& symbol,
                                                       std::string& text,
                                                       std::string& message) {
            return localization.symbol(symbol, &character, text, message);
        };

        const auto expected_rows = std::array<std::array<int, 5>, 4>{{
            {{2, 4, 5, 6, 3}}, {{1, 13, 14, 15, 7}},
            {{1, 13, 14, 15, 7}}, {{1, 13, 14, 15, 7}}}};
        for (int list_id = 0; list_id < 4; ++list_id) {
            character.source_faery_list_id = list_id;
            for (std::size_t slot = 0; slot < 5; ++slot)
                check(borrow.lists()[std::size_t(list_id)][slot] == expected_rows[std::size_t(list_id)][slot],
                      "actual source row mapping changed");
            bindings.difficulty = list_id % 3;
            dh::foundation::faery_menu::CharacterStateFaeryPageV1 page;
            check(dh::foundation::faery_menu::present_character_state_faery_v1(bindings, page, error),
                  "CharacterState Faery page projection failed");
            check(page.list_id == list_id && page.list_name == borrow.list_names()[std::size_t(list_id)] &&
                  page.selected_slot == character.faery_by_difficulty[std::size_t(bindings.difficulty)].current_faery,
                  "projected Faery list/selection owner differs");
            check(page.text.size() == 4 && page.text[3].value == "Faeries",
                  "actual source title localization differs from English pack");
            const auto selected_level = character.faery_by_difficulty[
                std::size_t(bindings.difficulty)].faeries[std::size_t(page.selected_slot)].level;
            for (std::size_t field = 0; field < page.text.size(); ++field) {
                std::string symbol, expected_text;
                const auto slot = field == 3 ? -1 : page.selected_slot;
                const auto level = field == 2 ? selected_level : 0;
                check(dh::foundation::faery_menu::source_text_symbol_v1(
                          page.text[field].field.path, slot, level, symbol, error) &&
                      localization.symbol(symbol, &character, expected_text, error) &&
                      page.text[field].value == expected_text && expected_text != symbol,
                      "page field did not resolve through actual English MenuLocalization corpus");
            }
            for (std::size_t slot = 0; slot < 5; ++slot) {
                const auto record_id = expected_rows[std::size_t(list_id)][slot];
                const auto& record = borrow.faeries()[std::size_t(record_id)];
                check(page.slots[slot].table_record_id == record_id &&
                      page.slots[slot].table_name == borrow.faery_names()[std::size_t(record_id)] &&
                      page.slots[slot].elemental_id == record.scalar.words[2] &&
                      page.slots[slot].description_symbol == record.scalar.words[1] &&
                      page.slots[slot].name_symbol == record.scalar.words[4] &&
                      page.slots[slot].model_file_id == record.scalar.words[3] &&
                      page.slots[slot].spell_type == static_cast<std::int32_t>(record.scalar.words[7]) &&
                      page.slots[slot].source_type == static_cast<std::int32_t>(record.scalar.words[8]) &&
                      page.slots[slot].spell_script == record.script,
                      "actual Faery field projection differs from source table row");
            }
        }

        character.source_faery_list_id = 2;
        std::string hotty_name;
        check(localization.symbol("GAMEPLAYMENUS_FAERY_5B", &character, hotty_name, error) &&
              hotty_name == "Avalon – Awakened Form",
              "source menu slot4/Hotty actual upgraded localization differs");
        for (int difficulty = 0; difficulty < 3; ++difficulty) {
            bindings.difficulty = difficulty;
            dh::foundation::faery_menu::CharacterStateFaeryPageV1 page;
            check(dh::foundation::faery_menu::present_character_state_faery_v1(bindings, page, error),
                  "three-difficulty Faery state projection failed");
            const auto& source = character.faery_by_difficulty[std::size_t(difficulty)];
            check(page.selected_slot == source.current_faery, "difficulty-specific current Faery mismatch");
            for (std::size_t slot = 0; slot < 5; ++slot) {
                const auto status = dh::foundation::faery_menu::source_faery_knowledge_v1(
                    true, source.faeries[slot].state, true, false);
                const auto expected = source.faeries[slot].state == 1
                    ? dh::foundation::faery_menu::SourceFaeryKnowledgeV1::known
                    : dh::foundation::faery_menu::SourceFaeryKnowledgeV1::locked;
                check(status == expected && page.slots[slot].knowledge == status &&
                      page.slots[slot].raw_saved_state == source.faeries[slot].state &&
                      page.slots[slot].saved_level == source.faeries[slot].level,
                      "source-faithful tri-state or saved progress mismatch");
                check(slot == std::size_t(source.current_faery)
                          ? page.slots[slot].visual == dh::foundation::faery_menu::ButtonVisual::focused
                          : page.slots[slot].visual == (source.faeries[slot].state == 1
                              ? dh::foundation::faery_menu::ButtonVisual::idle
                              : dh::foundation::faery_menu::ButtonVisual::locked),
                      "source selected/focused/locked frame precedence differs");
            }
        }

        bindings.difficulty = 0;
        bindings.debug_unlock_all_faeries = [](bool& unlocked, std::string&) {
            unlocked = true;
            return true;
        };
        dh::foundation::faery_menu::CharacterStateFaeryPageV1 debug_page;
        check(dh::foundation::faery_menu::present_character_state_faery_v1(
                  bindings, debug_page, error) &&
              debug_page.slots[1].raw_saved_state == 0 &&
              debug_page.slots[1].knowledge == dh::foundation::faery_menu::SourceFaeryKnowledgeV1::known,
              "source UnlockAllFaeries override was not applied to actual page status");
        bindings.debug_unlock_all_faeries = {};
        dh::foundation::faery_menu::CharacterStateFaeryPageV1 no_debug_page;
        check(dh::foundation::faery_menu::present_character_state_faery_v1(
                  bindings, no_debug_page, error) &&
              no_debug_page.slots[1].knowledge == dh::foundation::faery_menu::SourceFaeryKnowledgeV1::unknown &&
              no_debug_page.button_art[1].empty(),
              "missing Debug source fact must not invent locked/unlocked state");

        character.source_faery_state_known = false;
        bindings.difficulty = 1;
        dh::foundation::faery_menu::CharacterStateFaeryPageV1 unknown;
        check(dh::foundation::faery_menu::present_character_state_faery_v1(bindings, unknown, error),
              "legacy unknown Faery state should remain displayable");
        check(unknown.selected_slot == -1, "unknown constructor current slot was treated as selected");
        for (std::size_t slot = 0; slot < unknown.slots.size(); ++slot)
            check(unknown.slots[slot].knowledge == dh::foundation::faery_menu::SourceFaeryKnowledgeV1::unknown &&
                  unknown.button_art[slot].empty(), "unknown progress was rendered as locked or unlocked");
        const auto unchanged = character.faery_by_difficulty[1].current_faery;
        check(!dh::foundation::faery_menu::select_character_state_faery_v1(bindings, 2, error) &&
              character.faery_by_difficulty[1].current_faery == unchanged,
              "unknown progress must reject selection without mutation");

        character.source_faery_state_known = true;
        character.faery_by_difficulty[1].faeries[2].state = 0; // locked Faery: rejected in the provider (P14 G2).
        const auto d0 = character.faery_by_difficulty[0].current_faery;
        const auto d2 = character.faery_by_difficulty[2].current_faery;
        const auto before_locked = character.faery_by_difficulty[1].current_faery;
        check(!dh::foundation::faery_menu::select_character_state_faery_v1(bindings, 2, error) &&
              error.find("locked") != std::string::npos &&
              character.faery_by_difficulty[1].current_faery == before_locked,
              "locked Faery selection must be rejected by the provider without mutation");
        check(dh::foundation::faery_menu::select_character_state_faery_v1(bindings, 3, error) &&
              character.faery_by_difficulty[1].current_faery == 3 &&
              character.faery_by_difficulty[0].current_faery == d0 &&
              character.faery_by_difficulty[2].current_faery == d2 &&
              character.faery_by_difficulty[1].faeries[2].state == 0,
              "unlocked selection must change only current slot at the selected difficulty");
        const auto committed = character.faery_by_difficulty[1].current_faery;
        check(!dh::foundation::faery_menu::select_character_state_faery_v1(bindings, 5, error) &&
              character.faery_by_difficulty[1].current_faery == committed,
              "invalid source slot must preserve the prior selected slot");
        std::uint32_t dispatched_slot = 99;
        auto action_bindings = bindings;
        action_bindings.activate_slot = [&](std::uint32_t slot, std::string&) {
            dispatched_slot = slot;
            character.faery_by_difficulty[1].current_faery = static_cast<std::int32_t>(slot);
            return true;
        };
        check(dh::foundation::faery_menu::select_character_state_faery_v1(action_bindings, 1, error) &&
              dispatched_slot == 1 && character.faery_by_difficulty[1].current_faery == 1,
              "validated source click did not dispatch the supplied same-owner action callback");
        check(!dh::foundation::faery_menu::select_character_state_faery_v1(action_bindings, 4, error) &&
              dispatched_slot == 1 && character.faery_by_difficulty[1].current_faery == 1,
              "locked slot must not reach the same-owner action callback");
        character.faery_by_difficulty[1].current_faery = committed;

        dh::foundation::character_menu::SourcePageProviderV1 provider;
        check(dh::foundation::faery_menu::bind_character_state_faery_provider_v1(
                  bindings, provider, error), "generic CharacterState provider adapter failed");
        dh::foundation::character_menu::SourceCompositionV1 composition(owner);
        check(composition.register_page(dh::foundation::character_menu::Tab::faery,
                                        std::move(provider), error),
              "same-owner SourceComposition registration failed");
        dh::foundation::character_menu::Bindings menu_bindings;
        check(composition.install_content(menu_bindings, error),
              "SourceComposition content adapter install failed");
        dh::foundation::character_menu::Frame menu_frame;
        check(menu_bindings.content(dh::foundation::character_menu::Tab::faery,
                                    menu_frame, error) &&
              !menu_frame.art.batches.empty() && menu_frame.text.size() == 4,
              "generic Faery page did not append source SWF geometry and fields");

        auto routed = bindings;
        routed.difficulty = 0; // difficulty 0 row: slot 0 is unlocked (state 1) in this fixture.
        std::uint32_t action_count = 0;
        std::uint32_t action_slot = 99;
        routed.activate_slot = [&](std::uint32_t slot, std::string&) {
            ++action_count;
            action_slot = slot;
            character.faery_by_difficulty[std::size_t(routed.difficulty)].current_faery =
                static_cast<std::int32_t>(slot);
            return true;
        };
        dh::foundation::character_menu::SourcePageProviderV1 release_provider;
        check(dh::foundation::faery_menu::bind_character_state_faery_provider_v1(
                  routed, release_provider, error), "same-owner activation provider bind failed");
        check(dh::foundation::faery_menu::slot_at(80.f, 80.f) == 0 &&
              release_provider.release(80.f, 80.f, error) && action_count == 1 && action_slot == 0,
              "exact authored locked-looking hit did not dispatch same-owner activation exactly once");
        check(character.faery_by_difficulty[std::size_t(routed.difficulty)].current_faery == 0,
              "source-valid activation did not update the selected difficulty's current slot");
        const auto before_outside = character.faery_by_difficulty[std::size_t(routed.difficulty)].current_faery;
        check(release_provider.release(0.f, 0.f, error) && action_count == 1 &&
              character.faery_by_difficulty[std::size_t(routed.difficulty)].current_faery == before_outside,
              "outside authored hit contour must not dispatch or mutate selection");
        character.faery_by_difficulty[0].faeries[0].state = 0; // lock slot 0 for the release path
        check(!release_provider.release(80.f, 80.f, error) && action_count == 1 &&
              error.find("locked") != std::string::npos,
              "locked Faery click must be rejected by the provider before activation");
        character.faery_by_difficulty[0].faeries[0].state = 1;
        character.faery_by_difficulty[std::size_t(routed.difficulty)].current_faery = -1;
        check(!release_provider.release(80.f, 80.f, error) && action_count == 1,
              "known progress with invalid current source index must reject before activation");
        character.faery_by_difficulty[std::size_t(routed.difficulty)].current_faery = before_outside;
        // T3 script effects and T2 host commit on a fresh CharacterState (creation zeroes every row).
        {
            using namespace dh::foundation::faery_menu;
            dh::foundation::CharacterState fresh;
            fresh.source_faery_state_known = true;
            fresh.source_faery_list_id = 0;
            check(fresh.faery_by_difficulty[0].faeries[0].state == 0, "fresh creation row must start locked");
            check(apply_source_set_faery_state_v1(fresh, active_faery_difficulty_v1(), 0, 1, error) &&
                  fresh.faery_by_difficulty[0].faeries[0].state == 1 &&
                  fresh.faery_by_difficulty[0].faeries[1].state == 0 &&
                  fresh.faery_by_difficulty[1].faeries[0].state == 0 &&
                  fresh.faery_by_difficulty[0].current_faery == 0,
                  "SetFaeryState(slot 0, 1) must write one row of the active difficulty only");
            check(!apply_source_set_faery_state_v1(fresh, 0, 5, 1, error) && !error.empty(),
                  "SetFaeryState slot 5 must be rejected");
            check(!apply_source_set_faery_state_v1(fresh, 0, 0, 256, error) &&
                  fresh.faery_by_difficulty[0].faeries[0].state == 1,
                  "SetFaeryState byte overflow must be rejected without mutation");
            check(!apply_source_set_faery_state_v1(fresh, 3, 0, 1, error),
                  "difficulty 3 must be rejected");
            check(apply_source_inc_faery_level_v1(fresh, 0, 0, error) &&
                  fresh.faery_by_difficulty[0].faeries[0].level == 1 &&
                  fresh.faery_by_difficulty[0].faeries[0].state == 1 &&
                  fresh.faery_by_difficulty[0].faeries[1].level == 0,
                  "IncFaeryLevel must add one level to one row and leave its state");
            fresh.faery_by_difficulty[0].faeries[0].level = 0xFFFF;
            check(!apply_source_inc_faery_level_v1(fresh, 0, 0, error) &&
                  fresh.faery_by_difficulty[0].faeries[0].level == 0xFFFF,
                  "IncFaeryLevel overflow must be rejected");
            fresh.source_faery_state_known = false;
            check(!apply_source_inc_faery_level_v1(fresh, 0, 1, error) &&
                  !apply_source_set_faery_state_v1(fresh, 0, 1, 1, error),
                  "unknown source Faery rows must reject script writes");
            fresh.source_faery_state_known = true;

            auto fresh_owner = std::make_shared<dh::foundation::CharacterState>(fresh);
            fresh_owner->faery_by_difficulty[0].faeries[1].state = 1;
            CharacterStateFaeryPageHostV1 host;
            host.owner = fresh_owner;
            host.tables = borrow;
            host.localize = [&character, &localization](const std::string& symbol, std::string& text, std::string& message) {
                return localization.symbol(symbol, &character, text, message);
            };
            int persists = 0, refreshes = 0;
            bool fail_persist = true;
            host.persist = [&](std::string& message) {
                ++persists;
                if (fail_persist) { message = "save refused"; return false; }
                return true;
            };
            host.refresh_hud = [&] { ++refreshes; };
            check(!commit_character_state_faery_selection_v1(host, 0, 1, error) &&
                  fresh_owner->faery_by_difficulty[0].current_faery == 0 && persists == 1 && refreshes == 0,
                  "failed selection save must roll back current_faery and skip the HUD refresh");
            fail_persist = false;
            check(commit_character_state_faery_selection_v1(host, 0, 1, error) &&
                  fresh_owner->faery_by_difficulty[0].current_faery == 1 && persists == 2 && refreshes == 1,
                  "accepted selection must commit, persist once and refresh the HUD once");

            dh::foundation::character_menu::SourceCompositionV1 host_composition(fresh_owner);
            check(register_character_state_faery_page_v1(host_composition, host, error),
                  "CharacterState Faery host registration failed");
            dh::foundation::character_menu::Bindings host_bindings;
            check(host_composition.install_content(host_bindings, error),
                  "host composition content install failed");
            dh::foundation::character_menu::Frame host_frame;
            check(host_bindings.content(dh::foundation::character_menu::Tab::faery, host_frame, error) &&
                  !host_frame.art.batches.empty() && host_frame.text.size() == 4,
                  "host-registered Faery page did not append source geometry and fields");
            CharacterStateFaeryPageHostV1 unbound;
            check(!register_character_state_faery_page_v1(host_composition, unbound, error),
                  "host without tables/localization must not register");
        }
        // P14 FAERY follow-up: legacy rows are normalized to creation zeros, Swamp_Intro unlock works, and
        // an unlocked Rocky (no spell) is selectable with a no-spell diagnostic instead of a silent key 4.
        {
            using namespace dh::foundation::faery_menu;
            dh::foundation::CharacterState legacy;
            legacy.source_faery_state_known = false;
            legacy.source_faery_list_id = 0;
            legacy.faery_by_difficulty[0].current_faery = 3; // garbage from an old slot must not survive
            check(ensure_source_faery_rows_v1(legacy) && legacy.source_faery_state_known,
                  "legacy save without Faery rows must be normalized on first use");
            for (const auto& difficulty : legacy.faery_by_difficulty) {
                check(difficulty.current_faery == 0, "normalized current Faery must be creation zero");
                for (const auto& faery : difficulty.faeries)
                    check(faery.state == 0 && faery.level == 0, "normalized Faery rows must be creation zeros");
            }
            check(!ensure_source_faery_rows_v1(legacy), "known rows must not be re-initialized");
            check(apply_source_set_faery_state_v1(legacy, active_faery_difficulty_v1(), 0, 1, error) &&
                  legacy.faery_by_difficulty[0].faeries[0].state == 1,
                  "Swamp_Intro SetFaeryState(slot 0, 1) must unlock Celest on a normalized legacy save");
            check(faery_slot_has_spell_v1(0) && faery_slot_has_spell_v1(4) &&
                  !faery_slot_has_spell_v1(1) && !faery_slot_has_spell_v1(2) && !faery_slot_has_spell_v1(3),
                  "only Celest (slot 0) and Hotty (slot 4) have spells");
            check(faery_no_spell_message_v1(2).find("no spell implemented") != std::string::npos,
                  "no-spell diagnostic must name the missing spell");
            check(apply_source_set_faery_state_v1(legacy, active_faery_difficulty_v1(), 2, 1, error) &&
                  commit_source_faery_selection_v1(legacy, active_faery_difficulty_v1(), 2, error) &&
                  legacy.faery_by_difficulty[0].current_faery == 2 &&
                  !faery_slot_has_spell_v1(legacy.faery_by_difficulty[0].current_faery),
                  "unlocked Rocky must be selectable and identified as a no-spell Faery");
        }
        std::cout << "PASS CharacterState Faery page: actual four source lists/20 row links, table fields/icons, tri-state gate, all three difficulty rows, unknown legacy state, provider rejection of locked slots without mutation, same-owner action dispatch, invalid-current rejection, single-cell selection transaction, script SetFaeryState/IncFaeryLevel row effects, and host persist rollback\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
