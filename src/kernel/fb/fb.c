#include <fb/fb.h>
#include <stdarg.h>
#include <util.h>

// System framebuffer manager
// Copyright (C) 2024 Panagiotis

Framebuffer fb = {0};

void drawRect(int x, int y, int w, int h, int r, int g,
              int b) { // Draw a filled rectangle
  unsigned int offset =
      (x + y * fb.width) *
      4; // Finding the location of the pixel in the video array
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) { // color each line
      fb.virt[offset + j * 4] = b;
      fb.virt[offset + j * 4 + 1] = g;
      fb.virt[offset + j * 4 + 2] = r;
      fb.virt[offset + j * 4 + 3] = 0;
    }
    offset += fb.pitch; // switch to the beginnering of next line
  }
}


