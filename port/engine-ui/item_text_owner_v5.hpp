#pragma once
#include "hud_text_v1.hpp"
#include "item_text_varargs_v5.hpp"
namespace dh2::ui {
// Connects genuine owned item/Character/cache/localization data. All borrowed
// owners/environment must outlive this stable context and bound inventory.
// No class/item/name dictionary fallback, fake formatter or default language.
class ItemTextOwnerV5 {
 const data::ItemTable* items_;const data::CharacterTable* characters_;HudTextV1* text_;HudTextEnvironmentV1 environment_;
 static const data::Item* metadata(void*,const data::ItemInstanceV1&,std::string&);
 static bool invoke(void*,data::ItemInstanceV1&,const data::ItemTextRequestV5&,data::ItemTextResponseV5&,std::string&,std::string&);
 bool raw_string(std::int32_t,std::string&,std::string&);
 bool constant(const char*,const char*,std::int32_t&,std::string&);
public:
 ItemTextOwnerV5(const data::ItemTable&,const data::CharacterTable&,HudTextV1&,const HudTextEnvironmentV1&);
 ItemTextOwnerV5(const ItemTextOwnerV5&)=delete;ItemTextOwnerV5& operator=(const ItemTextOwnerV5&)=delete;
 data::ItemTextServicesV5 services()noexcept{return {this,metadata,invoke};}
};
}
