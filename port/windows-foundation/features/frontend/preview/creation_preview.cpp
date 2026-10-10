#include "creation_preview.hpp"
#include "scene_materials.hpp"
#include "../../../asset_catalog.hpp"
#include "../../../content_paths.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

namespace dh::foundation::frontend {
namespace {
float bits(std::uint32_t v) {float f;std::memcpy(&f,&v,4);return f;}
std::string path(const AssetCatalog& a,const std::string& uri) {
    return resolve_content_path(a,uri).lexically_relative(a.root()).generic_string();
}
std::vector<std::uint8_t> table(const AssetCatalog& a,const std::string& name) {
    try{return read_content(a,"data/pydata/"+name);}
    catch(const std::runtime_error&){return read_content(a,"data/"+name);}
}
}
bool CreationPreview::load(const AssetCatalog& assets,std::string& error) {
    try {
        const char* groups[]={"character_properties","character_classes","loot_table","animations"};
        const char* suffixes[]={"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
        std::array<std::array<std::vector<std::uint8_t>,3>,4> raw;
        for(unsigned i=0;i<4;++i)for(unsigned j=0;j<3;++j)
            raw[i][j]=table(assets,std::string(groups[i])+suffixes[j]);
        auto bytes=[&](unsigned i,unsigned j){return dh2::data::Bytes{raw[i][j].data(),raw[i][j].size()};};
        auto dictionary=[&](const char* prefix,dh2::data::Dictionary& out){
            const auto values=table(assets,std::string(prefix)+"_pyarray.bin");
            const auto names=table(assets,std::string(prefix)+"_pyarraynames.bin");
            return dh2::data::load_dictionary({names.data(),names.size()},{values.data(),values.size()},out,error);
        };
        dh2::data::CharacterTable characters;dh2::data::ClassTables classes;
        dh2::data::PropertyRules rules;dh2::data::LootTablesV2 loot;
        dh2::data::AnimationTables animations;dh2::data::Dictionary clips,models;
        std::array<dh2::data::ClassPreviewDefinition,3> definitions;
        if(!dh2::data::load_characters(bytes(0,0),bytes(0,1),bytes(0,2),characters,error)||
           !dh2::data::load_classes(bytes(1,0),bytes(1,1),bytes(1,2),classes,error)||
           !dh2::data::load_property_rules(characters,rules,error)||
           !loot.load(bytes(2,0),bytes(2,1),bytes(2,2),error)||
           !dictionary("animations_dictionary",clips)||!dictionary("character_models_dictionary",models)||
           !dh2::data::load_animation_tables(bytes(3,0),bytes(3,1),bytes(3,2),clips,animations,error)||
           !dh2::data::class_preview_definitions(characters,classes,rules,loot.borrow(),animations,clips,definitions,error))return false;
        const auto field=std::find(characters.fields.begin(),characters.fields.end(),"ModelFile");
        if(field==characters.fields.end())throw std::runtime_error("Original ModelFile field absent");
        const auto modelField=std::size_t(field-characters.fields.begin());
        std::array<CreationPreviewActor,3> next;
        for(unsigned i=0;i<3;++i) {
            auto& actor=next[i];actor.definition=definitions[i];
            const auto model=definitions[i].properties.resolved.at(modelField);
            if(model<0||std::size_t(model)>=models.values.size())throw std::runtime_error("Original preview ModelFile dictionary index rejected");
            CharacterVisualConfig config;config.model_path=path(assets,models.values[model]);
            config.template_clip_path=path(assets,definitions[i].template_clip);
            config.clips={{"MenuIdle",path(assets,definitions[i].idle_clip)},{"MenuOnSelect",path(assets,definitions[i].select_clip)}};
            config.controller_ids={"MC_Head__naked-mesh-skin"};
            std::vector<EquipmentVisualDefinition> weapons;
            for(const auto& item:definitions[i].starting_items) {
                if(item.module.find("MC_Torso_")==0||item.module.find("MC_Feet_")==0||item.module.find("MC_Hands_")==0)
                    config.controller_ids.push_back(item.module+"-mesh-skin");
                if(item.module.find("MC_RWeapon_")==0) {
                    if(weapons.size()>=2)throw std::runtime_error("Original starting weapon count exceeds two");
                    EquipmentVisualDefinition weapon;weapon.id="starting-weapon-"+std::to_string(weapons.size());
                    weapon.model_uri=item.module+".bdae";
                    weapon.anchor_name=weapons.empty()?"anchor_weapon_right_offset":"anchor_weapon_left_offset";
                    weapons.push_back(std::move(weapon));
                }
            }
            if(config.controller_ids.size()!=4||weapons.empty())throw std::runtime_error("Original starting appearance incomplete");
            config.expected_controller_count=4;
            // Original optimized prince skeleton omits old_anchor_weapon_*
            // tracks in MenuOnSelect. Retain source missing-target policy.
            config.allow_missing_animation_targets=true;
            if(!actor.body.load(assets,config,error)||!actor.body.select("MenuIdle",true,error)||
               !actor.equipment.load(assets,weapons,error)||!actor.equipment.update(actor.body,error))return false;
            // Character::LoadMeshVisual 3b4f3c: base12/13 use .009;
            // base14 uses .01. Preserve source single-precision constants.
            for(unsigned k=0;k<3;++k)actor.scale[k*5]=float(definitions[i].properties.base[12+k])*bits(k==2?0x3c23d70a:0x3c1374bc);
        }
        actors_=std::move(next);selected_=-1;loaded_=true;error.clear();return true;
    } catch(const std::exception& e){error=e.what();return false;}
}
bool CreationPreview::select(int index,std::string& error) {
    if(!loaded_||index<0||index>2){error="Creation preview class unavailable";return false;}
    if(index!=selected_) {
        // MenuCharacterSelect::Update428498 assigns all three states on the
        // first update and each selection change, including initial Knight.
        for(unsigned i=0;i<actors_.size();++i) {
            const bool selected=int(i)==index;
            if(!actors_[i].body.restart(selected?"MenuOnSelect":"MenuIdle",!selected,error))return false;
            actors_[i].selection_active=selected;
            actors_[i].idle_transition_pending=false;
            actors_[i].idle_transition_delivered=false;
        }
    }
    selected_=index;error.clear();return true;
}
bool CreationPreview::update(double seconds,std::string& error) {
    if(!loaded_||!std::isfinite(seconds)||seconds<0){error="Creation preview time unavailable";return false;}
    for(auto& actor:actors_) {
        if(actor.selection_active&&!actor.idle_transition_pending) {
            std::int32_t start{},end{};
            if(!actor.body.animation_range("MenuOnSelect",start,end,error))return false;
            const auto duration=double(end-start)/1000;
            const auto remaining=std::max(0.,duration-actor.body.animation_elapsed_seconds());
            if(!actor.body.update(std::min(seconds,remaining),error))return false;
            if(seconds>=remaining)actor.idle_transition_pending=true;
        } else if(!actor.idle_transition_pending) {
            if(!actor.body.update(seconds,error))return false;
        }
        if(actor.idle_transition_pending&&idle_provider_) {
            if(!idle_provider_(actor,error)) {
                if(error.empty())error="Original showcase idle-transition provider failed";
                return false;
            }
            actor.idle_transition_pending=false;
            actor.idle_transition_delivered=true;
            actor.selection_active=false;
        }
        if(!actor.equipment.update(actor.body,error))return false;
    }
    error.clear();return true;
}
bool CreationPreview::idle_transition_required()const noexcept {
    return std::any_of(actors_.begin(),actors_.end(),[](const auto& actor){return actor.idle_transition_pending;});
}
const char* CreationPreview::idle_transition_status()const noexcept {
    if(idle_transition_required())return "source-showcase-end-held-idle-owner-required";
    if(selected_>=0&&actors_[selected_].idle_transition_delivered)return "native-idle-provider-delivered";
    return "not-required";
}
Camera original_menu_preview_camera() {
    Camera c;c.eye={0,-900,150};c.target={0,0,225};c.up={0,0,1};
    c.verticalFovDegrees=bits(0x3f3579c8)*(180.f/3.14159265358979323846f);
    c.aspectRatio=bits(0x3fd578e9);c.nearPlane=10;c.farPlane=2000;return c;
}
bool load_menu_preview_backdrop(const AssetCatalog& a,OriginalScene& out,std::string& error) {
    try{auto bytes=read_content(a,"models/main_menu_charactere_swamp.bdae");OriginalScene next;
        if(!decode_original_scene(bytes,next,error)||!apply_preview_scene_materials(bytes,next,error))return false;
        out=std::move(next);return true;}
    catch(const std::exception& e){error=e.what();return false;}
}
} // namespace dh::foundation::frontend
