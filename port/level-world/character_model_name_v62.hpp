#pragma once
#include "../game-data/data.hpp"
#include <functional>
#include <memory>
namespace dh2::character {
struct CharacterModelNameServicesV62 {
 std::shared_ptr<void> receiver;
 std::function<bool(bool&,std::string&)> is_faery,is_player,high_performance,is_local_player;
 std::function<bool(std::uintptr_t&,std::string&)> faery_master418;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> current_faery;
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t&,std::string&)> faery_model;
 // Borrow the original global model-CString field at38c/398/3a4.
 // Never substitute DesignSettings' unrelated numeric rows.
 std::function<bool(std::uint32_t,const char*&,std::string&)> low_performance_model;
 std::function<bool(std::string&)> invalid_low_performance_class;
};
bool character_model_name_v62(std::int32_t model_id,std::int16_t property_id,
 const data::Dictionary&,const CharacterModelNameServicesV62&,const char*&,std::string&);
}
