#include "swf_menu_save_slots.hpp"
#include "swf_frame_connection.hpp"
#include "swf_input_history.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_object.h"
#include <fstream>
#include <cstring>
#include <cstdio>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
struct Input{
    std::ifstream file;
    explicit Input(const char* path):file(path,std::ios::binary){check(bool(file),"fixture missing");}
    std::uint32_t word(){unsigned char b[4];file.read(reinterpret_cast<char*>(b),4);check(bool(file),"fixture truncated");return b[0]|(b[1]<<8)|(b[2]<<16)|(std::uint32_t(b[3])<<24);}
    std::int32_t integer(){auto value=word();std::int32_t out;std::memcpy(&out,&value,4);return out;}
    std::string text(){auto count=word();check(count<65536,"fixture string too long");std::string out(count,'\0');file.read(out.data(),count);check(bool(file),"fixture string truncated");return out;}
    double number(){std::uint64_t bits=word();bits|=std::uint64_t(word())<<32;double out;std::memcpy(&out,&bits,8);return out;}
};
struct Object:gameswf::as_object{
    struct Write{std::string name;gameswf::as_value value;};
    std::vector<Write> writes;
    bool reject=false;
    explicit Object(gameswf::player* player):as_object(player){}
    bool set_member(const tu_stringi& key,const gameswf::as_value& value)override{
        writes.push_back({key.c_str(),value});
        return reject?false:as_object::set_member(key,value);
    }
};
int main(int argc,char** argv){try{
    check(argc==2,"usage fixture");Input input(argv[1]);check(input.word()==0x31505353,"fixture magic");auto count=input.word();
    gameswf::gc_ptr<gameswf::player> player=new gameswf::player;
    auto history=std::make_shared<SwfInputHistory>();SwfFrameConnection frames;std::string error;
    check(history->bind(player.get_ptr(),error)&&frames.bind(player.get_ptr(),history,error),"AS owners unavailable");
    unsigned writes=0;
    for(unsigned i=0;i<count;++i){
        SwfFrontSaveSlotDetailsV1 details;
        details.slot_id=input.integer();details.in_use=input.word()!=0;details.player_level=input.integer();
        details.current_act=input.integer();details.difficulty=input.integer();details.difficulty_unlocked=input.integer();
        const bool reject=input.word()!=0;
        details.player_name=input.text();details.player_class=input.text();details.string_class_level=input.text();
        details.player_location=input.text();details.last_save=input.text();
        gameswf::gc_ptr<Object> object=new Object(player.get_ptr());object->reject=reject;
        swf_front_write_save_slot_details_v1(object.get_ptr(),details);
        const auto expected=input.word();check(expected==11&&object->writes.size()==expected,"setter count");
        for(unsigned j=0;j<expected;++j){
            const auto key=input.text();const auto type=input.word();const auto& actual=object->writes[j];
            check(key==actual.name,"setter order");
            if(type==1)check(actual.value.is_bool()&&actual.value.to_bool()==bool(input.word()),"boolean type/value");
            else if(type==2)check(actual.value.is_number()&&actual.value.to_number()==input.number(),"number type/value");
            else if(type==4)check(actual.value.is_string()&&std::string(actual.value.to_string())==input.text(),"string type/value");
            else throw std::runtime_error("fixture type");
            ++writes;
        }
        swf_front_write_save_slot_details_v1(nullptr,details);
    }
    check(input.file.peek()==std::char_traits<char>::eof(),"fixture trailing bytes");
    std::printf("PASS %u original AS property cases / %u typed setters / null receiver and rejected setters\n",count,writes);
}catch(const std::exception& error){std::fprintf(stderr,"FAIL %s\n",error.what());return 1;}}
