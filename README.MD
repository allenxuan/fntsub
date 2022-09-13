C++实现的对字体进行子集化的小工具

本项目只是对google的[sfntly](https://github.com/googlei18n/sfntly)项目C++版本进行了一些修改，在此表示对开源的感谢。

本项目实现的字体子集化工具生成的字体文件大小与sfntly Java版本的接近，解决C++版本提取出的文件过大问题。

`文章 https://www.veaxen.com/c%E7%89%88%E7%9A%84sfntly-%E5%AD%97%E4%BD%93%E5%AD%90%E9%9B%86%E5%8C%96%E5%B7%A5%E5%85%B7.html`


先编译fntsub
然后 cd cmake-build-debug/bin
然后 ./fntsub ../../data/HuaGuangGangTieZhiHei-KeBianTi-2.ttf ../../data/subset/HuaGuangGangTieZhiHei-KeBianTi-2 -s 你好
