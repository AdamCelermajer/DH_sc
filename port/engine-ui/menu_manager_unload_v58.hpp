#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::ui {
struct MenuMovieBorrowV58 {
 std::shared_ptr<void> actual_owner;
 std::uintptr_t identity{};
};
struct MenuReceiverBorrowV58 {
 std::uintptr_t identity{},render{};
 std::uint8_t owned7d{};
};
// MenuManager.UnloadMenu42d4bc. Registry operations refer to its live 64..68
// directory, not an independent menu list or a Hide/Pop approximation.
struct MenuManagerUnloadServicesV58 {
 std::shared_ptr<void> actual_manager;
 std::function<bool(std::string&)> clear_map_icons;
 std::function<bool(std::uint32_t,MenuMovieBorrowV58&,std::string&)> movie_slot;
 std::function<bool(std::uint32_t&,std::string&)> registry_count;
 std::function<bool(std::uint32_t,MenuReceiverBorrowV58&,std::string&)> registry_at;
 std::function<bool(std::uintptr_t,std::string&)> clear_render;
 std::function<bool(std::uintptr_t,std::uint8_t&,std::string&)> read_owned7d;
 std::function<bool(std::uintptr_t,std::string&)> deleting_virtual4;
 std::function<bool(std::uint32_t,std::uintptr_t,std::string&)> erase_registry;
 std::function<bool(const MenuMovieBorrowV58&,std::string&)> reset_scan_for_anims;
 std::function<bool(std::int32_t,std::string&)> unload_swf_file;
};
class MenuManagerUnloadV58 {
 MenuManagerUnloadServicesV58 services_;
 bool busy_{};
public:
 explicit MenuManagerUnloadV58(MenuManagerUnloadServicesV58);
 bool unload(std::int32_t,std::string&);
};
}
