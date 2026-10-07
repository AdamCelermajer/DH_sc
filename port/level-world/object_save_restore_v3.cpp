#include "object_save_restore_v3.hpp"
#include <cstring>
namespace dh2::world {
namespace {bool fail(std::string& e,const char* text){if(e.empty())e=text;return false;}}
bool canonical_gameobject_save_borrow_v3(CanonicalGameObjectBaseOwnerV1& a,ObjectSaveRestoreBorrowV3& out,std::string& e){
 ObjectSaveRestoreBorrowV3 b{a.identity(),a.byte(0x80),a.byte(0x8a),a.byte(0xac),a.byte(0xd0),a.integer(0x270),a.pointer(0x2d8)};
 b.visible_produced=&a.lifecycle().visible_written;
 if(!b.identity||!b.visible80||!b.enabled8a||!b.tested_ac||!b.tested_d0||!b.archetype270||!b.visual2d8)return fail(e,"Required same produced GameObject save fields");out=b;return true;
}
bool object_base_serialize_v3(SavegameStreamV2& s,const ObjectSaveRestoreBorrowV3& f,std::string& e){
 if(!f.identity||!f.visible80||!f.enabled8a)return fail(e,"Required same ObjectBase visible80/enabled8a");
 if(f.visible_produced&&!*f.visible_produced)return fail(e,"Required source visible80 producer before serialization read");
 return s.write({f.visible80,1},e)&&s.write({f.enabled8a,1},e);
}
bool object_base_deserialize_v3(SavegameStreamV2& s,const ObjectSaveRestoreBorrowV3& f,std::string& e){
 if(!f.identity||!f.visible80||!f.enabled8a)return fail(e,"Required same ObjectBase visible80/enabled8a");
 if(!s.read(f.visible80,1,e))return false;
 if(f.visible_produced)*f.visible_produced=true;
 if(!s.read(f.enabled8a,1,e))return false;
 if(!f.tested_ac)return fail(e,"Required actual ConditionData8c tested+20");*f.tested_ac=0;
 if(!f.tested_d0)return fail(e,"Required actual ConditionDatab0 tested+20");*f.tested_d0=0;return true;
}
bool gameobject_serialize_v3(SavegameStreamV2& s,const ObjectSaveRestoreBorrowV3& f,std::string& e){
 if(!object_base_serialize_v3(s,f,e))return false;if(!f.archetype270)return fail(e,"Required same GameObject archetype270");return s.write_u32(static_cast<std::uint32_t>(*f.archetype270),e);
}
bool gameobject_deserialize_v3(SavegameStreamV2& s,const ObjectSaveRestoreBorrowV3& f,ObjectSaveRestoreServicesV3 services,std::string& e){
 if(!object_base_deserialize_v3(s,f,e))return false;std::uint32_t saved;if(!s.read_u32(saved,e))return false;
 if(!f.archetype270)return fail(e,"Required same GameObject archetype270");std::int32_t id;std::memcpy(&id,&saved,4);
 if(id!=*f.archetype270){const ObjectSaveRestoreRequestV3 request{0x38b954,saved,static_cast<std::uint32_t>(*f.archetype270),f.identity,0};if(!services.invoke||!services.invoke(services.context,request,e))return fail(e,"Required original saved archetype mismatch assertion");}
 if(!f.visual2d8)return fail(e,"Required actual GameObject VisualObject2d8 slot");
 if(*f.visual2d8){const ObjectSaveRestoreRequestV3 request{0x4713d0,0,0,*f.visual2d8,f.identity};if(!services.invoke||!services.invoke(services.context,request,e))return fail(e,"Required whole same VisualObject SyncVisibility");}
 return true;
}
}
