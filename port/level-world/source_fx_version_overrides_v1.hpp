#pragma once
#include <string>
namespace dh2::fx {
// Version difference, explicit. The APK 1.0.2 AnimatedEffectTable row 121 (cutscene_dh2_witch_intro_scene_01_portal) stores step speed 0.0
// (captured by the original AnimatedEffectTable::read 0x4bc504; port/game-data/reference/effects-tables). AnimatedFX::SetSpeed 0x4924b0
// applies it to the timeline, which then stays at key 0 where the portal's diffuse-color tracks are black/transparent, so the 1.0.2 data
// would show no portal. The reference video (1.0.3) shows the animated purple floor ring and red wall during Intro_Witch (P1 945-955 s),
// so this one asset plays at speed 1.0. Any other zero speed (e.g. the mage mastery skills) keeps the table value.
inline float source_fx_effective_speed_v1(const std::string& resource_uri,float table_speed){
    static const char suffix[]="cutscene_dh2_witch_intro_scene_01_portal.bdae";
    const std::size_t n=sizeof(suffix)-1;
    if(table_speed!=0.f||resource_uri.size()<n)return table_speed;
    std::string tail=resource_uri.substr(resource_uri.size()-n);
    for(auto& c:tail)c=char(c>='A'&&c<='Z'?c-'A'+'a':c);
    return tail==suffix?1.0f:table_speed;
}
}
