#pragma once
#include "source_assertion_process_v76.hpp"
namespace dh2::world {
//ObjectHandle.GetObject33fdc0 asserted NULL branch. The source still returns
//NULL after mode0/1; mode2's deliberate fault is reported at this boundary.
inline bool source_object_handle_null_assertion_v105(std::string& e){
 auto process=SourceAssertionProcessV76::borrow();if(!process){e="Required actual ObjectHandle assertion process";return false;}
 const auto mode=*process->source_level();if(mode==2){e="Original ObjectHandle.GetObject deliberate NULL-store assertion: obj";return false;}
 if(mode==1)return process->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\ObjectManager\\ObjectHandle.cpp",0x31,"obj",e);
 e.clear();return true;
}
}
