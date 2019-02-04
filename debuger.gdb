
#startQemu kernel.bin.gdb
#sleepgdb

file kernel.bin.gdb
shell sleep 0.1

connectToKernel

break main

