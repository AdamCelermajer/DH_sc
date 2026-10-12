#include "../character_buffs.hpp"
// Only the imported class-library fixture is empty. The actual native
// class-to-base/property resolver instructions execute inside this callback.
extern "C" int dh2_buff_audit_recalculate(dh2::data::PropertyView* view){
 dh2::data::ClassRow row{};
 return dh2_class_recalc_base(&row,1,const_cast<std::int32_t*>(view->base),view)==0?1:0;
}
