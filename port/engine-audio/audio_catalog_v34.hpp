#pragma once
#include "audio_mixer_v34.hpp"
#include <map>
#include <string>
#include <vector>
namespace dh2::audio {
struct AudioSoundV34 {int uid{},bank{},group{},priority{},format{},loading_flags{};bool loop{};std::string label,filename;};
struct AudioGroupV34 {int uid{},position_type{},volume_group{};std::string name,bus;};
struct AudioEventV34 {int uid{},type{},history_limit{},probability{100};std::string label;std::vector<int> source,remaining,history;unsigned cursor{};};
struct AudioRandomV34 {void* context{};bool(*next)(void*,int&){};};
// Soundpack labels/UIDs and source authored names, not suffix candidates.
class AudioCatalogV34 {
 std::vector<AudioSoundV34> sounds_;std::vector<AudioBankV34> banks_;
 std::vector<AudioGroupV34> groups_;std::vector<AudioEventV34> events_;
 std::map<std::string,int> sound_labels_,event_labels_;
 std::map<std::string,std::uint32_t> group_masks_;
public:
 bool load_xml(const std::vector<std::uint8_t>&,std::string&);
 const AudioSoundV34* sound(int uid)const noexcept;
 const AudioGroupV34* group(int uid)const noexcept;
 int sound_uid(const char*)const noexcept;int event_uid(const char*)const noexcept;
 bool group_mask(const char*,std::uint32_t&)const noexcept;
 bool select_event(int event,AudioRandomV34,int& sound,std::string&);
 bool reset_event(int,std::string&);
 const auto& sounds()const noexcept{return sounds_;}
 const auto& banks()const noexcept{return banks_;}
 const auto& groups()const noexcept{return groups_;}
 const auto& events()const noexcept{return events_;}
};
// Exact SoundAutoGen source shape: UID, event flag. The older filename schema
// is not accepted as this shape and is never silently reinterpreted.
struct AudioSoundAutoGenV34 {std::int32_t uid{},event{};};
bool audio_sound_autogen_rows_v34(const std::uint8_t*,std::size_t,
 std::vector<AudioSoundAutoGenV34>&,std::string&);
}
