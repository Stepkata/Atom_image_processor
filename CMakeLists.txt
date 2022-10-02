cmake_minimum_required(VERSION 3.22)
project(ImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(ImageProcessor main.cpp image.cpp image.h stb_image.h stb_image_write.h stb_image_resize.h Processor.cpp Processor.h schrift.cpp schrift.h Font.cpp Font.h)
