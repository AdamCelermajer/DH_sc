#pragma once
#include "../../hud_geometry.hpp"
#include <optional>
namespace dh::foundation::skill_ui {
enum class HitKind { select,assign,train };
struct HitZone {HitKind kind;int position;std::string path;std::vector<HudGeometryVertex> triangles;};
struct IconState {std::string label;unsigned source_frame;std::vector<HudGeometryBatch> batches;};
enum class IconKind { tree,drag,slot };
struct IconPlacement {unsigned class_frame;int position;IconKind kind;std::array<float,6> matrix;std::string path;};
const std::vector<IconState>& original_skill_icon_states();
const std::vector<IconPlacement>& original_skill_icon_placements();
const std::vector<HitZone>& original_skill_hit_zones(unsigned class_frame);
// Original labels and transform, no atlas-crop approximation. Empty source
// SkillIcon selects authored undefined state as original AS20cfe does.
bool append_original_skill_icon(unsigned class_frame,int position,const std::string& icon,HudGeometry&,std::string&);
bool append_original_skill_slot_icon(unsigned class_frame,int slot,const std::string& icon,HudGeometry&,std::string&);
std::optional<HitZone> original_skill_hit(unsigned class_frame,float source_x,float source_y);
}
