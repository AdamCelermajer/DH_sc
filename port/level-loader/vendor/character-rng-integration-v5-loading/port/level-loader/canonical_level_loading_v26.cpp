#include "canonical_level_loading_v26.hpp"
namespace dh2::loader {
bool CanonicalLevelLoadingV26::current(void* p,std::uintptr_t& identity,std::string& e){
 auto& self=*static_cast<CanonicalLevelLoadingV26*>(p);CanonicalCurrentLevelBorrowV1 actual;
 if(!borrow_current_canonical_level_v1(self.globals_,actual,e))return false;
 identity=actual.identity();return true;
}
bool CanonicalLevelLoadingV26::fields(std::uintptr_t id,CanonicalLevelContextV1::LoadingFieldsV26& out,std::string& e){
 CanonicalCurrentLevelBorrowV1 actual;if(!borrow_current_canonical_level_v1(globals_,actual,e))return false;
 if(!id||!actual||actual.identity()!=id){e="Loading callback requires SAME current GSLevel receiver";return false;}
 return actual.level()->loading_fields_v26(out,e);
}
bool CanonicalLevelLoadingV26::progress(void* p,std::uintptr_t id,std::uint32_t& out,std::string& e){
 CanonicalLevelContextV1::LoadingFieldsV26 f;if(!static_cast<CanonicalLevelLoadingV26*>(p)->fields(id,f,e))return false;
 out=*f.progress30;return true;
}
bool CanonicalLevelLoadingV26::state(void* p,std::uintptr_t id,std::uint32_t& out,std::string& e){
 CanonicalLevelContextV1::LoadingFieldsV26 f;if(!static_cast<CanonicalLevelLoadingV26*>(p)->fields(id,f,e))return false;
 out=*f.state130;return true;
}
bool CanonicalLevelLoadingV26::online(void* p,std::uint32_t& out,std::string& e){
 auto& self=*static_cast<CanonicalLevelLoadingV26*>(p);
 if(!self.online_){e="Required actual OnlineGameState for absent-Level loading callback";return false;}
 return self.online_(out,e);
}
bool CanonicalLevelLoadingV26::advance(void* p,std::uintptr_t id,std::uint32_t expected,std::uint32_t next,std::string& e){
 CanonicalLevelContextV1::LoadingFieldsV26 f;if(!static_cast<CanonicalLevelLoadingV26*>(p)->fields(id,f,e))return false;
 if(expected!=36||next!=37||*f.state130!=expected){e="NativeEndLoading requires source SAME-Level state36 to37 transition";return false;}
 *f.state130=next;e.clear();return true;
}
}
