#ifndef __MM_H__
#define __MM_H__
#include "linux/type.h"
#include "libs/string.h"

#define PAGE_SIZE   4096

int mm_init();
void *get_free_page();
void free_page(void *p);


void *kmalloc(size_t len);
static inline void *kzalloc(size_t len)
{
    void *p = kmalloc(len);
    if(p) {
        memset(p, 0, len);
    }
    return p;
}
void kfree_s(void *ptr, size_t len);

#endif /* __MM_H__ */
