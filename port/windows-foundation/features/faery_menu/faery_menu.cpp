#include "faery_menu.hpp"
#include <algorithm>

namespace dh::foundation::faery_menu {
const SourceArt& original_faery_art();
const std::vector<HudGeometryVertex>& original_faery_tab_hit();

bool present(const Bindings& b,Frame& output,std::string& error){
    if(!b.owner||!b.character||!b.save||!static_cast<bool>(b.tables)){
        error="Faery page requires the actual same-player Save and FaeryTable borrow";return false;
    }
    if(!b.validate_same_owner||!b.current_difficulty||!b.unlocked||!b.text||!b.select){
        error="Faery page requires source owner, difficulty, unlock, text, and ChangeFaery services";return false;
    }
    if(!b.validate_same_owner(*b.save,b.character,error)){
        if(error.empty())error="Faery page Save/Character owner identity failed";return false;
    }
    std::int32_t difficulty{};
    if(!b.current_difficulty(difficulty,error))return false;
    if(difficulty<0||difficulty>=3){error="Source faery difficulty outside Savegame storage";return false;}
    if(!b.save->faeries_initialized()[static_cast<std::size_t>(difficulty)]){
        error="Source faery Save rows were not initialized";return false;
    }
    const auto& tables=b.tables;
    if(tables.lists().empty()||tables.faeries().empty()){
        error="Actual FaeryList/Faery cache has no source rows";return false;
    }
    const auto& ids=tables.lists().front();
    if(ids.size()!=5){error="Source default FaeryList is not the five authored menu slots";return false;}
    const auto selected=b.save->current_faery(static_cast<std::uint32_t>(difficulty));
    if(selected<0||selected>=static_cast<std::int32_t>(ids.size())){
        error="Saved current faery is outside the five source Save/ChangeFaery slots";return false;
    }
    Frame next;next.art=original_faery_art().batches;next.difficulty=difficulty;next.selected_id=selected;
    next.faery_visual.batches=original_faery_image(static_cast<unsigned>(selected));
    for(std::size_t slot=0;slot<ids.size();++slot){
        const auto record_id=ids[slot];
        if(record_id<0||static_cast<std::size_t>(record_id)>=tables.faeries().size()){
            error="Source FaeryList contains an out-of-range Faery id";return false;
        }
        next.faery_ids[slot]=static_cast<std::int32_t>(slot);
        next.table_record_ids[slot]=record_id;
        if(!b.unlocked(b.character,static_cast<std::uint32_t>(slot),next.unlocked[slot],error))return false;
        next.levels[slot]=b.save->faery_level(static_cast<std::uint32_t>(slot),static_cast<std::uint32_t>(difficulty));
        const auto visual=slot==static_cast<std::size_t>(selected)?ButtonVisual::focused:
            next.unlocked[slot]?ButtonVisual::idle:ButtonVisual::locked;
        next.button_visuals[slot]=original_faery_button(static_cast<unsigned>(slot),visual);
        next.button_solid_visuals[slot]=original_faery_button_solids(static_cast<unsigned>(slot),visual);
    }
    for(const auto& field:original_faery_art().text_fields){
        const std::int32_t id=field.path.find("menu_title/")!=std::string::npos?-1:selected;
        std::string value;
        if(!b.text(field.path,id,value,error)){
            if(error.empty())error="Required actual source faery text field was not resolved";return false;
        }
        next.text.push_back({field,std::move(value)});
    }
    output=std::move(next);error.clear();return true;
}

bool activate_slot(const Bindings& b,unsigned slot,Frame& refreshed,std::string& error){
    Frame current;
    if(!present(b,current,error))return false;
    if(slot>=current.faery_ids.size()){error="Faery menu slot outside five authored buttons";return false;}
    // The source ActionScript binds onRelease to every slot, including the
    // disabled-looking frame17 state. It still dispatches NativeHUDSetActiveFaery.
    if(!b.select||!b.select(static_cast<std::uint32_t>(current.faery_ids[slot]),error)){
        if(error.empty())error="Source Character::ChangeFaery owner rejected selection";return false;
    }
    return present(b,refreshed,error);
}

std::function<bool(dh::foundation::character_menu::Tab,
    dh::foundation::character_menu::Frame&,std::string&)> content_callback(Bindings bindings){
    return [bindings=std::move(bindings)](dh::foundation::character_menu::Tab tab,
        dh::foundation::character_menu::Frame& output,std::string& error){
        if(tab!=dh::foundation::character_menu::Tab::faery){error.clear();return true;}
        Frame page;
        if(!present(bindings,page,error))return false;
        auto next=output;
        const auto page_start=next.art.batches.size();
        next.art.batches.insert(next.art.batches.end(),page.art.begin(),page.art.end());
        const auto button_offset=original_faery_buttons_insert_at();
        std::size_t inserted_buttons=0;
        for(const auto& button:page.button_visuals){
            const auto at=next.art.batches.begin()+static_cast<std::ptrdiff_t>(page_start+button_offset+inserted_buttons);
            next.art.batches.insert(at,button.begin(),button.end());inserted_buttons+=button.size();
        }
        for(const auto& slot_solids:page.button_solid_visuals)for(const auto& solid:slot_solids){
            dh::foundation::character_menu::MenuSolidBatch batch;
            batch.geometry=solid.geometry;batch.rgba=solid.rgba;batch.after_bitmap_role=solid.after_bitmap_role;
            next.solids.push_back(std::move(batch));
        }
        const auto image_at=page_start+original_faery_image_insert_at()+inserted_buttons;
        next.art.batches.insert(next.art.batches.begin()+static_cast<std::ptrdiff_t>(image_at),
            page.faery_visual.batches.begin(),page.faery_visual.batches.end());
        for(const auto& item:page.text){
            const auto& f=item.field;
            dh::foundation::character_menu::MenuTextField field{
                f.path,f.character_id,f.font_id,f.source_height,f.bounds,f.rgba,f.align,
                f.matrix,f.local_bounds,f.margins,f.leading};
            next.text.push_back({std::move(field),item.value});
        }
        output=std::move(next);error.clear();return true;
    };
}
}

