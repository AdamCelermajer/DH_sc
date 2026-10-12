#pragma once
#include "openable_container_owner_v1.hpp"
#include "canonical_property_map_v1.hpp"
namespace dh2::world {
// Forwards inherited offsets to SAME canonical GameObject base; owns no fields.
class OpenableContainerPropertyConnectionV1 {
    OpenableContainerFieldsV1& fields_;
    CanonicalPropertyFieldServicesV1 base_;
    std::shared_ptr<void> lease_;
    static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
    static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);
    static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
    static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
    static bool write_float(void*,std::uint32_t,float,std::string&);
    static bool write_vector(void*,std::uint32_t,const std::array<float,3>&,std::string&);
    static bool write_point(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string&);
public:
    OpenableContainerPropertyConnectionV1(OpenableContainerFieldsV1& f,
        CanonicalPropertyFieldServicesV1 base,std::shared_ptr<void> lease):fields_(f),base_(base),lease_(std::move(lease)){}
    CanonicalPropertyFieldServicesV1 services();
};
}
