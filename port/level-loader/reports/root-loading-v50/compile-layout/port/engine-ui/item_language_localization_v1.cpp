#include "item_language_localization_v1.hpp"
namespace dh2::ui {
bool item_update_localization_v1(data::ItemInstanceV1& item,const data::ItemTextServicesV5& text,const ItemLanguagePowerServicesV1& powers,ItemLanguageReceiptV1& receipt,std::string& error){
 receipt={};receipt.phase=1;if(!data::item_update_name_v5(item,text,error))return false;
 receipt.phase=2;if(!data::item_update_stats_v5(item,text,error))return false;
 receipt.phase=3;if(!data::item_update_requirements_v5(item,text,error))return false;
 receipt.phase=4;const auto ids=item.powers;
 if(ids.empty()){receipt.phase=7;return true;}
 if(!powers.owner||!powers.clear||!powers.add){error="Required SAME item powered localization owner unavailable";return false;}
 receipt.phase=5;if(!powers.clear(item,error))return false;
 if(!item.powers.empty()){error="Original power erase did not clear actual item IDs";return false;}
 receipt.phase=6;for(const auto id:ids){if(!powers.add(item,id,-1,error))return false;++receipt.powers_rebuilt;}
 receipt.phase=7;return true;
}
}
