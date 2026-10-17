#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
namespace dh2::data {
enum class SavedStateTableV1 {levels,world_map};
struct SavedStateMemoryV1 {
    void* context=nullptr;
    // Source CustomAlloc(size,0). Return aligned storage with a defined native
    // backing policy. The provider and its context outlive all owned arrays.
    void* (*allocate)(void*,std::size_t,int)=nullptr;
    void (*release)(void*,void*)=nullptr; // synchronous and nonthrowing
};
struct SavedStateArrayV1 {
    std::int32_t* words=nullptr;
    std::uint32_t count=0;
    SavedStateMemoryV1 memory{};
    SavedStateArrayV1()=default;
    SavedStateArrayV1(const SavedStateArrayV1&)=delete;
    SavedStateArrayV1& operator=(const SavedStateArrayV1&)=delete;
    ~SavedStateArrayV1();
};
struct SavedLevelStateServicesV1 {
    void* context=nullptr;
    // Each invocation rereads the current actual table owner. Providers may
    // update their tables synchronously, but must not rebind the Save arrays.
    bool (*count)(void*,SavedStateTableV1,std::uint32_t*,std::string&)=nullptr;
    bool (*default_word)(void*,SavedStateTableV1,std::uint32_t,std::int32_t*,std::string&)=nullptr;
    SavedStateMemoryV1 memory{};
};
} // namespace dh2::data
