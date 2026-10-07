#pragma once
#include "authored_shader_program.hpp"
#include "../engine-ui/swf_movie.hpp"
#include "swf_vertex_cache_v36.hpp"
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
    void initialize(AAssetManager*); // new context: abandon old GL names
    bool image(std::int32_t width,std::int32_t height,unsigned channels,
               const std::uint8_t* pixels,std::size_t pitch,ui::SwfTexture&,std::string&);
    bool draw(const ui::SwfDraw&,std::string&);
    bool scene_pane(const float twips[4],void*,bool (*)(void*,int,int,std::string&),std::string&);
    void set_grid_fit(bool value) noexcept {grid_fit_=value;}
    bool stencil(const float bounds[4],std::uint8_t pattern,bool&,std::string&);
    void abort() noexcept; // restore destination after a failed command/provider
    bool reset_images(std::string&); // only after all dependent movies/fonts die
private:
    struct Texture {GLuint name{};int width{},height{};unsigned channels{};std::vector<std::uint8_t> pixels;GLenum wrap=GL_CLAMP_TO_EDGE;};
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
    void upload(Texture&);
    void primitive(const ui::SwfDraw&,const ui::SwfFill&,GLenum,float line_width=0);
    void mask_rectangle();
    void target(int width,int height);
    void materialize_v36();
};
}
