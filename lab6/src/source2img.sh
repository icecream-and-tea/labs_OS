#!/bin/sh

TEST_DIRS=$1

if [ -z ${TEST_DIRS} ]; then
    TEST_DIRS = ls tests
fi

echo ${TEST_DIRS} "===="

echo "=====================make allclean========================"
rm -rf output

for test in ${TEST_DIRS}
do
    echo "=============编译链接${test}并制作二进制映像============="
    make clean

    #prepare test sources
    rm -rf userApp
    mkdir -p userApp
    cp tests/${test}/* userApp/  #copy userApp

    echo "*************make" ${test} "*************"
    make

        if [$? -ne 0]; then
            echo "make failed"
        else
            echo "make succeed"
            qemu-system-i386 -kernel output/myOS.elf -serial pty &
        fi
done

