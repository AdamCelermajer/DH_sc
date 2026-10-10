#include "../renderer.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <iostream>
#include <stdexcept>
#include <array>
using namespace dh::foundation;
static void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct HiddenContext {
    HWND window{}; HDC device{}; HGLRC context{};
    HiddenContext(){
        WNDCLASSA wc{}; wc.style=CS_OWNDC; wc.lpfnWndProc=DefWindowProcA;
        wc.hInstance=GetModuleHandleA(nullptr);wc.lpszClassName="DHViewportTestHidden";
        check(RegisterClassA(&wc)!=0,"RegisterClass");
        window=CreateWindowA(wc.lpszClassName,"Hidden viewport test",WS_POPUP,0,0,128,128,nullptr,nullptr,wc.hInstance,nullptr);
        check(window!=nullptr,"CreateWindow");device=GetDC(window);
        PIXELFORMATDESCRIPTOR p{};p.nSize=sizeof(p);p.nVersion=1;
        p.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL;p.iPixelType=PFD_TYPE_RGBA;
        p.cColorBits=24;p.cDepthBits=24;p.iLayerType=PFD_MAIN_PLANE;
        const int format=ChoosePixelFormat(device,&p);check(format!=0,"ChoosePixelFormat");
        check(SetPixelFormat(device,format,&p)!=FALSE,"SetPixelFormat");
        context=wglCreateContext(device);check(context!=nullptr,"CreateContext");
        check(wglMakeCurrent(device,context)!=FALSE,"MakeCurrent");
    }
    ~HiddenContext(){wglMakeCurrent(nullptr,nullptr);if(context)wglDeleteContext(context);
        if(device)ReleaseDC(window,device);if(window)DestroyWindow(window);}
};
struct State {
    std::array<GLint,4> viewport{},clip{}; GLint mode{};
    std::array<GLfloat,16> projection{},model{}; GLboolean scissor{},blend{},vertex{};
    State(){glGetIntegerv(GL_VIEWPORT,viewport.data());glGetIntegerv(GL_SCISSOR_BOX,clip.data());
        glGetIntegerv(GL_MATRIX_MODE,&mode);glGetFloatv(GL_PROJECTION_MATRIX,projection.data());
        glGetFloatv(GL_MODELVIEW_MATRIX,model.data());scissor=glIsEnabled(GL_SCISSOR_TEST);
        blend=glIsEnabled(GL_BLEND);vertex=glIsEnabled(GL_VERTEX_ARRAY);}
    bool same(const State& s)const{return viewport==s.viewport&&clip==s.clip&&mode==s.mode&&
        projection==s.projection&&model==s.model&&scissor==s.scissor&&blend==s.blend&&vertex==s.vertex;}
};
int main(){try{
    HiddenContext context;Renderer renderer;check(renderer.initialize(128,128),"Initialize");
    Camera camera;renderer.beginFrame(camera);
    glDisable(GL_SCISSOR_TEST);glClearColor(0,0,1,1);glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);glScissor(40,40,20,20);glEnable(GL_BLEND);
    glEnableClientState(GL_VERTEX_ARRAY);glMatrixMode(GL_TEXTURE);
    const State before;
    check(renderer.withViewport(20,20,60,60,camera,[&]{
        GLint box[4];glGetIntegerv(GL_VIEWPORT,box);check(box[0]==20&&box[2]==60,"Viewport");
        glGetIntegerv(GL_SCISSOR_BOX,box);check(box[0]==40&&box[1]==40&&box[2]==20&&box[3]==20,"Inherited scissor intersection");
        // Entry preserves menu color while clearing only clipped depth.
        std::array<unsigned char,3> pixel{};glReadPixels(45,45,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel.data());
        check(pixel[2]>250&&pixel[0]==0,"Pane entry cleared menu color");
        glClearColor(1,0,0,1);glClear(GL_COLOR_BUFFER_BIT);
        const State outer;
        check(renderer.withViewport(50,50,30,30,camera,[]{
            GLint nested[4];glGetIntegerv(GL_SCISSOR_BOX,nested);
            check(nested[0]==50&&nested[1]==50&&nested[2]==10&&nested[3]==10,"Nested intersection");
        }),"Nested viewport admission");
        check(outer.same(State{}),"Nested viewport restoration");
        glDisable(GL_BLEND);glDisableClientState(GL_VERTEX_ARRAY);
    }),"Viewport admission");
    check(before.same(State{}),"Normal state restoration");
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    std::array<unsigned char,128*128*3> pixels{};
    glReadPixels(0,0,128,128,GL_RGB,GL_UNSIGNED_BYTE,pixels.data());
    for(int y=0;y<128;++y)for(int x=0;x<128;++x){
        const auto offset=static_cast<std::size_t>((y*128+x)*3);
        const bool inside=x>=40&&x<60&&y>=40&&y<60;
        check(pixels[offset+(inside?0:2)]>250&&pixels[offset+(inside?2:0)]==0,"Pixel escaped clipped pane");
    }
    bool caught=false;try{renderer.withViewport(20,20,60,60,camera,[]{
        glDisable(GL_SCISSOR_TEST);glMatrixMode(GL_MODELVIEW);glLoadIdentity();throw std::runtime_error("Expected");
    });}catch(const std::runtime_error&){caught=true;}
    check(caught&&before.same(State{}),"Exception state restoration");
    bool called=false;check(!renderer.withViewport(0,0,0,10,camera,[&]{called=true;}),"Invalid rectangle accepted");
    check(!renderer.withViewport(100,100,10,10,camera,[&]{called=true;}),"Disjoint scissor accepted");
    check(!called,"Rejected callback invoked");
    check(glGetError()==GL_NO_ERROR,"GL error");
    std::cout<<"PASS hidden WGL viewport, inherited clipping, unchanged menu color, pixel containment, normal/throw restoration, invalid rejection\n";
    return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
