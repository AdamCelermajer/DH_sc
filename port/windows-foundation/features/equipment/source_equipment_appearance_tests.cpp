#include "source_equipment_appearance.hpp"
#include "source_equipment_render_bridge.hpp"
#include "source_equipment_material_binding.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../source_material_pass.hpp"
#include "../../actor_lighting.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static std::uint32_t bres_word(const dh2::resources::BresView& view,std::uint32_t offset){
    check(offset<=view.size&&view.size-offset>=4,"BRES test field outside image");
    std::uint32_t value{};std::memcpy(&value,view.bytes+offset,4);return value;
}
static std::string bres_text(const dh2::resources::BresView& view,std::uint32_t offset){
    check(offset<view.size,"BRES test string outside image");
    const auto* end=static_cast<const std::uint8_t*>(std::memchr(view.bytes+offset,0,view.size-offset));
    check(end!=nullptr,"BRES test string is unterminated");
    return {reinterpret_cast<const char*>(view.bytes+offset),static_cast<std::size_t>(end-(view.bytes+offset))};
}
static dh2::skinning::VisualAssetResultV6 weapon_read(void* context,const char* uri,
 std::vector<std::uint8_t>& bytes,std::string& error){
    try{bytes=read_content(*static_cast<const AssetCatalog*>(context),uri);return dh2::skinning::VisualAssetResultV6::found;}
    catch(const std::exception& e){error=e.what();return dh2::skinning::VisualAssetResultV6::failed;}
}
int main(int argc,char** argv){try{
    check(argc==2,"Supply workspace root");const auto root=std::filesystem::path(argv[1]);
    AssetCatalog body_assets(root/"port/android-native/app/src/main/assets");
    AssetCatalog item_assets(root/".local-inputs/windows-shared-assets");
    AssetCatalog weapons(root/".local-inputs/windows-equipment-assets/original-cache");
    CharacterVisual body;CharacterVisualConfig config;
    config.model_path="models/prince_modular.bdae";config.template_clip_path="animations/prince_template_anim.bdae";
    config.clips={{"idle","animations/prince_idle_shield.bdae"},{"attack","animations/prince_1hand_combo_01.bdae"}};
    config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;
    std::string error;check(body.load(body_assets,config,error),error);
    const auto* scene=body.retained_scene_borrow();check(scene!=nullptr,"Actual body Scene borrow absent");
    std::unique_ptr<dh2::skinning::VisualSkinOwnerV6> owner;SourceEquipmentImageLeaseV1 image;
    check(bind_source_equipment_render_skin(body_assets,body,{&weapons,weapon_read},owner,image,error),error);
    check(image.retention&&image.image.bytes==image.bytes&&image.image.size==image.byte_count&&
          image.resource_uri==config.model_path,"Renderer lease does not pin the skin owner's exact BRES");
    check(body.retained_scene_borrow()==scene,"Binding source skin replaced body Scene");
    const auto table=std::filesystem::path("original-cache/data/pydata");
    auto b=item_assets.read(table/"loot_table_pyarray.bin"),n=item_assets.read(table/"loot_table_pyarraynames.bin"),f=item_assets.read(table/"loot_table_pystructnames.bin");
    dh2::data::ItemTable items;check(dh2::data::load_items({b.data(),b.size()},{n.data(),n.size()},{f.data(),f.size()},items,error),error);
    CharacterState character;character.inventory={{"suit","StartingSuit",1},{"sword","Longsword01",1},{"boots","StartingBoots",1},{"hands","StartingGloves",1}};
    character.equipment={{"slot0","suit"},{"slot1","sword"},{"slot3","boots"},{"slot4","hands"}};
    EquipmentAdapterOptions options;SourceEquipmentAppearancePlan plan;
    check(prepare_source_equipment_appearance(character,items,options.slots,*owner,plan,error),error);
    check(plan.steps.size()==6,"Source category update count differs");
    const unsigned source_order[]{0,1,2,3,4,8};for(unsigned i=0;i<6;++i)check(plan.steps[i].source_slot==source_order[i],"Source update order differs");
    check(plan.steps[1].module==items.rows[dh2::data::item_id(items,"Longsword01")].name&&plan.steps[1].anchor=="anchor_weapon_right_offset","Source Item modular module or SetWeapon anchor differs");
    check(plan.steps[5].module=="MC_Head__naked","Unequip invented module rather than source naked");
    const auto elapsed=body.animation_elapsed_seconds();unsigned debug_calls=0;
    SourceAppearanceDebugServices debug;
    debug.load=[&](std::string&){++debug_calls;return true;};
    debug.query=[&](const char* name,std::string&){++debug_calls;check(std::string(name)=="isTracingChar_Modular","Source Debug switch key differs");return true;};
    check(!apply_source_equipment_appearance(*owner,plan,{},error),"Unbound genuine Debug services accepted");
    check(apply_source_equipment_appearance(*owner,plan,debug,error),error);check(debug_calls==8,"Source modular Debug delivery count differs");
    const std::vector<dh2::skinning::VisualDrawViewV32>* views=nullptr;check(owner->draw_views(views,error),error);
    check(views&&views->size()>4,"Source equipment skin owner lacks actual weapon/body draws");
    check(owner->weapon_uri(1)==plan.steps[1].weapon_uri,"Source weapon retained wrong URI");
    check(body.retained_scene_borrow()==scene&&body.animation_elapsed_seconds()==elapsed,"Source refresh cloned scene or advanced pose");
    SourceEquipmentRenderServicesV1 render_services;
    unsigned material_calls=0,weapon_image_calls=0;
    render_services.weapon_image=[&](const dh2::skinning::VisualDrawViewV32& view,const std::string& uri,
        SourceEquipmentImageLeaseV1& lease,std::string&){
        check(view.weapon_slot==1&&uri==plan.steps[1].weapon_uri,"Weapon renderer requested wrong live source URI");
        struct TestWeaponImage {std::vector<std::uint8_t> bytes;dh2::scene::Scene scene;};
        auto pin=std::make_shared<TestWeaponImage>();pin->bytes=read_content(weapons,uri);dh2::resources::BresView bres{};
        check(dh2_bres_open(&bres,pin->bytes.data(),pin->bytes.size())==dh2::resources::BresError::ok,"Actual weapon BRES lease invalid");
        std::string load_error;check(dh2::scene::load(bres,pin->scene,load_error),load_error);
        lease.retention=pin;lease.bytes=pin->bytes.data();lease.byte_count=pin->bytes.size();lease.image=bres;
        lease.material_scene=&pin->scene;lease.resource_uri=uri;++weapon_image_calls;return true;
    };
    render_services.material=[&](const SourceEquipmentDrawIdentityV1& source,const dh2::resources::BresView& bres,
        const dh2::scene::Material& material,Material& output,std::string&){
        check(bres.bytes&&material.id==source.material_id,"Renderer material source mismatch");
        output.sourcePass=SourceMaterialPass{};output.lightingEnabled=false;
        if(!material.diffuse.empty())output.texture=1;++material_calls;return true;
    };
    SourceEquipmentRenderBridgeV1 renderer(*owner,image,render_services);
    const auto render_clock=body.animation_elapsed_seconds();
    std::shared_ptr<const SourceEquipmentRenderFrameV1> frame;check(renderer.prepare(frame,error),error);
    check(body.animation_elapsed_seconds()==render_clock,"Renderer preparation advanced the live source clock");
    check(frame&&!frame->packets.empty()&&material_calls==frame->packets.size()&&weapon_image_calls>0,
          "Renderer bridge omitted source primitive packets or weapon BRES lease");
    auto incomplete_services=render_services;incomplete_services.weapon_image={};
    SourceEquipmentRenderBridgeV1 incomplete_renderer(*owner,image,incomplete_services);
    std::shared_ptr<const SourceEquipmentRenderFrameV1> rejected_frame;
    check(!incomplete_renderer.prepare(rejected_frame,error)&&!error.empty()&&!rejected_frame,
          "Renderer accepted missing exact weapon BRES lease callback");
    check(owner->draw_views(views,error),error);std::size_t packet_index=0;
    for(const auto& view:*views)for(std::size_t p=0;p<view.geometry->primitives.size();++p){
        const auto& packet=frame->packets.at(packet_index++);const auto& primitive=view.geometry->primitives[p];
        const auto material_id=view.material_table->at(view.materials->at(p)).id;
        const auto uv_index=primitive.attributes[4];const auto& uv=view.geometry->attributes.at(std::size_t(uv_index));
        check(packet.source_retention&&packet.source.geometry_id==view.geometry->id&&
              packet.source.material_id==material_id&&packet.source.category==view.category&&
              packet.source.module==view.module&&packet.source.weapon_slot==view.weapon_slot&&
              packet.source.pose_revision==view.pose_revision&&packet.mesh.indices==primitive.indices&&
              packet.world==view.world&&packet.mesh.vertices.size()==view.positions->size(),
              "Renderer packet lost original source identity, topology or transform");
        for(std::size_t v=0;v<view.positions->size();++v){
            const auto& source_position=view.positions->at(v);const auto& vertex=packet.mesh.vertices[v];
            check(vertex.position.x==source_position[0]&&vertex.position.y==source_position[1]&&
                  vertex.position.z==source_position[2]&&vertex.u==uv.values[v*2]&&vertex.v==uv.values[v*2+1],
                  "Renderer packet changed original posed positions or UVs");
        }
    }
    check(packet_index==frame->packets.size(),"Renderer packet count differs from source primitive count");
    std::uint32_t next_texture=1;unsigned decoded_uploads=0,submitted_frames=0;
    effects::EffectTextureServices textures;
    textures.upload=[&](const TextureImage& decoded,std::uint32_t& texture,std::string&){
        check(decoded.width>0&&decoded.height>0&&!decoded.rgba.empty(),"Original material texture decode is empty");
        texture=next_texture++;++decoded_uploads;return true;
    };
    textures.release=[](std::uint32_t){};
    SourceEquipmentOriginalBindingsV1 original_bindings(weapons,body_assets,std::move(textures),
        [&](std::shared_ptr<const SourceEquipmentRenderFrameV1> submitted,std::string&){
            check(submitted&&!submitted->packets.empty(),"Original renderer submit received empty frame");
            ++submitted_frames;return true;
        });
    SourceEquipmentRenderBridgeV1 original_renderer(*owner,image,original_bindings.callbacks());
    std::shared_ptr<const SourceEquipmentRenderFrameV1> original_frame;
    check(original_renderer.prepare(original_frame,error),error);
    check(original_frame&&original_frame->packets.size()==frame->packets.size()&&
          original_bindings.texture_count()>0&&decoded_uploads>0,
          "Original COMMON material/texture binding omitted source packets or textures");
    check(owner->draw_views(views,error),error);
    const auto weapon_view=std::find_if(views->begin(),views->end(),[](const auto& view){return view.weapon_slot==1;});
    check(weapon_view!=views->end(),"Actual weapon draw view absent for BRES mismatch test");
    auto actual_callbacks=original_bindings.callbacks();SourceEquipmentImageLeaseV1 mismatched_image;
    check(!actual_callbacks.weapon_image(*weapon_view,"data/3d/characters/prince/weapons/not-the-current-weapon.bdae",
          mismatched_image,error)&&!mismatched_image.retention,
          "Weapon BRES resolver accepted a URI without the actual source weapon resource");
    // The shipped lizard body does not select this branch. This test-only
    // derivative selects its actual serialized `lighting` technique by moving
    // that existing technique record to index zero; shader/pass bytes remain
    // untouched. It exercises the production callback's missing-light lease
    // failure without claiming a shipped model or runtime selects this branch.
    AssetCatalog original_cache(root/".local-inputs/windows-shared-assets/original-cache");
    const std::string lizard_uri="data/3d/characters/lizardman/lizardman.bdae";
    auto lizard_bytes=original_cache.read(lizard_uri);dh2::resources::BresView lizard_image{};
    check(dh2_bres_open(&lizard_image,lizard_bytes.data(),lizard_bytes.size())==dh2::resources::BresError::ok,
          "Original-cache lizard COMMON BRES invalid");
    std::uint32_t material_row=0,effect_row=0;
    for(unsigned i=0;i<dh2_bres_library_count(&lizard_image,dh2::resources::Library::material);++i){
        const auto* row=dh2_bres_library_item(&lizard_image,dh2::resources::Library::material,static_cast<std::int32_t>(i));
        if(row&&bres_text(lizard_image, bres_word(lizard_image,static_cast<std::uint32_t>(row-lizard_image.bytes)))=="diffuse"){
            material_row=static_cast<std::uint32_t>(row-lizard_image.bytes);break;
        }
    }
    check(material_row!=0,"Original-cache lizard diffuse COMMON material missing");
    auto effect_uri=bres_text(lizard_image,bres_word(lizard_image,material_row+12));
    check(!effect_uri.empty()&&effect_uri.front()=='#',"Lizard diffuse effect is not embedded COMMON");
    for(unsigned i=0;i<dh2_bres_library_count(&lizard_image,dh2::resources::Library::effect);++i){
        const auto* row=dh2_bres_library_item(&lizard_image,dh2::resources::Library::effect,static_cast<std::int32_t>(i));
        if(row&&bres_text(lizard_image,bres_word(lizard_image,static_cast<std::uint32_t>(row-lizard_image.bytes)))==effect_uri.substr(1)){
            effect_row=static_cast<std::uint32_t>(row-lizard_image.bytes);break;
        }
    }
    check(effect_row!=0,"Original-cache lizard diffuse effect record missing");
    const auto technique_count=bres_word(lizard_image,effect_row+32);
    const auto technique_base=bres_word(lizard_image,effect_row+36);
    std::uint32_t lighting_row=0;
    for(unsigned i=0;i<technique_count;++i){const auto row=technique_base+12*i;
        if(bres_text(lizard_image,bres_word(lizard_image,row))=="lighting")lighting_row=row;
    }
    check(lighting_row&&bres_text(lizard_image,bres_word(lizard_image,technique_base))=="default",
          "Original-cache COMMON default/lighting variants differ");
    std::array<std::uint8_t,12> default_technique{};
    std::memcpy(default_technique.data(),lizard_bytes.data()+technique_base,default_technique.size());
    std::memcpy(lizard_bytes.data()+technique_base,lizard_bytes.data()+lighting_row,default_technique.size());
    std::memcpy(lizard_bytes.data()+lighting_row,default_technique.data(),default_technique.size());
    dh2::resources::BresView selected_lit_image{};
    check(dh2_bres_open(&selected_lit_image,lizard_bytes.data(),lizard_bytes.size())==dh2::resources::BresError::ok,
          "Derived original-cache lighting BRES invalid");
    CommonMaterialPass selected_lit_pass;
    check(resolve_common_material_pass(selected_lit_image,"diffuse",selected_lit_pass,error)==CommonMaterialPassResult::applied,error);
    check(selected_lit_pass.technique=="lighting"&&
          classifySourceVertexLighting(selected_lit_pass.vertexShader,selected_lit_pass.vertexDefines)==SourceVertexLighting::CommonLit,
          "Derived fixture did not select original serialized COMMON LIGHTING pass");
    dh2::scene::Scene lizard_scene;check(dh2::scene::load(selected_lit_image,lizard_scene,error),error);
    const auto lizard_material=std::find_if(lizard_scene.materials.begin(),lizard_scene.materials.end(),
        [](const auto& material){return material.id=="diffuse";});
    check(lizard_material!=lizard_scene.materials.end(),"Selected lit fixture lost original material record");
    SourceEquipmentDrawIdentityV1 lit_identity;lit_identity.resource_uri=lizard_uri;
    lit_identity.material_id="diffuse";lit_identity.primitive_index=0;lit_identity.material_index=0;
    Material untouched_output;untouched_output.texture=0x55aa;
    untouched_output.color={.2f,.3f,.4f,.5f};untouched_output.sourcePass=SourceMaterialPass{};
    const auto output_color=untouched_output.color;
    const auto submit_count_before_lit=submitted_frames;
    check(!actual_callbacks.material(lit_identity,selected_lit_image,*lizard_material,untouched_output,error),
          "Production equipment callback accepted lit COMMON without source light lease");
    check(error=="Required original FX lit vertex shader/light-set evaluation",
          "Lit COMMON rejection did not preserve the established source-lighting diagnostic");
    check(untouched_output.texture==0x55aa&&untouched_output.color==output_color&&
          untouched_output.sourcePass&&untouched_output.sourcePass->blend==false&&
          untouched_output.sourcePass->depthWrite==true&&submitted_frames==submit_count_before_lit,
          "Failed lit COMMON binding fabricated a default material or submitted a draw packet");
    check(original_renderer.submit(error),error);check(submitted_frames==1,"Original frame submit callback not delivered");
    const auto retained_first=frame->packets.front().mesh.vertices.front().position;
    const auto old_pose_revision=frame->packets.front().source.pose_revision;
    const auto retained_frame=frame;check(body.update(.05,error),error);check(renderer.prepare(frame,error),error);
    check(frame->packets.front().source.pose_revision>=old_pose_revision&&
          retained_frame->packets.front().mesh.vertices.front().position.x==retained_first.x,
          "Deferred renderer packet changed copied source position after live pose update");
    std::vector<std::vector<std::array<float,3>>> before;for(const auto& view:*views)before.push_back(*view.positions);
    check(body.select("attack",false,error)&&body.update(.15,error),error);check(owner->draw_views(views,error),error);
    bool moved=false;for(unsigned i=0;i<views->size();++i)if(before[i]!=*(*views)[i].positions)moved=true;
    check(moved&&body.retained_scene_borrow()==scene,"Source owner did not follow exact live body pose");
    EquipmentAttachmentSet staged;check(prepare_source_equipment_attachments(weapons,body,plan,staged,error),error);
    check(staged.attachments().size()==1,"Caller attachment owner stage differs");
    Mat4 socket;check(body.bone_world(plan.steps[1].anchor,socket,error),error);
    check(staged.attachments()[0].socket_world==socket,"Attachment staging guessed source offset");
    character.equipment.clear();check(prepare_source_equipment_appearance(character,items,options.slots,*owner,plan,error),error);
    check(apply_source_equipment_appearance(*owner,plan,debug,error),error);check(owner->weapon_uri(1).empty(),"Source unequip left weapon retained");
    check(owner->draw_views(views,error)&&views->size()==4,"Source naked body module count differs");
    check(prepare_source_equipment_attachments(weapons,body,plan,staged,error)&&staged.attachments().empty(),"Caller attachment unequip left stale geometry");
    std::cout<<"source_equipment_appearance PASS: same live Scene, exact appearance, source material leases, lit COMMON fail-closed, naked unequip\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
