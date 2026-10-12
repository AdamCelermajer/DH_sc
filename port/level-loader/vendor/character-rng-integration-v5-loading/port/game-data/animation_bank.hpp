#pragma once
#include "data.hpp"
namespace dh2::data {
using AnimationBankDigest=std::array<std::uint8_t,32>;
struct AnimationBankResource {
 std::int32_t clip_id=-1;
 std::uint32_t bytes=0;
 AnimationBankDigest sha256{};
 std::string authored_path,asset,cache_entry;
};
struct AnimationBank {
 std::string character;
 std::uint32_t animation_table=0,animation_set_id=0;
 std::int32_t template_clip_id=-1;
 std::uint32_t identity_policy=0;
 AnimationBankDigest manifest_sha256{},cache_sha256{},original_sha256{},producer_sha256{};
 std::vector<AnimationBankResource> resources;
 std::vector<std::int32_t> registration_requests;
};
// PAB1 v1 metadata asset. Owned paths/ordered occurrences, atomic commit.
// Policy1 assigns one port cache identity token (resource index+1) per exact
// unique asset path; it does not reconstruct original CCDB pointer addresses.
bool load_animation_bank(Bytes,AnimationBank&,std::string& error);
std::int32_t animation_resource_index(const AnimationBank&,std::int32_t clip_id)noexcept;
const AnimationBankResource* animation_resource(const AnimationBank&,std::int32_t clip_id)noexcept;
std::uintptr_t animation_resource_identity(const AnimationBank&,std::int32_t clip_id)noexcept;
}
