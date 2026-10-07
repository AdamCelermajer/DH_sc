#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
// Actual AnimController::s_scalingEnabled process .data999841 starts1.
// Enter/ExitCutscene are source writers; this is not a HUD display flag.
std::uint8_t source_animation_scaling_enabled_v99() noexcept;
void source_store_animation_scaling_v99(std::uint8_t) noexcept;
using VisualTimelineCallbackV4=void(*)(std::uintptr_t,void*);
using VisualEventCallbackV4=void(*)(const void*,void*);
struct VisualAnimatorBorrowV4 {
 std::uintptr_t identity{};void* context{};
 bool(*set_callbacks)(void*,VisualTimelineCallbackV4,void*,VisualEventCallbackV4,void*,std::string&){};
};
struct VisualAnimControllerRootServicesV4 {
 void* context{};
 bool(*get_animators)(void*,const std::vector<VisualAnimatorBorrowV4>*&,std::string&){};
 bool(*remove_animators)(void*,std::string&){};
};
// Actual retained native controller object. Its root lease is the SAME root
// allocation, implementing source grab/drop lifetime; no unrelated raw-count
// metadata or second scene is created. Foreign ARM vptr layout is not claimed.
class VisualAnimControllerOwnerV4 {
 std::shared_ptr<void> root_;
 std::uintptr_t root_identity_{};
 VisualAnimControllerRootServicesV4 services_;
 bool attempted_{};
 static void do_nothing_timeline(std::uintptr_t,void*)noexcept{}
 static void do_nothing_event(const void*,void*)noexcept{}
public:
 VisualAnimControllerOwnerV4()=default;
 virtual ~VisualAnimControllerOwnerV4(){close();}
 VisualAnimControllerOwnerV4(const VisualAnimControllerOwnerV4&)=delete;
 VisualAnimControllerOwnerV4& operator=(const VisualAnimControllerOwnerV4&)=delete;
 bool construct(const std::shared_ptr<void>& actual_root,std::uintptr_t,
  VisualAnimControllerRootServicesV4,bool remove_animators,std::string&);
 bool set_callbacks_on_all(VisualTimelineCallbackV4,void*,VisualEventCallbackV4,void*,std::string&);
 void close()noexcept{root_.reset();root_identity_=0;}
 std::uintptr_t root_identity()const noexcept{return root_identity_;}
};
}
