#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <utility>

namespace dh::foundation::audio {
enum class NamedUiCueStatusV1 { submitted, absent_in_sound_table, asset_missing, output_not_ready, failed, no_audio_runtime };
struct NamedUiCueResultV1 {
    NamedUiCueStatusV1 status{NamedUiCueStatusV1::no_audio_runtime};
    std::int32_t ordinal{-1};
    std::int32_t uid{-1};
    std::uint64_t token{}; // queued command identity; started/finished receipts prove mixer consumption
};
inline const char* named_ui_cue_status_v1(NamedUiCueStatusV1 status) {
    switch(status) {
    case NamedUiCueStatusV1::submitted:return "submitted";
    case NamedUiCueStatusV1::absent_in_sound_table:return "absent_in_sound_table";
    case NamedUiCueStatusV1::asset_missing:return "asset_missing";
    case NamedUiCueStatusV1::output_not_ready:return "output_not_ready";
    case NamedUiCueStatusV1::failed:return "failed";
    default:return "no_audio_runtime";
    }
}

// Host startup/lifecycle adaptation: defer only an unavailable device output.
// The original show action fires once; pending work belongs to that occurrence.
// begin replaces the previous occurrence even when the label is identical.
class NamedUiCueRetryV1 {
public:
    using Submit = std::function<NamedUiCueResultV1(const char*)>;
    explicit NamedUiCueRetryV1(Submit submit):submit_(std::move(submit)) {}
    void begin(const char* name) { pending_=name?name:"";attempts_=0;attempt(); }
    void tick(bool occurrence_visible) {
        if(!occurrence_visible) {pending_.clear();return;}
        if(!pending_.empty())attempt();
    }
    bool pending() const noexcept {return !pending_.empty();}
    unsigned attempts() const noexcept {return attempts_;}
    static constexpr unsigned attempt_limit=300;
private:
    void attempt() {
        if(pending_.empty())return;
        ++attempts_;
        const auto result=submit_?submit_(pending_.c_str()):NamedUiCueResultV1{};
        if(result.status!=NamedUiCueStatusV1::output_not_ready||attempts_>=attempt_limit)pending_.clear();
    }
    Submit submit_;
    std::string pending_;
    unsigned attempts_{};
};
}
