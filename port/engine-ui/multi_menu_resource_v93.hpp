#pragma once
#include "menu_manager_unload_v58.hpp"
#include "viewport.hpp"
#include <functional>
#include <utility>
namespace dh2::ui {
// Source437df0 allocates MenuFlash2DCamera52, C1 42ccd0, slot+144.
// This is its existing source camera payload, not a font/input substitute.
struct MenuFlash2DCameraOwnerV93 {
 std::uintptr_t movie{};
 std::int32_t half_dimensions[2]{};
 bool deleted{};
 MenuFlash2DCameraOwnerV93(std::uintptr_t fx,std::int32_t width,std::int32_t height)
  :movie(fx),half_dimensions{width/2,height/2},owned_(std::make_shared<FlashCamera40>()),owner_(owned_),state_(owned_.get()){}
 // HUD/character generation can lend its EXISTING camera payload. Weak owner
 // avoids owner -> camera envelope -> owner cycles; borrow pins it per call.
 MenuFlash2DCameraOwnerV93(std::uintptr_t fx,std::int32_t width,std::int32_t height,
                          std::weak_ptr<void> actual_owner,FlashCamera40& actual_state)
  :movie(fx),half_dimensions{width/2,height/2},owner_(std::move(actual_owner)),state_(&actual_state){}
 bool borrow_state(FlashCamera40*& out,std::shared_ptr<void>& owner,std::string& e){
  out=nullptr;owner.reset();if(deleted||!state_||!(owner=owner_.lock())){e="Required live SAME MenuFlash2DCamera payload owner";return false;}
  out=state_;e.clear();return true;
 }
private:
 std::shared_ptr<FlashCamera40> owned_;
 std::weak_ptr<void> owner_;
 FlashCamera40* state_{};
};
struct MenuCameraBorrowV93 {std::shared_ptr<void> actual_owner;std::uintptr_t identity{};};
// All providers refer to the SAME current slot/active array. No second table,
// copied stack, Hide/Pop approximation, or inferred empty resource is created.
struct MultiMenuResourceServicesV93 {
 std::shared_ptr<void> actual_manager;
 std::function<bool(std::uint32_t,MenuMovieBorrowV58&,std::string&)> movie_slot;
 std::function<bool(std::int32_t&,std::string&)> active_count;
 std::function<bool(std::uint32_t,std::uintptr_t&,std::string&)> active_at;
 std::function<bool(std::uint32_t,std::uintptr_t,std::string&)> remove_active;
 std::function<bool(const MenuMovieBorrowV58&,std::string&)> movie_virtual_c;
 std::function<bool(std::uint32_t,const MenuMovieBorrowV58&,std::string&)> deleting_virtual4;
 std::function<bool(std::uint32_t,std::string&)> clear_movie_slot;
 std::function<bool(std::uint32_t,MenuCameraBorrowV93&,std::string&)> camera_slot;
 std::function<bool(std::uint32_t,const MenuCameraBorrowV93&,std::string&)> deleting_camera_virtual4;
 std::function<bool(std::uint32_t,std::string&)> clear_camera_slot;
 //Optional native continuation only for an admitted post-D0 field-clear prefix.
 //resumed=true bypasses completed movie Unload/D0, then continues its camera tail.
 std::function<bool(std::uint32_t,bool& resumed,std::string&)> resume_movie_clear_v98;
};
// Original437b3c..437c18. Missing reached producers fail, with prefix retained.
bool multi_menu_unload_swf_v93(const MultiMenuResourceServicesV93&,std::int32_t,std::string&);
}
