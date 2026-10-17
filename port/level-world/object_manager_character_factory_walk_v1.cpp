#include "object_manager_character_factory_walk_v1.hpp"

#include <vector>

namespace dh2::object_manager_character_factory_walk_v1 {

Status walk(manager::Owner& objects, const factory::Owner& characters,
            const Services& services, Result* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !services.visit) {
        error = "ObjectManager Character walk requires output and visitor";
        return Status::invalid_argument;
    }

    struct IdentityRow {
        manager::SourceHandle handle;
        manager::Address identity;
    };
    std::vector<IdentityRow> rows;
    manager::Owner::Cursor cursor{};
    objects.reset(&cursor);
    for (;;) {
        manager::GameObject* object = nullptr;
        const auto status = objects.next(&cursor, &object);
        if (status != manager::Status::ok) {
            error = "ObjectManager Character walk found an invalid map row";
            return Status::manager_failed;
        }
        if (!object) break;
        if (!object->identity) {
            error = "ObjectManager Character walk found an identity-free map row";
            return Status::manager_failed;
        }
        try {
            rows.push_back({object->source_handle, object->identity});
        } catch (...) {
            error = "ObjectManager Character walk snapshot allocation failed";
            return Status::manager_failed;
        }
    }
    output->manager_rows = rows.size();

    for (const auto& row : rows) {
        const auto* record = characters.find(row.handle);
        if (!record) continue;
        const auto* object = objects.find_by_source_handle(row.handle);
        if (!object || object->identity != row.identity ||
            record->source_handle != row.handle ||
            record->game_object.identity != row.identity ||
            record->character.identity != row.identity ||
            !record->object_registered || !record->character_listed) {
            error = "ObjectManager Character row no longer matches its Factory owner";
            return Status::identity_mismatch;
        }
        try {
            if (services.visit(services.context, *record, error) != 0) {
                if (error.empty()) error = "ObjectManager Character visitor failed";
                return Status::callback_failed;
            }
        } catch (...) {
            if (error.empty()) error = "ObjectManager Character visitor threw";
            return Status::callback_failed;
        }
        ++output->characters;
    }
    return Status::complete;
}

} // namespace dh2::object_manager_character_factory_walk_v1
