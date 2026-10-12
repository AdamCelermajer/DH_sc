#include "menu_end_loading_source_v114.hpp"
#include <exception>
namespace dh2::ui {
bool menu_fs_end_loading_v114(const char*,const char*,void*,const LoadingMenuStateServicesV1& services,
 MenuEndLoadingResultV114& out,std::string& e){
 out={};
 try{
  //Reuse whole existing native state kernel/provider contract. GetCurrentLevel
  //once; NULLno-op; same130!=36 no-op; only source36->37 commit. No LoadProcess,
  //terminal37, controller/HUD/network/phase38 work is performed here.
  bool advanced{};
  if(!loading_menu_finish_v1(services,advanced,e))return false;
  out.advanced=advanced;out.native_return=0;e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
 catch(...){e="Actual EndLoading provider threw; native event prefix retained";return false;}
}
}
