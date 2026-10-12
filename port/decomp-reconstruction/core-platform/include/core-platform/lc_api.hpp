// LC_API_*: the thin C wrappers of the LC network and file library that the
// original links against. Most forward directly to the C library.
#pragma once

#include <cstddef>

// Empty in the binary (the bodies are a bare return). Kept so the link names exist.
void FUN_008ac2f4();                   // 0x89c2f4
void LC_API_STRCHR();                  // 0x89c2f8
void LC_DEBUG_INIT();                  // 0x89c2fc
void LC_DEBUG_FINALIZE();              // 0x89c300
void LC_DEBUG_OUT();                   // 0x89c304
void LC_API_FTOA();                    // 0x89c310
void LC_API_GetNationality(char* out); // 0x89c99c

// 0x89c29c LC_API_STRCHRFIND: pointer just past the n-th occurrence of c in s, s
// itself when n is 0, or nullptr when there are fewer than n occurrences.
char* LC_API_STRCHRFIND(char* s, int c, int n);
// 0x89c2cc FUN_008ac2cc: the same search, reached through a register-passed
// entry. Partial: the entry state is assumed to match LC_API_STRCHRFIND.
char* FUN_008ac2cc(char* s, int c, int n);

// 0x89c314 LC_API_ITOA. Partial: the listing stores only the least significant
// digit (after a '-' for negatives) and tail-calls a divide; see NOTES.md.
void LC_API_ITOA(int value, char* out, int base);

// 0x89c88c LC_API_PARSE_DATA: copies the text after skipping `skip` separators;
// returns the offset of the first copied character, or -1.
int LC_API_PARSE_DATA(const char* in, char* out, int skip, char separator);
// 0x89c9a0 getValueLC: same copy as LC_API_PARSE_DATA. The listing returns 0 on
// every path, so the result is always 0.
int getValueLC(const char* in, char* out, int skip, char separator);

// 0x89ca14 _Z14LC_API_TOLOWERPKcPc
void LC_API_TOLOWER(const char* in, char* out);

// Debug logs: two fixed files, log_recv.txt and log_send.txt.
void LC_DEBUG_OPEN_RECVDATA_LOG();   // 0x89cb6c
void LC_DEBUG_CLOSE_RECVDATA_LOG();  // 0x89caf8
void LC_DEBUG_SAVE_RECVDATA_LOG(const char* data, int size);  // 0x89ca7c
void LC_DEBUG_OPEN_SENDDATA_LOG();   // 0x89cbac
void LC_DEBUG_CLOSE_SENDDATA_LOG();  // 0x89cb18
void LC_DEBUG_SAVE_SENDDATA_LOG(const char* data, int size);  // 0x89caa4

// File slots: a table of eight FILE pointers. Handles are 1-based.
int LC_API_FILE_OPEN(const char* name, const char* mode);  // 0x89cbec; 0 on failure
void LC_API_FILE_CLOSE(int handle);                         // 0x89cb38
void LC_API_FILE_WRITE(const void* data, std::size_t size, std::size_t count, int handle);  // 0x89cacc
void LC_API_FILE_READ(void* data, std::size_t size, std::size_t count, int handle);         // 0x89ce98
int LC_API_FILE_GET_LENGTH(int handle);                     // 0x89ce28

// Strings and memory: thin wrappers.
std::size_t LC_API_STRLEN(const char* s);                  // 0x89cc98
double LC_API_ATOF(const char* s);                         // 0x89cec4
int LC_API_ATOI(const char* s);                            // 0x89cecc
char* LC_API_STRSTR(const char* haystack, const char* needle);  // 0x89ced0
char* LC_API_STRCAT(char* dest, const char* src);          // 0x89ced4
char* LC_API_STRNCPY(char* dest, const char* src, std::size_t n);  // 0x89ced8
char* LC_API_STRCPY(char* dest, const char* src);          // 0x89cedc
int LC_API_STRCASECMP(const char* a, const char* b);       // 0x89cee0
int LC_API_STRNCMP(const char* a, const char* b, std::size_t n);  // 0x89cee4
int LC_API_STRCMP(const char* a, const char* b);           // 0x89cee8
void* LC_API_MEMCPY(void* dest, const void* src, std::size_t n);  // 0x89ceec
void* LC_API_MEMSET(void* dest, int value, std::size_t n);  // 0x89cf00
void LC_API_STRTRIM(char* s);                              // 0x89cd00
void* LC_API_MALLOC(std::size_t n);                        // 0x89d364
void LC_API_FREE(void* p);                                 // 0x89d360
int LC_API_GET_TIME();                                     // 0x89d3a8 (alias of OS_GetTime)

// 0x89cc4c LC_API_IP_INT2STR: dotted quad, low byte first.
char* LC_API_IP_INT2STR(unsigned int address, char* out);

// 0x89cc9c LC_API_GET_RANDOM. Partial: srand48/lrand48 are not available on the
// Windows target, so std::rand over a time seed stands in for them.
long LC_API_GET_RANDOM(const char* seed);
