#include "semantic_input.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace dh::foundation::platform_input;
namespace {
void require(bool ok,const char* text) {if(!ok)throw std::runtime_error(text);}
Surface surface() {
    return {
        [](Point p)->Hit {
            if(p.x<0)return {};
            if(p.x<100)return {Control::joystick,0};
            if(p.x<200)return {Control::attack,0};
            if(p.x<300)return {Control::profile,0};
            if(p.x<400)return {Control::skill2,0};
            if(p.x<500)return {Control::menu_item,17};
            return {Control::world,0};
        },
        [](Point origin,Point current) { return dh::foundation::InputMove2D{(current.x-origin.x)/10,(origin.y-current.y)/10}; }
    };
}
}
int main() {try {
    SemanticInput input;input.set_surface(surface());
    input.key('D',true);auto movement=input.take_frame();
    require(movement.actions.move2D.x==1&&movement.actions.run,"PC movement did not run by default");
    input.key(0x10,true);movement=input.take_frame();
    require(movement.actions.move2D.x==1&&!movement.actions.run,"Held Shift did not select walking");
    input.key(0x10,false);movement=input.take_frame();
    require(movement.actions.run,"Releasing Shift did not restore running");
    input.key('D',false);movement=input.take_frame();
    require(!movement.actions.run,"Idle input requested running");
    Bindings legacyBindings;legacyBindings.default_run=false;SemanticInput legacy(legacyBindings);
    legacy.key('D',true);require(!legacy.take_frame().actions.run,"Legacy walk preference lost");
    legacy.key(0x10,true);require(legacy.take_frame().actions.run,"Legacy Shift run preference lost");
    input.key(0x20,true);auto f=input.take_frame();
    require(f.attack.pressed&&f.attack.held&&!f.attack.released&&f.actions.attack,"Space attack press lost");
    input.key(0x20,true);f=input.take_frame();
    require(!f.attack.pressed&&f.attack.held,"OS key repeat fabricated new combo press");
    input.key(0x20,false);f=input.take_frame();
    require(f.attack.released&&!f.attack.held&&!f.actions.attack,"Space release lost");
    input.key(0x20,true);input.key(0x20,false);f=input.take_frame();
    require(f.attack.pressed&&f.attack.released&&!f.attack.held,"Subframe tap edges lost");
    input.pointer(11,PointerPhase::down,{150,0});f=input.take_frame();require(f.attack.pressed&&f.attack.held,"Touch press lost");
    input.pointer(12,PointerPhase::down,{150,0});f=input.take_frame();require(!f.attack.pressed&&f.attack.held,"Second finger fabricated attack edge");
    input.pointer(11,PointerPhase::up,{700,0});f=input.take_frame();require(!f.attack.released&&f.attack.held,"One release cleared another held finger");
    input.pointer(12,PointerPhase::move,{-100,0});f=input.take_frame();require(f.attack.held,"Captured attack lost on drag outside");
    input.pointer(12,PointerPhase::cancel,{0,0});f=input.take_frame();require(f.attack.released&&!f.attack.held,"Cancel left attack stuck");
    input.pointer(3,PointerPhase::down,{10,0});input.pointer(3,PointerPhase::move,{10,-20});
    input.pointer(4,PointerPhase::down,{150,0});f=input.take_frame();
    require(f.actions.move2D.y==1&&f.attack.held,"Multitouch joystick+attack failed");
    input.set_menu_open(true);f=input.take_frame();
    require(f.attack.released&&!f.attack.held&&f.actions.move2D.y==0&&input.captured_pointers()==0,"Menu did not consume gameplay captures");
    input.pointer(4,PointerPhase::up,{150,0});f=input.take_frame();require(!f.attack.pressed&&!f.target_point_requested,"Late touch leaked through modal menu");
    input.pointer(4,PointerPhase::down,{150,0});f=input.take_frame();require(!f.attack.held,"Modal hit-test returned underlying attack");
    input.pointer(4,PointerPhase::up,{150,0});
    input.pointer(5,PointerPhase::down,{450,0});input.pointer(5,PointerPhase::up,{451,0});f=input.take_frame();
    require(f.clicks.size()==1&&f.clicks[0].hit.item==17,"Authored menu item click missing");
    input.pointer(5,PointerPhase::down,{450,0});input.pointer(5,PointerPhase::up,{550,0});f=input.take_frame();require(f.clicks.empty(),"Release outside activated menu item");
    input.key(0x1b,true);f=input.take_frame();require(f.menu_back,"Menu Escape did not become back");input.key(0x1b,false);
    input.key(0x20,true);input.set_menu_open(false);f=input.take_frame();require(!f.attack.held,"Held key resumed after closing menu without new press");
    input.key(0x20,false);input.key(0x20,true);input.key('D',true);input.key(0x10,true);input.key(0x09,true);input.key('E',true);f=input.take_frame(true);
    require(f.attack.pressed&&f.actions.attack&&f.actions.move2D.x==0&&!f.actions.run&&!f.actions.targetSelect&&!f.actions.interact,"Controller mask precleared source attack or failed movement gate");
    input.lose_focus();f=input.take_frame();require(f.attack.released&&!f.actions.attack&&!f.actions.run,"Focus loss left held control");
    input.key('2',true);f=input.take_frame();require(f.skills[1].pressed&&f.skills[1].held,"Skill slot2 mapping failed");
    input.set_level_input_enabled(false);f=input.take_frame();require(f.skills[1].released&&!f.skills[1].held,"Level disabled did not release skill control");
    input.set_level_input_enabled(true);f=input.take_frame();require(!f.skills[1].held,"Level reenable revived held skill");input.key('2',false);
    input.pointer(9,PointerPhase::down,{250,0});input.pointer(9,PointerPhase::up,{250,0});f=input.take_frame();require(f.profile_pressed,"Profile receiver click lost");
    input.pointer(9,PointerPhase::down,{600,0});input.pointer(9,PointerPhase::up,{600,0});f=input.take_frame();require(f.target_point_requested&&f.target_point.x==600,"World touch target lost");
    input.pointer(10,PointerPhase::down,{std::numeric_limits<float>::quiet_NaN(),0});require(input.captured_pointers()==0,"Nonfinite pointer captured");
    input.pointer(10,PointerPhase::down,{150,0});input.take_frame();
    input.pointer(10,PointerPhase::cancel,{std::numeric_limits<float>::quiet_NaN(),0});f=input.take_frame();
    require(f.attack.released&&input.captured_pointers()==0,"Coordinate-free Android cancel left a captured attack");
    input.key('D',true);input.key('W',true);f=input.take_frame();require(std::abs(std::hypot(f.actions.move2D.x,f.actions.move2D.y)-1)<1e-5,"Diagonal intent not normalized");
    SemanticInput pauseInput;Surface pauseSurface;
    pauseSurface.hit=[](Point p){return p.x<50?Hit{Control::pause,1}:Hit{};};
    pauseInput.set_surface(std::move(pauseSurface));
    pauseInput.pointer(1,PointerPhase::down,{20,20});pauseInput.pointer(1,PointerPhase::up,{20,20});
    require(pauseInput.take_frame().pause_pressed,"Source HUD pause release lost");
    require(!pauseInput.take_frame().pause_pressed,"HUD pause release replayed");
    pauseInput.pointer(1,PointerPhase::down,{20,20});pauseInput.lose_focus();pauseInput.pointer(1,PointerPhase::up,{20,20});
    require(!pauseInput.take_frame().pause_pressed,"Canceled HUD pause capture activated");
    pauseInput.key(0x1b,true);require(pauseInput.take_frame().pause_pressed,"Gameplay Escape did not open pause");
    pauseInput.key(0x1b,true);require(!pauseInput.take_frame().pause_pressed,"Held Escape retriggered pause");
    pauseInput.key(0x1b,false);pauseInput.set_menu_open(true);pauseInput.key(0x1b,true);
    const auto pauseBack=pauseInput.take_frame();require(pauseBack.menu_back&&!pauseBack.pause_pressed,"Modal Escape did not remain Back");
    std::cout<<"semantic_input_tests PASS: edges, multi-source hold, touch capture, modal consumption, controller gates, focus/level cancel, authored hits, pause routing\n";
    return 0;
}catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;} }
