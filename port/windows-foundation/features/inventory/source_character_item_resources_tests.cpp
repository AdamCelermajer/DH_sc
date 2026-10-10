#include "source_character_item_resources.hpp"
#include "../character_menu/menu_text.hpp"
#include "../../asset_catalog.hpp"
#include "../../../level-world/character_candidate_cache_v62.hpp"
#include "../../../level-world/canonical_character_spawn_select_v87.hpp"
#include "../../../level-world/canonical_character_save_v86.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::inventory;
using namespace dh2;

static void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
static data::Bytes span(const std::vector<std::uint8_t>& value){return {value.data(),value.size()};}

int main(int argc,char** argv){try{
    check(argc==2,"Supply actual staged source assets root");
    auto assets=std::make_shared<AssetCatalog>(argv[1]);std::string error;
    auto read=[&](const std::string& path){return assets->read(std::string("original-cache/")+path);};

    character::ScriptAssetServicesV1 files;
    files.read=[assets](const std::string& uri,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){
        try{bytes=assets->read(std::string("original-cache/")+uri);found=true;e.clear();return true;}
        catch(const std::exception& failure){found=false;e=failure.what();return false;}
    };
    auto cache=std::make_shared<character::CharacterCandidateCacheV62>(files);
    check(cache->load(error),error);

    std::vector<std::vector<std::uint8_t>> design_bytes;design_bytes.reserve(15);
    character::GameDesignInputs256 design_input{};
    character::GameDesignTableInput48* tables[]{&design_input.characters,&design_input.classes,
        &design_input.ai,&design_input.factions,&design_input.levels};
    const char* table_names[]{"character_properties","character_classes","ai","ai_factions","levels"};
    for(unsigned i=0;i<5;++i){
        for(const char* suffix:{"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"})
            design_bytes.push_back(read(std::string("data/pydata/")+table_names[i]+suffix));
        const auto base=i*3;*tables[i]={span(design_bytes[base]),span(design_bytes[base+1]),span(design_bytes[base+2])};
    }
    auto design=std::make_shared<character::CharacterGameDesign>();
    check(design->initialize(design_input,error),error);
    auto localization=std::make_shared<character_menu::MenuLocalization>();
    check(localization->load(*assets,"original-cache/data",0,error),error);
    check(localization->bind_profile(nullptr,error),error);
    ui::HudTextV1* text{};ui::HudTextEnvironmentV1 environment;
    check(localization->borrow_text(text,environment,error),error);

    character::SourceItemResourceInputsV88 resource_input;
    resource_input.owner=cache;resource_input.localization_owner=localization;
    resource_input.loot=cache->loot().borrow();resource_input.design=design->borrow();
    resource_input.localization=text;resource_input.text_environment=environment;
    resource_input.asset=[assets](const char* uri,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){
        if(!uri){e="Null source item asset URI";return false;}
        try{bytes=assets->read(std::string("original-cache/")+uri);found=true;e.clear();return true;}
        catch(const std::exception& failure){found=false;e=failure.what();return false;}
    };
    auto item_resources=std::make_shared<character::SourceItemResourcesV88>(std::move(resource_input));
    check(item_resources->load(error),error);

    // This shell proves typed association/leases only. It intentionally has no
    // actor, Save, completed-init receipt, or live Gear/player authority.
    auto record=std::make_shared<world::CanonicalCharacterCandidateRecordV60>();
    record->services.loot_tables=&cache->loot();record->services.design=design.get();
    record->design=design->borrow();
    auto associations=std::make_shared<SourceCharacterItemResourcesV1>();
    player::PlayerEquipmentRenderInputsV1 gear_inputs;
    auto host_services=std::make_shared<std::string>("existing gear host lease");
    check(associations->configure_gear_inputs(record,item_resources,gear_inputs,environment,{host_services},error),error);
    check(&gear_inputs.immutable_loot_v88.items()==&cache->loot().borrow().items()&&
          &gear_inputs.immutable_powers_v88.rows()==&item_resources->definitions().rows()&&
          gear_inputs.text_environment.localization.context==environment.localization.context&&
          gear_inputs.services_lease_v62,
          "Gear inputs did not receive the same cached Loot/Power/text environment/resource pin");
    auto borrow=associations->borrower();
    std::shared_ptr<const character::SourceItemResourcesV88> borrowed;
    check(borrow(*record,borrowed,error),error);
    check(borrowed.get()==item_resources.get()&&borrowed->presentation().get()==item_resources->presentation().get(),
          "Typed V60 association returned a duplicate or different Power/Text resource owner");
    auto other=std::make_shared<world::CanonicalCharacterCandidateRecordV60>();
    other->services.loot_tables=&cache->loot();other->services.design=design.get();other->design=design->borrow();
    check(!borrow(*other,borrowed,error),"Item resources borrowed through a foreign Character record");
    check(associations->release_after_character_unpublication(*record,error),error);
    check(!borrow(*record,borrowed,error),"Released Character retained its typed SourceItemResources association");
    std::cout<<"source character item resources PASS actual cache/design/localization, exact typed owner association and Gear pin\n";
    return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
