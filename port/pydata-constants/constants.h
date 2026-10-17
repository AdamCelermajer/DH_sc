#ifndef DH2_PYDATA_CONSTANTS_H
#define DH2_PYDATA_CONSTANTS_H
#include <stdint.h>
/* Borrowed immutable bytes; callers retain the buffer until the view is unused. */
struct dh2_pycst_view { const unsigned char *bytes; uint32_t size,groups,entries; };
struct dh2_pycst_entry { uint32_t group_offset,group_length,name_offset,name_length; int32_t value; };
struct dh2_pycst_result { uint32_t found; int32_t value; };
/* 0 succeeds, 1 invalid arguments/aliasing, 2 malformed/unsupported integer file.
 * Rejection preserves output. Names are at most 255 bytes and contain no NUL.
 * File limit: 16 MiB. Complete structural consumption is required. */
uint32_t dh2_pycst_open(struct dh2_pycst_view *,const void *,uint32_t);
uint32_t dh2_pycst_entry_at(const struct dh2_pycst_view *,uint32_t,struct dh2_pycst_entry *);
uint32_t dh2_pycst_copy_entries(const struct dh2_pycst_view *,struct dh2_pycst_entry *,uint32_t capacity);
uint32_t dh2_pycst_get(const struct dh2_pycst_view *,const void *,uint32_t,
                     const void *,uint32_t,struct dh2_pycst_result *);
#endif
