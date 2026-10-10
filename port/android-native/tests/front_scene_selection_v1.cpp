#include "front_scene_selection_v1.hpp"

using dh2::android_ui::FrontSceneSelectionV1;
using dh2::android_ui::front_scene_selection_v1;

static_assert(front_scene_selection_v1(true, false, false) == FrontSceneSelectionV1::menu_swamp,
              "ordinary menu keeps its authored 3D swamp");
static_assert(front_scene_selection_v1(true, false, true) == FrontSceneSelectionV1::authored_swf,
              "name entry must show its authored SWF background over a retained MainMenu");
static_assert(front_scene_selection_v1(false, false, true) == FrontSceneSelectionV1::authored_swf,
              "name entry is still authored when no slot-2 MainMenu clip is visible");
static_assert(front_scene_selection_v1(true, true, false) == FrontSceneSelectionV1::class_selection,
              "class selection owns its authored 3D scene");
static_assert(front_scene_selection_v1(true, true, true) == FrontSceneSelectionV1::class_selection,
              "SelectClass takes precedence during a stacked Name-to-Class transition");

int main() { return 0; }
