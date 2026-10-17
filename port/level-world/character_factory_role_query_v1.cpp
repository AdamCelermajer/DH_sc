#include "character_factory_role_query_v1.hpp"

namespace dh2::character_factory_role_query_v1 {

Status query(const factory::Record& record, Role role,
             const Services& services, Result* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || !services.capture ||
        (role != Role::player && role != Role::merchant) ||
        !record.character.identity ||
        record.character.identity != record.game_object.identity ||
        record.character.object != &record.aggro_object ||
        !record.object_registered || !record.character_listed ||
        !record.components.find(factory::ctor::Component::properties)) {
        error = "Character role query requires a registered canonical Factory record";
        return Status::invalid_argument;
    }

    classification::State state{};
    classification::Services source{};
    try {
        if (services.capture(services.context, record, &state, &source, error)) {
            if (error.empty()) error = "Character source-role capture failed";
            return Status::service_unavailable;
        }
    } catch (...) {
        if (error.empty()) error = "Character source-role capture threw";
        return Status::service_unavailable;
    }
    if (state.character != record.character.identity || !source.invoke) {
        error = "Character source-role facts do not match the Factory identity";
        return Status::identity_mismatch;
    }

    classification::Result result{};
    const auto source_role = role == Role::player
        ? classification::Query::player : classification::Query::merchant;
    const auto status = classification::query(source_role, &state, &source, &result);
    output->character_identity = record.character.identity;
    output->source_calls = result.calls;
    if (status != classification::Status::complete) {
        error = "Original Character role predicate could not resolve its source facts";
        return Status::source_query_failed;
    }
    output->value = result.word != 0;
    return Status::complete;
}

} // namespace dh2::character_factory_role_query_v1
