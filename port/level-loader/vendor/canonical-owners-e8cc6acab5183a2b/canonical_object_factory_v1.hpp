#pragma once
#include "canonical_object_manager_v1.hpp"
#include <array>
namespace dh2::world {
struct CanonicalFactoryEntryV1 {const char* name;std::uint32_t original_address;};
const std::array<CanonicalFactoryEntryV1,33>& canonical_factories_v1()noexcept;
// Loader adapter pins XmlDocument::Borrow/ObjectEntry by one shared lease.
// element/module occurrence are diagnostic source addresses, never handles.
struct CanonicalSourceObjectRequestV1 {
 std::shared_ptr<const void> source_lease;
 void* source_context{};
 std::uint32_t element{},module_occurrence{UINT32_MAX};
 const char*(*attribute)(void*,std::uint32_t,const char*){};
 std::array<float,3> module_offset{};
 std::int32_t runtime_module_id{-1};
 const char* type_filter{};
};
struct CanonicalClassServicesV1 {
 void* context{};
 bool(*construct)(void*,const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,CanonicalObjectBorrowV1&,std::string&){};
 bool(*init_properties)(void*,const CanonicalObjectBorrowV1&,std::string&){};
 bool(*set_template)(void*,const CanonicalObjectBorrowV1&,const char*,std::string&){};
 bool(*load_defaults)(void*,const CanonicalObjectBorrowV1&,std::string&){};
 bool(*load_overrides)(void*,const CanonicalObjectBorrowV1&,const CanonicalSourceObjectRequestV1&,std::string&){};
 bool(*init_post)(void*,const CanonicalObjectBorrowV1&,std::string&){};
 bool(*is_game_object)(void*,const CanonicalObjectBorrowV1&,bool&,std::string&){};
 bool(*position)(void*,const CanonicalObjectBorrowV1&,std::array<float,3>&,std::string&){};
 bool(*set_position)(void*,const CanonicalObjectBorrowV1&,const std::array<float,3>&,bool,std::string&){};
 bool(*unknown_type_debug)(void*,const char*,std::string&){};
};
enum class CanonicalFactoryStageV1 {empty,source_skip,constructed,registered,properties,template_set,defaults,overrides,early_init_post,offset,complete,failed};
class CanonicalObjectFactoryAttemptV1 {
 CanonicalSourceObjectRequestV1 request_;
 target_providers::Handle16 handle_{0,UINT32_MAX,0};
 CanonicalFactoryStageV1 stage_{CanonicalFactoryStageV1::empty},prefix_{CanonicalFactoryStageV1::empty};
 bool attempted_{};
public:
 explicit CanonicalObjectFactoryAttemptV1(CanonicalSourceObjectRequestV1 r):request_(std::move(r)){}
 bool execute(CanonicalObjectManagerV1&,const CanonicalClassServicesV1&,std::string&);
 CanonicalFactoryStageV1 stage()const noexcept{return stage_;}
 CanonicalFactoryStageV1 prefix()const noexcept{return prefix_;}
 const target_providers::Handle16& handle()const noexcept{return handle_;}
 const CanonicalSourceObjectRequestV1& source()const noexcept{return request_;}
};
}
