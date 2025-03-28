cmake_minimum_required(VERSION 3.30)
project(tguitar)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# requirements
find_package(RtAudio REQUIRED)
find_package(ftxui REQUIRED)
#find_package(FFTW REQUIRED)
#find_package(FFTW3 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
