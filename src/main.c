#include "core.h"
#include <flanterm.h>
#include <backends/fb.h>


void framebuffer_init() {
    struct limine_framebuffer *limframebuffer = framebuffer_request.response->framebuffers[0];
    
    struct flanterm_context *ft_ctx = flanterm_fb_init(
        NULL,
        NULL,
        limframebuffer->address, limframebuffer->width, limframebuffer->height, limframebuffer->pitch,
        limframebuffer->red_mask_size, limframebuffer->red_mask_shift,
        limframebuffer->green_mask_size, limframebuffer->green_mask_shift,
        limframebuffer->blue_mask_size, limframebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );
    const char msg[] = "Hello world\n";

    flanterm_write(ft_ctx, msg, sizeof(msg));
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}


void kmain(void) {
    // Bootloader base revision kontrolü
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Framebuffer kontrolü
    if (framebuffer_request.response == NULL 
        || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    framebuffer_init();

    

    // İşimiz bitti, bekle
    hcf();
}