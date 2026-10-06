#include "localization_parse_ex_v1.hpp"
#include <cmath>
#include <cstdio>
#include <limits>
#include <cstring>
#include <utility>
namespace dh2::ui {
namespace {
bool safe(const std::string& s){return s.size()<=1024*1024&&s.find('\0')==std::string::npos;}
bool integer(float value,std::int32_t& result){
    if(!std::isfinite(value)||static_cast<double>(value)<-2147483648.0||static_cast<double>(value)>=2147483648.0)return false;
    result=static_cast<std::int32_t>(value);return true;
}
std::string grouped(std::int32_t value,const LocalizationNumberStyleV1& style){
    char buffer[32]{};
    if(value<style.group_at)std::snprintf(buffer,sizeof(buffer),"%d",value);
    else{
        const auto millions=value/1000000,thousands=(value%1000000)/1000,units=value%1000;
        if(millions)std::snprintf(buffer,sizeof(buffer),"%d%s%03d%s%03d",millions,style.thousands.c_str(),thousands,style.thousands.c_str(),units);
        else if(thousands)std::snprintf(buffer,sizeof(buffer),"%d%s%03d",thousands,style.thousands.c_str(),units);
        else std::snprintf(buffer,sizeof(buffer),"%d",units);
    }
    return buffer;
}
}
bool localization_parse_ex_v1(const std::string& input,const std::vector<LocalizationArgumentV1>& args,
    const LocalizationNumberStyleV1& style,bool spacing,const LocalizationParseExServicesV1& services,
    std::string& output,bool& changed,std::string& error){
    auto fail=[&](const char* message){error=message;return false;};
    if(!safe(input)||!safe(style.decimal)||!safe(style.thousands)||args.size()>65536)return fail("Parsed localization input outside bounds");
    for(const auto& arg:args)if(arg.has_text&&!safe(arg.text))return fail("Parsed localization argument outside bounds");
    std::string next;std::size_t argument=0;bool escape=false,flag=false;
    for(unsigned char c:input){
        if(!escape){if(c=='^')escape=true;else if(c=='|'){next+='\x11';flag=true;}else next+=char(c);continue;}
        escape=false;
        if(c=='#'||c=='*'||c=='^')next+=char(c);
        else if(c=='n')next+='\n';
        else if(c=='t'||c=='v'){
            std::string value;
            if(c=='v'){
                if(!services.version)return fail("Parsed version service unavailable");
                if(!services.version(services.context,value,error))return false;
                if(!safe(value)||value.size()>=10)return fail("Parsed version exceeds source buffer");
            }else{
                if(!services.title)return fail("Parsed title service unavailable");
                if(!services.title(services.context,value,error))return false;
                if(!safe(value)||value.size()>=32)return fail("Parsed title exceeds source buffer");
            }
            next+=value;
        }else if(c=='s'){
            if(argument<args.size()){const auto& value=args[argument++];if(value.has_text)next+=value.text;}
        }else if(std::strchr("dfghikmp",c)){
            if(argument>=args.size())continue;
            float value=args[argument++].number;
            if(c=='g'||c=='k')value=value/1000.f;
            else if(c=='h'||c=='p')value=value*100.f;
            else if(c=='i')value=value/5.f;
            else if(c=='m'){value=value/100.f;value=value+(value<0.f?-.05f:.05f);}
            const bool decimal=std::strchr("fghim",c)!=nullptr;
            if(decimal&&c!='m')value=value+(value<0.f?-.005f:.005f);
            double whole{};float fraction{};
            if(decimal){fraction=static_cast<float>(std::modf(static_cast<double>(value),&whole));fraction=fraction-(value<0.f?-.005f:.005f);}
            std::int32_t digits{};
            if(!integer(decimal?static_cast<float>(whole):value,digits))return fail("Parsed numeric argument outside source signed conversion bounds");
            next+=grouped(digits,style);
            if(decimal&&std::fabs(fraction)>=.0001f){
                char buffer[16]{};std::snprintf(buffer,sizeof(buffer),c=='m'?"%.1f":"%.2f",static_cast<double>(std::fabs(fraction)));
                next+=style.decimal;next+=buffer+2;
            }
        }
        if(next.size()>1024*1024)return fail("Parsed localization output outside bounds");
    }
    output=localization_utf_text_v1(next,spacing);changed=flag;error.clear();return true;
}
}
