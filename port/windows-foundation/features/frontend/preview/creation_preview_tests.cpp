#include "creation_preview.hpp"
#include "class_preview_scene.hpp"
#include "../../../asset_catalog.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool v,const std::string& e){if(!v)throw std::runtime_error(e);}
int main(int argc,char** argv){try{
    check(argc==2,"Original asset root required");AssetCatalog a(argv[1]);
    frontend::CreationPreview p;std::string e;check(p.load(a,e),e);
    const int rows[]={263,325,290};const unsigned weaponCounts[]={1,2,1};
    for(unsigned i=0;i<3;++i){auto& actor=p.actors()[i];
        check(actor.definition.row==rows[i],"Class order differs from original");
        check(actor.body.loaded()&&actor.body.meshes().size()==4,"Starting modular body absent");
        check(actor.equipment.attachments().size()==weaponCounts[i],"Source starting weapon multiplicity differs");
        check(actor.body.configuration()->controller_ids[0]=="MC_Head__naked-mesh-skin","Empty head fallback differs");
        std::cout<<actor.definition.character<<" | "<<actor.body.configuration()->model_path<<" | "<<actor.definition.idle_clip<<" | weapons "<<weaponCounts[i]<<'\n';
    }
    check(p.actors()[0].body.configuration()->model_path==p.actors()[2].body.configuration()->model_path,"Authored shared model lost");
    check(p.selected()==-1&&p.select(0,e)&&p.actors()[0].selection_active,"Initial Knight did not enter original showcase");
    check(p.select(1,e)&&p.update(.15,e),e);check(p.selected()==1,"Selection lost");
    check(p.actors()[1].selection_active,"Source selection animation absent");
    check(p.update(10,e),e);check(p.actors()[1].selection_active&&p.idle_transition_required(),"Source endpoint did not report required idle owner");
    check(p.actors()[1].body.animation_elapsed_seconds()==5.,"OnSelect clock was not clamped to source endpoint");
    const auto heldVertices=p.actors()[1].body.meshes()[0].vertices;
    const auto heldSocket=p.actors()[1].equipment.attachments()[0].socket_world;
    check(p.update(2,e),e);check(p.actors()[1].body.animation_elapsed_seconds()==5.&&p.actors()[1].equipment.attachments()[0].socket_world==heldSocket,"Pending idle owner did not hold source endpoint/socket");
    for(std::size_t i=0;i<heldVertices.size();++i){const auto&a=heldVertices[i].position;const auto&b=p.actors()[1].body.meshes()[0].vertices[i].position;
        check(a.x==b.x&&a.y==b.y&&a.z==b.z,"Pending idle owner changed held geometry");}
    unsigned providerCalls=0;
    p.set_idle_transition_provider([&](frontend::CreationPreviewActor& actor,std::string& error){++providerCalls;check(actor.definition.row==325&&actor.body.animation_elapsed_seconds()==5.,"Provider did not receive original actor endpoint");error="fixture original idle owner rejected";return false;});
    check(!p.update(0,e)&&e=="fixture original idle owner rejected"&&p.idle_transition_required()&&providerCalls==1,"Provider failure lost original boundary");
    // Recording transport only: this fixture acknowledges delivery, it does
    // not fabricate an ordinary idle clip or claim native FSM integration.
    p.set_idle_transition_provider([&](frontend::CreationPreviewActor&,std::string& error){++providerCalls;error.clear();return true;});
    check(p.update(0,e)&&!p.idle_transition_required()&&providerCalls==2&&std::string(p.idle_transition_status())=="native-idle-provider-delivered","Provider delivery boundary differs");
    check(!p.select(3,e)&&p.selected()==1,"Invalid selection altered owner");
    auto camera=frontend::original_menu_preview_camera();check(std::abs(camera.verticalFovDegrees-40.6164f)<.01f&&camera.eye.y==-900&&camera.up.z==1,"Original camera setters differ");
    check(!p.update(-1,e),"Negative time accepted");
    for(int index=0;index<3;++index){
        frontend::CreationPreview endpoint;check(endpoint.load(a,e)&&endpoint.select(index,e),e);
        auto& actual=endpoint.actors()[index];std::int32_t start{},end{};
        check(actual.body.animation_range("MenuOnSelect",start,end,e),e);
        CharacterVisual reference;const auto config=*actual.body.configuration();
        check(reference.load(a,config,e)&&reference.select("MenuOnSelect",false,e)&&reference.update(double(end-start)/1000.,e),e);
        check(endpoint.update(double(end-start)/1000.,e)&&endpoint.idle_transition_required(),e);
        check(actual.body.animation_elapsed_seconds()==double(end-start)/1000.,"Actual full showcase endpoint not sampled");
        for(std::size_t mesh=0;mesh<actual.body.meshes().size();++mesh)for(std::size_t vertex=0;vertex<actual.body.meshes()[mesh].vertices.size();++vertex){
            const auto&lhs=actual.body.meshes()[mesh].vertices[vertex].position;const auto&rhs=reference.meshes()[mesh].vertices[vertex].position;
            check(lhs.x==rhs.x&&lhs.y==rhs.y&&lhs.z==rhs.z,"Held endpoint differs from direct original clip sample");
        }
        check(endpoint.select((index+1)%3,e)&&!endpoint.idle_transition_required(),"New source selection retained old idle boundary");
        std::cout<<"SHOWCASE_ENDPOINT class "<<index<<" sourceMs "<<end<<" exactOriginalPose held; idle owner required\n";
    }
    frontend::ClassPreviewScene scene;check(scene.load(a,e),e);
    check(!scene.input_enabled(),"Unselected scene admitted input");
    check(scene.sample(0,0,e),e);
    check(scene.loaded()&&scene.camera().nearPlane==50&&scene.camera().farPlane==50000,"Source class camera absent");
    check(scene.camera().aspectRatio==float(4.0/3.0),"Original base camera4/3 aspect changed with viewport");
    const auto& light=scene.light_inputs();
    check(light.authored_light_present&&light.authored_node=="Omni01"&&light.light_set==0&&light.slot==0,"Original Show first light/tweaker route differs");
    check(light.attenuation_setter[0]==.375f&&light.normalized_attenuation[1]==light.attenuation_setter[1]/1000.f&&light.normalized_attenuation[2]==light.attenuation_setter[2]/1000000.f,"Source LightBase setter unit conversion differs");
    check(light.ambient==std::array<float,4>{1,1,1,1}&&light.scene_ambient==std::array<float,4>{0,0,0,1},"Original Show light/scene ambient setter inputs differ");
    check(scene.anchors()[0]!=scene.anchors()[1],"Distinct source class anchors lost");
    check(scene.selected_clip()=="lol_2_to_1"&&!scene.input_enabled(),"Initial native scene transition differs");
    check(scene.sample(0,10000,e)&&scene.sample(0,0,e),e);
    check(scene.selected_clip()=="lol_1_idle","Completed initial scene did not return to authored idle");
    check(scene.sample(1,0,e)&&!scene.input_enabled(),e);
    check(scene.sample(1,10000,e)&&scene.input_enabled(),e);
    check(!scene.sample(4,0,e),"Invalid scene selection accepted");
    for(int initial=0;initial<3;++initial) {
        frontend::ClassPreviewScene fresh;check(fresh.load(a,e),e);
        check(fresh.sample(initial,16,e),"Initial class "+std::to_string(initial)+": "+e);
        const char* initialClips[]={"lol_2_to_1","lol_3_to_2","lol_2_to_3"};
        check(!fresh.input_enabled()&&fresh.selected_clip()==initialClips[initial],"Initial class native transition differs");
        for(int target=0;target<3;++target) {
            frontend::ClassPreviewScene pair;check(pair.load(a,e)&&pair.sample(initial,0,e),e);
            check(pair.sample(initial,10000,e)&&pair.sample(initial,0,e),e);
            const bool ok=pair.sample(target,0,e);
            check(ok,e);check(pair.input_enabled()==(initial==target),"Authored transition gate differs");
            const auto expected=initial==target?"lol_"+std::to_string(target+1)+"_idle":target==0?"lol_2_to_1":target==2?"lol_2_to_3":initial==0?"lol_1_to_2":"lol_3_to_2";
            check(pair.selected_clip()==expected,"Native destination-specific transition branch differs");
            std::cout<<"Class scene pair "<<initial<<" -> "<<target<<" | "<<pair.selected_clip()<<" | sampled "<<pair.sampled_milliseconds()<<'\n';
            check(pair.sample(target,10000,e)&&pair.input_enabled(),e);
        }
    }
    OriginalScene menu;check(frontend::load_menu_preview_backdrop(a,menu,e),e);check(!menu.mesh.vertices.empty(),"Source swamp backdrop absent");
    std::cout<<"PASS original class models, appearance, weapons, select/idle, source camera\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
