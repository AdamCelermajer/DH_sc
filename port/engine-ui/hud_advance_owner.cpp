#include "hud_advance_owner.hpp"
#include "gameswf/gameswf_sprite.h"
#include <map>
#include <set>
namespace dh2::ui {
struct HudAdvanceOwner::Impl {struct Record {weak_ptr<gameswf::character> weak;bool dirty=false;};std::map<const gameswf::character*,Record> records;};
HudAdvanceOwner::HudAdvanceOwner():impl_(new Impl){}HudAdvanceOwner::~HudAdvanceOwner()=default;HudAdvanceOwner::HudAdvanceOwner(HudAdvanceOwner&&) noexcept=default;HudAdvanceOwner&HudAdvanceOwner::operator=(HudAdvanceOwner&&) noexcept=default;
bool HudAdvanceOwner::notify(gameswf::sprite_instance* sprite,std::string& error){if(!impl_||!sprite){error="HUD advance owner/clip unavailable";return false;}try{gameswf::gc_ptr<gameswf::sprite_instance> pin=sprite;std::set<const gameswf::character*> visited;
 for(gameswf::character* current=sprite;current;){if(visited.size()>=65536||!visited.insert(current).second){error="Invalid HUD parent topology";return false;}auto& r=impl_->records[current];if(r.weak.get_ptr()!=current)r.weak=current;r.dirty=true;
  // Actual upstream weak_ptr.get_ptr performs dead-control release and clears
  // the stale parent slot before returning null, matching source parent gate.
  current=current->get_parent();
 }return true;}catch(const std::exception& e){error=e.what();return false;}}
bool HudAdvanceOwner::needs_advance(const gameswf::character* c)const{if(!impl_||!c)return false;const auto i=impl_->records.find(c);return i!=impl_->records.end()&&i->second.weak.get_ptr()==c&&i->second.dirty;}
std::size_t HudAdvanceOwner::live_nodes(){if(!impl_)return 0;for(auto i=impl_->records.begin();i!=impl_->records.end();)if(!i->second.weak.get_ptr())i=impl_->records.erase(i);else ++i;return impl_->records.size();}
std::size_t HudAdvanceOwner::dirty_nodes(){live_nodes();std::size_t count=0;if(impl_)for(const auto& r:impl_->records)count+=r.second.dirty;return count;}
void HudAdvanceOwner::reset_after_advance(){live_nodes();if(impl_)for(auto& r:impl_->records)r.second.dirty=false;}
}
