#!/usr/bin/env bash
set -x # show cmds
set -e # fail globally

# Ana dizini argüman olarak al
BUILD_DIR="$1"

# Eğer BUILD_DIR verilmediyse, hata mesajı ver
if [ -z "$BUILD_DIR" ]; then
    echo "Hata: Ana dizin argümanı verilmedi."
    exit 1
fi

# Ana build dizinine git
cd "${BUILD_DIR}"

TARGET_VERSION="8.0.5"

fetchLimine() {
    rm -rf limine/
    git clone https://github.com/limine-bootloader/limine.git --branch=v9.x-binary --depth=1
    cd limine/
    make
    cd ../
    echo "$TARGET_VERSION" >.version
}

# Versiyon kontrolü ve Limine'nin doğru şekilde build edilip edilmediğini kontrol et
if [[ ! -f ".version" ]] || ! cat .version | grep -i "$TARGET_VERSION" || [[ ! -f "limine/limine" ]]; then
    fetchLimine
fi
