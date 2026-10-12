#pragma once
#include "fixed_declarations_v1.hpp"
namespace dh2::loader {
// Source-pose geometry for the inspection renderer only. Does not construct an
// AnimatedDecor, resolve templates, activate conditions or execute startanim.
struct StaticDecorInspectionV1 {
    FixedDeclarationsV1::Borrow source_owner;
    FixedMapV1::Borrow map;
    std::vector<ObjectDeclarationV1> declarations;
    struct Skipped {ObjectDeclarationV1 declaration;std::string reason;};
    std::vector<Skipped> skipped;
};
bool prepare_static_decor_inspection_v1(const assets::ZipAssetPackV1&,
    FixedDeclarationsV1::Borrow,StaticDecorInspectionV1&,std::string& error);
}
