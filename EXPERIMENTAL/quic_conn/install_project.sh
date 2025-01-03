#!/bin/bash

# Atualiza o sistema e instala as dependências
echo "Atualizando o sistema e instalando dependências..."
sudo apt-get update
# Para compilar o ngtcp2 corretamente a biclioteca libdev eh necessaria. Mas foi colocada o libdev e o libev-dev como redundancia pra nn dar errado
# TODO: consertar a instalacao da biblioteca correta para a compilacao(LIBEV ou LIBDEV)
sudo apt-get install -y git g++ clang-15 make autoconf automake autotools-dev libtool pkg-config libev-dev libjemalloc-dev libevdev-dev

# Instala o wolfSSL
echo "Baixando e instalando wolfSSL..."
git clone --depth 1 -b v5.7.2-stable https://github.com/wolfSSL/wolfssl
cd wolfssl || exit
autoreconf -i
./configure --disable-dependency-tracking --enable-static --enable-all --enable-aesni --enable-harden --enable-keylog-export --disable-ech
make -j$(nproc)
sudo make install-strip
cd .. || exit
rm -rf wolfssl

# Instala o nghttp3
echo "Baixando e instalando nghttp3..."
git clone --recursive --shallow-submodules --depth 1 https://github.com/ngtcp2/nghttp3
cd nghttp3 || exit
autoreconf -i
./configure --disable-dependency-tracking --enable-lib-only CC=clang-15 CXX=clang++-15
make -j$(nproc)
sudo make install-strip
cd .. || exit
rm -rf nghttp3

# Instala o ngtcp2
echo "Baixando e instalando ngtcp2..."
git clone --recursive --shallow-submodules --depth 1 -b main https://github.com/ngtcp2/ngtcp2
cd ngtcp2 || exit
autoreconf -i
./configure \
    CC=clang-15 \
    CXX=clang++-15 \
    LDFLAGS="-static-libgcc -static-libstdc++" \
    LIBTOOL_LDFLAGS="-static-libtool-libs" \
    LIBEV_LIBS="-lev" \
    JEMALLOC_LIBS="-ljemalloc -lm" \
    --disable-dependency-tracking \
    --with-wolfssl
make -j$(nproc)
sudo make install-strip
cd .. || exit
rm -rf ngtcp2

echo "Instalação concluída! Verifique os binários em /usr/local/bin."
