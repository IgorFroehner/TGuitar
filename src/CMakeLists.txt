
set(SOURCES main.cpp)

add_executable(tguitar ${SOURCES})

target_link_libraries(tguitar
    RtAudio::rtaudio
)

target_include_directories(tguitar
        PRIVATE
        ${RtAudio_INCLUDE_DIRS}
        ${ftxui_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)
