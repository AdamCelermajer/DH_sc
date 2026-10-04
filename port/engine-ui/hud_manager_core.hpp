#pragma once
#include "hud_manager.hpp"
#include "hud_sprite_core.hpp"
#include <memory>
#include <string>
namespace gameswf {struct character;}
namespace dh2::ui {
// Caller owns the whole retained movie/Impl and enters its protected Scope for
// every bind/dispatch. This sidecar owns paths and weak caches, not that owner.
// It must be destroyed before the movie owner; it creates no generic AS entry.
class HudManagerCore {
 public:
 using RequiredOperation=int(*)(void*,const HudManagerRequest&,HudManagerResponse&,std::string&);
 HudManagerCore();~HudManagerCore();
 HudManagerCore(const HudManagerCore&)=delete;HudManagerCore&operator=(const HudManagerCore&)=delete;
 bool bind(gameswf::character* root,const char* verified_sha256,const HudSpriteCoreServices&,std::string&);
 // Source text parser/position/list providers are supplied by the retained
 // owner. A reached unsupported operation is never treated as a successful no-op.
 void required_operations(void* context,RequiredOperation);
 // 1 delivered,0 reached required failure,-1 not a core operation. World and
 // source RenderFX.SetPosition providers remain explicit outside this adapter.
 int dispatch(const HudManagerRequest&,HudManagerResponse&,std::string&);
 private:struct Impl;std::unique_ptr<Impl> impl_;
};
}
