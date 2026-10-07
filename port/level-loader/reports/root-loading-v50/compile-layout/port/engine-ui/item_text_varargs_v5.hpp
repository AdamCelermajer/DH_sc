#pragma once
#include "hud_text_format_v1.hpp"
#include "../game-data/item_presentation_v5.hpp"
namespace dh2::ui {
// Source 508ef4 integer/string directive domain, with typed arguments replacing
// ARM32 va_list storage. ^d reads exact signed32 (not rounded float); ^k uses
// signed /1000, ^p wraps int*100. Floating/dollar directives are required
// unrecovered continuations, rejected explicitly; this is not full parse parity.
bool item_text_varargs_v5(const char*,const data::ItemTextArgumentV5*,std::size_t,
 const HudTextServicesV1&,std::string& output,bool& changed,std::string& error);
}
