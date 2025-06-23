#include <fb/vga.h>
#include <fb/fb.h>
#include <limine.h>
#include <print.h>
#include <bootloader.h>

static volatile struct limine_framebuffer_request limineFBreq = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

void initiateVGA() {
  struct limine_framebuffer *framebufferRes =
      limineFBreq.response->framebuffers[0];
  fb.virt = (uint8_t *)framebufferRes->address;
  fb.phys = (size_t)fb.virt - bootloader.hhdmOffset;
  fb.height = framebufferRes->height;
  fb.width = framebufferRes->width;
  fb.pitch = framebufferRes->pitch;
  fb.bpp = framebufferRes->bpp;

  fb.red_shift = framebufferRes->red_mask_shift;
  fb.red_size = framebufferRes->red_mask_size;

  fb.green_shift = framebufferRes->green_mask_shift;
  fb.green_size = framebufferRes->green_mask_size;

  fb.blue_shift = framebufferRes->blue_mask_shift;
  fb.blue_size = framebufferRes->blue_mask_size;

  //   memcpy(&framebufferLimine, framebufferRes, sizeof(struct
  //   limine_framebuffer));
  printf("[graphics] Resolution fixed: fb{%hhn} dim(xy){%ldx%ld} bpp{%ld}\n",
         fb.virt, fb.width, fb.height, fb.bpp);
}