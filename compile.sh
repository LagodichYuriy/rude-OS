#!/bin/sh

mkdir -p ./bin/
mkdir -p ./out/

fasm ./src/fboot.asm      ./bin/fboot.bin      # simple bootloader
fasm ./src/fkernel.asm    ./bin/fkernel.bin    # simple shell

fasm ./src/fmake.asm      ./out/os.bin         # combine binaries

qemu-system-i386          ./out/os.bin         # launch it
