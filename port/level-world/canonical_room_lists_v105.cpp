#include "canonical_object_manager_v1.hpp"
#include "source_assertion_process_v76.hpp"
#include <algorithm>
namespace dh2::world {namespace {
bool assertion(std::int32_t line,const char* expression,std::string& e){
 auto process=SourceAssertionProcessV76::borrow();if(!process){e="Required actual ObjectManager room-list assertion owner";return false;}
 const auto mode=*process->source_level();
 if(mode==2){e=std::string("Original ObjectManager room-list deliberate NULL-store assertion: ")+expression;return false;}
 if(mode==1)return process->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\ObjectManager\\ObjectManager.cpp",line,expression,e);
 e.clear();return true;
}
}
bool CanonicalObjectManagerV1::source_add_room_objects_v105(const std::list<std::uintptr_t>* room,std::string& e){
 if(!room&&!assertion(0x909,"ro",e))return false;
 if(std::find(room_objects80_v105_.begin(),room_objects80_v105_.end(),room)!=room_objects80_v105_.end()&&
    !assertion(0x90a,"std::find(m_roomObjectsList.begin(), m_roomObjectsList.end(), ro) == m_roomObjectsList.end()",e))return false;
 //Source modes0/1 still insert, including duplicate/NULL pointer. Any reached
 //later dereference must be handled by its actual caller, not suppressed here.
 room_objects80_v105_.push_back(room);e.clear();return true;
}
bool CanonicalObjectManagerV1::source_del_room_objects_v105(const std::list<std::uintptr_t>* room,std::string& e){
 if(!room&&!assertion(0x912,"ro",e))return false;
 room_objects80_v105_.remove(room);e.clear();return true;
}
}
