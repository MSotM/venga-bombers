#!/usr/bin/env bash

build-gtest() {
    mkdir $GTESTDIR
    cd $GTESTDIR
    wget https://github.com/google/googletest/archive/release-1.7.0.zip
    unzip release-1.7.0.zip
    cd googletest-release-1.7.0
    cmake -DBUILD_SHARED_LIBS=ON
    make
}

[ -d $GTEST_ROOT/include ] || build-gtest
