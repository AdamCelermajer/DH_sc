#pragma once
#include "data.hpp"
#include <memory>
namespace dh2::data {
struct LootAudioVisualRowV8 {std::int32_t audio_drop{},audio_pickup{};std::string visual;};
class LootAudioVisualV8 {
 struct Snapshot;std::shared_ptr<const Snapshot> snapshot_;
public:
 class Borrow {friend class LootAudioVisualV8;std::shared_ptr<const Snapshot> p_;explicit Borrow(std::shared_ptr<const Snapshot> p):p_(std::move(p)){};
 public:Borrow()=default;explicit operator bool()const noexcept{return bool(p_);}const std::vector<LootAudioVisualRowV8>& rows()const;const std::vector<std::string>& names()const;};
 bool load(Bytes,Bytes,Bytes,std::string&);
 Borrow borrow()const{return Borrow(snapshot_);}
};
}
