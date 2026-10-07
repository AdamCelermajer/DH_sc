#include "authored_character_panel_platform_v4.hpp"
#include "authored_menu_native_drm_v4.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::ui;
int main(){
 AuthoredMenuApplicationFieldsV3 fields;AuthoredMenuTouchScreenV3 touch(854,480);
 AuthoredMenuNativeDrmV4 drm;assert(!drm.source_global());
 AuthoredCharacterPanelPlatformServicesV4 services;services.owner=std::make_shared<int>(1);services.fields=&fields;services.touch=&touch;
 services.scoped_graph=[](void*,auto,std::string& e){e="Declared fixture has no native movie Scope";return false;};
 AuthoredCharacterPanelServicesV2 panel;std::string error;
 AuthoredCharacterPanelPlatformV4 missing(services);assert(!missing.bind(panel,error)&&!panel.globals);
 services.native_drm=[&](bool& value,std::string&){value=drm.source_global();return true;};
 int debug=0;services.debug=[&](const char*,std::int32_t& value,std::string&){++debug;value=1;return true;};
 AuthoredCharacterPanelPlatformV4 platform(services);assert(platform.bind(panel,error)&&panel.globals&&panel.globals->use_native_drm==0);
 assert(!platform.bind(panel,error));AuthoredMenuFieldsV1 menu;menu.identity=101;menu.name="menu_CharacterMenu";
 std::int32_t result=0;
 assert(panel.lifecycle.invoke(menu,{AuthoredMenuOperationV1::store_rollover_event_enabled,nullptr,1},result,error)&&fields.rollover()==1);
 assert(panel.lifecycle.invoke(menu,{AuthoredMenuOperationV1::store_application_ec,nullptr,0},result,error)&&fields.application_ec().has_value()&&*fields.application_ec()==0);
 fields.manager60()=123;assert(panel.lifecycle.invoke(menu,{AuthoredMenuOperationV1::clear_manager_60},result,error)&&fields.manager60()==0);
 assert(!panel.lifecycle.invoke(menu,{AuthoredMenuOperationV1::save_settings},result,error));
 MenuStackV1 stack{};MenuStackMenuV1 receiver{};receiver.identity=101;
 MenuStackRequestV1 request{};request.menu=&receiver;request.operation=MenuStackOperationV1::register_listener;
 assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==0&&fields.listeners().at(101));
 request.operation=MenuStackOperationV1::unregister_listener;assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==0&&!fields.listeners().at(101)&&debug==1);
 request.operation=MenuStackOperationV1::reset_touch;assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==0);
 touch.active_source_bytes()[3]=1;assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==-2&&touch.active_source_bytes()[3]==1);
 request.operation=MenuStackOperationV1::process_touch;assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==0);
 touch.source_tail()=1;assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==-2&&touch.source_head()==0);
 request.operation=MenuStackOperationV1::debug_switch;request.text="isTracingMenuManager";
 assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==0&&request.result==1&&debug==2);
 request.operation=MenuStackOperationV1::license_check;assert(panel.remaining_stack.invoke(panel.remaining_stack.context,&stack,&request)==-2);
 std::cout<<"Menu platform SAME fields/listeners/touch and required failure prefixes PASS; declared fixtures, no live movie claim\n";
}
