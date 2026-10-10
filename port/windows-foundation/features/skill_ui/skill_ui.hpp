#pragma once
#include "../../../game-data/skill_tables.hpp"
#include "../character_menu/character_menu.hpp"
#include <functional>
#include <optional>
#include <utility>
namespace dh::foundation::skill_ui {
// Original native UI skill IDs are positions in Character's SkillList, not
// global SkillTable IDs. The service owns progression and saved slot bindings.
struct Progress { int level=0; bool available=false,can_increment=false,equippable=false; };
struct Services {
 std::function<bool(int& list_id,int& points,std::string&)> character;
 std::function<bool(int position,Progress&,std::string&)> progress;
 std::function<bool(std::vector<int>& positions,std::string&)> slots;
 std::function<bool(int position,bool source_flag,std::string&)> increment;
 // NativeSkillsTrainSkill(true, position, player) is a non-mutating source
 // probe: provider success and an ordinary rejected result are distinct.
 std::function<bool(int position,bool& accepted,std::string&)> probe_increment;
 std::function<bool(int slot,int position,std::string&)> assign;
 std::function<bool(int position,const std::string& field,std::string& value,std::string&)> text;
 // One native NativeGetSkillDetails call can fill all selected skill fields;
 // this avoids repeating source skill-info/temp-sheet work for each label.
 std::function<bool(int position,std::vector<std::pair<std::string,std::string>>&,std::string&)> selected_texts;
 // Exact SWF frame/icon resolver; required for drawing dynamically authored art.
 std::function<bool(int position,const std::string& source_icon,character_menu::Frame&,std::string&)> icon;
 // Source class frame follows original playerClassAsStr base-class branch.
 std::function<bool(unsigned&,std::string&)> source_class_frame;
};
struct Row { int position=0,table_id=0;std::string source_name,source_icon;int required_level=0;Progress progress; };
struct View { int list_id=0,points=0;std::vector<Row> rows;std::vector<int> slots; };
// Reads the class row's authored SkillTree for diagnostics. The active list
// remains Character::GetCharSkillListId from the live character; properties
// may override the class-row value, so this metadata must not gate rendering.
bool source_skill_tree_metadata(const dh2::data::CharacterTable&,int class_row,
 dh2::data::SkillTables::Borrow,int& authored_list,std::string&);
// Reproduces the original MenuBase::FS_GetPlayerClass2 / playerClassAsStr
// branch for its three supported base-class save rows. Other rows remain
// unsupported exactly as in source; no specialization family is inferred.
bool original_class_frame_for_row(int source_class_row,unsigned& frame,std::string&);
class Presenter {
 dh2::data::SkillTables::Borrow tables_;Services services_;std::optional<int> selected_,selected_list_;
public:
 Presenter(dh2::data::SkillTables::Borrow tables,Services services):tables_(std::move(tables)),services_(std::move(services)){}
 bool view(View&,std::string&)const;
 bool select(int position,std::string&);
 std::optional<int> selected()const noexcept{return selected_;}
 bool probe_training(bool& accepted,std::string&)const;
 bool train(bool source_flag,std::string&);
 bool assign(int slot,std::string&);
 // Appends only dynamic content; character_menu retains source frame ownership.
 bool append(character_menu::Frame&,std::string&)const;
 bool release(float source_x,float source_y,std::string&);
};
}

