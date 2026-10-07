#pragma once
#include "canonical_object_factory_v1.hpp"
#include <optional>
namespace dh2::world {
// Instantiate with loader::ObjectEntryV1 after importing the frozen loader
// package. Its Borrow retains original XML bytes, attributes, nested elements
// and resource URI. No ownership transfer into a diagnostic placement record.
template<class ObjectEntry> CanonicalSourceObjectRequestV1 canonical_loader_request_v1(
 ObjectEntry entry,std::uint32_t module_occurrence,const std::array<float,3>& offset,
 std::int32_t source_runtime_module_id,std::optional<std::string> filter={}){
 struct Retained {ObjectEntry entry;std::optional<std::string> filter;};
 auto retained=std::make_shared<Retained>(Retained{std::move(entry),std::move(filter)});
 CanonicalSourceObjectRequestV1 request;request.source_lease=retained;request.source_context=retained.get();
 request.element=retained->entry.element;request.module_occurrence=module_occurrence;
 request.module_offset=offset;request.runtime_module_id=source_runtime_module_id;
 request.type_filter=retained->filter?retained->filter->c_str():nullptr;
 request.attribute=[](void* p,std::uint32_t,const char* key)->const char*{
  const auto& entry=static_cast<Retained*>(p)->entry;const auto* value=entry.source().attribute(key);return value?value->c_str():nullptr;
 };return request;
}
}
