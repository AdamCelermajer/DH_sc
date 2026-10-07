#pragma once
#include <cstdint>
#include <deque>
#include <string>
#include <memory>
namespace dh2::character {
// Ordered observations, not cached replacement actor state. Each callback must
// borrow the registered AI owner's actual controller/Character fields.
enum class WorldAIQueueQueryV1 : std::uint32_t {
 Forced, GlobalBlocked, Locked, Dead, RemotelyUpdated, Byte80, Zoned,
 Byte2ee, Byte2f0, Follower, Faerie, Player
};
struct WorldAIQueueServicesV1 {
 void* context{};
 bool (*query)(void*,std::uintptr_t,WorldAIQueueQueryV1,std::int32_t&,std::string&){};
};
class CharacterWorldAIQueueV1 {
 public:
 // Call at the actual CharAI construction/destruction boundaries.
 bool add(std::uintptr_t ai);
 bool remove(std::uintptr_t ai);
 bool advance(std::int32_t dt,const WorldAIQueueServicesV1&);
 bool is_my_turn(std::uintptr_t ai,const WorldAIQueueServicesV1&,bool&);
 std::int32_t countdown() const { return countdown_; }
 const std::deque<std::uintptr_t>& actors() const { return actors_; }
 const std::string& error() const { return error_; }
 private:
 bool read(const WorldAIQueueServicesV1&,std::uintptr_t,WorldAIQueueQueryV1,std::int32_t&);
 void rotate();
 std::deque<std::uintptr_t> actors_;
 // Original ELF data word 0x999760, rather than a guessed zero BSS field.
 std::int32_t countdown_{-1};
 std::string error_;
};
// Same source process deque/countdown; not a fresh per-Level scheduler.
std::shared_ptr<CharacterWorldAIQueueV1> character_ai_queue_v105();
}
