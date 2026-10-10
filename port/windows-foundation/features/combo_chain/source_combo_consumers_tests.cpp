#include "source_combo_consumers.hpp"
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation::combo;
using namespace dh2::character;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
struct Fixture {
    std::shared_ptr<int> lease=std::make_shared<int>(1);
    AttackState64 attack{};
    TargetOwner16 owner{1,31,0,0};TargetState48 target{};TargetBindings48 targets{};
    ControllerCommandState32 controller{11,1,0,0,0,0};
    float position[3]{0,0,0},targetPoint[3]{10,0,5},heading=0;
    std::uintptr_t activeAIS=7,publishedTarget=2,calledAIS=0;
    int virtualCalls=0,positionQueries=0,debugCalls=0,eligibilityQueries=0,preIndex=-1;
    bool enemy=true,debugFailure=false,swapAIS=false;
    static int targetService(void* p,TargetState48*,const TargetRequest24* request,std::uint32_t* result){
        auto& f=*static_cast<Fixture*>(p);*result=0;
        if(request->service==target_debug_load||request->service==target_debug_query){++f.debugCalls;return f.debugFailure?-1:0;}
        return -1;
    }
    static bool eligibility(void* p,std::uintptr_t owner,std::uintptr_t target,WorldAIAttackQueryV1 query,std::int32_t& value,std::string&){
        auto& f=*static_cast<Fixture*>(p);check(owner==1&&target==2,"Eligibility substituted owner/current target");++f.eligibilityQueries;
        if(query==WorldAIAttackQueryV1::IsEnemy){value=f.enemy;if(f.swapAIS)f.activeAIS=8;}
        else value=1;return true;
    }
    Fixture(){attack.owner=1;attack.target=attack.last_target=2;attack.continued=1;attack.index=7;attack.finisher=1;
        target.identity=10;target.owner=&owner;target.candidate=target.target=target.last_target=2;target.changed=1;
        targets.state=&target;targets.services={this,targetService};}
    ConsumerBindings bindings(){ConsumerBindings b;b.lease=lease;b.attack=&attack;b.targets=&targets;b.controller=&controller;b.gamePosition=position;b.headingAngle=&heading;
        b.canAttack={this,eligibility};b.activeAIS=&activeAIS;
        b.aisPreAttack=[&](std::uintptr_t ais,std::int32_t index,std::string&){++virtualCalls;calledAIS=ais;preIndex=index;return true;};
        b.targetPosition=[&](std::uintptr_t id,const float*& actual,std::string&){check(id==2,"LookAt used a substituted target");++positionQueries;actual=targetPoint;return true;};
        b.publishTargets=[&](const TargetState48& actual,std::string&){publishedTarget=actual.target;return true;};return b;}
};
int main(){try{
    Fixture fixture;SourceComboConsumers consumer;std::string error;bool handled=false;check(consumer.bind(fixture.bindings(),error),error);
    check(consumer.execute({attack_anim_controller_look_v1,0,2},handled,error)&&handled,error);
    check(std::abs(fixture.heading-1.5707964f)<0.000001f&&fixture.positionQueries==1,"Original controller LookAt did not publish source heading");
    fixture.controller.global_blocked=1;fixture.heading=0;check(consumer.execute({attack_anim_controller_look_v1,0,2},handled,error),error);
    check(fixture.heading==0&&fixture.positionQueries==1,"Blocked controller reached original LookAt body");
    fixture.controller.forced=1;check(consumer.execute({attack_anim_controller_look_v1,0,2},handled,error),error);
    check(fixture.positionQueries==2&&fixture.heading>1.5f,"Forced controller did not bypass gate");
    check(consumer.execute({attack_anim_clear_nonsticky_v1},handled,error),error);check(fixture.target.target==2&&fixture.debugCalls==0,"Sticky source target was cleared or reached deep providers");
    fixture.target.changed=0;check(consumer.execute({attack_anim_clear_nonsticky_v1},handled,error),error);
    check(fixture.target.target==0&&fixture.target.last_target==0&&fixture.attack.target==0&&fixture.attack.last_target==0&&fixture.publishedTarget==0&&fixture.owner.word14d0==0,"Nonsticky source clear omitted setter/last-target/publication");
    fixture.target.target=fixture.target.last_target=fixture.target.candidate=2;fixture.owner.word14d0=31;fixture.debugFailure=true;
    check(!consumer.execute({attack_anim_clear_nonsticky_v1},handled,error),"Missing source debug provider was hidden");
    check(fixture.target.candidate==0&&fixture.target.target==2&&fixture.owner.word14d0==0&&fixture.publishedTarget==2,"Failed original target setter prefix was discarded or fabricated");
    fixture.debugFailure=false;fixture.swapAIS=true;check(consumer.execute({attack_anim_pre_attack_v1,7},handled,error),error);
    check(fixture.virtualCalls==1&&fixture.calledAIS==8&&fixture.preIndex==7&&fixture.eligibilityQueries==3,"Preattack did not gate and reload actual active AIS after callbacks");
    fixture.enemy=false;auto withoutActive=fixture.bindings();withoutActive.activeAIS=nullptr;withoutActive.aisPreAttack={};check(consumer.bind(withoutActive,error),error);
    check(consumer.execute({attack_anim_pre_attack_v1,0},handled,error),error);check(fixture.virtualCalls==1,"Rejected target reached preattack virtual");
    fixture.enemy=true;check(!consumer.execute({attack_anim_pre_attack_v1,0},handled,error),"Reached absent actual active AIS storage was hidden");
    fixture.activeAIS=0;fixture.swapAIS=false;auto nullAIS=fixture.bindings();nullAIS.aisPreAttack={};check(consumer.bind(nullAIS,error),error);
    check(consumer.execute({attack_anim_pre_attack_v1,0},handled,error),error);check(fixture.virtualCalls==1,"Source null active AIS invented a callback");
    std::uint32_t flags=0x12345678;std::uint8_t active=255;std::uintptr_t ooi=91;std::int8_t type=8;
    check(refresh_owner_fields(fixture.attack,{fixture.lease,1,&flags,&active,&ooi,&type},error),error);
    check(fixture.attack.owner_flags528==flags&&fixture.attack.heading_active==255&&fixture.attack.object_of_interest==91&&fixture.attack.object_of_interest_type==8&&fixture.attack.continued==1&&fixture.attack.index==7&&fixture.attack.finisher==1,"Owner refresh fabricated combo/heading/OOI fields");
    const auto previous=fixture.attack;check(!refresh_owner_fields(fixture.attack,{fixture.lease,1,&flags,nullptr,&ooi,&type},error)&&fixture.attack.heading_active==previous.heading_active,"Missing owner heading storage introduced defaults");
    check(consumer.execute({attack_anim_set_step_v1,2},handled,error)&&!handled,"Consumer stole retained source cursor ownership");
    std::cout<<"PASS genuine look/control gates, sticky clear with deep source prefixes, preattack eligibility/active AIS reload, same-owner heading/OOI fields, reached missing-provider failures\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
