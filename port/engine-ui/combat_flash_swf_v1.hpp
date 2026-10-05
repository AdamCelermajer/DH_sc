#pragma once
#include "combat_flash_queue_v1.hpp"
#include "swf_movie.hpp"
namespace dh2::ui {
struct CombatFlashProjectionV1 {
 void* context{};
 bool(*world_to_screen)(void*,const float[3],std::int32_t*,std::int32_t*,std::string&){};
 // SAME SwfMovie's connected source viewport, not an inspection rectangle.
 bool(*display_rectangle)(void*,float[4],std::int32_t[4],std::string&){};
};
class CombatFlashSwfV1 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 CombatFlashSwfV1(SwfMovie&,CombatFlashProjectionV1);
 ~CombatFlashSwfV1();
 bool scan(std::string&);
 bool play(const char* style,const float[3],const char* text,std::int32_t number,
           std::int32_t color,bool numeric,std::string&);
 // Original Level+130 is the retained _LoadProcess phase, not level kind.
 bool update(std::uint32_t application_dt,std::int32_t actual_level_load_phase,std::string&);
 bool draw(bool actual_debug_disabled,std::string&);
 void stop_all();
 const CombatFlashQueueV1& queue()const;
};
}
