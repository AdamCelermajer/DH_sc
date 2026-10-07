#pragma once
#include "authored_menu_lifecycle_v1.hpp"
#include "swf_actionscript_connection.hpp"
namespace dh2::ui {
// The registered character itself is weak, exactly like UIBase+48/+4c.
// A later object at the same authored path must not replace this receiver.
// Values returned by borrow are temporary strong pins within the movie Scope.
class AuthoredMenuWeakCharacterV59 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 AuthoredMenuWeakCharacterV59();~AuthoredMenuWeakCharacterV59();
 AuthoredMenuWeakCharacterV59(const AuthoredMenuWeakCharacterV59&)=delete;
 AuthoredMenuWeakCharacterV59& operator=(const AuthoredMenuWeakCharacterV59&)=delete;
 bool bind(SwfAsGraph&,const SwfAsValue&,std::string&);
 //Only genuine RegisterState/RefreshCache replay after resource LoadMenu may
 //assign a newly found receiver. Normal borrow/update never re-finds by name.
 bool source_assign_v67(SwfAsGraph&,const SwfAsValue&,std::string&);
 bool source_assign_null_v68(SwfAsGraph&,std::string&);
 //DebugCachedCharacter.GetChar permits its genuine C1 NULL weak receiver.
 bool source_get_char_v68(SwfAsGraph&,SwfAsValue&,bool& live,std::string&);
 bool borrow(SwfAsGraph&,SwfAsValue&,bool& live,std::string&);
 // Exact MenuBase.Update421fe0 + RenderFX.IsAnimOver7a7dd4. Neither advances
 // the movie nor changes its play state. Counter78 is an unsigned source word.
 bool update(SwfAsGraph&,AuthoredMenuFieldsV1&,std::string&);
 // Original raw weak query; get_ptr performs actual dead-proxy drop/clear.
 bool source_live_v114()noexcept;
 void reset()noexcept;
};
}
