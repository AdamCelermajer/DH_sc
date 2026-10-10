#pragma once
#include <string>

namespace model_renderer {
// GL-thread delivery for the reconstructed nativePause/appPause focus-loss
// save edge. The returned error is diagnostic and must not be mistaken for a
// durable-save receipt unless the shared Application queue flush completes.
bool source_application_pause_save_v1(std::string& error);
}
