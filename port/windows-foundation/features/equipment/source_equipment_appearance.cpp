#include "source_equipment_appearance.hpp"
#include "../../asset_catalog.hpp"
#include <algorithm>
#include <set>
namespace dh::foundation {
bool bind_source_equipment_skin(const AssetCatalog& assets,const CharacterVisual& body,
 dh2::skinning::VisualAssetServicesV6 services,
 std::unique_ptr<dh2::skinning::VisualSkinOwnerV6>& output,std::string& error){
    const auto* scene=body.retained_scene_borrow();const auto* config=body.configuration();
    if(!scene||!config){error="Source equipment requires the actual loaded body Scene";return false;}
    try{
        dh2::skinning::VisualSkinResourcesV6 resources;
        if(!resources.load(assets.read(config->model_path),error))return false;
        auto next=std::make_unique<dh2::skinning::VisualSkinOwnerV6>(resources.borrow(),*scene,services);
        if(!next->initialize(error))return false;
        output=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool prepare_source_equipment_appearance(const CharacterState& character,const dh2::data::ItemTable& items,
 const std::vector<std::string>& slots,const dh2::skinning::VisualSkinOwnerV6& owner,
 SourceEquipmentAppearancePlan& output,std::string& error){
    if(slots.size()!=9||std::set<std::string>(slots.begin(),slots.end()).size()!=9){error="Source appearance requires nine distinct source slot names";return false;}
    // Exact PlayerGearEffectsV5::update_skin categories (note source8Head).
    constexpr const char* categories[]{"MC_Torso","MC_RWeapon","MC_LWeapon","MC_Feet","MC_Hands",nullptr,nullptr,nullptr,"MC_Head"};
    SourceEquipmentAppearancePlan next;
    for(unsigned slot=0;slot<9;++slot){
        if(!categories[slot])continue;
        SourceEquipmentAppearanceStep step;step.source_slot=slot;step.category=categories[slot];
        const InventoryItem* selected=nullptr;
        for(const auto& binding:character.equipment)if(binding.slot==slots[slot]){
            if(selected){error="Duplicate source appearance slot";return false;}
            auto found=std::find_if(character.inventory.begin(),character.inventory.end(),[&](const auto& x){return x.instance_id==binding.item_instance_id;});
            if(found==character.inventory.end()){error="Source appearance equipment binding is not owned";return false;}
            selected=&*found;
        }
        step.equipped=selected!=nullptr;
        if(selected){const auto* item=dh2::data::item(items,dh2::data::item_id(items,selected->definition_id));
            if(!item){error="Actual source appearance item definition missing";return false;}step.module=item->name;
        }else step.module=step.category+"__naked";
        if(slot==1||slot==2){
            if(step.module.find("Shield")!=std::string::npos)step.weapon_mode=0;
            else if(step.module.find("Bow")!=std::string::npos)step.weapon_mode=2;
            else if(slot==2&&step.module.find("Claw")!=std::string::npos){
                auto at=step.module.find("RWeapon");if(at!=std::string::npos)step.module[at]='L';step.weapon_mode=2;
            }else step.weapon_mode=std::int32_t(slot);
            constexpr const char* anchors[]{"anchor_shield_left_offset","anchor_weapon_right_offset","anchor_weapon_left_offset"};
            step.anchor=anchors[step.weapon_mode];
            if(step.equipped)step.weapon_uri="data/3d/characters/prince/weapons/"+step.module+".bdae";
        }else{
            step.category_id=owner.category_id(step.category.c_str());
            step.module_id=owner.module_id(step.category_id,step.module.c_str());
            if(step.equipped&&step.module_id==-1){step.module=step.category+"__placeholder";step.module_id=owner.module_id(step.category_id,step.module.c_str());}
        }
        next.steps.push_back(std::move(step));
    }
    output=std::move(next);error.clear();return true;
}
bool apply_source_equipment_appearance(dh2::skinning::VisualSkinOwnerV6& owner,
 const SourceEquipmentAppearancePlan& plan,const SourceAppearanceDebugServices& debug,std::string& error){
    if(!debug.load||!debug.query){error="Source modular refresh requires actual Debug Load/GetSwitch services";return false;}
    for(const auto& step:plan.steps){
        if(step.weapon_mode!=-1){
            if(!owner.set_weapon(step.equipped?step.module.c_str():nullptr,std::int32_t(step.source_slot),step.weapon_mode,error))return false;
        }else{
            if(!debug.load(error)||!debug.query("isTracingChar_Modular",error)||!owner.set_modular(step.category_id,step.module_id,error))return false;
        }
    }
    error.clear();return true;
}
bool prepare_source_equipment_attachments(const AssetCatalog& assets,const CharacterVisual& body,
 const SourceEquipmentAppearancePlan& plan,EquipmentAttachmentSet& output,std::string& error){
    std::vector<EquipmentVisualDefinition> definitions;
    for(const auto& step:plan.steps)if(step.weapon_mode!=-1&&step.equipped){
        EquipmentVisualDefinition definition;definition.id=step.category;definition.model_uri=step.weapon_uri;definition.anchor_name=step.anchor;
        definitions.push_back(std::move(definition));
    }
    EquipmentAttachmentSet next;
    if(!next.load(assets,definitions,error)||!next.update(body,error))return false;
    output=std::move(next);return true;
}
}
