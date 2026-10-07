#pragma once
#include "character_design_services.hpp"
#include "character_skill_ai_v3.hpp"
#include "character_skill_combat_v6.hpp"
#include "../game-data/data.hpp"
#include <array>
#include <memory>
namespace dh2::trophies {
// Original Structs::Trophy (+4..1c), in serialized member order.
struct TrophyRowV1 {std::int32_t desc,gl_index,gl_live,grade,label,name,type;};
// Original TrophyData constructor assignments; no source pointer overlay.
struct TrophyDataV1 {std::int32_t id,name,desc;std::uint8_t unlocked{};
 std::int32_t type,grade,label,gl_live,gl_index;};
class TrophyCatalogV1 {
 std::vector<TrophyRowV1> rows_;std::vector<std::string> names_;
public:
 static std::shared_ptr<const TrophyCatalogV1> load(data::Bytes rows,data::Bytes names,data::Bytes schema,std::string&);
 const std::vector<TrophyRowV1>& rows()const noexcept{return rows_;}
 const std::vector<std::string>& names()const noexcept{return names_;}
 std::int32_t index(const char*)const noexcept;
};
struct AchievementMessageV1 {std::string name,desc;std::int32_t type,grade,label;};
enum TrophyServiceV1:std::uint32_t {resolve_text_v1=1,application_in_game_v1,queue_message_v1,save_bitmap_v1};
struct TrophyRequestV1 {std::uint32_t service;std::int32_t text_id;const AchievementMessageV1* message;const std::array<std::uint32_t,4>* bitmap;};
struct TrophyResponseV1 {std::string text;bool boolean{};};
struct TrophyServicesV1 {void* context{};int(*invoke)(void*,const TrophyRequestV1*,TrophyResponseV1*){};};
class TrophyManagerOwnerV1 {
 std::shared_ptr<const TrophyCatalogV1> catalog_;
 std::vector<TrophyDataV1> data_;std::vector<std::int32_t> unlocking_;
 character::DebugSwitches* debug_;const character::DebugFileServices24* files_;
 TrophyServicesV1 services_;std::string error_;bool initialized_{};
 TrophyManagerOwnerV1(std::shared_ptr<const TrophyCatalogV1>,character::DebugSwitches*,const character::DebugFileServices24*,TrophyServicesV1);
 int tracing();int ask(const TrophyRequestV1&,TrophyResponseV1&);
public:
 static std::unique_ptr<TrophyManagerOwnerV1> create(std::shared_ptr<const TrophyCatalogV1>,character::DebugSwitches*,const character::DebugFileServices24*,TrophyServicesV1,std::string&);
 const TrophyDataV1* get(std::int32_t)const noexcept;
 bool is_unlocked(std::int32_t)const noexcept;bool is_unlocking(std::int32_t)const noexcept;
 int unlock(std::int32_t);int unlocked_callback(std::int32_t);
 // Exact source four-word /128-bit trophies save; no new profile format.
 int bitmap(std::array<std::uint32_t,4>&)const noexcept;
 int save();int load_bitmap(const std::array<std::uint32_t,4>&);
 bool initialized()const noexcept{return initialized_;}
 const TrophyCatalogV1& catalog()const noexcept{return *catalog_;}
 const std::vector<TrophyDataV1>& data()const noexcept{return data_;}
 const std::vector<std::int32_t>& unlocking()const noexcept{return unlocking_;}
 const std::string& error()const noexcept{return error_;}
};
// Borrow this one retained actual manager from SkillAI, HitFor and potion
// continuations. Unknown services remain failures, never accepted providers.
class TrophyNativeBindingsV1 {
 TrophyManagerOwnerV1& owner_;std::vector<const char*> names_;
public:
 explicit TrophyNativeBindingsV1(TrophyManagerOwnerV1&);
 int skill_ai(const character::skills::SkillAIRequest32V3*,character::skills::SkillAIResponse32V3*);
 int hit(const character::HitRequest32*,std::uintptr_t*);
 int unlock_named(const char*);
};
}
