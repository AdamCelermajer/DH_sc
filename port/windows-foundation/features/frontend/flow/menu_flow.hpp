#pragma once
#include <functional>
#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dh::foundation::frontend::flow {
// A desktop projection of authored navigation, not an ActionScript VM or a
// replacement for MenuManager's animated movie/3D lifecycle.
enum class Mode { offline_single_player, online_unavailable };
enum class CreationStage { editing, saved, assigned, returned_to_main, complete };
struct SlotFact { int id = -1; bool in_use = false; std::filesystem::path save_path; };
struct SourceText {
    std::string path,value;
    bool localization_symbol=false;
};
struct PresentationFacts {
    SlotFact selected_slot;
    bool erase_confirmation=false;
    unsigned class_index=0;
    // Initial authored layer is uppercase. This is deliberately separate from
    // source isCaps=false (SWF36820); first shift toggles to true and stays upper.
    bool upper_keyboard_visible=true;
    int difficulty=0,unlocked_difficulty=0;
    std::string entered_name;
    // Actual pointer capture supplied by input owner. Empty means unpressed.
    std::string pressed_button_path;
    // Actual save-owner field projections; no demo name/level/date values.
    std::vector<SourceText> profile_text;
};
struct PresentationState {
    std::string menu_name;
    std::vector<std::pair<std::string,std::string>> timeline_labels;
    std::vector<std::string> hidden_paths;
    std::vector<SourceText> text;
    bool draw_main_scene=false,draw_class_scene=false,draw_saved_avatar=false;
};
// Paths use dots (the source AS spelling). Renderer may normalize them to
// authored placement slash paths. Labels must resolve in the actual timeline.
PresentationState presentation(std::string_view menu_name,const PresentationFacts&);
// The same save projection feeds Main and StartGame, whose authored SWF text
// receiver roots differ. Preserve values while rebasing only that known root.
void rebase_profile_text_paths(std::vector<SourceText>&,std::string_view menu_name);
struct Services {
    std::function<bool(const std::string& name,const std::string& playable_class,int& slot,std::string&)> create_save;
    std::function<bool(int slot,int player,std::string&)> assign_save;
    std::function<bool(int difficulty,std::string&)> start_game;
    // Generic Load route: receives the exact fact selected by the visible
    // profile arrows. Native services may omit this and retain their own path.
    std::function<bool(const SlotFact&,std::string&)> load_selected_profile;
    // Source Main-menu profile navigation. The host resolves each exact slot
    // path; selection never invents a neighboring filename in the UI layer.
    std::function<bool(int slot,SlotFact&,std::string&)> inspect_slot;
    std::function<bool(int current_slot,int direction,SlotFact&,std::string&)> select_slot;
    // Called only after the explicit authored accept button. Implementations
    // should preserve recoverability and never overwrite another profile.
    std::function<bool(const SlotFact& exact_selected_profile,std::string&)> remove_selected_slot;
    std::function<void(const SlotFact&)> selected_profile_changed;
    // Read-only projection from this exact selected file, used to replace the
    // SWF's static Warrior placeholder fields. No default profile is supplied.
    std::function<bool(const SlotFact&,std::vector<SourceText>&,std::string&)> project_selected_profile;
    // Optional synchronous visual owner. Accept proposed stack before committing
    // it; owner must report a real failure. Pure navigation tests omit this.
    std::function<bool(const std::vector<std::string>& previous,const std::vector<std::string>& next,std::string&)> present_stack;
};
bool valid_authored_name(std::string_view raw) noexcept;
const char* main_menu_error_symbol(int source_event) noexcept;
const char* playable_class(unsigned index) noexcept;
class Navigator {
    Services services_;
    std::vector<std::string> stack_{"menu_MainMenu"};
    Mode mode_ = Mode::offline_single_player;
    CreationStage creation_ = CreationStage::editing;
    std::string name_,class_ = "KnightPlayerBase",message_;
    int slot_ = -1;
    SlotFact selected_slot_{};
    bool erase_confirmation_{};
    int created_slot_ = -1;
    bool start_delivered_{};
    bool change(std::vector<std::string>,std::string&);
public:
    explicit Navigator(Services services = {}):services_(std::move(services)){}
    const std::vector<std::string>& stack()const noexcept{return stack_;}
    std::string_view top()const noexcept{return stack_.empty()?std::string_view{}:std::string_view(stack_.back());}
    const std::string& player_name()const noexcept{return name_;}
    const std::string& player_class()const noexcept{return class_;}
    const std::string& message_symbol()const noexcept{return message_;}
    int current_slot()const noexcept{return slot_;}
    const SlotFact& selected_slot_fact()const noexcept{return selected_slot_;}
    bool erase_confirmation()const noexcept{return erase_confirmation_;}
    Mode mode()const noexcept{return mode_;}
    CreationStage creation_stage()const noexcept{return creation_;}
    bool start_delivered()const noexcept{return start_delivered_;}
    void set_selected_slot(SlotFact fact) noexcept { slot_=fact.id;selected_slot_=std::move(fact); }
    bool push(std::string_view,std::string&);
    // Named-pop removes that state; pop_above preserves the target state.
    bool pop(std::string_view name,std::string&);
    bool back(std::string&);
    bool pop_above(std::string_view,std::string&);
    bool pop_all(std::string&);
    bool go_to_main_menu(int source_event,std::string&);
    bool single_player(SlotFact,std::string&);
    bool select_adjacent_slot(int direction,std::string&);
    bool begin_remove_selected(std::string&);
    bool resolve_remove_selected(bool accept,std::string&);
    bool accept_name(std::string raw,std::string&);
    bool select_class(unsigned index,std::string&);
    // Preserves successful create/assign prefixes on failure; retry never creates
    // a second profile. This desktop policy is explicit, not inferred APK retry.
    bool confirm_class(std::string&);
    bool start_game(int difficulty,std::string&);
    bool multiplayer(std::string&);
    // Native AS values must be converted by the actual gameswf bridge. These
    // arguments are already to_xstring results; no JSON/object coercion here.
    bool navigation_callback(std::string_view callback,const std::vector<std::string>& converted_args,std::string&);
};
}
