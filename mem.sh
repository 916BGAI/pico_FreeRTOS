#!/bin/sh

FLASH_SIZE=2048
MEM_SIZE=264

arr=(`arm-none-eabi-size @project_name@.elf | sed -n '2p'`)
let flash=(${arr[0]}+${arr[1]})
let mem=(${arr[1]}+${arr[2]})
let flash_size=($FLASH_SIZE)*1024;
let mem_size=($MEM_SIZE)*1024;
flash_usage1=`echo "scale=2; a=$flash*100/$flash_size; if (length(a)==scale(a)) print 0;print a "|bc`
mem_usage1=`echo "scale=2; a=$mem*100/$mem_size; if (length(a)==scale(a)) print 0;print a "|bc`
echo ""
echo "-------------------------------------------------------------"
echo -e "\033[32mFlash: $flash / $flash_size bytes, $flash_usage1% Full (.text + .data)\033[0m"
echo -e "\033[32mSRAM:  $mem / $mem_size bytes, $mem_usage1% Full (.data + .bss)\033[0m"
echo "-------------------------------------------------------------"
echo ""