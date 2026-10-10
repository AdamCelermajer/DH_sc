#include "combat_text_live.hpp"
#include "../../content_paths.hpp"
#include <cmath>
#include <iterator>

namespace dh::foundation {
bool original_combat_text_interval(std::int32_t phase,std::int32_t&out,std::string&error){
    const auto fps=original_combat_text_frame_rate();
    if(!std::isfinite(fps)||fps<=0){error="Original combat HUD frame rate unavailable";return false;}
    const auto interval=phase>=2&&phase<=26?33:static_cast<std::int32_t>(1000.f/fps);
    if(interval<=0){error="Original combat HUD frame interval invalid";return false;}
    out=interval;error.clear();return true;
}
bool CombatTextLiveAdapter::load(const AssetCatalog&assets,CombatSession&session,CombatTextLiveServices services,std::string&error){
    if(!services.target_position||!services.bounds_height||!services.project||!services.glyph_draw){error="Combat text requires SAME live geometry/camera/glyph renderer providers";return false;}
    if(!design_.load(assets,services.design,error))return false;
    try{if(!font_.load(resolve_content_path(assets,original_combat_text_font_resource()),error))return false;}
    catch(const std::exception&e){error=e.what();return false;}
    services.synchronous_resolution_capture=services.synchronous_resolution_capture||services.delivery==CombatTextDeliveryMode::synchronous_snapshot;
    session_=&session;services_=std::move(services);clear_for_reload();error.clear();return true;
}
dh2::character::skills::CombatTextServicesV1 CombatTextLiveAdapter::queries(){
    dh2::character::skills::CombatTextServicesV1 q{};q.context=this;
    q.follower=[](void*p,std::uintptr_t id,bool*out){
        auto&self=*static_cast<CombatTextLiveAdapter*>(p);auto*world=self.session_?self.session_->world():nullptr;
        const auto*props=world?world->combat_properties(id):nullptr;if(!props)return -1;
        const auto*ai=dh2::data::ai_props(world->factions(),props->sheets.resolved[1]);if(!ai)return -1;
        *out=ai->type==2;return 0; // Character::IsFollower3a307c/GetCharType3a3054
    };
    q.position=[](void*p,std::uintptr_t id,float*out){auto&self=*static_cast<CombatTextLiveAdapter*>(p);std::array<float,3>v{};std::string error;if(!self.services_.target_position(id,v,error))return -1;for(unsigned i=0;i<3;++i){if(!std::isfinite(v[i]))return -1;out[i]=v[i];}return 0;};
    q.height=[](void*p,std::uintptr_t id,float*out){auto&self=*static_cast<CombatTextLiveAdapter*>(p);std::string error;if(!self.services_.bounds_height(id,*out,error)||!std::isfinite(*out)||*out<0)return -1;return 0;};
    q.property=[](void*p,std::uintptr_t id,int field,std::int32_t*out){auto&self=*static_cast<CombatTextLiveAdapter*>(p);auto*world=self.session_?self.session_->world():nullptr;const auto*props=world?world->combat_properties(id):nullptr;if(!props||field<0||std::size_t(field)>=props->sheets.resolved.size())return -1;*out=props->sheets.resolved[field];return 0;};
    q.dual_wield=[](void*p,std::uintptr_t id,bool*out){auto&self=*static_cast<CombatTextLiveAdapter*>(p);auto*world=self.session_?self.session_->world():nullptr;const auto*props=world?world->combat_properties(id):nullptr;if(!props)return -1;*out=props->facts.dual_wield;return 0;};
    q.is_player=[](void*p,std::uintptr_t id,bool*out){
        auto&self=*static_cast<CombatTextLiveAdapter*>(p);auto*world=self.session_?self.session_->world():nullptr;
        const auto*props=world?world->combat_properties(id):nullptr;const auto*traits=world?world->traits(id):nullptr;if(!props||!traits)return -1;
        const auto*ai=dh2::data::ai_props(world->factions(),props->sheets.resolved[1]);if(!ai)return -1;
        // Nonzero AI.Type native branch; Type0 uses caller's already-bound
        // genuine IsPlayer ownership/archetype fact, never actor-ID guessing.
        *out=ai->type?ai->type==1:traits->is_player;return 0;
    };
    q.constant=[](void*p,const char*g,const char*k,std::int32_t*out){return static_cast<CombatTextLiveAdapter*>(p)->design_.constant(g,k,out);};
    q.localized=[](void*p,std::int32_t id,const char**out){return static_cast<CombatTextLiveAdapter*>(p)->design_.localized(id,out);};
    return q;
}
bool CombatTextLiveAdapter::capture_resolution(const PlayableCombatResolution&r,std::string&error){
    if(!session_||!services_.synchronous_resolution_capture){error="Combat text synchronous capture requires explicitly bound actual result observer mode";return false;}
    std::vector<CombatTextDisplayEvent>events;const auto completed=select_combat_text(r,queries(),events,error);
    captured_=events;
    pending_.insert(pending_.end(),std::make_move_iterator(events.begin()),std::make_move_iterator(events.end()));
    if(services_.display_event)for(const auto&event:captured_)if(!services_.display_event(event,error))return false;
    return completed;
}
bool CombatTextLiveAdapter::update(std::uint64_t ticket,std::uint32_t dt,std::int32_t interval,float sx,float sy,std::string&error){
    if(!session_){error="Combat text live session not bound";return false;}
    if(last_ticket_){if(ticket==*last_ticket_){error.clear();return true;}if(ticket<*last_ticket_){error="Combat text caller frame serial regressed";return false;}}
    last_ticket_=ticket;
    if(!services_.synchronous_resolution_capture){
        for(const auto&resolution:session_->resolutions()){
            std::vector<CombatTextDisplayEvent>events;const auto completed=select_combat_text(resolution,queries(),events,error);
            captured_=events;
            pending_.insert(pending_.end(),std::make_move_iterator(events.begin()),std::make_move_iterator(events.end()));
            if(services_.display_event)for(const auto&event:captured_)if(!services_.display_event(event,error))return false;
            if(!completed)return false;
        }
    }
    // Publish the serial before callbacks: a reached provider failure cannot
    // replay the same combat results and create duplicate labels on a retry.
    for(const auto&event:pending_)if(!presenter_.enqueue(event,services_.project,sx,sy,error)){pending_.clear();return false;}
    pending_.clear();return presenter_.update_ms(dt,interval,error);
}
bool CombatTextLiveAdapter::after_host_update(std::uint64_t ticket,std::uint32_t dt,float sx,float sy,std::string&error){
    std::int32_t interval=0;if(!original_combat_text_interval(0,interval,error))return false;
    return update(ticket,dt,interval,sx,sy,error);
}
bool CombatTextLiveAdapter::after_source_update(std::uint64_t ticket,const dh2::ui::CombatFlashTickBorrowV1&borrow,float sx,float sy,std::string&error){
    dh2::ui::CombatFlashTickInputsV1 inputs{};
    if(dh2::ui::combat_flash_tick_inputs_v1(&inputs,borrow)!=1){error="Combat text requires actual Application dt and selected-Level phase fields";return false;}
    std::int32_t interval=0;if(!original_combat_text_interval(inputs.load_phase,interval,error))return false;
    return update(ticket,inputs.application_dt,interval,sx,sy,error);
}
bool CombatTextLiveAdapter::draw(float sx,float sy,std::string&error){
    if(!session_){error="Combat text live session not bound";return false;}
    std::vector<CombatTextGlyph>quads;if(!presenter_.geometry(font_,sx,sy,quads,error))return false;
    return services_.glyph_draw(quads,error);
}
void CombatTextLiveAdapter::clear_for_reload()noexcept{presenter_.clear();pending_.clear();captured_.clear();last_ticket_.reset();}
} // namespace dh::foundation
