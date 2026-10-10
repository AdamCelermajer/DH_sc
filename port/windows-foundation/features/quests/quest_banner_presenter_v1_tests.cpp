// Preview 16 QUESTUI: quest banner layout and queue behaviour (no renderer).
#include "quest_banner_presenter_v1.hpp"

#include <cstdio>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using quest_runtime::QuestBannerV1;

namespace {

void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

QuestBannerV1 make(QuestBannerV1::Kind kind, std::int32_t row) {
    QuestBannerV1 b;
    b.kind = kind;
    b.row = row;
    b.required = 8;
    return b;
}

bool has_line(const std::vector<QuestBannerLineV1>& lines, const std::string& text) {
    for (const auto& l : lines) if (l.text == text) return true;
    return false;
}

} // namespace

int main() {
    try {
        // Headings and rewards (original English strings; reward lines only when granted).
        auto nq = make(QuestBannerV1::Kind::new_quest, 53);
        auto lines = layout_quest_banner_v1(nq);
        check(lines.size() == 1 && lines[0].text == "New Quest" && lines[0].slot == 0,
              "NEW QUEST heading only when no body text resolves (heading slot)");
        lines = layout_quest_banner_v1(make(QuestBannerV1::Kind::new_quest, 53));
        check(lines.size() == 1, "no body, one line");
        auto nq_text = make(QuestBannerV1::Kind::new_quest, 53);
        nq_text.text = "Kill 8 Bog Moths.";
        lines = layout_quest_banner_v1(nq_text);
        check(lines.size() == 2 && lines[1].text == "Kill 8 Bog Moths." && lines[1].slot == 1,
              "NEW QUEST sentence goes to the sentence slot of the original frame");

        auto done = make(QuestBannerV1::Kind::completed, 53);
        done.reward_xp = 20;
        done.reward_gold = 150;
        lines = layout_quest_banner_v1(done);
        check(has_line(lines, "Quest Completed") && has_line(lines, "Reward") &&
              has_line(lines, "20 EXP") && has_line(lines, "150 GOLD"), "QUEST COMPLETED lines carry the authored rewards");
        auto done_no_gold = make(QuestBannerV1::Kind::completed, 53);
        done_no_gold.reward_xp = 50;
        check(!has_line(layout_quest_banner_v1(done_no_gold), "0 GOLD"), "zero gold is not shown");

        // The source has no objective-counter banner: a counter lays out nothing and is never queued.
        auto counter = make(QuestBannerV1::Kind::updated, 53);
        counter.quantity = 3;
        check(layout_quest_banner_v1(counter).empty(), "no original counter banner: nothing is laid out");

        // Queue: counters are not shown; a completion is shown and carries its reward slots.
        QuestBannerPresenterV1 presenter;
        presenter.push(nq);
        presenter.tick(3.1f);
        check(!presenter.visible(), "NEW QUEST ends after 3 seconds");
        presenter.push(counter);
        check(!presenter.visible(), "counter is not shown");
        presenter.push(done);
        check(presenter.queued() == 1 && presenter.current().completed,
              "completion is shown");
        const auto reward = presenter.current().lines;
        // The test banner has no body text: heading, reward heading, EXP line, GOLD line.
        check(reward.size() == 4 && reward[0].slot == 0 && reward[1].slot == 2 && reward[2].slot == 3 && reward[3].slot == 3 &&
              reward[2].stack == 0 && reward[3].stack == 1,
              "reward values share the reward slot, one line per value");
        presenter.tick(4.5f);
        check(!presenter.visible(), "completion ends after 4.5 seconds");

        // Fade over the last half second.
        presenter.push(nq);
        presenter.tick(2.75f);
        check(presenter.current().alpha < 1.0f && presenter.current().alpha > 0.0f, "banner fades before it ends");
        std::printf("quest_banner_presenter_v1: all tests passed\n");
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "quest_banner_presenter_v1 test failed: " << e.what() << '\n';
        return 1;
    }
}
