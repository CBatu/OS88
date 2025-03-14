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

override CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep 'clang' >/dev/null 2>&1; echo $$?)

# If the C compiler is Clang, set the target as needed.
ifeq ($(CC_IS_CLANG),1)
    override CC += \
        -target x86_64-unknown-none
endif


# Kullanılacak C ve Linker flag'leri
# Genel Derleyici ve Linker Flag'leri
CFLAGS := -m64 -g -O2 -pipe -Wall -Wextra -ffreestanding -fno-stack-protector -nostdinc -Iflanterm -I$(BUILD_DIR)/limine -fno-stack-check -fno-PIC -m64 -march=x86-64 -Iinclude -mcmodel=kernel

# CPPFLAGS - C preprocessor flags
CPPFLAGS := -I src -DLIMINE_API_REVISION=3 -MMD -MP

# Gelişmiş derleyici seçenekleri (override CFLAGS ile birleştirilir)
override CFLAGS += \
    -g \
    -Wall \
    -Wextra \
    -std=gnu99 \
    -nostdinc \
    -ffreestanding \
    -fno-omit-frame-pointer \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-PIC \
    -ffunction-sections \
    -fdata-sections \
    -fno-strict-aliasing \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel

# Internal C preprocessor flags that should not be changed by the user.
override CPPFLAGS := \
    -I c \
    -isystem freestnd-c-hdrs \
    $(CPPFLAGS) \
    -MMD \
    -MP

# Ekstra flag'ler (flanterm özel)
flanterm/backends/fb.c: override CPPFLAGS += \
    -DFLANTERM_FB_DISABLE_BUMP_ALLOC

# Linker flag'leri
LDFLAGS :=

# Internal linker flags that should not be changed by the user.
override LDFLAGS += \
    -Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -Wl,--gc-sections \
    -T linker.ld

# NASMFLAGS (assembly için)
NASMFLAGS := -Wall -f elf64

# C derleyicisi (CC) ve linker (LD) tanımlamaları
CC := clang   # Burada x86_64 için özel bir cross-compiler kullanıyoruz

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
$(OUTPUT): $(OBJ_FILES) 
	mkdir -p $(BUILD_DIR)/bin
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $@

# Temizleme
clean:
	rm -rf $(BUILD_DIR)
