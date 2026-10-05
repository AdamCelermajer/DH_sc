#include "character_menu_stats_owner_v1.hpp"
#include <algorithm>
namespace dh2::ui {
bool character_menu_assign_stat_v1(CharacterMenuStatGraphV1& g,std::uint32_t stat,std::string& error){
    error.clear();
    if(stat>3){error="source stat assignment index outside dispatch";return false;}
    if(!g.state||!g.view||!g.actors||!g.classes||!g.class_count||
       g.view->base!=g.state->base.data()||g.view->saved!=g.state->saved.data()||
       g.view->gear!=g.state->gear.data()||g.view->resolved!=g.state->resolved.data()||
       dh2_property_validate(g.view)){
        error="source stat assignment requires the same complete live property graph";return false;
    }
    // PROPS_GetInt(index,false) reads the already resolved sheet.
    const bool spent=(g.state->resolved[148]>>8)>0;
    if(spent){
        if(dh2_property_add(g.view,148,-256)||dh2_property_add(g.view,149+stat,256)){
            error="source stat property update failed";return false;
        }
        // ResetBaseProperties → LoadBaseProperties(actor) changes ONLY base.
        std::copy_n(g.view->defaults,224,g.state->base.begin());
        if(g.actor_index>=0&&std::size_t(g.actor_index)<g.actors->rows.size())
            g.state->base=g.actors->rows[std::size_t(g.actor_index)];
        // RecalcProperties(true): apply the current base class then resolve
        // all 224 fields against the same saved/gear/ordered buff groups.
        if(dh2_class_recalc_base(g.classes,g.class_count,g.state->base.data(),g.view)){
            error="source stat class/property recalculation failed";return false;
        }
    }
    if(!g.debug_load||!g.debug_query){error="source stat assignment requires actual Debug load/query providers";return false;}
    if(!g.debug_load(error))return false;
    bool tracing{};return g.debug_query("isTracingChar_Stats",tracing,error);
}
}
