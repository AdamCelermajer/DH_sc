#include "swf_text_filter_attachment_v1.hpp"
#include "gameswf/gameswf_stream.h"
#include "gameswf/gameswf_types.h"
#include <stdexcept>
namespace dh2::ui {
namespace {thread_local std::shared_ptr<text_filter_v1::Effect> effect;}
SwfTextEffectScopeV1::SwfTextEffectScopeV1(std::shared_ptr<text_filter_v1::Effect> next):prior_(std::move(effect)){effect=std::move(next);}
SwfTextEffectScopeV1::~SwfTextEffectScopeV1(){effect=std::move(prior_);}
std::shared_ptr<text_filter_v1::Effect> SwfTextEffectScopeV1::current(){return effect;}
void swf_read_text_filters_v1(text_filter_v1::Effect& out,gameswf::stream& s){
    text_filter_v1::Reader r;
    r.byte=[&](auto& out,std::string&){out=s.read_u8();return true;};r.half=[&](auto& out,std::string&){out=s.read_u16();return true;};
    r.bits=[&](auto width,auto& out,std::string&){out=s.read_uint(width);return true;};r.bit=[&](bool& out,std::string&){out=s.read_uint(1)!=0;return true;};
    r.fixed=[&](float& out,std::string&){out=s.read_fixed();return true;};r.rgba=[&](auto& out,std::string&){gameswf::rgba c;c.read_rgba(&s);out={c.m_r,c.m_g,c.m_b,c.m_a};return true;};
    std::string error;if(!text_filter_v1::read(out,r,error))throw std::runtime_error(error);
}
}
