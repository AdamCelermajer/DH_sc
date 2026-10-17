#include "character_oid_cache_v1.hpp"

#include <algorithm>

namespace dh2::character_oid_cache_v1 {

Status Owner::begin_level(std::uintptr_t identity,
                          std::uint32_t character_table_size) noexcept {
    if (!identity || !character_table_size) return Status::invalid_argument;
    if (level_identity_==identity) {
        return character_table_size_==character_table_size
            ? Status::complete : Status::invalid_argument;
    }
    counts_.clear();
    level_identity_=identity;
    character_table_size_=character_table_size;
    ++generation_;
    return Status::complete;
}

Status Owner::add_char_oid(std::uint32_t id,std::uint32_t requested,
                           Result* out) {
    if (!out) return Status::invalid_argument;
    *out={};
    if (!level_identity_) return out->status=Status::no_level;
    if (id>=character_table_size_) return out->status=Status::out_of_range;
    auto [at,inserted]=counts_.emplace(id,requested);
    if (!inserted && requested>at->second) {
        at->second=requested;
        out->changed=true;
    } else {
        out->changed=inserted;
    }
    out->value=at->second;
    return Status::complete;
}

Status Owner::count(std::uint32_t id,Result* out) const noexcept {
    if (!out) return Status::invalid_argument;
    *out={};
    if (!level_identity_) return out->status=Status::no_level;
    if (id>=character_table_size_) return out->status=Status::out_of_range;
    const auto at=counts_.find(id);
    out->value=at==counts_.end()?0:at->second;
    out->changed=at!=counts_.end();
    return Status::complete;
}

Status Owner::clear_level(std::uintptr_t identity,std::uint64_t generation) noexcept {
    if (!identity) return Status::invalid_argument;
    if (!level_identity_) return Status::complete;
    if (identity!=level_identity_ || generation!=generation_)
        return Status::stale_level;
    counts_.clear();
    level_identity_=0;
    character_table_size_=0;
    ++generation_;
    return Status::complete;
}

Status register_summon(Owner& owner,const Arguments& args,Result* out) {
    if (!out) return Status::invalid_argument;
    *out={};
    if (!args.values && args.count) return out->status=Status::invalid_argument;
    if (!args.count || args.values[0].kind!=ValueKind::unsigned_integer)
        return Status::complete;
    const auto id=args.values[0].value;
    if (id>=owner.character_table_size()) return Status::complete;
    std::uint32_t requested=1;
    if (args.count>1 && args.values[1].kind==ValueKind::unsigned_integer) {
        if (args.values[1].value>UINT32_MAX)
            return out->status=Status::out_of_range;
        requested=static_cast<std::uint32_t>(args.values[1].value);
    }
    return owner.add_char_oid(static_cast<std::uint32_t>(id),requested,out);
}

} // namespace dh2::character_oid_cache_v1
