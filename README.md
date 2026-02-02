编译方式
```
git clone http://github.com/tianocore/edk2.git
cd edk2
git checkout edk2-stable202511
git submodeule update --init
source edksetup.sh
make -C BaseTools
cd ..
make
```
编译结果位于Build文件夹内