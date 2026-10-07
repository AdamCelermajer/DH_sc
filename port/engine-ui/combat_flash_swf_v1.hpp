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
struct CombatFlashScanReceiptV92 {
 std::uintptr_t backend{},movie{},graph{};
 std::uint64_t binding_epoch{},scan_serial{};
};
class CombatFlashSwfV1 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 // Process backend constructs the SAME twelve contexts only once.
 CombatFlashSwfV1();
 // Legacy standalone fixture: caller must outlive this borrowed backend.
 CombatFlashSwfV1(SwfMovie&,CombatFlashProjectionV1);
 bool bind_movie_v92(const std::shared_ptr<SwfMovie>&,
                     std::weak_ptr<void> actual_projection_owner,
                     CombatFlashProjectionV1,std::string&);
 bool scan_receipt_v92(CombatFlashScanReceiptV92&,std::string&);
 bool validate_receipt_v92(const CombatFlashScanReceiptV92&)const noexcept;
 bool reset_movie_v92(std::uintptr_t expected_movie,std::string&);
 bool reset_scan_fields_v92(std::string&);
 bool discard_unscanned_binding_v92(std::uintptr_t expected_movie,std::string&);
 std::uintptr_t bound_movie_v92()const noexcept;
 bool busy_v92()const noexcept;
 bool source_frame_rate_v92(float&,std::string&);
 bool update_interval_v92(std::uint32_t,std::int32_t,std::string&);
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
