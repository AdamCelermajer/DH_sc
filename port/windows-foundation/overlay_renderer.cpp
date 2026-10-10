#include "overlay_renderer.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include "frame_perf.hpp"

namespace dh::foundation {

void OverlayRenderer::begin(int width, int height) {
    if (active_) end();
    glGetIntegerv(GL_MATRIX_MODE,&previousMatrixMode_);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,std::max(1,width),std::max(1,height),0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    active_ = true;
}

void OverlayRenderer::drawSprite(const OverlaySprite& sprite) {
    if (!active_ || sprite.width <= 0 || sprite.height <= 0 || !sprite.texture) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,sprite.texture);
    glColor4fv(sprite.color.data());
    {auto& c=perf::FramePerf::get().counters();++c.immediateBatches;c.immediateVerts+=4;++c.calls;c.triangles+=2;++c.stateChanges;++c.texBinds;} // B066 stats
    glBegin(GL_QUADS);
    glTexCoord2f(sprite.u0,sprite.v0); glVertex2f(sprite.x,sprite.y);
    glTexCoord2f(sprite.u1,sprite.v0); glVertex2f(sprite.x+sprite.width,sprite.y);
    glTexCoord2f(sprite.u1,sprite.v1); glVertex2f(sprite.x+sprite.width,sprite.y+sprite.height);
    glTexCoord2f(sprite.u0,sprite.v1); glVertex2f(sprite.x,sprite.y+sprite.height);
    glEnd();
}

void OverlayRenderer::drawProgress(const OverlaySprite& sprite, float fraction, ProgressDirection direction) {
    if (!std::isfinite(fraction)) return;
    fraction = std::clamp(fraction,0.0f,1.0f);
    OverlaySprite clipped = sprite;
    if (direction == ProgressDirection::LeftToRight) {
        clipped.width *= fraction;
        clipped.u1 = sprite.u0+(sprite.u1-sprite.u0)*fraction;
    } else {
        clipped.y += sprite.height*(1.0f-fraction);
        clipped.height *= fraction;
        clipped.v0 = sprite.v1-(sprite.v1-sprite.v0)*fraction;
    }
    drawSprite(clipped);
}

bool OverlayRenderer::drawTriangles(const OverlayTriangleVertex* vertices, std::size_t count,
                                   std::uint32_t texture, const std::array<float,4>& color) {
    // A UI submission is deliberately bounded; larger sources should be split
    // by the asset boundary into independent contours or panels.
    constexpr std::size_t maximumVertices = 3*65536;
    if (!active_ || !vertices || count == 0 || count % 3 != 0 || count > maximumVertices)
        return false;
    for (const float channel : color) if (!std::isfinite(channel)) return false;
    for (std::size_t i=0; i<count; ++i) {
        const auto& vertex = vertices[i];
        if (!std::isfinite(vertex.x) || !std::isfinite(vertex.y)
            || !std::isfinite(vertex.u) || !std::isfinite(vertex.v)) return false;
    }
    if(texture){glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,texture);}
    else glDisable(GL_TEXTURE_2D);
    glColor4fv(color.data());
    {auto& c=perf::FramePerf::get().counters();++c.immediateBatches;c.immediateVerts+=count;++c.calls;c.triangles+=count/3;++c.stateChanges;++c.texBinds;} // B066 stats
    glBegin(GL_TRIANGLES);
    for (std::size_t i=0; i<count; ++i) {
        glTexCoord2f(vertices[i].u,vertices[i].v);
        glVertex2f(vertices[i].x,vertices[i].y);
    }
    glEnd();
    return true;
}

void OverlayRenderer::end() {
    if (!active_) return;
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopClientAttrib();
    glPopAttrib();
    glMatrixMode(previousMatrixMode_);
    active_ = false;
}

} // namespace dh::foundation
