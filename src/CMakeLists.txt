
set(SOURCES
        main.cpp
        audio/AudioProcessor.cpp
        audio/AudioData.cpp
        ui/InitialMenu.cpp
        ui/MainInterface.cpp
)

add_executable(tguitar ${SOURCES})

target_link_libraries(tguitar
        RtAudio::rtaudio
        ftxui::screen
        ftxui::dom
        ftxui::component # Not needed for this example.
)

target_include_directories(tguitar
        PRIVATE
        ${RtAudio_INCLUDE_DIRS}
        ${ftxui_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)
