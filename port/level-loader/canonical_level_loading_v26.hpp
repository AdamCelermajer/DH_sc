#pragma once
#include "canonical_level_context_v1.hpp"
#include "../engine-ui/loading_menu_v1.hpp"
namespace dh2::loader {
// Original UI callbacks over the SINGLE actual GSLevel global and live C1
// fields. It never supplies progress from a timer or marks Level.Init complete.
class CanonicalLevelLoadingV26 : public std::enable_shared_from_this<CanonicalLevelLoadingV26> {
 CanonicalGSLevelGlobalSlotV1 globals_;
 std::function<bool(std::uint32_t&,std::string&)> online_;
 static bool current(void*,std::uintptr_t&,std::string&);
 static bool progress(void*,std::uintptr_t,std::uint32_t&,std::string&);
 static bool state(void*,std::uintptr_t,std::uint32_t&,std::string&);
 static bool online(void*,std::uint32_t&,std::string&);
 static bool advance(void*,std::uintptr_t,std::uint32_t,std::uint32_t,std::string&);
 bool fields(std::uintptr_t,CanonicalLevelContextV1::LoadingFieldsV26&,std::string&);
public:
 CanonicalLevelLoadingV26(CanonicalGSLevelGlobalSlotV1 actual,std::function<bool(std::uint32_t&,std::string&)> online):globals_(std::move(actual)),online_(std::move(online)){}
 // This bridge owns only the process GS global/provider, never its GS or World.
 // A retained Front packet therefore survives retirement and observes the same
 // empty/re-published global on a late callback or the next campaign.
 ui::LoadingMenuStateServicesV1 services()noexcept{return {this,current,progress,online,state,advance,weak_from_this().lock()};}
};
}
