#pragma once
#include <cstdint>
namespace dh2::ui {
// One process renderer field corresponding to the actual linked handler's
// byte1f4. RenderFX.SetWireFrame selects virtual40=7d3e70, exactly a byte store.
// Native context rebuilds must not replay a source renderer/default store.
class SwfRendererWireframeV62 {
 std::uint8_t byte1f4_{};bool produced_{};
public:
 void source_set(bool value)noexcept{byte1f4_=value;produced_=true;}
 bool draw_wireframe()const noexcept{return produced_&&byte1f4_!=0;}
};
inline SwfRendererWireframeV62& swf_renderer_wireframe_v62(){static SwfRendererWireframeV62 owner;return owner;}
}
