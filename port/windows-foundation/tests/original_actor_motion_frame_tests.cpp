#include "original_actor_motion_frame.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
struct Receiver {ActorState actor;std::array<float,3> previous{},previousRotation{},targetPosition{};};
int main(){try{
    std::string error;auto receiver=std::make_shared<Receiver>();receiver->actor.id=41816;receiver->actor.transform.position={1090.75f,-212.202f,258};receiver->actor.transform.rotation={0,0,.2f};receiver->actor.health=17;receiver->actor.target_id=99;
    OriginalActorMotionFrameBorrow borrow{receiver,&receiver->actor,receiver->actor.id,&receiver->previous,&receiver->previousRotation};OriginalActorMotionFrameServices services;services.runtime_lease=receiver;std::vector<int> order;
    // Explicit executable host service fixture. Each phase changes observable
    // backing; no live path/FSM/physics/target implementation is claimed here.
    services.update_path=[&](const auto& b,auto&){check(b.actor==&receiver->actor,"Path borrowed another actor");order.push_back(1);b.actor->transform.position[0]+=10;return true;};
    services.update_rotation=[&](const auto& b,auto&){check(order==std::vector<int>{1},"Rotation preceded path");order.push_back(2);b.actor->transform.rotation[2]=1;return true;};
    services.update_subobjects=[&](const auto& b,auto&){check(order==std::vector<int>({1,2}),"Subobjects preceded rotation");order.push_back(3);b.actor->transform.position[2]=255;return true;};
    services.update_target_position=[&](const auto& b,auto&){check(order==std::vector<int>({1,2,3}),"Target preceded subobjects");order.push_back(4);receiver->targetPosition=b.actor->transform.position;return true;};
    OriginalActorMotionFrameResult result;for(auto action:{CharacterAction::moving,CharacterAction::attacking,CharacterAction::idle,CharacterAction::dead}){receiver->actor.action=action;receiver->actor.transform.position={1090.75f,-212.202f,258};receiver->actor.transform.rotation={0,0,.2f};order.clear();check(update_original_actor_motion_frame(borrow,services,result,error),error);check(order==std::vector<int>({1,2,3,4})&&result.completed_mask==31&&result.phase==OriginalActorMotionFramePhase::complete_prefix,"Source phases filtered/reordered by action");check(receiver->previous==std::array<float,3>{1090.75f,-212.202f,258}&&receiver->previousRotation[2]==.2f,"Source snapshots taken after callbacks or from another position");check(receiver->actor.health==17&&receiver->actor.target_id==99&&receiver->targetPosition==receiver->actor.transform.position,"Coordinator created gameplay authority or target cache ran early");}
    auto missing=services;missing.update_subobjects={};order.clear();receiver->actor.transform.position={1,2,3};check(!update_original_actor_motion_frame(borrow,missing,result,error)&&result.phase==OriginalActorMotionFramePhase::subobjects&&result.completed_mask==7&&order==std::vector<int>({1,2}),"Reached missing subobjects did not preserve exact prefix");check(receiver->actor.transform.position[0]==11&&receiver->previous[0]==1,"Callback failure rolled back source prefix");
    auto failing=services;failing.update_rotation=[](const auto& b,auto& e){b.actor->transform.rotation[2]=2;e="Unbound real visual rotation";return false;};order.clear();check(!update_original_actor_motion_frame(borrow,failing,result,error)&&result.phase==OriginalActorMotionFramePhase::rotation&&result.completed_mask==3&&receiver->actor.transform.rotation[2]==2,"Reached rotation failure prefix not retained");
    auto alias=borrow;alias.previous_position190=&receiver->actor.transform.position;const auto previous=receiver->previous;const auto priorMask=result.completed_mask;check(!update_original_actor_motion_frame(alias,services,result,error)&&receiver->previous==previous&&result.completed_mask==priorMask,"Malformed snapshot alias changed source/output");
    auto empty=services;empty.update_path={};std::uint32_t word=0x80000000u;std::memcpy(&receiver->actor.transform.rotation[0],&word,4);word=0x7fc01234u;std::memcpy(&receiver->actor.transform.rotation[1],&word,4);check(!update_original_actor_motion_frame(borrow,empty,result,error)&&result.phase==OriginalActorMotionFramePhase::path&&result.completed_mask==1&&!std::memcmp(receiver->previousRotation.data(),receiver->actor.transform.rotation.data(),12),"Source snapshot altered IEEE payloads or skipped reached missing path");
    std::cout<<"PASS borrowed source snapshots, ordered motion phases across all actions, lazy reached-service failure prefixes, no duplicate gameplay state and IEEE word preservation\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
