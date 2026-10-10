#include "quest_banner_presenter_v1.hpp"

#include <algorithm>

namespace dh::foundation {
namespace {

// Original English headings (global.english: "New Quest", "Quest Completed"; gameplaymenus.english: "Reward", "EXP", "GOLD").
constexpr const char* kNewQuestHeadingV1 = "New Quest";
constexpr const char* kQuestCompletedHeadingV1 = "Quest Completed";
constexpr const char* kRewardHeadingV1 = "Reward";
constexpr const char* kExpLabelV1 = "EXP";
constexpr const char* kGoldLabelV1 = "GOLD";

// Presentation colours (placeholder palette sampled from the reference frames: gold headings, ivory body).
constexpr std::uint32_t kHeadingRgbV1 = 0xE8C35A;
constexpr std::uint32_t kBodyRgbV1 = 0xF2EAD6;

constexpr float kNewQuestSecondsV1 = 3.0f;
constexpr float kUpdatedSecondsV1 = 2.0f;
constexpr float kCompletedSecondsV1 = 4.5f;
constexpr float kFadeSecondsV1 = 0.5f;

QuestBannerLineV1 line(std::string text, std::uint32_t rgb, int height) {
    QuestBannerLineV1 out;
    out.text = std::move(text);
    out.rgb = rgb;
    out.source_height = height;
    return out;
}

float seconds_for(const quest_runtime::QuestBannerV1& banner) {
    switch (banner.kind) {
    case quest_runtime::QuestBannerV1::Kind::new_quest: return kNewQuestSecondsV1;
    case quest_runtime::QuestBannerV1::Kind::updated: return kUpdatedSecondsV1;
    case quest_runtime::QuestBannerV1::Kind::completed: return kCompletedSecondsV1;
    }
    return kNewQuestSecondsV1;
}

} // namespace

std::vector<QuestBannerLineV1> layout_quest_banner_v1(const quest_runtime::QuestBannerV1& banner) {
    std::vector<QuestBannerLineV1> lines;
    const std::string body = banner.text;
    switch (banner.kind) {
    case quest_runtime::QuestBannerV1::Kind::new_quest:
        lines.push_back(line(kNewQuestHeadingV1, kHeadingRgbV1, 18));
        if (!body.empty()) lines.push_back(line(body, kBodyRgbV1, 14));
        break;
    case quest_runtime::QuestBannerV1::Kind::updated:
        // Objective counter: the authored objective text (when resolved) over "counted / authored".
        if (!body.empty()) lines.push_back(line(body, kHeadingRgbV1, 16));
        lines.push_back(line(std::to_string(banner.quantity) + " / " + std::to_string(banner.required), kBodyRgbV1, 14));
        break;
    case quest_runtime::QuestBannerV1::Kind::completed:
        lines.push_back(line(kQuestCompletedHeadingV1, kHeadingRgbV1, 18));
        if (!body.empty()) lines.push_back(line(body, kBodyRgbV1, 13));
        lines.push_back(line(kRewardHeadingV1, kHeadingRgbV1, 16));
        if (banner.reward_xp > 0)
            lines.push_back(line(std::to_string(banner.reward_xp) + " " + kExpLabelV1, kBodyRgbV1, 14));
        if (banner.reward_gold > 0)
            lines.push_back(line(std::to_string(banner.reward_gold) + " " + kGoldLabelV1, kBodyRgbV1, 14));
        break;
    }
    return lines;
}

void QuestBannerPresenterV1::push(const quest_runtime::QuestBannerV1& banner) {
    const bool counter = banner.kind == quest_runtime::QuestBannerV1::Kind::updated;
    const bool completes = banner.kind == quest_runtime::QuestBannerV1::Kind::completed;
    // A counter replaces the queued counter of the same row (the newest count is what matters), and a
    // completion drops the row's queued counters, so a quick run of kills does not queue seconds of banners.
    if (counter || completes) {
        queue_.erase(std::remove_if(queue_.begin(), queue_.end(), [&](const Entry& e) {
            return e.counter && e.row == banner.row && e.difficulty == banner.difficulty;
        }), queue_.end());
    }
    Entry entry;
    entry.display.lines = layout_quest_banner_v1(banner);
    entry.display.completed = completes;
    entry.counter = counter;
    entry.row = banner.row;
    entry.difficulty = banner.difficulty;
    entry.total = seconds_for(banner);
    entry.remaining = entry.total;
    queue_.push_back(std::move(entry));
}

void QuestBannerPresenterV1::tick(float seconds) {
    if (queue_.empty()) return;
    queue_.front().remaining -= std::max(seconds, 0.0f);
    while (!queue_.empty() && queue_.front().remaining <= 0.0f) queue_.pop_front();
}

QuestBannerDisplayV1 QuestBannerPresenterV1::current() const {
    if (queue_.empty()) return {};
    const auto& entry = queue_.front();
    QuestBannerDisplayV1 out = entry.display;
    out.alpha = std::clamp(entry.remaining / kFadeSecondsV1, 0.0f, 1.0f);
    return out;
}

} // namespace dh::foundation
