#pragma once
#include <character_melee_animation_event_v1.hpp>
#include <character_script_session_v3.hpp>
#include <character_combat_text_v1.hpp>
#include <memory>
#include <functional>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
class SourceCampaignCombatV115;
//The native actor/PM/Debug/Hit/FX/AI portions are supplied by the retained
//campaign. Only actual presentation receivers are borrowed from the UI.
struct SourceCombatPresentationV115 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t,const char*&,std::string&)> localized;
 std::function<bool(const dh2::character::skills::CombatTextRequestV1&,std::string&)> enqueue;
 std::function<bool(std::uintptr_t,std::string&)> critical_camera;
 //Exact reached HitFor player tail. These borrow the existing plain Vox
 //Play and source settings-update job, not Play3D or profile-save jobs.
 std::function<bool(const char*,std::int32_t&,std::string&)> sound_index;
 std::function<bool(std::int32_t,bool,std::int32_t,std::int32_t,bool,std::string&)> play_sound;
 std::function<bool(std::string&)> settings_job_start;
};
bool bind_source_campaign_combat_v115(const SourceCampaignCandidateBorrowV55&,std::string&);
bool bind_source_campaign_combat_presentation_v115(const std::shared_ptr<void>&,
 SourceCombatPresentationV115,std::string&);
bool source_campaign_melee_event_v115(const std::shared_ptr<void>&,std::uintptr_t,
 const dh2::character::skills::MeleeAnimationRequestV1&,
 dh2::character::skills::MeleeAnimationResponseV1&,std::string&);
bool bind_source_character_combat_natives_v115(const std::shared_ptr<void>&,
 std::uintptr_t,dh2::character::CharacterScriptSessionInput&,
 std::shared_ptr<void>&,std::string&);
bool bind_source_character_combat_natives_v115(const std::shared_ptr<void>&,
 std::uintptr_t,dh2::character::CharacterScriptSessionInputV3&,
 std::shared_ptr<void>&,std::string&);
bool source_campaign_forget_combat_actor_v115(const std::shared_ptr<void>&,
 std::uintptr_t,std::string&);
}
