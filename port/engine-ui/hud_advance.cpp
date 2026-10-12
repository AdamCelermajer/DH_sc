#include "hud_advance.hpp"
namespace {
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool node(const dh2::ui::HudAdvanceNode32* p){if(!aligned(p))return false;for(auto b:p->reserved)if(b)return false;return true;}
}
extern "C" int dh2_ui_hud_notify_v1(dh2::ui::HudAdvanceNode32* n,const dh2::ui::HudAdvanceServices16* services) noexcept {
 if(!node(n)||!aligned(services))return -1;
 for(unsigned visited=0;visited<65536;++visited){n->needs_advance=1;if(!n->parent)return 0;auto* proxy=n->proxy;if(!aligned(proxy))return -1;for(auto byte:proxy->reserved)if(byte)return -1;
  if(proxy->alive){if(!node(n->parent))return -1;n=n->parent;continue;}
  --proxy->references;
  if(!proxy->references&&(!services->destroy_proxy||services->destroy_proxy(services->context,proxy)!=1))return -2;
  n->parent=nullptr;n->proxy=nullptr;return 0;
 }
 return -1; // Invalid cyclic/provider topology; no source valid cycle domain.
}
