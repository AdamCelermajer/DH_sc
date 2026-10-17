#include "character_ai_classification.hpp"

#include <cstddef>
#include <cstring>

namespace dh2::character_ai_classification { namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* value, std::size_t size, std::size_t alignment, Range& out) {
    const auto at = reinterpret_cast<std::uintptr_t>(value);
    if (!value || at % alignment || at > UINTPTR_MAX-size) return false;
    out = {at,at+size}; return true;
}
bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
struct Execution {
    State* state; Services services; Result* result;
    const Range (&protected_ranges)[3];
    Status call(Operation operation, Response& response, const char* name=nullptr) {
        if (!services.invoke) return Status::service_unavailable;
        const Request request{operation,state->character,name};
        ++result->calls;
        try {
            if (services.invoke(services.context,state,&request,&response)) return Status::service_failed;
        } catch (...) { return Status::service_failed; }
        return Status::complete;
    }
    Status id(bool faction, std::int32_t& output) {
        // The source loads the cached word BEFORE its count-global read.
        const auto captured = faction ? state->faction_id : state->ai_id;
        if (captured < 0) { output=faction?10:8; return Status::complete; }
        Response response{};
        auto status=call(faction?Operation::faction_count:Operation::ai_count,response);
        if (status != Status::complete) return status;
        output=captured<response.count?captured:(faction?10:8);
        return Status::complete;
    }
    Status row(const AiRow*& output) {
        Response response{};
        auto status=call(Operation::ai_table,response);
        if (status != Status::complete) return status;
        ++result->table_captures;
        Range table_range{},rows_range{};
        const auto* captured=response.table;
        if (!range(captured,sizeof(*captured),alignof(AiTable),table_range)) return Status::invalid_source_fact;
        for (const auto& p:protected_ranges) if(overlap(table_range,p)) return Status::invalid_source_fact;
        const auto* rows=captured->rows;
        const auto capacity=captured->capacity;
        if (!capacity || capacity>65536 ||
            !range(rows,std::size_t(capacity)*sizeof(AiRow),alignof(AiRow),rows_range)) return Status::invalid_source_fact;
        for (const auto& p:protected_ranges) if(overlap(rows_range,p)) return Status::invalid_source_fact;
        std::int32_t selected=0;
        status=id(false,selected);
        if (status != Status::complete) return status;
        if (selected<0 || static_cast<std::uint32_t>(selected)>=capacity) return Status::invalid_source_fact;
        output=rows+selected; result->row=output;
        return Status::complete;
    }
    Status type(std::int32_t& output) {
        const AiRow* selected=nullptr;
        auto status=row(selected);
        if (status == Status::complete) { output=selected->type; ++result->type_reads; }
        return status;
    }
};
} // namespace

Status query(Query query, State* state, const Services* services, Result* result) {
    Range protected_ranges[3];
    if (static_cast<unsigned>(query)>static_cast<unsigned>(Query::dead) ||
        !range(state,sizeof(*state),alignof(State),protected_ranges[0]) ||
        !range(services,sizeof(*services),alignof(Services),protected_ranges[1]) ||
        !range(result,sizeof(*result),alignof(Result),protected_ranges[2]) || !state->character)
        return Status::invalid_argument;
    for (unsigned i=0;i<3;++i) for(unsigned j=0;j<i;++j)
        if(overlap(protected_ranges[i],protected_ranges[j])) return Status::invalid_argument;
    Execution execution{state,*services,result,protected_ranges}; *result={};
    if (query==Query::dead) { result->word=state->dead; return Status::complete; }
    std::int32_t value=0;
    if (query==Query::ai_id || query==Query::faction_id) {
        const auto status=execution.id(query==Query::faction_id,value);
        if(status==Status::complete) std::memcpy(&result->word,&value,4);
        return status;
    }
    if (query==Query::ai_row || query==Query::miniboss || query==Query::boss || query==Query::sitting) {
        const AiRow* row=nullptr;const auto status=execution.row(row);
        if (status==Status::complete && query!=Query::ai_row)
            result->word=(row->flags>>(query==Query::miniboss?1:query==Query::boss?2:3))&1u;
        return status;
    }
    auto status=execution.type(value);
    if (status!=Status::complete) return status;
    if (query==Query::type) {std::memcpy(&result->word,&value,4);return Status::complete;}
    if (query==Query::npc) {
        if(value==6) {result->word=1;return Status::complete;}
        status=execution.type(value);if(status!=Status::complete)return status;
        if(value==7) {result->word=1;return Status::complete;}
        status=execution.type(value);if(status!=Status::complete)return status;
        result->word=value==8;return Status::complete;
    }
    if (query==Query::player) {
        if(value!=0) {result->word=value==1;return Status::complete;}
        const auto* name=state->name;
        if(!name)return Status::invalid_source_fact;
        Response response{};
        status=execution.call(Operation::find_player_name,response,name);
        if(status==Status::complete)result->word=name==response.match;
        return status;
    }
    const std::int32_t expected=query==Query::monster?4:query==Query::follower?2:
        query==Query::faerie?3:query==Query::summoned?5:query==Query::merchant?7:
        query==Query::invisible_man?9:8;
    result->word=value==expected;return Status::complete;
}
} // namespace dh2::character_ai_classification
