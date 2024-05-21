#!/bin/sh

CUR_PATH=`pwd`
INSTALL_PATH=${INSTALL_ROOT}/$1

#zlib
build_zlib()
{
    cd ${CUR_PATH}
    tar -xvzf zlib-1.3.1.tar.gz
    cd ./zlib-1.3.1
    ./configure --prefix=${INSTALL_PATH}
    sed -i 's/LDSHARED=gcc/LDSHARED=arm-linux-gnueabihf-gcc/g' Makefile
    make CC=${HOST}-gcc AR=${HOST}-ar
    make install
}

#curl
build_curl()
{
    cd ${CUR_PATH}
    tar -xvzf curl-8.7.1.tar.gz
    cd curl-8.7.1
    ./configure --prefix=${INSTALL_PATH} --host=${HOST} CC=${HOST}-gcc --without-ssl --with-zlib=${INSTALL_PATH}
    make
    make install
}

echo "building curl..."
build_zlib
build_curl
echo "building curl end..."
