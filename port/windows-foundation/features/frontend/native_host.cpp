#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include "native_host.hpp"
#include <utility>
namespace dh::foundation::frontend {
namespace {constexpr wchar_t property[]=L"DH2OriginalFrontendInputOwner";}
NativeHostInput::~NativeHostInput(){auto window=static_cast<HWND>(window_);if(window&&IsWindow(window)){SetWindowLongPtrW(window,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(previous_));RemovePropW(window,property);}}
bool NativeHostInput::attach_focused_window(std::string& error){
 if(window_){error="Native input host already attached";return false;}
 auto window=GetFocus();DWORD process{};if(!window||!GetWindowThreadProcessId(window,&process)||process!=GetCurrentProcessId()){error="Diagnostic focused window unavailable";return false;}
 if(!SetPropW(window,property,this)){error="Cannot retain diagnostic input owner";return false;}
 SetLastError(0);auto previous=SetWindowLongPtrW(window,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&NativeHostInput::procedure));
 if(!previous&&GetLastError()){RemovePropW(window,property);error="Cannot subclass diagnostic input window";return false;}
 window_=window;previous_=reinterpret_cast<void*>(previous);RegisterTouchWindow(window,0);error.clear();return true;
}
std::vector<HostEvent> NativeHostInput::take_events(){auto result=std::move(events_);events_.clear();return result;}
bool NativeHostInput::post_pointer(input::Point point,bool down){auto window=static_cast<HWND>(window_);return window&&PostMessageW(window,down?WM_LBUTTONDOWN:WM_LBUTTONUP,down?MK_LBUTTON:0,MAKELPARAM(int(point.x),int(point.y)));}
bool NativeHostInput::post_key(int key,bool down){auto window=static_cast<HWND>(window_);return window&&PostMessageW(window,down?WM_KEYDOWN:WM_KEYUP,WPARAM(key),0);}
bool NativeHostInput::post_ascii_text(const std::string&text){auto window=static_cast<HWND>(window_);if(!window)return false;for(unsigned char c:text)if(c>=128||!PostMessageW(window,WM_CHAR,c,0))return false;return true;}
std::intptr_t NativeHostInput::procedure(void* handle,unsigned message,std::uintptr_t wp,std::intptr_t lp){
 auto window=static_cast<HWND>(handle);auto* self=static_cast<NativeHostInput*>(GetPropW(window,property));if(!self)return DefWindowProcW(window,message,wp,lp);
 HostEvent event;bool enqueue=true;
 switch(message){
 case WM_KEYDOWN:case WM_KEYUP:event.kind=HostEvent::Kind::key;event.key=int(wp);event.down=message==WM_KEYDOWN;event.shift=(GetKeyState(VK_SHIFT)&0x8000)!=0;break;
 case WM_CHAR:{event.kind=HostEvent::Kind::text;wchar_t ch=wchar_t(wp);if(ch<32){enqueue=false;break;}char bytes[4]{};int n=WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,&ch,1,bytes,4,nullptr,nullptr);if(n>0)event.text.assign(bytes,n);else enqueue=false;break;}
 case WM_LBUTTONDOWN:case WM_LBUTTONUP:case WM_MOUSEMOVE:
  // WM_TOUCH is authoritative for promoted touch input; Windows also emits
  // mouse messages with this documented pointer signature.
  if((static_cast<std::uintptr_t>(GetMessageExtraInfo())&0xffffff00u)==0xff515700u){enqueue=false;break;}
  if(message==WM_MOUSEMOVE&&!(wp&MK_LBUTTON)){enqueue=false;break;}
  event.kind=HostEvent::Kind::pointer;event.pointer=-1;event.phase=message==WM_LBUTTONDOWN?input::PointerPhase::down:message==WM_LBUTTONUP?input::PointerPhase::up:input::PointerPhase::move;event.point={float(GET_X_LPARAM(lp)),float(GET_Y_LPARAM(lp))};break;
 case WM_TOUCH:{std::vector<TOUCHINPUT> touches(LOWORD(wp));if(GetTouchInputInfo(reinterpret_cast<HTOUCHINPUT>(lp),unsigned(touches.size()),touches.data(),sizeof(TOUCHINPUT)))for(const auto&t:touches){POINT p{TOUCH_COORD_TO_PIXEL(t.x),TOUCH_COORD_TO_PIXEL(t.y)};ScreenToClient(window,&p);HostEvent touch;touch.kind=HostEvent::Kind::pointer;touch.pointer=t.dwID;touch.point={float(p.x),float(p.y)};touch.phase=(t.dwFlags&TOUCHEVENTF_DOWN)?input::PointerPhase::down:(t.dwFlags&TOUCHEVENTF_UP)?input::PointerPhase::up:input::PointerPhase::move;self->events_.push_back(std::move(touch));}CloseTouchInputHandle(reinterpret_cast<HTOUCHINPUT>(lp));return 0;}
 case WM_KILLFOCUS:event.kind=HostEvent::Kind::focus_lost;break;
 case WM_CAPTURECHANGED:if(GetAsyncKeyState(VK_LBUTTON)&0x8000)event.kind=HostEvent::Kind::focus_lost;else enqueue=false;break;
 default:enqueue=false;break;
 }
 if(enqueue)self->events_.push_back(std::move(event));
 return CallWindowProcW(reinterpret_cast<WNDPROC>(self->previous_),window,message,wp,lp);
}
}
