#include "swf_viewport_connection.hpp"
#include "swf_frame_connection.hpp"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_function.h"
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
using namespace dh2::ui;
namespace {
std::uint32_t word(const unsigned char* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
bool equal(const void* a,const void* b,std::size_t n){auto*x=static_cast<const unsigned char*>(a);auto*y=static_cast<const unsigned char*>(b);for(std::size_t i=0;i<n;i+=4){if(word(x+i)==word(y+i))continue;float p,q;std::memcpy(&p,x+i,4);std::memcpy(&q,y+i,4);if(!(std::isnan(p)&&std::isnan(q)))return false;}return true;}
void rectangle(gameswf::rect& r,const float v[4]){r.m_x_min=v[0];r.m_x_max=v[1];r.m_y_min=v[2];r.m_y_max=v[3];}
// Only direct-core fixture ownership plumbing differs from the frozen test.
struct Core {
    std::shared_ptr<SwfInputHistory> history=std::make_shared<SwfInputHistory>();
    SwfFrameConnection frames;
    gameswf::gc_ptr<gameswf::player> player;
    gameswf::gc_ptr<gameswf::movie_def_impl> definition;
    gameswf::gc_ptr<gameswf::root> root;
    unsigned* destroyed;
    explicit Core(unsigned* d):destroyed(d){player=new gameswf::player;std::string source_error;if(!history->bind(player.get_ptr(),source_error)||!frames.bind(player.get_ptr(),history,source_error))throw std::runtime_error(source_error);
        definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);
        const float authored[4]={0,9600,0,6400};rectangle(definition->m_frame_size,authored);definition->set_frame_count(1);definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);
        root=definition->create_root();}
    ~Core(){root->get_root_movie()->m_members.clear();root->get_root_movie()->m_proto=nullptr;
        player->get_global()->m_members.clear();player->get_global()->m_proto=nullptr;player->clear_heap();
        definition->m_instance=nullptr;player->m_current_root=nullptr;root=nullptr;definition=nullptr;player=nullptr;++*destroyed;}
};
struct Driver {
    std::int32_t facts[4]{};unsigned queries=0;std::vector<unsigned> calls;bool fail=false;
    static bool orientation(void* p,std::int32_t& value,std::string& error){auto&d=*static_cast<Driver*>(p);d.calls.push_back(1);if(d.fail){error="actual driver fixture unavailable";return false;}value=d.facts[d.queries++?1:0];return true;}
    static bool dimensions(void* p,std::int32_t& w,std::int32_t& h,std::string&){auto&d=*static_cast<Driver*>(p);d.calls.push_back(2);w=d.facts[2];h=d.facts[3];return true;}
    SwfViewportDriver services(){return {this,orientation,dimensions};}
};
gameswf::as_object* viewport(Core& c){gameswf::as_value v;c.player->get_global()->get_member("Viewport",&v);return v.to_object();}
std::vector<std::array<float,4>>* observed_publications=nullptr;
void observe(const gameswf::fn_call& call){*call.result=call.arg(2);auto* object=call.arg(2).to_object();
    if(!object||object->m_members.size()!=4)throw std::runtime_error("AS publication shape changed");
    std::array<float,4> rect{};const char*keys[4]={"xMin","yMin","xMax","yMax"};
    for(unsigned i=0;i<4;++i){gameswf::as_value value;if(!object->get_member(keys[i],&value))throw std::runtime_error("AS member missing");rect[i]=static_cast<float>(value.to_number());}
    observed_publications->push_back(rect);}
