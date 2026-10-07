#include "swf_menu_save_slots.hpp"
#include "swf_frame_connection.hpp"
#include "swf_input_history.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_object.h"
#include <fstream>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <unistd.h>
using namespace dh2::ui;
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
std::uint32_t word(std::ifstream& file){unsigned char b[4];file.read(reinterpret_cast<char*>(b),4);check(bool(file),"truncated fixture");return b[0]|(b[1]<<8)|(b[2]<<16)|(std::uint32_t(b[3])<<24);}
struct Fixture{
    unsigned mask=0,loads=0,slot=0;bool used=false,reject_load=false;int difficulty=0,fail_query=-1;
    std::vector<unsigned> queries;
    static bool exists(void* p,unsigned slot,bool& used,std::string& error){auto& f=*static_cast<Fixture*>(p);f.queries.push_back(slot);if(int(slot)==f.fail_query){error="TEST-query-rejected";return false;}used=(f.mask>>slot)&1;return true;}
    static bool load(void* p,unsigned slot,bool used,int difficulty,SwfFrontSaveSlotDetailsV1& out,std::string& error){
        auto& f=*static_cast<Fixture*>(p);++f.loads;f.slot=slot;f.used=used;f.difficulty=difficulty;
        if(f.reject_load){error="TEST-load-rejected";return false;}
        out.slot_id=99;out.in_use=!used;out.player_name="Prince";out.player_class="Knight";
        out.player_level=7;out.string_class_level="Knight LVL 7";out.player_location="Crypt";out.last_save="11/23     17:08";
        out.current_act=2;out.difficulty=difficulty==-1?1:difficulty;out.difficulty_unlocked=2;error.clear();return true;
    }
    void reset(){queries.clear();loads=0;fail_query=-1;reject_load=false;}
};
double number(gameswf::as_object* object,const char* key){gameswf::as_value v;check(object->get_member(key,&v)&&v.is_number(),"missing numeric slot member");return v.to_number();}
int main(int argc,char** argv){try{
    check(argc==2,"usage selection fixture");std::ifstream file(argv[1],std::ios::binary);check(word(file)==0x31534c53,"fixture magic");auto count=word(file);
    gameswf::gc_ptr<gameswf::player> player=new gameswf::player;
    auto history=std::make_shared<SwfInputHistory>();SwfFrameConnection frames;std::string error;
    check(history->bind(player.get_ptr(),error)&&frames.bind(player.get_ptr(),history,error),"AS owners unavailable");
    gameswf::gc_ptr<gameswf::movie_def_impl> definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);
    definition->set_frame_count(1);definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);
    gameswf::gc_ptr<gameswf::root> root=definition->create_root();
    gameswf::as_environment env(player.get_ptr());Fixture fixture;
    SwfFrontSaveSlotServicesV1 services{&fixture,Fixture::exists,Fixture::load};
    gameswf::as_value result(55);const gameswf::as_value call_this;
    auto invoke=[&](double index,const gameswf::as_value& receiver,int argc,double difficulty=2){
        result.set_double(55);
        if(argc==4)env.push(999);
        if(argc>=3)env.push(difficulty);
        env.push(receiver);env.push(index);
        gameswf::fn_call call(&result,call_this,&env,argc,env.get_top_index());
        bool ok=swf_front_save_slot_details(call,services,error);env.drop(argc);return ok;
    };
    unsigned checks=0;
    for(unsigned i=0;i<count;++i){
        fixture.mask=word(file);auto index=word(file),slot=word(file),used=word(file);
        for(int argc:{2,3,4}){
            fixture.reset();gameswf::gc_ptr<gameswf::as_object> object=new gameswf::as_object(player.get_ptr());
            check(invoke(index,gameswf::as_value(object.get_ptr()),argc),"slot service call failed");
            check(fixture.queries==std::vector<unsigned>({0,1,2,3})&&fixture.loads==1,"source query/load order");
            check(fixture.slot==slot&&fixture.used==bool(used)&&fixture.difficulty==(argc==3?2:-1),"original slot selection or optional argument");
            check(result.to_object()==object.get_ptr()&&number(object.get_ptr(),"SlotID")==slot,"AS receiver or selected slot overwritten");
            gameswf::as_value in_use;check(object->get_member("InUse",&in_use)&&in_use.is_bool()&&in_use.to_bool()==bool(used),"AS occupancy from provider wrongly trusted");
            check(number(object.get_ptr(),"PlayerLVL")==double(used?7:-1),"empty/occupied level");++checks;
        }
    }
    fixture.reset();fixture.mask=10;
    check(invoke(0,gameswf::as_value(),2)&&fixture.loads==1&&result.to_number()==55,"null receiver skipped source reads or changed result");
    fixture.reset();fixture.fail_query=2;
    check(!invoke(0,gameswf::as_value(),2)&&fixture.queries==std::vector<unsigned>({0,1,2})&&fixture.loads==0&&error=="TEST-query-rejected"&&result.to_number()==55,"query rejection prefix");
    fixture.reset();fixture.reject_load=true;
    check(!invoke(0,gameswf::as_value(),2)&&fixture.loads==1&&error=="TEST-load-rejected"&&result.to_number()==55,"load rejection preserved");
    fixture.reset();
    for(double index:{-1.,4.,std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity()})
        check(!invoke(index,gameswf::as_value(),2)&&fixture.loads==0&&fixture.queries.empty(),"unsafe display index accepted");
    check(!invoke(0,gameswf::as_value(),3,3)&&fixture.queries.empty(),"unsafe difficulty accepted");
    services.exists=nullptr;check(!invoke(0,gameswf::as_value(),2)&&error=="Save-slot profile services unavailable","missing provider accepted");
    char temporary[]="/data/local/tmp/dh2-save-slot-files-v66-XXXXXX";
    check(mkdtemp(temporary)!=nullptr,"isolated filesystem fixture creation failed");
    const std::string directory=temporary;
    auto file_call=[&](unsigned index,gameswf::as_object* object){
        env.push(object);env.push(static_cast<int>(index));result.set_double(55);
        gameswf::fn_call call(&result,call_this,&env,2,env.get_top_index());
        bool ok=swf_front_save_slot_details(call,directory,error);env.drop(2);return ok;
    };
    gameswf::gc_ptr<gameswf::as_object> disk_object=new gameswf::as_object(player.get_ptr());
    for(unsigned index=0;index<4;++index)
        check(file_call(index,disk_object.get_ptr())&&number(disk_object.get_ptr(),"SlotID")==0,"absent file inventory");
    const auto backup=directory+"/dh2_002.savegame.bak";
    {std::ofstream output(backup);output<<"fixture";check(bool(output),"backup fixture write failed");}
    check(!file_call(0,disk_object.get_ptr())&&error=="Existing campaign save requires PlayerSavegame loading"&&result.to_number()==55,"backup treated as empty");
    check(file_call(1,disk_object.get_ptr())&&number(disk_object.get_ptr(),"SlotID")==0,"first empty after backup");
    const auto base=directory+"/dh2_000.savegame";
    {std::ofstream output(base);output<<"fixture";check(bool(output),"base fixture write failed");}
    check(!file_call(0,disk_object.get_ptr())&&!file_call(1,disk_object.get_ptr()),"occupied files accepted without loader");
    check(file_call(2,disk_object.get_ptr())&&number(disk_object.get_ptr(),"SlotID")==1,"gapped first free slot");
    check(std::remove(base.c_str())==0&&std::remove(backup.c_str())==0&&rmdir(directory.c_str())==0,"fixture cleanup failed");
    std::printf("PASS %u original occupancy/index cases / %u real AS service calls / optional nargs semantics, provider prefix failures, null receiver, unsafe bounds and isolated base/backup/gap file inventory\n",count,checks);
}catch(const std::exception& error){std::fprintf(stderr,"FAIL %s\n",error.what());return 1;}}
