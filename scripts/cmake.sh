#!/bin/sh

printf 'Run this script from project root!\n'

build() {
    printf 'Building...\n'
    cmake --build . -j8
}

configure() {
    printf 'Configuring for Debug...\n'
    mkdir -p build
    cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug
}

if [ $1 = "build" ]; then
    cd build 2>/dev/null || configure
    build
elif [ $1 = "configure" ]; then
    configure
fi

