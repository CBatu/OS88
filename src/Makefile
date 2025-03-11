# Projenin adı
PROJECT_NAME := batos

# Kaynak dizini (tüm C dosyalarının bulunduğu dizin)
SRC_DIR := .

# Build dizini
BUILD_DIR := ../build

# Kernel ve nesne dosyalarının çıkacağı dizin
OBJ_DIR := $(BUILD_DIR)/kernel

# Çıktı dosyası
OUTPUT := $(BUILD_DIR)/bin/$(PROJECT_NAME).bin

# Kullanılacak C ve Linker flag'leri
CFLAGS := -m64 -g -O2 -pipe -Wall -Wextra -ffreestanding -fno-stack-protector -I$(BUILD_DIR)/limine -fno-stack-check -fno-PIC -m64 -march=x86-64 -Iinclude -mcmodel=kernel
LDFLAGS := -m elf_x86_64 -nostdlib -static -T linker.ld

# CPPFLAGS - C preprocessor flags
CPPFLAGS := -I src -DLIMINE_API_REVISION=3 -MMD -MP

# NASMFLAGS (assembly için)
NASMFLAGS := -Wall -f elf64

# C derleyicisi (CC) ve linker (LD) tanımlamaları
CC := x86_64-elf-gcc   # Burada x86_64 için özel bir cross-compiler kullanıyoruz
LD := x86_64-elf-ld    # Linkleme işlemi için x86_64-elf-ld kullanıyoruz

# C dosyalarını bul
SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.c")

# Derlenen nesne dosyaları (c -> .o)
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Tüm hedefler
.PHONY: all clean

# Varsayılan hedef
all: $(OUTPUT)

# Derleme işlemi (C dosyalarını derle)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	# Hedef dizini oluştur
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Bağlama işlemi (linkleme)
$(OUTPUT): $(OBJ_FILES) font.o 
	mkdir -p $(BUILD_DIR)/bin
	$(LD) $(LDFLAGS) font.o $(OBJ_FILES) -o $@

font.o: font.psf 
	objcopy -O elf64-x86-64 -B i386 -I binary font.psf font.o

# Temizleme
clean:
	rm -rf $(BUILD_DIR)
