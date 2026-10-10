#include "frontend_cleanup_v1.hpp"
#include "rich_text.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    std::vector<int> released;
    bool text_alive = true;
    bool text_cleanup_saw_live_owner = false;
    try {
        struct TextOwner {
            dh::foundation::frontend::FrontendText text;
            bool& alive;
            explicit TextOwner(bool& flag) : alive(flag) {}
            ~TextOwner() { alive = false; }
        } text_owner(text_alive);
        dh::foundation::frontend::FrontendCleanupStackV1 cleanup;
        cleanup.own([&] { released.push_back(1); });
        cleanup.own([&] { released.push_back(2); });
        cleanup.own([&] {
            const void* text_address = &text_owner.text;
            text_cleanup_saw_live_owner = text_alive && text_address != nullptr;
        });
        throw std::runtime_error("presentation failure");
    } catch (const std::runtime_error&) {
    }
    if (released != std::vector<int>{2, 1} || !text_cleanup_saw_live_owner || text_alive) {
        std::cerr << "exception cleanup order or actual FrontendText lifetime was invalid\n";
        return 1;
    }
    std::cout << "PASS frontend cleanup exception path and FrontendText lifetime\n";
    return 0;
}
