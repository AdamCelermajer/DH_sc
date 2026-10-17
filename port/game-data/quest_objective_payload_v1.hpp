#pragma once
#include "quest_objective_factory_v1.hpp"
#include "player_saved_quests_v1.hpp"
#include <cstdint>
namespace dh2::data::quest_objective_payload_v1 {
using Record=quest_objective_factory_v1::Record;
using StreamRef=player_saved_quests_v1::StreamRef;
enum class Primitive:std::uint32_t {boolean,signed_word};
// The original returned-value readers use uninitialized local stack scratch.
// Callers must explicitly supply its bounded native residue; a short read must
// not manufacture zeros. This transient scratch owns no source gameplay field,
// stream bytes or cursor. Keep it separate from all controls/canonical stores.
struct ReaderScratch {
 std::uint8_t boolean;
 std::uint32_t signed_word;
 ReaderScratch(std::uint8_t b,std::uint32_t word):boolean(b),signed_word(word){}
};
struct AssertionRequest {
 Primitive primitive=Primitive::boolean;
 std::uint32_t source_function=0,source_caller=0,line=0x44;
 const char* format=nullptr;const char* condition=nullptr;const char* file=nullptr;
};
struct Services {
 void* context=nullptr;
 // Actual IStreamBase virtual+18 read: requested and returned counts retain
 // source uint64 width. Writes go to scratch, not directly to Objective fields.
 // Failed delivery/throw retains actual cursor and partial scratch writes.
 std::int32_t (*read)(void*,StreamRef&,void* destination,std::uint64_t requested,
                      std::uint64_t* returned)=nullptr;
 std::int32_t (*assertion_mode)(void*,std::int32_t*)=nullptr;
 // Reached mode1 must use the actual logger owner; no quiet success substitute.
 std::int32_t (*log_assert)(void*,const AssertionRequest&)=nullptr;
};
enum class Operation:std::uint32_t {none,read_boolean,read_signed,assertion_mode,log_assert,store_done,store_quantity,complete};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_assertion,reentrant,projection_changed,outside_domain};
struct Result {
 Status status=Status::complete;Operation last_operation=Operation::none;
 std::uint32_t service_calls=0,read_calls=0,assertion_reads=0,assertion_logs=0,
 field_stores=0,value=0,source_caller=0;
 std::uint64_t requested=0,returned=0;
};
class Runtime {
 Record& record_;Services services_;bool busy_=false;
 Status execute(StreamRef&,ReaderScratch&,std::uint32_t method,Result*);
public:
 Runtime(Record& record,Services services):record_(record),services_(services){}
 Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
 Status load(StreamRef&,ReaderScratch&,Result*);
 Status load_base(StreamRef&,ReaderScratch&,Result*);
 Status load_saved_quantity(StreamRef&,ReaderScratch&,Result*);
 Status read_bool(StreamRef&,ReaderScratch&,Result*);
 Status read_signed(StreamRef&,ReaderScratch&,Result*);
};
// Whole Objective::_loadData, Objective_SavedQty::_loadData and returned-value
// readAs<bool>/readAs<int>. Actual returned count must equal requested size;
// mismatch mode2 stops at the original null-store assertion boundary, mode1
// logs then returns current scratch, other modes return current scratch. The
// raw bool byte is preserved (no normalization); signed word preserves32 bits.
// SavedQty captures its method at entry, publishes done14 after bool delivery,
// then reaches signed reader and only subsequently publishes quantity20.
// Providers may mutate fields/scratch as original leaves permit, but controls,
// projections, Record identity and stream identity remain borrowed and stable.
// Native alias/bounds/reentry/provider-error policies are separate source guards.
}
