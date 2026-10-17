#include "netstruct_members_v1.hpp"
#include <cstring>
#include <new>

namespace dh2::netstruct_members_v1 {
namespace {
struct Busy {bool& flag;explicit Busy(bool& f):flag(f){flag=true;}~Busy(){flag=false;}};
bool valid(BytesView b) {return b.size>=0 && (!b.size || b.data);}
Status release(Bytes& b,const Memory& memory) {
    if(!b.data) return Status::complete;
    if(!memory.release) return Status::missing_provider;
    try {memory.release(memory.context,b.data);} catch(...) {return Status::provider_failed;}
    b.data=nullptr;return Status::complete; // Original retains the size word.
}
Status buffer(Bytes& b,BytesView value,const Memory& memory) {
    if(!valid(value)) return Status::invalid_argument;
    if(b.size!=value.size) {
        const auto status=release(b,memory);if(status!=Status::complete) return status;
        b.size=value.size;
        if(!memory.allocate) return Status::missing_provider;
        try {b.data=static_cast<std::uint8_t*>(memory.allocate(memory.context,value.size,2));}
        catch(...) {return Status::provider_failed;}
        // A null positive-size allocation is an explicit native failure. The
        // source allocator's unrecoverable OOM path is not emulated as success.
        if(value.size && !b.data) return Status::allocation_failed;
    }
    if(b.data && b.size>0) {
        std::memset(b.data,0,static_cast<std::size_t>(b.size));
        std::memcpy(b.data,value.data,static_cast<std::size_t>(b.size));
    }
    return Status::complete;
}
Status changed(Member& m) {
    return character_level_member::mark_changed(&m.header,m.serial)==character_level_member::Status::complete
        ?Status::complete:Status::invalid_argument;
}
void prefix(Member& m,std::uint32_t bits) {
    m.header.size_bits=bits;m.header.revision=0;m.header.field_10=UINT32_MAX;
    m.header.field_14=UINT32_MAX;m.header.source_stamp=0;m.header.changed=0;
}
Status scalar(Member& m,std::int32_t value) {
    if(m.kind==Kind::boolean) {
        const auto byte=static_cast<std::uint8_t>(value);
        if(m.header.reserved[0]==byte) return Status::complete;
        m.header.reserved[0]=byte;return changed(m);
    }
    character_level_member::Result result{};
    return character_level_member::set_value(&m.header,m.serial,value,&result)==character_level_member::Status::complete
        ?Status::complete:Status::invalid_argument;
}
Status string(Member& m,std::string_view value) {
    if(m.text==value) return Status::complete;
    try {m.text.assign(value.data(),value.size());}catch(const std::bad_alloc&){return Status::allocation_failed;}
    catch(...){return Status::provider_failed;}
    return changed(m);
}
Status bytes(Member& m,BytesView value) {
    if(!valid(value)) return Status::invalid_argument;
    if(m.bytes.size==value.size && (!value.size ||
       (m.bytes.data && std::memcmp(m.bytes.data,value.data,static_cast<std::size_t>(value.size))==0))) return Status::complete;
    const auto status=buffer(m.bytes,value,m.memory);
    return status==Status::complete?changed(m):status;
}
bool ready(const Member& m,Kind kind) {return m.constructed && !m.busy && m.kind==kind && m.serial;}
void copy_prefix(Member& dest,const Member& source) {
    // Original scalar/string copies do not write padding +1d..1f. Bool's
    // actual value at +1d is copied separately, leaving +1e..1f untouched.
    dest.header.size_bits=source.header.size_bits;dest.header.revision=source.header.revision;
    dest.header.field_10=source.header.field_10;dest.header.field_14=source.header.field_14;
    dest.header.source_stamp=source.header.source_stamp;dest.header.changed=source.header.changed;
}
}
std::uint32_t vtable(Kind kind,std::uint32_t bits,bool base) {
    if(base) return 0x963648;
    switch(kind) {
    case Kind::integer:
        switch(bits){case 6:return 0x9923e0;case 8:return 0x9637f0;case 16:return 0x963730;case 17:return 0x9637c0;case 32:return 0x9635e8;default:return 0;}
    case Kind::unsigned_integer:
        switch(bits){case 1:return 0x965828;case 5:return 0x9925e0;case 8:return 0x963670;case 32:return 0x965858;default:return 0;}
    case Kind::boolean:return bits==1?0x9636d0:0;
    case Kind::string32:return bits==256?0x963760:0;
    case Kind::byte_array:
        switch(bits){case 24:return 0x963898;case 240:return 0x9638e0;case 288:return 0x963820;default:return 0;}
    }
    return 0;
}
Status construct_scalar(Member& m,Kind kind,std::uint32_t bits,std::int32_t initial,std::uint64_t* serial) {
    if(!serial || !vtable(kind,bits) || (kind!=Kind::integer && kind!=Kind::unsigned_integer && kind!=Kind::boolean)) return Status::invalid_argument;
    if(m.constructed || m.busy) return Status::invalid_state;
    Busy busy(m.busy);m.kind=kind;m.serial=serial;prefix(m,bits);
    m.header.vtable=vtable(kind,bits);m.constructed=true;return scalar(m,initial);
}
Status construct_string(Member& m,std::string_view initial,std::uint64_t* serial) {
    if(!serial) return Status::invalid_argument;
    if(m.constructed || m.busy) return Status::invalid_state;
    Busy busy(m.busy);m.kind=Kind::string32;m.serial=serial;prefix(m,256);
    m.header.vtable=0x963790;m.text.clear();m.constructed=true;
    const auto status=string(m,initial);if(status!=Status::complete) return status;
    m.header.vtable=vtable(m.kind,256);return Status::complete;
}
Status construct_bytes(Member& m,std::uint32_t width,BytesView initial,std::uint64_t* serial,Memory memory) {
    if(!serial || !valid(initial) || (width!=3 && width!=30 && width!=36)) return Status::invalid_argument;
    if(m.constructed || m.busy) return Status::invalid_state;
    Busy busy(m.busy);Bytes temporary{};
    auto status=buffer(temporary,initial,memory);if(status!=Status::complete) {release(temporary,memory);return status;}
    m.kind=Kind::byte_array;m.serial=serial;m.memory=memory;prefix(m,width*8);
    m.header.vtable=0x963868;m.bytes={};m.constructed=true;
    status=bytes(m,{temporary.data,temporary.size});
    const auto retired=release(temporary,memory);
    if(status!=Status::complete) return status;
    if(retired!=Status::complete) return retired;
    m.header.vtable=vtable(m.kind,width*8);return Status::complete;
}
Status set_int(Member& m,std::int32_t value) {
    if(!ready(m,Kind::integer)) return Status::invalid_state;
    Busy busy(m.busy);return scalar(m,value);
}
Status set_uint(Member& m,std::uint32_t value) {
    if(!ready(m,Kind::unsigned_integer)) return Status::invalid_state;
    std::int32_t signed_value;std::memcpy(&signed_value,&value,4);
    Busy busy(m.busy);return scalar(m,signed_value);
}
Status set_bool(Member& m,std::uint8_t value) {
    if(!ready(m,Kind::boolean)) return Status::invalid_state;
    Busy busy(m.busy);return scalar(m,value);
}
Status set_string(Member& m,std::string_view value) {
    if(!ready(m,Kind::string32)) return Status::invalid_state;
    Busy busy(m.busy);return string(m,value);
}
Status set_bytes(Member& m,BytesView value) {
    if(!ready(m,Kind::byte_array)) return Status::invalid_state;
    if(m.bytes.size<0 || (m.bytes.size>0 && !m.bytes.data)) return Status::invalid_state;
    Busy busy(m.busy);return bytes(m,value);
}
Status set_buffer(Member& m,BytesView value) {
    if(!ready(m,Kind::byte_array)) return Status::invalid_state;
    if(m.bytes.size<0 || (m.bytes.size>0 && !m.bytes.data)) return Status::invalid_state;
    Busy busy(m.busy);Bytes temporary{};
    auto status=buffer(temporary,value,m.memory);
    if(status==Status::complete) status=bytes(m,{temporary.data,temporary.size});
    const auto retired=release(temporary,m.memory);
    return status==Status::complete?retired:status;
}
Status copy_construct(Member& dest,const Member& source) {
    if(&dest==&source || dest.constructed || dest.busy || !source.constructed || source.busy) return Status::invalid_state;
    Busy busy(dest.busy);dest.kind=source.kind;dest.serial=source.serial;dest.memory=source.memory;
    copy_prefix(dest,source);dest.header.vtable=vtable(source.kind,source.header.size_bits);dest.constructed=true;
    if(source.kind==Kind::boolean) dest.header.reserved[0]=source.header.reserved[0];
    else if(source.kind==Kind::string32) {
        try{dest.text=source.text;}catch(const std::bad_alloc&){return Status::allocation_failed;}catch(...){return Status::provider_failed;}
    } else if(source.kind==Kind::byte_array) {
        dest.bytes={};return buffer(dest.bytes,{source.bytes.data,source.bytes.size},dest.memory);
    } else dest.header.value=source.header.value;
    return Status::complete;
}
Status assign(Member& dest,const Member& source) {
    if(!dest.constructed || !source.constructed || dest.busy || source.busy || dest.kind!=source.kind) return Status::invalid_state;
    Busy busy(dest.busy);
    if(dest.kind==Kind::boolean) return scalar(dest,source.header.reserved[0]);
    if(dest.kind==Kind::string32) return string(dest,source.text);
    if(dest.kind==Kind::byte_array) return bytes(dest,{source.bytes.data,source.bytes.size});
    return scalar(dest,source.header.value);
}
Status transition_to_base(Member& m) {
    if(!m.constructed || m.busy) return Status::invalid_state;
    m.header.vtable=vtable(m.kind,m.header.size_bits,true);return Status::complete;
}
Status retire_payload(Member& m) {
    if(!m.constructed || m.busy) return Status::invalid_state;
    Busy busy(m.busy);
    if(m.kind==Kind::byte_array) return release(m.bytes,m.memory);
    if(m.kind==Kind::string32) std::string{}.swap(m.text);
    return Status::complete;
}
Status destroy(Member& m) {
    if(m.busy) return Status::invalid_state;
    if(!m.constructed) return Status::complete;
    if(m.kind==Kind::byte_array) m.header.vtable=0x963868;
    if(m.kind==Kind::string32) m.header.vtable=0x963790;
    auto status=retire_payload(m);if(status!=Status::complete) return status;
    status=transition_to_base(m);m.constructed=false;return status;
}
Member::~Member() {destroy(*this);}
Status construct(NetStruct& n) {
    if(n.constructed || n.busy) return Status::invalid_state;
    Busy busy(n.busy);n.source_vtable=0x9924d8;n.count=0;n.enabled_108=0;
    n.history.clear();n.field_124=0;n.field_125=0;n.field_128=0;n.members.fill(nullptr);
    n.constructed=true;return Status::complete;
}
Status declare_member(NetStruct& n,Member& m) {
    if(!n.constructed || n.busy || n.count>=n.members.size() || !m.constructed) return Status::invalid_state;
    n.members[n.count]=&m;++n.count;return Status::complete;
}
Status assign(NetStruct& dest,const NetStruct& source) {
    if(!dest.constructed || !source.constructed || dest.busy || source.busy) return Status::invalid_state;
    Busy busy(dest.busy);dest.members=source.members;dest.count=source.count;dest.enabled_108=source.enabled_108;
    try{dest.history=source.history;}catch(const std::bad_alloc&){return Status::allocation_failed;}catch(...){return Status::provider_failed;}
    dest.field_124=source.field_124;dest.field_125=source.field_125;dest.field_128=source.field_128;
    return Status::complete;
}
Status copy_construct(NetStruct& dest,const NetStruct& source) {
    if(&dest==&source || dest.constructed || dest.busy || !source.constructed || source.busy) return Status::invalid_state;
    dest.source_vtable=0x9924d8;dest.constructed=true;return assign(dest,source);
}
Status destroy(NetStruct& n) {
    if(n.busy) return Status::invalid_state;
    if(!n.constructed) return Status::complete;
    Busy busy(n.busy);n.source_vtable=0x9924d8;n.history.clear();n.constructed=false;return Status::complete;
}
} // namespace dh2::netstruct_members_v1
