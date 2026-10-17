#pragma once

#include "quest_condition_eval_v1.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::data::condition_data_v1 {

struct Bytes {const std::uint8_t* data=nullptr;std::uint32_t size=0;};
struct Predicate {std::int32_t operation=0,quest_id=0,required_state=0;};
struct Definition {std::string name;std::vector<Predicate> predicates;std::int32_t source_type=0;};
struct Table {std::vector<Definition> rows;};
enum class Status:std::uint32_t {complete,invalid_argument,malformed,unsupported_operation,service_unavailable};
struct EvalResult {Status status=Status::complete;bool value=false;std::uint32_t predicates_evaluated=0;};
using QuestStateLookup=bool(*)(void*,std::int32_t quest_id,std::int32_t* state);

// `v2ConditionStub::read` loads a count, count x 12-byte (Op, Quest, State)
// records, then its trailing source Type. The four input tables are passed as
// immutable bytes from the unmodified cache copy in Android assets.
bool load(Bytes packed,Bytes names,Bytes schema,Bytes constants,Table& out,std::string& error);
const Definition* find(const Table&,const char* name)noexcept;
// Mirrors ConditionData::IsTrue plus ConditionList::Eval for the recovered
// quest-state operation families only. Lookup must be the canonical
// SG_GetQuestByID(-1) source path; a miss returns false, as the original does.
Status evaluate(const Definition&,bool tested,QuestStateLookup,void*,EvalResult*)noexcept;

} // namespace dh2::data::condition_data_v1
