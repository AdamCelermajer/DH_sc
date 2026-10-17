#include "../script_runtime.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

static unsigned checks=0,observers=0,required_calls=0;
static void check(bool ok,const char* why){++checks;if(!ok)throw std::runtime_error(why);}
static int load(dh2_script_vm* vm,const std::string& code){return dh2_script_vm_load(vm,code.data(),code.size(),"@compatibility");}
static int source_file(dh2_script_vm* vm,const std::string& code){return dh2_script_vm_load_source_file(vm,code.data(),code.size());}
static dh2_script_value global(dh2_script_vm* vm,const char* name){dh2_script_value value{};check(!dh2_script_vm_get_global(vm,name,&value),"global query failed");return value;}
static int missing_required(void*,const dh2_script_value*,unsigned,dh2_script_value*,unsigned,unsigned*,char*,size_t){
    ++required_calls;return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
static int ordinary_error(void*,const dh2_script_value*,unsigned,dh2_script_value*,unsigned,unsigned*,char* error,size_t capacity){
    std::snprintf(error,capacity,"ordinary argument error");return 1;
}
struct Snapshot {
    dh2_script_vm* vm=nullptr;
    dh2_script_first_return_v1 value{};
    std::string text;
    unsigned calls=0;
    int error=0;
    bool probe_busy=false;
    static int observe(void* c,const dh2_script_first_return_v1* value,char* error_text,size_t capacity) {
        auto& self=*static_cast<Snapshot*>(c);++self.calls;++observers;self.value=*value;
        self.text=value->text?std::string(value->text,value->text_bytes):std::string{};
        if(self.probe_busy) {
            dh2_script_value untouched;std::memset(&untouched,0xa5,sizeof(untouched));auto old=untouched;
            check(dh2_script_vm_get_global(self.vm,"changed",&untouched)==-1 && !std::memcmp(&old,&untouched,sizeof(old)),"observer reentry mutated output");
            check(dh2_script_vm_call_first_source_v1(self.vm,"empty",nullptr,0,observe,&self)==-1,"recursive observer entered VM");
            check(dh2_script_vm_load_source_file(self.vm,nullptr,0)==-1,"nested file load entered VM");
        }
        if(self.error){std::snprintf(error_text,capacity,"observer rejected");return self.error;}
        return 0;
    }
};
struct AllSnapshot {
    dh2_script_vm* vm=nullptr;
    std::vector<dh2_script_first_return_v1> values;
    std::vector<std::string> texts;
    unsigned calls=0;
    int error=0;
    bool empty_pointer=false,reentry_rejected=false;
    static int observe(void* raw,const dh2_script_first_return_v1* values,unsigned count,char* text,size_t capacity) {
        auto& self=*static_cast<AllSnapshot*>(raw);++self.calls;++observers;
        try {
            self.empty_pointer=!values;
            self.values.clear();self.texts.clear();self.texts.resize(count);
            for(unsigned i=0;i<count;++i){
                if(values[i].count!=count)return 1;
                self.values.push_back(values[i]);
                if(values[i].text)self.texts[i].assign(values[i].text,values[i].text_bytes);
            }
            for(unsigned i=0;i<count;++i)if(self.values[i].text)self.values[i].text=self.texts[i].c_str();
            dh2_script_value guard{};guard.reserved=0xa5;
            self.reentry_rejected=dh2_script_vm_get_global(self.vm,"changed",&guard)==-1 && guard.reserved==0xa5 &&
                dh2_script_vm_call_all_source_v1(self.vm,"empty",nullptr,0,observe,&self)==-1;
            if(self.error){std::snprintf(text,capacity,"all observer rejected");return self.error;}
            return 0;
        }catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
    }
};
static int all(dh2_script_vm* vm,const char* name,AllSnapshot& result,
               const dh2_script_value* args=nullptr,unsigned count=0) {
    result.vm=vm;return dh2_script_vm_call_all_source_v1(vm,name,args,count,AllSnapshot::observe,&result);
}
static int indexed(dh2_script_vm* vm,const char* name,unsigned index,Snapshot& result,
                   const dh2_script_value* args=nullptr,unsigned count=0) {
    result.vm=vm;return dh2_script_vm_call_indexed_source_v3(vm,name,args,count,index,Snapshot::observe,&result);
}
static int identity(void*,const dh2_script_value*,unsigned,dh2_script_value* out,unsigned capacity,unsigned* count,char*,size_t) {
    if(!capacity){return 1;}out[0]={};out[0].type=DH2_SCRIPT_IDENTITY;out[0].identity=0x1234567887654321ull;*count=1;return 0;
}
int main(){try {
    auto* vm=dh2_script_vm_create(8*1024*1024);check(vm,"create failed");const auto original_vm=vm;
    check(!dh2_script_vm_bind_source_values(vm,"MissingRequired",missing_required,nullptr),"required bind failed");
    check(!dh2_script_vm_bind(vm,"OrdinaryError",ordinary_error,nullptr),"ordinary bind failed");
    check(!dh2_script_vm_bind(vm,"Identity",identity,nullptr),"identity bind failed");
    check(dh2_script_vm_required_failure_epoch(vm)==0,"fresh epoch nonzero");
    check(source_file(vm,"")==0,"empty source file rejected");
    check(source_file(vm,"--"+std::string(3073,'x')+"\nreader_crossed=1024")==0 &&
          global(vm,"reader_crossed").number==1024,"1024-byte reader boundary lost bytes");
    check(source_file(vm,"file_ran=1; function empty() end; function one() return 3 end")==0,"source file load failed");
    check(vm==original_vm && global(vm,"file_ran").number==1,"source extension changed VM identity/state");
    check(source_file(vm,"function broken(")==3 && std::strstr(dh2_script_vm_error(vm),"loadFile()"),"source syntax status/name lost");
    check(source_file(vm,"changed=17; error('source runtime')")==2 && global(vm,"changed").number==17,"runtime effects/status lost");
    check(source_file(vm,"error(123.5)")==2 && std::strstr(dh2_script_vm_error(vm),"123.5"),"numeric error not converted");
    for(const char* object:{"{}","nil","true","function()end","coroutine.create(function()end)","newproxy()"}) {
        check(source_file(vm,std::string("error(")+object+")")==-4,"unsupported source error object accepted");
    }
    check(load(vm,"error({})")==-2,"legacy load status changed");
    check(source_file(vm,"error('a'..string.char(0)..'b',0)")==2 && std::string(dh2_script_vm_error(vm))=="a","source error first-NUL semantics lost");
    const auto before=dh2_script_vm_required_failure_epoch(vm);
    check(source_file(vm,"pcall(MissingRequired); required_caught=1")==0 &&
          dh2_script_vm_required_failure_epoch(vm)==before+1,"file caught required failure not observable");
    check(source_file(vm,"MissingRequired()")==2 && dh2_script_vm_required_failure_epoch(vm)==before+2 &&
          std::strstr(dh2_script_vm_error(vm),"game service rejected"),"default required diagnostic/epoch lost");
    check(source_file(vm,"OrdinaryError()")==2 && dh2_script_vm_required_failure_epoch(vm)==before+2,"ordinary error advanced epoch");
    check(load(vm,
        "projection_order=''; captured=Identity(); "
        "function many() local a=setmetatable({}, {__index=function(_,k) projection_order=projection_order..'A'; return captured end}); "
        "local b=setmetatable({}, {__index=function(_,k) projection_order=projection_order..'B'; captured=nil; return nil end}); "
        "return a,'a'..string.char(0)..'b',true,16777217,-1/(1/0),Identity(),function()end,coroutine.create(function()end),newproxy(),b end; "
        "function caught() pcall(MissingRequired); return true end; function raw_required() MissingRequired() end; "
        "function ordinary_caught() pcall(OrdinaryError); return 7 end; function argument_error() OrdinaryError() end; "
        "function nil_error() error(nil) end; function number_error() error(321,0) end; "
        "function bad_projection() projection_started=1; return 'kept',setmetatable({},{__index=function()error('projection rejected')end}) end; "
        "function echo(...) return ... end; function lots() local t={} for i=1,96 do t[i]=i end return unpack(t) end; "
        "function numeric() return 0/0,1/0,-1/0 end")==0,"fixture load failed");
    for(unsigned index=0;index<11;++index) {
        check(source_file(vm,"projection_order=''; captured=Identity()")==0,"projection reset failed");
        Snapshot s;s.probe_busy=index==0;check(!indexed(vm,"many",index,s) && s.calls==1 && s.value.count==10,"indexed source call failed");
        check(std::string(global(vm,"projection_order").text)=="AB","not all returns projected before observer");
        switch(index) {
        case 0:check(s.value.type==7 && s.value.identity==0x1234567887654321ull,"table identity narrowed or reread");break;
        case 1:check(s.value.type==4 && s.text=="a" && s.value.text_bytes==1,"source string not first-NUL");break;
        case 2:check(s.value.type==1 && s.value.boolean==1 && s.value.number==1,"boolean projection differs");break;
        case 3:check(s.value.type==3 && s.value.number==16777216.f,"float32 result differs");break;
        case 4:check(s.value.type==3 && s.value.number==0 && std::signbit(s.value.number),"signed zero lost");break;
        case 5:check(s.value.type==2 && s.value.identity==0x1234567887654321ull,"lightuserdata narrowed");break;
        case 9:check(s.value.type==7 && !s.value.identity,"null table identity differs");break;
        default:check(s.value.type==0,"unsupported/absent value not nil");break;
        }
    }
    Snapshot absent;check(!indexed(vm,"one",0xffffffffu,absent) && absent.value.type==0 && absent.value.count==1,"large absent index failed");
    Snapshot first;first.vm=vm;check(!dh2_script_vm_call_first_source_v1(vm,"one",nullptr,0,Snapshot::observe,&first) && first.value.number==3 && first.value.count==1,"first source alias failed");
    Snapshot empty;check(!indexed(vm,"empty",0,empty) && empty.value.count==0 && empty.value.type==0,"empty returns differ");
    Snapshot lots;check(!indexed(vm,"lots",95,lots) && lots.value.count==96 && lots.value.number==96,"fixed return cap introduced");
    std::vector<dh2_script_value> args(32);for(unsigned i=0;i<args.size();++i){args[i].type=3;args[i].number=float(i);}
    Snapshot echoed;check(!indexed(vm,"echo",31,echoed,args.data(),unsigned(args.size())) && echoed.value.count==32 && echoed.value.number==31,"new call imposed legacy16arg cap");
    for(unsigned i=0;i<3;++i){Snapshot n;check(!indexed(vm,"numeric",i,n) && n.value.type==3,"numeric projection failed");check(i==0?std::isnan(n.value.number):std::isinf(n.value.number),"NaN/infinity changed");}
    Snapshot caught;const auto caught_epoch=dh2_script_vm_required_failure_epoch(vm);
    check(indexed(vm,"caught",0,caught)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS && caught.calls==1 && caught.value.boolean==1 &&
          dh2_script_vm_required_failure_epoch(vm)==caught_epoch+1,"pcall-caught required failure synthesized success");
    Snapshot raw_required;check(indexed(vm,"raw_required",0,raw_required)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS &&
        !raw_required.calls,"uncaught required failure reached observer");
    Snapshot ordinary_caught;auto ordinary_epoch=dh2_script_vm_required_failure_epoch(vm);
    check(!indexed(vm,"ordinary_caught",0,ordinary_caught) && ordinary_caught.value.number==7 &&
          dh2_script_vm_required_failure_epoch(vm)==ordinary_epoch,"caught ordinary error marked required");
    Snapshot bad_projection;check(indexed(vm,"bad_projection",0,bad_projection)==2 && !bad_projection.calls &&
          global(vm,"projection_started").number==1,"later projection error observed earlier result or rolled back effects");
    Snapshot error;check(indexed(vm,"argument_error",0,error)==2 && !error.calls,"ordinary source error status differs");
    check(indexed(vm,"nil_error",0,error)==-4 && !error.calls,"nonstring Call error not unsupported");
    check(indexed(vm,"number_error",0,error)==2 && !error.calls && std::string(dh2_script_vm_error(vm))=="321","Call numeric error differs");
    Snapshot rejected;rejected.error=1;check(indexed(vm,"one",0,rejected)==2 && rejected.calls==1,"ordinary observer failure lost");
    Snapshot required;required.error=DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;auto epoch=dh2_script_vm_required_failure_epoch(vm);
    check(indexed(vm,"one",0,required)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS && dh2_script_vm_required_failure_epoch(vm)==epoch+1,"observer required marker lost");
    // A skill check returns usable AND active from one source Lua call. Full
    // ReturnValues must not replay side effects to observe the second value.
    check(!source_file(vm,"pair_calls=0; function pair() pair_calls=pair_calls+1; return false,true end"),"pair fixture failed");
    AllSnapshot pair;check(!all(vm,"pair",pair) && pair.calls==1 && pair.values.size()==2 &&
        pair.values[0].type==1 && !pair.values[0].boolean && pair.values[1].type==1 && pair.values[1].boolean &&
        global(vm,"pair_calls").number==1 && pair.reentry_rejected,"all return observer replayed pair or entered busy VM");
    check(!source_file(vm,"projection_order=''; captured=Identity()"),"all projection reset failed");
    AllSnapshot many;check(!all(vm,"many",many) && many.calls==1 && many.values.size()==10 &&
        many.values[0].type==7 && many.values[0].identity==0x1234567887654321ull &&
        many.values[1].type==4 && many.texts[1]=="a" && many.values[1].text_bytes==1 &&
        many.values[3].number==16777216.f && std::signbit(many.values[4].number) &&
        many.values[5].identity==0x1234567887654321ull && many.values[6].type==0 &&
        many.values[7].type==0 && many.values[8].type==0 && many.values[9].type==7 &&
        !many.values[9].identity && std::string(global(vm,"projection_order").text)=="AB",
        "all projection lost order, tags, strings, float32 or native identities");
    AllSnapshot zero;check(!all(vm,"empty",zero) && zero.calls==1 && zero.empty_pointer && zero.values.empty(),"all zero returns differs");
    AllSnapshot large;check(!all(vm,"lots",large) && large.values.size()==96 && large.values[95].number==96,"all return cap introduced");
    AllSnapshot all_echo;check(!all(vm,"echo",all_echo,args.data(),unsigned(args.size())) && all_echo.values.size()==32 && all_echo.values.back().number==31,"all argument cap introduced");
    AllSnapshot broken;check(all(vm,"bad_projection",broken)==2 && !broken.calls && global(vm,"projection_started").number==1,"all observer saw partial failed projection");
    AllSnapshot caught_all;epoch=dh2_script_vm_required_failure_epoch(vm);
    check(all(vm,"caught",caught_all)==-5 && caught_all.calls==1 && dh2_script_vm_required_failure_epoch(vm)==epoch+1,"all caught required failure lost");
    AllSnapshot raw_all;check(all(vm,"raw_required",raw_all)==-5 && !raw_all.calls,"all uncaught required observed results");
    AllSnapshot ordinary_all;check(!all(vm,"ordinary_caught",ordinary_all) && ordinary_all.values[0].number==7,"all ordinary caught error rejected");
    AllSnapshot error_all;check(all(vm,"argument_error",error_all)==2 && !error_all.calls && all(vm,"nil_error",error_all)==-4,"all Lua error semantics changed");
    AllSnapshot reject_all;reject_all.error=1;check(all(vm,"pair",reject_all)==2 && reject_all.calls==1,"all ordinary observer status changed");
    AllSnapshot required_all;required_all.error=-1001;epoch=dh2_script_vm_required_failure_epoch(vm);
    check(all(vm,"pair",required_all)==-5 && required_all.calls==1 && dh2_script_vm_required_failure_epoch(vm)==epoch+1,"all required observer marker lost");
    check(dh2_script_vm_call_all_source_v1(vm,"pair",nullptr,0,nullptr,nullptr)==-1 &&
        dh2_script_vm_call_all_source_v1(nullptr,"pair",nullptr,0,AllSnapshot::observe,&pair)==-1 &&
        all(vm,nullptr,pair)==-1 && all(vm,"pair",pair,nullptr,1)==-1 &&
        all(vm,"pair",pair,args.data(),0xffffffffu)==-1,"all malformed inputs accepted");
    // Existing generic and discarded source APIs retain their old statuses.
    unsigned returned=0;dh2_script_value output[2]{};
    check(dh2_script_vm_call(vm,"caught",nullptr,0,output,2,&returned)==0 && returned==1 && output[0].boolean==1,"legacy caught status changed");
    check(dh2_script_vm_call_discard_source(vm,"caught",nullptr,0)==0,"legacy discard caught status changed");
    check(dh2_script_vm_call_discard_source(vm,"nil_error",nullptr,0)==-2,"legacy discard error status changed");
    check(load(vm,"function embedded() return 'a'..string.char(0)..'b' end")==0,"embedded fixture failed");
    check(dh2_script_vm_call(vm,"embedded",nullptr,0,output,2,&returned)==0 && output[0].text_bytes==3,"legacy raw string semantics changed");
    check(source_file(vm,"projection_order=''; captured=Identity()")==0 && dh2_script_vm_call_discard_source(vm,"many",nullptr,0)==0 &&
          std::string(global(vm,"projection_order").text)=="AB","legacy ordered discard projection changed");
    Snapshot guards;
    check(dh2_script_vm_call_indexed_source_v3(vm,"one",nullptr,0,0,nullptr,&guards)==-1,"missing observer accepted");
    check(dh2_script_vm_call_indexed_source_v3(nullptr,"one",nullptr,0,0,Snapshot::observe,&guards)==-1,"null VM accepted");
    check(dh2_script_vm_call_indexed_source_v3(vm,nullptr,nullptr,0,0,Snapshot::observe,&guards)==-1,"null name accepted");
    check(indexed(vm,"one",0,guards,nullptr,1)==-1,"missing args accepted");
    check(indexed(vm,"one",0,guards,args.data(),0xffffffffu)==-1,"argument count overflow accepted");
    check(indexed(vm,"one",0,guards,reinterpret_cast<const dh2_script_value*>(
        reinterpret_cast<const unsigned char*>(args.data())+1),1)==-1,"misaligned argument storage accepted");
    check(indexed(vm,"one",0,guards,reinterpret_cast<const dh2_script_value*>(UINTPTR_MAX-7),1)==-1,"wrapping argument range accepted");
    auto invalid=args[0];invalid.reserved=1;check(indexed(vm,"one",0,guards,&invalid,1)==-1,"reserved value accepted");
    invalid=args[0];invalid.type=7;check(indexed(vm,"one",0,guards,&invalid,1)==-1,"unsupported source input object accepted");
    check(dh2_script_vm_load_source_file(nullptr,nullptr,0)==-1 && dh2_script_vm_load_source_file(vm,nullptr,1)==-1,"invalid file input accepted");
    check(dh2_script_vm_load_source_file(vm,"x",8388609)==-1,"oversized source bytes accepted");
    check(source_file(vm,"error('one',0)")==2,"alias fixture failed");
    const char* error_alias=dh2_script_vm_error(vm);
    check(dh2_script_vm_load_source_file(vm,error_alias,3)==-1 && std::string(dh2_script_vm_error(vm))=="one","file error alias cleared VM storage");
    check(indexed(vm,error_alias,0,guards)==-1 && std::string(dh2_script_vm_error(vm))=="one","function error alias cleared VM storage");
    dh2_script_value alias_argument{};alias_argument.type=4;alias_argument.text=error_alias;alias_argument.text_bytes=3;
    check(indexed(vm,"echo",0,guards,&alias_argument,1)==-1 && std::string(dh2_script_vm_error(vm))=="one","argument error alias cleared VM storage");
    check(!guards.calls && vm==original_vm && global(vm,"file_ran").number==1,"guards altered VM/observer state");
    auto* deferred=dh2_script_vm_create_deferred(1024*1024);check(deferred,"deferred VM failed");
    check(!source_file(deferred,"function plain()return 7 end"),"deferred same-state load failed");
    Snapshot plain;check(!indexed(deferred,"plain",0,plain) && plain.value.number==7,"deferred observer implicitly required libraries");
    check(global(deferred,"math").type==0,"source extension opened libraries");dh2_script_vm_destroy(deferred);
    const std::string binary_source="binary_source_number=16777217; function binary_result()return binary_source_number end";
    size_t required_bytes=0;check(dh2_script_vm_compile(vm,binary_source.data(),binary_source.size(),"@source-binary",nullptr,0,&required_bytes)==-3,"binary size query failed");
    std::vector<unsigned char> binary(required_bytes);size_t written=0;
    check(!dh2_script_vm_compile(vm,binary_source.data(),binary_source.size(),"@source-binary",binary.data(),binary.size(),&written),"binary compilation failed");
    check(!dh2_script_vm_load_source_file(vm,binary.data(),binary.size()),"real float32 binary source file failed");
    Snapshot binary_result;check(!indexed(vm,"binary_result",0,binary_result) && binary_result.value.number==16777216.f,"binary ReturnValues changed");
    check(dh2_script_vm_load_source_file(vm,binary.data(),binary.size()-1)==3,"truncated binary source status differs");
    auto* tiny=dh2_script_vm_create(65536);check(tiny,"tiny VM create failed");
    check(source_file(tiny,"blob=string.rep('x',2000000)")==4,"protected source allocation error status differs");
    check(source_file(tiny,"function recovered()return 1 end")==0,"VM not usable after allocation error");
    Snapshot recovered;check(!indexed(tiny,"recovered",0,recovered) && recovered.value.number==1,"post-error observer failed");dh2_script_vm_destroy(tiny);
    check(dh2_script_vm_required_failure_epoch(nullptr)==0,"null epoch query differs");dh2_script_vm_destroy(vm);
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"observers\":"<<observers
             <<",\"required_callbacks\":"<<required_calls<<",\"same_vm\":true,\"legacy_status_preserved\":true,\"native_player_active\":false}\n";
    return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
