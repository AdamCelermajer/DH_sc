#pragma once
#include "../level-world/character_script_session_v3.hpp"
#include <functional>
namespace dh2::ui {
// Borrow exactly one native player skill owner (V3 or its V6 successor).
// No VM/state/Save/Buff ownership is copied. Caller graph retains the owner.
class CharacterMenuSkillAuthorityV1 {
 void* identity_{};
 std::function<bool()> ready_;
 std::function<character::CharacterScriptSessionV3&()> session_;
 std::function<int()> update_;
 std::function<int(std::uint32_t,std::uint32_t,float*)> info_;
 std::function<const std::string&()> error_;
public:
 CharacterMenuSkillAuthorityV1()=default;
 template<class Player> CharacterMenuSkillAuthorityV1(Player* p){*this=p;}
 template<class Player> CharacterMenuSkillAuthorityV1& operator=(Player* p){
  if(!p){*this=CharacterMenuSkillAuthorityV1{};return *this;}identity_=p;
  ready_=[p]{return p->ready();};session_=[p]()->character::CharacterScriptSessionV3&{return p->session();};
  update_=[p]{return p->update();};info_=[p](auto row,auto level,float* fraction){return p->info(row,level,fraction);};error_=[p]()->const std::string&{return p->error();};return *this;
 }
 explicit operator bool()const noexcept{return identity_!=nullptr;}
 const CharacterMenuSkillAuthorityV1* operator->()const noexcept{return this;}
 CharacterMenuSkillAuthorityV1* operator->()noexcept{return this;}
 const CharacterMenuSkillAuthorityV1& operator*()const noexcept{return *this;}
 CharacterMenuSkillAuthorityV1& operator*()noexcept{return *this;}
 void* identity()const noexcept{return identity_;}
 bool ready()const{return ready_&&ready_();}
 character::CharacterScriptSessionV3& session()const{return session_();}
 int update()const{return update_();}
 int info(std::uint32_t row,std::uint32_t level,float* fraction)const{return info_(row,level,fraction);}
 const std::string& error()const{return error_();}
};
}
