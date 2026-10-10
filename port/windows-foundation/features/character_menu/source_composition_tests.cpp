#include "source_composition.hpp"

#include <iostream>
#include <stdexcept>

using namespace dh::foundation::character_menu;
static void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

int main() {
    try {
        std::string error;
        auto owner = std::static_pointer_cast<void>(std::make_shared<int>(7));
        SourceCompositionV1 composition(owner);
        bool ready = false;
        unsigned render_calls = 0, root_calls = 0, release_calls = 0;
        SourcePageProviderV1 equipment;
        equipment.owner = owner;
        equipment.ready = [&](std::string& e) { e = ready ? "" : "actual ItemName provider unavailable"; return ready; };
        equipment.append = [&](Frame& frame, std::string&) {
            ++render_calls;
            MenuTextField field; field.path = "equipment-page-provider";
            frame.text.push_back({field, "from source page"});
            return true;
        };
        equipment.release = [&](float, float, std::string&) { ++release_calls; return true; };
        check(composition.register_page(Tab::equipment, equipment, error), error.c_str());
        auto wrong_owner = std::static_pointer_cast<void>(std::make_shared<int>(8));
        auto rejected = equipment; rejected.owner = wrong_owner;
        check(!composition.register_page(Tab::skills, rejected, error) &&
              error.find("same selected-character owner") != std::string::npos,
              "page provider detached from canonical source owner was accepted");

        Presenter presenter; presenter.open();
        check(!composition.select(presenter, Tab::equipment, error) &&
              presenter.tab() == Tab::stats && render_calls == 0,
              "unready page changed the currently valid tab");
        ready = true;
        check(composition.select(presenter, Tab::equipment, error) &&
              presenter.tab() == Tab::equipment,
              "ready page was not selected after its readiness check");
        check(!composition.select(presenter, Tab::faery, error) &&
              presenter.tab() == Tab::equipment,
              "unbound Faery page displaced the selected Equipment page");

        // Instantiate the generic release path as well as page selection.
        // This consumer deliberately never registers a NativeStats action;
        // it must still link and route an Equipment click using only the
        // generic provider contract.
        (void)composition.release(presenter, 0.f, 0.f, 480, 320, error);
        check(release_calls == 1,
              "generic Equipment release did not reach its same-owner provider");

        Bindings bindings;
        bindings.content = [&](Tab tab, Frame& frame, std::string&) {
            ++root_calls;
            if (tab == Tab::equipment) {
                MenuTextField field; field.path = "root-owned-content";
                frame.text.push_back({field, "root"});
            }
            return true;
        };
        check(composition.install_content(bindings, error), error.c_str());
        Bindings duplicate_bindings;
        check(!composition.install_content(duplicate_bindings, error),
              "content callback installed twice and could double-append provider art");
        Frame frame;
        check(bindings.content(Tab::equipment, frame, error) && root_calls == 1 &&
              render_calls == 1 && frame.text.size() == 2 &&
              frame.text[1].field.path == "equipment-page-provider",
              "page provider did not append after existing root content");
        check(bindings.content(Tab::stats, frame, error) && root_calls == 2 && render_calls == 1,
              "equipment provider rendered on a different tab");

        std::cout << "source composition PASS: same-owner providers, preselection readiness, chained content\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
