#include "front_loading_render_policy_v1.hpp"

using dh2::android_ui::FrontLoadingRenderPhaseV1;
using dh2::android_ui::front_loading_render_phase_v1;
using dh2::android_ui::front_movie_slot_visible_v1;
using dh2::android_ui::front_loading_clip_visible_v1;

constexpr auto menu = front_loading_render_phase_v1(false, false);
static_assert(menu == FrontLoadingRenderPhaseV1::front_menu);
static_assert(front_movie_slot_visible_v1(menu, 0));
static_assert(front_movie_slot_visible_v1(menu, 2));

constexpr auto loading = front_loading_render_phase_v1(true, false);
static_assert(loading == FrontLoadingRenderPhaseV1::source_loading);
static_assert(front_movie_slot_visible_v1(loading, 0)); // retained shared loading SWF
static_assert(!front_movie_slot_visible_v1(loading, 2)); // no stale 3D menu preview
static_assert(!front_movie_slot_visible_v1(loading, 1));
static_assert(!front_movie_slot_visible_v1(loading, 3));
static_assert(front_loading_clip_visible_v1(loading, 0, "_root.menu_Loading"));
static_assert(front_loading_clip_visible_v1(loading, 0, "_root.menu_Loading.loading_anim"));
static_assert(!front_loading_clip_visible_v1(loading, 0, "_root.menu_MainMenu"));
static_assert(!front_loading_clip_visible_v1(loading, 2, "_root.menu_Loading"));

constexpr auto gameplay = front_loading_render_phase_v1(true, true);
static_assert(gameplay == FrontLoadingRenderPhaseV1::gameplay);
static_assert(front_movie_slot_visible_v1(gameplay, 0));
static_assert(front_movie_slot_visible_v1(gameplay, 2));

int main() { return 0; }
