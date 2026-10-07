#pragma once
#include "authored_character_panel_v2.hpp"
#include "authored_menu_application_fields_v3.hpp"
#include "authored_menu_character_projection_v4.hpp"
namespace dh2::ui {
struct AuthoredSharedMenuServicesV27 {
 std::shared_ptr<void> owner;
 AuthoredMenuApplicationFieldsV3* fields{};
 AuthoredMenuLocalizationServicesV1 localization;
 std::function<bool(const char*,std::int32_t&,std::string&)> debug;
 // Actual manager/touch/render services not implemented by MenuBase itself.
 std::function<bool(MenuStackV1&,MenuStackRequestV1&,std::string&)> remaining;
};
// PostLoad MenuBase receivers discovered from actual already-loaded movies.
// Owns receiver fields/projections only; shares ONE MenuManager directory and
// pins each same CPU movie. Player/Save/actor initialization is not required.
class AuthoredSharedMenuRosterV27 {
 struct Receiver;
 std::shared_ptr<MenuStackOwnerV1> stack_;
 AuthoredSharedMenuServicesV27 services_;
 std::vector<std::unique_ptr<Receiver>> receivers_;
 std::string failure_;
 Receiver* receiver(std::uintptr_t)noexcept;
 bool lifecycle(Receiver&,const AuthoredMenuRequestV1&,std::int32_t&,std::string&);
 static int dispatch(void*,MenuStackV1*,MenuStackRequestV1*);
public:
 AuthoredSharedMenuRosterV27(std::shared_ptr<MenuStackOwnerV1>,AuthoredSharedMenuServicesV27);
 ~AuthoredSharedMenuRosterV27();
 bool post_load(SwfMovie&,std::shared_ptr<void> same_movie_lease,std::uint32_t source_flags,std::string&);
 bool route(MenuStackV1&,MenuStackRequestV1&,bool& handled,std::string&);
 MenuStackServicesV1 stack_services()noexcept{return {this,dispatch};}
 bool invoke(std::uintptr_t actual_menu,const char* method,std::string&);
 bool check(std::uintptr_t actual_menu,std::string&);
 const std::string& error()const noexcept{return failure_;}
};
}
