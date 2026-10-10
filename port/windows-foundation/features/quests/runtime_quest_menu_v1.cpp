#include "runtime_quest_menu_v1.hpp"
#include "runtime_quest_menu_art_v1.hpp"
#include <algorithm>

namespace dh::foundation {
namespace {
bool fail(std::string& error,const char* message){error=message;return false;}
bool same_quest(const CharacterQuestIdV1& a,const CharacterQuestIdV1& b){
    return a.collection==b.collection&&a.difficulty==b.difficulty&&a.row==b.row;
}
// One authored list (Assigned or Completed) as page rows; authored_row_index is the list position.
std::vector<RuntimeQuestMenuRowV1> rows_of_list_v1(const CharacterQuestPageSnapshotV1& snapshot,
    const RuntimeQuestMenuArtV1& art){
    std::vector<RuntimeQuestMenuRowV1> rows;rows.reserve(snapshot.rows.size());
    for(std::size_t index=0;index<snapshot.rows.size();++index){
        const auto& row=snapshot.rows[index];
        rows.push_back({row.id,row.title,row.current,
            static_cast<std::uint32_t>(index),static_cast<std::uint32_t>(index*art.row_step_swf_pixels)});
    }
    return rows;
}
bool list_contains_v1(const CharacterQuestPageSnapshotV1& snapshot,const CharacterQuestIdV1& id){
    return std::any_of(snapshot.rows.begin(),snapshot.rows.end(),
        [&](const auto& row){return same_quest(row.id,id);});
}
std::array<float,6> compose(const std::array<float,6>& a,const std::array<float,6>& b){
    return {a[0]*b[0]+a[2]*b[1],a[1]*b[0]+a[3]*b[1],
            a[0]*b[2]+a[2]*b[3],a[1]*b[2]+a[3]*b[3],
            a[0]*b[4]+a[2]*b[5]+a[4],a[1]*b[4]+a[3]*b[5]+a[5]};
}
character_menu::MenuTextField place_field(character_menu::MenuTextField field,
    const std::array<float,6>& placement){
    field.matrix=compose(placement,field.matrix);
    const auto& b=field.local_bounds;const auto& m=field.matrix;
    const std::array<std::array<float,2>,4> points{{
        {{m[0]*b[0]+m[2]*b[2]+m[4],m[1]*b[0]+m[3]*b[2]+m[5]}},
        {{m[0]*b[1]+m[2]*b[2]+m[4],m[1]*b[1]+m[3]*b[2]+m[5]}},
        {{m[0]*b[1]+m[2]*b[3]+m[4],m[1]*b[1]+m[3]*b[3]+m[5]}},
        {{m[0]*b[0]+m[2]*b[3]+m[4],m[1]*b[0]+m[3]*b[3]+m[5]}}
    }};
    field.bounds={points[0][0],points[0][0],points[0][1],points[0][1]};
    for(const auto& p:points){field.bounds[0]=std::min(field.bounds[0],p[0]);
        field.bounds[1]=std::max(field.bounds[1],p[0]);
        field.bounds[2]=std::min(field.bounds[2],p[1]);
        field.bounds[3]=std::max(field.bounds[3],p[1]);}
    return field;
}
void place_batch(HudGeometryBatch& batch,const std::array<float,6>& matrix,
                 const std::string& suffix={}){
    if(!suffix.empty())batch.role+=suffix;
    for(auto& v:batch.triangles){const auto x=v.x,y=v.y;
        v.x=matrix[0]*x+matrix[2]*y+matrix[4];
        v.y=matrix[1]*x+matrix[3]*y+matrix[5];}
}
character_menu::MenuSolidBatch place_solid(character_menu::MenuSolidBatch solid,
    const std::array<float,6>& matrix,const std::string& suffix,
    const std::string& after={}){
    if(!suffix.empty())solid.geometry.role+=suffix;
    if(!after.empty())solid.after_bitmap_role=after;
    for(auto& v:solid.geometry.triangles){const auto x=v.x,y=v.y;
        v.x=matrix[0]*x+matrix[2]*y+matrix[4];
        v.y=matrix[1]*x+matrix[3]*y+matrix[5];}
    return solid;
}
}

RuntimeQuestMenuV1::RuntimeQuestMenuV1(CharacterState& character,
    CharacterQuestProgressV1& progress,
    std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> tables,
    CharacterQuestLogPolicyV1 policy, CharacterQuestTextV1 text)
    : character_(&character),progress_(&progress),tables_(std::move(tables)),policy_(policy),
      page_(character,progress,tables_,std::move(text)) {
    frame_.art=source_art;
}

bool RuntimeQuestMenuV1::load_progress_from_character(std::string& error){
    if(!character_||!progress_||!tables_||!tables_->ready())
        return fail(error,"Quest save load requires the same CharacterState, progress model and original Quest table");
    if(character_->source_quest_progress_cqpg.empty()){
        if(progress_->belongs_to(*character_)){
            CharacterQuestProgressV1::BucketView bucket;
            if(!progress_->bucket(*character_,0,0,bucket,error))return false;
            if(bucket.origin!=CharacterQuestProgressV1::Origin::unknown)
                return fail(error,"Initialized Quest progress cannot be loaded from an empty legacy CharacterState field");
            error.clear();return true;
        }
        return progress_->bind_character(*character_,error);
    }
    return progress_->decode(*character_,*tables_,character_->source_quest_progress_cqpg,error);
}

bool RuntimeQuestMenuV1::persist_progress_to_character(std::string& error){
    if(!character_||!progress_||!tables_||!progress_->belongs_to(*character_))
        return fail(error,"Quest save write requires the matching CharacterState/progress owner and original Quest table");
    std::vector<std::uint8_t> encoded;
    if(!progress_->encode(*tables_,encoded,error))return false;
    character_->source_quest_progress_cqpg=std::move(encoded);
    error.clear();return true;
}

bool RuntimeQuestMenuV1::initialize_fresh_progress(std::string& error){
    if(!character_||!progress_||!tables_||!tables_->ready())
        return fail(error,"Fresh Quest initialization requires the same CharacterState and original Quest table");
    if(!progress_->initialize_fresh(*character_,*tables_,error))return false;
    return persist_progress_to_character(error);
}

bool RuntimeQuestMenuV1::record_accepted_source_state(const CharacterQuestIdV1& id,
    std::int32_t source_state,std::string& error){
    if(!character_||!progress_||!progress_->record_source_state(*character_,id,source_state,error))return false;
    return persist_progress_to_character(error);
}

bool RuntimeQuestMenuV1::refresh(std::uint32_t collection,
    std::int32_t difficulty,CharacterQuestCategoryV1 category,std::string& error) {
    if(!character_||!progress_||!progress_->belongs_to(*character_))
        return fail(error,"Quest menu requires its original CharacterState/progress owner");
    CharacterQuestProgressV1::BucketView bucket;
    if(!progress_->bucket(*character_,collection,difficulty,bucket,error))return false;
    RuntimeQuestMenuFrameV1 staged;
    staged.art=source_art;staged.collection=collection;staged.difficulty=difficulty;
    staged.category=category;staged.progress_origin=bucket.origin;
    if(bucket.origin==CharacterQuestProgressV1::Origin::unknown){
        staged.availability=RuntimeQuestMenuAvailabilityV1::unknown;
        frame_=std::move(staged);current_page_valid_=false;error.clear();return true;
    }
    // Both authored lists are shown at once (source sheet: AllQuests/content/Assigned and .../Completed).
    CharacterQuestPageSnapshotV1 assigned,completed;
    if(!page_.refresh(collection,difficulty,CharacterQuestCategoryV1::assigned,policy_,assigned,error))return false;
    if(!page_.refresh(collection,difficulty,CharacterQuestCategoryV1::completed,policy_,completed,error))return false;
    staged.availability=RuntimeQuestMenuAvailabilityV1::ready;
    staged.progress_origin=bucket.origin;
    staged.rows=rows_of_list_v1(assigned,source_art);
    staged.completed_rows=rows_of_list_v1(completed,source_art);
    // The CharacterMenu binding refreshes every frame while the tab is open. Keep the selected row while it is
    // still listed (its details are re-read from the same progress), otherwise the selection is cleared.
    if(frame_.selection){
        const auto& id=frame_.selection->row.id;
        const auto* list=list_contains_v1(assigned,id)?&assigned:list_contains_v1(completed,id)?&completed:nullptr;
        if(list){
            CharacterQuestPageSelectionV1 selection;
            if(!page_.select(*list,id,selection,error))return false;
            staged.selection=std::move(selection);
        }
    }
    frame_=std::move(staged);current_page_valid_=true;error.clear();return true;
}

bool RuntimeQuestMenuV1::route_hit(RuntimeQuestMenuHitV1 hit,
    const CharacterQuestIdV1& id,std::string& error) {
    if(!current_page_valid_||frame_.availability!=RuntimeQuestMenuAvailabilityV1::ready)
        return fail(error,"Quest SWF hit route requires a ready source page snapshot");
    CharacterQuestPageSnapshotV1 assigned,completed;
    if(!page_.refresh(frame_.collection,frame_.difficulty,CharacterQuestCategoryV1::assigned,
                      policy_,assigned,error))return false;
    if(!page_.refresh(frame_.collection,frame_.difficulty,CharacterQuestCategoryV1::completed,
                      policy_,completed,error))return false;
    if(hit==RuntimeQuestMenuHitV1::quest_row_release){
        // A row release selects from whichever authored list holds the row (Completed rows show details only).
        const auto& list=list_contains_v1(assigned,id)?assigned:completed;
        CharacterQuestPageSelectionV1 selection;
        if(!page_.select(list,id,selection,error))return false;
        frame_.selection=std::move(selection);error.clear();return true;
    }
    if(!page_.activate(assigned,id,policy_,error))return false;
    if(!persist_progress_to_character(error))return false;
    frame_.selection.reset();
    // Source `NativeSetCurrentQuest` mutates currentquest; refresh consumes
    // that same owner and makes the source LightOn marker visible.
    return refresh(frame_.collection,frame_.difficulty,frame_.category,error);
}

RuntimeQuestCharacterMenuBindingV1::RuntimeQuestCharacterMenuBindingV1(
    std::shared_ptr<RuntimeQuestMenuV1> menu,
    std::shared_ptr<CharacterState> character,std::shared_ptr<void> same_source_owner,
    RuntimeQuestMenuActivePageV1 active_page,
    RuntimeQuestMenuSymbolTextV1 source_symbol_text)
    : menu_(std::move(menu)),character_(std::move(character)),
      source_owner_(std::move(same_source_owner)),
      active_page_(std::move(active_page)),source_symbol_text_(std::move(source_symbol_text)) {}

bool RuntimeQuestCharacterMenuBindingV1::append_source_page(
    const character_menu::Bindings& bindings,character_menu::Frame& frame,
    std::string& error) const {
    if(!menu_||!character_||bindings.character!=character_.get()||
       menu_->character_owner()!=character_.get())
        return fail(error,"Quest page projection requires the same CharacterMenu and progress CharacterState owner");
    const auto& snapshot=menu_->frame();
    const auto& art=original_runtime_quest_menu_art_v1();
    auto next=frame;
    next.art.batches.insert(next.art.batches.end(),art.page_art.begin(),art.page_art.end());
    next.solids.insert(next.solids.end(),art.page_solids.begin(),art.page_solids.end());

    auto symbol=[&](const char* name,std::string& value)->bool{
        if(!source_symbol_text_){error="Original Quest menu source-symbol provider is unavailable";return false;}
        if(!source_symbol_text_(name,value,error)){if(error.empty())error="Original Quest menu symbol lookup failed";return false;}
        return true;
    };
    for(const auto& field:art.page_fields){
        const char* key=nullptr;
        if(field.path=="menu_QuestLogSheetNEW/menu_title/txt_title")key="GAMEPLAYMENUS_QUEST_JOURNAL_TITLE";
        else if(field.path=="menu_QuestLogSheetNEW/AllQuests/content/Assigned/Header/text")key="GAMEPLAYMENUS_QUEST_LOG";
        else if(field.path=="menu_QuestLogSheetNEW/AllQuests/content/Completed/CompletedTitle/text")key="GAMEPLAYMENUS_COMPLETED";
        else if(field.path=="menu_QuestLogSheetNEW/QuestDetailsText/text"&&snapshot.selection&&snapshot.selection->details.title)
            next.text.push_back({field,*snapshot.selection->details.title});
        else if(field.path=="menu_QuestLogSheetNEW/QuestDesc/Description/text"&&snapshot.selection&&snapshot.selection->details.pre_description)
            next.text.push_back({field,*snapshot.selection->details.pre_description});
        // Tag above the title: Quest::IsPrimary selects MAIN QUEST, otherwise SIDE QUEST (menu.english 317/316).
        else if(field.path=="menu_QuestLogSheetNEW/QuestMain/text"&&snapshot.selection)
            key=snapshot.selection->details.primary?"MENU_MAIN_QUEST":"MENU_SIDE_QUEST";
        if(key){std::string value;if(!symbol(key,value))return false;
            if(!value.empty())next.text.push_back({field,std::move(value)});}
    }

    // Assigned rows (parent 0, current marker, selectable) and Completed rows (parent 1, never selected
    // with the orange state here: the source Completed list uses the same unselected button art).
    auto draw_rows=[&](const std::vector<RuntimeQuestMenuRowV1>& rows,bool assigned_list){
        const auto category_index=assigned_list?0u:1u;
        const auto parent=art.row_parent_matrices[category_index];
        for(const auto& row:rows){
            const bool selected=snapshot.selection&&same_quest(snapshot.selection->row.id,row.id);
            auto placement=parent;
            const auto offset=static_cast<float>(row.authored_row_index*snapshot.art.row_step_swf_pixels);
            placement[4]+=placement[2]*offset;placement[5]+=placement[3]*offset;
            const auto state=selected?1u:0u;
            const auto suffix=std::string(assigned_list?"/row":"/completed")+std::to_string(row.authored_row_index);
            std::string last_role;
            for(auto batch:art.row_art[state]){
                place_batch(batch,placement,suffix);last_role=batch.role;
                next.art.batches.push_back(std::move(batch));
            }
            for(auto solid:art.row_solids[state]){
                next.solids.push_back(place_solid(std::move(solid),placement,suffix,last_role));
            }
            if(row.is_current&&assigned_list){
                for(auto batch:art.current_marker_art){place_batch(batch,placement,suffix);next.art.batches.push_back(std::move(batch));}
                for(auto solid:art.current_marker_solids)next.solids.push_back(place_solid(solid,placement,suffix,last_role));
            }
            if(row.title){
                for(auto field:art.row_fields[state]){
                    field.path="menu_QuestLogSheetNEW/AllQuests/"+
                        std::string(assigned_list?"content/Assigned":"content/Completed")+
                        "/btnQuests/TextBox/QuestName";
                    next.text.push_back({place_field(std::move(field),placement),*row.title});
                }
            }
        }
    };
    draw_rows(snapshot.rows,true);
    draw_rows(snapshot.completed_rows,false);
    if(snapshot.selection&&snapshot.selection->activation_visible){
        next.art.batches.insert(next.art.batches.end(),art.activate_art.begin(),art.activate_art.end());
        next.solids.insert(next.solids.end(),art.activate_solids.begin(),art.activate_solids.end());
        for(const auto& field:art.activate_fields){
            std::string value;
            if(!symbol("GAMEPLAYMENUS_QUEST_MAKE_ACTIVE",value))return false;
            if(!value.empty())next.text.push_back({field,std::move(value)});
        }
    }
    frame=std::move(next);error.clear();return true;
}

bool RuntimeQuestCharacterMenuBindingV1::show(std::uint32_t collection,
    std::int32_t difficulty,CharacterQuestCategoryV1 category,std::string& error) {
    if(!menu_||!character_||menu_->character_owner()!=character_.get())
        return fail(error,"Quest CharacterMenu binding requires the same CharacterState owner as its RuntimeQuestMenu");
    return menu_->refresh(collection,difficulty,category,error);
}

bool RuntimeQuestCharacterMenuBindingV1::load_progress_from_character(std::string& error){
    if(!menu_||!character_||menu_->character_owner()!=character_.get())
        return fail(error,"Quest save load requires the same CharacterState menu owner");
    return menu_->load_progress_from_character(error);
}

bool RuntimeQuestCharacterMenuBindingV1::initialize_fresh_progress(std::string& error){
    if(!menu_||!character_||menu_->character_owner()!=character_.get())
        return fail(error,"Fresh Quest initialization requires the same CharacterState menu owner");
    return menu_->initialize_fresh_progress(error);
}

bool RuntimeQuestCharacterMenuBindingV1::record_accepted_source_state(
    const CharacterQuestIdV1& id,std::int32_t source_state,std::string& error){
    if(!menu_||!character_||menu_->character_owner()!=character_.get())
        return fail(error,"Accepted Quest state update requires the same CharacterState menu owner");
    return menu_->record_accepted_source_state(id,source_state,error);
}

bool RuntimeQuestCharacterMenuBindingV1::route_hit(RuntimeQuestMenuHitV1 hit,
    const CharacterQuestIdV1& id,std::string& error) {
    if(!menu_||!character_||menu_->character_owner()!=character_.get())
        return fail(error,"Quest hit routing requires the same CharacterState owner as its RuntimeQuestMenu");
    return menu_->route_hit(hit,id,error);
}

const RuntimeQuestMenuFrameV1* RuntimeQuestCharacterMenuBindingV1::current_frame() const noexcept {
    return menu_?&menu_->frame():nullptr;
}

bool RuntimeQuestCharacterMenuBindingV1::source_menu_page_ready(std::string& error) const {
    if(!menu_||!character_||menu_->character_owner()!=character_.get())
        return fail(error,"Quest source menu page lost its same CharacterState/progress owner");
    if(!source_owner_||!active_page_||!source_symbol_text_)
        return fail(error,"Quest source menu page lost its canonical owner or source text/page providers");
    bool active{};
    try { active=active_page_(); }
    catch(...) { return fail(error,"Quest source menu active-page provider threw"); }
    if(!active)return fail(error,"Exact Quest source menu symbol is not the active pushed page");
    if(menu_->frame().availability!=RuntimeQuestMenuAvailabilityV1::ready)
        return fail(error,"Quest source menu page has unknown or unavailable CQPG progress");
    error.clear();return true;
}

bool RuntimeQuestCharacterMenuBindingV1::append_source_menu_page(
    character_menu::Frame& frame,std::string& error) const {
    if(!source_menu_page_ready(error))return false;
    character_menu::Bindings bindings;
    bindings.character=character_.get();
    return append_source_page(bindings,frame,error);
}

bool RuntimeQuestCharacterMenuBindingV1::retains_source_owner(
    const CharacterState* character,const std::shared_ptr<void>& owner) const noexcept {
    return character&&character_.get()==character&&menu_&&
        menu_->character_owner()==character&&owner&&source_owner_.get()==owner.get();
}

bool RuntimeQuestCharacterMenuBindingV1::install_content(
    character_menu::Bindings& bindings,std::string& error) {
    if(content_installed_)return fail(error,"Quest CharacterMenu content callback is already installed");
    if(!menu_||!character_||bindings.character!=character_.get()||
       menu_->character_owner()!=character_.get())
        return fail(error,"CharacterMenu Bindings and Quest menu must borrow the same CharacterState");
    if(!source_owner_||!active_page_||!source_symbol_text_)
        return fail(error,"Quest content callback requires source-owner token, active-page gate and original StringManager symbol provider");
    if(!menu_->load_progress_from_character(error))return false;
    std::shared_ptr<RuntimeQuestCharacterMenuBindingV1> self;
    try { self=shared_from_this(); }
    catch(const std::bad_weak_ptr&) {
        return fail(error,"Quest CharacterMenu binding must be retained by shared_ptr before installation");
    }
    auto previous=std::move(bindings.content);
    bindings.content=[self=std::move(self),previous=std::move(previous)](
        character_menu::Tab tab,character_menu::Frame& frame,std::string& message) mutable {
        if(previous&&!previous(tab,frame,message))return false;
        bool active{};
        try { active=self->active_page_(); }
        catch(...) { message="Quest source-page active gate threw";return false; }
        if(!active){message.clear();return true;}
        if(!self->menu_||!self->character_||self->menu_->character_owner()!=self->character_.get()){
            message="Quest content callback lost its same CharacterState/progress owner";return false;
        }
        character_menu::Bindings source_bindings;
        source_bindings.character=self->character_.get();
        return self->append_source_page(source_bindings,frame,message);
    };
    content_installed_=true;error.clear();return true;
}

} // namespace dh::foundation
