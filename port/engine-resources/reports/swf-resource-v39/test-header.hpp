#pragma once
#include "authored_shader_program.hpp"
#include "../engine-ui/swf_movie.hpp"
#include "swf_vertex_cache_v36.hpp"
#include "native_resource_budget_v38.hpp"
#include "/mnt/c/Users/adamc/Desktop/workspace/DH_sc/port/engine-resources/retained_bytes_v39.hpp"
#include <array>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace dh2::android_ui {
// GLES2 drawing owner for the native GameSWF command stream. This is a modern
// backend; upstream timeline execution and recovered Gameloft hooks are separate.
class SwfGpu {
public:
    struct StatsV36 {std::uint64_t primitives{},scratch_growths{},stream_uploads{},stream_bytes{},displays{},empty_displays{},materializations{};ui::SwfVertexCacheStatsV36 cache;};
    StatsV36 stats_v36()const noexcept{return {primitives_v36_,scratch_growths_v36_,stream_uploads_v36_,stream_bytes_v36_,displays_v36_,empty_displays_v36_,materializations_v36_,vertex_cache_v36_.stats()};}
    struct StatsV37 {std::uint64_t transparent_noops{},error_queries{},capability_queries{},commands{};};
    StatsV37 stats_v37()const noexcept{return {transparent_noops_v37_,error_queries_v37_,capability_queries_v37_,command_serial_v37_};}
    void initialize(AAssetManager*,resources::ResourceScopeV37); // new context: abandon old GL names
    bool image(std::int32_t width,std::int32_t height,unsigned channels,
               const std::uint8_t* pixels,std::size_t pitch,ui::SwfTexture&,std::string&);
    bool draw(const ui::SwfDraw&,std::string&);
    bool scene_pane(const float twips[4],void*,bool (*)(void*,int,int,std::string&),std::string&);
    void set_grid_fit(bool value) noexcept {grid_fit_=value;}
    bool stencil(const float bounds[4],std::uint8_t pattern,bool&,std::string&);
    void abort() noexcept; // restore destination after a failed command/provider
    std::shared_ptr<resources::ContextResourceBudgetV37> resource_budget_lease_v39()const noexcept{return budget_v39_;}
    bool reset_images(std::string&); // only after all dependent movies/fonts die
public:
    struct Texture {GLuint name{};int width{},height{};unsigned channels{};resources::RetainedBytesV39 pixels;resources::ResourceTokenV37 gpu_budget_v39;std::uint64_t generation_v39{};GLenum wrap=GL_CLAMP_TO_EDGE;};
    ui::SwfVertexCacheV36 vertex_cache_v36_;
    std::vector<float> vertices_v36_;
    std::uint64_t primitives_v36_{},scratch_growths_v36_{},stream_uploads_v36_{},stream_bytes_v36_{};
    std::uint64_t displays_v36_{},empty_displays_v36_{},materializations_v36_{};
    Program normal_{},premultiplied_{};
    std::unordered_map<std::uintptr_t,Texture> textures_;
    std::uintptr_t next_identity_=1,white_{};
    GLuint buffer_{};
    GLuint target_{},query_target_{},target_color_{},query_color_{},stencil_buffer_{};
    GLint destination_{};
    int target_width_{},target_height_{};
    float bounds_[4]{};
    int viewport_[4]{},mask_level_=0;
    bool frame_=false,submitting_mask_=false,begin_pending_=false,materialized_v36_=false;
    bool grid_fit_=false;
    GLint max_texture_size_v37_{},stencil_bits_v37_{};
    GLfloat line_width_range_v37_[2]{};
    std::uint64_t transparent_noops_v37_{},error_queries_v37_{},capability_queries_v37_{},command_serial_v37_{},checked_serial_v37_{};
    void barrier_v37(const char* action);
    std::shared_ptr<resources::ContextResourceBudgetV37> budget_v39_;
    resources::ResourceScopeV37 scope_v39_=resources::ResourceScopeV37::other;
    std::array<resources::ResourceTokenV37,5> target_budget_v39_{};
    std::uint64_t target_generation_v39_{};
    void release_charge_v39(resources::ResourceTokenV37& token){if(token){std::string error;if(!budget_v39_||!budget_v39_->release(token,error))throw std::runtime_error(error);}}
    void release_texture_v39(Texture&,bool);
    void release_targets_v39(bool);
    void upload(Texture&);
    void primitive(const ui::SwfDraw&,const ui::SwfFill&,GLenum,float line_width=0);
    void mask_rectangle();
    void target(int width,int height);
    void materialize_v36();
};
}
