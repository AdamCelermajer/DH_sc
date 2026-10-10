#include "../character_menu/source_composition.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation::character_menu;

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main() {
    try {
        constexpr const char* quest_menu = "menu_QuestLogSheetNEW";
        std::string error;
        auto owner = std::static_pointer_cast<void>(std::make_shared<int>(599));
        auto foreign_owner = std::static_pointer_cast<void>(std::make_shared<int>(267));
        SourceCompositionV1 composition(owner);
        bool page_ready = false;
        unsigned ready_calls = 0, append_calls = 0, release_calls = 0;
        std::vector<std::string> routed;
        float released_x = -1.f, released_y = -1.f;

        SourcePageProviderV1 quest;
        quest.owner = owner;
        quest.ready = [&](std::string& message) {
            ++ready_calls;
            if (!page_ready) message = "same CharacterState Quest progress/text is unavailable";
            else message.clear();
            return page_ready;
        };
        quest.append = [&](Frame& frame, std::string&) {
            ++append_calls;
            routed.push_back("append");
            MenuTextField row0; row0.path = "menu_QuestLogSheetNEW/Assigned/row0";
            MenuTextField row1; row1.path = "menu_QuestLogSheetNEW/Assigned/row1";
            frame.text.push_back({row0, "source row 0"});
            frame.text.push_back({row1, "source row 1"});
            return true;
        };
        quest.release = [&](float x, float y, std::string&) {
            ++release_calls;
            released_x = x; released_y = y;
            routed.push_back("release");
            return true;
        };

        auto wrong_owner = quest;
        wrong_owner.owner = foreign_owner;
        check(!composition.register_source_menu_page(quest_menu, wrong_owner, error) &&
              error.find("same selected-character owner") != std::string::npos,
              "pushed Quest page accepted a different selected-character owner");
        check(!composition.register_source_menu_page("", quest, error),
              "pushed page registration accepted a missing source menu symbol");
        check(composition.register_source_menu_page(quest_menu, quest, error), error.c_str());
        check(!composition.register_source_menu_page(quest_menu, quest, error) &&
              error.find("already registered") != std::string::npos,
              "duplicate source menu symbol replaced the original page provider");

        Frame frame;
        MenuTextField root_field; root_field.path = "root-existing-content";
        frame.text.push_back({root_field, "root"});
        const auto original_frame = frame;
        check(!composition.source_menu_page_ready("Menu_QuestLogSheetNEW", error),
              "source page lookup normalized an inexact menu symbol");
        check(!composition.append_source_menu_page(quest_menu, frame, error) &&
              error.find("unavailable") != std::string::npos && append_calls == 0 &&
              frame.text.size() == original_frame.text.size(),
              "unready source page appended partial content or bypassed readiness");
        check(!composition.release_source_menu_page(quest_menu, 20.f, 30.f, error) &&
              release_calls == 0,
              "unready source page dispatched an input action");

        page_ready = true;
        check(composition.source_menu_page_ready(quest_menu, error), error.c_str());
        check(composition.append_source_menu_page(quest_menu, frame, error), error.c_str());
        check(frame.text.size() == 3 && frame.text[0].field.path == "root-existing-content" &&
              frame.text[1].field.path == "menu_QuestLogSheetNEW/Assigned/row0" &&
              frame.text[2].field.path == "menu_QuestLogSheetNEW/Assigned/row1" &&
              frame.text[1].value == "source row 0" && frame.text[2].value == "source row 1",
              "pushed Quest page did not preserve root frame and authored provider row order");
        check(composition.release_source_menu_page(quest_menu, 123.5f, 78.25f, error), error.c_str());
        check(ready_calls == 5 && append_calls == 1 && release_calls == 1 &&
              released_x == 123.5f && released_y == 78.25f &&
              routed == std::vector<std::string>{"append", "release"},
              "symbol-keyed page readiness/append/release order or authored coordinates changed");

        std::cout << "source menu page PASS: exact menu symbol, same owner, readiness gate, atomic append, ordered rows and release route\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
