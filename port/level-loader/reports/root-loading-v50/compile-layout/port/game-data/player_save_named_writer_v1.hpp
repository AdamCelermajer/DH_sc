#pragma once
#include "player_save_load_owner_v1.hpp"
namespace dh2::data {
struct PlayerSaveByteWriterV1 {
 std::shared_ptr<void> owner;
 // Consume the passed retained byte span synchronously. Failures retain the
 // previously written stream prefix; never substitute an empty successful sink.
 std::function<bool(Bytes,std::string&)> write;
};
class PlayerSaveNamedWriterV1 {
 std::shared_ptr<PlayerSaveLoadOwnerV1> authority_;
 SkillTables::Borrow skills_;
 bool running_{};
public:
 explicit PlayerSaveNamedWriterV1(std::shared_ptr<PlayerSaveLoadOwnerV1>,SkillTables::Borrow={});
 bool write(const char* section,const PlayerSaveByteWriterV1&,std::string&);
 PlayerSaveLoadOwnerV1& authority()const noexcept{return *authority_;}
};
}
