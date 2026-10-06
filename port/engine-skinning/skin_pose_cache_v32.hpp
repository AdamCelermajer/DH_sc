#pragma once
#include "skinning.hpp"
#include <cstdint>

namespace dh2::skinning {
struct SkinPoseCountersV32 {
 std::uint64_t calls{},hits{},deformations{},joint_checks{},vertices{},storage_growths{};
};
// One render owner, one immutable Skin/rest stream. Bind stores typed borrows,
// never copies geometry; their owner must outlive this cache. Rebind after any
// Skin/rest mutation or owner relocation. Scene may change on every call.
// A cache hit compares every actual contributing joint world matrix bitwise;
// it does not infer pose validity from a frame number, visibility or timeline.
class SkinPoseCacheV32 {
 const Skin* skin_{};
 const std::vector<std::array<float,3>>* rest_{};
 std::vector<Matrix> worlds_,candidate_worlds_;
 std::vector<float> palette_;
 std::vector<std::array<float,3>> positions_,candidate_positions_;
 bool valid_{};std::uint64_t revision_{};
 SkinPoseCountersV32 counters_;
public:
 void bind(const Skin&,const std::vector<std::array<float,3>>&);
 void reset() noexcept;
 bool sample(const scene::Scene&,std::string&);
 const std::vector<std::array<float,3>>& positions()const noexcept{return positions_;}
 const SkinPoseCountersV32& counters()const noexcept{return counters_;}
 std::uint64_t revision()const noexcept{return revision_;}
 bool changed_last_call()const noexcept{return changed_;}
private:bool changed_{};
};
}
