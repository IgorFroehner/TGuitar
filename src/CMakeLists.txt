
set(SOURCES
        main.cpp
        audio/AudioProcessor.cpp
        audio/AudioData.cpp
        audio/Globals.cpp
        audio/Effect.cpp
        audio/DistortionEffect.cpp
        audio/DelayEffect.cpp
        audio/Metronome.cpp
        ui/InitialMenu.cpp
        ui/TGuitarUI.cpp
)

add_executable(tguitar ${SOURCES})

target_link_libraries(tguitar
        RtAudio::rtaudio
        ftxui::screen
        ftxui::dom
        ftxui::component # Not needed for this example.
#        ${FFTW3_LIBRARIES}
)

target_include_directories(tguitar
        PRIVATE
        ${RtAudio_INCLUDE_DIRS}
        ${ftxui_INCLUDE_DIRS}
#        ${FFTW3_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
)
