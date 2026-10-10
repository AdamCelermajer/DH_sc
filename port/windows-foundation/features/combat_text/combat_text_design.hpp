#pragma once
#include "../../asset_catalog.hpp"
#include <functional>
#include <memory>
namespace dh::foundation {
struct CombatTextDesignQueries {
    std::function<bool(const char*,std::string&)> debug;
    std::function<bool(std::uintptr_t&,std::string&)> player_character;
    std::function<bool(std::uintptr_t,std::string&,std::string&)> player_name;
};
// Exact design/common-text constants and original StringManager sheet parsing.
// Asset root can be normal original-cache assets or feature-local exact data.
class CombatTextDesign {
public:
    CombatTextDesign();~CombatTextDesign();
    bool load(const AssetCatalog&,CombatTextDesignQueries,std::string& error);
    int constant(const char* group,const char* key,std::int32_t* value) const;
    int localized(std::int32_t id,const char** value);
    const std::string& error() const noexcept;
private:struct Impl;std::unique_ptr<Impl> impl_;
};
} // namespace dh::foundation
