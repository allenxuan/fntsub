# 此CMakeList只支持gcc和clang
#cmake_minimum_required(VERSION 2.6.2)
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

# 项目名
project(font_subsetter)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(TEST_FULL_EXECUTABLE  OFF) #是否编译二进制可执行文件，对应src/fntsub/main.cc中的main函数
set(TEST_FONT_SHRINKER_EXECUTABLE  ON) #是否编译用来测试font_shrinker动态库接口的二进制可执行文件，对应src/test/font_shrinker_test_main.cc中的main函数

# 默认构建Debug版本
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
	add_definitions("-DDEBUG -D_DEBUG -g -fno-inline -fno-omit-frame-pointer -fno-builtin")
endif(NOT CMAKE_BUILD_TYPE)

# 设置输出lib和bin的路径
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

# 设置include的目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
		    ${CMAKE_CURRENT_SOURCE_DIR}/src/sfntly
		    ${CMAKE_CURRENT_SOURCE_DIR}/src/subtly)

# 不使用异常
add_definitions(-DSFNTLY_NO_EXCEPTION)
# 设置编译器警告/报错级别
#add_definitions(-D__wur=__attribute__\(\(warn_unused_result\)\) -Wall -Werror -Wno-error=strict-overflow -fno-exceptions)
add_definitions(-D__wur=__attribute__\(\(warn_unused_result\)\) -Wall -Wno-error=strict-overflow -fno-exceptions)
# 启用C++11
add_definitions(-std=c++11)
# 启用CMap和Bitmap
add_definitions(-DSFNTLY_EXPERIMENTAL)

# 查找所有文件并添加
file(GLOB SFNTLY_CORE_FILES src/sfntly/*.h src/sfntly/*.cc)
file(GLOB SFNTLY_PORT_FILES src/sfntly/port/*.h src/sfntly/port/*.cc)
file(GLOB SFNTLY_DATA_FILES src/sfntly/data/*.h src/sfntly/data/*.cc)
file(GLOB SFNTLY_MATH_FILES src/sfntly/math/*.h src/sfntly/math/*.cc)
file(GLOB SFNTLY_TABLE_COMMON_FILES src/sfntly/table/*.h src/sfntly/table/*.cc)
file(GLOB SFNTLY_TABLE_BITMAP_FILES src/sfntly/table/bitmap/*.h src/sfntly/table/bitmap/*.cc)
file(GLOB SFNTLY_TABLE_CORE_FILES src/sfntly/table/core/*.h src/sfntly/table/core/*.cc)
file(GLOB SFNTLY_TABLE_TTF_FILES src/sfntly/table/truetype/*.h src/sfntly/table/truetype/*.cc)
source_group(core FILES ${SFNTLY_CORE_FILES})
source_group(ports FILES ${SFNTLY_PORT_FILES})
source_group(data FILES ${SFNTLY_DATA_FILES})
source_group(math FILES ${SFNTLY_MATH_FILES})
source_group(table FILES ${SFNTLY_TABLE_COMMON_FILES})
source_group(table\\bitmap FILES ${SFNTLY_TABLE_BITMAP_FILES})
source_group(table\\core FILES ${SFNTLY_TABLE_CORE_FILES})
source_group(table\\truetype FILES ${SFNTLY_TABLE_TTF_FILES})
add_library(sfntly STATIC
      	    ${SFNTLY_CORE_FILES}
      	    ${SFNTLY_PORT_FILES}
      	    ${SFNTLY_DATA_FILES}
      	    ${SFNTLY_MATH_FILES}
      	    ${SFNTLY_TABLE_COMMON_FILES}
      	    ${SFNTLY_TABLE_BITMAP_FILES}
      	    ${SFNTLY_TABLE_CORE_FILES}
      	    ${SFNTLY_TABLE_TTF_FILES})
if (${APPLE})
	message(STATUS "APPLE = ${APPLE}")
	message(AUTHOR_WARNING "Make sure you have brew-installed icu4c on your Mac!")
	set (ICU_ROOT /usr/local/opt/icu4c)
	find_package(ICU REQUIRED components uc)
	message(STATUS "ICU_FOUND  = ${ICU_FOUND}")
	message(STATUS "ICU_INCLUDE_DIRS = ${ICU_INCLUDE_DIRS}")
	message(STATUS "ICU_INCLUDE_DIRS = ${ICU_LIBRARIES}")
	target_include_directories(sfntly PUBLIC ${ICU_INCLUDE_DIRS})
	target_link_libraries(sfntly ${ICU_LIBRARIES})
endif ()

file(GLOB SUBTLY_FILES src/subtly/*.h src/subtly/*.cc)
add_library(subtly STATIC ${SUBTLY_FILES})

file(GLOB FNTSUB_FILES src/fntsub/*.h src/fntsub/*.cc)
file(GLOB FNTSUB_MAIN src/fntsub/main.cc)
list(REMOVE_ITEM FNTSUB_FILES ${FNTSUB_MAIN})

add_library(font_shrinker SHARED src/font_shrinker.cc)
target_include_directories(font_shrinker PUBLIC
		include)
if (${APPLE})
	target_link_libraries(font_shrinker subtly sfntly pthread)
else ()
	target_link_libraries(font_shrinker subtly sfntly icuuc pthread)
endif ()
if (TEST_FULL_EXECUTABLE)
	add_executable(fntsub ${FNTSUB_MAIN})
	target_link_libraries(fntsub font_shrinker)
	if(FNTSUB_FILES)
		add_library(fntsublib STATIC ${FNTSUB_FILES})
		target_link_libraries(fntsub fntsublib)
	endif(FNTSUB_FILES)
endif ()
if (TEST_FONT_SHRINKER_EXECUTABLE)
	add_executable(font_shrinker_test src/test/font_shrinker_test_main.cc)
	target_link_libraries(font_shrinker_test font_shrinker)
endif ()
