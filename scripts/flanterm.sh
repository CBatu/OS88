#!/usr/bin/env bash
set -x  # show cmds
set -e  # fail globally

# Ana dizini argüman olarak al
BUILD_DIR="$1"

# Eğer BUILD_DIR verilmediyse, hata mesajı ver
if [ -z "$BUILD_DIR" ]; then
    echo "Hata: Ana dizin argümanı verilmedi."
    exit 1
fi

# Ana build dizinine git
cd "${BUILD_DIR}"

TARGET_VERSION="1.1"

# flanterm'ı almak veya güncellemek için fonksiyon
fetchLimine() {
    if [ -d "flanterm" ]; then  # Eğer flanterm dizini varsa
        # Versiyon kontrolü
        cd flanterm
        LOCAL_VERSION=$(cat .flanterm.version || echo "")
        cd ..

        if [ "$LOCAL_VERSION" == "$TARGET_VERSION" ]; then
            echo "flanterm zaten doğru versiyonla mevcut ($LOCAL_VERSION). Derleme yapılmayacak."
            return 0  # Versiyonlar eşitse, derleme yapma
        else
            echo "flanterm versiyonu ($LOCAL_VERSION) güncel değil. Güncelleniyor..."
            rm -rf flanterm/
        fi
    fi

    # flanterm'ı klonla veya güncelle
    git clone https://github.com/mintsuki/flanterm.git --depth=1
    cd flanterm/
    echo "$TARGET_VERSION" > .flanterm.version  # Hedef versiyonu kaydet
    cd ..
}

# fetchLimine fonksiyonunu çalıştır
fetchLimine
