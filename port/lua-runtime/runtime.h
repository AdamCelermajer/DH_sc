#ifndef DH2_LUA_RUNTIME_H
#define DH2_LUA_RUNTIME_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct dh2_lua dh2_lua;
/* Modern owned runtime, not the original engine's C++ object layout.
 * Single owner/thread, no reentry. Bytes are borrowed only during the call.
 * Diagnostics are truncated to the caller's capacity and always terminated.
 * No game engine callbacks are installed. */
dh2_lua *dh2_lua_create(size_t memory_limit);
void dh2_lua_destroy(dh2_lua *runtime);
/* Source-only compiler. Does not execute the chunk. */
int dh2_lua_compile(dh2_lua *runtime, const void *source, size_t bytes,
                    char *error, size_t capacity);
/* Controlled execution helper; instruction budget counts in 1000-op blocks.
 * Installs base/math/table/string, with filesystem loaders and print removed.
 * Returns zero on success; stack and hook are cleared after each call. */
int dh2_lua_execute(dh2_lua *runtime, const void *source, size_t bytes,
                    uint32_t instruction_blocks, char *error, size_t capacity);
size_t dh2_lua_memory_used(const dh2_lua *runtime);
#ifdef __cplusplus
}
#endif
#endif
