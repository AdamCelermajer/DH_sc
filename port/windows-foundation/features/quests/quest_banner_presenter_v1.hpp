#pragma once
// Quest banners (Preview 16 QUESTUI): line layout and timing for the runtime banners.
//
// Source behaviour (IDA Quest::SetState): NEW QUEST = DialogMsg(GLOBAL_QUEST_NEW heading,
// quest text, DialogStyles.QuestMsgDialog); QUEST COMPLETED = DialogMsg(QuestCompletedMsgDialog)
// with the reward string. Headings are the original English strings ("New Quest",
// "Quest Completed" from global.english; "Reward", "EXP", "GOLD" from gameplaymenus.english).
// Body text is the authored StringID text resolved by the runtime service.
//
// The dialog frame art (dqhud QuestMsgDialog / QuestCompletedMsgDialog movies) is not exported
// by this build. The drawn panel is a PLACEHOLDER (see the Preview 16 QUESTUI report).
#include "../quest_runtime/quest_runtime_v1.hpp"

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace dh::foundation {

struct QuestBannerLineV1 {
    std::string text;
    std::uint32_t rgb{0xFFFFFF};   // 0xRRGGBB
    int source_height{14};          // source pixels (glyph raster size)
};

struct QuestBannerDisplayV1 {
    std::vector<QuestBannerLineV1> lines;
    float alpha{1.f};               // 1 while shown, fades over the last 0.5 s
    bool completed{false};
};

// Lines for one runtime banner. Pure: no renderer, no clock.
std::vector<QuestBannerLineV1> layout_quest_banner_v1(const quest_runtime::QuestBannerV1& banner);

// Shown banners, one at a time, in the order the runtime queued them.
class QuestBannerPresenterV1 {
public:
    void push(const quest_runtime::QuestBannerV1& banner);
    void tick(float seconds);
    bool visible() const noexcept { return !queue_.empty(); }
    QuestBannerDisplayV1 current() const;
    std::size_t queued() const noexcept { return queue_.size(); }
private:
    struct Entry {
        QuestBannerDisplayV1 display;
        float remaining{};
        float total{};
        bool counter{};          // QUEST UPDATED (objective counter)
        std::int32_t row{-1};
        std::int32_t difficulty{0};
    };
    std::deque<Entry> queue_;
};

} // namespace dh::foundation
