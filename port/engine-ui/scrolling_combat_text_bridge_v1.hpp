#pragma once

#include "scrolling_combat_text_owner_v1.hpp"
#include <array>

namespace dh2::ui {

// PyData color integers follow the source RenderFX `#RRGGBB` path; the high
// byte is not part of the SWF text color. Output is opaque RGBA.
void decode_scrolling_combat_text_color_rgb_v1(std::int32_t source_color,
                                                std::uint8_t rgba[4]) noexcept;

// Non-owning adapter for the live source owners. The position callback must
// resolve identity-matched GetTargetPosition and apply Character bounds delta
// (relative_box[5] - relative_box[2]) to Z. Color decoding remains an explicit
// source provider until the original FlashAnimContext byte layout is proven.
struct ScrollingCombatTextBridgeProvidersV1 {
    void* source_context{};
    bool (*is_follower)(void*,std::uintptr_t,bool&,std::string&){};
    bool (*position)(void*,std::uintptr_t,float xyz[3],std::string&){};
    bool (*source_property)(void*,std::uintptr_t,std::int32_t,std::int32_t&,std::string&){};
    bool (*is_dual_wielding)(void*,std::uintptr_t,bool&,std::string&){};
    bool (*is_local_player)(void*,std::uintptr_t,bool&,std::string&){};

    void* ui_context{};
    bool (*constant)(void*,const char*,const char*,std::int32_t&,std::string&){};
    bool (*localized_string)(void*,std::int32_t,std::string&,std::string&){};
    bool (*localized_formatted_string)(void*,std::int32_t,std::int32_t,
                                       std::string&,std::string&){};
    bool (*style_id)(void*,const char*,std::int32_t&,std::string&){};
    bool (*decode_color_rgba)(void*,std::int32_t,std::uint8_t rgba[4],std::string&){};
    bool (*play_authored_screen)(void*,const char*,std::uint32_t,float,float,
                                 const char*,const std::uint8_t rgba[4],std::string&){};
};

class ScrollingCombatTextBridgeV1 {
public:
    bool bind(const ScrollingCombatTextBridgeProvidersV1&,
              const float view_projection[16],int width,int height,
              std::string& error) noexcept;
    ScrollingCombatTextServicesV1 services() noexcept;

private:
    ScrollingCombatTextBridgeProvidersV1 providers_{};
    std::array<float,16> view_projection_{};
    int width_{},height_{};
    bool bound_{};

    static bool follower(void*,std::uintptr_t,bool&,std::string&);
    static bool position(void*,std::uintptr_t,float[3],std::string&);
    static bool property(void*,std::uintptr_t,std::int32_t,std::int32_t&,std::string&);
    static bool dual(void*,std::uintptr_t,bool&,std::string&);
    static bool local(void*,std::uintptr_t,bool&,std::string&);
    static bool constant(void*,const char*,const char*,std::int32_t&,std::string&);
    static bool localized(void*,std::int32_t,std::string&,std::string&);
    static bool formatted(void*,std::int32_t,std::int32_t,std::string&,std::string&);
    static bool style(void*,const char*,std::int32_t&,std::string&);
    static bool play_text(void*,const char*,const float[3],const char*,
                          std::int32_t,std::string&);
    static bool play_value(void*,const char*,const float[3],std::int32_t,
                           std::int32_t,std::string&);
};

} // namespace dh2::ui
