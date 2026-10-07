#pragma once
#include "authored_menu_lifecycle_v1.hpp"
#include "swf_actionscript_connection.hpp"
#include "menu_stack_v1.hpp"
namespace dh2::ui {
struct AuthoredMenuMovieStackServicesV4 {
 std::shared_ptr<void> owner;
 // Resolve SAME source MenuBase fields and actual movie Scope by RenderFX id.
 std::function<AuthoredMenuFieldsV1*(std::uintptr_t)> fields;
 std::function<bool(std::uintptr_t,void*,bool(*)(void*,SwfAsGraph&,std::string&),std::string&)> scoped;
 AuthoredMenuLifecycleServicesV1 lifecycle;
 std::function<bool(MenuStackV1&,MenuStackRequestV1&,std::string&)> required;
};
class AuthoredMenuMovieStackV4 {
 AuthoredMenuMovieStackServicesV4 services_;
 bool movie(MenuStackRequestV1&,AuthoredMenuFieldsV1&,std::string&);
public:
 explicit AuthoredMenuMovieStackV4(AuthoredMenuMovieStackServicesV4);
 bool invoke(MenuStackV1&,MenuStackRequestV1&,std::string&);
};
}
