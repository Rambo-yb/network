#!/bin/sh

BASE_PATH=`pwd`

export INSTALL_ROOT="${BASE_PATH}/_install"
export HOST="arm-linux-gnueabihf"

#third_lib build
build_third_lib()
{
    cd $BASE_PATH
    if [ ! -d $INSTALL_ROOT ] || [ ! -d ${INSTALL_ROOT}/$1 ];
    then
        cd $BASE_PATH/third_lib/$1
        ./build.sh $1
    fi 
}

build_main()
{
    cd $BASE_PATH
    if [ -d $BASE_PATH/build ];
    then
        rm -r $BASE_PATH/build
    fi 

    mkdir build
    cd $BASE_PATH/build
    cmake .. -DCMAKE_C_COMPILER=${HOST}-gcc -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}/network
    make
    make install
}

build_third_lib curl
build_main

cd $BASE_PATH/_install/curl
cp -raf ./lib/*.a $INSTALL_ROOT/network/lib/
cp -raf ./lib/*.so* $INSTALL_ROOT/network/lib/

