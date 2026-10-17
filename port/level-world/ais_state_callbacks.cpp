#include "ais_state_callbacks.hpp"

namespace dh2::ais_state_callbacks {
Status invoke(State* state, Callback callback, const Services* services, Result* out) {
    if (!state || !state->ais || !out ||
        static_cast<std::uint32_t>(callback) > static_cast<std::uint32_t>(Callback::post))
        return Status::invalid_argument;
    const Table* table = state->active;
    if (!table) {
        *out = {};
        return Status::complete;
    }
    const char* name = nullptr;
    std::uint32_t offset = 0;
    switch (callback) {
        case Callback::update: name = table->update; offset = 0x14; break;
        case Callback::conditions: name = table->conditions; offset = 0x2c; break;
        case Callback::init: name = table->init; offset = 0x44; break;
        case Callback::post: name = table->post; offset = 0x5c; break;
    }
    *out = {0, offset, table->identity, name};
    if (!services || !services->call) return Status::service_unavailable;
    const Request request{state->ais, table->identity, name, callback, offset};
    out->called = 1;
    return services->call(services->context, state, &request) ?
        Status::service_failed : Status::complete;
}
}
