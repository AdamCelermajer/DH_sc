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
        check(lines.size() == 1 && lines[0].text == "New Quest", "NEW QUEST heading only when no body text resolves");

        auto done = make(QuestBannerV1::Kind::completed, 53);
        done.reward_xp = 20;
        done.reward_gold = 150;
        lines = layout_quest_banner_v1(done);
        check(has_line(lines, "Quest Completed") && has_line(lines, "Reward") &&
              has_line(lines, "20 EXP") && has_line(lines, "150 GOLD"), "QUEST COMPLETED lines carry the authored rewards");
        auto done_no_gold = make(QuestBannerV1::Kind::completed, 53);
        done_no_gold.reward_xp = 50;
        check(!has_line(layout_quest_banner_v1(done_no_gold), "0 GOLD"), "zero gold is not shown");

        auto counter = make(QuestBannerV1::Kind::updated, 53);
        counter.quantity = 3;
        lines = layout_quest_banner_v1(counter);
        check(lines.size() == 1 && lines[0].text == "3 / 8", "objective counter reads counted / authored");

        // Queue: a counter replaces the queued counter of its row; a completion drops the row's counters.
        QuestBannerPresenterV1 presenter;
        presenter.push(nq);
        presenter.tick(3.1f);
        check(!presenter.visible(), "NEW QUEST ends after 3 seconds");
        presenter.push(counter);
        counter.quantity = 4;
        presenter.push(counter);
        check(presenter.queued() == 1, "counter replaces the queued counter of the same row");
        check(presenter.current().lines[0].text == "3 / 8" || presenter.current().lines[0].text == "4 / 8",
              "counter is shown");
        presenter.push(done);
        check(presenter.queued() == 1 && presenter.current().completed,
              "completion drops the row's queued counters and is shown");
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
