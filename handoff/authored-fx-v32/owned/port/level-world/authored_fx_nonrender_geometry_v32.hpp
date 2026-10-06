#pragma once
#include "../engine-resources/resources.hpp"
#include <string>
namespace dh2::fx {
// Source constructGeometry60e634 returns a null IMesh for geometry kind1.
// This is a retained authored Circle helper declaration, never an empty mesh.
struct AuthoredFxNonrenderGeometryV32 {
 std::uint32_t geometry{},payload{},fields[5]{};
};
bool authored_fx_nonrender_geometry_v32(const resources::BresView&,unsigned,
 AuthoredFxNonrenderGeometryV32&,bool& nonrender,std::string&);
}

