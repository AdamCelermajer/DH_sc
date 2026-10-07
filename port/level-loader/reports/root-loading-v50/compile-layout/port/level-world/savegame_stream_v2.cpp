#include "savegame_stream_v2.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
namespace dh2::level {
void SavegameStreamV2::seek(std::uint64_t p)noexcept{read_=std::min<std::uint64_t>(p,bytes_.size());}
void SavegameStreamV2::seek_write(std::uint64_t p)noexcept{write_=std::min<std::uint64_t>(p,bytes_.size());}
bool SavegameStreamV2::read(void* out,std::size_t n,std::string& e){if((n&&!out)||read_>bytes_.size()||n>bytes_.size()-read_){e="Required complete source stream read/assertion";return false;}if(n)std::memcpy(out,bytes_.data()+read_,n);read_+=n;return true;}
bool SavegameStreamV2::write(data::Bytes b,std::string& e){if((b.size&&!b.data)||b.size>std::numeric_limits<std::uint32_t>::max()-write_){e="Source StreamBuffer native size domain exceeded";return false;}if(write_+b.size>bytes_.size())bytes_.resize(static_cast<std::size_t>(write_+b.size));if(b.size)std::memcpy(bytes_.data()+write_,b.data,b.size);write_+=b.size;return true;}
bool SavegameStreamV2::read_u32(std::uint32_t& v,std::string& e){std::uint8_t b[4];if(!read(b,4,e))return false;v=std::uint32_t(b[0])|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);return true;}
bool SavegameStreamV2::read_u64(std::uint64_t& v,std::string& e){std::uint8_t b[8];if(!read(b,8,e))return false;v=0;for(unsigned i=0;i<8;++i)v|=std::uint64_t(b[i])<<(8*i);return true;}
bool SavegameStreamV2::write_u32(std::uint32_t v,std::string& e){std::uint8_t b[4];for(unsigned i=0;i<4;++i)b[i]=static_cast<std::uint8_t>(v>>(8*i));return write({b,4},e);}
bool SavegameStreamV2::write_u64(std::uint64_t v,std::string& e){std::uint8_t b[8];for(unsigned i=0;i<8;++i)b[i]=static_cast<std::uint8_t>(v>>(8*i));return write({b,8},e);}
bool SavegameStreamV2::write_string(const std::string& s,std::string& e){if(s.size()>=std::numeric_limits<std::int32_t>::max()){e="Source CString serialized size domain exceeded";return false;}return write_u32(static_cast<std::uint32_t>(s.size()+1),e)&&write({reinterpret_cast<const std::uint8_t*>(s.c_str()),s.size()+1},e);}
bool SavegameStreamV2::read_string(std::string& out,std::string& e){std::uint32_t n;if(!read_u32(n,e))return false;if(static_cast<std::int32_t>(n)<=0){e="Required source CString invalid-length assertion";return false;}if(n>bytes_.size()-read_){e="Truncated source CString stream";return false;}auto* p=bytes_.data()+read_;if(p[n-1]!=0){e="Required source CString terminator";return false;}out.assign(reinterpret_cast<const char*>(p),n-1);read_+=n;return true;}
}
