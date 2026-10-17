#include "../app/src/main/cpp/native_character_roster_transaction.hpp"
#include "../../level-world/character_ai_association.hpp"
#include "../../level-world/character_ai_initialization.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace tx = dh2::native::character_roster_transaction;
using Manager = dh2::object_manager_runtime_owner_v1::Owner;
using ManagerObject = dh2::object_manager_runtime_owner_v1::GameObject;
using CharacterList = dh2::native::character_list::Owner;
using Character = CharacterList::Character;

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

struct Fixture {
    Manager manager;
    CharacterList characters;
    ManagerObject object{};
    dh2::character::aggro_search::GameObject search_object{};
    Character character{};

    explicit Fixture(std::uintptr_t identity) {
        object.identity = identity;
        search_object.identity = identity;
        character.identity = identity;
        character.object = &search_object;
    }
};

struct CharAIQueueFixture {
    std::vector<std::uintptr_t> order;
};

std::int32_t append_char_ai(void* context,
        dh2::character_ai_initialization::State* state,
        std::uintptr_t ai) {
    if (!context || !state || !ai || state->identity != ai) return 1;
    auto& queue = *static_cast<CharAIQueueFixture*>(context);
    queue.order.push_back(ai);
    return 0;
}
} // namespace

int main() {
    try {
        {
            Fixture f(0x1001);
            tx::Result result{};
            require(tx::register_after_add(&f.manager, &f.characters, 11,
                        &f.object, &f.character, false, &result) == tx::Status::complete,
                    "registration transaction failed");
            require(result.map_committed && result.character_committed &&
                        f.manager.object_count() == 1 &&
                        f.characters.owned_nodes() == 1,
                    "registration did not commit both owners");

            require(tx::register_after_add(&f.manager, &f.characters, 12,
                        &f.object, &f.character, false, &result) ==
                        tx::Status::duplicate_identity,
                    "duplicate identity was accepted");
            require(f.manager.object_count() == 1 &&
                        f.characters.owned_nodes() == 1,
                    "duplicate identity changed an owner");

            require(tx::remove_after_remove(&f.manager, &f.characters, 11,
                        &f.character, &result) == tx::Status::complete,
                    "paired teardown failed");
            require(f.manager.object_count() == 0 &&
                        f.characters.owned_nodes() == 0 &&
                        result.removed_character_nodes == 1,
                    "paired teardown left a registration behind");
        }

        {
            Fixture f(0x2002);
            tx::Result result{};
            // Force the second owner to reject its append after the map has
            // committed. The transaction must remove that map prefix.
            auto& source = const_cast<CharacterList::Source&>(f.characters.source());
            source.sentinel.next = nullptr;
            require(tx::register_after_add(&f.manager, &f.characters, 21,
                        &f.object, &f.character, false, &result) ==
                        tx::Status::character_list_failed,
                    "malformed-list append did not fail at the second owner");
            require(result.map_rolled_back && !result.map_committed &&
                        !result.character_committed && f.manager.object_count() == 0,
                    "failed append left the map prefix committed");
            f.characters.clear();
        }

        {
            // Crypt's DACT projections share one ObjectManager map and one
            // CharacterList. The map stays key-sorted while the flat list
            // preserves successful Add order, with each node borrowing the
            // exact Character owned by the existing actor projection.
            Fixture first(0x2501), second(0x2502);
            tx::Result result{};
            require(tx::register_after_add(&first.manager, &first.characters, 52,
                        &first.object, &first.character, false, &result) ==
                        tx::Status::complete,
                    "first shared-owner Add failed");
            require(tx::register_after_add(&first.manager, &first.characters, 51,
                        &second.object, &second.character, false, &result) ==
                        tx::Status::complete,
                    "second shared-owner Add failed");
            Manager::Cursor cursor{};
            first.manager.reset(&cursor);
            ManagerObject* entry = nullptr;
            require(first.manager.next(&cursor, &entry) ==
                        dh2::object_manager_runtime_owner_v1::Status::ok && entry &&
                        entry->source_handle == 51 &&
                        first.manager.next(&cursor, &entry) ==
                        dh2::object_manager_runtime_owner_v1::Status::ok && entry &&
                        entry->source_handle == 52,
                    "shared manager did not retain source-key iteration order");
            const auto& list = first.characters.source();
            require(list.character_count == 2 &&
                        list.sentinel.next->character == &first.character &&
                        list.sentinel.next->next->character == &second.character,
                    "shared CharacterList did not retain Add insertion order");
            require(tx::remove_after_remove(&first.manager, &first.characters, 52,
                        &first.character, &result) == tx::Status::complete &&
                        tx::remove_after_remove(&first.manager, &first.characters, 51,
                        &second.character, &result) == tx::Status::complete &&
                        first.manager.object_count() == 0 &&
                        first.characters.owned_nodes() == 0,
                    "shared-owner Remove left a map entry or borrowed Character node");
        }

        {
            Fixture f(0x3003);
            tx::Result result{};
            require(tx::register_after_add(&f.manager, &f.characters, 31,
                        &f.object, &f.character, true, &result) ==
                        tx::Status::duplicate_noop,
                    "resolved-name duplicate did not preserve Add no-op behavior");
            require(f.manager.object_count() == 0 &&
                        f.characters.owned_nodes() == 0,
                    "resolved-name duplicate changed an owner");
        }

        {
            // Source Character construction completes CharAI construction and
            // Character association before ObjectManager::Add publishes the
            // same native Character identity into its map and CharacterList.
            Fixture f(0x4004);
            dh2::character_ai_initialization::State ai{};
            ai.identity = reinterpret_cast<std::uintptr_t>(&ai);
            CharAIQueueFixture queue;
            const dh2::character_ai_initialization::Services services{
                &queue, append_char_ai};
            dh2::character_ai_initialization::Result constructed{};
            require(dh2::character_ai_initialization::construct(
                        &ai, 0xD2CA1001u, &services, &constructed) ==
                        dh2::character_ai_initialization::Status::complete &&
                        constructed.queue_calls == 1 && constructed.queued == 1 &&
                        queue.order.size() == 1 && queue.order.front() == ai.identity,
                    "Character factory did not construct/register its one CharAI");
            require(dh2::character_ai_association::associate(&ai, f.character.identity) ==
                        dh2::character_ai_association::Status::complete &&
                        ai.owner_04 == f.character.identity && !ai.active_ais_1c &&
                        !ai.alternate_ais_20,
                    "CharAI was not associated to the canonical Character identity");
            f.object.source_handle = 41;
            tx::Result result{};
            require(tx::register_after_add(&f.manager, &f.characters, 41,
                        &f.object, &f.character, false, &result) == tx::Status::complete &&
                        f.manager.find_by_identity(f.character.identity) &&
                        f.characters.contains_identity(f.character.identity),
                    "factory-created Character was not published to shared owners");
            require(tx::remove_after_remove(&f.manager, &f.characters, 41,
                        &f.character, &result) == tx::Status::complete &&
                        f.manager.object_count() == 0 &&
                        f.characters.owned_nodes() == 0 && queue.order.size() == 1,
                    "ObjectManager teardown destroyed CharAI queue ownership unexpectedly");
            queue.order.clear(); // CharAI teardown owns its queue removal separately.
        }

        std::cout << "{\"registration\":true,\"duplicate_identity\":true,"
                     "\"failure_prefix_rolled_back\":true,\"paired_teardown\":true,"
                     "\"shared_dact_add_remove_order\":true,"
                     "\"duplicate_name_noop\":true,\"character_charai_factory_prefix\":true}\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
