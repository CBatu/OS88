PROJECT_NAME := batos
BUILD_DIR := $(PWD)/build

SCRIPT := scripts/limine.sh

all: build_limine

.PHONY: build_limine
build_limine:
	@mkdir -p $(BUILD_DIR)
	@chmod +x $(SCRIPT)
	$(SCRIPT) $(BUILD_DIR)

kernel: build_limine
	make -C src BUILD_DIR=$(BUILD_DIR) PROJECT_NAME=$(PROJECT_NAME)


iso: kernel 
	mkdir -p $(BUILD_DIR)/iso
	mkdir -p $(BUILD_DIR)/iso/boot
	cp $(BUILD_DIR)/bin/$(PROJECT_NAME).bin $(BUILD_DIR)/iso/boot
	mkdir -p $(BUILD_DIR)/iso/boot/limine
	cp -v src/limine.conf $(BUILD_DIR)/limine/limine-bios.sys $(BUILD_DIR)/limine/limine-bios-cd.bin \
		$(BUILD_DIR)/limine/limine-uefi-cd.bin $(BUILD_DIR)/iso/boot/limine/
	mkdir -p $(BUILD_DIR)/iso/EFI/BOOT
	cp -v $(BUILD_DIR)/limine/BOOTX64.EFI $(BUILD_DIR)/iso/EFI/BOOT/
	cp -v $(BUILD_DIR)/limine/BOOTIA32.EFI $(BUILD_DIR)/iso/EFI/BOOT/
	cd $(BUILD_DIR)
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		build/iso -o $(PROJECT_NAME).iso
	$(BUILD_DIR)/limine/limine bios-install $(PROJECT_NAME).iso

test: iso
	qemu-system-x86_64 $(PROJECT_NAME).iso

clean:
	rm -rf $(BUILD_DIR)/bin $(BUILD_DIR)/kernel src/font.o $(PROJECT_NAME).iso