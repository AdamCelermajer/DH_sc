#include "swf_controller_storage_v91.hpp"
#include "swf_menu_unload_v91.hpp"
#include "gameswf/gameswf_render.h"
#include "gameswf/gameswf_character.h"
#include <map>
#include <vector>
#include <stdexcept>
namespace dh2::ui {
bool swf_actual_renderer_borrow_v91(std::uintptr_t& out,std::string& error)noexcept{
 out=reinterpret_cast<std::uintptr_t>(gameswf::get_render_handler());error.clear();return true;
}
struct SwfControllerStorageV91::Impl {
 SwfInputState288 fields{};
 std::map<gameswf::character*,std::vector<std::unique_ptr<gameswf::gc_ptr<gameswf::character>>>> pins;
 Impl(){for(auto& slot:fields.slots)slot.enabled=1;}
};
SwfControllerStorageV91::SwfControllerStorageV91():p_(new Impl){}
// Explicit source teardown uses release_all while failures are reportable.
// Fallback destruction releases the actual RAII pin pool, without trusting
// corrupted raw fields or calling checked drop from an implicit destructor.
// It returns no source-success result and does not prove ordered source cleanup.
SwfControllerStorageV91::~SwfControllerStorageV91()=default;
SwfInputState288& SwfControllerStorageV91::fields()noexcept{return p_->fields;}
void SwfControllerStorageV91::retain(gameswf::character* c){if(!c)throw std::runtime_error("Null source controller strong reference");p_->pins[c].push_back(std::make_unique<gameswf::gc_ptr<gameswf::character>>(c));}
void SwfControllerStorageV91::drop(gameswf::character* c){
 auto i=p_->pins.find(c);if(i==p_->pins.end()||i->second.empty())throw std::runtime_error("Unbalanced source controller reference");
 auto pin=std::move(i->second.back());i->second.pop_back();if(i->second.empty())p_->pins.erase(i);pin.reset();
}
bool SwfControllerStorageV91::reset_controller(std::uint32_t i,std::string& e){
 if(i>=4){e="Invalid source controller index";return false;}
 try{auto& slot=p_->fields.slots[i];for(auto* field:{&slot.focus,&slot.hover,&slot.graphic,&slot.pending,&slot.pressed}){
  if(*field){drop(reinterpret_cast<gameswf::character*>(*field));*field=0;}
 }e.clear();return true;}catch(const std::exception& x){e=x.what();return false;}
}
bool SwfControllerStorageV91::release_all(std::string& e){
 for(std::uint32_t i=0;i<4;++i)if(!reset_controller(i,e))return false;
 e.clear();return true;
}
}
