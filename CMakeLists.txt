cmake_minimum_required(VERSION 3.30)
project(tguitar)

set(CMAKE_CXX_STANDARD 17)

# requirements
find_package(RtAudio REQUIRED)
find_package(ftxui REQUIRED)


add_subdirectory(src)
