#pragma once
#include "player_saved_quests_v1.hpp"
#include <cstdint>

namespace dh2::data::quest_stream_read_v1 {
using StreamRef=player_saved_quests_v1::StreamRef;
enum class Primitive:std::uint32_t {unsigned_word,signed_word,quest_signed_word};
struct AssertionRequest {
 Primitive primitive=Primitive::unsigned_word;
 std::uint32_t source_function=0,source_caller=0,line=0;
 const char* format=nullptr;const char* condition=nullptr;const char* file=nullptr;
};
struct Services {
 void* context=nullptr;
 // Actual IStreamBase virtual+18. The provider writes the supplied live field
 // directly, advances the one borrowed cursor, and returns the full64 count.
 // A delivered short read returns service success with its actual byte count.
 std::int32_t (*read)(void*,StreamRef&,void* destination,std::uint64_t requested,
                      std::uint64_t* returned)=nullptr;
 std::int32_t (*assertion_mode)(void*,std::int32_t*)=nullptr;
 std::int32_t (*log_assert)(void*,const AssertionRequest&)=nullptr;
};
enum class Operation:std::uint32_t {none,read,assertion_mode,log_assert,complete};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_assertion,reentrant,projection_changed};
struct Result {
 Status status=Status::complete;Operation last_operation=Operation::none;
 std::uint32_t service_calls=0,read_calls=0,assertion_reads=0,assertion_logs=0,
 source_function=0,source_caller=0;
 std::uint64_t requested=0,returned=0;
};
class Runtime {
 StreamRef& stream_;Services services_;bool busy_=false;
 Status execute(void* destination,Primitive,Result*);
public:
 Runtime(StreamRef& stream,Services services):stream_(stream),services_(services){}
 Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
 Status read_unsigned(std::uint32_t* destination,Result*);
 Status read_signed(std::int32_t* destination,Result*);
 Status read_quest_signed(std::int32_t* destination,Result*);
};
// Whole direct-destination IStreamBase::readAs<unsigned>/readAs<int> and
// StreamReader::readAs<int>(stream,int*) bodies. All request4/0 bytes at virtual
// slot18; return count must equal uint64(4). Partial field/cursor writes remain
// even when delivery fails, assertion providers fail, or mode2 reaches the
// original null-store boundary. Mode2 reports source_assertion; it cannot return
// successfully through that unsafe source store. Mode1 requires the real logger;
// other actual modes return with current destination contents. No scratch,
// cursor, bytes, Save, property or Quest owner is created. Native alias,
// alignment, reentry, provider-error and identity guards are separate policies.
}
