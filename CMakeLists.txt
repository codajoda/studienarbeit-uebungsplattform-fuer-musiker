cmake_minimum_required(VERSION 3.16)

project(Musica VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2)
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Charts)
find_package(Qt6 6.5 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 6.5 REQUIRED COMPONENTS Sql)

set(JUCE_DIR "${CMAKE_SOURCE_DIR}/JUCE")
add_subdirectory(${JUCE_DIR})

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appMusica
    main.cpp
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(appMusica
    URI Musica
    VERSION 1.0
    QML_FILES
        Main.qml

        SOURCES audiosource/DelayAudioSource.h
        SOURCES audiosource/DistortionAudioSource.h
        SOURCES audiosource/EffectAudioSource.h
        SOURCES audiosource/RawAudioSource.h
        SOURCES audiosource/ReverbAudioSource.h

        SOURCES controller/audiocontroller.h controller/audiocontroller.cpp
        SOURCES controller/dbcontroller.h controller/dbcontroller.cpp

        SOURCES repository/dbrepository.h repository/dbrepository.cpp
        SOURCES repository/settings.h repository/settings.cpp
        SOURCES repository/Project.h
        SOURCES repository/Recording.h
        SOURCES repository/Tag.h
        SOURCES repository/track.h

        SOURCES service/audioeffectservice.h service/audioeffectservice.cpp
        SOURCES service/audiomanagerservice.h service/audiomanagerservice.cpp
        SOURCES service/audiomonitoringservice.h service/audiomonitoringservice.cpp
        SOURCES service/audioplaybackservice.h service/audioplaybackservice.cpp
        SOURCES service/audiorecordservice.h service/audiorecordservice.cpp
        SOURCES service/projectservice.h service/projectservice.cpp
        SOURCES service/recordservice.h service/recordservice.cpp
        SOURCES service/tagservice.h service/tagservice.cpp
        SOURCES service/waveformservice.h service/waveformservice.cpp

        QML_FILES ui/component/AudioSettingsComponent.qml
        QML_FILES ui/component/CreateProjectComponent.qml
        QML_FILES ui/component/EffectsComponent.qml
        QML_FILES ui/component/ProjectComponent.qml
        QML_FILES ui/component/RecordIslandComponent.qml
        QML_FILES ui/component/TopBarComponent.qml
        QML_FILES ui/component/TrackComponent.qml
        QML_FILES ui/component/WaveFormComponent.qml

        QML_FILES ui/page/OverviewPage.qml
        QML_FILES ui/page/ProjectPage.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMusica PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMusica
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/macos/Info.plist
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appMusica PRIVATE
    ${CMAKE_SOURCE_DIR}/controller
    ${CMAKE_SOURCE_DIR}/service
    ${CMAKE_SOURCE_DIR}/repository
    ${CMAKE_SOURCE_DIR}/audiosource
)

target_link_libraries(appMusica
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Concurrent
    Qt6::QuickControls2
    Qt6::Charts
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_core
    juce::juce_cryptography
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_javascript
    juce::juce_midi_ci
    juce::juce_osc
    juce::juce_product_unlocking
)

include(GNUInstallDirs)
install(TARGETS appMusica
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
