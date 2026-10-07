#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
namespace dh2::ui {
struct CombatFlashContextV1 {
 std::int32_t x{},y{},color{},frame{},timer{};std::uint32_t flags{};
 std::int32_t style{-1},instance{};std::array<char,48> text{};
};
struct CombatFlashInstanceV1 {std::uintptr_t clip{},text{};bool busy{};};
struct CombatFlashStyleV1 {std::string name;std::array<CombatFlashInstanceV1,8> instances;};
struct CombatFlashServicesV1 {
 void* context{};
 // Scan exact RenderFX FindCharacters(root,"anim_",0) order and actual names.
 int(*scan)(void*,std::vector<CombatFlashStyleV1>&){};
 int(*clone)(void*,std::uintptr_t source,const char* name,CombatFlashInstanceV1*){};
 int(*set_text)(void*,std::uintptr_t,const char*){};
 int(*frame_count)(void*,std::uintptr_t,std::int32_t*){};
 int(*project)(void*,const float[3],std::int32_t*,std::int32_t*){};
 int(*inverse_pixel_scale)(void*,std::uintptr_t,float*,float*){};
 // Exact native Draw sequencing: buffering off/begin, per-clip frame/color/
 // temporary visibility/display/position restore, buffering on/end.
 int(*begin)(void*){};
 int(*draw)(void*,std::uintptr_t,std::uintptr_t,const CombatFlashContextV1*){};
 int(*end)(void*){};
};
class CombatFlashQueueV1 {
 std::array<CombatFlashContextV1,12> contexts_{};
 std::vector<CombatFlashStyleV1> styles_;CombatFlashServicesV1 services_;
 mutable std::map<std::string,std::int32_t> names_;
public:
 explicit CombatFlashQueueV1(CombatFlashServicesV1);
 int scan();
 int style_id(const char*)const;
 int play(const char*,const float[3],const char*,std::int32_t,std::int32_t,bool numeric);
 int update(std::uint32_t dt,std::int32_t interval);
 int draw(bool source_debug_disabled);
 void stop_all();
 void reset_scan(); // stops contexts/clears instances; source name map survives
 const std::array<CombatFlashContextV1,12>& contexts()const{return contexts_;}
 const std::vector<CombatFlashStyleV1>& styles()const{return styles_;}
};
}
