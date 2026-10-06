#include "native_physical_filter_v1.hpp"
#include <exception>
namespace dh2::physical {
bool native_physical_filter_v1(const NativePhysicalFilterBorrowV1& b,bool enabled,std::string& error){
 if(!b.disabled){error="Required sole PhysicalObject filter-disabled byte";return false;}
 if(bool(*b.disabled)==!enabled){*b.disabled=enabled?0:1;return true;}
 if(!b.primary||!b.secondary||!b.saved||!b.world||!b.world->backend()||!b.body){error="Required actual physical filter/shape/world borrow";return false;}
 try{
  // Reload each source shape and saved filter after previous Refilter delivery.
  for(auto slot:{b.primary,b.secondary})if(auto* shape=*slot){
   if(shape->GetBody()!=b.body->body){error="Physical filter shape belongs to another body";return false;}
   b2FilterData filter;
   if(enabled)filter=*b.saved;
   else{filter.groupIndex=0;filter.categoryBits=0;filter.maskBits=0;}
   shape->SetFilterData(filter);b.world->backend()->Refilter(shape);
  }
  *b.disabled=enabled?0:1;return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
