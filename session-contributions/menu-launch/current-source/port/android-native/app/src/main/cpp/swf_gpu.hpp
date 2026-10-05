#pragma once
#include "authored_shader_program.hpp"
#include "../engine-ui/swf_movie.hpp"
#include <unordered_map>
#include <vector>

namespace dh2::android_ui {
// GLES2 drawing owner for the native GameSWF command stream. This is a modern
// backend; upstream timeline execution and recovered Gameloft hooks are separate.
class SwfGpu {
public:
    void initialize(AAssetManager*); // new context: abandon old GL names
    bool image(std::int32_t width,std::int32_t height,unsigned channels,
               const std::uint8_t* pixels,std::size_t pitch,ui::SwfTexture&,std::string&);
    bool draw(const ui::SwfDraw&,std::string&);
    bool scene_pane(const float twips[4],void*,bool (*)(void*,int,int,std::string&),std::string&);
    bool stencil(const float bounds[4],std::uint8_t pattern,bool&,std::string&);
    void abort() noexcept; // restore destination after a failed command/provider
    bool reset_images(std::string&); // only after all dependent movies/fonts die
private:
    struct Texture {GLuint name{};int width{},height{};unsigned channels{};std::vector<std::uint8_t> pixels;};
    Program normal_{},premultiplied_{};
    std::unordered_map<std::uintptr_t,Texture> textures_;
    std::uintptr_t next_identity_=1,white_{};
    GLuint buffer_{};
    GLuint target_{},query_target_{},target_color_{},query_color_{},stencil_buffer_{};
    GLint destination_{};
    int target_width_{},target_height_{};
    float bounds_[4]{};
    int viewport_[4]{},mask_level_=0;
    bool frame_=false,submitting_mask_=false,begin_pending_=false;
    void upload(Texture&);
    void primitive(const ui::SwfDraw&,const ui::SwfFill&,GLenum);
    void mask_rectangle();
    void target(int width,int height);
};
}
