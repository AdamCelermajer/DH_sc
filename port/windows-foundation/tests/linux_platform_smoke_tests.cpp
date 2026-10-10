#include "../platform_win32.hpp"
#include "../renderer.hpp"

#include <SDL.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace dh::foundation;

static void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

int main() {
    try {
        Renderer renderer;
        bool selected = false;
        std::string error;
        check(!renderer.game_object_visual_quality(selected, error),
              "Uninitialized renderer admitted source quality");

        Window window;
        check(window.open("DH2 Linux platform smoke", 320, 240), window.error());
        check(window.width() == 320 && window.height() == 240 && !window.should_close(),
              "SDL window dimensions or initial state are incorrect");
        check(renderer.initialize(window.width(), window.height()),
              "OpenGL renderer initialization failed");
        check(renderer.game_object_visual_quality(selected, error) && selected,
              "Full visual quality unavailable on the current Linux context");

        bool other_thread_accepted = true;
        std::thread other_thread([&] {
            std::string thread_error;
            bool thread_selected = true;
            other_thread_accepted = renderer.game_object_visual_quality(thread_selected, thread_error);
        });
        other_thread.join();
        check(!other_thread_accepted, "Renderer accepted a non-owner Linux thread");

        renderer.setVisualAssetQuality(VisualAssetQuality::ReducedOptional);
        check(renderer.game_object_visual_quality(selected, error) && !selected,
              "Reduced optional visual quality was not observed");
        renderer.setVisualAssetQuality(static_cast<VisualAssetQuality>(255));
        selected = true;
        check(!renderer.game_object_visual_quality(selected, error) && selected,
              "Unknown visual quality was admitted or changed output");

        SDL_Event close_event{};
        close_event.type = SDL_WINDOWEVENT;
        close_event.window.windowID = SDL_GetWindowID(SDL_GL_GetCurrentWindow());
        close_event.window.event = SDL_WINDOWEVENT_CLOSE;
        check(SDL_PushEvent(&close_event) == 1, "Could not enqueue SDL window close event");
        window.poll();
        check(window.should_close(), "SDL window close event was not observed");

        std::cout << "PASS SDL2/OpenGL window, renderer context/thread policy, quality selection, close event\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
