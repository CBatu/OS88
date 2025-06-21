
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
    if [ -d "hdrs" ]; then  # Eğer flanterm dizini varsa
        # Versiyon kontrolü
        cd hdrs
        LOCAL_VERSION=$(cat .hdrs.version || echo "")
        cd ..

        if [ "$LOCAL_VERSION" == "$TARGET_VERSION" ]; then
            echo "hdrs zaten doğru versiyonla mevcut ($LOCAL_VERSION). Derleme yapılmayacak."
            return 0  # Versiyonlar eşitse, derleme yapma
        else
            echo "hdrs versiyonu ($LOCAL_VERSION) güncel değil. Güncelleniyor..."
            rm -rf hdrs/
        fi
    fi

    # flanterm'ı klonla veya güncelle
    git clone https://codeberg.org/osdev/freestnd-c-hdrs-0bsd.git --depth=1 hdrs
    cd hdrs/
    echo "$TARGET_VERSION" > .hdrs.version  # Hedef versiyonu kaydet
    cd ..
}

# fetchLimine fonksiyonunu çalıştır
fetchLimine
