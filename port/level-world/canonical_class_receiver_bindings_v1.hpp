#pragma once
#include "canonical_property_map_v1.hpp"
#include "canonical_object_loading_fields_v95.hpp"
#include <functional>
namespace dh2::world {
// One retained receiver record per actual canonical identity. Allocation and
// class initialization remain the real renderer-owned transport, not fixtures.
struct CanonicalClassReceiverV1 {
 CanonicalObjectBorrowV1 object;
 std::shared_ptr<const void> source_lease;
 std::function<CanonicalPropertyActorV1()> properties;
 std::function<bool(std::string&)> init_post;
 std::function<bool(bool&,std::string&)> is_game_object;
 std::function<bool(std::array<float,3>&,std::string&)> position;
 std::function<bool(const std::array<float,3>&,bool,std::string&)> set_position;
 // Original receiver virtual38/58 and SAME ObjectBase CString/condition cells.
 std::function<bool(CanonicalObjectLoadingFieldsV95&,std::string&)> source_loading_fields_v95;
 std::function<bool(bool&,std::string&)> source_is_updatable_v95;
 std::function<bool(std::string&)> source_init_final_v95;
};
// Works directly with RetainedCharacterActorV1 and
// CanonicalOpenableContainerV1; both expose canonical(lease)/properties().
// Callers attach the actual class virtual/SetPosition continuations below.
template<class Receiver>
CanonicalClassReceiverV1 canonical_class_receiver_v1(std::shared_ptr<Receiver> receiver){
 CanonicalClassReceiverV1 result;
 result.object=receiver->canonical(receiver);
 result.properties=[receiver]{return receiver->properties();};
 return result;
}
struct CanonicalReceiverConstructionV1 {
 void* context{};
 bool(*character)(void*,const CanonicalSourceObjectRequestV1&,CanonicalClassReceiverV1&,std::string&){};
 bool(*openable_container)(void*,const CanonicalSourceObjectRequestV1&,CanonicalClassReceiverV1&,std::string&){};
 bool(*animated_decor)(void*,const CanonicalSourceObjectRequestV1&,CanonicalClassReceiverV1&,std::string&){};
 bool(*unknown_type_debug)(void*,const char*,std::string&){};
};
class CanonicalClassReceiverBindingsV1 {
 CanonicalPropertyMapV1& map_;
 CanonicalReceiverConstructionV1 construction_;
 std::map<std::uintptr_t,CanonicalClassReceiverV1> receivers_;
 CanonicalClassReceiverV1* find(const CanonicalObjectBorrowV1&,std::string&);
 static bool construct(void*,const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,CanonicalObjectBorrowV1&,std::string&);
 static bool init_properties(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool set_template(void*,const CanonicalObjectBorrowV1&,const char*,std::string&);
 static bool defaults(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool overrides(void*,const CanonicalObjectBorrowV1&,const CanonicalSourceObjectRequestV1&,std::string&);
 static bool init_post(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool is_game_object(void*,const CanonicalObjectBorrowV1&,bool&,std::string&);
 static bool position(void*,const CanonicalObjectBorrowV1&,std::array<float,3>&,std::string&);
 static bool set_position(void*,const CanonicalObjectBorrowV1&,const std::array<float,3>&,bool,std::string&);
 static bool unknown(void*,const char*,std::string&);
public:
 CanonicalClassReceiverBindingsV1(CanonicalPropertyMapV1& map,CanonicalReceiverConstructionV1 c):map_(map),construction_(c){}
 CanonicalClassServicesV1 services() noexcept;
 // Called by actual deleting destructor/removal after manager unpublishes it.
 // Duplicate Add discards only its newly constructed identity.
 void erased(std::uintptr_t identity){receivers_.erase(identity);}
 std::size_t retained_count()const noexcept{return receivers_.size();}
};
}
