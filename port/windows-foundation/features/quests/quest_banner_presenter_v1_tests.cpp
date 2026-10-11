// Preview 16 QUESTUI: quest banner layout and queue behaviour (no renderer).
#include "quest_banner_presenter_v1.hpp"
#include "../audio/named_ui_cue_v1.hpp"

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
        presenter.tick(2.1f);
        check(!presenter.visible(), "NEW QUEST ends after 2 seconds (reference video 174.5-176.5 s)");
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
        presenter.tick(1.75f);
        check(presenter.current().alpha < 1.0f && presenter.current().alpha > 0.0f, "banner fades before it ends");

        // v20.1 SOUNDS (B099/B104): the show cue fires once per banner, when it becomes the shown one (queued banners later).
        {
            QuestBannerPresenterV1 sounds;
            std::vector<bool> shown;
            sounds.on_shown = [&](bool completed) { shown.push_back(completed); };
            sounds.push(nq);
            check(shown.size() == 1 && !shown[0], "NEW QUEST cue on show");
            sounds.push(counter);
            check(shown.size() == 1, "counter has no cue");
            sounds.push(done);
            check(shown.size() == 1, "queued completion waits for its turn");
            sounds.tick(1.0f);
            check(shown.size() == 1, "no repeat while the same banner stays");
            sounds.tick(1.5f);
            check(shown.size() == 2 && shown[1], "QUEST COMPLETED cue when it becomes current");
            sounds.tick(5.0f);
            check(shown.size() == 2 && !sounds.visible(), "no cue when the queue empties");
        }
        // B099/B104 host lifecycle: pending work belongs to a show occurrence.
        {
            using namespace dh::foundation::audio;
            std::vector<std::string> attempts;
            NamedUiCueStatusV1 status=NamedUiCueStatusV1::output_not_ready;
            NamedUiCueRetryV1 retry([&](const char* name) {
                attempts.emplace_back(name);
                NamedUiCueResultV1 result;result.status=status;return result;
            });
            QuestBannerPresenterV1 banners;
            banners.on_shown=[&](bool completed){retry.begin(completed?"QuestComplete":"QuestNew");};
            banners.push(nq);
            for(unsigned i=1;i<NamedUiCueRetryV1::attempt_limit;++i)retry.tick(banners.visible());
            check(retry.attempts()==300&&!retry.pending(), "one unavailable-output occurrence exhausts its own bounded budget");
            banners.push(done);
            banners.tick(2.f);
            check(retry.attempts()==1&&retry.pending()&&attempts.back()=="QuestComplete", "promoted occurrence has a fresh retry budget");
            status=NamedUiCueStatusV1::submitted;
            retry.tick(banners.visible());
            const auto after_success=attempts.size();
            for(int i=0;i<3;++i)retry.tick(banners.visible());
            check(!retry.pending()&&attempts.size()==after_success, "successful submission clears retry and cannot duplicate");
            banners.tick(4.5f);
            status=NamedUiCueStatusV1::output_not_ready;
            banners.push(nq);
            const auto before_retire=attempts.size();
            banners.tick(2.f);
            retry.tick(banners.visible());
            status=NamedUiCueStatusV1::submitted;
            retry.tick(banners.visible());
            check(!retry.pending()&&attempts.size()==before_retire, "retired pending cue cannot play after output recovery");
            status=NamedUiCueStatusV1::output_not_ready;
            banners.push(nq);
            banners.push(done);
            banners.tick(2.f); // pending QuestNew is replaced by the completion occurrence
            status=NamedUiCueStatusV1::submitted;
            retry.tick(banners.visible());
            check(attempts.back()=="QuestComplete", "replacement retries only the current occurrence");
            for(const auto permanent:{NamedUiCueStatusV1::asset_missing,NamedUiCueStatusV1::absent_in_sound_table,
                                     NamedUiCueStatusV1::failed,NamedUiCueStatusV1::no_audio_runtime}) {
                status=permanent;retry.begin("QuestNew");
                const auto before_tick=attempts.size();retry.tick(true);
                check(!retry.pending()&&attempts.size()==before_tick, "permanent failures never retry");
            }
            status=NamedUiCueStatusV1::submitted;
            retry.begin("QuestNew");retry.begin("QuestNew");
            check(retry.attempts()==1, "same label on a later occurrence is a new cue, not globally deduplicated");
        }
        std::printf("quest_banner_presenter_v1: all tests passed\n");
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "quest_banner_presenter_v1 test failed: " << e.what() << '\n';
        return 1;
    }
}
