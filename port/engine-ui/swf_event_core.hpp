#pragma once
#include "swf_viewport_connection.hpp"
namespace gameswf {struct character;}
namespace dh2::ui {
// Invoke inside the retaining facade's existing GameSWF Scope. The lease
// retains exact graph and service lifetimes through synchronous AS callbacks.
// source_invoked is source InvokeASCallback's boolean (not method existence).
// A null/non-sprite without sprite parent is delivered source false.
bool swf_event_method(const SwfViewportLease&,gameswf::character*,const char*,bool& source_invoked,std::string&);
}
