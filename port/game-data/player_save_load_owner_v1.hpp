#pragma once
#include "player_savegame_v1.hpp"
#include <functional>
#include <memory>
namespace dh2::data {
struct PlayerSaveProfileV1 {
 std::uintptr_t identity{};
 std::shared_ptr<void> owner;
};
enum class PlayerSaveLoadOpV1 : std::uint32_t {
 filename,create_profile,load_section,init_levels,init_skills,init_faeries,
 init_quests,online,hosting_quest_flag,local_hosting,load_volatile_flag,
 volatile_stream,stream_size,stream_seek,quest_definition,unpack_quests
};
struct PlayerSaveLoadRequestV1 {
 PlayerSaveLoadOpV1 operation;
 PlayerSavegameV1* save{};
 PlayerSaveProfileV1 profile{};
 const char* section{};
 const char* filename{};
 std::uint32_t argument{};
 // CFEE's source reader pointer is null offline or when the source hosting
 // flag is set. The writer callback remains supplied even on that branch.
 bool reader_enabled=true;
 std::int32_t definition{};
};
struct PlayerSaveLoadResponseV1 {
 PlayerSaveProfileV1 profile{};
 std::string text;
 std::uint64_t amount{};
 std::int32_t value{};
 bool flag{};
};
struct PlayerSaveLoadServicesV1 {
 // Retains genuine filename/profile, named-section readers and online/quest
 // producers through synchronous callbacks. No successful missing operation.
 std::shared_ptr<void> owner;
 std::function<bool(const PlayerSaveLoadRequestV1&,PlayerSaveLoadResponseV1&,std::string&)> invoke;
};
class PlayerSaveLoadOwnerV1 {
 friend class PlayerSaveWriteOwnerV1;
 std::shared_ptr<PlayerSavegameV1> save_;
 PlayerSaveProfileV1 profile_;
 PlayerSaveLoadServicesV1 services_;
 std::uint32_t phase_{},calls_{};
 // Original default PlayerSavegame constructor 465ae0: +0xc false,
 // +0x178 zero. The slot constructor leaves +0x178 uninitialized before Load;
 // this retained native Save is the existing default-constructor authority.
 bool save_disabled_{};
 std::int32_t save_mode_{};
 bool send(PlayerSaveLoadRequestV1,PlayerSaveLoadResponseV1&,std::string&);
 bool section(const char*,const PlayerSaveProfileV1&,bool,std::string&);
 bool initialize(PlayerSaveLoadOpV1,std::uint32_t,std::string&);
 bool load_fields(std::uint32_t,std::string&);
 bool load_volatile(std::uint32_t,std::string&);
public:
 // Sole source Save+8 profile authority for THIS same retained Save. Default
 // profile is genuinely null; slot==-1 alone never implies a null profile.
 explicit PlayerSaveLoadOwnerV1(std::shared_ptr<PlayerSavegameV1>,PlayerSaveLoadServicesV1={});
 PlayerSaveLoadOwnerV1(const PlayerSaveLoadOwnerV1&)=delete;
 PlayerSaveLoadOwnerV1& operator=(const PlayerSaveLoadOwnerV1&)=delete;
 PlayerSavegameV1& save()const noexcept{return *save_;}
 const PlayerSaveProfileV1& profile()const noexcept{return profile_;}
 bool save_disabled()const noexcept{return save_disabled_;}
 std::int32_t save_mode()const noexcept{return save_mode_;}
 // Actual source profile publication/rebinding, including callback mutation.
 // Caller must not destroy this owner during an active callback.
 bool publish_profile(PlayerSaveProfileV1,std::string&);
 bool load(std::int32_t source_mask,std::string&);
 std::uint32_t reached_phase()const noexcept{return phase_;}
 std::uint32_t delivered_calls()const noexcept{return calls_;}
};
}
