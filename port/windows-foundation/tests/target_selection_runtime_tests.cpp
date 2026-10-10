#include "../target_selection_runtime.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
ActorState actor(ActorId id,int faction,Vec3 point){ActorState a;a.id=id;a.faction_id=faction;a.health=a.max_health=100;a.transform.position={point.x,point.y,point.z};return a;}
OriginalActorProperties properties(int faction){OriginalActorProperties p;p.faction_id=faction;p.sheets.resolved[0]=faction;return p;}
}
int main(){try{
    dh2::data::AiTables factions;factions.factions.resize(11);
    factions.factions[0]={{1,-1},{2,1}};factions.factions[10]={{1,-1}};
    ActorState source=actor(1,0,{0,0,0}),enemy=actor(2,1,{0,-10,0}),side=actor(3,1,{10,0,0}),friendly=actor(4,2,{0,-2,0});
    TargetRegistry registry{&source,&enemy,&side,&friendly};TargetSelectionRuntime runtime(factions);std::string error;
    ActorSelectionProperties metadata;metadata.visible=metadata.targetable=metadata.inAllowedZone=true;
    metadata.interactionRadius=1;metadata.meleeRadius=1;metadata.capabilities=8;
    for(const auto* a:registry)check(runtime.bind(*a,properties(a->faction_id),metadata,error),"bind failed");
    SelectionPolicy policy;policy.range=8;policy.halfConeRadians=0.25f;policy.requiredCapabilities=8;
    SelectionInput input;input.command=SelectionCommand::nearest;
    auto selected=runtime.update(source,registry,input,policy);
    check(selected.valid&&selected.selectedId==enemy.id&&selected.aim.edgeDistance==8,"edge distance or actor heading differs");
    check(std::abs(selected.aim.facingRadians)<0.001f&&selected.aim.direction.y==-1,"aim heading convention differs");
    input.command=SelectionCommand::direct;input.directId=friendly.id;
    check(!runtime.update(source,registry,input,policy).valid,"friendly accepted as enemy");
    input.directId=side.id;check(!runtime.update(source,registry,input,policy).valid,"actor cone ignored");
    policy.coneBasis=SelectionConeBasis::cameraView;CameraPose camera;camera.position={0,0,10};camera.target={10,0,10};camera.up={0,0,1};
    check(runtime.update(source,registry,input,policy,nullptr,&camera).selectedId==side.id,"camera cone conversion failed");
    check(source.transform.rotation[2]==0,"selection changed actor heading");
    policy.coneBasis=SelectionConeBasis::actorHeading;policy.halfConeRadians=3.1415927f;
    input.command=SelectionCommand::next;check(runtime.update(source,registry,input,policy).selectedId==enemy.id,"deterministic cycle wrap failed");
    enemy.health=0;enemy.action=CharacterAction::dead;input.command=SelectionCommand::maintain;
    check(!runtime.update(source,registry,input,policy).valid&&source.target_id==0,"dead target retained");
    enemy.health=100;enemy.action=CharacterAction::idle;
    input.command=SelectionCommand::cursorRay;input.rayOrigin={0,0,0};input.rayDirection={0,-1,0};input.rayLength=20;
    check(runtime.update(source,registry,input,policy).selectedId==enemy.id,"cursor ray failed");
    CollisionScene collision;collision.triangles.push_back({{-3,-5,-3},{3,-5,-3},{0,-5,3},false});
    check(!runtime.update(source,registry,input,policy,&collision).valid,"cursor selected through collision");
    input.command=SelectionCommand::direct;input.directId=enemy.id;
    check(runtime.update(source,registry,input,policy,&collision).valid,"unrequested LOS changed original direct search");
    policy.requireLineOfSight=true;check(!runtime.update(source,registry,input,policy,&collision).valid,"LOS opt-in ignored");policy.requireLineOfSight=false;
    metadata.isPlayer=true;check(runtime.bind(source,properties(0),metadata,error)&&runtime.bind(enemy,properties(1),metadata,error),"player bind failed");
    check(!runtime.update(source,registry,input,policy).valid,"player pair became hostile");
    metadata.isPlayer=false;check(runtime.bind(source,properties(0),metadata,error)&&runtime.bind(enemy,properties(1),metadata,error),"rebind failed");
    source.faction_id=-1;check(runtime.bind(source,properties(-1),metadata,error),"invalid-faction bind failed");
    check(runtime.update(source,registry,input,policy).valid,"original invalid faction fallback10 differs");
    source.faction_id=2;check(!runtime.update(source,registry,input,policy).valid,"stale property binding accepted");
    Vec3 origin,direction;camera.position={0,0,0};camera.target={0,1,0};camera.up={0,0,1};camera.verticalFovDegrees=90;
    check(selectionCameraRay(camera,0,0,1,origin,direction)&&direction.y==1,"center camera ray failed");
    check(selectionCameraRay(camera,1,0,1,origin,direction)&&direction.x>0&&direction.y>0,"right camera ray basis reversed");
    camera.up={0,1,0};check(!selectionCameraRay(camera,0,0,1,origin,direction),"degenerate camera accepted");
    std::cout<<"target selection runtime tests passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
