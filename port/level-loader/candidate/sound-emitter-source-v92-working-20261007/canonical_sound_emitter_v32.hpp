#pragma once
#include "canonical_family_fields_v15.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include <optional>
namespace dh2::world {
class CanonicalSoundEmitterV32;
struct SoundEmitterServicesV32 {
 std::shared_ptr<void> owner;
 std::shared_ptr<const std::vector<std::string>> sound_names;
 std::function<bool(CanonicalSoundEmitterV32&,std::string&)> whole_update,whole_destroy;
 // Exact inherited GameObjectD2 and existing JNI nativeStopSoundBig primitive.
 // Neither is the SoundEmitterD1 body: that source order is implemented here.
 std::function<bool(CanonicalSoundEmitterV32&,std::string&)> destroy_base_v92;
 std::function<bool(std::int32_t,std::string&)> platform_stop_v92;
};
// Original factory340ccc/C1 39551c. Sound name resolution is original
// InitPost395144; playback/update and shutdown borrow the main audio system.
class CanonicalSoundEmitterV32 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 initialization_services_;
 GameObjectInitializationOwnerV1 initialization_;
 SoundEmitterServicesV32 services_;
 std::optional<std::uint8_t> loop374_;
 std::string sound378_;
 std::int32_t sound_id390_{-1};float dist_min394_{-1},dist_max398_{-1};std::uint8_t byte39c_{};
 bool destroyed_{},destruction_started_{},destroying_{};
 bool audio_stop_complete_v92_{},string_released_v92_{}; // actual completed cleanup prefixes only
public:
 CanonicalSoundEmitterV32(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,SoundEmitterServicesV32);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept;
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);
 bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);
 bool write_float(std::uint32_t,float,std::string&);
 bool write_string(std::uint32_t,const std::string&,std::string&);
 const std::optional<std::uint8_t>& loop()const noexcept{return loop374_;}
 const std::string& sound()const noexcept{return sound378_;}
 std::int32_t sound_id()const noexcept{return sound_id390_;}
 float distance_min()const noexcept{return dist_min394_;}float distance_max()const noexcept{return dist_max398_;}
 std::uint8_t source_byte39c()const noexcept{return byte39c_;}
 std::uint8_t& source_playing39c_v92()noexcept{return byte39c_;}
 std::string& source_sound378_v92()noexcept{return sound378_;}
 bool& source_audio_stop_complete_v92()noexcept{return audio_stop_complete_v92_;}
 bool& source_string_released_v92()noexcept{return string_released_v92_;}
 bool destruction_started_v92()const noexcept{return destruction_started_;}
 bool init_post(std::string&);bool update(std::string&);bool destroy(std::string&);
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool set_position(const std::array<float,3>&,bool,std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalSoundEmitterV32>,std::shared_ptr<const void>);
};
}
