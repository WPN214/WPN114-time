cmake_minimum_required(VERSION 3.1)
project(wpn114time-qml LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_LINK_FLAGS)

find_package(Qt5 REQUIRED COMPONENTS Quick Core Qml)

set(WPN114TIME_QML_SOURCES
    qml_plugin.cpp
    source/time.hpp
    source/time.cpp
    )

if (UNIX AND NOT APPLE)
    set(WPN114_ROOT /usr/lib/qt/qml/WPN114)
    set(WPN114AUDIO_ROOT ${WPN114_ROOT}/Audio)
    set(WPN114AUDIO_INCLUDE ${WPN114AUDIO_ROOT}/include)
endif()

add_subdirectory(examples)
add_library(${PROJECT_NAME} SHARED ${WPN114TIME_QML_SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick Qt5::Qml -L${WPN114AUDIO_ROOT} -lwpn114audio-qml)
target_include_directories(${PROJECT_NAME} PUBLIC ${WPN114AUDIO_INCLUDE})

install(TARGETS ${PROJECT_NAME} DESTINATION ${WPN114AUDIO_ROOT}/Time)
install(FILES qml/time.qmltypes qml/qmldir DESTINATION ${WPN114AUDIO_ROOT}/Time)
