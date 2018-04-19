#!/usr/bin/env bash

## Compile manual
#g++ src/rafaelcardoso_tap03.cpp -o src/rafaelcardoso_tap03.exe -std=c++11
#./src/rafaelcardoso_tap03.exe < test/in.txt

## Compile CLion
#./cmake-build-debug/tap03 < test/in.txt
#cp ./src/rafaelcardoso_tap03.exe ./grammar/a.out

# test 40
#./cmake-build-debug/tap03 < ./grammar/in/file40

# test 98
#./cmake-build-debug/tap03 < ./grammar/in/file98

## Run GRADE
cp ./cmake-build-debug/tap03 ./grammar/a.out
cd ./grammar
./grade.bash
