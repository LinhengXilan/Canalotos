# Canalotos

## 参考资料
《Orange S:一个操作系统的实现》 于渊  
《一个64位操作系统的设计与实现》 田宇  
https://wiki.osdev.org

## 语言
ASM C

## 编译器
nasm -f elf64 -w-zeroing  
gcc -mcmodel=large -fno-builtin -m64 -c -I $(RootDir)/include -fno-stack-protector -std=c23 -O -march=core2
ld -b elf64-x86-64 -T kernel.lds -z muldefs
## 虚拟机
bochs  
configure: --enable-show-ips --enable-debugger --enable-debugger-gui --enable-x86-debugger --with-x11 --enable-debugger-gui --enable-iodebug
--enable-a20-pin --enable-x86-64 --enable-smp --enable-long-phy-address --enable-large-ramfile --enable-fpu --enable-vmx=2 --enable-memtype --enable-avx --enable-evex --enable-amx --enable-usb --enable-usb-xhci --enable-e1000 --enable-cdrom --enable-es1370 --enable-busmouse --enable-configurable-msrs