#pragma once
#include <cstring>
namespace dh2::android_ui {
// Exact Android log transport labels only. GameSWF trace may append one LF
// or CRLF; no other suffix/prefix, error or AS behavior is filtered here.
inline int hardcoded_menu_label(const char* text) noexcept {
    if(!text)return -1;
    static constexpr const char* labels[]={
        "Note To Self--> Hard-coded text --> \"Stats\" in textfield undefined",
        "Note To Self--> Hard-coded text --> \"Equipment\" in textfield undefined",
        "Note To Self--> Hard-coded text --> \"Skills\" in textfield undefined",
        "Note To Self--> Hard-coded text --> \"Faeries\" in textfield undefined",
        "Note To Self--> Hard-coded text --> \"Quests\" in textfield undefined"};
    for(unsigned i=0;i<5;++i){
        const auto n=std::strlen(labels[i]);if(std::strncmp(text,labels[i],n))continue;
        const auto* tail=text+n;
        if(!tail[0]||(tail[0]=='\n'&&!tail[1])||(tail[0]=='\r'&&tail[1]=='\n'&&!tail[2]))return int(i);
    }
    return -1;
}
}
