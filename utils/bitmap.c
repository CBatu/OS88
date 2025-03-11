#include "font-12x24.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Font verilerini görsel olarak ekrana yazdıran fonksiyon
void print_character_bitmap(unsigned char* data, int lines) {
    printf("\nKarakter Görünümü:\n");
    printf("----------------\n");
    
    for (int i = 0; i < lines; i += 2) {
        // Her satır için 16-bit değeri oluştur
        uint16_t line = (data[i] << 8) | data[i + 1];
        
        // Her bit için karakter yazdır
        for (int bit = 15; bit >= 0; bit--) {
            printf("%c", (line & (1 << bit)) ? '#' : '.');
        }
        printf("\n");
    }
}

int main() {

    int char_size = 50; // 24 satır * 2 byte
    int total_chars = sizeof(console_font_12x24) / char_size;

    // Her karakteri işle
    for (int char_index = 0; char_index < total_chars; char_index++) {
        printf("\nKarakter %d:\n", char_index);
        
        // Hex değerlerini yazdır
        printf("Hex değerleri:\n");
        for (int i = 0; i < char_size; i += 2) {
            printf("0x%02X,0x%02X  ", 
                console_font_12x24[char_index * char_size + i],
                console_font_12x24[char_index * char_size + i + 1]);
            
            if ((i + 2) % 8 == 0) printf("\n");
        }
        
        // Bitmap'i görsel olarak yazdır
        print_character_bitmap(&console_font_12x24[char_index * char_size], 24);
    }

    return 0;
}