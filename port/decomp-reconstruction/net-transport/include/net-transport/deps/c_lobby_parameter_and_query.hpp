// Stand-in for CLobbyParameterAndQuery (owned by another category). The connection code
// only calls the packing slot (vtable +0x10).
#pragma once

#include <cstddef>

class CLobbyParameterAndQuery {
public:
    virtual ~CLobbyParameterAndQuery() = default;

    // Vtable +0x10: returns a malloc'ed buffer of *length bytes (freed with LC_API_FREE_0),
    // or nullptr when there is nothing to send. Stand-in: nothing to send.
    virtual void* PackBuffer(int* length) {
        *length = 0;
        return nullptr;
    }
};
