// XP_API_*: the thin platform wrappers that the GLX and lobby code calls instead of
// libc. Most forward one call to the libc function of the same role. Each comment
// names the target. The XP_API_GET_TIME and XP_DEBUG_OUT stand-ins live in deps/.
//
// The pseudocode of several wrappers shows void or a truncated body because the call
// they end in is a tail call. Where the ARM listing passes the callee's r0 back, the
// return type here is the callee's type, and the ledger notes say so.
#pragma once

#include <cstddef>

void XP_DEBUG_INIT();       // 0x82a664, no-op
void XP_DEBUG_FINALIZE();   // 0x82a668, no-op

char* XP_API_STRSTR(char* haystack, char* needle);      // 0x82b334 -> strstr
char* XP_API_STRCAT(char* dest, char* src);             // 0x82b338 -> strcat
char* XP_API_STRNCPY(char* dest, char* src, std::size_t n);  // 0x82b33c -> strncpy
char* XP_API_STRCPY(char* dest, char* src);             // 0x82b340 -> strcpy
int XP_API_STRCASECMP(char* s1, char* s2);              // 0x82b344 -> strcasecmp
int XP_API_STRNCMP(char* s1, char* s2, std::size_t n);  // 0x82b348 -> strncmp
int XP_API_STRCMP(char* s1, char* s2);                  // 0x82b34c -> strcmp
void* XP_API_MEMSET(void* s, int c, std::size_t n);     // 0x82b364 -> memset
int XP_API_ATOI(char* text);                            // 0x82b320 -> atoi
double XP_API_ATOF(char* text);                         // 0x82b318 -> strtod(text, nullptr)
std::size_t XP_API_STRLEN(char* s);                     // 0x82afac -> strlen
void* XP_API_MEMCPY(void* dest, void* src, std::size_t n);  // 0x82b350 -> memcpy, returns dest
void* XP_API_MALLOC(std::size_t size);                  // 0x82b9f0 -> malloc
void XP_API_FREE(void* p);                              // 0x82b9e4: free unless null
// 0x82b9a0: a new copy of the string, made with operator new[] and terminated.
// Release it with delete[] (the original pairs it with the wrong free; see NOTES.md).
char* XP_API_STRNEW(char* text);
// 0x82b324: mode 1 searches from the start (strchr); any other mode from the end (strrchr).
char* XP_API_STRCHR(char* s, int c, int mode);
// 0x82b014: removes the leading spaces in place and returns s. Trailing spaces stay.
char* XP_API_STRTRIM(char* s);
// 0x82acdc _Z21XP_API_GetNationalityPc: no-op.
void XP_API_GetNationality(char* out);
