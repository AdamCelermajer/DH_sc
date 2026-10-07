#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include "swf_input_policy.hpp"
namespace gameswf {struct player;struct character;struct as_object;}
namespace dh2::ui {
// Bind BEFORE any shared/root character construction; keep this receiver and
// the exact player alive through graph destruction. Opt-in overlay observer
// executes source constructor/notify stores; missing observation is an error.
class SwfInputHistory {
public:
 SwfInputHistory()=default;~SwfInputHistory();
 SwfInputHistory(const SwfInputHistory&)=delete;
 SwfInputHistory&operator=(const SwfInputHistory&)=delete;
 bool bind(gameswf::player*,std::string&);
 void release() noexcept;
 bool read(gameswf::character*,SwfInputHistoryFlags&,std::string&)const;
 // Scheduler-only source byte stores. notify follows actual weak parents.
 bool write_need(gameswf::character*,bool,std::string&);
 bool notify(gameswf::character*,std::string&);
 struct State;
private:std::shared_ptr<State> state_;
};
// Only the versioned core overlay calls these; no arbitrary host setter scan.
void swf_input_observe_constructor(gameswf::character*);
void swf_input_observe_assignment(gameswf::as_object*,const char*);
}
