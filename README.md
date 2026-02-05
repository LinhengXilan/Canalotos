# 编译方式
如果未下载edk2，按照以下命令编译
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
如果已正确配置edk2，则直接通过`make`编译  

编译结果位于Build文件夹内