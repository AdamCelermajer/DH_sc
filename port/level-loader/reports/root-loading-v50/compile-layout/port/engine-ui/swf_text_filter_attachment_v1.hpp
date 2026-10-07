#pragma once
#include "text_filter_v1.hpp"
namespace gameswf {struct stream;}
namespace dh2::ui {
// Synchronous native placement argument. Nested AS placement restores its
// outer argument. The actual tag/character each retains the same effect lease.
class SwfTextEffectScopeV1 {
    std::shared_ptr<text_filter_v1::Effect> prior_;
public:
    explicit SwfTextEffectScopeV1(std::shared_ptr<text_filter_v1::Effect>);
    ~SwfTextEffectScopeV1();
    SwfTextEffectScopeV1(const SwfTextEffectScopeV1&)=delete;
    static std::shared_ptr<text_filter_v1::Effect> current();
};
void swf_read_text_filters_v1(text_filter_v1::Effect&,gameswf::stream&);
}
