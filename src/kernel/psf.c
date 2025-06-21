#include <fb/fb.h>
#include <psf.h>
#include <print.h>

// ONLY include this here!
#include <gohufont.h>

PSF1Header *psf;

bool psfLoad(void *buffer) {
  PSF1Header *header = (PSF1Header *)buffer;

  if (header->magic != PSF1_MAGIC) {
    printf("[console] Invalid PSF magic! Only PSF1 is supported{0x0436} "
           "supplied{%04X}\n",
           header->magic);
    return false;
  }

  if (!(header->mode & PSF1_MODE512) && !(header->mode & PSF1_MODEHASTAB)) {
    printf("[console] Invalid PSF mode! No unicode table found... mode{%02X}\n",
           header->mode);
    return false;
  }

  psf = buffer;

  printf("[console] Initiated with font: dim(xy){%dx%d}\n", 8, psf->height);

  return true;
}

bool psfLoadDefaults() { return psfLoad(&u_vga16_psf[0]); }

void psfPutC(char c, uint32_t x, uint32_t y, uint32_t r, uint32_t g,
             uint32_t b) {
  uint8_t *targ =
      (uint8_t *)((size_t)psf + sizeof(PSF1Header) + c * psf->height);
  for (int i = 0; i < psf->height; i++) {
    for (int j = 0; j < 8; j++) {
      if (targ[i] & (1 << (8 - j))) // NOT little endian
        drawPixel(x + j, y + i, r, g, b);
    }
  }
}