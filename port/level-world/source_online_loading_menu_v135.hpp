#pragma once
#include "source_online_loading_owner_v55.hpp"
#include <loading_menu_v1.hpp>
#include <memory>
namespace dh2::application {
// The authored Loading.BarState calls completion/host queries at progress100
// even offline. Supply the actual process COnline byte5 before their lazy
// offline branches. Positive PlayerManager/Matching leaves stay required.
inline ui::LoadingMenuMultiplayerServicesV1 source_online_loading_menu_v135(
 const std::shared_ptr<SourceOnlineLoadingOwnerV55>& online) {
 ui::LoadingMenuMultiplayerServicesV1 services;
 if(!online)return services;
 services.context=online.get();services.context_owner=online;
 services.enabled=[](void* context,bool& enabled,std::string& error){
  enabled=static_cast<SourceOnlineLoadingOwnerV55*>(context)->byte5()!=0;
  error.clear();return true;
 };
 return services;
}
}
