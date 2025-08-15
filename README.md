# Canalotos

## 参考资料
《Orange S:一个操作系统的实现》 于渊  
《一个64位操作系统的设计与实现》 田宇  
https://wiki.osdev.org

## 语言
ASM  
C

## 编译器
nasm 2.16.03  
编译参数：-f elf64 -w-zeroing  
gcc 15.2.1  
编译参数：-mcmodel=large -fno-builtin -m64 -c -I $(RootDir)/include -fno-stack-protector -std=c23 -O -march=core2  
ld 2.45.0  
链接参数：-b elf64-x86-64 -T kernel.lds -z muldefs
## 虚拟机
bochs 3.0  
编译选项：--enable-show-ips --enable-debugger --enable-debugger-gui --enable-x86-debugger --with-x11 --enable-debugger-gui --enable-iodebug  
--enable-a20-pin --enable-x86-64 --enable-smp --enable-long-phy-address --enable-large-ramfile --enable-fpu --enable-vmx=2 --enable-memtype --enable-avx --enable-evex --enable-amx --enable-usb --enable-usb-xhci --enable-e1000 --enable-cdrom --enable-es1370 --enable-busmouse --enable-configurable-msrs