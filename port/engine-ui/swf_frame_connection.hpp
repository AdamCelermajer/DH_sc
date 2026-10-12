#pragma once
#include "swf_input_history.hpp"
#include <memory>
#include <string>
namespace gameswf {struct root;struct sprite_instance;struct character;struct as_object;}
namespace dh2::ui {
// Install before shared/root construction, beside SwfInputHistory in the
// facade graph_start scope. This receiver owns only weak core identities.
// The versioned sprite/root overlay is mandatory. Action/tag interpretation
// remains the actual linked GameSWF interpreter, not a second fake evaluator.
class SwfFrameConnection {
public:
 SwfFrameConnection()=default;~SwfFrameConnection();
 SwfFrameConnection(const SwfFrameConnection&)=delete;
 SwfFrameConnection&operator=(const SwfFrameConnection&)=delete;
 bool bind(gameswf::player*,std::shared_ptr<SwfInputHistory>,std::string&);
 void release() noexcept;
 bool advance(gameswf::root*,float,bool,std::string&);
 bool gc_remaining(gameswf::root*,float&,std::string&)const;
 struct State;
private:std::shared_ptr<State> state_;
};
// Only versioned core TUs call these. A reached missing receiver throws;
// facade/core protected scopes propagate that required-backend failure.
void swf_frame_observe_sprite(gameswf::sprite_instance*);
void swf_frame_construct(gameswf::character*);
void swf_frame_init_actions(gameswf::sprite_instance*);
void swf_frame_sprite_advance(gameswf::sprite_instance*,float);
void swf_frame_tags(gameswf::sprite_instance*,int,bool);
void swf_frame_actions(gameswf::sprite_instance*);
void swf_frame_goto(gameswf::sprite_instance*,int);
void swf_frame_play(gameswf::sprite_instance*,int);
// One-argument core calls explicitly use source advance flag=false. Hosts
// needing catch-up must call SwfFrameConnection.advance(...,true,...).
void swf_frame_root_advance(gameswf::root*,float,bool);
void swf_frame_start_drag(gameswf::root*);
void swf_frame_this_alive(gameswf::as_object*);
void swf_frame_drag(gameswf::character*);
void swf_frame_character_advance(gameswf::character*,float);
}
