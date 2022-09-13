C++实现的对字体进行子集化的小工具

本项目只是对google的[sfntly](https://github.com/googlei18n/sfntly)项目C++版本进行了一些修改，在此表示对开源的感谢。

本项目实现的字体子集化工具生成的字体文件大小与sfntly Java版本的接近，解决C++版本提取出的文件过大问题。

`文章 https://www.veaxen.com/c%E7%89%88%E7%9A%84sfntly-%E5%AD%97%E4%BD%93%E5%AD%90%E9%9B%86%E5%8C%96%E5%B7%A5%E5%85%B7.html`

工程根目录Cmake中默认编译Demo: 
```cmake
set(TEST_FONT_SHRINKER_EXECUTABLE  ON) #是否编译用来测试font_shrinker动态库接口的二进制可执行文件，对应src/test/font_shrinker_test_main.cc中的main函数
```
编译成功后运行Demo，如
```bash
cd cmake-build-debug/bin #进入二进制可执行文件目录

#运行可执行文件: ./font_shrinker_test <输入ttf文件> <输出ttf文件> -s <保留的字符合集>
./font_shrinker_test ../../data/HuaGuangGangTieZhiHei-KeBianTi-2.ttf ../../data/shrinker.ttf -s 老六
```

工程根目录Cmake中把Demo编译关闭，则只保留font_shrinker动态库的编译
```cmake
set(TEST_FONT_SHRINKER_EXECUTABLE  OFF)
...
...
add_library(font_shrinker SHARED src/font_shrinker.cc)
```
font_shrinker动态库接口
```c++
//include/font_shrinker.h
namespace font_shrinker {
    int shrink(const char* font_path, const char* output_font_path, const char *utf8Bytes);
}
```

