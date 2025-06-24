PROJECT_NAME := batos

# Kaynak dizini (tüm C dosyalarının bulunduğu dizin)
SRC_DIR := .

# Build dizini
BUILD_DIR := ../build

# Kernel ve nesne dosyalarının çıkacağı dizin
OBJ_DIR := $(BUILD_DIR)/

# Çıktı dosyası
OUTPUT := $(BUILD_DIR)/bin/$(PROJECT_NAME).bin


# Kullanılacak C ve Linker flag'leri
# Genel Derleyici ve Linker Flag'leri
CFLAGS := -m64 -g -c -ffreestanding -Wall -Werror -fcommon -fPIE -mno-80387 \
    -mno-mmx \
    -mno-sse \
		-nostartfiles \
		-nostdlib \
    -mno-sse2 \
    -mno-red-zone -fno-stack-protector \
    -fno-stack-check \
    -fno-lto -I$(BUILD_DIR)/hdrs -nostdinc -I../include -Iflanterm 

# CPPFLAGS - C preprocessor flags
CPPFLAGS := -I src -DLIMINE_API_REVISION=3 -MMD -MP





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
LDFLAGS := -m elf_x86_64 \
		-nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T ../linker.ld


# Internal linker flags that should not be changed by the user.

# NASMFLAGS (assembly için)
NASMFLAGS := -f elf64

# C derleyicisi (CC) ve linker (LD) tanımlamaları
CC := x86_64-elf-gcc   # Burada x86_64 için özel bir cross-compiler kullanıyoruz

# C dosyalarını bul
SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.c")
ASM_FILES := $(shell find $(SRC_DIR) -type f -name "*.asm")

# Derlenen nesne dosyaları (c -> .o)
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
ASM_OBJ_FILES := $(ASM_FILES:$(SRC_DIR)/%.asm=$(OBJ_DIR)/%.o)

# Tüm hedefler
.PHONY: all clean

# Varsayılan hedef
all: $(OUTPUT)

$(OBJ_DIR)/kernel/mm/malloc.o: kernel/mm/malloc.c
	mkdir -p $(dir $@)
	@echo Compiling $<
	$(CC) $(CFLAGS) $(CPPFLAGS) -DHAVE_MMAP=0 -DLACKS_TIME_H=1 -DLACKS_SYS_PARAM_H=1 -LACKS_STRING_H=0 -Dmalloc_getpagesize=4096 -DNO_MALLOC_STATS=1 -DMORECORE_CONTIGUOUS=0 -DUSE_LOCKS=2 "-DMALLOC_FAILURE_ACTION=abort()" -c $< -o $@

$(OBJ_DIR)/kernel/print.o: kernel/print.c
	mkdir -p $(dir $@)
	@echo Compiling $<
	$(CC) $(CFLAGS) $(CPPFLAGS) -DPRINTF_INCLUDE_CONFIG_H=1 -c $< -o $@


# Derleme işlemi (C dosyalarını derle)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	# Hedef dizini oluştur
	mkdir -p $(dir $@)
	@echo Compiling $<
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@nasm $(NASMFLAGS) -o $@ $<

# Bağlama işlemi (linkleme)
$(OUTPUT): $(OBJ_FILES) $(ASM_OBJ_FILES)
	mkdir -p $(BUILD_DIR)/bin
	x86_64-elf-ld $(LDFLAGS) $(OBJ_FILES) $(ASM_OBJ_FILES) -o $@


# Temizleme
clean:
	@rm -rf $(BUILD_DIR) 