struct Watch {SwfViewportConnection* connection{};bool nested=false,release=false;unsigned calls=0;bool survived=false;unsigned* destroyed{};};
Watch* active_watch=nullptr;
void changed(const gameswf::fn_call& call){auto& w=*active_watch;++w.calls;*call.result=call.arg(2);
    if(w.nested){w.nested=false;std::int32_t b[4]={7,9,640,480};std::string error;if(!w.connection->set_bounds(b,0,error))throw std::runtime_error(error);}
    if(w.release){w.release=false;w.connection->release();w.survived=*w.destroyed==0;}}
}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream file(argv[1],std::ios::binary);std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)),{});
    if(data.size()<8||word(data.data())!=0x31505756)return 2;
    unsigned destroyed=0;auto core=std::make_shared<Core>(&destroyed);SwfViewportConnection connection;std::string error;std::size_t at=8;
    gameswf::as_value observer_function(observe);core->player->get_global()->m_watch=new stringi_hash<gameswf::as_object::as_watch>;
    gameswf::as_object::as_watch observer_slot;observer_slot.m_func=observer_function.to_function();core->player->get_global()->m_watch->set("Viewport",observer_slot);
    unsigned publications=0,driver_calls=0,comparisons=word(data.data()+4),guards=0;
    for(unsigned i=0;i<comparisons;++i){if(at+12>data.size())return 2;auto il=word(data.data()+at),ol=word(data.data()+at+4),ec=word(data.data()+at+8);at+=12;
        if(il!=152||ol!=120||at+il+ol+ec*40>data.size())return 2;
        auto*raw=data.data()+at;at+=il;auto*expected=data.data()+at;at+=ol;auto*events=data.data()+at;at+=ec*40;
        ViewportState64 seed;FlashCamera40 camera;std::int32_t xywh[4];float point[4]{};Driver driver;
        std::memcpy(&seed,raw+4,64);std::memcpy(&camera,raw+68,40);std::memcpy(xywh,raw+108,16);std::memcpy(point,raw+124,8);std::memcpy(driver.facts,raw+132,16);
        rectangle(core->definition->m_frame_size,seed.movie_rect);
        core->root->m_player=seed.player_receiver?core->player.get_ptr():nullptr;
        std::vector<std::array<float,4>> observed;observed_publications=&observed;
        gameswf::gc_ptr<gameswf::as_object> before=viewport(*core);
        if(!connection.bind({core,core->root.get_ptr()},seed,driver.services(),error)){std::cerr<<error;return 1;}
        bool ok=false;switch(word(raw)){case 0:ok=connection.set_bounds(xywh,static_cast<std::int32_t>(word(raw+148)),error);break;
        case 1:ok=connection.set_viewport(xywh,error);break;case 2:ok=connection.screen_to_logical(point,error);break;
        case 3:ok=connection.logical_to_screen(point,error);break;case 4:ok=connection.camera_update(camera,error);break;
        case 5:ok=connection.display_rectangle(point,error);break;default:return 2;}
        auto actual_state=connection.state();actual_state.player_receiver=actual_state.player_receiver?1:0;
        std::array<unsigned char,120> actual{};std::memcpy(actual.data(),&actual_state,64);std::memcpy(actual.data()+64,&camera,40);std::memcpy(actual.data()+104,point,16);
        if(!ok||!equal(actual.data(),expected,120)){std::cerr<<"connection record "<<i<<" mismatch "<<error;return 1;}
        std::vector<unsigned> expected_driver;const unsigned char* last=nullptr;unsigned pub_index=0;
        for(unsigned k=0;k<ec;++k){auto*event=events+k*40;auto op=word(event);if(op==1||op==2)expected_driver.push_back(op);if(op==3){last=event+24;++publications;
            if(pub_index>=observed.size()||!equal(observed[pub_index++].data(),last,16)){std::cerr<<"ordered actual AS publication mismatch "<<i;return 1;}}}
        if(pub_index!=observed.size())return 1;
        if(driver.calls!=expected_driver){std::cerr<<"driver order mismatch "<<i;return 1;}driver_calls+=driver.calls.size();
        auto* current=viewport(*core);
        if(last){if(!current||current==before.get_ptr()||current->m_members.size()!=4)return 1;
            const char*keys[4]={"xMin","yMin","xMax","yMax"};for(unsigned k=0;k<4;++k){gameswf::as_value value;float actual;
                if(!current->get_member(keys[k],&value))return 1;actual=static_cast<float>(value.to_number());if(!equal(&actual,last+k*4,4)){std::cerr<<"actual AS rectangle mismatch "<<i;return 1;}}}
        else if(current!=before.get_ptr())return 1;
        if(core->root->m_viewport_x0!=actual_state.viewport[0]||core->root->m_viewport_y0!=actual_state.viewport[1]||
           core->root->m_viewport_width!=actual_state.viewport[2]||core->root->m_viewport_height!=actual_state.viewport[3]||
           !equal(&core->root->m_pixel_scale,&actual_state.pixel_scale,4))return 1;
    }
    if(at!=data.size())return 2;
    Driver driver;ViewportState64 seed{{0,9600,0,6400},{0,0,480,320},{0,0,480,320},1,0,1};
    rectangle(core->definition->m_frame_size,seed.movie_rect);core->root->m_player=core->player.get_ptr();
    if(!connection.bind({core,core->root.get_ptr()},seed,driver.services(),error))return 1;
    auto saved=connection.state();auto bad=seed;bad.movie_rect[0]=1;
    if(connection.bind({core,core->root.get_ptr()},bad,driver.services(),error)||std::memcmp(&saved,&connection.state(),64))return 1;++guards;
    if(connection.bind({{},core->root.get_ptr()},seed,driver.services(),error)||std::memcmp(&saved,&connection.state(),64))return 1;++guards;
    std::int32_t dimensions[4]={7,9,640,480};driver.fail=true;
    if(connection.set_viewport(dimensions,error)||std::memcmp(connection.state().viewport,dimensions,16))return 1;++guards;
    if(connection.notify_mouse_state(INT32_MIN,INT32_MAX,-7,error)==false||core->root->m_mouse_x!=INT32_MIN||core->root->m_mouse_y!=INT32_MAX||core->root->m_mouse_buttons!=-7)return 1;++guards;
    driver.fail=false;Watch watch{&connection,true,false,0,false,&destroyed};active_watch=&watch;
    gameswf::as_value function(changed);auto* global=core->player->get_global();
    gameswf::as_object::as_watch slot;slot.m_func=function.to_function();global->m_watch->set("Viewport",slot);
    std::int32_t b[4]={3,5,854,480};if(!connection.set_bounds(b,0,error)||watch.calls!=2||connection.state().bounds[0]!=7||connection.state().bounds[1]!=9)return 1;++guards;
    auto weak=std::weak_ptr<Core>(core);core.reset();if(weak.expired()||destroyed)return 1;++guards;
    watch.release=true;b[0]=1;if(!connection.set_bounds(b,0,error)||!watch.survived||!weak.expired()||destroyed!=1)return 1;++guards;
    active_watch=nullptr;
    if(connection.set_viewport(dimensions,error)||connection.notify_mouse_state(0,0,0,error))return 1;++guards;
    std::cout<<"{\"validation\":\"PASS\",\"original_gold_comparisons\":"<<comparisons<<",\"source_publications\":"<<publications
        <<",\"actual_AS_final_rectangle_checks\":"<<publications<<",\"ordered_driver_calls\":"<<driver_calls<<",\"ownership_and_failure_guards\":"<<guards
        <<",\"actual_Gameswf_core_executed\":true,\"original_full_frame_parity\":false,\"live_Android\":false,\"mismatches\":0}\n";
}
