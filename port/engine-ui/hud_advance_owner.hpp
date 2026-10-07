#pragma once
#include <memory>
#include <string>
#include <cstddef>
namespace gameswf {struct character;struct sprite_instance;}
namespace dh2::ui {
// Source +9d needs-advance storage projected into an owned weak sidecar because
// upstream1714 lacks that member. This owner never retains a movie/character.
class HudAdvanceOwner {
 public:
 HudAdvanceOwner();~HudAdvanceOwner();HudAdvanceOwner(HudAdvanceOwner&&) noexcept;HudAdvanceOwner&operator=(HudAdvanceOwner&&) noexcept;
 HudAdvanceOwner(const HudAdvanceOwner&)=delete;HudAdvanceOwner&operator=(const HudAdvanceOwner&)=delete;
 bool notify(gameswf::sprite_instance*,std::string&);
 bool needs_advance(const gameswf::character*) const;
 std::size_t live_nodes(); // sweeps expired weak observations
 std::size_t dirty_nodes();
 void reset_after_advance(); // explicit modern consumer, no source clear claim
 private:struct Impl;std::unique_ptr<Impl> impl_;
};
}
