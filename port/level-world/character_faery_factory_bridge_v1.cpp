#include "character_faery_factory_bridge_v1.hpp"

#include <limits>
#include <cstring>
#include <vector>

namespace dh2::character_faery_factory_bridge_v1 {
namespace classification = character_ai_classification;
namespace data = ::dh2::data;
namespace {

struct TypeQueryContext {
    std::uintptr_t character{};
    const data::AiTables* source_tables{};
    std::vector<classification::AiRow> rows;
    classification::AiTable table{};
};

int invoke_type_query(void* raw, classification::State* state,
                      const classification::Request* request,
                      classification::Response* response) {
    auto* context = static_cast<TypeQueryContext*>(raw);
    if (!context || !state || !request || !response ||
        request->character != context->character ||
        state->character != context->character || !context->source_tables)
        return 1;
    *response = {};
    if (request->operation == classification::Operation::ai_table) {
        const auto& source_rows = context->source_tables->rows;
        if (source_rows.empty() || source_rows.size() > 65536) return 1;
        try {
            context->rows.clear();
            context->rows.reserve(source_rows.size());
            for (const auto& row : source_rows)
                context->rows.push_back({row.flags, row.type});
        } catch (...) {
            return 1;
        }
        context->table = {context->rows.data(),
            static_cast<std::uint32_t>(context->rows.size())};
        response->table = &context->table;
        return 0;
    }
    if (request->operation == classification::Operation::ai_count) {
        const auto count = context->source_tables->rows.size();
        if (count > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()))
            return 1;
        response->count = static_cast<std::int32_t>(count);
        return 0;
    }
    return 1;
}

} // namespace

Status register_character(factory::Owner& characters,
                          manager::SourceHandle source_handle,
                          std::int32_t source_character_type,
                          links::Role role, links::Owner& link_owner,
                          std::string& error) {
    error.clear();
    auto* record = characters.find(source_handle);
    if (!record) {
        error = "Faery bridge source handle has no live factory Character";
        return Status::missing_factory_character;
    }
    auto* ai_owner = record->canonical_ai();
    if (!ai_owner) {
        error = "Faery bridge Character has no matching canonical CharAI owner";
        return Status::missing_canonical_ai;
    }
    const auto status = link_owner.register_character(
        *record, *ai_owner, source_character_type, role, error);
    if (status != links::Status::complete)
        return Status::link_owner_rejected;
    error.clear();
    return Status::complete;
}

Status register_faery_from_source(factory::Owner& characters,
                                  manager::SourceHandle source_handle,
                                  const data::AiTables& ai_tables,
                                  links::Owner& link_owner,
                                  std::string& error) {
    error.clear();
    auto* record = characters.find(source_handle);
    if (!record) {
        error = "Faery bridge source handle has no live factory Character";
        return Status::missing_factory_character;
    }
    auto* ai_owner = record->canonical_ai();
    if (!ai_owner) {
        error = "Faery bridge Character has no matching canonical CharAI owner";
        return Status::missing_canonical_ai;
    }
    if (!record->nonplayer_ai_postload_complete ||
        record->nonplayer_source_ai_id < 0 ||
        ai_owner->owner_04 != record->character.identity) {
        error = "Faery CharType requires the completed canonical non-player AI InitPost branch";
        return Status::source_ai_not_ready;
    }
    if (ai_tables.rows.empty() || ai_tables.rows.size() > 65536) {
        error = "Faery CharType source AIProps table is unavailable or outside the port bound";
        return Status::source_type_unavailable;
    }

    TypeQueryContext context{};
    context.character = record->character.identity;
    context.source_tables = &ai_tables;
    classification::State state{record->character.identity,
        record->nonplayer_source_ai_id, -1, nullptr, 0};
    const classification::Services services{&context, &invoke_type_query};
    classification::Result type{};
    const auto query_status = classification::query(
        classification::Query::type, &state, &services, &type);
    if (query_status != classification::Status::complete) {
        error = "Character::GetCharType could not resolve the retained source AIProps row";
        return Status::source_type_query_failed;
    }
    std::int32_t source_type = 0;
    static_assert(sizeof(source_type) == sizeof(type.word));
    std::memcpy(&source_type, &type.word, sizeof(source_type));
    const auto status = link_owner.register_character(
        *record, *ai_owner, source_type, links::Role::faery, error);
    if (status != links::Status::complete)
        return Status::link_owner_rejected;
    error.clear();
    return Status::complete;
}

Status unregister_character(factory::Owner& characters,
                            manager::SourceHandle source_handle,
                            links::Owner& link_owner,
                            std::string& error) noexcept {
    error.clear();
    const auto* record = characters.find(source_handle);
    if (!record) {
        error = "Faery bridge source handle has no live factory Character";
        return Status::missing_factory_character;
    }
    const auto status = link_owner.unregister_character(
        record->constructor.identity, error);
    if (status != links::Status::complete)
        return Status::link_owner_rejected;
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_faery_factory_bridge_v1
