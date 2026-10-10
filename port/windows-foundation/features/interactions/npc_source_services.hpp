#pragma once
#include "npc_interact.hpp"
#include "../../../level-world/character_idle_events.hpp"
#include "../../../level-loader/game_event_runtime_v75.hpp"
namespace dh::foundation::interactions {
struct NpcCharacterEventBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t character{};
    dh2::character::CharacterIdleEvents* events{};
};
// Uses the already attached selected CharacterScriptSession/AI/FSM event owner.
// Event5 is a genuine Character event; the source AI dispatcher sends it to
// the captured FSM. It is not translated into a guessed Lua callback name.
inline bool bind_npc_event5(NpcInteractServices& out,NpcCharacterEventBorrow borrow,std::string& e){
    if(!borrow.receiver||!borrow.character||!borrow.events){e="Interactions require SAME retained Character event owner";return false;}
    out.raise_character_event=[borrow](std::uintptr_t character,std::int32_t event,std::uintptr_t actor,std::string& error){
        if(character!=borrow.character||event!=5){error="Interactions Character event receiver/domain mismatch";return false;}
        if(!borrow.events->raise(5,actor)){error=borrow.events->error();return false;}return true;
    };return true;
}
struct NpcLevelEventBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t source_level{};
    dh2::events::EventManagerOwnerV12* events{};
};
using NpcCapturedLevelResolver=std::function<bool(std::uintptr_t,NpcLevelEventBorrow&,std::string&)>;
// Exact QE_TalkToNPC local field producer corresponding to caller stores
// 3a4ff0..3a5018. This is the source event's sole native field backing.
struct NpcTalkSourceFields {
    std::int32_t type4{};
    std::uintptr_t character8{};
    std::int32_t room_c{};
    std::uint8_t pending_network10{},from_network11{};
    std::int32_t quantity14=-1,id18{};
};
inline bool bind_npc_quest_raise(NpcInteractServices& out,NpcCapturedLevelResolver resolver,std::string& e){
    if(!resolver){e="Interactions require SAME captured Level/EventManager resolver";return false;}
    out.raise_async=[resolver=std::move(resolver)](std::uintptr_t captured_level,const NpcTalkEvent& produced,std::string& error){
        NpcLevelEventBorrow level;if(!resolver(captured_level,level,error))return false;
        if(!level.receiver||!level.events||!captured_level||level.source_level!=captured_level){error="Interactions require captured source Level EventManager";return false;}
        auto fields=std::make_shared<NpcTalkSourceFields>();
        fields->type4=produced.objective_type;fields->character8=produced.actor;fields->room_c=produced.room;
        fields->pending_network10=produced.byte18;fields->from_network11=produced.byte19;
        fields->quantity14=produced.source_index;fields->id18=produced.data_id;
        dh2::loader::GameEventQuestBorrowV75 projection;
        projection.receiver=fields;projection.character8=fields->character8;projection.id18=fields->id18;
        projection.pending_network10=&fields->pending_network10;projection.from_network11=&fields->from_network11;
        projection.quantity14=&fields->quantity14;projection.character8_cell=&fields->character8;projection.id18_cell=&fields->id18;
        dh2::loader::ScopedGameQuestEventV75 event(reinterpret_cast<std::uintptr_t>(fields.get()),&fields->type4,std::move(projection));
        // Original RaiseAsync339090 tails to synchronous Raise338ebc. The same
        // scoped payload remains mutable through listeners; no queue/clone.
        return level.events->raise_async(event.borrow(),error);
    };return true;
}
}
