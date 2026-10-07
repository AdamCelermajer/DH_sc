#pragma once
#include <loading_menu_v1.hpp>
#include <cstdint>
namespace dh2::ui {
struct MenuEndLoadingResultV114 {
 std::uintptr_t native_return{}; //original MenuBase.FS_EndLoading always0.
 bool advanced{}; //host observation only; not a readiness or phase producer.
};
//Original MenuBase.FS_EndLoading420b48 ignores command/args/userdata.
//The existing LoadingMenuStateServicesV1 lends SAME actual App/GS/Level cells;
//only actual native/menu event delivery may invoke this wrapper.
bool menu_fs_end_loading_v114(const char* command,const char* argument,void* userdata,
 const LoadingMenuStateServicesV1&,MenuEndLoadingResultV114&,std::string&);
//NativeEndLoading43ab00 has identical state body, no AS-result store and no
//MenuFX query or FS-command recursion. Existing swf_loading_end_v1 already
//routes it through loading_menu_finish_v1; no second native wrapper is needed.
}
