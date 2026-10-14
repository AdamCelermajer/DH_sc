// GCSingleton<T>: the original's singleton base for the GameCenter class. The
// instance is taken from CustomAlloc and constructed in place.
#pragma once

#include "core-platform/memory.hpp"

#include <cstdio>
#include <cstdlib>
#include <new>

template <class T>
class GCSingleton {
public:
    virtual ~GCSingleton() {}  // 0x31f78c: the original's destructor body is empty

    // 0x32bd08 GCSingleton<GameCenter>::GetInstance(). Partial: the original calls the
    // constructor even when the allocation fails (on a null pointer); here it is
    // skipped. The block is sizeof(T), not the original's 0x7c.
    static T* GetInstance();

public:  // decision 9: Application::Quit deletes and clears the instance directly
    static T* m_instance;
};

template <class T>
T* GCSingleton<T>::m_instance = nullptr;

template <class T>
T* GCSingleton<T>::GetInstance() {
    if (m_instance == nullptr) {
        void* block = CustomAlloc(sizeof(T));
        if (block != nullptr) {
            m_instance = new (block) T();
        }
        if (m_instance == nullptr) {
            if (gAssertLevel == 2) {
                // The original writes to address 0 here to force a crash.
                std::abort();
            } else if (gAssertLevel == 1) {
                std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "m_instance != 0",
                             "..\\..\\project_vs2005\\Game/..\\..\\sources/Game/Multiplayer/GameCenter.h", 0x1b);
            }
        }
    }
    return m_instance;
}
