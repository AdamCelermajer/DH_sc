#include "../equipment_visual.hpp"
#include "../asset_catalog.hpp"
#include "../../scene-materials/scene.hpp"
#include <filesystem>
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
bool same(const Mat4& a,const Mat4& b){for(unsigned i=0;i<16;++i)if(std::abs(a[i]-b[i])>.0001f)return false;return true;}
int main(int argc,char**argv){try {
    check(argc==2,"workspace root required");const auto root=std::filesystem::path(argv[1]);
    AssetCatalog bodyAssets(root/"port/android-native/app/src/main/assets");
    AssetCatalog equipmentAssets(root/".local-inputs/windows-equipment-assets/original-cache");
    CharacterVisualConfig config;config.model_path="models/prince_modular.bdae";config.template_clip_path="animations/prince_template_anim.bdae";config.clips={{"rest","animations/prince_idle_shield.bdae"},{"action","animations/prince_1hand_combo_01.bdae"}};config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;config.motion_node_id="auto";config.consume_root_motion=true;
    CharacterVisual body;std::string error;check(body.load(bodyAssets,config,error),error);
    // Actual starting-loot item664/module and authored mode1 anchor, supplied as data.
    EquipmentVisualDefinition sword;sword.id="item664";sword.model_uri="data/3d/characters/prince/weapons/mc_rweapon_longsword_01.bdae";sword.anchor_name="anchor_weapon_right_offset";
    EquipmentAttachmentSet equipment;check(equipment.load(equipmentAssets,{sword},error),error);check(equipment.attachments().size()==1,"wrong equipment count");check(equipment.update(body,error),error);
    Mat4 anchor{};check(body.bone_world(sword.anchor_name,anchor,error),error);check(same(anchor,equipment.attachments()[0].socket_world),"socket does not match source anchor world");
    // Verify original weapon instance transform is baked exactly once in vertices.
    const auto bytes=equipmentAssets.read(sword.model_uri);dh2::resources::BresView image{};check(dh2_bres_open(&image,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"weapon BRES");dh2::scene::Scene source;check(dh2::scene::load(image,source,error),error);check(!source.instances.empty(),"no original weapon instance");const auto& instance=source.instances[0];dh2::assets::Mesh mesh{};check(dh2_mesh_open(&mesh,&image,instance.geometry)==dh2::assets::Error::ok,"weapon geometry");dh2::assets::Primitive primitive{};check(dh2_mesh_primitive(&mesh,0,&primitive)==dh2::assets::Error::ok,"weapon primitive");dh2::assets::Attribute positions{};check(dh2_mesh_attribute(&mesh,primitive.attributes[0],&positions)==dh2::assets::Error::ok,"weapon positions");float raw[16]{};check(dh2_attribute_read(&positions,0,raw),"weapon vertex");float expected[3]{};for(unsigned axis=0;axis<3;++axis){expected[axis]=instance.world[12+axis];for(unsigned k=0;k<3;++k)expected[axis]+=instance.world[k*4+axis]*raw[k];}const auto actual=equipment.attachments()[0].visual.meshes()[0].vertices[0].position;check(std::abs(actual.x-expected[0])+std::abs(actual.y-expected[1])+std::abs(actual.z-expected[2])<.0001,"authored weapon instance transform differs");
    const auto before=equipment.attachments()[0].socket_world;check(body.select("action",false,error),error);check(body.update(.15,error),error);check(equipment.update(body,error),error);check(!same(before,equipment.attachments()[0].socket_world),"animated source hand anchor did not move");
    auto offsetDefinition=sword;offsetDefinition.local_offset[12]=5;offsetDefinition.local_offset[13]=7;check(equipment.load(equipmentAssets,{offsetDefinition},error),error);check(equipment.update(body,error),error);check(body.bone_world(sword.anchor_name,anchor,error),error);check(same(dh2::scene::multiply(anchor,offsetDefinition.local_offset),equipment.attachments()[0].socket_world),"caller-supplied local offset composition differs");
    auto previous=equipment.attachments()[0].socket_world;auto bad=sword;bad.model_uri="missing.bdae";check(!equipment.load(equipmentAssets,{bad},error),"missing weapon accepted");check(same(previous,equipment.attachments()[0].socket_world),"failed load changed equipment");check(!equipment.load(equipmentAssets,{sword,sword},error),"duplicate equipment accepted");
    equipment.mutable_attachments()[0].definition.anchor_name="missing-anchor";check(!equipment.update(body,error),"unknown attachment anchor accepted");check(same(previous,equipment.attachments()[0].socket_world),"failed pose changed equipment socket");equipment.clear();check(equipment.attachments().empty(),"clear failed");
    std::cout<<"PASS original sword item664 anchor, authored instance, animated hand, local offset, atomic failures\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
