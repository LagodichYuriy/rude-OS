macro align value { db value-1 - ($ + value-1) mod (value) dup 0 }

file  "./../bin/fboot.bin", 512         ; simple bootloader
file  "./../bin/fkernel.bin"            ; simple shell
