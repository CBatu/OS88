PROJECT_NAME := batos
BUILD_DIR := $(PWD)/build

SCRIPT := scripts/limine.sh

all: build_limine

.PHONY: build_limine

build_limine:
	@echo "[*] Creating build directory..."
	@mkdir -p $(BUILD_DIR) > /dev/null 2>&1 && echo "[✓] Build directory created." || { echo "[✗] Failed to create build directory."; exit 1; }

	@echo "[*] Making Limine script executable..."
	@chmod +x $(SCRIPT) > /dev/null 2>&1 && echo "[✓] Script made executable." || { echo "[✗] Failed to chmod script."; exit 1; }

	@echo "[*] Running Limine script..."
	@$(SCRIPT) $(BUILD_DIR) > /dev/null 2>&1 && echo "[✓] Limine setup complete." || { echo "[✗] Limine script failed."; exit 1; }

flanterm:
	@echo "[*] Launching Flanterm..."
	@bash scripts/flanterm.sh src/kernel > /dev/null 2>&1 && echo "[✓] Flanterm exited." || { echo "[✗] Flanterm failed."; exit 1; }

hdrs:
	@echo "[*] Copying headers..."
	@bash scripts/hdrs.sh $(BUILD_DIR) > /dev/null 2>&1 && echo "[✓] Headers copied." || { echo "[✗] Failed to copy headers."; exit 1; }

kernel: build_limine flanterm hdrs
	@echo "[*] Building kernel..."
	@make -s -C src BUILD_DIR=$(BUILD_DIR) PROJECT_NAME=$(PROJECT_NAME)

iso: kernel 
	@echo "[*] Creating ISO structure..."

	@mkdir -p $(BUILD_DIR)/iso/boot > /dev/null 2>&1 && echo "[✓] ISO boot directory created." || { echo "[✗] Failed to create ISO boot directory."; exit 1; }

	@cp $(BUILD_DIR)/bin/$(PROJECT_NAME).bin $(BUILD_DIR)/iso/boot > /dev/null 2>&1 && echo "[✓] Kernel binary copied." || { echo "[✗] Failed to copy kernel binary."; exit 1; }

	@mkdir -p $(BUILD_DIR)/iso/boot/limine > /dev/null 2>&1

	@cp limine.conf \
		$(BUILD_DIR)/limine/limine-bios.sys \
		$(BUILD_DIR)/limine/limine-bios-cd.bin \
		$(BUILD_DIR)/limine/limine-uefi-cd.bin \
		$(BUILD_DIR)/iso/boot/limine/ > /dev/null 2>&1 && echo "[✓] Limine files copied." || { echo "[✗] Failed to copy Limine files."; exit 1; }

	@mkdir -p $(BUILD_DIR)/iso/EFI/BOOT > /dev/null 2>&1

	@cp $(BUILD_DIR)/limine/BOOTX64.EFI $(BUILD_DIR)/iso/EFI/BOOT/ > /dev/null 2>&1 && echo "[✓] BOOTX64.EFI copied." || { echo "[✗] Failed to copy BOOTX64.EFI."; exit 1; }

	@cp $(BUILD_DIR)/limine/BOOTIA32.EFI $(BUILD_DIR)/iso/EFI/BOOT/ > /dev/null 2>&1 && echo "[✓] BOOTIA32.EFI copied." || { echo "[✗] Failed to copy BOOTIA32.EFI."; exit 1; }

	@echo "[*] Generating ISO with xorriso..."
	@xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(BUILD_DIR)/iso -o $(PROJECT_NAME).iso > /dev/null 2>&1 && echo "[✓] ISO image generated." || { echo "[✗] Failed to create ISO."; exit 1; }

	@$(BUILD_DIR)/limine/limine bios-install $(PROJECT_NAME).iso > /dev/null 2>&1 && echo "[✓] Limine BIOS install complete." || { echo "[✗] Limine BIOS install failed."; exit 1; }

test: iso
	@echo "[*] Booting in QEMU..."
	@qemu-system-x86_64 -serial stdio $(PROJECT_NAME).iso 
	@echo "[✓] QEMU started." || { echo "[✗] Failed to run QEMU."; exit 1; }

clean:
	@echo "[*] Cleaning build files..."
	@rm -rf $(BUILD_DIR) $(PROJECT_NAME).iso > /dev/null 2>&1 && echo "[✓] Cleanup complete." || { echo "[✗] Cleanup failed."; exit 1; }
