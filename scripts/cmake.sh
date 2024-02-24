#!/bin/sh

# Set yours (or run from root folder)
ROOT_FOLDER="$PWD"

help() {
    echo "helper for building Sound Server. Usage: build [-c|configure] [-b|build]"
}

configure() {
    echo "Configuring..."
    cd "$ROOT_FOLDER/build" || mkdir build
    conan -v || "Error: Conan was not found."
    cd "$ROOT_FOLDER/build" && conan install "$ROOT_FOLDER" --output-folder="$ROOT_FOLDER/build" --build=missing
    cd "$ROOT_FOLDER/build" && cmake "$ROOT_FOLDER" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="$ROOT_FOLDER/build/conan_toolchain.cmake"
}

build() {
    echo "Building..."
    cd "$ROOT_FOLDER/build" || $(printf "Build floder does not exist, configuring..." && configure)
    cmake --build "$ROOT_FOLDER/build"
}
while getopts "hcb" arg; do 
    if [ "$arg" = "c" ]; then
        configure
    elif [ "$arg" = "b" ]; then
        build
    else 
        help
    fi
done
