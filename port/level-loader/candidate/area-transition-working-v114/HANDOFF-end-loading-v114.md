MenuBase.FS_EndLoading — finite source route

New menu_end_loading_source_v114.hpp/.cpp exposes menu_fs_end_loading_v114(command,argument,userdata,actualLoadingServices,result,error). All three original callback inputs are ignored. Native return is literal0 on every normal branch; result.advanced is only a host observation.

Whole original MenuBase::FS_EndLoading420b48 (68 bytes) queries Application.GetCurrentLevel once, returns native0 for actual NULL, reads SAME Level130, and only when it equals36 rereads130/adds1/stores37. NativeEndLoading43ab00 (64 bytes) has the identical state body and leaves the AS result untouched. It does not call RenderFX or recursively run FSCommands. Existing loading_menu_finish_v1/LoadingMenuStateServicesV1 ALREADY implement the whole body, and current FrontUiSessionV87 NativeEndLoading ->swf_loading_end_v1 already consumes it. This new portable wrapper closes the actual registered FSCommand EndLoading seam without another implementation/Level/GS/menu owner.

Parent/Main routes actual registered MenuBaseFSRegistryV62 EndLoading/source420b48 to this helper using the SAME retained loading-state provider already supplied by native GS bootstrap. Inputs are not synthesized from readiness flags/progress. A successful native_return0 is NOT an unhandled-command/failure signal. Preserve real event delivery: no scheduler auto-invocation or forced36->37 advance.

Current TerminalLoadingV97 consumes native36 waiting behavior and actual37 cleanup, then normal dispatcher progresses38. FS_EndLoading performs none of that terminal37 body. MenuBase/MenuFX/Application/GS stay the current owners; helper lends no new state.

Main adoption: copy two new files to coherent current port/level-loader (existing owner include paths include engine-ui loading_menu_v1.hpp), add CPP to existing loader_sources once, and use handler inside Parent's existing FS router. No change to existing NativeEndLoading route is needed. The remaining genuine dependency is actual authored loading movie EndLoading event reaching registered FS handler on its owner thread, using actual current Level providers.

Source-only completion. No shared writes, tests/compiler/build, hashes/proofs/freezes or device/emulator jobs.
