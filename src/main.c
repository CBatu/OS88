#include "core.h"
#include "psf.h"

typedef struct framebuffer {
    uint32_t width;
    uint32_t height;
    uint32_t* buffer;
    uint16_t bpp;
    uint32_t pitch;  // Her satır için byte cinsinden uzunluk
} framebuffer_t;


static framebuffer_t framebuffer_ins = {0};

// Framebuffer sınırlarını kontrol eden yardımcı fonksiyon
static inline bool is_within_bounds(uint16_t x, uint16_t y) {
    return x < framebuffer_ins.width && y < framebuffer_ins.height;
}

void framebuffer_init() {
    struct limine_framebuffer *limframebuffer = framebuffer_request.response->framebuffers[0];
    
    framebuffer_ins.width = limframebuffer->width;
    framebuffer_ins.height = limframebuffer->height;
    framebuffer_ins.buffer = (uint32_t*)limframebuffer->address;
    framebuffer_ins.bpp = limframebuffer->bpp;
    framebuffer_ins.pitch = limframebuffer->pitch;
    
    // Ekranı temizle
    for (uint32_t i = 0; i < framebuffer_ins.width * framebuffer_ins.height; i++) {
        framebuffer_ins.buffer[i] = 0x000000;  // Siyah
    }
}

void draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    // Sınırları kontrol et
    if (!is_within_bounds(x, y)) {
        return;
    }

    uint32_t *framebuffer_ptr = framebuffer_ins.buffer;
    uint32_t offset = y * (framebuffer_ins.pitch / sizeof(uint32_t)) + x;

    uint8_t src_a = (color >> 24) & 0xFF;
    
    // Tam opak renk için hızlı yol
    if (src_a == 255) {
        framebuffer_ptr[offset] = color;
        return;
    }
    
    // Alpha blending için
    uint8_t src_r = (color >> 16) & 0xFF;
    uint8_t src_g = (color >> 8) & 0xFF;
    uint8_t src_b = color & 0xFF;

    uint32_t dest_color = framebuffer_ptr[offset];
    uint8_t dest_r = (dest_color >> 16) & 0xFF;
    uint8_t dest_g = (dest_color >> 8) & 0xFF;
    uint8_t dest_b = dest_color & 0xFF;

    uint8_t out_r = (src_r * src_a + dest_r * (255 - src_a)) / 255;
    uint8_t out_g = (src_g * src_a + dest_g * (255 - src_a)) / 255;
    uint8_t out_b = (src_b * src_a + dest_b * (255 - src_a)) / 255;

    framebuffer_ptr[offset] = (0xFF << 24) | (out_r << 16) | (out_g << 8) | out_b;
}

extern uint8_t psf_font_version;

void _fb_putchar(char symbol, size_t cx, size_t cy, uint32_t fg, uint32_t bg){
    uint8_t *framebuffer = (uint8_t *) framebuffer_ins.buffer;
    uint32_t pitch = framebuffer_ins.pitch;
    uint32_t width, height;
    width = _psf_get_width(psf_font_version);
    height = _psf_get_height(psf_font_version);

    //uint32_t charsize = default_font->height * ((default_font->width + 7)/8);
    //uint8_t *glyph = (uint8_t*)&_binary_fonts_default_psf_start +
    //    default_font->headersize + (symbol>0&&symbol<default_font->numglyph?symbol:0) * default_font->bytesperglyph;
    uint8_t *glyph = _psf_get_glyph(symbol, psf_font_version);
    //bytesperline is the number of bytes per each row of the glyph
    size_t bytesperline =  (width + 7)/8;
    size_t offset = (cy * height * pitch) +
        (cx * (width) * sizeof(uint32_t));
    // x,y = current coordinates on the glyph bitmap

    uint32_t x, y, line;
    //uint32_t mask;
    for(y=0; y<height; y++){
        line = offset;
        //mask = 1 << (width - 1);
        for(x=0; x<width; x++){
            //*((uint32_t*) (framebuffer + line)) = *((unsigned int*) glyph) & mask ? fg : bg;
            //We are plotting the pixel
            //0x80 = 0b10000000, it is shifted right at every iteration this for loop.
            //glyph[x/8] if widht > 8, x/8 = byte selector
            //(ie width = 16bits, when x < 8, x/8, so we read glyph[0]. if x>8 then x/8 = 1, and we read glyph[1]
            //if the bit at position x is 1 plot the foreground color if is 0 plot the background color
            *((uint32_t*) (framebuffer + line)) = glyph[x/8] & (0x80 >> (x & 7)) ? fg : bg;
            //mask >>= 1;
            line +=sizeof(framebuffer_ins.bpp / 8);
        }
        glyph += bytesperline;
        offset +=pitch;
    }
}

void draw_line(uint16_t x_start, uint16_t y_start, uint16_t length, uint32_t color) {
    // Sınırları kontrol et
    if (x_start + length > framebuffer_ins.width) {
        length = framebuffer_ins.width - x_start;
    }
    
    if (!is_within_bounds(x_start, y_start)) {
        return;
    }

    for (uint16_t x = x_start; x < x_start + length; x++) {
        draw_pixel(x, y_start, color);
    }
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

    // Test çizgisi çiz - Kırmızı renk (ARGB formatında)
    draw_line(50, 50, 200, 0xFFFF0000);
    draw_line(50, 110, 200, 0xFFFF0000);

    // İşimiz bitti, bekle
    hcf();
}