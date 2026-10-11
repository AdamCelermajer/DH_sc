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

// Slot indices of the original banner frame (see QuestBannerLineV1).
constexpr int kHeadingSlotV1 = 0;
constexpr int kSentenceSlotV1 = 1;
constexpr int kRewardHeadingSlotV1 = 2;
constexpr int kRewardValuesSlotV1 = 3;

// Presentation timing (placeholder fit to the reference frames; the original hold is not decoded).
constexpr float kNewQuestSecondsV1 = 3.0f;
constexpr float kCompletedSecondsV1 = 4.5f;
constexpr float kFadeSecondsV1 = 0.5f;

QuestBannerLineV1 line(std::string text, int slot, int stack = 0) {
    QuestBannerLineV1 out;
    out.text = std::move(text);
    out.slot = slot;
    out.stack = stack;
    return out;
}

float seconds_for(const quest_runtime::QuestBannerV1& banner) {
    return banner.kind == quest_runtime::QuestBannerV1::Kind::completed ? kCompletedSecondsV1 : kNewQuestSecondsV1;
}

} // namespace

std::vector<QuestBannerLineV1> layout_quest_banner_v1(const quest_runtime::QuestBannerV1& banner) {
    std::vector<QuestBannerLineV1> lines;
    const std::string body = banner.text;
    switch (banner.kind) {
    case quest_runtime::QuestBannerV1::Kind::new_quest:
        lines.push_back(line(kNewQuestHeadingV1, kHeadingSlotV1));
        if (!body.empty()) lines.push_back(line(body, kSentenceSlotV1));
        break;
    case quest_runtime::QuestBannerV1::Kind::updated:
        // No original counter banner exists (no call site in the source); nothing is laid out.
        break;
    case quest_runtime::QuestBannerV1::Kind::completed:
        lines.push_back(line(kQuestCompletedHeadingV1, kHeadingSlotV1));
        if (!body.empty()) lines.push_back(line(body, kSentenceSlotV1));
        lines.push_back(line(kRewardHeadingV1, kRewardHeadingSlotV1));
        if (banner.reward_xp > 0)
            lines.push_back(line(std::to_string(banner.reward_xp) + " " + kExpLabelV1, kRewardValuesSlotV1, 0));
        if (banner.reward_gold > 0)
            lines.push_back(line(std::to_string(banner.reward_gold) + " " + kGoldLabelV1, kRewardValuesSlotV1,
                                 banner.reward_xp > 0 ? 1 : 0));
        break;
    }
    return lines;
}

void QuestBannerPresenterV1::push(const quest_runtime::QuestBannerV1& banner) {
    // The source has no objective-counter banner: a counter is not shown (the runtime still logs it).
    if (banner.kind == quest_runtime::QuestBannerV1::Kind::updated) return;
    const bool completes = banner.kind == quest_runtime::QuestBannerV1::Kind::completed;
    Entry entry;
    entry.display.lines = layout_quest_banner_v1(banner);
    entry.display.completed = completes;
    entry.counter = false;
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
