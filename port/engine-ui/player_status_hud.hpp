#pragma once
#include "swf_movie.hpp"
#include "hud_advance_owner.hpp"
#include "hud_player_values.hpp"
#include <array>

namespace dh2::ui {
// Connected status subsystem: actual retained authored clips, source FastUpdate
// values and timeline services. The world lends its resolved sheet only during
// update(); no player pointer or property buffer is retained between frames.
class PlayerStatusHud {
public:
    explicit PlayerStatusHud(SwfMovie&);
    bool bind(const char* verified_hud_sha256,std::string&);
    bool update(const std::int32_t* resolved,std::size_t count,
                std::uintptr_t character,std::string&);
    void release();
    const std::array<std::int32_t,5>& frames() const {return frames_;}
    std::uintptr_t character() const {return character_;}
    std::size_t dirty_nodes() {return advance_.dirty_nodes();}
private:
    SwfMovie& movie_;
    std::array<SwfHudClip,5> clips_{};
    std::array<std::int32_t,5> frames_{};
    HudAdvanceOwner advance_;
    bool bound_{};
    std::uintptr_t character_{};
    std::string failure_;
    static int values(void*,HudValuesState24*,const HudValueRequest32*,HudValueResponse16*);
    static bool notify(void*,gameswf::sprite_instance*,std::string&);
    static bool sound(void*,std::uintptr_t&,std::string&);
    static bool pause(void*,std::uintptr_t,std::int32_t,bool,std::string&);
    HudSpriteCoreServices sprite_services();
};
}
