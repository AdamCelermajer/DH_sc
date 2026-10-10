#include "source_world_objects.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original asset root");AssetCatalog assets(argv[1]);std::string error;
    std::vector<ActorDefinition> definitions;check(load_actor_definitions(assets,"data/scene/001_swamp.mlx",definitions,error),error);
    SourceWorldObjects world;check(world.load(definitions,error),error);
    ActorId id=123;bool found=false;check(world.named_character("_prim_Monster_LizManIntro1",-1,id,found,error)&&found,error);
    const auto* actor=world.definition(id);check(actor&&actor->properties.at("auto_spawn")=="0","Source hidden actor missing");
    const auto occurrence=actor->moduleName;const auto sourceActor=id;
    int context=123;check(!world.module_context(sourceActor,context,error)&&context==123,"Missing actual constructor binding fabricated context");
    check(!world.named_character(actor->name,1,id,found,error),"Loader ordinal was mistaken for runtime module ID");
    // Deliberately unrelated fixture ID, not a claim about actual module ID.
    check(world.bind_module(occurrence,99,error),error);check(world.named_character(actor->name,99,id,found,error)&&found&&id==sourceActor,error);
    check(world.module_context(sourceActor,context,error)&&context==99,"Source context did not use actual bound ID");
    check(!world.module_context(0,context,error),"Unknown authored object obtained a context");
    check(world.named_character(actor->name,98,id,found,error)&&!found,error);
    check(!world.bind_module(occurrence,1,error),"Existing source module binding silently replaced");
    check(world.named_object("_prim_Waypoint_NewCamSpot",-1,id,found,error)&&found,"Authored dummy object missing");CameraVec3 position;
    check(world.anchor(id,position,error),error);check(std::abs(position.x+3763.13989f)<.01f&&std::abs(position.y-314.78f)<.01f,"Source dummy module offset missing");
    check(!world.anchor(sourceActor,position,error),"Character anchor silently fell back to authored placement");
    world.bind_actor_anchor([](ActorId,CameraVec3& value,std::string&){value={9,8,7};return true;});check(world.anchor(sourceActor,position,error)&&position.x==9,error);
    auto duplicate=definitions.front();duplicate.stableId=definitions.back().stableId;definitions.push_back(duplicate);
    check(!world.load(definitions,error)&&world.definition(sourceActor),"Failed load lost current source objects");
    std::cout<<"Source world lookup passed original hidden actors, explicit module scope, dummy world anchor and live Character seam\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
